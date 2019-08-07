#define NOMINMAX	//std::max()の衝突の回避
#include"StageSelectScene.h"
#include"DxLib.h"
#include"GraphicControl.h"
#include"FileRead.h"
#include<Windows.h>
#include"CommonConstParameter.h"
#include"GeneralPurposeResource.h"
#include"FilePath.h"
#include"BGMManager.h"
#include<algorithm>

#include"BattleScene.h"
#include"TitleScene.h"

#include"StageSelectUIInStageSelect.h"
#include"LevelSelectUIInStageSelect.h"

namespace {
	const int backButtonWidth=80;
	const int backButtonHeight=80;
	const int backButtonX=40;
	const int backButtonY=CommonConstParameter::gameResolutionY-backButtonX-backButtonHeight;
}

//----------------------StageSelectScene------------------
std::shared_ptr<GameScene> StageSelectScene::StageSelectSceneFactory::CreateIncompleteScene()const{
	return std::shared_ptr<GameScene>(new StageSelectScene());
}

StageSelectScene::StageSelectScene()
	:m_nextSceneName(NextSceneName::e_title)
	,m_backPic(LoadGraphEX(FilePath::graphicDir+"nonfree/stageSelectBack.png"))
	,m_backButton(backButtonX,backButtonY,backButtonWidth,backButtonHeight,LoadGraphEX(FilePath::graphicDir+"backButton.png"))
	,m_stageNameFont(CreateFontToHandleEX("メイリオ",32,2,-1))
	,m_explainFont(CreateFontToHandleEX("メイリオ",24,1,-1))
	,m_bgm(Resource::BGM::Load(FilePath::bgmDir+"nonfree/title/"))
	,m_uiControledData(new BaseUIInStageSelect::ControledData(0,StageLevel::e_easy))
{
	//フォルダを検索
	const std::vector<std::string> dirNameVec={"1_1","1_2","1_3","1_4","2_1","2_2","2_3","2_4","3_1","3_2","3_3","3_4","4_1","4_2","4_3","4_4","4_5"};
	//各フォルダ名から、m_stageInfoFactoryMapを構築していく
	for(const std::string &dirName:dirNameVec){
		m_stageInfoFactoryMap.insert(std::make_pair(dirName,LoadGraphEX((FilePath::stageDir+dirName+"/nonfree/minimap.png").c_str())));
	}
}

StageSelectScene::~StageSelectScene(){
	//グラフィックの解放
	DeleteGraphEX(m_backPic);
	for(const auto pair:m_stageInfoFactoryMap){
		DeleteGraphEX(pair.second);
	}
	//フォントの解放
	DeleteFontToHandleEX(m_stageNameFont);
	DeleteFontToHandleEX(m_explainFont);
	//音の解放
	m_bgm.Delete();
}

void StageSelectScene::InitCompletely(){
	//スコアデータの読み込み
	const ScoreRankingData rankingData;
	//m_stageInfoFactoryMapからフォルダ検索を行いながら、StageInfoInStageSelectを構成していく
	for(const std::pair<std::string,int> &info:m_stageInfoFactoryMap){
		try{
			m_stageInfoVec.push_back(StageInfoInStageSelect(
				info.second
				,info.first
				,FileStrRead((FilePath::stageDir+info.first+"/explain.txt").c_str())
				,rankingData
			));
		} catch(const FileOpenFailedException &){
			//この場面では、ファイルを開くのに失敗することを許容する。
			//ファイルを開くのに失敗した場合は、insert()を行わないだけでそのまま処理を続ける
		}
	}
}

void StageSelectScene::Activate(){
	//UIの作成
	m_ui=std::shared_ptr<StageSelectUIInStageSelect>(new StageSelectUIInStageSelect(m_uiControledData,m_backButton,m_stageInfoVec,m_stageNameFont,m_explainFont));
	//bgm再生
	if(BGMManager::s_instance.has_value()){
		BGMManager::s_instance->PlayWithCopy(m_bgm);
	}
}

int StageSelectScene::Calculate(){
	//表示情報の位置更新
	m_uiControledData->stageInfoPos.Update();
	m_uiControledData->levelInfoPos.Update();
	//選択ステージの更新
	const auto updateResult=m_ui->Update();
	if(updateResult==BaseUIInStageSelect::UpdateResult::e_gotoBattle){
		//ゲームプレイへ進む
		m_nextSceneName=NextSceneName::e_battle;
		return -1;
	} else if(updateResult==BaseUIInStageSelect::UpdateResult::e_gotoTitle){
		//タイトル画面へ戻る
		m_nextSceneName=NextSceneName::e_title;
		return -1;
	} else if(updateResult==BaseUIInStageSelect::UpdateResult::e_gotoStageSelect){
		//ステージセレクトにUI遷移
		m_ui=std::shared_ptr<BaseUIInStageSelect>(new StageSelectUIInStageSelect(m_uiControledData,m_backButton,m_stageInfoVec,m_stageNameFont,m_explainFont));
	} else if(updateResult==BaseUIInStageSelect::UpdateResult::e_gotoLevelSelect){
		//レベルセレクトにUI遷移
		m_ui=std::shared_ptr<BaseUIInStageSelect>(new LevelSelectUIInStageSelect(m_uiControledData,m_backButton,m_stageInfoVec[m_uiControledData->stageIndex],m_stageNameFont,m_explainFont));
	}

	return 0;
}

void StageSelectScene::Draw()const{
	//背景の描画
	DrawGraph(0,0,m_backPic,TRUE);
	//ステージ一覧と経路の描画
	for(size_t i=0,siz=m_stageInfoVec.size();i<siz;i++){
		//ステージの経路の描画
		if(i+1<siz){
			//次のステージとの距離を求める
			const Vector2D dir=m_stageInfoVec[i+1].m_pos-m_stageInfoVec[i].m_pos;
			const float dist=dir.size();
			//何個丸を描画するか決める
			const float circleSize=10.0f;
			const int divideNum=std::max((int)(dist/(circleSize*6.0f)),2);//最低1個は描画、divideNumは隙間の個数なので(divideNum-1)個の丸が道として描画される
			//描画
			for(int j=1;j<divideNum;j++){
				//ここの中身はdivideNum>0の時のみ処理されるので、divideNumの0チェックは必要ない
				const Vector2D pos=m_stageInfoVec[i].m_pos+dir*((float)j)/((float)divideNum);
				DrawCircleAA(pos.x,pos.y,circleSize,10,GetColor(64,64,128),TRUE);
				DrawCircleAA(pos.x,pos.y,circleSize*0.6f,10,GetColor(64,128,196),TRUE);
			}
		}
		//ステージの位置の描画
		DrawCircleAA(m_stageInfoVec[i].m_pos.x,m_stageInfoVec[i].m_pos.y,30,10,GetColor(64,64,255),TRUE);
		DrawCircleAA(m_stageInfoVec[i].m_pos.x,m_stageInfoVec[i].m_pos.y,20,10,GetColor(128,196,255),TRUE);
	}
	//UIの描画
	m_ui->Draw();
	//戻るボタンの描画
	m_backButton.DrawButton();
}

std::shared_ptr<GameScene> StageSelectScene::VGetNextScene(const std::shared_ptr<GameScene> &thisSharedPtr)const{
	if(m_nextSceneName==NextSceneName::e_title){
		const auto titleFactory=std::make_shared<TitleScene::TitleSceneFactory>();
		return CreateFadeOutInSceneCompletely(thisSharedPtr,titleFactory,15,15);
	} else if(m_nextSceneName==NextSceneName::e_battle){
		const std::shared_ptr<GameScene::SceneFactory> battleFactory=std::make_shared<BattleScene::BattleSceneFactory>(
			m_stageInfoVec[m_uiControledData->stageIndex].m_dirName
			,m_stageInfoVec[m_uiControledData->stageIndex].m_titleName
			,m_uiControledData->selectLevel);
		return CreateFadeOutInSceneCompletely(thisSharedPtr,battleFactory,15,15);
	}
	return std::shared_ptr<GameScene>();
}
