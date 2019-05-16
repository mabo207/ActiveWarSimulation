#include"StageSelectScene.h"
#include"DxLib.h"
#include"GraphicControl.h"
#include"FileRead.h"
#include<Windows.h>
#include"BattleScene.h"
#include"CommonConstParameter.h"
#include"GeneralPurposeResourceManager.h"
#include"ToolsLib.h"

//----------------------StageSelectScene------------------
StageSelectScene::StageInfo::StageInfo(const int mapPic,const std::string &dirName,const std::string &stageInfo,const std::string &explain)
	:m_mapPic(mapPic),m_dirName(dirName),m_explain(explain)
{
	//stageInfo.txtの生データを読み込み可能な形式に変換
	const StringBuilder infoBuilder(stageInfo,',','(',')',true,true);
	for(const StringBuilder &sb:infoBuilder.GetVector()){
		if(sb.m_vec.size()>=2){
			if(sb.m_vec[0].GetString()=="title"){
				m_stageName=sb.m_vec[1].GetString();
			} else if(sb.m_vec[0].GetString()=="level"){
				m_level=std::atoi(sb.m_vec[1].GetString().c_str());
			}
		}
	}
}

StageSelectScene::StageInfo::~StageInfo(){
	//DeleteGraphEX(m_mapPic);
}

StageSelectScene::StageSelectScene(std::shared_ptr<MainControledGameScene::RequiredInfoToMakeClass> *const pReqInfo)
	:m_pReqInfo(pReqInfo)
	,m_beforeStageButton(100,300,LoadGraphEX("Graphic/beforeItem.png"))
	,m_afterStageButton(1770,300,LoadGraphEX("Graphic/afterItem.png"))
	,m_backButton(60,940,LoadGraphEX("Graphic/backButton.png"))
	,m_playButton(900,940,LoadGraphEX("Graphic/combatButton.png"))
	,m_backPic(LoadGraphEX("Graphic/nonfree/titleScene.png"))
	,m_stageNameFont(CreateFontToHandleEX("メイリオ",32,2,-1))
	,m_explainFont(CreateFontToHandleEX("メイリオ",24,1,-1))
{
//	FpsMeasuring fps;
	//フォルダを検索
	char cdir[1024];
	GetCurrentDirectory(1024,cdir);
	const std::string cdir_str(cdir);
	WIN32_FIND_DATA find_dir_data;
	HANDLE hFind=FindFirstFile((cdir_str+"/Stage/*").c_str(),&find_dir_data);
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
	//各フォルダの中身を検索して、StageInfoを構成していく
	for(const std::string &dirName:dirNameVec){
		if(dirName!="demo" && dirName!="tutorial" && dirName!="tutorial_2"){
			m_stageInfoVec.push_back(StageInfo(
				LoadGraphEX(("Stage/"+dirName+"/nonfree/minimap.png").c_str())
				,dirName
				,FileStrRead(("Stage/"+dirName+"/stageInfo.txt").c_str())
				,FileStrRead(("Stage/"+dirName+"/explain.txt").c_str())
			));
		}
	}
	//インデックスの初期化
	if(m_stageInfoVec.empty()){
		m_selectStageIndex=-1;
	} else{
		m_selectStageIndex=0;
	}
}

StageSelectScene::~StageSelectScene(){
	//グラフィックの解放
	DeleteGraphEX(m_backPic);
	for(const StageInfo &info:m_stageInfoVec){
		DeleteGraphEX(info.m_mapPic);
	}
	//フォントの解放
	DeleteFontToHandleEX(m_stageNameFont);
	DeleteFontToHandleEX(m_explainFont);
	//音の解放

}

int StageSelectScene::Calculate(){
	//選択ステージの更新
	if(m_selectStageIndex!=-1){
		//-1の時は読み込んでいるステージがないので更新はできない
		const size_t beforeIndex=m_selectStageIndex;//効果音再生の可否判定に用いる
		const size_t infoSize=m_stageInfoVec.size();
		if(keyboard_get(KEY_INPUT_LEFT)==1 || m_beforeStageButton.JudgePressMoment()){
			m_selectStageIndex=(m_selectStageIndex+infoSize-1)%infoSize;
		} else if(keyboard_get(KEY_INPUT_RIGHT)==1 || m_afterStageButton.JudgePressMoment()){
			m_selectStageIndex=(m_selectStageIndex+1)%infoSize;
		}
		if(m_selectStageIndex!=beforeIndex){
			//変更があれば効果音再生
			PlaySoundMem(GeneralPurposeResourceManager::selectSound,DX_PLAYTYPE_BACK,TRUE);
		}
	}

	//遷移系ボタンの処理
	if(m_selectStageIndex!=-1 &&
		(keyboard_get(KEY_INPUT_Z)==1 || m_playButton.JudgePressMoment())
		)
	{
		//ゲームプレイへ進む
		if(m_pReqInfo!=nullptr){
			*m_pReqInfo=std::shared_ptr<MainControledGameScene::RequiredInfoToMakeClass>(new BattleScene::RequiredInfoToMakeBattleScene(m_stageInfoVec[m_selectStageIndex].m_dirName));//ゲームプレイ場面を作るのに必要な情報は渡しておく
		}
		PlaySoundMem(GeneralPurposeResourceManager::decideSound,DX_PLAYTYPE_BACK,TRUE);//決定の効果音
		return -1;
	} else if(keyboard_get(KEY_INPUT_X)==1 || m_backButton.JudgePressMoment()){
		//タイトル画面へ戻る
		PlaySoundMem(GeneralPurposeResourceManager::cancelSound,DX_PLAYTYPE_BACK,TRUE);//戻るの効果音(鳴ると同時にデータが消えるので鳴らない)
		return -2;
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
	//ボタンの描画
	m_beforeStageButton.DrawButton();
	m_afterStageButton.DrawButton();
	m_backButton.DrawButton();
	m_playButton.DrawButton();
	//ステージ情報の描画
	if(m_selectStageIndex!=-1){
		int stageDx,stageDy;
		const int explainX=400;
		GetGraphSize(m_stageInfoVec[m_selectStageIndex].m_mapPic,&stageDx,&stageDy);
		DrawRotaGraph(CommonConstParameter::gameResolutionX/2,CommonConstParameter::gameResolutionY/3,((double)CommonConstParameter::gameResolutionY/2)/stageDy,0.0,m_stageInfoVec[m_selectStageIndex].m_mapPic,TRUE);
		DrawStringCenterBaseToHandle(CommonConstParameter::gameResolutionX/2,CommonConstParameter::gameResolutionY*3/5,m_stageInfoVec[m_selectStageIndex].m_stageName.c_str(),GetColor(255,255,255),m_stageNameFont,false);
		DrawStringNewLineToHandle(explainX,CommonConstParameter::gameResolutionY*2/3,CommonConstParameter::gameResolutionX-explainX*2,CommonConstParameter::gameResolutionY/4,m_stageInfoVec[m_selectStageIndex].m_explain.c_str(),GetColor(255,255,255),m_explainFont);
	}
}
