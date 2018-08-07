#include"StageSelectScene.h"
#include"DxLib.h"
#include"GraphicControl.h"
#include"FileRead.h"
#include<Windows.h>

//----------------------StageSelectScene------------------
StageSelectScene::StageInfo::~StageInfo(){
	//DeleteGraphEX(m_mapPic);
}

StageSelectScene::StageSelectScene()
	:m_beforeStageButton(100,300,LoadGraphEX("Graphic/beforeItem.png"))
	,m_afterStageButton(100,300,LoadGraphEX("Graphic/afterItem.png"))
	,m_backButton(60,940,LoadGraphEX("Graphic/backButton.png"))
	,m_playButton(900,940,LoadGraphEX("Graphic/playButton.png"))
	,m_backPic(LoadGraphEX("Graphic/titleScene.png"))
	,m_stageNameFont(CreateFontToHandleEX("メイリオ",32,2,-1))
	,m_explainFont(CreateFontToHandleEX("メイリオ",24,1,-1))
{
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
		m_stageInfoVec.push_back(StageInfo(
			LoadGraphEX(("Stage/"+dirName+"/map.png").c_str())
			,dirName
			,FileStrRead(("Stage/"+dirName+"/stageName.txt").c_str())
			,FileStrRead(("Stage/"+dirName+"/explain.txt").c_str())
		));
	}
	//インデックスの初期化
	if(m_stageInfoVec.empty()){
		m_selectStageIndex=-1;
	} else{
		m_selectStageIndex=0;
	}
}

StageSelectScene::~StageSelectScene(){
	DeleteGraphEX(m_backPic);
	for(const StageInfo &info:m_stageInfoVec){
		DeleteGraphEX(info.m_mapPic);
	}
	DeleteFontToHandleEX(m_stageNameFont);
	DeleteFontToHandleEX(m_explainFont);
}

int StageSelectScene::Calculate(){
	//選択ステージの更新
	if(m_selectStageIndex!=-1){
		//-1の時は読み込んでいるステージがないので更新はできない
		const size_t infoSize=m_stageInfoVec.size();
		if(keyboard_get(KEY_INPUT_LEFT)==1 || m_beforeStageButton.JudgePressMoment()){
			m_selectStageIndex=(m_selectStageIndex+infoSize-1)%infoSize;
		} else if(keyboard_get(KEY_INPUT_RIGHT)==1 || m_afterStageButton.JudgePressMoment()){
			m_selectStageIndex=(m_selectStageIndex+1)%infoSize;
		}
	}

	return 0;
}

void StageSelectScene::Draw()const{
	const std::pair<int,int> resolution=GetWindowResolution();
	//背景の描画
	DrawGraph(0,0,m_backPic,TRUE);
	//暗めに描画するために、上から黒長方形を半透明で重ねる
	int mode,pal;
	GetDrawBlendMode(&mode,&pal);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA,128);
	DrawBox(0,0,resolution.first,resolution.second,GetColor(0,0,0),TRUE);
	SetDrawBlendMode(mode,pal);
	//ボタンの描画
	m_beforeStageButton.DrawButton();
	m_afterStageButton.DrawButton();
	m_backButton.DrawButton();
	m_playButton.DrawButton();
	//ステージ情報の描画
	if(m_selectStageIndex!=-1){
		int stageDx,stageDy;
		GetGraphSize(m_stageInfoVec[m_selectStageIndex].m_mapPic,&stageDx,&stageDy);
		DrawRotaGraph(resolution.first/2,resolution.second*2/5,540.0/stageDx,0.0,m_stageInfoVec[m_selectStageIndex].m_mapPic,TRUE);
		DrawStringCenterBaseToHandle(resolution.first/2,resolution.second*3/5,m_stageInfoVec[m_selectStageIndex].m_stageName.c_str(),GetColor(255,255,255),m_stageNameFont,false);
		DrawStringNewLineToHandle(200,resolution.second*2/3,resolution.first-200*2,resolution.second/4,m_stageInfoVec[m_selectStageIndex].m_explain.c_str(),GetColor(255,255,255),m_explainFont);
	}
}
