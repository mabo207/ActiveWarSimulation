#include"ResearchScene.h"
#include"DxLib.h"
#include"GraphicControl.h"
#include"CommonConstParameter.h"
#include"FilePath.h"

namespace {
	const int paramBoxInitLeftX=-600;
	const int paramBoxInitRightX=CommonConstParameter::gameResolutionX+10;
	const int paramBoxAimedLeftX=120;
	const int paramBoxAimedRightX=1200;
	const int paramBoxY=140;
}

//-------------------ResearchScene---------------------
const int ResearchScene::explainFontSize=16;

ResearchScene::ResearchScene(std::shared_ptr<BattleSceneData> battleSceneData)
	:BattleSceneElement(SceneKind::e_research)
	,m_palFont(CreateFontToHandleEX("���C���I",24,1,-1))
	,m_nameFont(LoadFontDataToHandleEX(FilePath::fontDir+"ExplainGothicFont_Large.dft",0))
	,m_explainFont(LoadFontDataToHandleEX(FilePath::fontDir+"ExplainGothicFont_Small.dft",0))
	,m_palBackPic(LoadGraphEX(FilePath::graphicDir+"researchInfoBack.png"))
	,m_battleSceneData(battleSceneData)
	,m_researchPic(LoadGraphEX(FilePath::graphicDir+"operatedCursor.png"))
	,m_mousePosJustBefore(GetMousePointVector2D())
	,m_pointerVec(battleSceneData->m_operateUnit->getPos())
	,m_moveButton(1620,980,80,80,LoadGraphEX(FilePath::graphicDir+"backButton.png"))
	,m_paramBoxX(paramBoxInitLeftX,10,Easing::TYPE_IN,Easing::FUNCTION_LINER,1.0)
	,m_researchUnit(nullptr)
{
	m_paramBoxX.EnforceEnd();//�������Ă��Ȃ���Ԃɂ���
}

ResearchScene::~ResearchScene(){
	DeleteFontToHandleEX(m_palFont);
	DeleteFontToHandleEX(m_nameFont);
	DeleteFontToHandleEX(m_explainFont);
	DeleteGraphEX(m_palBackPic);
	DeleteGraphEX(m_researchPic);
}

void ResearchScene::UpdatePointer(){
	//�ЂƂ܂��}�E�X���g���đ��삷��̂�z�肷��
	const Vector2D mouseVec=GetMousePointVector2D();
	if(keyboard_get(KEY_INPUT_A)==1 || keyboard_get(KEY_INPUT_S)==1){
		//A:�I�[�_�[�̖�������擪�֒��ׂ郆�j�b�g��i�߂�
		//S:�I�[�_�[�̐擪���疖���֒��ׂ郆�j�b�g��i�߂�
		//�����Ԗڂ̃��j�b�g�𒲂ׂĂ��邩�𔻒�
		size_t researchIndex=0;
		const size_t size=m_battleSceneData->m_unitList.size();
		for(;researchIndex<size;researchIndex++){
			if(m_battleSceneData->m_unitList[researchIndex]==m_researchUnit){
				break;
			}
		}
		//�I�[�_�[���Đi�߂�
		size_t index=researchIndex;//�i�߂���Bsize�Ȃ�I�[�_�[�̒N�ł��Ȃ����Ƃ��Ӗ�����B
		if(keyboard_get(KEY_INPUT_A)==1){
			index=(researchIndex+size)%(size+1);//���ǂ�
		} else if(keyboard_get(KEY_INPUT_S)==1){
			index=(researchIndex+1)%(size+1);//������
		}
		//�i�߂���̃L�����̈ʒu�܂Œ��ׂ�ꏊ�𓮂���
		if(index<size){
			m_pointerVec=m_battleSceneData->m_unitList[index]->getPos();
		} else{
			//�N�����ׂȂ��Ƃ�����ԂɂȂ����ꍇ��
			//���ɉ������Ȃ�
		}
	}else{
		//���j�b�g�؂�ւ���p���Ȃ��������́A�}�E�X�E�W���C�X�e�B�b�N�E�L�[���͂Œ��ׂ�ʒu���ׂ����ړ�
		if(JudgeMouseInWindow() && (mouseVec-m_mousePosJustBefore).sqSize()>1.0f){
			//�}�E�X�̈ړ����傫���Ȃ�΁A�}�E�X�̈ʒu�𒲍��ꏊ�Ƃ���
			//�}�E�X���E�C���h�E�ɓ����Ă��Ȃ����͍X�V���Ȃ��ق�������
			//�덷���e���傫������ƃ}�E�X�𓮂����Ă��J�[�\�����������C��������
			m_pointerVec=mouseVec;
		} else if(analogjoypad_get(DX_INPUT_PAD1)!=Vector2D()){
			//�W���C�X�e�B�b�N���������Ȃ�΁A������Q�l�ɒ����ꏊ���ړ�����
			Vector2D stick=analogjoypad_get(DX_INPUT_PAD1);
			//�A�i���O�X�e�B�b�N�̕����I�ȃY�����ɂ��stick�̔����͂�����
			const float gap=50.0f;
			if(std::abs(stick.x)<gap){
				stick.x=0.0f;
			}
			if(std::abs(stick.y)<gap){
				stick.y=0.0f;
			}
			m_pointerVec+=stick/64.0f;
		} else{
			//����ȊO�̓L�[����
			//�L�[�{�[�h���͂��Ă���Ȃ�A�����p����
			Vector2D moveVec=Vector2D();
			if(keyboard_get(KEY_INPUT_UP)>4){
				moveVec+=Vector2D(0.0f,-15.0f);
			} else if(keyboard_get(KEY_INPUT_UP)>0){
				moveVec+=Vector2D(0.0f,-1.0f);
			}
			if(keyboard_get(KEY_INPUT_LEFT)>4){
				moveVec+=Vector2D(-15.0f,0.0f);
			} else if(keyboard_get(KEY_INPUT_LEFT)>0){
				moveVec+=Vector2D(-1.0f,0.0f);
			}
			if(keyboard_get(KEY_INPUT_RIGHT)>4){
				moveVec+=Vector2D(15.0f,0.0f);
			} else if(keyboard_get(KEY_INPUT_RIGHT)>0){
				moveVec+=Vector2D(1.0f,0.0f);
			}
			if(keyboard_get(KEY_INPUT_DOWN)>4){
				moveVec+=Vector2D(0.0f,15.0f);
			} else if(keyboard_get(KEY_INPUT_DOWN)>0){
				moveVec+=Vector2D(0.0f,1.0f);
			}
			m_pointerVec+=moveVec;
		}
	}
	//�}�E�X�̒��O�ʒu�̍X�V
	m_mousePosJustBefore=mouseVec;
	//�|�C���^�[�̈ʒu�ɂ��郆�j�b�g��m_researchUnit�Ɋi�[����
	const Unit *beforeResearchUnit=m_researchUnit;
	m_researchUnit=m_battleSceneData->GetUnitPointer(m_pointerVec);
	if(m_researchUnit!=nullptr && m_researchUnit!=beforeResearchUnit){
		//���j�b�g���w�肵���u�Ԃł���ΐ؂�ւ��̌��ʉ���
		PlaySoundMem(m_battleSceneData->m_aimchangeSound,DX_PLAYTYPE_BACK,TRUE);
	}
}

int ResearchScene::thisCalculate(){
	const Unit *beforeFrameResearchUnit=m_researchUnit;//��r�ɗp����

	//�|�C���^�[�̍X�V
	UpdatePointer();

	//�p�����[�^�\��UI�̕`��ʒu�̍X�V
	m_paramBoxX.Update();
	if(m_researchUnit!=beforeFrameResearchUnit){
		//���\�����j�b�g���ύX���ꂽ��A�ڕW�ʒu��ς���
		//�����킹�ɂ��邩�ǂ���
		bool existLeft;//UI�̈ʒu�̊�������ǂ���
		bool enterFlag;//��ʂɓ���or�o��
		if(m_researchUnit!=nullptr){
			existLeft=((int)(m_researchUnit->getPos().x)>CommonConstParameter::gameResolutionX/2);
			enterFlag=true;
			if(m_paramBoxX.GetEndFlag() && beforeFrameResearchUnit==nullptr){
				//�O���ňړ��I�����Ă��鎞�́Am_paramBox�̏����ʒu���X�V
				m_paramBoxX=Easing(existLeft?paramBoxInitLeftX:paramBoxInitRightX,m_paramBoxX.GetMaxFrame(),m_paramBoxX.GetType(),m_paramBoxX.GetFunction(),m_paramBoxX.GetDegree());
			}
		} else{
			//beforeFrameResearchUnit��nullptr�łȂ�
			enterFlag=false;
			existLeft=((int)(beforeFrameResearchUnit->getPos().x)>CommonConstParameter::gameResolutionX/2);
		}
		//�ڕW�ʒu�̍X�V
		if(enterFlag){
			if(existLeft){
				//��ʍ��ɓ���
				m_paramBoxX.SetTarget(paramBoxAimedLeftX,true);
			} else{
				m_paramBoxX.SetTarget(paramBoxAimedRightX,true);
			}
		} else{
			if(existLeft){
				m_paramBoxX.SetTarget(paramBoxInitLeftX,true);
			} else{
				m_paramBoxX.SetTarget(paramBoxInitRightX,true);
			}
		}
	}

	//�J�ڏ���
	if(keyboard_get(KEY_INPUT_F)==1 || keyboard_get(KEY_INPUT_X)==1 || m_moveButton.JudgePressMoment()){
		return 0;//�}�b�v���׃��[�h�ؑփ{�^���܂��͖߂�{�^�����͂Œ��O��ʂ�
	}

	return SceneKind::e_research;
}

void ResearchScene::thisDraw()const{
	//�t�B�[���h�̕`��
	m_battleSceneData->DrawField();

	//���j�b�g�̕`��
	m_battleSceneData->DrawUnit(true,std::set<const Unit *>{m_battleSceneData->m_operateUnit,m_researchUnit});

	//���ׂĂ��郆�j�b�g�ɂ��Ă̕`��
	if(m_researchUnit!=nullptr){
		m_researchUnit->DrawUnit(Vector2D(),m_battleSceneData->m_fpsMesuring.GetFrame(),false,true,true);
		m_researchUnit->DrawMaxMoveInfo();//�ő�ړ�������\��
	}

	//���쒆���j�b�g�̕`��(m_researchUnit�Ƃ��Ԃ��Ă�����`�悷��K�v�͂Ȃ�)
	if(m_battleSceneData->m_operateUnit!=m_researchUnit){
		m_battleSceneData->m_operateUnit->BattleObject::VDraw();
		Vector2D pos=m_battleSceneData->m_operateUnit->getPos();
	}


	//�S���j�b�g��HP�Q�[�W�̕`��
	m_battleSceneData->DrawHPGage();

	//���j�b�g�̃I�[�_�[���Ԃ�`��
	m_battleSceneData->DrawOrder(std::set<const BattleObject*>{m_researchUnit});

	//�ړ��V�[���ɖ߂�{�^���̕`��
	m_moveButton.DrawButton();

	//���ׂĂ���ꏊ���ǂ�����`��
	{
		//���w�����Ă���ɋ���
		const Vector2D pos=m_pointerVec;
		float dx,dy;
		GetGraphSizeF(m_researchPic,&dx,&dy);
		DrawGraph((int)(pos.x-dx/2.0f),(int)(pos.y-dy),m_researchPic,TRUE);
	}

	//�p�����[�^�̕`��
	{
		//���n�̕`��
		const int paramBoxX=m_paramBoxX.GetX();
		DrawGraph(paramBoxX,paramBoxY,m_palBackPic,TRUE);
		if(m_researchUnit!=nullptr){
			const Unit::BaseStatus base=m_researchUnit->GetBaseStatus();
			const Unit::BattleStatus battle=m_researchUnit->GetBattleStatus();
			//�l�[���v���[�g����
			m_researchUnit->DrawFacePic(Vector2D((float)(paramBoxX+70),(float)(paramBoxY+60)));
			DrawStringToHandle(paramBoxX+150,paramBoxY+12,base.name.c_str(),GetColor(255,255,255),m_nameFont);
			DrawFormatStringToHandle(paramBoxX+180,paramBoxY+65,GetColor(255,255,255),m_palFont,"%s Lv%d",Unit::Profession::GetName(base.profession).c_str(),base.lv);
			//�p�����[�^����
			DrawFormatStringToHandle(paramBoxX+68,paramBoxY+159,GetColor(255,255,255),m_palFont,"%d/%d",battle.HP,base.maxHP);
			DrawFormatStringToHandle(paramBoxX+68,paramBoxY+203,GetColor(255,255,255),m_palFont,"%.0f/%.0f",battle.OP,battle.maxOP);
			DrawFormatStringToHandle(paramBoxX+94,paramBoxY+247,GetColor(255,255,255),m_palFont,"%d",base.power);
			DrawFormatStringToHandle(paramBoxX+94,paramBoxY+291,GetColor(255,255,255),m_palFont,"%d",base.def);
			DrawFormatStringToHandle(paramBoxX+234,paramBoxY+247,GetColor(255,255,255),m_palFont,"%d",base.mpower);
			DrawFormatStringToHandle(paramBoxX+234,paramBoxY+291,GetColor(255,255,255),m_palFont,"%d",base.mdef);
			DrawFormatStringToHandle(paramBoxX+94,paramBoxY+336,GetColor(255,255,255),m_palFont,"%d",base.move);
			//������������
			//�ʏ푕��
			DrawStringToHandle(paramBoxX+312,paramBoxY+159,m_researchUnit->GetBattleStatus().weapon->GetName().c_str(),GetColor(255,255,255),m_palFont);
			DrawStringNewLineToHandle(paramBoxX+317,paramBoxY+192,263,60,m_researchUnit->GetBattleStatus().weapon->GetEffectivenessString(m_researchUnit).c_str(),GetColor(255,255,255),m_explainFont,4);
			DrawStringNewLineToHandle(paramBoxX+317,paramBoxY+240,263,80,m_researchUnit->GetBattleStatus().weapon->GetExplain().c_str(),GetColor(255,255,255),m_explainFont,4);
		}
	}
}

int ResearchScene::UpdateNextScene(int index){
	//���ɉ������Ȃ�
	return index;
}

void ResearchScene::ReturnProcess(){
	//���ɉ������Ȃ�
}
