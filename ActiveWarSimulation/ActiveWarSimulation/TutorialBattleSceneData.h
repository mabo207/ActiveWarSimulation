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
			,e_blank
		};
		const TutorialKind m_kind;
		TutorialBase(TutorialKind kind):m_kind(kind){}
		virtual ~TutorialBase(){}
		virtual void DrawSupplement(int font)const=0;//補足説明を描画する
		static std::shared_ptr<TutorialBase> Create(const std::string &str,const BattleSceneData &gameData);
	};
	//移動に関するチュートリアルデータを管理するクラス
	struct MoveTutorial:public TutorialBase{
		static const float minDisplayPopOP;//ポップアップを出す場合は、これ以上のOPから移動しないといけない
		bool m_displayPopFlag;//目的地点に到達できなかった時にポップアップを出すかどうか
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
		Easing m_rate;//説明絵の拡大率(中央で一旦停止した後更新される)
		const int m_gHandle;//説明絵のデータ
		bool m_secondMoveFlag;//2回目の移動が始まったかどうか(チュートリアル終了の判定に用いる)

		ExplainTutorial(const char *gFileName);
		~ExplainTutorial();
		void DrawSupplement(int font)const;//説明絵の描画を行う
		void SetSecondMoveFlag();//2回目の移動を始められるように設定する
	};
	//数回の行動終了まで特に何もチュートリアルしないクラス
	struct BlankTutorial:public TutorialBase{
		int m_leftCount;//残り回数
		
		explicit BlankTutorial(int count);
		~BlankTutorial()=default;
		void DrawSupplement(int font)const;//何もしない
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
	TutorialBattleSceneData(const std::string &stageName);
	~TutorialBattleSceneData();


};


#endif // !DEF_TUTORIALBATTLESCENEDATA_H
