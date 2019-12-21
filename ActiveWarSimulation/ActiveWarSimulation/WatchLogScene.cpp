#include"DxLib.h"
#include"input.h"
#include"WatchLogScene.h"
#include"ToolsLib.h"
#include"FileRead.h"
#include"StringBuilder.h"
#include<map>
#include<optional>
#include"TitleScene.h"
//���O�ꗗ
#include"AttackLog.h"
#include"WaitLog.h"
#include"InitLog.h"
#include"FinishLog.h"

//---------------------WatchLogScene-----------------
WatchLogScene::WatchLogScene(const std::string &logFileName)
	:m_logIndex(0)
{
	//�t�@�C����ǂݍ��݁A�����񕪊�
	const StringBuilder sb(FileStrRead(logFileName.c_str()),',','(',')');
	//����
	std::string stageDirName;
	StageLevel stageLevel;
	const StringBuilder *logSb=nullptr;
	for(const StringBuilder &ssb:sb.m_vec){
		if(ssb.m_vec.size()>=2 && ssb.m_vec[0].GetString()=="stageDirName"){
			stageDirName=ssb.m_vec[1].GetString();
		}
		if(ssb.m_vec.size()>=2 && ssb.m_vec[0].GetString()=="stageLevel"){
			stageLevel=StageLevel::CreateFromString(ssb.m_vec[1].GetString());
		}
		if(ssb.m_vec.size()>=2 && ssb.m_vec[0].GetString()=="log"){
			logSb=&ssb;
		}
	}
	//�X�e�[�W�\�z
	m_battleSceneData=std::shared_ptr<BattleSceneData>(new BattleSceneData(stageDirName,"",stageLevel));
	//���O�\�z
	if(logSb!=nullptr){
		//���O�ɂ�����index��punit�̏ƍ�
		std::map<int,Unit *> indexToUnitPtr;
		for(const StringBuilder &ssb:logSb->m_vec[1].m_vec){
			if(!ssb.m_vec.empty() && ssb.m_vec[0].GetString()=="unitList"){
				for(size_t i=1,siz=ssb.m_vec.size();i<siz;i++){
					//���j�b�g���̈����o��
					std::optional<int> index;
					std::optional<Vector2D> pos;
					for(const StringBuilder &sssb:ssb.m_vec[i].m_vec){
						if(sssb.m_vec.size()>=2 && sssb.m_vec[0].GetString()=="index"){
							index.emplace(std::strtol(sssb.m_vec[1].GetString().c_str(),nullptr,10));
						}
						if(sssb.m_vec.size()>=3 && sssb.m_vec[0].GetString()=="pos"){
							Vector2D p;
							p.x=std::strtof(sssb.m_vec[1].GetString().c_str(),nullptr);
							p.y=std::strtof(sssb.m_vec[2].GetString().c_str(),nullptr);
							pos.emplace(p);
						}
					}
					//�Y�����j�b�g��T��
					if(index.has_value() && pos.has_value()){
						for(Unit *pu:m_battleSceneData->m_unitList){
							if(pu->getPos()==pos.value()){
								indexToUnitPtr.insert(std::pair<int,Unit *>(index.value(),pu));
							}
						}
					}
				}
			}
		}
		//�e���O�̍\�z
		for(size_t logIndex=1,logSize=logSb->m_vec.size();logIndex<logSize;logIndex++){
			//���O�̎�ނ̌���ƁA�S�Ă̎�ނ̃��O�ŋ��ʂ�unitList�̎��o�����s��
			std::string logKind;
			std::vector<LogElement::UnitLogData> unitList;
			for(const StringBuilder &ssb:logSb->m_vec[logIndex].m_vec){
				if(ssb.m_vec.size()>=2 && ssb.m_vec[0].GetString()=="kind"){
					logKind=ssb.m_vec[1].GetString();
				}
				if(!ssb.m_vec.empty() && ssb.m_vec[0].GetString()=="unitList"){
					for(size_t unitIndex=1,unitSize=ssb.m_vec.size();unitIndex<unitSize;unitIndex++){
						//�v�f�𒊏o
						std::optional<int> index;
						std::optional<int> hp;
						std::optional<float> op;
						std::optional<Vector2D> pos;
						for(const StringBuilder &dataSb:ssb.m_vec[unitIndex].m_vec){
							if(dataSb.m_vec.size()>=2 && dataSb.m_vec[0].GetString()=="index"){
								index.emplace(std::strtol(dataSb.m_vec[1].GetString().c_str(),nullptr,10));
							}
							if(dataSb.m_vec.size()>=2 && dataSb.m_vec[0].GetString()=="hp"){
								hp.emplace(std::strtol(dataSb.m_vec[1].GetString().c_str(),nullptr,10));
							}
							if(dataSb.m_vec.size()>=2 && dataSb.m_vec[0].GetString()=="op"){
								op.emplace(std::strtof(dataSb.m_vec[1].GetString().c_str(),nullptr));
							}
							if(dataSb.m_vec.size()>=3 && dataSb.m_vec[0].GetString()=="pos"){
								Vector2D p;
								p.x=std::strtof(dataSb.m_vec[1].GetString().c_str(),nullptr);
								p.y=std::strtof(dataSb.m_vec[2].GetString().c_str(),nullptr);
								pos.emplace(p);
							}
						}
						//unitList�̊i�[
						if(index.has_value() && hp.has_value() && op.has_value() && pos.has_value()){
							unitList.push_back(LogElement::UnitLogData(hp.value(),op.value(),pos.value(),indexToUnitPtr.find(index.value())->second));
						}
					}
				}
			}
			//�e���O�̍쐬
			if(logKind=="AttackLog"){
				//attackLog�̂݁A�_��ꂽ���j�b�g�������o��
				const Unit *aimedUnit=nullptr;
				for(const StringBuilder &ssb:logSb->m_vec[logIndex].m_vec){
					if(ssb.m_vec.size()>=2 && ssb.m_vec[0].GetString()=="aimed"){
						aimedUnit=indexToUnitPtr.find(std::strtol(ssb.m_vec[1].GetString().c_str(),nullptr,10))->second;
					}
				}
				if(aimedUnit!=nullptr){
					m_logList.push_back(std::shared_ptr<LogElement>(new AttackLog(unitList,aimedUnit,std::vector<RouteInfo>{})));
				}
			} else if(logKind=="WaitLog"){
				m_logList.push_back(std::shared_ptr<LogElement>(new WaitLog(unitList,std::vector<RouteInfo>{})));
			} else if(logKind=="InitLog"){
				m_logList.push_back(std::shared_ptr<LogElement>(new InitLog(unitList)));
			} else if(logKind=="FinishLog"){
				m_logList.push_back(std::shared_ptr<LogElement>(new FinishLog(unitList,0)));
			}
		}
	}
	//���O��m_battleSceneData�ɓK�p����
	AdaptLog();
}

WatchLogScene::~WatchLogScene(){}

void WatchLogScene::InitCompletely(){}

void WatchLogScene::Activate(){}

int WatchLogScene::Calculate(){
	//��ʂ̑I��
	if(m_logIndex>0 && (keyboard_get(KEY_INPUT_LEFT)==1 || (keyboard_get(KEY_INPUT_LEFT)>30 && keyboard_get(KEY_INPUT_LEFT)%5==0))){
		m_logIndex--;
		AdaptLog();
	} else if(m_logIndex+1<m_logList.size() && (keyboard_get(KEY_INPUT_RIGHT)==1 || (keyboard_get(KEY_INPUT_RIGHT)>30 && keyboard_get(KEY_INPUT_RIGHT)%5==0))){
		m_logIndex++;
		AdaptLog();
	}
	//�J��
	if(keyboard_get(KEY_INPUT_X)==1 || mouse_get(MOUSE_INPUT_RIGHT)==1){
		return 1;
	}

	return 0;
}

void WatchLogScene::Draw()const{
	//�w�i�̕`��
	m_battleSceneData->DrawField();
	//���j�b�g�̕`��
	m_battleSceneData->DrawUnit(true);
	//HP�Q�[�W�̕`��
	m_battleSceneData->DrawHPGage();
	//���Ԃ̕`��
	m_battleSceneData->DrawOrder();
}

std::shared_ptr<GameScene> WatchLogScene::VGetNextScene(const std::shared_ptr<GameScene> &thisSharedPtr)const{
	//�^�C�g����ʂ�
	return CreateFadeOutInSceneCompletely(thisSharedPtr,std::make_shared<TitleScene::TitleSceneFactory>(),15,15);
}

void WatchLogScene::AdaptLog(){
	//���샆�j�b�g�̕ύX
	for(BattleObject *obj:m_battleSceneData->m_field){
		if(obj->GetType()==BattleObject::Type::e_unit && obj==m_logList[m_logIndex]->m_unitDataList.begin()->punit){
			Unit *pu=dynamic_cast<Unit *>(obj);
			if(pu!=nullptr){
				m_battleSceneData->m_operateUnit=pu;
			}
			break;
		}
	}
	//���j�b�g�̏�Ԃ�K�p����
	for(BattleObject *ob:m_battleSceneData->m_field){
		if(ob->GetType()==BattleObject::Type::e_unit){
			Unit * const pu=dynamic_cast<Unit *>(ob);
			if(pu!=nullptr){
				//pu�����O�ɑ��݂��邩�𒲂ׂ�
				bool logExist=false;
				for(const LogElement::UnitLogData &data:m_logList[m_logIndex]->m_unitDataList){
					if(pu==data.punit){
						logExist=true;
						//���݂����ꍇ�A���K�p����
						pu->SetFix(Shape::Fix::e_static);//�������Ă���̂�static��Ԃɂ���
						pu->SetOP(data.op);
						pu->AddHP(data.hp-pu->GetBattleStatus().HP);
						pu->Warp(data.pos);
						pu->SetPenetratable(m_battleSceneData->m_operateUnit->GetBattleStatus().team);
						break;
					}
				}
				if(!logExist){
					//���O�����݂��Ȃ������ꍇ�́A���j�b�g���}�b�v��ɏo���Ȃ�
					pu->SetFix(Shape::Fix::e_ignore);
				}
			}
		}
	}
	//m_unitList�̏��Ԃ𐮂���
	m_battleSceneData->m_unitList.clear();
	for(const LogElement::UnitLogData &data:m_logList[m_logIndex]->m_unitDataList){
		//m_battleSceneData->m_unitList.push_back(data.punit);//�{���͂������������ǂ��Adata.punit��const�Ȃ̂Ŋi�[�ł��Ȃ�
		//data.punit��m_battleSceneData->m_field����T��
		for(BattleObject *obj:m_battleSceneData->m_field){
			if(obj->GetType()==BattleObject::Type::e_unit && obj==data.punit){
				Unit *pu=dynamic_cast<Unit *>(obj);
				if(pu!=nullptr){
					m_battleSceneData->m_unitList.push_back(pu);
				}
				break;
			}
		}
	}
}
