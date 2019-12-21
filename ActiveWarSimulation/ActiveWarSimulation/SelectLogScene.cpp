#include"SelectLogScene.h"
#include"input.h"
#include"DxLib.h"
#include"GeneralPurposeResource.h"
#include"CommonConstParameter.h"
#include"WatchLogScene.h"
#include"TitleScene.h"
#include<Windows.h>

//-----------------SelectLogScene-----------------
void SelectLogScene::InitCompletely(){}

void SelectLogScene::Activate(){}

int SelectLogScene::Calculate(){
	//選択ファイル変更
	const auto recognize=[](int frame){return (frame==1 || (frame>30 && frame%5==0));};
	if(m_selectFileIt!=m_fileNameList.begin() && (recognize(keyboard_get(KEY_INPUT_LEFT)) || recognize(keyboard_get(KEY_INPUT_UP)))){
		m_selectFileIt--;
	} else if(m_selectFileIt!=m_fileNameList.end() && (recognize(keyboard_get(KEY_INPUT_RIGHT)) || recognize(keyboard_get(KEY_INPUT_DOWN)))){
		m_selectFileIt++;
	}
	//場面遷移
	if(keyboard_get(KEY_INPUT_Z)==1 || mouse_get(MOUSE_INPUT_LEFT)==1){
		//選択したファイルのログを見に行く
		return 1;
	} else if(keyboard_get(KEY_INPUT_X)==1 || mouse_get(MOUSE_INPUT_RIGHT)==1){
		//タイトル画面に戻る
		m_selectFileIt=m_fileNameList.end();//何も指していない状態にすることで、タイトルに戻る事を指し示す
		return 1;
	}
	return 0;
}

void SelectLogScene::Draw()const{
	//描画をするファイル名の場所を決める
	const int maxGap=6;
	std::vector<std::string>::const_iterator it=m_selectFileIt;
	int gap=0;
	for(size_t i=0;i<maxGap;i++){
		if(it!=m_fileNameList.begin()){
			it--;
			gap--;
		} else{
			//m_fileNameListの先頭より前に戻らないようにする
			break;
		}
	}
	//選択しているものの前後のファイル名もまとめて描画
	for(;gap<=maxGap;gap++){
		const int centerX=CommonConstParameter::gameResolutionX/2,centerY=CommonConstParameter::gameResolutionY/2+gap*60;
		DrawStringCenterBaseToHandle(centerX,centerY,(it!=m_fileNameList.end()?it->c_str():"タイトル画面に戻る"),GetColor(255,255,255),gap==0?GeneralPurposeResource::popLargeFont:GeneralPurposeResource::gothicMiddleFont,true);
		//表示するファイル名の更新
		if(it==m_fileNameList.end()){
			break;
		} else{
			it++;
		}
	}
}

SelectLogScene::SelectLogScene(){
	//m_fileNameListの初期化（実行パスにあるログファイルをすべて格納する）
	//フォルダを検索
	char cdir[1024];
	GetCurrentDirectory(1024,cdir);
	const std::string cdir_str(cdir);
	WIN32_FIND_DATA find_dir_data;
	HANDLE hFind=FindFirstFile((cdir_str+"/*").c_str(),&find_dir_data);
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
	do{
		if(hFind!=INVALID_HANDLE_VALUE){
			const std::string filename=GetFileName(find_dir_data);
			if(filename!="." && filename!=".."){
				if(find_dir_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY){
					//フォルダである
					//特に何もしない
				} else{
					//ファイルである
					bool firstMatch=true;//先頭が一致しているかどうか
					const std::string firstStr="GameLog_";
					if(filename.size()>=firstStr.size()){
						for(size_t i=0,siz=firstStr.size();i<siz;i++){
							if(firstStr[i]!=filename[i]){
								firstMatch=false;
								break;
							}
						}
					} else{
						firstMatch=false;
					}
					if(firstMatch){
						//filenameの先頭が"GameLog_"に一致しているものだけ格納する
						m_fileNameList.push_back(filename);
					}
				}
			}
		}
	} while(FindNextFile(hFind,&find_dir_data));
	//m_selectFileItの初期化
	m_selectFileIt=m_fileNameList.begin();
}

SelectLogScene::~SelectLogScene(){}

std::shared_ptr<GameScene> SelectLogScene::VGetNextScene(const std::shared_ptr<GameScene> &thisSharedPtr)const{
	if(m_selectFileIt==m_fileNameList.end()){
		return CreateFadeOutInSceneCompletely(thisSharedPtr,std::shared_ptr<GameScene::SceneFactory>(new TitleScene::TitleSceneFactory()),15,15);
	} else{
		return CreateFadeOutInSceneCompletely(thisSharedPtr,std::shared_ptr<GameScene::SceneFactory>(new WatchLogScene::WatchLogSceneFactory(*m_selectFileIt)),15,15);
	}
}
