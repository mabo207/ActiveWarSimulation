#include"StageSelectScene.h"
#include"DxLib.h"
#include"GraphicControl.h"
#include"FileRead.h"
#include<Windows.h>
#include"CommonConstParameter.h"
#include"GeneralPurposeResource.h"
#include"FilePath.h"

#include"BattleScene.h"
#include"TitleScene.h"

#include"StageSelectUIInStageSelect.h"
#include"LevelSelectUIInStageSelect.h"

//----------------------StageSelectScene------------------
std::shared_ptr<GameScene> StageSelectScene::StageSelectSceneFactory::CreateScene()const{
	return std::shared_ptr<GameScene>(new StageSelectScene());
}

StageSelectScene::StageSelectScene()
	:m_nextSceneName(NextSceneName::e_title)
	,m_backPic(LoadGraphEX(FilePath::graphicDir+"nonfree/stageSelectBack.png"))
	,m_stageNameFont(CreateFontToHandleEX("メイリオ",32,2,-1))
	,m_explainFont(CreateFontToHandleEX("メイリオ",24,1,-1))
	,m_uiControledData(new BaseUIInStageSelect::ControledData(0,StageLevel::e_easy))
{
	//スコアデータの読み込み
	const ScoreRankingData rankingData;
	//フォルダを検索
	char cdir[1024];
	GetCurrentDirectory(1024,cdir);
	const std::string cdir_str(cdir);
	WIN32_FIND_DATA find_dir_data;
	HANDLE hFind=FindFirstFile((cdir_str+"/"+FilePath::stageDir+"/*").c_str(),&find_dir_data);
	auto GetFileName=[](WIN32_FIND_DATA data){
		//find_dir_dataのファイル名をstd::string型に変換する関数
		std::string s;
		s.reserve(260);//配列を見る限り、ファイル名の長さが260字いないらしい
		for(int i=0;i<260;i++){
			if(data.cFileName[i]!='\0'){
				s.push_back(data.cFileName[i]);
			} else{
				//s.push_back('\0');//これは入れてはいけない。string比較の時には邪魔になる。
				//例：char[] c="."とすると、c.size=2,c[0]='.',c[1]='\0'。std::string c="."とすると、c.size=1,c[0]='.'。char[]だと終端文字は認識され、stringだと終端文字は無視される。
				break;
			}
		}
		return s;
	};
	std::vector<std::string> dirNameVec;
	do{
		if(hFind!=INVALID_HANDLE_VALUE){
			const std::string filename=GetFileName(find_dir_data);
			if(filename!="." && filename!=".."){
				if(find_dir_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY){
					//フォルダである
					dirNameVec.push_back(filename);
				} else{
					//ファイルである
					//特に何もしない
				}
			}
		}
	} while(FindNextFile(hFind,&find_dir_data));
	//各フォルダの中身を検索して、StageInfoInStageSelectを構成していく
	for(const std::string &dirName:dirNameVec){
		if(dirName!="demo" && dirName!="tutorial" && dirName!="tutorial_2"){
			m_stageInfoVec.push_back(StageInfoInStageSelect(
				LoadGraphEX((FilePath::stageDir+dirName+"/nonfree/minimap.png").c_str())
				,dirName
				,FileStrRead((FilePath::stageDir+dirName+"/explain.txt").c_str())
				,rankingData
			));
		}
	}
	//UIの作成
	m_ui=std::shared_ptr<StageSelectUIInStageSelect>(new StageSelectUIInStageSelect(m_uiControledData,m_stageInfoVec,m_stageNameFont,m_explainFont));
}

StageSelectScene::~StageSelectScene(){
	//グラフィックの解放
	DeleteGraphEX(m_backPic);
	for(const StageInfoInStageSelect &info:m_stageInfoVec){
		DeleteGraphEX(info.m_mapPic);
	}
	//フォントの解放
	DeleteFontToHandleEX(m_stageNameFont);
	DeleteFontToHandleEX(m_explainFont);
	//音の解放

}

int StageSelectScene::Calculate(){
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
		m_ui=std::shared_ptr<BaseUIInStageSelect>(new StageSelectUIInStageSelect(m_uiControledData,m_stageInfoVec,m_stageNameFont,m_explainFont));
	} else if(updateResult==BaseUIInStageSelect::UpdateResult::e_gotoLevelSelect){
		//レベルセレクトにUI遷移
		m_ui=std::shared_ptr<BaseUIInStageSelect>(new LevelSelectUIInStageSelect(m_uiControledData,m_stageInfoVec[m_uiControledData->stageIndex],m_stageNameFont,m_explainFont));
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
			const int num=(int)(dist/(circleSize*6.0f));
			//描画
			for(int j=1;j<num;j++){
				//ここの中身はnum>0の時のみ処理されるので、numの0チェックは必要ない
				const Vector2D pos=m_stageInfoVec[i].m_pos+dir*((float)j)/((float)num);
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
}

std::shared_ptr<GameScene> StageSelectScene::VGetNextScene(const std::shared_ptr<GameScene> &thisSharedPtr)const{
	if(m_nextSceneName==NextSceneName::e_title){
		const auto titleFactory=std::make_shared<TitleScene::TitleSceneFactory>();
		return CreateFadeOutInScene(thisSharedPtr,titleFactory,15,15);
	} else if(m_nextSceneName==NextSceneName::e_battle){
		const std::shared_ptr<GameScene::SceneFactory> battleFactory=std::make_shared<BattleScene::BattleSceneFactory>(
			m_stageInfoVec[m_uiControledData->stageIndex].m_dirName
			,m_stageInfoVec[m_uiControledData->stageIndex].m_titleName
			,m_uiControledData->selectLevel);
		return CreateFadeOutInScene(thisSharedPtr,battleFactory,15,15);
	}
	return std::shared_ptr<GameScene>();
}
