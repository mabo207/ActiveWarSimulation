#define NOMINMAX
#include"StageClearScene.h"
#include"DxLib.h"
#include"GraphicControl.h"
#include"input.h"
#include"CommonConstParameter.h"
#include"GeneralPurposeResource.h"
#include"ScoreRankingData.h"
#include"FilePath.h"
#include"BrowserTweet.h"
#include"ClearStageData.h"

namespace {
	const int bonusFontSize=25;
	const int bonusLineHeight=bonusFontSize+20;
	const int bonusStrAreaHeight=450;
}

//--------------------StageClearScene------------------
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
	,m_frame(0)
	,m_inputCharControler("\\\"\'",11)
	,m_nowProcess(ProcessKind::e_watchScore)
	,m_bonusStrDY(0,15,Easing::TYPE_IN,Easing::FUNCTION_LINER,0.0)
	,m_tweetButton(420,700,LoadGraphEX((FilePath::graphicDir+"tweetButton.png").c_str()))
	,m_backToStageSelectButton(1100,700,LoadGraphEX((FilePath::graphicDir+"backToStageSelectButton.png").c_str()))
{
	const int bonusStrTotalHeight=m_scoreExpression->m_bonusVec.size()*bonusLineHeight;//ボーナスをすべて描画したらどのくらいの高さがあるか
	m_bonusStrMinDY=std::min(0,bonusStrAreaHeight-bonusStrTotalHeight);
}

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
}

void StageClearScene::ResisterScoreSave(){
	//必要なデータを抽出（std::shared_ptrを渡すと循環参照になりそうで怖い）
	const int totalPoint=m_scoreExpression->m_totalScorePoint;
	const std::string playerName=m_inputCharControler.GetString();
	const __time64_t now=_time64(nullptr);
	const std::string dirName=m_battleSceneData->m_stageDirName;
	const StageLevel level=m_battleSceneData->m_stageLevel;
	//関数オブジェクト作成
	const std::function<void(void)> saveFunc=[totalPoint,playerName,now,dirName,level](){
		ScoreRankingData rankingData;
		rankingData.InputData(ScoreRankingData::PlayerData(totalPoint,playerName,now),dirName,level);
		rankingData.Save();
	};
	//登録
	m_battleSceneData->ResisterSceneEndProcess(saveFunc);
}

void StageClearScene::ResisterClearSave(){
	//クリア時のみクリアデータの更新の登録をする
	if(m_winFlag){
		//必要なデータの抽出
		const std::string dirName=m_battleSceneData->m_stageDirName;
		//関数オブジェクト作成
		const std::function<void(void)> saveFunc=[dirName](){
			ClearStageData clearData;
			clearData.BecomeClear(dirName);
			clearData.Save();
		};
		//登録
		m_battleSceneData->ResisterSceneEndProcess(saveFunc);
	}
}

int StageClearScene::thisCalculate(){
	m_frame++;
	//ボーナスの描画位置更新
	m_bonusStrDY.Update();

	//場面ごとの入力処理
	if(m_nowProcess==ProcessKind::e_watchScore){
		if(m_frame>30 &&
			(keyboard_get(KEY_INPUT_Z)==1 || mouse_get(MOUSE_INPUT_LEFT)==1)
			)
		{
			//スコアを見ている時に0.5秒たって以降に決定ボタンを押せば名前入力への終了
			PlaySoundMem(GeneralPurposeResource::decideSound,DX_PLAYTYPE_BACK,TRUE);//決定の効果音を鳴らす
			m_nowProcess=ProcessKind::e_inputName;
		} else if(keyboard_get(KEY_INPUT_UP)%15==1 || mouse_wheel_get()>0){
			//マウスを上スクロールするか、上キーを押すと、ボーナス画面が下に進む
			m_bonusStrDY.SetTarget(std::min(m_bonusStrDY.GetendX()+bonusLineHeight,0),true);
		} else if(keyboard_get(KEY_INPUT_DOWN)%15==1 || mouse_wheel_get()<0){
			//マウスを下スクロールするか、下キーを押すと、ボーナス画面が上に進む
			m_bonusStrDY.SetTarget(std::max(m_bonusStrDY.GetendX()-bonusLineHeight,m_bonusStrMinDY),true);
		}
	} else if(m_nowProcess==ProcessKind::e_inputName){
		m_inputCharControler.Update();
		if(!m_inputCharControler.GetInputFlag() || m_backToStageSelectButton.JudgePressMoment()){
			//入力終了したら
			PlaySoundMem(GeneralPurposeResource::decideSound,DX_PLAYTYPE_BACK,TRUE);//決定の効果音を鳴らす
			//記録処理を登録
			ResisterScoreSave();
			ResisterClearSave();
			//バトルパート終了
			return SceneKind::END;
		}
		if(m_tweetButton.JudgePressMoment()){
			//効果音
			PlaySoundMem(GeneralPurposeResource::decideSound,DX_PLAYTYPE_BACK,TRUE);
			//twitter投稿のURI作成のための情報を作成
			std::string text;
			const std::string name=m_inputCharControler.GetString();
			if(m_winFlag){
				//クリア時の文言
				if(!name.empty()){
					//名前入力がされている場合は、「○○さんが」という文言を載せる
					text+=name+"さんが";
				}
				text+="『"+m_battleSceneData->m_stageTitleName+'('+m_battleSceneData->m_stageLevel.GetString()+")』をクリアしたよ！";
				text+="\n生存ユニット数："+std::to_string(m_scoreExpression->m_surviveCount)+" / "+std::to_string(m_scoreExpression->m_unitCount);
				text+="\nクリアターン数："+std::to_string(m_scoreExpression->m_turnCount);
			} else{
				//ミス時の文言
				if(!name.empty()){
					//名前入力がされていない場合は「○○さんは」という文言を載せる
					text+=name+"さんは";
				}
				text+="『"+m_battleSceneData->m_stageTitleName+'('+m_battleSceneData->m_stageLevel.GetString()+")』で敵にボコボコにされてしまいました……";
				text+="\n撃破ユニット数："+std::to_string(m_scoreExpression->m_defeatCount)+" / "+std::to_string(m_scoreExpression->m_enemyCount);
				text+="\n粘ったターン数："+std::to_string(m_scoreExpression->m_turnCount);
			}
			text+="\nScore："+std::to_string(m_scoreExpression->m_totalScorePoint);
			text+="\n\n";
			const std::vector<std::string> hashtagVec={"ActiveWarSimulation","あくうぉー"};
			//既定ブラウザで上記tweetページを開いてツイートする
			BrowserTweet::TweetText(text,hashtagVec);
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
		const int x=50,y=60;
		SetDrawBlendMode(DX_BLENDMODE_ALPHA,256*m_turnBar.second/100);
		DrawGraph(backX+x,backY+y,m_turnBar.first,TRUE);
		SetDrawBlendMode(mode,pal);
		DrawStringRightJustifiedToHandle(backX+x+550,backY+y+40,std::to_string(m_scoreExpression->m_turnCount),GetColor(255,255,255),GeneralPurposeResource::popLargeFont);
	}
	//生存数描画
	{
		const int x=50,y=210;
		const int stringY=backY+y+40;
		SetDrawBlendMode(DX_BLENDMODE_ALPHA,256*m_survivalBar.second/100);
		DrawGraph(backX+x,backY+y,m_survivalBar.first,TRUE);
		SetDrawBlendMode(mode,pal);
		DrawStringRightJustifiedToHandle(backX+x+550,stringY,std::to_string(m_scoreExpression->m_surviveCount).c_str(),GetColor(255,255,255),GeneralPurposeResource::popLargeFont);
		DrawStringToHandle(backX+x+555,stringY,("/"+std::to_string(m_scoreExpression->m_unitCount)).c_str(),GetColor(255,255,255),GeneralPurposeResource::popLargeFont);
	}
	//ボーナス画面描画
	{
		const int x=850,y=50;
		int bonusWidth,bonusHeight;
		GetGraphSize(m_bonusBar.first,&bonusWidth,&bonusHeight);
		SetDrawBlendMode(DX_BLENDMODE_ALPHA,256*m_bonusBar.second/100);
		DrawGraph(backX+x,backY+y,m_bonusBar.first,TRUE);
		SetDrawBlendMode(mode,pal);
		const int strX=backX+x+20,strY=backY+y+115;
		const int strWidth=bonusWidth-40;
		SetDrawArea(strX,strY,strX+bonusWidth,strY+bonusStrAreaHeight);
		const int strStartY=strY+m_bonusStrDY.GetX();//描画開始位置と描画範囲の上は違う
		for(int i=0;i<(int)m_scoreExpression->m_bonusVec.size();i++){
			const int drawY=strStartY+i*bonusLineHeight;
			DrawStringToHandle(strX,drawY,m_scoreExpression->m_bonusVec[i].GetExplain().c_str(),GetColor(255,255,255),m_bonusFont);
			DrawStringRightJustifiedToHandle(strX+strWidth,drawY,std::to_string(m_scoreExpression->m_bonusVec[i].GetScore()),GetColor(255,255,255),m_bonusFont);
		}
		SetDrawAllArea();
	}
	//合計スコア描画
	{
		const int x=840,y=670;
		DrawGraph(backX+x,backY+y,m_scoreBarPic,TRUE);
		DrawStringToHandle(backX+x+650,backY+y+30,(std::to_string(m_scoreExpression->m_totalScorePoint)).c_str(),GetColor(255,255,255),GeneralPurposeResource::popLargeFont);
	}

	//名前入力中の描画処理
	if(m_nowProcess==ProcessKind::e_inputName){
		//暗くする
		{
			int mode,pal;
			GetDrawBlendMode(&mode,&pal);
			SetDrawBlendMode(DX_BLENDMODE_ALPHA,196);
			DrawBox(0,0,CommonConstParameter::gameResolutionX,CommonConstParameter::gameResolutionY,GetColor(0,0,0),TRUE);
			SetDrawBlendMode(mode,pal);
		}
		//文字列の描画
		{
			//見出しの描画
			DrawStringCenterBaseToHandle(CommonConstParameter::gameResolutionX/2,CommonConstParameter::gameResolutionY/4,"名前を入力しよう！",GetColor(255,255,255),GeneralPurposeResource::popLargeFont,true);
			//名前の描画(縦棒描画のための情報を計算するため、DrawStringCenterBaseToHandle()を使う必要はない)
			const std::string name=m_inputCharControler.GetString();
			const int strWidth=GetDrawStringWidthToHandle(name.c_str(),name.size(),GeneralPurposeResource::popLargeFont);
			const int strHeight=GetFontSizeToHandle(GeneralPurposeResource::popLargeFont);
			const int strX=CommonConstParameter::gameResolutionX/2-strWidth/2;
			const int strY=CommonConstParameter::gameResolutionY/2-strHeight/2;
			DrawStringToHandle(strX,strY,name.c_str(),GetColor(255,255,255),GeneralPurposeResource::popLargeFont);
			//下線の描画
			const int lineY=CommonConstParameter::gameResolutionY/2+GetFontSizeToHandle(GeneralPurposeResource::popLargeFont)/2+5;
			DrawLine(CommonConstParameter::gameResolutionX*2/7,lineY,CommonConstParameter::gameResolutionX*5/7,lineY,GetColor(255,255,255),6);
			//入力可能だとわかるようにするため、点滅している縦棒の描画
			if((m_battleSceneData->m_fpsMesuring.GetFrame()/30)%2==0){
				//印象を統一するために文字描画で表現
				DrawStringToHandle(strX+strWidth,strY,"|",GetColor(255,255,255),GeneralPurposeResource::popLargeFont);
			}
		}
		//ツイートボタンの描画
		m_tweetButton.DrawButton();
		//次に進むボタンの描画
		m_backToStageSelectButton.DrawButton();
	}
}

int StageClearScene::UpdateNextScene(int index){
	//次の場面なんてない
	return index;
}

void StageClearScene::ReturnProcess(){
	//特に何もしない
}
