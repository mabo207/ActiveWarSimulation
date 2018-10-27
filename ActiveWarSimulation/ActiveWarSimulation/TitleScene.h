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
			,e_demo
			,e_gameFinish
			,COUNTER
		};
		static std::string GetString(const Kind kind);
	};
public:
	struct RequiredInfoToMakeTitleScene:public RequiredInfoToMakeClass{
		//クラスを作るのに必要なデータはない
		RequiredInfoToMakeTitleScene(){}
		Kind GetKind()const{
			return e_titleScene;
		}
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

	//他のクラスを作るのに必要なデータ
	std::shared_ptr<RequiredInfoToMakeClass> m_reqInfo;//こいつへのポインタを渡すことで、他のクラスでもこの値を弄れる。それを用いて、このクラスから戻る時に、次どのクラスに行けばいいかなどが計算できる。

	//グラフィック
	const int m_backPic;//背景
	const int m_titleLogo;//タイトルロゴ
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
	~TitleScene();
	int Calculate();
	void Draw()const;
	std::shared_ptr<MainControledGameScene> VGetNextMainControledScene()const;
};

#endif // !DEF_TITLESCENE_H
