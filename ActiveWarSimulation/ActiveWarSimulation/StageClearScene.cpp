#include"StageClearSceme.h"
#include"DxLib.h"
#include"GraphicControl.h"
#include"input.h"
#include"CommonConstParameter.h"

//--------------------StageClearScene------------------
StageClearScene::StageClearScene(std::shared_ptr<BattleSceneData> battleSceneData,bool winFlag,const std::string &explain)
	:BattleSceneElement(SceneKind::e_clear)
	,m_winFlag(winFlag)
	,m_explain(explain)
	,m_battleSceneData(battleSceneData)
	,m_stageClearBox(LoadGraphEX("Graphic/stageClearBox.png"))
	,m_clearFont(CreateFontToHandleEX("Bell MT",48,4,DX_FONTTYPE_ANTIALIASING_EDGE_4X4,-1,2))
	,m_explainFont(CreateFontToHandleEX("���C���I",24,1,-1))
	,m_flame(0)
{}

StageClearScene::~StageClearScene(){
	DeleteGraphEX(m_stageClearBox);
	DeleteFontToHandleEX(m_clearFont);
	DeleteFontToHandleEX(m_explainFont);
}

int StageClearScene::thisCalculate(){
	m_flame++;

	//0.5�b�����Ĉȍ~�Ɍ���{�^���������΃o�g����ʂ̏I��
	if(m_flame>30 &&
		(keyboard_get(KEY_INPUT_Z)==1 || mouse_get(MOUSE_INPUT_LEFT)==1)
		)
	{
		return SceneKind::END;
	}

	return SceneKind::e_clear;
}

void StageClearScene::thisDraw()const{
	//�}�b�v�̕\��
	m_battleSceneData->DrawField();
	//���j�b�g�̕\��
	m_battleSceneData->DrawUnit(false,{});
	//HP�Q�[�W�̕`��
	m_battleSceneData->DrawHPGage();

	//�}�b�v�n�͈Â����邽�߂ɍ��������`��`��
	int mode,pal;
	GetDrawBlendMode(&mode,&pal);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA,128);
	DrawBox(0,0,CommonConstParameter::gameResolutionX,CommonConstParameter::gameResolutionY,GetColor(0,0,0),TRUE);
	SetDrawBlendMode(mode,pal);

	//�X�e�[�W�N���A�󋵂̕\��
	int dx,dy;
	GetGraphSize(m_stageClearBox,&dx,&dy);
	DrawGraph((CommonConstParameter::gameResolutionX-dx)/2,(CommonConstParameter::gameResolutionY-dy)/2,m_stageClearBox,TRUE);
	DrawStringCenterBaseToHandle(CommonConstParameter::gameResolutionX/2,(CommonConstParameter::gameResolutionY-dy)/2+48,(m_winFlag?"STAGE CLEAR!":"MISSION FAILED..."),GetColor(255,255,255),m_clearFont,true,(m_winFlag?GetColor(220,128,128):GetColor(128,128,200)));
	DrawStringNewLineToHandle(CommonConstParameter::gameResolutionX/2-dx/4,(CommonConstParameter::gameResolutionY-dy)/2+120,dx/2,dy/4,m_explain.c_str(),GetColor(255,255,255),m_explainFont);
}

int StageClearScene::UpdateNextScene(int index){
	//���������ɐi�ނ��Ƃ͂Ȃ��B
	return index;
}

void StageClearScene::ReturnProcess(){
	//���������ɏ�ʂ�����͂����Ȃ�
}
