#ifndef DEF_DEMOSCENE_H
#define DEF_DEMOSCENE_H

#include"BattleScene.h"

//デモを行うクラス(コンストラクタでBattleScene.cppにてSwitchUnitSceneだったところを変えれば良いだけ)
class DemoScene:public BattleScene{
	//型・列挙体
public:

	//定数
protected:

public:

	//変数
protected:

   //関数
protected:
	int m_flame;//DemoScene専用のフレーム数カウンタ
	int m_font;//注意書きのフォント

public:
	DemoScene();
	~DemoScene();
	
	//仮想関数のオーバーライド
	std::shared_ptr<BattleSceneElement> VGetSwitchUnitScene()const;//SwitchUnitSceneかDemoSwitchUnitSceneのどっちを使うか
	int Calculate();
	void Draw()const;

};

#endif // !DEF_DEMOSCENE_H
