#ifndef DEF_LOGELEMENT_H
#define DEF_LOGELEMENT_H

#include<memory>
#include<functional>
#include<vector>
#include"ToolsLib.h"

struct BattleSceneData;
class Unit;

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
		UnitLogData()
			:UnitLogData(0,0.0f,Vector2D(),nullptr){}
		explicit UnitLogData(const Unit * const punit);
		bool operator==(const UnitLogData &otherObj)const;
		//変数を追加した場合は==やコンストラクタの変更を忘れずに！
		const int hp;
		const float op;
		const Vector2D pos;
		const Unit * const punit;
	};

	LogKind GetLogKind()const{
		return m_kind;
	}
	const UnitLogData GetOperateUnitData()const;
	bool JudgeEveryUnitData(const std::function<bool(const UnitLogData &)> &func,bool all)const;//allがtrueなら「全て成立」を、falseなら「成立しているものがある」(exist)を調べる。
	const UnitLogData FindUnitData(const Unit *punit)const;

	const std::vector<UnitLogData> m_unitDataList;

protected:
	//battleDataは、必要な値を取り出すだけなのでstd::shared_ptr<>である必要はない。
	LogElement(LogKind kind,const BattleSceneData * const battleData);
	LogElement(LogKind kind,const std::vector<Unit *> &unitList);
	virtual ~LogElement(){}

private:
	static std::vector<UnitLogData> CreateUnitDataList(const std::vector<Unit *> &unitList);

	const LogKind m_kind;
};

#endif // !DEF_LOGELEMENT_H