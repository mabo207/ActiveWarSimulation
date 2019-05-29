#ifndef DEF_TITLESCENE_H
#define DEF_TITLESCENE_H

#include"GameScene.h"
#include"ToolsLib.h"
#include"Shape.h"
#include<memory>
#include<array>
#include<string>

//タイトル画面のクラス
class TitleScene:public GameScene{
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
	//GameSceneを継承しているためクラスをfactoryクラスを作る必要がある
	class TitleSceneFactory:public SceneFactory{
	public:
		//クラスを作るのに必要なデータはない
		TitleSceneFactory();
		virtual ~TitleSceneFactory();
		std::shared_ptr<GameScene> CreateScene()const;
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

protected:
	TitleScene();

public:
	virtual ~TitleScene();
	int Calculate();
	void Draw()const;
	std::shared_ptr<GameScene> VGetNextScene()const;
};

#endif // !DEF_TITLESCENE_H
