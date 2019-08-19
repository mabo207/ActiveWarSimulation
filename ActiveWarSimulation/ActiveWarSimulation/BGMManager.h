#ifndef DEF_BGMMANAGER_H
#define DEF_BGMMANAGER_H

#include"BGM.h"
#include<optional>

//BGMの再生を管理するクラス
//インスタンスは１つにしたいので、staticな静的メンバを持ち、そこにアクセスする
class BGMManager{
public:
	//再生中のものと異なる場合、originBGMをCopyして、Copyしたものを再生する
	void PlayWithCopy(const Resource::BGM &originBGM);
	//再生中のBGMを止め、m_nowPlayingBGMを解放する
	void Stop();
	//再生中のBGMの情報をコピーして取得する
	Resource::BGM GetCopiedBGM()const;
	//デストラクタ
	~BGMManager();

	//インスタンス初期化・解放
	static void Init();
	static void Release();
	
	//インスタンスは１つ以下だけ存在するようにする(Dxlibの初期化後にインスタンスを作成し、Dxlibの解放前にインスタンスを解放したい)
	static std::optional<BGMManager> s_instance;

private:
	BGMManager();

	Resource::BGM m_nowPlayingBGM;//再生中のBGM
};

#endif // !DEF_BGMMANAGER_H
