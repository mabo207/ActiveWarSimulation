#include"BGMManager.h"
#include"DxLib.h"

//---------------BGMManager----------------
std::optional<BGMManager> BGMManager::s_instance;

void BGMManager::PlayWithCopy(const Resource::BGM &originBGM){
	if(!(m_nowPlayingBGM==originBGM)){
		//既存のBGMを中断・解放
		Stop();
		//新しいBGMをm_nowPlayeingBGMにコピーして再生
		m_nowPlayingBGM=originBGM.Copy();
		m_nowPlayingBGM.SetAndPlay(DX_PLAYTYPE_LOOP,TRUE);
	}
}

void BGMManager::Stop(){
	//既存のBGMを中断・解放
	m_nowPlayingBGM.Stop();
	m_nowPlayingBGM.Delete();
	//m_nowPlayingBGMに再生できないBGMを入れておく
	m_nowPlayingBGM=Resource::BGM::errorObject;
}

Resource::BGM BGMManager::GetCopiedBGM()const{
	return m_nowPlayingBGM.Copy();
}

void BGMManager::Init(){
	s_instance=BGMManager();
}

void BGMManager::Release(){
	s_instance.reset();
}

BGMManager::BGMManager()
	:m_nowPlayingBGM(Resource::BGM::errorObject)
{}

BGMManager::~BGMManager(){
	Stop();
}
