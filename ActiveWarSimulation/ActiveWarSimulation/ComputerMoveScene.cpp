#include"input.h"
#include"DxLib.h"
#include"ComputerMoveScene.h"

//---------------ComputerMoveScene------------------
const size_t ComputerMoveScene::squareSize=(size_t)Unit::unitCircleSize;

ComputerMoveScene::ComputerMoveScene(std::shared_ptr<BattleSceneData> battleSceneData)
	:MoveScene(battleSceneData)
	,m_xLatticeNum((size_t)(battleSceneData->m_stageSize.x)/squareSize+1)
	,m_yLatticeNum((size_t)(battleSceneData->m_stageSize.y)/squareSize+1)
{
	//�i�q�_�̐������z����m��
	m_latticeInShape=std::vector<int>(m_xLatticeNum*m_yLatticeNum,0);
	//���삷�郆�j�b�g���ʂ�Ȃ��ꏊ��1���i�[���Ă���
	//�u��Q���̒������ɂ��Ȃ��ƃ��j�b�g�͒ʂ�Ȃ��v�Ƃ����l������p����
	for(const BattleObject *object:m_battleSceneData->m_field){
		
		if(object==m_battleSceneData->m_operateUnit){
			//���샆�j�b�g�͒ʂ�邩�ۂ��ɉe����^���Ȃ�

		} else if(object->JudgePointInsideShape(m_battleSceneData->m_operateUnit->getPos())){
			//object�����Ƀ��j�b�g������Ȃ�A���̊O����ʂ�Ȃ��悤�ɂ���
			std::vector<int> info(m_latticeInShape.size(),1);//�ʂ�邩�̏�����U�i�[����B�ʂ��Ƃ���݂̂�0�Ƃ���B
			object->GetHitJudgeShape()->RecordLatticePointInShape(info,m_xLatticeNum,m_yLatticeNum,squareSize,squareSize,0);
			for(size_t i=0,size=m_latticeInShape.size();i<size;i++){
				if(info[i]==1){
					//�ʂ�Ȃ��ƕ��������ꏊ��ʂ�Ȃ�������
					m_latticeInShape[i]=1;
				}
			}
		} else{
			//object�O���Ƀ��j�b�g������Ȃ�A���̓����͒ʂ�Ȃ��悤�ɂ���
			object->GetHitJudgeShape()->RecordLatticePointInShape(m_latticeInShape,m_xLatticeNum,m_yLatticeNum,squareSize,squareSize,1);
		}
	}
	//1�ׂ̗�2��u��
	auto Input2Only0=[](int &a){if(a==0){ a=2; }};
	for(size_t y=0;y<m_yLatticeNum;y++){
		for(size_t x=0;x<m_xLatticeNum;x++){
			const size_t index=x+y*m_xLatticeNum;
			if(m_latticeInShape[index]==1){
				//�S�Ă�1�ł���i�q�_�ɗאڂ���i�q�_��2�ɂ���B
				if(y>0){
					//��̗אړ_
					Input2Only0(m_latticeInShape[index-m_xLatticeNum]);
				}
				if(x>0){
					//���̗אړ_
					Input2Only0(m_latticeInShape[index-1]);
				}
				if(x+1<m_xLatticeNum){
					//��̗אړ_
					Input2Only0(m_latticeInShape[index+1]);
				}
				if(y+1<m_yLatticeNum){
					//��̗אړ_
					Input2Only0(m_latticeInShape[index+m_xLatticeNum]);
				}
			}
		}
	}
}

Vector2D ComputerMoveScene::CalculateInputVec()const{
	Vector2D moveVec;
	//�R���s���[�^���쎞�AAI�����������߂�
	//�^�[���J�n����1�b�o������ЂƂ܂��ŋߖT���j�b�g�ɒP���ɋ߂Â�
	if(m_battleSceneData->m_fpsMesuring.GetProcessedTime()>1.0){
		const Unit *nearestUnit=nullptr;
		for(const Unit *pu:m_battleSceneData->m_unitList){
			if(pu->GetBattleStatus().team!=m_battleSceneData->m_operateUnit->GetBattleStatus().team){
				if(nearestUnit==nullptr){
					nearestUnit=pu;
				} else if((pu->getPos()-m_battleSceneData->m_operateUnit->getPos()).sqSize()<(nearestUnit->getPos()-m_battleSceneData->m_operateUnit->getPos()).sqSize()){
					nearestUnit=pu;
				}
			}
		}
		if(nearestUnit!=nullptr){
			moveVec=nearestUnit->getPos()-m_battleSceneData->m_operateUnit->getPos();
		}
	}
	return moveVec;
}

int ComputerMoveScene::thisCalculate(){
	//m_operateUnit�̈ʒu�X�V
	const Vector2D beforeVec=m_battleSceneData->m_operateUnit->getPos();
	PositionUpdate(CalculateInputVec());
	const float moveSqLength=(beforeVec-m_battleSceneData->m_operateUnit->getPos()).sqSize();
	const double processedTime=m_battleSceneData->m_fpsMesuring.GetProcessedTime();
	if(m_battleSceneData->m_fpsMesuring.GetProcessedTime()>1.0){
		//1�b�o������s������
		if(JudgeAttackCommandUsable()){
			//�U���Ώۂ����݂��AOP������Ă���ꍇ�̂ݍU���������s��
			//FinishUnitOperation();//�s���I������(���Ƃ�)
			return SceneKind::e_attackNormal;//�U����ʂ�
		} else if(m_battleSceneData->m_operateUnit->GetBattleStatus().OP<2.0f || processedTime>10.0 || (moveSqLength<0.1f && processedTime>2.0)){
			//�ړ��ł��Ȃ��Ȃ�����A�܂���10�b�o������A�܂��ړ����������Ȃ��ꍇ�͑ҋ@
			FinishUnitOperation();
			return 0;
		}
	}
	return SceneKind::e_move;
}

void ComputerMoveScene::thisDraw()const{
	//�v���C���[�̂Ɠ����`�揈��
	MoveScene::thisDraw();

	//�f�o�b�O�p�`��
	//�i�q�_��`��
	for(size_t i=0,size=m_latticeInShape.size();i<size;i++){
		const int x=(i%m_xLatticeNum)*squareSize,y=(i/m_xLatticeNum)*squareSize;
		unsigned int color;
		switch(m_latticeInShape[i]){
		case(0):
			color=GetColor(255,255,0);
			break;
		case(1):
			color=GetColor(0,255,255);
			break;
		case(2):
			color=GetColor(255,0,255);
			break;
		}
		DrawCircleAA((float)(x),(float)(y),2,6,color,TRUE);
	}
}
