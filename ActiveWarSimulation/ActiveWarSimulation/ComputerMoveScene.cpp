#include"input.h"
#include"DxLib.h"
#include"ComputerMoveScene.h"
#include"CommonConstParameter.h"

#include"Circle.h"


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
const size_t ComputerMoveScene::squareSize=CommonConstParameter::unitCircleSize;

ComputerMoveScene::ComputerMoveScene(std::shared_ptr<BattleSceneData> battleSceneData)
	:MoveScene(battleSceneData)
	,m_xLatticeNum((size_t)(battleSceneData->m_stageSize.x)/squareSize+1)
	,m_yLatticeNum((size_t)(battleSceneData->m_stageSize.y)/squareSize+1)
	,m_actionWaiting(false)
	,m_nextScene(SceneKind::e_move)
	,m_aimChangeFlame(0)
{
	//�i�q�_�̐������z����m��
	m_latticeInShape=std::vector<int>(m_xLatticeNum*m_yLatticeNum,0);
	//���삷�郆�j�b�g���ʂ�Ȃ��ꏊ��1���i�[���Ă���
	//�u��Q���̒������ɂ��Ȃ��ƃ��j�b�g�͒ʂ�Ȃ��v�Ƃ����l������p����
	for(const BattleObject *object:m_battleSceneData->m_field){
		if(object==m_battleSceneData->m_operateUnit){
			//���샆�j�b�g�͒ʂ�邩�ۂ��ɉe����^���Ȃ�

		} else if(object->GetFix()==Shape::Fix::e_ignore){
			//�����蔻��̂Ȃ��I�u�W�F�N�g�͒ʂ�邩�ۂ��ɉe����^���Ȃ�

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
				ImpassableLatticeInShape(x,y);//m_latticeInShape[index]��1�Ƃ��鏈�����܂ނ��A���X1�Ȃ̂Ŗ��Ȃ�
			}
		}
	}

	//�ʂ�i�q�_�̌v�Z
	CalculateLatticeRoute();
}

ComputerMoveScene::~ComputerMoveScene(){}

Vector2D ComputerMoveScene::CalculateLatticePointPos(size_t x,size_t y)const{
	return Vector2D((float)(x*squareSize),(float)(y*squareSize));
}

Vector2D ComputerMoveScene::CalculateLatticePointPos(size_t index)const{
	return CalculateLatticePointPos(index%m_xLatticeNum,index/m_xLatticeNum);
}

std::pair<std::pair<size_t,Vector2D>,Unit *> ComputerMoveScene::DecideTargetPoint(const std::vector<LatticeDistanceInfo> &distanceInfo)const{
	//�_�����j�b�g�����߂�
	Unit *targetUnit=nullptr;//���̊֐����ł͕ύX����Ȃ�
	float point=-50000.0f;
	for(Unit *pu:m_battleSceneData->m_unitList){
		if(m_battleSceneData->m_operateUnit->GetBattleStatus().weapon->JudgeWeild(m_battleSceneData->m_operateUnit,pu)){
			//�U���ΏۂƂȂ郆�j�b�g����������
			if(targetUnit==nullptr){
				//�܂���₪���Ȃ��ꍇ��
				targetUnit=pu;//���̃��j�b�g�����ɂ���
				point=CalculateEvaluate(targetUnit,distanceInfo);//���̃��j�b�g�̍U���ΏۂƂ��Ă̕]���l���v�Z����
			} else{
				//���Ɍ�₪����ꍇ�͕]���l���v�Z����
				const float tmpPoint=CalculateEvaluate(pu,distanceInfo);
				if(tmpPoint>point){
					//�]���l��������΂��܌������Ă��郆�j�b�g���U���ΏۂƂ���
					targetUnit=pu;
					point=tmpPoint;
				}
			}
		}
	}
	//�i�q�_�T��
	const size_t vecSize=m_latticeInShape.size();
	if(targetUnit!=nullptr){
		//�_�����j�b�g�����߂���A���̃��j�b�g���U���ł���i�q�_��T��
		//�×~�@�ł���
		Unit copiedUnit(*dynamic_cast<Unit *>(m_battleSceneData->m_operateUnit->VCopy().get()));//��Ɨp�ɃR�s�[�����
		size_t target=vecSize;
		for(size_t y=0;y<m_yLatticeNum;y++){
			for(size_t x=0;x<m_xLatticeNum;x++){
				const size_t index=x+y*m_xLatticeNum;
				if(distanceInfo[index].dist>=0.0f && (target>=vecSize || distanceInfo[index]<distanceInfo[target])){
					//���ɕʂ̂��߂��ڕW�n�_��₪���݂���Ȃ�V�����ڕW�n�_�ɂȂ鎖�͂Ȃ�
					const Vector2D pos=CalculateLatticePointPos(x,y);
					copiedUnit.Warp(pos);
					if(copiedUnit.JudgeAttackable(targetUnit)){
						//�U���\�Ȃ炱�̈ʒu���b��̖ڕW�n�_�Ƃ���
						target=index;
					}
				}
			}
		}
		if(target!=vecSize){
			return std::pair<std::pair<size_t,Vector2D>,Unit *>(std::pair<size_t,Vector2D>(target,targetUnit->getPos()),targetUnit);
		}
	} else{
		//�_�����j�b�g�������Ȃ�A���̏�őҋ@����
	}
	return std::pair<std::pair<size_t,Vector2D>,Unit *>(std::pair<size_t,Vector2D>(vecSize,m_battleSceneData->m_operateUnit->getPos()),targetUnit);
}

void ComputerMoveScene::CalculateLatticeDistanceInfo(std::vector<LatticeDistanceInfo> &latticeDistanceInfo)const{
	//���O����
	const size_t latticeNum=m_latticeInShape.size();//���x���g���̂ŗ\�ߌv�Z���Ă���
	latticeDistanceInfo=std::vector<LatticeDistanceInfo>(latticeNum,LatticeDistanceInfo(latticeNum,latticeNum,-0.1f));//������
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

}

void ComputerMoveScene::CalculateLatticeRoute(){
	m_latticeRoute.clear();
	//�e�i�q�_�ւ̋����}�b�v�̍쐬
	const size_t latticeNum=m_latticeInShape.size();
	std::vector<LatticeDistanceInfo> latticeDistanceInfo;//�e�i�q�_�ɐi�ނ��߂̍ŏ������ƁA�ǂ̊i�q�_����ړ�����΂悢�����i�[����Ă���B
	CalculateLatticeDistanceInfo(latticeDistanceInfo);//�����}�b�v�v�Z�֐�

	distvec=latticeDistanceInfo;//�f�o�b�O�p

	//�ڕW�n�_�̌���
	const std::pair<std::pair<size_t,Vector2D>,Unit *> targetPoint=DecideTargetPoint(latticeDistanceInfo);//������AI�̍s���X���ɂ���ĈقȂ�̂�
	m_targetUnit=targetPoint.second;

	//���[�g�̑I��Ɗi�[
	m_latticeRoute.push_back(std::pair<size_t,Vector2D>(latticeNum,targetPoint.first.second));//�ŏI�ڕW�̊i�q�_�ɂ�����Ɍ������ʒu���ŏ��Ɋi�[���Ă����i����͋��炭���j�b�g�����œ���Ȃ��j
	for(size_t point=targetPoint.first.first;point<latticeNum;point=latticeDistanceInfo[point].from){
		//latticeDistanceInfo[point].from��H���Ă����΍ŒZ�����ƂȂ�
		Vector2D v;
		if(point<latticeNum){
			//�i�q�_�����݂���Ȃ炻�̈ʒu��
			v=CalculateLatticePointPos(point);
		} else{
			//�i�q�_�����݂��Ȃ��Ȃ炻�̏��(point==latticeNum�̎��̂݁B����́u���샆�j�b�g�̈ʒu�ɂ��邱�Ɓv��\��)
			v=m_battleSceneData->m_operateUnit->getPos();
		}
		m_latticeRoute.insert(m_latticeRoute.begin(),std::pair<size_t,Vector2D>(point,v));
	}

}

Vector2D ComputerMoveScene::CalculateInputVec()const{
	Vector2D moveVec;
	//�R���s���[�^���쎞�AAI�����������߂�
	//�^�[���J�n����1�b�o�����瓮��
	if(m_battleSceneData->m_fpsMesuring.GetProcessedTime()>1.0){
		//m_latticeRoute�̐擪�Ɍ������ē���
		if(!m_latticeRoute.empty()){
			moveVec=m_latticeRoute.front().second-m_battleSceneData->m_operateUnit->getPos();
		}
	}
	return moveVec;
}

float ComputerMoveScene::CalculateEvaluate(const Unit *punit,const std::vector<LatticeDistanceInfo> &distanceInfo)const{
	//const float sqDistance=-(punit->getPos()-m_battleSceneData->m_operateUnit->getPos()).sqSize();//���j�b�g�Ԃ̋�����2��l�B
	//punit�ɍU�����͂��ʒu�܂ł̋����ɂ��Ă̕]���l
	float distanceEvaluate=-50000.0f;//�ЂƂ܂������_��������
	if(punit->GetHitJudgeShape()->GetType()==Shape::Type::e_circle){
		const Circle *pcircle=dynamic_cast<const Circle *>(punit->GetHitJudgeShape());//��Ɨp�̃��j�b�g�}�`�̊m��
		if(pcircle!=nullptr){
			Circle c(pcircle->GetPosition(),pcircle->GetR()+m_battleSceneData->m_operateUnit->GetBattleStatus().weapon->GetLength(),pcircle->m_fix);//�U���\�͈�
			//�U���\�͈͂̒��ɂ���i�q�_�̒��ōŋߓ_�����߂�
			const size_t size=distanceInfo.size();
			size_t nearestIndex=size;//���ꂪsize�ł���Ԃ�
			for(size_t i=0;i<size;i++){
				if(m_latticeInShape[i]==0 && distanceInfo[i].dist>=0.0f && c.VJudgePointInsideShape(CalculateLatticePointPos(i)) && (nearestIndex>=size || distanceInfo[i].dist<distanceInfo[nearestIndex].dist)){
					//�_i���N���\�����B�\�ŁA�U���\�͈͓��ɂ���A�������݂̍ŋߓ_���߂��̂ł����
					nearestIndex=i;//�ŋߓ_�̍X�V
				}
			}
			if(nearestIndex<size){
				//�ŋߓ_�����݂���Ȃ��(=���x��������punit�ɍU���ł���̂ł����)�܂Ƃ��ȓ_��������
				if(m_battleSceneData->m_operateUnit->GetMoveDistance()<distanceInfo[nearestIndex].dist){
					//1�^�[���̊Ԃɂ��ǂ蒅����ꏊ�łȂ��Ȃ�A�����������Ȃ�قǂɓ_����������
					distanceEvaluate=-distanceInfo[nearestIndex].dist;
				} else{
					//1�^�[���̊Ԃɂ��ǂ蒅����ꏊ�ł���Ȃ�A�ꗥ�ł��������ǂ��_������
					distanceEvaluate=0.0f;
				}
			}

		}
	}

	//�s�������ۂ̉e���x�ɂ��Ă̕]���l
	const float actionEvaluate=CalculateActionEvaluate(punit);

	//���v�̕]���l��Ԃ�
	return distanceEvaluate+actionEvaluate;
}

float ComputerMoveScene::CalculateActionEvaluate(const Unit *punit)const{
	//�s�������ۂ̉e���x�ɂ��Ă̕]���l
	float actionEvaluate=-50000.0f;
	//2���j�b�g���G���m���Ƃ��ĕ]���l���v�Z���A�����ł���΍Ō�ɕ]���l�̐����𔽓]������
	Weapon::AttackInfo attackinfo=m_battleSceneData->m_operateUnit->GetBattleStatus().weapon->GetAttackInfo(m_battleSceneData->m_operateUnit,punit);
	actionEvaluate=m_battleSceneData->m_operateUnit->GetMoveDistance()*std::fminf(1.0f,(float)(1.0*attackinfo.damage/punit->GetBattleStatus().HP));//��{�I�Ƀs���`�̓G�ɍU���E�s���`�̖����ɉ񕜂����������]���������B���K�����s���B
	if(Unit::Team::JudgeFriend(m_battleSceneData->m_operateUnit->GetBattleStatus().team,punit->GetBattleStatus().team)){
		actionEvaluate=-actionEvaluate;
	}
	return actionEvaluate;
}

void ComputerMoveScene::ImpassableLatticeInShape(const size_t index){
	const size_t x=index%m_xLatticeNum,y=index/m_xLatticeNum;
	ImpassableLatticeInShape(x,y);
}

void ComputerMoveScene::ImpassableLatticeInShape(const size_t x,const size_t y){
	const size_t index=x+y*m_xLatticeNum;
	if(x<m_xLatticeNum && y<m_yLatticeNum){
		m_latticeInShape[index]=1;
	}
	//�S�Ă�1�ł���i�q�_�ɗאڂ���i�q�_��2�ɂ���B
	auto Input2Only0=[](int &a){if(a==0){ a=2; }};
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

int ComputerMoveScene::BranchingWaitingProcess(){
	//������������n�_����s���Ώۂɂł���L���������邩�𒲂ׂ�B��������ꍇ�͕]���l�̍����L������ΏۂƂ���B
	float evaluate=-50000.0f;
	Unit *pActedUnit=nullptr;
	for(Unit *pUnit:m_battleSceneData->m_unitList){
		if(JudgeBecomeAimedUnit(pUnit)){
			//�s���Ώۂɂł����ꍇ�́A�X�V���邩���肵�ďꍇ�ɂ���Ă͍X�V����
			const float pal=CalculateActionEvaluate(pUnit);
			if(pActedUnit==nullptr || evaluate<pal){
				pActedUnit=pUnit;
				evaluate=pal;
			}
		}
	}
	//�U�����邩�ҋ@���邩�̕���
	if(pActedUnit==nullptr){
		//�s���Ώۂ����Ȃ���Αҋ@
		FinishUnitOperation();
		return 0;
	} else{
		//�s���Ώۂ�����΂��̃��j�b�g��Ώۂɂ��čs����ʂ�
		m_aimedUnit=pActedUnit;
		return SceneKind::e_attackNormal;
	}
}

int ComputerMoveScene::thisCalculate(){
	//m_aimChangeFlame�̍X�V
	m_aimChangeFlame++;
	//m_operateUnit�̈ʒu�X�V
	const Vector2D beforeVec=m_battleSceneData->m_operateUnit->getPos();
	PositionUpdate(CalculateInputVec());
	const float moveSqLength=(beforeVec-m_battleSceneData->m_operateUnit->getPos()).sqSize();
	const double processedTime=m_battleSceneData->m_fpsMesuring.GetProcessedTime();
	if(!m_actionWaiting){
		//�s���܂ł̑҂����Ԃ�҂��Ă����Ԃł͂Ȃ���
		if(processedTime>1.0){
			//1�b�o������s������
			if(JudgeAttackCommandUsable() && m_aimedUnit==m_targetUnit){
				//m_aimerUnit��AI�����߂Ă����U���ΏۂɈ�v�������A�U���������s��
				//FinishUnitOperation();//�s���I������(���Ƃ�)
				//return SceneKind::e_attackNormal;//�U����ʂ�
				m_nextScene=SceneKind::e_attackNormal;
				m_battleSceneData->m_fpsMesuring.RecordTime();
				m_actionWaiting=true;
			} else if(m_targetUnit!=nullptr && m_battleSceneData->m_operateUnit->JudgeAttackable(m_targetUnit) && m_aimedUnit!=m_targetUnit){
				//AI�����߂Ă����U���Ώۂ��U���͈͓��ɂ��邪�Am_aimedUnit������Ɉ�v���Ȃ��Ƃ��́A�U���Ώۂ𓮂���
				if(m_aimChangeFlame>5){
					//�������A5�t���[���قǊԊu����������
					SetAimedUnit(1);
					PlaySoundMem(m_battleSceneData->m_aimchangeSound,DX_PLAYTYPE_BACK,TRUE);//�_���؂�ւ��Ȃ̂ł��̌��ʉ���
					m_aimChangeFlame=0;
				}
			} else if(m_battleSceneData->m_operateUnit->GetBattleStatus().OP<2.0f
				//			|| processedTime>10.0//�f�o�b�O�̂��߂Ɉ�x�Ȃ��Ă���
				|| keyboard_get(KEY_INPUT_Q)==1//���Ԑ������Ȃ��ۂɃQ�[���ɖ߂��悤�ɂ��邽��
				){
				//�ړ��ł��Ȃ��Ȃ�����A�܂���10�b�o������ҋ@
				//return BranchingWaitingProcess();//�s���Ώۂ�����΍s������
				m_nextScene=BranchingWaitingProcess();
				m_battleSceneData->m_fpsMesuring.RecordTime();
				m_actionWaiting=true;
			} else if((moveSqLength<0.1f && processedTime>2.0)){
				//�ړ����������Ȃ��ꍇ�͈ړ���̕ύX
				if(m_latticeRoute.size()<2){
					//�i�ޏꏊ���Ȃ��܂��͍Ō��1�_�ɂ��ǂ蒅�����Ɏ~�܂��Ă���ꍇ�͑ҋ@�ł悢
					//�Ō��1�_�̏ꍇ���ҋ@���s�����R�́A�擪�_��i���s�ɂ��鎖�Ń��[�g�ύX���s�����A�Ō��1�_�͑��͌��X�i���s�Ń��[�g���ς�炸�A�������[�v�ƂȂ��Ă��܂�����B
					//return BranchingWaitingProcess();//�s���Ώۂ�����΍s������
					m_nextScene=BranchingWaitingProcess();
					m_battleSceneData->m_fpsMesuring.RecordTime();
					m_actionWaiting=true;
				} else{
					//�o�R�_�ōs���l�܂��Ă���ꍇ�A���̃��[�g�͊Ԉ���Ă���ƍl������̂Ń��[�g�ύX������
					if(m_latticeRoute.front().first<m_latticeInShape.size()){
						ImpassableLatticeInShape(m_latticeRoute.front().first);//���ʂ낤�Ƃ��Ă���i�q�_��ʂ�Ȃ�����
						CalculateLatticeRoute();//���[�g�ύX����
					}
				}
			} else if(!m_latticeRoute.empty() && m_battleSceneData->m_operateUnit->JudgePointInsideShape(m_latticeRoute.front().second)){
				//m_latticeRoute�̐擪�̓_�����j�b�g�̓����蔻��}�`�ɓ������炻���܂ł͊ȒP�ɓ�����Ƃ������ƂȂ̂ňړ����ς���
				m_latticeRoute.erase(m_latticeRoute.begin());
			}
		}
	} else{
		//�҂����Ԃ�҂��Ă��鎞
		if(processedTime>0.1){
			//0.1�b�҂��Ă���s����
			if(m_nextScene==SceneKind::e_attackNormal && !JudgeAttackCommandUsable()){
				//�U���I���̏ꍇ�AJudgeAttackCommandUsable()������B�}�`�����o�������̉e���ŁA�u�U���ł���Ǝv������ł��Ȃ��v���������鎖�����邽�߁B
				FinishUnitOperation();//�Č����������̂ŁA���R�����I�ɔ�������̂�҂�
				return 0;
			} else{
				return m_nextScene;
			}
		}
	}
	return SceneKind::e_move;
}

void ComputerMoveScene::thisDraw()const{
	//�v���C���[�̂Ɠ����`�揈��
	MoveScene::thisDraw();

	//�f�o�b�O�p�`��
	if(m_battleSceneData->m_drawObjectShapeFlag){
		//�i�q�_��`��
		for(size_t i=0,size=m_latticeInShape.size();i<size;i++){
			const int x=(i%m_xLatticeNum)*squareSize,y=(i/m_xLatticeNum)*squareSize;
			unsigned int color;
			if(distvec[i].dist<0.0f){
				color=GetColor(0,0,255);
			} else{
				int level=(int)distvec[i].dist/3;
				color=GetColor(level%256,255-level%256,0);
			}
			DrawCircleAA((float)(x),(float)(y),2,6,color,TRUE);
		}
		//���[�g��`��
		if(!m_latticeRoute.empty()){
			DrawCircleAA(m_latticeRoute.front().second.x,m_latticeRoute.front().second.y,5,15,GetColor(255,255,0),FALSE);
			for(size_t i=0,size=m_latticeRoute.size();i+1<size;i++){
				DrawCircleAA(m_latticeRoute[i+1].second.x,m_latticeRoute[i+1].second.y,5,15,GetColor(255,255,0),FALSE);
				DrawLineAA(m_latticeRoute[i].second.x,m_latticeRoute[i].second.y,m_latticeRoute[i+1].second.x,m_latticeRoute[i+1].second.y,GetColor(255,255,0));
			}
		}
	}
}
