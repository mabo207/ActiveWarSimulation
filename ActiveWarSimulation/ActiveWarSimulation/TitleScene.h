#ifndef DEF_TITLESCENE_H
#define DEF_TITLESCENE_H

#include"GameScene.h"
#include"ToolsLib.h"
#include"Shape.h"
#include<memory>
#include<array>
#include<string>

//タイトル画面のクラス
class TitleScene:public MainControledGameScene{
	//型・列挙体
private:
	struct SelectItem{
		//タイトル画面で選択できる項目
		enum Kind{
			e_stageSelect
			,e_tutorial
			,e_tutorial_2
			,e_demo
			,e_gameFinish
			,COUNTER
		};
		static std::string GetString(const Kind kind);
	};
public:
	//MainControledGameSceneを継承しているためクラスをfactoryクラスを作る必要がある
	class TitleSceneFactory:public MainSceneFactory{
	public:
		//クラスを作るのに必要なデータはない
		TitleSceneFactory();
		virtual ~TitleSceneFactory();
		std::shared_ptr<MainControledGameScene> CreateScene()const;
	};
	//TitleSceneで管理するデータの内、他のシーンクラスに共有したいデータ
	//BattleSceneに対するBattleSceneData
	struct SharedData{
		SharedData();
		virtual ~SharedData();

		std::shared_ptr<MainSceneFactory> m_requiredInfo;//他のmain関数管理下のクラスを作るための情報
	};

	
	//定数
private:
	static const Vector2D strPos[SelectItem::COUNTER];

	//変数
protected:
	int m_frame;//文字にアニメーションをつけるため
	Vector2D m_mousePosJustBefore;//直前フレームにおけるマウスの位置
	SelectItem::Kind m_selectItem;//現在選択している項目
	std::array<std::shared_ptr<Shape>,SelectItem::COUNTER> m_hitJudgeShapeVec;
	std::shared_ptr<GameScene> m_nextScene;//次のシーン。これがnullptrなら、タイトルの処理を行う。

	//TitleSceneから直接遷移したクラスと共有するデータ(他クラスにはweak_ptrで渡すべき。)
	std::shared_ptr<SharedData> m_sharedData;

	//グラフィック
	const int m_backPic;//背景
	const int m_itemFont;//選択項目のフォント

	//bgm
	const int m_bgm;

	//効果音
	const int m_aimchangeSound;

	//関数
private:
	std::shared_ptr<Shape> MakeHexagon(const Vector2D center,const float size)const;
	int thisCalculate();//SelectItem::COUNTER:現状維持 0~SelectItem::COUNTER-1:遷移
	void thisDraw()const;

public:
	TitleScene();
	virtual ~TitleScene();
	int Calculate();
	void Draw()const;
	std::shared_ptr<MainControledGameScene> VGetNextMainControledScene()const;
};

#endif // !DEF_TITLESCENE_H
