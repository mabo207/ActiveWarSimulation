#include"StageClearSceme.h"
#include"DxLib.h"
#include"GraphicControl.h"
#include"input.h"
#include"CommonConstParameter.h"
#include"GeneralPurposeResourceManager.h"

//--------------------StageClearScene------------------
const int StageClearScene::bonusFontSize=25;
const int StageClearScene::scoreFontSize=60;

StageClearScene::StageClearScene(std::shared_ptr<BattleSceneData> battleSceneData,bool winFlag)
	:BattleSceneElement(SceneKind::e_clear)
	,m_winFlag(winFlag)
	,m_battleSceneData(battleSceneData)
	,m_scoreExpression(battleSceneData->m_scoreObserver->GetScoreExpression(winFlag))
	,m_backPic(LoadGraphEX("Graphic/result/back.png"),75)
	,m_bonusBar(LoadGraphEX("Graphic/result/bonusBar.png"),75)
	,m_turnBar(LoadGraphEX("Graphic/result/turnBar.png"),75)
	,m_survivalBar(LoadGraphEX("Graphic/result/survivalBar.png"),75)
	,m_resultBarPic(LoadGraphEX((winFlag?"Graphic/result/stageClear.png":"Graphic/result/missionFailed.png")))
	,m_scoreBarPic(LoadGraphEX("Graphic/result/scoreBar.png"))
	,m_bonusFont(CreateFontToHandleEX("�肢�|�b�v�p R",bonusFontSize,4,DX_FONTTYPE_ANTIALIASING_4X4))
	,m_scoreFont(CreateFontToHandleEX("�肢�|�b�v�p R",scoreFontSize,7,DX_FONTTYPE_ANTIALIASING_4X4))
	,m_frame(0)
{}

StageClearScene::~StageClearScene(){
	//�O���t�B�b�N���
	DeleteGraphEX(m_backPic.first);
	DeleteGraphEX(m_bonusBar.first);
	DeleteGraphEX(m_turnBar.first);
	DeleteGraphEX(m_survivalBar.first);
	DeleteGraphEX(m_resultBarPic);
	DeleteGraphEX(m_scoreBarPic);
	//�t�H���g���
	DeleteFontToHandleEX(m_bonusFont);
	DeleteFontToHandleEX(m_scoreFont);
}

int StageClearScene::thisCalculate(){
	m_frame++;

	//0.5�b�����Ĉȍ~�Ɍ���{�^���������΃o�g����ʂ̏I��
	if(m_frame>30 &&
		(keyboard_get(KEY_INPUT_Z)==1 || mouse_get(MOUSE_INPUT_LEFT)==1)
		)
	{
		PlaySoundMem(GeneralPurposeResourceManager::decideSound,DX_PLAYTYPE_BACK,TRUE);//����̌��ʉ���炷
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
	//�X�R�A�w�i�̕\��
	int width,height;
	GetGraphSize(m_backPic.first,&width,&height);
	const int backX=(CommonConstParameter::gameResolutionX-width)/2,backY=250;
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA,256*m_backPic.second/100);
		DrawGraph(backX,backY,m_backPic.first,TRUE);
		SetDrawBlendMode(mode,pal);
	}
	//�N���A�o�[�̕\��
	DrawGraph(0,0,m_resultBarPic,TRUE);
	//�^�[�����`��
	{
		const int x=50,y=50;
		SetDrawBlendMode(DX_BLENDMODE_ALPHA,256*m_turnBar.second/100);
		DrawGraph(backX+x,backY+y,m_turnBar.first,TRUE);
		SetDrawBlendMode(mode,pal);
		DrawStringRightJustifiedToHandle(backX+x+550,backY+y+40,std::to_string(m_scoreExpression->m_turnCount),GetColor(255,255,255),m_scoreFont);
	}
	//�������`��
	{
		const int x=50,y=200;
		const int stringY=backY+y+40;
		SetDrawBlendMode(DX_BLENDMODE_ALPHA,256*m_survivalBar.second/100);
		DrawGraph(backX+x,backY+y,m_survivalBar.first,TRUE);
		SetDrawBlendMode(mode,pal);
		DrawStringRightJustifiedToHandle(backX+x+550,stringY,std::to_string(m_scoreExpression->m_surviveCount).c_str(),GetColor(255,255,255),m_scoreFont);
		DrawStringToHandle(backX+x+555,stringY,("/"+std::to_string(m_scoreExpression->m_unitCount)).c_str(),GetColor(255,255,255),m_scoreFont);
	}
	//�{�[�i�X��ʕ`��
	{
		const int x=850,y=70;
		int bonusWidth,bonusHeight;
		GetGraphSize(m_bonusBar.first,&bonusWidth,&bonusHeight);
		SetDrawBlendMode(DX_BLENDMODE_ALPHA,256*m_bonusBar.second/100);
		DrawGraph(backX+x,backY+y,m_bonusBar.first,TRUE);
		SetDrawBlendMode(mode,pal);
		for(int i=0;i<(int)m_scoreExpression->m_bonusVec.size();i++){
			const int drawY=backY+y+115+(int)(i*bonusFontSize*1.3);
			DrawStringToHandle(backX+x+20,drawY,m_scoreExpression->m_bonusVec[i].GetExplain().c_str(),GetColor(255,255,255),m_bonusFont);
			DrawStringRightJustifiedToHandle(backX+x+bonusWidth-40,drawY,std::to_string(m_scoreExpression->m_bonusVec[i].GetScore()),GetColor(255,255,255),m_bonusFont);
		}
	}
	//���v�X�R�A�`��
	{
		const int x=840,y=670;
		DrawGraph(backX+x,backY+y,m_scoreBarPic,TRUE);
		DrawStringToHandle(backX+x+650,backY+y+30,(std::to_string(m_scoreExpression->m_totalScorePoint)).c_str(),GetColor(255,255,255),m_scoreFont);
	}
}

int StageClearScene::UpdateNextScene(int index){
	//���������ɐi�ނ��Ƃ͂Ȃ��B
	return index;
}

void StageClearScene::ReturnProcess(){
	//���������ɏ�ʂ�����͂����Ȃ�
}
