#ifndef DEF_LOGELEMENT_H
#define DEF_LOGELEMENT_H

#include"Unit.h"
#include"BattleSceneData.h"
#include<list>
#include<memory>

//"BattleSceneData.h"と"ScoreSystem.h"と"LogElement.h"は循環参照
struct BattleSceneData;

//全ての種類のログの基本となるもの
class LogElement{
public:
	//ログの種類
	enum class LogKind{
		e_wait
		,e_attack
		,e_init
		,e_finish
	};
	//ユニット1体についてのデータ形式
	struct UnitLogData{
		UnitLogData(int i_hp,float i_op,const Vector2D &i_pos,const Unit * const i_punit)
			:hp(i_hp)
			,op(i_op)
			,pos(i_pos)
			,punit(i_punit)
		{}
		explicit UnitLogData(const Unit * const punit)
			:UnitLogData(punit->GetBattleStatus().HP
				,punit->GetBattleStatus().OP
				,punit->getPos()
				,punit)
		{}
		bool operator==(const UnitLogData &otherObj)const{
			return (this->hp==otherObj.hp
				&& this->op==otherObj.op
				&& this->pos==otherObj.pos
				&& this->punit==otherObj.punit);
		}
		//変数を追加した場合は==やコンストラクタの変更を忘れずに！
		const int hp;
		const float op;
		const Vector2D pos;
		const Unit * const punit;
	};

	LogKind GetLogKind()const{
		return m_kind;
	}
	const Unit *GetOperateUnit()const;

protected:
	//battleDataは、必要な値を取り出すだけなのでstd::shared_ptr<>である必要はない。
	LogElement(LogKind kind,const BattleSceneData * const battleData);
	virtual ~LogElement(){}

	const std::vector<UnitLogData> m_unitDataList;

private:
	static std::vector<UnitLogData> CreateUnitDataList(const BattleSceneData * const battleData);

	const LogKind m_kind;
};

#endif // !DEF_LOGELEMENT_H