#include"ResearchScene.h"
#include"DxLib.h"
#include"input.h"
#include"GraphicControl.h"

//-------------------ResearchScene---------------------
const int ResearchScene::explainFontSize=16;

ResearchScene::ResearchScene(std::shared_ptr<BattleSceneData> battleSceneData)
	:BattleSceneElement(SceneKind::e_research)
	,m_palFont(CreateFontToHandleEX("���C���I",28,1,-1))
	,m_nameFont(CreateFontToHandleEX("���C���I",32,1,-1))
	,m_explainFont(CreateFontToHandleEX("���C���I",explainFontSize,1,-1))
	,m_palBackPic(LoadGraphEX("Graphic/researchInfoBack.png"))
	,m_battleSceneData(battleSceneData)
	,m_researchPic(LoadGraphEX("Graphic/operatedCursor.png"))
{
	//���샆�j�b�g���̏�����
	UpdatePointer();
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
	m_pointerVec=GetMousePointVector2D();
	//�|�C���^�[�̈ʒu�ɂ��郆�j�b�g��m_researchUnit�Ɋi�[����
	m_researchUnit=m_battleSceneData->GetUnitPointer(m_pointerVec);
}

int ResearchScene::thisCalculate(){
	//�|�C���^�[�̍X�V
	UpdatePointer();

	//�J�ڏ���
	if(keyboard_get(KEY_INPUT_F)==1){
		return 0;//�}�b�v���׃��[�h�ؑփ{�^�����͂Œ��O��ʂ�
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
		m_researchUnit->BattleObject::VDraw();
		m_researchUnit->GetHitJudgeShape()->Draw(Vector2D(),GetColor(255,255,255),FALSE);//���ׂĂ����Ă��Ƃ�����
		//���w�����Ă���ɋ���
		Vector2D pos=m_researchUnit->getPos();
		float dx,dy;
		GetGraphSizeF(m_researchPic,&dx,&dy);
		DrawGraph((int)(pos.x-dx/2.0f),(int)(pos.y-dy-Unit::unitCircleSize+10.0f),m_researchPic,TRUE);
	}

	//���쒆���j�b�g�̕`��(m_researchUnit�Ƃ��Ԃ��Ă�����`�悷��K�v�͂Ȃ�)
	if(m_battleSceneData->m_operateUnit!=m_researchUnit){
		m_battleSceneData->m_operateUnit->BattleObject::VDraw();
		Vector2D pos=m_battleSceneData->m_operateUnit->getPos();
		//DrawTriangleAA(pos.x-15.0f,pos.y-60.0f,pos.x+15.0f,pos.y-60.0f,pos.x,pos.y-30.0f,GetColor(255,255,0),TRUE);
	}

	//�S���j�b�g��HP�Q�[�W�̕`��
	m_battleSceneData->DrawHPGage();

	//���j�b�g�̃I�[�_�[���Ԃ�`��
	m_battleSceneData->DrawOrder(std::set<const BattleObject*>{m_researchUnit});

	//�p�����[�^�̕`��
	if(m_researchUnit!=nullptr){
		//�p�����[�^�̕\��
		int gx=1200,gy=140;//�p�����[�^��ʑS�̂̕`��ʒu
		std::pair<int,int> windowSize=GetWindowResolution();
		if(m_researchUnit->getPos().x<(int)windowSize.first/2){
			//���j�b�g�̈ʒu����ʍ������ɂ���Ȃ�
			gx=1200;
		} else{
			//�E�����Ȃ�
			gx=120;
		}
		DrawGraph(gx,gy,m_palBackPic,TRUE);
		const Unit::BaseStatus base=m_researchUnit->GetBaseStatus();
		const Unit::BattleStatus battle=m_researchUnit->GetBattleStatus();
		//�l�[���v���[�g����
		m_researchUnit->DrawFacePic(Vector2D((float)(gx+70),(float)(gy+60)));
		DrawStringToHandle(gx+150,gy+12,base.name.c_str(),GetColor(255,255,255),m_nameFont);
		DrawFormatStringToHandle(gx+180,gy+65,GetColor(255,255,255),m_palFont,"%s Lv%d",Unit::Profession::GetName(base.profession).c_str(),base.lv);
		//�p�����[�^����
		DrawFormatStringToHandle(gx+68,gy+159,GetColor(255,255,255),m_palFont,"%d/%d",battle.HP,base.maxHP);
		DrawFormatStringToHandle(gx+68,gy+203,GetColor(255,255,255),m_palFont,"%.0f/%.0f",battle.OP,battle.maxOP);
		DrawFormatStringToHandle(gx+94,gy+247,GetColor(255,255,255),m_palFont,"%d",base.power);
		DrawFormatStringToHandle(gx+94,gy+291,GetColor(255,255,255),m_palFont,"%d",base.def);
		DrawFormatStringToHandle(gx+94,gy+335,GetColor(255,255,255),m_palFont,"%d",base.mpower);
		DrawFormatStringToHandle(gx+94,gy+379,GetColor(255,255,255),m_palFont,"%d",base.mdef);
		DrawFormatStringToHandle(gx+94,gy+423,GetColor(255,255,255),m_palFont,"%d",base.speed);
		DrawFormatStringToHandle(gx+94,gy+467,GetColor(255,255,255),m_palFont,"%d",base.move);
		//������������
		//�ʏ푕��
		DrawStringToHandle(gx+261,gy+159,m_researchUnit->GetBattleStatus().weapon->GetName().c_str(),GetColor(255,255,255),m_palFont);
		DrawStringNewLineToHandle(gx+272,gy+192,310,36,m_researchUnit->GetBattleStatus().weapon->GetExplain().c_str(),GetColor(255,255,255),m_explainFont,2);
		DrawStringNewLineToHandle(gx+272,gy+240,310,36,m_researchUnit->GetBattleStatus().weapon->GetEffectivenessString(m_researchUnit).c_str(),GetColor(255,255,255),m_explainFont,2);
	}
}

int ResearchScene::UpdateNextScene(int index){
	//���ɉ������Ȃ�
	return index;
}

void ResearchScene::ReturnProcess(){
	//���ɉ������Ȃ�
}
