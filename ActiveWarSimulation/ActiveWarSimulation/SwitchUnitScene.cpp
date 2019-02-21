#include"DxLib.h"
#include"SwitchUnitScene.h"
#include"PlayerMoveScene.h"
#include"ComputerMoveScene.h"
#include"StageClearSceme.h"
#include"GraphicControl.h"

#include"TutorialPlayerMoveScene.h"

//------------------SwitchUnitScene-------------------
SwitchUnitScene::SwitchUnitScene(std::shared_ptr<BattleSceneData> battleSceneData)
	:BattleSceneElement(SceneKind::e_switch)
	,m_battleSceneData(battleSceneData)
	,m_judgeEnd(JudgeEnd::e_notEnd)
	,m_turnFont(CreateFontToHandleEX("���C���I",80,8,DX_FONTTYPE_ANTIALIASING_EDGE_4X4,-1,3))
{
	ReturnProcess();
	if(m_judgeEnd==JudgeEnd::e_notEnd){
		//�o�g���J�n����͂��Ȃ炸e_notEnd����e_notEnd_TurnChange�ɐ؂�ւ���
		//�������A�v���O�����̍\����ς��ă^�[���؂�ւ��̂��тɂ��̃R���X�g���N�^���Ă΂��悤�ɂȂ�Ɩ��ɂȂ��Ă��܂��̂ŁA�����𒼂������B
		m_judgeEnd=JudgeEnd::e_notEnd_TurnChange;
	}
}

SwitchUnitScene::~SwitchUnitScene(){
	DeleteFontToHandleEX(m_turnFont);
}

int SwitchUnitScene::CalculateTurn()const{
	return (int)(m_battleSceneData->m_totalOP/Unit::BattleStatus::maxOP)+1;
}

int SwitchUnitScene::thisCalculate(){
	//�^�C�}�[�X�V
	m_battleSceneData->m_fpsMesuring.Update();
	
	//���s����ɂ���āA�J�ڏ����͕���
	switch(m_judgeEnd){
	case(JudgeEnd::e_notEnd):
	case(JudgeEnd::e_notEnd_TurnChange):
		//���������Ă��Ȃ�
		//��莞�Ԍo�����烆�j�b�g�ړ���
		if(m_battleSceneData->m_fpsMesuring.GetProcessedTime()>1.0f){
			//���j�b�g�ڍs������
			m_battleSceneData->m_fpsMesuring.RecordTime();//�^�C�}�[������
			return SceneKind::e_move;
		}
		break;
	case(JudgeEnd::e_playerWin):
		//�v���C���[������
		return SceneKind::e_clear;
		break;
	case(JudgeEnd::e_playerLose):
		//�v���C���[���s�k
		return SceneKind::e_clear;
		break;
	}
	
	return SceneKind::e_switch;
}

void SwitchUnitScene::thisDraw()const{
	//�}�b�v�̕\��
	m_battleSceneData->DrawField();
	//���j�b�g�̕\��
	m_battleSceneData->DrawUnit(false,{});
	//HP�Q�[�W�̕`��
	m_battleSceneData->DrawHPGage();
	//�I�[�_�[�̕\���͕`�悵�Ȃ�

	//�^�[���؂�ւ��\��
	if(m_judgeEnd==JudgeEnd::e_notEnd_TurnChange){
		int mode,pal;
		GetDrawBlendMode(&mode,&pal);
		SetDrawBlendMode(DX_BLENDMODE_ALPHA,128);
		DrawBox(1020,750,1920,850,GetColor(0,0,0),TRUE);
		SetDrawBlendMode(mode,pal);
		DrawStringToHandle(1030,760,("BATTLE PHASE "+to_string_0d(CalculateTurn(),2)).c_str(),GetColor(255,255,255),m_turnFont,GetColor(0,0,0));
	}
}

int SwitchUnitScene::UpdateNextScene(int index){
	switch(index){
	case(SceneKind::e_move):
		if(m_battleSceneData->m_operateUnit->GetBattleStatus().team==Unit::Team::e_player){
			//�v���C���[�L�����Ȃ玩����N���X��
			if(m_battleSceneData->m_playMode!=BattleSceneData::PlayMode::e_tutorial){
				//�ʏ�o�g���ł���ꍇ��PlayerMoveScene��
				m_nextScene=std::shared_ptr<BattleSceneElement>(new PlayerMoveScene(m_battleSceneData));
			} else{
				//�`���[�g���A���ł���ꍇ��TutorialPlayerMoveScene��
				m_nextScene=std::shared_ptr<BattleSceneElement>(new TutorialPlayerMoveScene(m_battleSceneData));
			}
		} else{
			//����ȊO�Ȃ�AI����N���X��
			m_nextScene=std::shared_ptr<BattleSceneElement>(new ComputerMoveScene(m_battleSceneData));
		}
		return index;
	case(SceneKind::e_clear):
		if(m_judgeEnd==JudgeEnd::e_playerWin){
			//�v���C���[�̏���
			//�P�ސ��̋L�^
			int deathUnitNum=0;
			for(size_t i=0,size=m_battleSceneData->m_field.size();i<size;i++){
				if(m_battleSceneData->m_field[i]->GetType()==BattleObject::Type::e_unit){
					const Unit *punit=dynamic_cast<const Unit *>(m_battleSceneData->m_field[i]);
					if(!(punit->GetFix()==Shape::Fix::e_static
						|| punit->GetFix()==Shape::Fix::e_dynamic
						)
						)
					{
						//�}�b�v��ɑ��݂��Ă��郆�j�b�g�͐��ɓ���Ȃ�
						switch(punit->GetBattleStatus().team){
						case(Unit::Team::e_player):
							deathUnitNum++;
							break;
						default:
							break;
						}
					}
				}
			}
			//�N���A�^�[�����̋L�^
			int clearTurn=(int)(m_battleSceneData->m_totalOP/Unit::BattleStatus::maxOP)+1;//0~150.0f��1�^�[���Ȃ̂�+1����
			//nextScene�̍쐬
			const std::string str=
				"�P�ސ��F"+std::to_string(deathUnitNum)+"�l\n"
				+"�N���A�^�[�����F"+std::to_string(clearTurn);
			m_nextScene=std::shared_ptr<BattleSceneElement>(new StageClearScene(m_battleSceneData,true,str));
		} else if(m_judgeEnd==JudgeEnd::e_playerLose){
			//�v���C���[�̔s�k
			int killUnitNum=0,totalUnitNum=0;
			for(size_t i=0,size=m_battleSceneData->m_field.size();i<size;i++){
				if(m_battleSceneData->m_field[i]->GetType()==BattleObject::Type::e_unit){
					const Unit *punit=dynamic_cast<const Unit *>(m_battleSceneData->m_field[i]);
					switch(punit->GetBattleStatus().team){
					case(Unit::Team::e_enemy):
						totalUnitNum++;
						if(!(punit->GetFix()==Shape::Fix::e_static
							|| punit->GetFix()==Shape::Fix::e_dynamic
							)
							)
						{
							//�}�b�v��ɑ��݂��Ă��郆�j�b�g�͐��ɓ���Ȃ�
							killUnitNum++;
						}
						break;
					default:
						break;
					}
				}
			}
			m_nextScene=std::shared_ptr<BattleSceneElement>(new StageClearScene(m_battleSceneData,false,"���j���F"+std::to_string(killUnitNum)+"�l / "+std::to_string(totalUnitNum)+"�l"));
		}
		return index;
	default:
		return index;
	}
}

void SwitchUnitScene::ReturnProcess(){
	m_battleSceneData->m_fpsMesuring.RecordTime();//�^�C�}�[������
	//���s����
	size_t playerUnitNum=0,enemyUnitNum=0;
	for(size_t i=0,size=m_battleSceneData->m_unitList.size();i<size;i++){
		switch(m_battleSceneData->m_unitList[i]->GetBattleStatus().team){
		case(Unit::Team::e_player):
			playerUnitNum++;
			break;
		case(Unit::Team::e_enemy):
			enemyUnitNum++;
			break;
		default:
			break;
		}
	}
	if(playerUnitNum==0){
		//�v���C���[�L������0�Ȃ畉��
		m_judgeEnd=JudgeEnd::e_playerLose;
	} else if(enemyUnitNum==0){
		//�G�L������0�Ȃ珟��
		m_judgeEnd=JudgeEnd::e_playerWin;
	} else{
		//�ǂ�ł��Ȃ��Ȃ�퓬���s
		const int beforeTurn=CalculateTurn();
		m_battleSceneData->FinishUnitOperation();//���j�b�g�̐؂�ւ�����(�퓬���s�ȊO�̎��͂��Ȃ��A�N���A�^�[�����̌v�Z���]���ɉ��Z���ꂩ�˂Ȃ�����)
		if(CalculateTurn()!=beforeTurn){
			//�؂�ւ������̑O��Ń^�[�������ς���Ă�����A�^�[���؂�ւ���ʂ�\������悤�ɂ���
			m_judgeEnd=JudgeEnd::e_notEnd_TurnChange;
		} else{
			//�؂�ւ������̑O��Ń^�[�������ς���Ă�����A�^�[���؂�ւ��\���͂��Ȃ�
			m_judgeEnd=JudgeEnd::e_notEnd;
		}
	}
}
