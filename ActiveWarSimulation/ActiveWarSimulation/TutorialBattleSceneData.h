#ifndef DEF_TUTORIALBATTLESCENEDATA_H
#define DEF_TUTORIALBATTLESCENEDATA_H

#include"BattleSceneData.h"
#include"BattleSceneElement.h"
#include"ToolsLib.h"

struct TutorialBattleSceneData:public BattleSceneData{
	//列挙体・型
public:
	//チュートリアルデータの基底クラス、MoveSceneにあるデータを数多く用いる必要があるのでこのクラスにチュートリアル課題クリアを判定する関数は用意しない
	struct TutorialBase{
		enum class TutorialKind{
			e_move
			,e_normalAttack
			,e_wait
			,e_explain
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
	//説明を表示するチュートリアルを管理するクラス
	struct ExplainTutorial:public TutorialBase{
		//PositionControl m_pos;//説明絵の描画中心位置(中央で一旦停止した後更新される)
		Easing m_rate;//説明絵の拡大率(1000が最大)(中央で一旦停止した後更新される)
		const int m_gHandle;//説明絵のデータ
		bool m_secondMoveFlag;//2回目の移動が始まったかどうか(チュートリアル終了の判定に用いる)
		ExplainTutorial(const char *gFileName);
		~ExplainTutorial();
		void DrawSupplement(int font)const;//説明絵の描画を行う
		void SetSecondMoveFlag();//2回目の移動を始められるように設定する
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
