#include"StageClearScene.h"
#include"DxLib.h"
#include"GraphicControl.h"
#include"input.h"
#include"CommonConstParameter.h"
#include"GeneralPurposeResource.h"
#include"ScoreRankingData.h"
#include"FilePath.h"
#include"BrowserTweet.h"

//--------------------StageClearScene------------------
const int StageClearScene::bonusFontSize=25;

StageClearScene::StageClearScene(std::shared_ptr<BattleSceneData> battleSceneData,bool winFlag)
	:BattleSceneElement(SceneKind::e_clear)
	,m_winFlag(winFlag)
	,m_battleSceneData(battleSceneData)
	,m_scoreExpression(battleSceneData->m_scoreObserver->GetScoreExpression(winFlag))
	,m_backPic(LoadGraphEX(FilePath::graphicDir+"result/back.png"),75)
	,m_bonusBar(LoadGraphEX(FilePath::graphicDir+"result/bonusBar.png"),75)
	,m_turnBar(LoadGraphEX(FilePath::graphicDir+"result/turnBar.png"),75)
	,m_survivalBar(LoadGraphEX(FilePath::graphicDir+"result/survivalBar.png"),75)
	,m_resultBarPic(LoadGraphEX((winFlag?FilePath::graphicDir+"result/stageClear.png":FilePath::graphicDir+"result/missionFailed.png")))
	,m_scoreBarPic(LoadGraphEX(FilePath::graphicDir+"result/scoreBar.png"))
	,m_bonusFont(CreateFontToHandleEX("�肢�|�b�v�p R",bonusFontSize,4,DX_FONTTYPE_ANTIALIASING_4X4))
	,m_frame(0)
	,m_inputCharControler("\\\"\'",11)
	,m_nowProcess(ProcessKind::e_watchScore)
	,m_tweetButton(100,900,LoadGraphEX((FilePath::graphicDir+"tweetButton.png").c_str()))
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
}

void StageClearScene::ResisterScoreSave(){
	//�K�v�ȃf�[�^�𒊏o�istd::shared_ptr��n���Əz�Q�ƂɂȂ肻���ŕ|���j
	const int totalPoint=m_scoreExpression->m_totalScorePoint;
	const std::string playerName=m_inputCharControler.GetString();
	const __time64_t now=_time64(nullptr);
	const std::string dirName=m_battleSceneData->m_stageDirName;
	const StageLevel level=m_battleSceneData->m_stageLevel;
	//�֐��I�u�W�F�N�g�쐬
	const std::function<void(void)> saveFunc=[totalPoint,playerName,now,dirName,level](){
		ScoreRankingData rankingData;
		rankingData.InputData(ScoreRankingData::PlayerData(totalPoint,playerName,now),dirName,level);
		rankingData.Save();
	};
	//�o�^
	m_battleSceneData->ResisterSceneEndProcess(saveFunc);
}

int StageClearScene::thisCalculate(){
	m_frame++;

	if(m_nowProcess==ProcessKind::e_watchScore){
		//�X�R�A�����Ă��鎞��0.5�b�����Ĉȍ~�Ɍ���{�^���������Ζ��O���͂ւ̏I��
		if(m_frame>30 &&
			(keyboard_get(KEY_INPUT_Z)==1 || mouse_get(MOUSE_INPUT_LEFT)==1)
			)
		{
			PlaySoundMem(GeneralPurposeResource::decideSound,DX_PLAYTYPE_BACK,TRUE);//����̌��ʉ���炷
			m_nowProcess=ProcessKind::e_inputName;
		}
	} else if(m_nowProcess==ProcessKind::e_inputName){
		m_inputCharControler.Update();
		if(!m_inputCharControler.GetInputFlag()){
			//���͏I��������
			PlaySoundMem(GeneralPurposeResource::decideSound,DX_PLAYTYPE_BACK,TRUE);//����̌��ʉ���炷
			//�L�^������o�^
			ResisterScoreSave();
			//�o�g���p�[�g�I��
			return SceneKind::END;
		}
		if(m_tweetButton.JudgePressMoment()){
			//���ʉ�
			PlaySoundMem(GeneralPurposeResource::decideSound,DX_PLAYTYPE_BACK,TRUE);
			//twitter���e��URI�쐬�̂��߂̏����쐬
			const std::string text="���肠�I";
			const std::vector<std::string> hashtagVec={"ActiveWarSimulation","���������["};
			//����u���E�U�ŏ�Ltweet�y�[�W���J���ăc�C�[�g����
			BrowserTweet::TweetText(text,hashtagVec);
		}
	}

	return SceneKind::e_clear;
}

void StageClearScene::thisDraw()const{
	//�}�b�v�̕\��
	m_battleSceneData->DrawField();
	//���j�b�g�̕\��
	m_battleSceneData->DrawUnit(false,{});

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
		DrawStringRightJustifiedToHandle(backX+x+550,backY+y+40,std::to_string(m_scoreExpression->m_turnCount),GetColor(255,255,255),GeneralPurposeResource::popLargeFont);
	}
	//�������`��
	{
		const int x=50,y=200;
		const int stringY=backY+y+40;
		SetDrawBlendMode(DX_BLENDMODE_ALPHA,256*m_survivalBar.second/100);
		DrawGraph(backX+x,backY+y,m_survivalBar.first,TRUE);
		SetDrawBlendMode(mode,pal);
		DrawStringRightJustifiedToHandle(backX+x+550,stringY,std::to_string(m_scoreExpression->m_surviveCount).c_str(),GetColor(255,255,255),GeneralPurposeResource::popLargeFont);
		DrawStringToHandle(backX+x+555,stringY,("/"+std::to_string(m_scoreExpression->m_unitCount)).c_str(),GetColor(255,255,255),GeneralPurposeResource::popLargeFont);
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
		DrawStringToHandle(backX+x+650,backY+y+30,(std::to_string(m_scoreExpression->m_totalScorePoint)).c_str(),GetColor(255,255,255),GeneralPurposeResource::popLargeFont);
	}

	//���O���͒��̕`�揈��
	if(m_nowProcess==ProcessKind::e_inputName){
		//�Â�����
		{
			int mode,pal;
			GetDrawBlendMode(&mode,&pal);
			SetDrawBlendMode(DX_BLENDMODE_ALPHA,128);
			DrawBox(0,0,CommonConstParameter::gameResolutionX,CommonConstParameter::gameResolutionY,GetColor(0,0,0),TRUE);
			SetDrawBlendMode(mode,pal);
		}
		//������̕`��
		{
			DrawStringCenterBaseToHandle(CommonConstParameter::gameResolutionX/2,CommonConstParameter::gameResolutionY/4,"���O����͂��悤�I",GetColor(255,255,255),GeneralPurposeResource::popLargeFont,true);
			DrawStringCenterBaseToHandle(CommonConstParameter::gameResolutionX/2,CommonConstParameter::gameResolutionY/2,m_inputCharControler.GetStringCStr(),GetColor(255,255,255),GeneralPurposeResource::popLargeFont,true);
			const int lineY=CommonConstParameter::gameResolutionY/2+GetFontSizeToHandle(GeneralPurposeResource::popLargeFont)/2+5;
			DrawLine(CommonConstParameter::gameResolutionX*2/7,lineY,CommonConstParameter::gameResolutionX*5/7,lineY,GetColor(255,255,255),6);
		}
		//�c�C�[�g�{�^���̕`��
		m_tweetButton.DrawButton();
	}
}

int StageClearScene::UpdateNextScene(int index){
	//���̏�ʂȂ�ĂȂ�
	return index;
}

void StageClearScene::ReturnProcess(){
	//���ɉ������Ȃ�
}
