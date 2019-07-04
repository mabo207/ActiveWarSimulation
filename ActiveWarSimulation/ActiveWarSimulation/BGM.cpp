#include"DxLib.h"
#include"BGM.h"
#include"FileRead.h"
#include<optional>
#include"StringBuilder.h"

//--------------Resource::BGM::LoadInfo-----------------
std::map<Resource::BGM::LoadInfo,std::pair<Resource::BGM,size_t>> Resource::BGM::s_bgmMap;
const Resource::BGM Resource::BGM::errorObject(-1,-1,-1,-1);

Resource::BGM Resource::BGM::LoadInfo::Load()const{
	int loopTop=-1;
	int loopBottom=-1;
	int volume=255;
	std::optional<std::string> fileName;
	//ループ位置の設定。info.txtに「ループ開始位置」「ループ終了位置」「音量%」「ファイル名」が格納されている
	StringBuilder builder(FileStrRead((dirName+"info.txt").c_str()),',','(',')');
	for(StringBuilder &sb:builder.m_vec){
		try{
			//std::stoiを用いるので例外処理を用意
			if(sb.m_vec.size()>2 && sb.m_vec[0].GetString()=="top"){
				loopTop=std::stoi(sb.m_vec[1].GetString());
			} else if(sb.m_vec.size()>2 && sb.m_vec[0].GetString()=="bottom"){
				loopBottom=std::stoi(sb.m_vec[1].GetString());
			} else if(sb.m_vec.size()>2 && sb.m_vec[0].GetString()=="volume"){
				volume=std::stoi(sb.m_vec[1].GetString());
			} else if(sb.m_vec.size()>2 && sb.m_vec[0].GetString()=="fileName"){
				fileName=dirName+sb.m_vec[1].GetString();
			}
		} catch(const std::invalid_argument &){
			//数値じゃないものを検出した場合
		} catch(const std::out_of_range &){
			//表現の範囲外の数値を検出した場合
		}
	}
	//ファイル名が発見された場合とそうでない場合がある
	if(fileName){
		return BGM(LoadSoundMem(fileName.value().c_str()),volume,loopTop,loopBottom);
	} else{
		return errorObject;
	}
}

//--------------Resource::BGM----------------------
Resource::BGM Resource::BGM::Load(const std::string &fileName){
	const LoadInfo info(fileName);
	return info.Load();
}

Resource::BGM Resource::BGM::Load(const LoadInfo &info){
	const auto it=s_bgmMap.find(info);
	if(it==s_bgmMap.end()){
		//まだロードされていない画像であるなら、新たに読み込む
		const BGM bgm=info.Load();
		if(!bgm.IsError()){
			//エラー値でなければ追加
			std::pair<BGM,size_t> inp(bgm,1);
			s_bgmMap.insert(std::pair<LoadInfo,std::pair<BGM,size_t>>(info,inp));
		}
		return bgm;
	} else{
		//既に読み込みされているなら、カウントを増やす
		it->second.second++;
		return it->second.first;
	}
}

void Resource::BGM::Clear(){
	//mainスレッド以外からこの処理が呼ばれたら何もせずに返す
	//s_bgmMap全てに対して削除処理
	for(const auto &data:s_bgmMap){
		DeleteSoundMem(data.second.first.GetHandle());
	}
	s_bgmMap.clear();
}

int Resource::BGM::GetHandle()const{
	return handle;
}

void Resource::BGM::SetOption()const{
	if(loopBottom>=0){
		SetLoopStartSamplePosSoundMem(loopBottom,handle);//ループが始まる位置の設定、ループ区間の後ろ端になる。
	}
	if(loopTop>=0){
		SetLoopPosSoundMem(loopTop,handle);//ループ位置の設定、ループするとどこに飛ぶかなのでループ区間の前端になる。
	}
	ChangeVolumeSoundMem(volume,handle);//ボリュームを0~255で設定
}

void Resource::BGM::SetAndPlay(int playType,int topPositionFlag)const{
	//設定
	SetOption();
	//再生
	PlaySoundMem(handle,playType,topPositionFlag);
}

bool Resource::BGM::IsError()const{
	return (handle<0);
}

Resource::BGM Resource::BGM::Copy()const{
	//instanceと同じデータを探して、参照個数を1増やす
	for(auto &data:s_bgmMap){
		if(data.second.first==*this){
			data.second.second++;
			break;
		}
	}
	return *this;
}

void Resource::BGM::Delete()const{
	//instanceと同じデータを探して、削除処理をする
	bool findFlag=false;//削除処理をしたかどうか
	for(auto it=s_bgmMap.begin(),ite=s_bgmMap.end();it!=ite;it++){
		if(it->second.first==*this){
			if(it->second.second<=1){
				//参照箇所が1箇所だけだった場合は、DeleteSoundMem()をしてs_bgmMapからも削除
				DeleteSoundMem(handle);
				s_bgmMap.erase(it);
			} else{
				//参照箇所が2箇所以上だった場合は、カウントを1減らす
				it->second.second--;
			}
			findFlag=true;
			break;
		}
	}
	//データが見つからなかったら、DeleteSoundMem()だけしておく
	if(!findFlag){
		DeleteSoundMem(handle);
	}
}

Resource::BGM::BGM(int i_handle,int i_volume,int i_loopTop,int i_loopBottom)
	:handle(i_handle)
	,volume(i_volume)
	,loopTop(i_loopTop)
	,loopBottom(i_loopBottom)
{}

bool Resource::BGM::operator==(const BGM &otherobj)const{
	return (this->handle==otherobj.handle
		&& this->volume==otherobj.volume
		&& this->loopTop==otherobj.loopTop
		&& this->loopBottom==otherobj.loopBottom);
}
