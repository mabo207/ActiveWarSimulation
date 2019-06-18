#include"StageClearScene.h"
#include"DxLib.h"
#include"GraphicControl.h"
#include"input.h"
#include"CommonConstParameter.h"
#include"GeneralPurposeResourceManager.h"
#include"ScoreRankingData.h"
#include"FilePath.h"

//--------------------StageClearScene------------------
const int StageClearScene::bonusFontSize=25;
const int StageClearScene::scoreFontSize=60;

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
	,m_bonusFont(CreateFontToHandleEX("りいポップ角 R",bonusFontSize,4,DX_FONTTYPE_ANTIALIASING_4X4))
	,m_scoreFont(CreateFontToHandleEX("りいポップ角 R",scoreFontSize,7,DX_FONTTYPE_ANTIALIASING_4X4))
	,m_frame(0)
	,m_inputCharControler("\\\"\'",11)
	,m_nowProcess(ProcessKind::e_watchScore)
{}

StageClearScene::~StageClearScene(){
	//グラフィック解放
	DeleteGraphEX(m_backPic.first);
	DeleteGraphEX(m_bonusBar.first);
	DeleteGraphEX(m_turnBar.first);
	DeleteGraphEX(m_survivalBar.first);
	DeleteGraphEX(m_resultBarPic);
	DeleteGraphEX(m_scoreBarPic);
	//フォント解放
	DeleteFontToHandleEX(m_bonusFont);
	DeleteFontToHandleEX(m_scoreFont);
}

int StageClearScene::thisCalculate(){
	m_frame++;

	if(m_nowProcess==ProcessKind::e_watchScore){
		//スコアを見ている時に0.5秒たって以降に決定ボタンを押せば名前入力への終了
		if(m_frame>30 &&
			(keyboard_get(KEY_INPUT_Z)==1 || mouse_get(MOUSE_INPUT_LEFT)==1)
			)
		{
			PlaySoundMem(GeneralPurposeResourceManager::decideSound,DX_PLAYTYPE_BACK,TRUE);//決定の効果音を鳴らす
			m_nowProcess=ProcessKind::e_inputName;
		}
	} else if(m_nowProcess==ProcessKind::e_inputName){
		m_inputCharControler.Update();
		if(!m_inputCharControler.GetInputFlag()){
			//入力終了したら
			PlaySoundMem(GeneralPurposeResourceManager::decideSound,DX_PLAYTYPE_BACK,TRUE);//決定の効果音を鳴らす
			//記録
			ScoreRankingData rankingData;
			const __time64_t now=_time64(nullptr);
			rankingData.InputData(ScoreRankingData::PlayerData(m_scoreExpression->m_totalScorePoint,m_inputCharControler.GetString(),now),m_battleSceneData->m_stageDirName,m_battleSceneData->m_stageLevel);
			rankingData.Save();
			//バトルパート終了
			return SceneKind::END;
		}
	}

	return SceneKind::e_clear;
}

void StageClearScene::thisDraw()const{
	//マップの表示
	m_battleSceneData->DrawField();
	//ユニットの表示
	m_battleSceneData->DrawUnit(false,{});

	//マップ系は暗くするために黒い長方形を描画
	int mode,pal;
	GetDrawBlendMode(&mode,&pal);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA,128);
	DrawBox(0,0,CommonConstParameter::gameResolutionX,CommonConstParameter::gameResolutionY,GetColor(0,0,0),TRUE);
	SetDrawBlendMode(mode,pal);

	//ステージクリア状況の表示
	//スコア背景の表示
	int width,height;
	GetGraphSize(m_backPic.first,&width,&height);
	const int backX=(CommonConstParameter::gameResolutionX-width)/2,backY=250;
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA,256*m_backPic.second/100);
		DrawGraph(backX,backY,m_backPic.first,TRUE);
		SetDrawBlendMode(mode,pal);
	}
	//クリアバーの表示
	DrawGraph(0,0,m_resultBarPic,TRUE);
	//ターン数描画
	{
		const int x=50,y=50;
		SetDrawBlendMode(DX_BLENDMODE_ALPHA,256*m_turnBar.second/100);
		DrawGraph(backX+x,backY+y,m_turnBar.first,TRUE);
		SetDrawBlendMode(mode,pal);
		DrawStringRightJustifiedToHandle(backX+x+550,backY+y+40,std::to_string(m_scoreExpression->m_turnCount),GetColor(255,255,255),m_scoreFont);
	}
	//生存数描画
	{
		const int x=50,y=200;
		const int stringY=backY+y+40;
		SetDrawBlendMode(DX_BLENDMODE_ALPHA,256*m_survivalBar.second/100);
		DrawGraph(backX+x,backY+y,m_survivalBar.first,TRUE);
		SetDrawBlendMode(mode,pal);
		DrawStringRightJustifiedToHandle(backX+x+550,stringY,std::to_string(m_scoreExpression->m_surviveCount).c_str(),GetColor(255,255,255),m_scoreFont);
		DrawStringToHandle(backX+x+555,stringY,("/"+std::to_string(m_scoreExpression->m_unitCount)).c_str(),GetColor(255,255,255),m_scoreFont);
	}
	//ボーナス画面描画
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
	//合計スコア描画
	{
		const int x=840,y=670;
		DrawGraph(backX+x,backY+y,m_scoreBarPic,TRUE);
		DrawStringToHandle(backX+x+650,backY+y+30,(std::to_string(m_scoreExpression->m_totalScorePoint)).c_str(),GetColor(255,255,255),m_scoreFont);
	}

	//名前入力中の描画処理
	if(m_nowProcess==ProcessKind::e_inputName){
		//暗くする
		{
			int mode,pal;
			GetDrawBlendMode(&mode,&pal);
			SetDrawBlendMode(DX_BLENDMODE_ALPHA,128);
			DrawBox(0,0,CommonConstParameter::gameResolutionX,CommonConstParameter::gameResolutionY,GetColor(0,0,0),TRUE);
			SetDrawBlendMode(mode,pal);
		}
		//文字列の描画
		{
			DrawStringCenterBaseToHandle(CommonConstParameter::gameResolutionX/2,CommonConstParameter::gameResolutionY/4,"名前を入力しよう！",GetColor(255,255,255),m_scoreFont,true);
			DrawStringCenterBaseToHandle(CommonConstParameter::gameResolutionX/2,CommonConstParameter::gameResolutionY/2,m_inputCharControler.GetStringCStr(),GetColor(255,255,255),m_scoreFont,true);
			const int lineY=CommonConstParameter::gameResolutionY/2+scoreFontSize/2+5;
			DrawLine(CommonConstParameter::gameResolutionX*2/7,lineY,CommonConstParameter::gameResolutionX*5/7,lineY,GetColor(255,255,255),6);
		}
	}
}

int StageClearScene::UpdateNextScene(int index){
	//次の場面なんてない
	return index;
}

void StageClearScene::ReturnProcess(){
	//特に何もしない
}
