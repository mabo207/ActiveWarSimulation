#ifndef DEF_BATTLESCENEELEMENT_H
#define DEF_BATTLESCENEELEMENT_H

#include<memory>

//バトルパートの各場面に用いる仮想基底クラス
//場面情報と普遍的な遷移アルゴリズムが内包されている
class BattleSceneElement{
	//型・列挙体
public:
	struct SceneKind{
		static const int start=1;
		enum Kind{
			e_move=start,
			e_attackNormal,
			e_switch,
			e_research,
			e_clear,
			e_systemMenu,
			e_turnStartDamage,
			e_gameReset,//これだけクラスがなく、BattleScene内で処理を行う
			END
		};
		static const int kindNum=END-start;
		static Kind link(int num){
			if(num>=start && num<=END){
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
	SceneKind::Kind GetTailSceneKind()const;
	virtual int thisCalculate()=0;//0:直前場面に戻る 1~END-1:その場面に進む END:バトルパート終了
	virtual void thisDraw()const=0;
	virtual int UpdateNextScene(int index)=0;//次場面の設定。indexに戻りたいが、実際に戻れるかは不明なので実際の戻り場所を示すint値を返し直す。基本的に返り値はindexに等しくなる。
	virtual void ReturnProcess()=0;//この場面に戻ってきた時の処理

public:
	virtual ~BattleSceneElement();
	int Calculate();//0:直前場面に戻る 1~END-1:その場面に進む END:バトルパート終了
	void Draw()const;
};


#endif // !DEF_BATTLESCENEELEMENT_H


#pragma once
