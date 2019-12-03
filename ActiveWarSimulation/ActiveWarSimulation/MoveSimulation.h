#ifndef DEF_REFLECTIONWORK_MOVESIMULATION_H
#define DEF_REFLECTIONWORK_MOVESIMULATION_H

#include"ReflectionWorkBase.h"
#include"Unit.h"
#include"LogElement.h"
#include"SubmissionRuleBase.h"

namespace ReflectionWork{
	//キャラを動かしながらシミュレーション学習してみるクラス
	class MoveSimulation:public Base{
	public:
		//関数
		MoveSimulation(const std::vector<BattleObject *> &field
			,Unit * const operateUnit
			,const Vector2D stageSize
			,const Unit * const attackedUnit
			,const Vector2D startPos
			,const float mapRate
			,const std::shared_ptr<const SubmissionRuleBase> &rule
			,const std::string &question);
		~MoveSimulation();
		//仮想関数のオーバーライド
		void Update();
		bool WorkClear()const;
		void WorkDraw()const;

	private:
		//関数
		std::shared_ptr<LogElement> CreateLog()const;//現在のシミュレーション学習の状態をログ形式にする関数
		
		//変数
		bool m_clear;//動かし終わったかを管理する
		SubmissionEvaluation m_evaluate;//現在の評価
		//バトルマップを表現するために必要な情報
		const std::vector<BattleObject *> m_field;//フィールド
		const Vector2D m_stageSize;//ステージの大きさ
		//動かすために必要な情報
		Unit *m_operateUnit;//操作ユニット
		const Vector2D m_startPos;//マップ右上の位置
		const float m_mapRate;//マップの倍率
		//ログを作成するために必要な情報
		const Unit *m_attackedUnit;//攻撃をされるユニット
		const std::shared_ptr<const SubmissionRuleBase> m_rule;//サブミッション評価手法
		//グラフィック変数
		const int m_font;
	};
}


#endif // !DEF_REFLECTIONWORK_MOVESIMULATION_H