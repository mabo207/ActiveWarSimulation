#include"input.h"
#include"DxLib.h"
#include"ComputerMoveScene.h"

//---------------ComputerMoveScene::LatticeDistanceInfo------------------
bool ComputerMoveScene::LatticeDistanceInfo::operator<(const LatticeDistanceInfo &otherobj)const{
	//�]���̗D�揇�ʂ́u�����v���u�C���f�b�N�X�v�̏��B�߂��Ƃ��납�珈�����Ă��������̂�
	if(this->dist<otherobj.dist){
		return true;
	} else if(this->dist>otherobj.dist){
		return false;
	}
	return (this->index<otherobj.index);
}

bool ComputerMoveScene::LatticeDistanceInfo::operator==(const LatticeDistanceInfo &otherobj)const{
	return (this->index==otherobj.index) && (this->dist==otherobj.dist);
}


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

	//�e�i�q�_�ւ̋����}�b�v�̍쐬
	const size_t latticeNum=m_latticeInShape.size();
	std::vector<LatticeDistanceInfo> latticeDistanceInfo(latticeNum,LatticeDistanceInfo(latticeNum,latticeNum,-0.1f));//�e�i�q�_�ɐi�ނ��߂̍ŏ������ƁA�ǂ̊i�q�_����ړ�����΂悢�����i�[����Ă���B
	std::set<LatticeDistanceInfo> latticeSet;//�T�����Ă����i�q�_�̃��X�g�B�߂�������T�����߂�set��p����B
	auto SearchUpdate=[&latticeSet,&latticeDistanceInfo](const size_t from,const float dist,const size_t x,const size_t y,const size_t width,const size_t height,const std::vector<int> &latticeInShape){
		//latticeDistanceInfo���X�V���AlatticeSet�Ɋi�[����B
		if(x>=0 && x<width && y>=0 && y<height){
			size_t index=x+y*width;
			int po=latticeInShape[index];
			if(latticeInShape[index]!=1 && (latticeDistanceInfo[index].dist<0.0f || dist<latticeDistanceInfo[index].dist)){
				//�i���s�ł͂Ȃ��A�Ȃ��������������[�g�����Z�����[�g����������Ă��Ȃ����
				//�����}�b�v�̍X�V���s��
				LatticeDistanceInfo info(index,from,dist);
				latticeDistanceInfo[index]=info;
				if(latticeInShape[index]==0){
					//�T������i�q�_�̒ǉ��́Am_latticeInShape[index]��0�i���Ȃ킿�ʂ��ꏊ�j��\��
					latticeSet.insert(info);
				}
			}
		}
	};
	//�����ʒu�ɋ߂�4�_��T��
	const size_t initX=(size_t)(m_battleSceneData->m_operateUnit->getPos().x/squareSize),initY=(size_t)(m_battleSceneData->m_operateUnit->getPos().y/squareSize);
	SearchUpdate(latticeNum,(m_battleSceneData->m_operateUnit->getPos()-Vector2D((float)(initX*squareSize),(float)(initY*squareSize))).size(),initX,initY,m_xLatticeNum,m_yLatticeNum,m_latticeInShape);
	SearchUpdate(latticeNum,(m_battleSceneData->m_operateUnit->getPos()-Vector2D((float)((initX+1)*squareSize),(float)(initY*squareSize))).size(),initX+1,initY,m_xLatticeNum,m_yLatticeNum,m_latticeInShape);
	SearchUpdate(latticeNum,(m_battleSceneData->m_operateUnit->getPos()-Vector2D((float)(initX*squareSize),(float)((initY+1)*squareSize))).size(),initX,initY+1,m_xLatticeNum,m_yLatticeNum,m_latticeInShape);
	SearchUpdate(latticeNum,(m_battleSceneData->m_operateUnit->getPos()-Vector2D((float)((initX+1)*squareSize),(float)((initY+1)*squareSize))).size(),initX+1,initY+1,m_xLatticeNum,m_yLatticeNum,m_latticeInShape);
	//�ʂ��\���̂��镔����S�ĒT��
	const int dx[8]={-1,0,1,-1,1,-1,0,1};
	const int dy[8]={-1,-1,-1,0,0,1,1,1};
	const float slantingdist=Vector2D((float)(squareSize),(float)(squareSize)).size();
	const float horizondist=(float)squareSize;
	const float dis[8]={slantingdist,horizondist,slantingdist,horizondist,horizondist,slantingdist,horizondist,slantingdist};
	while(!latticeSet.empty()){
		std::set<LatticeDistanceInfo>::iterator beginIt=latticeSet.begin();
		LatticeDistanceInfo beginInfo(*beginIt);
		latticeSet.erase(beginIt);//�T��������s��Ȃ��̂�set�����菜���Ă���
		//����8�̊i�q�_�ɂ��čX�V���s��
		const size_t x=beginInfo.index%m_xLatticeNum,y=beginInfo.index/m_xLatticeNum;
		for(size_t i=0;i<8;i++){
			SearchUpdate(beginInfo.index,beginInfo.dist+dis[i],x+dx[i],y+dy[i],m_xLatticeNum,m_yLatticeNum,m_latticeInShape);
		}
	}

	int a=0;

	distvec=latticeDistanceInfo;

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
/*
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
//*/
		if(distvec[i].dist<0.0f){
			color=GetColor(0,255,255);
		} else{
			int level=(int)distvec[i].dist/10;
			color=GetColor(level,40,40);
		}
		DrawCircleAA((float)(x),(float)(y),2,6,color,TRUE);
	}
}
