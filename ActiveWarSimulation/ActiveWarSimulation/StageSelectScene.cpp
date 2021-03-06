#define NOMINMAX	//std::max()の衝突の回避
#include"StageSelectScene.h"
#include"DxLib.h"
#include"GraphicControl.h"
#include"FileRead.h"
#include"CommonConstParameter.h"
#include"GeneralPurposeResource.h"
#include"FilePath.h"
#include"BGMManager.h"
#include<algorithm>
#include"ClearStageData.h"

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
	,m_backDefaultPic(LoadGraphEX(FilePath::graphicDir+"nonfree/stageSelectBack_default.png"))
	,m_backNightPic(LoadGraphEX(FilePath::graphicDir+"nonfree/stageSelectBack_night.png"))
	,m_backMorningPic(LoadGraphEX(FilePath::graphicDir+"nonfree/stageSelectBack_morning.png"))
	,m_backButton(backButtonX,backButtonY,backButtonWidth,backButtonHeight,LoadGraphEX(FilePath::graphicDir+"backButton.png"))
	,m_stageNameFont(LoadFontDataToHandleEX(FilePath::fontDir+"ExplainGothicFont_Large.dft",0))
	,m_bgm(Resource::BGM::Load("title.txt"))
	,m_uiControledData(new BaseUIInStageSelect::ControledData(0,StageLevel::e_easy))
	,m_clearStageNum(0)
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
	DeleteGraphEX(m_backDefaultPic);
	DeleteGraphEX(m_backNightPic);
	DeleteGraphEX(m_backMorningPic);
	for(const auto pair:m_stageInfoFactoryMap){
		DeleteGraphEX(pair.second);
	}
	//フォントの解放
	DeleteFontToHandleEX(m_stageNameFont);
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
	//ステージの個数の計算
	const ClearStageData clearStageData;
	for(size_t i=m_stageInfoVec.size()-1;;i--){
		//クリアしたステージの検索(末尾から)
		if(clearStageData.JudgeClear(m_stageInfoVec[i].m_dirName)){
			//検索がhitしたら、そこより前が全てクリアできているものとする
			m_clearStageNum=i+1;
			break;
		}
		if(i==0){
			//終了条件
			break;
		}
	}
	//選択ステージの初期化（一番最後のステージを選択するようにする）
	if(m_clearStageNum>=m_stageInfoVec.size()){
		//全ステージクリアしているなら、最終ステージを指すようにする
		m_uiControledData->stageIndex=m_stageInfoVec.size()-1;
	} else{
		//クリアしていないステージがあるなら、一番最新のステージを指すようにする
		m_uiControledData->stageIndex=m_clearStageNum;
	}
}

void StageSelectScene::Activate(){
	//UIの作成
	m_ui=std::shared_ptr<StageSelectUIInStageSelect>(new StageSelectUIInStageSelect(m_uiControledData,m_backButton,m_stageInfoVec,m_stageNameFont,GeneralPurposeResource::gothicMiddleFont,m_clearStageNum));
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
		m_ui=std::shared_ptr<BaseUIInStageSelect>(new StageSelectUIInStageSelect(m_uiControledData,m_backButton,m_stageInfoVec,m_stageNameFont,GeneralPurposeResource::gothicMiddleFont,m_clearStageNum));
	} else if(updateResult==BaseUIInStageSelect::UpdateResult::e_gotoLevelSelect){
		//レベルセレクトにUI遷移
		m_ui=std::shared_ptr<BaseUIInStageSelect>(new LevelSelectUIInStageSelect(m_uiControledData,m_backButton,m_stageInfoVec[m_uiControledData->stageIndex],m_stageNameFont,GeneralPurposeResource::gothicMiddleFont));
	}

	return 0;
}

void StageSelectScene::Draw()const{
	//背景の描画
	DrawBack();
	//ステージ一覧と経路の描画
	for(size_t i=0,siz=std::min(m_clearStageNum+1,m_stageInfoVec.size());i<siz;i++){
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
		unsigned int inColor,outColor;
		if(i<m_clearStageNum){
			//クリアステージは青基調で描画
			inColor=GetColor(128,196,255);
			outColor=GetColor(64,64,255);
		} else{
			//まだのステージは赤基調で描画
			inColor=GetColor(255,196,128);
			outColor=GetColor(255,64,64);
		}
		DrawCircleAA(m_stageInfoVec[i].m_pos.x,m_stageInfoVec[i].m_pos.y,30,10,outColor,TRUE);
		DrawCircleAA(m_stageInfoVec[i].m_pos.x,m_stageInfoVec[i].m_pos.y,20,10,inColor,TRUE);
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

void StageSelectScene::DrawBack()const{
	const size_t index=m_uiControledData->stageIndex;
	if(index>=12 && index<16){
		//"4_1"~"4_4"では夜背景
		DrawGraph(0,0,m_backNightPic,TRUE);
	} else if(index==16){
		//"4_5"では朝背景
		DrawGraph(0,0,m_backMorningPic,TRUE);
	} else{
		//それ以外はデフォルト背景
		DrawGraph(0,0,m_backDefaultPic,TRUE);
	}
}
