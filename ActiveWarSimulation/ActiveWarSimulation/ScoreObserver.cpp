#include"ScoreObserver.h"
#include<algorithm>
#include<cassert>
#include<map>

#include"WaitLog.h"
#include"AttackLog.h"
#include"FinishLog.h"
#include"InitLog.h"

void ScoreObserver::InitUpdate(const BattleSceneData * const battleData){
	m_logList.clear();
	m_logList.push_back(std::make_shared<InitLog>(battleData));
}

void ScoreObserver::FinishUpdate(const BattleSceneData * const battleData){
	m_logList.push_back(std::make_shared<FinishLog>(battleData));
}

void ScoreObserver::AttackUpdate(const BattleSceneData * const battleData,const Unit * const aimedUnit){
	m_logList.push_back(std::make_shared<AttackLog>(battleData,aimedUnit));
}

void ScoreObserver::ResearchUpdate(){
	m_researchCount++;
}

void ScoreObserver::WaitUpdate(const BattleSceneData * const battleData){
	m_logList.push_back(std::make_shared<WaitLog>(battleData));
}

void ScoreObserver::CancelUpdate(){
	m_cancelCount++;
}

std::string ScoreObserver::GetScoreExplain()const{
	//�X�R�A�v�Z���@���x�^��������Ă���A���t�@�N�^�����O�̕��@����
	std::vector<std::pair<std::string,int>> bonus;
	//��{�X�R�A
	std::pair<std::string,int> turnScore;//�^�[����
	std::pair<std::string,int> liveScore;//������
	//�悭�g��������I�ȃ��O�f�[�^
	const FinishLog * finishLog=nullptr;
	const InitLog *initLog=nullptr;
	if(!m_logList.empty()){
		const LogElement *ptr=m_logList.at(0).get();
		initLog=dynamic_cast<const InitLog *>(ptr);
		ptr=m_logList.back().get();
		finishLog=dynamic_cast<const FinishLog *>(ptr);
	}
	if(initLog==nullptr || finishLog==nullptr){
		//���O�̎��������������A�G���[
		assert(false);
		return "";
	}
	//���ʂ��ėp������f�[�^
	size_t totalPlayerUnitCount=0,livePlayerUnitCount=0;//�v���C���[���j�b�g�̍��v�Ɛ�����
	const auto judgeExistProfession=[](const LogElement::UnitLogData &logData,Unit::Profession::Kind profession){
		return (logData.punit!=nullptr
			&& logData.punit->GetBaseStatus().profession==profession
			&& logData.punit->GetBattleStatus().team==Unit::Team::e_player);
	};
	//�N���A�^�[����
	{
		//�N���A�^�[��������̓��_�v�Z
		const int clearTurn=finishLog->GetClearTurn();
		turnScore.first="�N���A�^�[�����F "+std::to_string(clearTurn);
		turnScore.second=std::max(0,20-clearTurn)*500;
		//�N���A�^�[�����̃{�[�i�X���_
		if(clearTurn<5){
			bonus.push_back(std::make_pair("�_���i�R",3000));
		} else if(clearTurn<7){
			bonus.push_back(std::make_pair("���U�i�R",2000));
		} else if(clearTurn>15){
			bonus.push_back(std::make_pair("�m���m���i�R",100));
		}
	}
	//������
	{
		//����������̓��_�v�Z(�Q�Ԃ��z�肵�Ă��Ȃ�)
		size_t totalPlayerUnitCount=0;
		for(const LogElement::UnitLogData &unitData:initLog->m_unitDataList){
			if(unitData.punit!=nullptr && unitData.punit->GetBattleStatus().team==Unit::Team::e_player){
				totalPlayerUnitCount++;
			}
		}
		size_t livePlayerUnitCount=0;
		for(const LogElement::UnitLogData &unitData:finishLog->m_unitDataList){
			if(unitData.punit!=nullptr && unitData.punit->GetBattleStatus().team==Unit::Team::e_player){
				livePlayerUnitCount++;
			}
		}
		liveScore.first="�������F "+std::to_string(livePlayerUnitCount)+" / "+std::to_string(totalPlayerUnitCount);
		if(totalPlayerUnitCount>0){
			liveScore.second=10000*livePlayerUnitCount/totalPlayerUnitCount;
		} else{
			liveScore.second=0;
		}
		//�������̃{�[�i�X���_
		if(totalPlayerUnitCount-livePlayerUnitCount==0){
			bonus.push_back(std::make_pair("�S������",3000));
		} else if(livePlayerUnitCount==1){
			bonus.push_back(std::make_pair("�قڈ�������",200));
		}
	}
	//��_���[�W%����
	{
		std::map<const Unit *,double> minRateMap;//���j�b�g�ƍŏ�HP�������L�^����
		for(const std::shared_ptr<LogElement> logData:m_logList){
			for(const LogElement::UnitLogData &unitData:logData->m_unitDataList){
				if(unitData.punit!=nullptr && unitData.punit->GetBattleStatus().team==Unit::Team::e_player){
					const double hpRate=1.0*unitData.hp/unitData.punit->GetBaseStatus().maxHP;
					std::map<const Unit *,double>::iterator it=minRateMap.find(unitData.punit);
					if(it!=minRateMap.end()){
						//���Ɍ��������Ƃ̂��郆�j�b�g�ł���΁A�ŏ�HP�����̍X�V����
						if(hpRate<it->second){
							it->second=hpRate;
						}
					} else{
						//�܂��L�^���������Ƃ��Ȃ��Ȃ�A�ǉ�
						minRateMap.insert(std::pair<const Unit *,double>(unitData.punit,hpRate));
					}
				}
			}
		}
		if(totalPlayerUnitCount-livePlayerUnitCount==0){
			//�S���������̂ݕ]�����ڂ̌v�Z
			double minMinRate=1.0,maxMinRate=0.0;//�S�������j�b�g�̍ŏ�HP�����ɂ��čŏ��l�ƍő�l�����߂�
			for(const std::pair<const Unit *,double> &pair:minRateMap){
				if(minMinRate<pair.second){
					minMinRate=pair.second;
				}
				if(maxMinRate>pair.second){
					maxMinRate=pair.second;
				}
			}
			//�ŏ�HP���������܂�΁A����ɉ������{�[�i�X���o��
			if(minMinRate>=1.00){
				bonus.push_back(std::make_pair("�m�[�_���[�W",5000));
			} else if(minMinRate>=0.75){
				bonus.push_back(std::make_pair("HP75%�L�[�v",2000));
			} else if(minMinRate>=0.50){
				bonus.push_back(std::make_pair("HP50%�L�[�v",1000));
			} else if(maxMinRate<=0.25){
				bonus.push_back(std::make_pair("�M���M���̐킢",500));
			}
		}
	}
	//���m�֘A
	{
		size_t moveCount=0;//��Ԃ�������
		float totalWalkOP=0.0f;//�ړ��Ɏg�������vOP
		int attackCount=0;//�U����
		for(const std::shared_ptr<const LogElement> logData:m_logList){
			const LogElement::UnitLogData operateData=logData->GetOperateUnitData();
			if(operateData.punit!=nullptr
				&& operateData.punit->GetBattleStatus().team==Unit::Team::e_player
				&& operateData.punit->GetBaseStatus().profession==Unit::Profession::e_soldier)
			{
				switch(logData->GetLogKind()){
				case(LogElement::LogKind::e_attack):
					attackCount++;
				case(LogElement::LogKind::e_wait):
					moveCount++;
					//���O�o�͂̌�ɍs������OP����s����̂ŁA�ҋ@�ƍU���̂ǂ�������������ŗǂ�
					totalWalkOP+=Unit::BattleStatus::maxOP-operateData.op;
					break;
				}
			}
		}
		const auto judgeExistSoldier=[&judgeExistProfession](const LogElement::UnitLogData &logData){
			return judgeExistProfession(logData,Unit::Profession::e_soldier);
		};
		if(initLog->JudgeEveryUnitData(judgeExistSoldier,false)){
			//�����ɕ��m������ꍇ�̂݃{�[�i�X�ǉ�����������
			//���ψړ�OP
			if(moveCount>0){
				double averageWalkOP=totalWalkOP/moveCount;
				if(averageWalkOP>80.0){
					bonus.push_back(std::make_pair("�V���̃v���t�F�b�V���i��",1500));
				} else if(averageWalkOP>60.0){
					bonus.push_back(std::make_pair("�V�����m",1000));
				}
			}
			//�U����
			if(attackCount>=finishLog->GetClearTurn()){
				bonus.push_back(std::make_pair("�����̃A�^�b�J�[",2000));
			}
		}
	}
	//�d�����֘A
	{
		size_t magicAttackedCount=0;//���@�ŏd�����ɍU�����ꂽ��
		size_t magicTotalCount=0;//�����S�̂Ŗ��@�ōU�����ꂽ��
		size_t physicAttackedCount=0;//�����ŏd�����ɍU�����ꂽ��
		size_t physicTotalCount=0;//�����S�̂ŕ����ōU�����ꂽ��
		for(const std::shared_ptr<const LogElement> logData:m_logList){
			if(logData->GetLogKind()==LogElement::LogKind::e_attack){
				const std::shared_ptr<const AttackLog> attackLog=std::dynamic_pointer_cast<const AttackLog>(logData);
				if(attackLog.get()!=nullptr
					&& attackLog->GetOperateUnitData().punit!=nullptr
					&& attackLog->GetAimedUnit()!=nullptr
					&& attackLog->GetAimedUnit()->GetBattleStatus().team==Unit::Team::e_player)
				{
					switch(attackLog->GetOperateUnitData().punit->GetBattleStatus().weapon->GetActionKind()){
					case(WeaponActionKind::e_physicalAttack):
						physicTotalCount++;
						if(attackLog->GetAimedUnit()->GetBaseStatus().profession==Unit::Profession::e_armer){
							physicAttackedCount++;
						}
						break;
					case(WeaponActionKind::e_magicAttack):
						magicTotalCount++;
						if(attackLog->GetAimedUnit()->GetBaseStatus().profession==Unit::Profession::e_armer){
							magicAttackedCount++;
						}
						break;
					}
				}
			}
		}
		const auto judgeExistArmer=[&judgeExistProfession](const LogElement::UnitLogData &logData){
			return judgeExistProfession(logData,Unit::Profession::e_armer);
		};
		if(initLog->JudgeEveryUnitData(judgeExistArmer,false)){
			//�����ɏd����������ꍇ�̂݃{�[�i�X����������
			//���@��e��
			if(magicAttackedCount==0){
				bonus.push_back(std::make_pair("�d�����Ŗ��@�U�����󂯂Ȃ�",2000));
			} else if(magicAttackedCount==magicTotalCount){
				bonus.push_back(std::make_pair("�d��������_��˂��ꑱ����",100));
			}
			//������e��
			if(physicTotalCount>0){
				double rate=1.0*physicAttackedCount/physicTotalCount;
				if(rate>=0.75){
					bonus.push_back(std::make_pair("�����S��",1500));
				} else if(rate>=0.50){
					bonus.push_back(std::make_pair("������",1000));
				}
			}
		}
	}
	//�ˎ�֘A
	{
		size_t attackCount=0;//���v�U����
		size_t effectiveAttackCount=0;//���ʓI�ȍU���̉񐔁i�ˎ�E�����m�E�q�����_���j
		float totalAttackLengthRate=0.0f;//�U���˒������̑��a
		const auto judgeEffective=[](const Unit *punit){
			if(punit!=nullptr){
				switch(punit->GetBaseStatus().profession){
				case(Unit::Profession::e_archer):
				case(Unit::Profession::e_mage):
				case(Unit::Profession::e_healer):
					return true;
				}
			}
			return false;
		};
		for(const std::shared_ptr<LogElement> logData:m_logList){
			if(logData->GetLogKind()==LogElement::LogKind::e_attack){
				const std::shared_ptr<const AttackLog> attackLog=std::dynamic_pointer_cast<const AttackLog>(logData);
				if(attackLog.get()!=nullptr
					&& attackLog->GetOperateUnitData().punit!=nullptr
					&& attackLog->GetOperateUnitData().punit->GetBattleStatus().team==Unit::Team::e_player
					&& attackLog->GetOperateUnitData().punit->GetBaseStatus().profession==Unit::Profession::e_archer
					&& attackLog->GetAimedUnit()!=nullptr)
				{
					attackCount++;
					//���ʓI�ȍU���ł����effectiveAttackCount�����Z
					if(judgeEffective(attackLog->GetAimedUnit())){
						effectiveAttackCount++;
					}
					//�U���˒������𒲂ׂ�
					const Vector2D vec=attackLog->GetOperateUnitData().punit->getPos()-attackLog->GetAimedUnit()->getPos();
					const float maxLength=attackLog->GetOperateUnitData().punit->GetBattleStatus().weapon->GetLength();
					totalAttackLengthRate+=(vec.size()-Unit::unitCircleSize)/maxLength;
				}
			}
		}
		const auto judgeExistArcher=[&judgeExistProfession](const LogElement::UnitLogData &logData){
			return judgeExistProfession(logData,Unit::Profession::e_archer);
		};
		if(initLog->JudgeEveryUnitData(judgeExistArcher,false)){
			//�A�[�`���[�����R�ɂ���΃{�[�i�X�v�Z
			//�U���˒�
			if(attackCount>0){
				const double averageRate=totalAttackLengthRate/attackCount;
				if(averageRate>=0.80){
					bonus.push_back(std::make_pair("�_����",2000));
				} else if(averageRate>=0.60){
					bonus.push_back(std::make_pair("��q",1000));
				}
			}
			//�_������
			size_t effectiveUnitCount=0;//���ʓI�ȍU������̐�
			for(const LogElement::UnitLogData &logData:initLog->m_unitDataList){
				if(judgeEffective(logData.punit)){
					effectiveUnitCount++;
				}
			}
			if(effectiveAttackCount>0 && effectiveAttackCount>=effectiveUnitCount){
				//���ʓI�ȍU������ɂP�񂸂U�����Ă���Ȃ�i�������A�����ɂ͂����񐔂̍��v�����݂�j
				bonus.push_back(std::make_pair("��_�_���̎ˎ�",1500));
			}
		}
	}
	//�����m�֘A
	{
		size_t armerAttackCount=0;//�d�����U����
		size_t nearAttackedCount=0;//�ߐڍU����e��
		for(const std::shared_ptr<const LogElement> logData:m_logList){
			if(logData->GetLogKind()==LogElement::LogKind::e_attack){
				const std::shared_ptr<const AttackLog> attackLog=std::dynamic_pointer_cast<const AttackLog>(logData);
				if(attackLog.get()!=nullptr
					&& attackLog->GetOperateUnitData().punit!=nullptr
					&& attackLog->GetAimedUnit()!=nullptr)
				{
					//�d�����ւ̍U��
					if(attackLog->GetOperateUnitData().punit->GetBattleStatus().team==Unit::Team::e_player
						&& attackLog->GetOperateUnitData().punit->GetBaseStatus().profession==Unit::Profession::e_mage
						&& attackLog->GetAimedUnit()->GetBattleStatus().team==Unit::Team::e_enemy
						&& attackLog->GetAimedUnit()->GetBaseStatus().profession==Unit::Profession::e_armer)
					{
						armerAttackCount++;
					}
					//�ߐڍU���̔�e
					if(attackLog->GetOperateUnitData().punit->GetBattleStatus().team==Unit::Team::e_enemy
						&& attackLog->GetAimedUnit()->GetBattleStatus().team==Unit::Team::e_player
						&& attackLog->GetAimedUnit()->GetBaseStatus().profession==Unit::Profession::e_mage)
					{
						switch(attackLog->GetOperateUnitData().punit->GetBaseStatus().profession){
						case(Unit::Profession::e_soldier):
						case(Unit::Profession::e_armer):
							nearAttackedCount++;
							break;
						}
					}
				}
			}
		}
		const auto judgeExistMage=[&judgeExistProfession](const LogElement::UnitLogData &logData){
			return judgeExistProfession(logData,Unit::Profession::e_mage);
		};
		if(initLog->JudgeEveryUnitData(judgeExistMage,false)){
			//�����m�����R�ɂ���ꍇ�̓{�[�i�X����������
			//�d�����ւ̍U����
			size_t armerCount=0;
			for(const LogElement::UnitLogData &unitData:initLog->m_unitDataList){
				if(unitData.punit!=nullptr && unitData.punit->GetBaseStatus().profession==Unit::Profession::e_armer){
					armerCount++;
				}
			}
			if(armerCount>0){
				const double average=1.0*armerAttackCount/armerCount;
				if(average>=1.5){
					bonus.push_back(std::make_pair("�d�����L���[",2000));
				} else if(average>=1.0){
					bonus.push_back(std::make_pair("�����m�ŏd�����_��",1000));
				}
			}
			//�ߐڍU����e��
			if(nearAttackedCount<=0){
				bonus.push_back(std::make_pair("�����m���ߐڍU�����󂯂Ȃ�",1500));
			}
		}
	}
	//�q�����֘A
	{
		size_t physicAttackedCount=0;//�����U����e��
		int totalRecoverPoint=0;//���v�񕜗�
		int totalDamage=0;//���v��_���[�W
		for(const std::shared_ptr<const LogElement> logData:m_logList){
			if(logData->GetLogKind()==LogElement::LogKind::e_attack){
				const std::shared_ptr<const AttackLog> attackLog=std::dynamic_pointer_cast<const AttackLog>(logData);
				if(attackLog.get()!=nullptr
					&& attackLog->GetOperateUnitData().punit!=nullptr
					&& attackLog->GetAimedUnit()!=nullptr)
				{
					//�����U����e
					if(attackLog->GetOperateUnitData().punit->GetBattleStatus().team==Unit::Team::e_enemy
						&& attackLog->GetOperateUnitData().punit->GetBattleStatus().weapon->GetActionKind()==WeaponActionKind::e_physicalAttack
						&& attackLog->GetAimedUnit()->GetBattleStatus().team==Unit::Team::e_player
						&& attackLog->GetAimedUnit()->GetBaseStatus().profession==Unit::Profession::e_healer)
					{
						physicAttackedCount++;
					}
					//�񕜗�
					if(attackLog->GetOperateUnitData().punit->GetBattleStatus().team==Unit::Team::e_player
						&& attackLog->GetOperateUnitData().punit->GetBaseStatus().profession==Unit::Profession::e_healer
						&& attackLog->GetOperateUnitData().punit->GetBattleStatus().weapon->GetActionKind()==WeaponActionKind::e_recover
						&& attackLog->GetAimedUnit()->GetBattleStatus().team==Unit::Team::e_enemy)
					{
						//�񕜗ʂ͕��̒l�ŕ\�������
						totalRecoverPoint+=-(attackLog->GetAttackInfo().damage);
					}
					//��_���[�W
					if(attackLog->GetOperateUnitData().punit->GetBattleStatus().team==Unit::Team::e_enemy
						&& attackLog->GetAimedUnit()->GetBattleStatus().team==Unit::Team::e_player)
					{
						switch(attackLog->GetOperateUnitData().punit->GetBattleStatus().weapon->GetActionKind()){
						case(WeaponActionKind::e_physicalAttack):
						case(WeaponActionKind::e_magicAttack):
							totalDamage+=attackLog->GetAttackInfo().damage;
							break;
						}
					}
				}
			}
		}
		const auto judgeExistHealer=[&judgeExistProfession](const LogElement::UnitLogData &logData){
			return judgeExistProfession(logData,Unit::Profession::e_healer);
		};
		if(initLog->JudgeEveryUnitData(judgeExistHealer,false)){
			//�����ɉq����������ꍇ�A�{�[�i�X����������
			//�����U����e��
			if(physicAttackedCount==0){
				bonus.push_back(std::make_pair("�q�����������U�����󂯂Ȃ�",2000));
			}
			//�񕜗�
			if(totalDamage>0){
				const double rate=1.0*totalRecoverPoint/totalDamage;
				if(rate>=0.75){
					bonus.push_back(std::make_pair("�񕜂̃X�y�V�����X�g",1500));
				} else if(rate>=0.50){
					bonus.push_back(std::make_pair("�񕜖�",1000));
				}
			}
		}
	}

	//�{�[�i�X��S�Čv�Z������A�X�R�A�v�Z
	int score=turnScore.second+liveScore.second;
	for(const std::pair<std::string,int> &pair:bonus){
		score+=pair.second;
	}
	//�������̕\��
	std::string explain;
	explain+="�g�[�^���X�R�A�F"+std::to_string(score)+"\n\n";
	explain+=turnScore.first+"�@�@�@"+std::to_string(turnScore.second)+"\n";
	explain+=liveScore.first+"�@�@�@"+std::to_string(liveScore.second)+"\n\n";
	explain+="BONUS SCORE\n";
	for(const std::pair<std::string,int> &pair:bonus){
		explain+=pair.first+"�@�@�@"+std::to_string(pair.second)+"\n";
	}

	return explain;
}

ScoreObserver::ScoreObserver()
	:m_researchCount(0)
	,m_cancelCount(0)
	,m_logList()
{}

ScoreObserver::~ScoreObserver(){}