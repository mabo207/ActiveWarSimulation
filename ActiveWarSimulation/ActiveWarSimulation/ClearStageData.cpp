#include"ClearStageData.h"
#include"StringBuilder.h"
#include"FileRead.h"
#include"FilePath.h"
#include<fstream>

namespace {
	const std::string fileName="clearStageData.txt";
	const std::string idName="dirVec";
}

//--------------ClearStageData--------------
ClearStageData::ClearStageData(){
	//データの読み込み
	try{
		const StringBuilder strBuilder(FileStrRead((FilePath::savedataDir+fileName).c_str()),',','(',')');
		for(const StringBuilder &data:strBuilder.m_vec){
			const size_t vecSize=data.m_vec.size();
			if(vecSize>2 && data.m_vec[0].GetString()==idName){
				//データの該当箇所を見つけた場合、全てのディレクトリ情報を挿入
				for(size_t i=1;i<vecSize;i++){
					m_clearDir.insert(data.m_vec[i].GetString());
				}
			}
		}
	} catch(const FileOpenFailedException &){
		//セーブデータがなかった場合等が考えられる。特に何もしない
	}
}

bool ClearStageData::JudgeClear(const std::string &stageDirName)const{
	return m_clearDir.find(stageDirName)!=m_clearDir.end();
}

void ClearStageData::BecomeClear(const std::string &stageDirName){
	m_clearDir.insert(stageDirName);
}

bool ClearStageData::Save()const{
	//全て書き換えるモードでファイルを開く
	std::ofstream ofs((FilePath::savedataDir+fileName).c_str(),std::ios_base::trunc);
	if(!ofs){
		return false;
	}
	//書き込み
	ofs<<"("<<idName;
	for(const std::string &dirName:m_clearDir){
		ofs<<','<<dirName;
	}
	ofs<<')'<<std::endl;
	//終了処理
	ofs.close();
	return true;
}
