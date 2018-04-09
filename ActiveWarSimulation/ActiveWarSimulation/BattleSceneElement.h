#ifndef DEF_BATTLESCENEELEMENT_H
#define DEF_BATTLESCENEELEMENT_H

#include"GameScene.h"
#include<memory>

//バトルパートの各場面に用いる仮想基底クラス
//場面情報と普遍的な遷移アルゴリズムが内包されている
class BattleSceneElement:public GameScene{
	//型・列挙体
public:
	struct SceneKind{
		enum Kind{
			e_move,
			e_attackNormal,
			END
		};
		static Kind link(int num){
			if(num>=0 && num<=END){
				return static_cast<Kind>(num);
			}
			return END;
		}
	};

	//定数

	//変数
private:
	const SceneKind::Kind m_sceneKind;
protected:
	std::shared_ptr<BattleSceneElement> m_nextScene;

	//関数
protected:
	BattleSceneElement(SceneKind::Kind sceneKind);
	SceneKind::Kind GetSceneKind()const{
		return m_sceneKind;
	}
	//virtual int thisCalculate()=0;
	//virtual void thisDraw()const=0;

public:
	virtual ~BattleSceneElement();
	//int Calculate();
	//void Draw();
};


#endif // !DEF_BATTLESCENEELEMENT_H


#pragma once
