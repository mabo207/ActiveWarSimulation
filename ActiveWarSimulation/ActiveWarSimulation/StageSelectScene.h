#ifndef DEF_STAGESELECTSCENE_H
#define DEF_STAGESELECTSCENE_H

#include"GameScene.h"
#include<vector>
#include<string>
#include"input.h"

class StageSelectScene:public GameScene{
	//型・列挙体
private:
	struct StageInfo{
		int m_mapPic;//マップグラフィック(縮小表示)(push_back()の際にデストラクタが呼ばれグラフィックが消されるので、削除はデストラクタでは行わない。どうしてもデストラクタでしたくなったら、コピーコンストラクタを作って再度CopyGraph()をしよう。)
		std::string m_dirName;//ディレクトリ名
		std::string m_stageName;//ステージ名
		std::string m_explain;//ステージ説明文
		StageInfo(const int mapPic,const std::string &dirName,const std::string &stageName,const std::string &explain)
			:m_mapPic(mapPic),m_dirName(dirName),m_stageName(stageName),m_explain(explain){}
		~StageInfo();
	};

	//定数

	//変数
protected:
	size_t m_selectStageIndex;//選択中のステージ
	std::vector<StageInfo> m_stageInfoVec;//ステージ一覧情報
	std::shared_ptr<MainControledGameScene::RequiredInfoToMakeClass> *const m_pReqInfo;
	
	//マウスでクリックできるボタン群
	const MouseButtonUI m_beforeStageButton;
	const MouseButtonUI m_afterStageButton;
	const MouseButtonUI m_backButton;
	const MouseButtonUI m_playButton;

	//グラフィック等
	const int m_backPic;
	const int m_stageNameFont;
	const int m_explainFont;
	
	//関数
protected:

public:
	StageSelectScene(std::shared_ptr<MainControledGameScene::RequiredInfoToMakeClass> *const pReqInfo);
	~StageSelectScene();
	int Calculate();
	void Draw()const;
};

#endif // !DEF_STAGESELECTSCENE_H

