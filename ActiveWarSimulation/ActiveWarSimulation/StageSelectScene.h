#ifndef DEF_STAGESELECTSCENE_H
#define DEF_STAGESELECTSCENE_H

#include"GameScene.h"
#include<vector>
#include<string>
#include"input.h"
#include"TitleScene.h"

class StageSelectScene:public GameScene{
	//型・列挙体
private:
	struct StageInfo{
		//変数
		int m_mapPic;//マップグラフィック(縮小表示)(push_back()の際にデストラクタが呼ばれグラフィックが消されるので、削除はデストラクタでは行わない。どうしてもデストラクタでしたくなったら、コピーコンストラクタを作って再度CopyGraph()をしよう。)
		std::string m_dirName;//ディレクトリ名
		std::string m_stageName;//ステージ名
		int m_level;//レベル
		std::string m_explain;//ステージ説明文

		//関数
		StageInfo(const int mapPic,const std::string &dirName,const std::string &stageInfo,const std::string &explain);
		~StageInfo();
		std::string GetLevelStr()const;//難易度を説明する文字列を返す
	};

	//定数

	//変数
protected:
	size_t m_selectStageIndex;//選択中のステージ
	std::vector<StageInfo> m_stageInfoVec;//ステージ一覧情報
	std::weak_ptr<TitleScene::SharedData> m_sharedData;//タイトルシーンの共有情報
	
	//マウスでクリックできるボタン群
	const MouseButtonUI m_beforeStageButton;
	const MouseButtonUI m_afterStageButton;
	const MouseButtonUI m_backButton;
	const MouseButtonUI m_playButton;

	//グラフィック等
	const int m_backPic;
	const int m_stageNameFont;
	const int m_explainFont;
	
	//効果音

	//関数
protected:

public:
	StageSelectScene(const std::weak_ptr<TitleScene::SharedData> &sharedData);
	~StageSelectScene();
	int Calculate();
	void Draw()const;
};

#endif // !DEF_STAGESELECTSCENE_H

