#include"input.h"
#include"DxLib.h"
#include"ComputerMoveScene.h"
#include"CommonConstParameter.h"

#include"Circle.h"


//---------------ComputerMoveScene::AttackFailedInfo------------------
ComputerMoveScene::AttackFailedInfo::AttackFailedInfo()
	:m_count(0),m_frame(0)
{}

bool ComputerMoveScene::AttackFailedInfo::JudgeAttackProcessProceed()const{
	return (m_count==0 || m_frame>6);//6�t���[���͈ړ��ɓ��Ă�
}

bool ComputerMoveScene::AttackFailedInfo::JudgeRetry()const{
	return (m_count<1);//��蒼���񐔂�1��
}

void ComputerMoveScene::AttackFailedInfo::RetryProcess(){
	m_count++;
	m_frame=0;
}

//---------------ComputerMoveScene------------------
ComputerMoveScene::ComputerMoveScene(std::shared_ptr<BattleSceneData> battleSceneData)
	:MoveScene(battleSceneData)
	,m_latticeField(battleSceneData->CalculateLatticeBattleField(true))
	,m_actionWaiting(false)
	,m_nextScene(SceneKind::e_move)
	,m_aimChangeFrame(0)
	,m_waitingFrame(0)
	,m_attackFailedInfo()
{
	//�ʂ�i�q�_�̌v�Z
	CalculateLatticeRoute();
}

ComputerMoveScene::~ComputerMoveScene(){}

std::pair<std::pair<size_t,Vector2D>,Unit *> ComputerMoveScene::DecideTargetPoint(const std::vector<LatticeBattleField::LatticeDistanceInfo> &distanceInfo)const{
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
	const size_t vecSize=m_latticeField->GetLatticeInShapeSize();
	Vector2D targetPointVec=m_battleSceneData->m_operateUnit->getPos();//target�ɑ���������W�܂œ��B�����ۂɂǂ̍��W�Ɍ������ē������B�����l�́u���̏�őҋ@����v
	size_t target=vecSize;
	if(targetUnit!=nullptr){
		//�_�����j�b�g�����߂���A���̃��j�b�g���U���ł���i�q�_��T��
		//�×~�@�ł���
		Unit copiedUnit(*dynamic_cast<Unit *>(m_battleSceneData->m_operateUnit->VCopy().get()));//��Ɨp�ɃR�s�[�����
		for(size_t y=0;y<m_latticeField->GetYLatticeNum();y++){
			for(size_t x=0;x<m_latticeField->GetXLatticeNum();x++){
				const size_t index=x+y*m_latticeField->GetXLatticeNum();
				if(distanceInfo[index].dist>=0.0f && (target>=vecSize || distanceInfo[index]<distanceInfo[target])){
					//���ɕʂ̂��߂��ڕW�n�_��₪���݂���Ȃ�V�����ڕW�n�_�ɂȂ鎖�͂Ȃ�
					const Vector2D pos=m_latticeField->CalculateLatticePointPos(x,y);
					copiedUnit.Warp(pos);
					if(copiedUnit.JudgeAttackable(targetUnit)){
						//�U���\�Ȃ炱�̈ʒu���b��̖ڕW�n�_�Ƃ���
						target=index;
					}
				}
			}
		}
		if(target!=vecSize){
			targetPointVec=targetUnit->getPos();//�b��ōŏI�ڕW���W���_���Ă��郆�j�b�g�ɂȂ�i�}���^AI�ł������肷��Ƃ�������ŕς��j
		}
	} else{
		//�_�����j�b�g�������Ȃ�A���̏�őҋ@����
		//(target=vecSize,targetPointVec=m_battleSceneData->m_operateUnit->getPos()�̂܂�)
	}

	//�_�����j�b�g�ƖړI�n�����܂����̂ŁAAI�̃��[���ɏ]���Ă����ɍs�����ǂ������߂�
	switch(m_battleSceneData->m_operateUnit->GetBattleStatus().aitype){
	case(Unit::AIType::e_wait):
		//�ҋ@�^AI
		//�K�����̏�őҋ@����
		target=vecSize;
		targetPointVec=m_battleSceneData->m_operateUnit->getPos();
		break;
	case(Unit::AIType::e_intercept):
	case(Unit::AIType::e_linkageIntercept):
		//�}���^AI�ƘA���}���^AI
		if(target>=vecSize || point<0.0f){
			//�ړI�n���P��̈ړ��œ͂��Ȃ��ꍇ�́A���̏�őҋ@����
			//point��0�ȏ�ł��鎖���P��̈ړ��œ͂����̕K�v�\�������ƂȂ��Ă���
			target=vecSize;
			targetPointVec=m_battleSceneData->m_operateUnit->getPos();
		}
		break;
	case(Unit::AIType::e_assult):
		//�ˌ��^AI
		//���߂��ړI�n�܂Ői��(target�ɕύX�͖���)
		break;
	}

	//�ړ����邩�ǂ����ɂ��AI�̕ω�����
	if(target!=vecSize){
		//�s������ꍇ�A�����ɕR�Â��Ă���A���}���^AI�̃L�����N�^��AI��ˌ��^�ɕύX����K�v������
		const int group=m_battleSceneData->m_operateUnit->GetBattleStatus().aiGroup;
		for(Unit *pu:m_battleSceneData->m_unitList){
			//�S�Ẵ��j�b�g�ɑ΂��Č���
			if(pu->GetBattleStatus().aiLinkage.count(group)!=0){
				//group�l��pu��aiLinkage�ɑ��݂��Ă��āA����pu���A���}���^AI�ł���ꍇ�́Apu��m_operateUnit�ɘA�����ē����O���[�v�ł���̂œˌ��^�ɂȂ�
				//pu���A���}���^�ł��邩�ǂ����̔���́AUnit::BecomeAssultAI()���Ŕ��肷��B�������̂ق����R�[�f�B���O�̃~�X�͌���Ǝv���B
				pu->BecomeAssultAI();
			}
		}
	}

	return std::pair<std::pair<size_t,Vector2D>,Unit *>(std::pair<size_t,Vector2D>(target,targetPointVec),targetUnit);
}

void ComputerMoveScene::CalculateLatticeRoute(){
	m_latticeRoute.clear();
	//�e�i�q�_�ւ̋����}�b�v�̍쐬
	const size_t latticeNum=m_latticeField->GetLatticeInShapeSize();
	std::vector<LatticeBattleField::LatticeDistanceInfo> latticeDistanceInfo;//�e�i�q�_�ɐi�ނ��߂̍ŏ������ƁA�ǂ̊i�q�_����ړ�����΂悢�����i�[����Ă���B
	m_latticeField->CalculateLatticeDistanceInfo(latticeDistanceInfo,m_battleSceneData->m_operateUnit->getPos());//�����}�b�v�v�Z�֐�

	distvec=latticeDistanceInfo;//�f�o�b�O�p

	//�ڕW�n�_�̌���
	const std::pair<std::pair<size_t,Vector2D>,Unit *> targetPoint=DecideTargetPoint(latticeDistanceInfo);//������AI�̍s���X���ɂ���ĈقȂ�̂�
	m_targetUnit=targetPoint.second;
	size_t targetPointIndex=targetPoint.first.first;//�ڕW�n�_�̊i�q�_�ԍ�
	Vector2D targetPointVec=targetPoint.first.second;//�ڕW���W
	if(targetPointIndex==latticeNum && m_battleSceneData->m_operateUnit->GetBattleStatus().aitype==Unit::AIType::e_assult){
		//�ˌ��^���ڕW�n�_�����݈ʒu�ł���ꍇ�A�l�܂肪�������Ă���B���̂��߁A�r���܂Ői�߂�悤�ɍēx�v�Z������B
		//���j�b�g�̓����蔻��𖳎������ڕW�n�_�̌v�Z���s��
		const std::shared_ptr<LatticeBattleField> virtualLatticeField=m_battleSceneData->CalculateLatticeBattleField(false);//���j�b�g���N�����Ȃ��n�`�Ōv�Z�����Ĕ�r���Ȃ��烋�[�g����������΁A�l�܂�n�_�܂ł͐i�߂�
		const size_t virtualLatticeNum=virtualLatticeField->GetLatticeInShapeSize();
		std::vector<LatticeBattleField::LatticeDistanceInfo> virtualLatticeDistanceInfo;
		virtualLatticeField->CalculateLatticeDistanceInfo(virtualLatticeDistanceInfo,m_battleSceneData->m_operateUnit->getPos());
		const std::pair<std::pair<size_t,Vector2D>,Unit *> virtualTargetPoint=DecideTargetPoint(virtualLatticeDistanceInfo);
		//�ڕW�n�_�v�Z(����ł��T���Ɏ��s����ƁA���̏�œ����Ȃ��Ȃ�B���ꎩ�͍̂\��Ȃ��B)
		for(size_t point=virtualTargetPoint.first.first;point<latticeNum;point=virtualLatticeDistanceInfo[point].from){
			//latticeDistanceInfo[point].from��H���Ă����΍ŒZ�����ƂȂ�
			if(latticeDistanceInfo[point].from<latticeNum){
				//point�ɂ��ǂ蒅�������ł���Ȃ��(point�ɂ��ǂ蒅�������ł���i�q�_�̑��݂���������Ηǂ�)
				targetPointIndex=point;
				targetPointVec=m_latticeField->CalculateLatticePointPos(point);//���̊i�q�_�̈ʒu���ŏI�ڕW�n�_�Ƃ���
				break;
			}
		}
	}

	//���[�g�̑I��Ɗi�[
	m_latticeRoute.push_back(std::pair<size_t,Vector2D>(latticeNum,targetPointVec));//�ŏI�ڕW�̊i�q�_�ɂ�����Ɍ������ʒu���ŏ��Ɋi�[���Ă����i����͋��炭���j�b�g�����œ���Ȃ��j
	for(size_t point=targetPointIndex;point<latticeNum;point=latticeDistanceInfo[point].from){
		//latticeDistanceInfo[point].from��H���Ă����΍ŒZ�����ƂȂ�
		Vector2D v;
		if(point<latticeNum){
			//�i�q�_�����݂���Ȃ炻�̈ʒu��
			v=m_latticeField->CalculateLatticePointPos(point);
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
	//�^�[���J�n����1�b�o�����瓮���A�܂��ҋ@�܂ł̑҂�����(=m_actionWaiting��true�̎�)�͈ړ����Ȃ�
	if(m_battleSceneData->m_fpsMesuring.GetProcessedTime()>1.0 && !m_actionWaiting){
		//m_latticeRoute�̐擪�Ɍ������ē���
		if(!m_latticeRoute.empty()){
			moveVec=m_latticeRoute.front().second-m_battleSceneData->m_operateUnit->getPos();
		}
	}
	return moveVec;
}

float ComputerMoveScene::CalculateEvaluate(const Unit *punit,const std::vector<LatticeBattleField::LatticeDistanceInfo> &distanceInfo)const{
	//const float sqDistance=-(punit->getPos()-m_battleSceneData->m_operateUnit->getPos()).sqSize();//���j�b�g�Ԃ̋�����2��l�B
	//punit�ɍU�����͂��ʒu�܂ł̋����ɂ��Ă̕]���l
	//distanceEvaluate�̒l��
	//	distanceEvaluate=0:�͂�
	//	distanceEvaluate<=m_operateUnit->GetMoveDistance():�͂��Ȃ�
	float distanceEvaluate=-50000.0f;//�ЂƂ܂������_��������
	if(punit->GetHitJudgeShape()->GetType()==Shape::Type::e_circle){
		const Circle *pcircle=dynamic_cast<const Circle *>(punit->GetUnitCircleShape());//��Ɨp�̃��j�b�g�}�`(����)�̊m��
		if(pcircle!=nullptr){
			Circle c(pcircle->GetPosition(),pcircle->GetR()+m_battleSceneData->m_operateUnit->GetBattleStatus().weapon->GetLength()+LatticeBattleField::latticeIntervalSize,pcircle->m_fix);//�U���\�͈�(�������A�ʒu�ɂ���Ă͊i�q�_�����o�ł��Ȃ����߁ALatticeBattleField::latticeIntervalSize�����]���ɍL���͈͂Ŋi�q�_��T��)
			//�U���\�͈͂̒��ɂ���i�q�_�̒��ōŋߓ_�����߂�
			const size_t size=distanceInfo.size();
			size_t nearestIndex=size;//���ꂪsize�ł���Ԃ�
			for(size_t i=0;i<size;i++){
				if(m_latticeField->GetLatticeInShapeAt(i)==LatticeBattleField::LatticePass::e_passable
					&& distanceInfo[i].dist>=0.0f
					&& c.VJudgePointInsideShape(m_latticeField->CalculateLatticePointPos(i))
					&& (nearestIndex>=size || distanceInfo[i].dist<distanceInfo[nearestIndex].dist))
				{
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
	//actionEvaluate�̒l��́A1�^�[���̊ԂŖڕW���j�b�g��action���ł��Ȃ��ꍇ��total�����̒l�ɂȂ�悤�ɂ��邽�߂ɂ�������
	//	actionEvaluate<=m_operateUnit->GetMoveDistance()
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
	if(pActedUnit==nullptr || evaluate<=0.0){
		//�s���Ώۂ����Ȃ���Αҋ@
		//�s���ɈӖ����Ȃ��Ă��ҋ@
		FinishUnitOperation();
		return 0;
	} else{
		//�s���Ώۂ�����΂��̃��j�b�g��Ώۂɂ��čs����ʂ�
		m_aimedUnit=pActedUnit;
		return SceneKind::e_attackNormal;
	}
}

int ComputerMoveScene::thisCalculate(){
	//�t���[���֌W�̍X�V
	m_aimChangeFrame++;
	m_attackFailedInfo.m_frame++;
	//m_operateUnit�̈ʒu�X�V
	const Vector2D beforeVec=m_battleSceneData->m_operateUnit->getPos();
	PositionUpdate(CalculateInputVec());
	const float moveSqLength=(beforeVec-m_battleSceneData->m_operateUnit->getPos()).sqSize();
	const size_t processedFrame=m_battleSceneData->m_fpsMesuring.GetProcessedFrame();
	if(!m_actionWaiting){
		//�s���܂ł̑҂����Ԃ�҂��Ă����Ԃł͂Ȃ���
		if(processedFrame>45){
			//1�b�o������s������
			if(JudgeAttackCommandUsable() && m_aimedUnit==m_targetUnit && m_attackFailedInfo.JudgeAttackProcessProceed()){
				//m_aimerUnit��AI�����߂Ă����U���ΏۂɈ�v�������A�U���������s��
				//�������Am_attackFailedInfo���番����Ĉړ��̍ۂ͋K��t���[����҂�
				m_nextScene=BranchingWaitingProcess();
				m_actionWaiting=true;
			} else if(m_targetUnit!=nullptr && m_battleSceneData->m_operateUnit->JudgeAttackable(m_targetUnit) && m_aimedUnit!=m_targetUnit){
				//AI�����߂Ă����U���Ώۂ��U���͈͓��ɂ��邪�Am_aimedUnit������Ɉ�v���Ȃ��Ƃ��́A�U���Ώۂ𓮂���
				if(m_aimChangeFrame>5){
					//�������A5�t���[���قǊԊu����������
					SetAimedUnit(1);
					PlaySoundMem(m_battleSceneData->m_aimchangeSound,DX_PLAYTYPE_BACK,TRUE);//�_���؂�ւ��Ȃ̂ł��̌��ʉ���
					m_aimChangeFrame=0;
				}
			} else if(m_battleSceneData->m_operateUnit->GetBattleStatus().OP<2.0f
				|| processedFrame>600//�s������������ɃQ�[���ɖ߂��悤�ɂ��邽��
				)
			{
				//�ړ��ł��Ȃ��Ȃ�����A�܂���10�b�o������ҋ@
				m_nextScene=BranchingWaitingProcess();
				m_actionWaiting=true;
			} else if((moveSqLength<0.1f && processedFrame>120)){
				//�ړ����������Ȃ��ꍇ�͈ړ���̕ύX
				if(m_latticeRoute.size()<2){
					//�i�ޏꏊ���Ȃ��܂��͍Ō��1�_�ɂ��ǂ蒅�����Ɏ~�܂��Ă���ꍇ�͑ҋ@�ł悢
					//�Ō��1�_�̏ꍇ���ҋ@���s�����R�́A�擪�_��i���s�ɂ��鎖�Ń��[�g�ύX���s�����A�Ō��1�_�͑��͌��X�i���s�Ń��[�g���ς�炸�A�������[�v�ƂȂ��Ă��܂�����B
					m_nextScene=BranchingWaitingProcess();
					m_actionWaiting=true;
				} else{
					//�o�R�_�ōs���l�܂��Ă���ꍇ�A���̃��[�g�͊Ԉ���Ă���ƍl������̂Ń��[�g�ύX������
					if(m_latticeRoute.front().first<m_latticeField->GetLatticeInShapeSize()){
						m_latticeField->BecomeImpassibleLattice(m_latticeRoute.front().first);//���ʂ낤�Ƃ��Ă���i�q�_��ʂ�Ȃ�����
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
		m_waitingFrame++;
		if(m_waitingFrame>6){
			//0.1�b�҂��Ă���s����
			if(m_nextScene==SceneKind::e_attackNormal && !JudgeAttackCommandUsable()){
				//�U���I���̏ꍇ�AJudgeAttackCommandUsable()������B�}�`�����o�������̉e���ŁA�u�U���ł���Ǝv������ł��Ȃ��v���������鎖�����邽�߁B
				//�����Ȃ�p�^�[���͗l�X���邽�߁A�P�x�����ĒT�����s�킹�A����ł��_���Ȃ�ҋ@���鏈���ɂ���B
				if(m_attackFailedInfo.JudgeRetry()){
					//�K��񐔓��̎��s�̎��A��蒼��������
					m_actionWaiting=!m_actionWaiting;
					m_waitingFrame=0;
					m_attackFailedInfo.RetryProcess();
				}else{
					//��蒼���񐔂��I�[�o�[������A�������������Ƀ^�[���I��
					FinishUnitOperation();//�Č����������̂ŁA���R�����I�ɔ�������̂�҂�
					m_battleSceneData->m_fpsMesuring.RecordTime();
					return 0;
				}
			} else{
				//���Ȃ��s���ł���Ȃ�A�s�������Ɉڍs����
				m_battleSceneData->m_fpsMesuring.RecordTime();
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
		for(size_t i=0,size=m_latticeField->GetLatticeInShapeSize();i<size;i++){
			const int x=(i%m_latticeField->GetXLatticeNum())*LatticeBattleField::latticeIntervalSize,y=(i/m_latticeField->GetXLatticeNum())*LatticeBattleField::latticeIntervalSize;
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
