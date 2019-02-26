#ifndef DEF_TUTORIALBATTLESCENEDATA_H
#define DEF_TUTORIALBATTLESCENEDATA_H

#include"BattleSceneData.h"
#include"BattleSceneElement.h"

struct TutorialBattleSceneData:public BattleSceneData{
	//列挙体・型
public:
	//チュートリアルデータの基底クラス、MoveSceneにあるデータを数多く用いる必要があるのでこのクラスにチュートリアル課題クリアを判定する関数は用意しない
	struct TutorialBase{
		enum class TutorialKind{
			e_move
			,e_normalAttack
			,e_wait
		};
		const TutorialKind m_kind;
		TutorialBase(TutorialKind kind):m_kind(kind){}
		virtual ~TutorialBase(){}
		virtual void DrawSupplement(int font)const=0;//補足説明を描画する
	};
	//移動に関するチュートリアルデータを管理するクラス
	struct MoveTutorial:public TutorialBase{
		const std::shared_ptr<Shape> m_moveTutorialArea;//どこに移動すれば良いか（移動ユニットの中心点がこの図形の中に入れば良い）
		MoveTutorial(std::shared_ptr<Shape> moveTutorialArea);
		~MoveTutorial();
		void DrawSupplement(int font)const;
	};
	//攻撃に関するチュートリアルデータを管理するクラス
	struct AttackTutorial:public TutorialBase{
		const Unit *m_targetUnit;//どのユニットを攻撃すればよいか
		AttackTutorial(const Unit *targetUnit);
		~AttackTutorial();
		void DrawSupplement(int font)const;
	};
	//待機に関するチュートリアルを管理するクラス
	struct WaitTutorial:public TutorialBase{
		WaitTutorial();
		~WaitTutorial();
		void DrawSupplement(int font)const;
	};

	//定数

	//変数
public:
	std::vector<std::shared_ptr<TutorialBase>> m_tutorialData;//どのようなチュートリアルを提供するか

	//フォント
	const int m_tutorialFont;//チュートリアルの補足説明に用いるフォント

	//関数
protected:

public:
	TutorialBattleSceneData();
	~TutorialBattleSceneData();


};


#endif // !DEF_TUTORIALBATTLESCENEDATA_H
