#include"ResearchScene.h"
#include"DxLib.h"
#include"input.h"
#include"GraphicControl.h"

//-------------------ResearchScene---------------------
ResearchScene::ResearchScene(std::shared_ptr<BattleSceneData> battleSceneData)
	:BattleSceneElement(SceneKind::e_research)
	,m_palFont(CreateFontToHandleEX("���C���I",28,1,-1))
	,m_nameFont(CreateFontToHandleEX("���C���I",32,1,-1))
	,m_palBackPic(LoadGraphEX("Graphic/researchInfoBack.png"))
	,m_battleSceneData(battleSceneData)
{
	//���샆�j�b�g���̏�����
	UpdatePointer();
}

ResearchScene::~ResearchScene(){
	DeleteFontToHandleEX(m_palFont);
	DeleteFontToHandleEX(m_nameFont);
	DeleteGraph(m_palBackPic);
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
	}

	//���쒆���j�b�g�̕`��(m_researchUnit�Ƃ��Ԃ��Ă�����`�悷��K�v�͂Ȃ�)
	if(m_battleSceneData->m_operateUnit!=m_researchUnit){
		m_battleSceneData->m_operateUnit->BattleObject::VDraw();
		Vector2D pos=m_battleSceneData->m_operateUnit->getPos();
		DrawTriangleAA(pos.x-15.0f,pos.y-60.0f,pos.x+15.0f,pos.y-60.0f,pos.x,pos.y-30.0f,GetColor(255,255,0),TRUE);
	}

	//�S���j�b�g��HP�Q�[�W�̕`��
	m_battleSceneData->DrawHPGage();

	//���j�b�g�̃I�[�_�[���Ԃ�`��
	m_battleSceneData->DrawOrder();

	//�p�����[�^�̕`��
	if(m_researchUnit!=nullptr){
		//�p�����[�^�̕\��
		const int gx=1200,gy=140;
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
	}
}

int ResearchScene::UpdateNextScene(int index){
	//���ɉ������Ȃ�
	return index;
}

void ResearchScene::ReturnProcess(){
	//���ɉ������Ȃ�
}
