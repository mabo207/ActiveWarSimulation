#ifndef DEF_RESOURCE_BGM_H
#define DEF_RESOURCE_BGM_H

#include<string>
#include<map>
#include<vector>

namespace Resource{
	//BGMデータを管理するクラス
	//描画処理は書かない
	//スレッドセーフではない
	class BGM{
		//静的メンバ(BGMを管理するクラス)
	public:
		static BGM Load(const std::string &dirName);
		static void Clear();

	private:
		//画像をロードする際に必要な情報
		struct LoadInfo{
			const std::string dirName;
			explicit LoadInfo(const std::string &i_dirName):dirName(i_dirName){}
			~LoadInfo()=default;
			BGM Load()const;
			bool operator<(const LoadInfo &other)const{
				return this->dirName<other.dirName;
			}
			bool operator==(const LoadInfo &other)const{
				return this->dirName==other.dirName;
			}
		};

		//LoadInfoを用いて読み込み
		static BGM Load(const LoadInfo &info);

		//読み込まれているBGM一覧
		static std::map<LoadInfo,std::pair<BGM,size_t>> s_bgmMap;
		//エラー値
		static const BGM errorObject;

		//メンバ
	public:
		int GetHandle()const;//再生以外の処理がしたくなりハンドルが必要になった時に用いる
		void SetOption()const;//volume,loopTop,loopButtomをハンドルに設定する
		void SetAndPlay(int playType,int topPositionFlag)const;//SetOption()した後に再生
		bool IsError()const;
		BGM Copy()const;
		void Delete()const;
		~BGM()=default;

	private:
		BGM(int i_handle,int i_volume,int i_loopTop,int i_loopBottom);//再生直前にvolume等を設定する事に注意
		bool operator==(const BGM &otherobj)const;

		int handle;//サウンドハンドル
		int volume;
		int loopTop;
		int loopBottom;
	};
}


#endif // !DEF_RESOURCE_BGM_H
