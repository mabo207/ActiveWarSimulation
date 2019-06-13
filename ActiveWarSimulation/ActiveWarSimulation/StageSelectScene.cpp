#include"StageSelectScene.h"
#include"DxLib.h"
#include"GraphicControl.h"
#include"FileRead.h"
#include<Windows.h>
#include"CommonConstParameter.h"
#include"GeneralPurposeResourceManager.h"
#include"FilePath.h"

#include"BattleScene.h"
#include"TitleScene.h"

#include"StageSelectUIInStageSelect.h"

//----------------------StageSelectScene------------------
std::shared_ptr<GameScene> StageSelectScene::StageSelectSceneFactory::CreateScene()const{
	return std::shared_ptr<GameScene>(new StageSelectScene());
}

StageSelectScene::StageSelectScene()
	:m_nextSceneName(NextSceneName::e_title)
	,m_backPic(LoadGraphEX(FilePath::graphicDir+"nonfree/titleScene.png"))
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
	m_ui=std::shared_ptr<StageSelectUIInStageSelect>(new StageSelectUIInStageSelect(m_uiControledData,m_stageInfoVec.size(),m_stageInfoVec));
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
	if(updateResult==BaseUIInStageSelect::UpdateResult::e_nextUI){
		m_ui=m_ui->GetNextUI(m_uiControledData);
	} else if(updateResult==BaseUIInStageSelect::UpdateResult::e_gotoBattle){
		//ゲームプレイへ進む
		m_nextSceneName=NextSceneName::e_battle;
		return -1;
	} else if(updateResult==BaseUIInStageSelect::UpdateResult::e_gotoTitle){
		//タイトル画面へ戻る
		m_nextSceneName=NextSceneName::e_title;
		return -1;
	}

	return 0;
}

void StageSelectScene::Draw()const{
	//背景の描画
	DrawGraph(0,0,m_backPic,TRUE);
	//暗めに描画するために、上から黒長方形を半透明で重ねる
	int mode,pal;
	GetDrawBlendMode(&mode,&pal);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA,128);
	DrawBox(0,0,CommonConstParameter::gameResolutionX,CommonConstParameter::gameResolutionY,GetColor(0,0,0),TRUE);
	SetDrawBlendMode(mode,pal);
	//ステージ一覧の描画
	for(const StageInfoInStageSelect &info:m_stageInfoVec){
		DrawCircleAA(info.m_pos.x,info.m_pos.y,30,10,GetColor(0,0,255),TRUE);
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
			,StageLevel::e_easy);//難易度はひとまず選択できないように
		return CreateFadeOutInScene(thisSharedPtr,battleFactory,15,15);
	}
	return std::shared_ptr<GameScene>();
}
