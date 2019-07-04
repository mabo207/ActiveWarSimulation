#ifndef DEF_GRAPH_H
#define DEF_GRAPH_H

#include<string>
#include<map>
#include<vector>
#include<mutex>
#include<thread>

namespace Resource{
	//画像データを管理するクラス
	//描画処理は書かない
	//スレッドセーフではない
	class Graph{
		//静的メンバ(Graphを管理するクラス)
	public:
		static int Load(const std::string &fileName);
		static int Copy(int handle);
		static void Delete(int handle);
		static void Clear();

	private:
		//画像をロードする際に必要な情報
		struct LoadInfo{
			const std::string fileName;
			explicit LoadInfo(const std::string &i_fileName):fileName(i_fileName){}
			~LoadInfo(){}
			Graph Load()const;
			bool operator<(const LoadInfo &other)const{
				return this->fileName<other.fileName;
			}
			bool operator==(const LoadInfo &other)const{
				return this->fileName==other.fileName;
			}
		};

		static int Load(const LoadInfo &info);

		static std::map<LoadInfo,std::pair<Graph,size_t>> s_graphMap;//読み込まれているグラフィックデータ一覧

		//メンバ
	public:
		int GetHandle()const;
		~Graph();

	private:
		explicit Graph(int i_handle);
		bool operator==(const Graph &otherobj)const{
			return (this->handle==otherobj.handle);
		}

		const int handle;//グラフィックハンドル
	};
}

#endif // !DEF_GRAPH_H
