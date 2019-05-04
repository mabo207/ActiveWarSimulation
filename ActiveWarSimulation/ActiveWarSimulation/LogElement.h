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
	};
	//ユニット1体についてのデータ形式
	struct UnitLogData{
		UnitLogData(int i_hp,float i_op,const Vector2D &i_pos,Unit::Profession::Kind i_profession,const std::string &i_name)
			:hp(i_hp)
			,op(i_op)
			,pos(i_pos)
			,profession(i_profession)
			,name(i_name)
		{}
		explicit UnitLogData(const Unit * const punit)
			:UnitLogData(punit->GetBattleStatus().HP
				,punit->GetBattleStatus().OP
				,punit->getPos()
				,punit->GetBaseStatus().profession
				,punit->GetBaseStatus().name)
		{}
		bool operator==(const UnitLogData &otherObj)const{
			return (this->hp==otherObj.hp
				&& this->op==otherObj.op
				&& this->pos==otherObj.pos
				&& this->profession==otherObj.profession
				&& this->name==otherObj.name);
		}
		//変数を追加した場合は==やコンストラクタの変更を忘れずに！
		const int hp;
		const float op;
		const Vector2D pos;
		const Unit::Profession::Kind profession;
		const std::string name;
	};

	LogElement(LogKind kind,const std::shared_ptr<const BattleSceneData> battleData);
	virtual ~LogElement(){}
	LogKind GetLogKind()const{
		return m_kind;
	}

protected:
	const std::vector<UnitLogData> m_unitDataList;

private:
	static std::vector<UnitLogData> CreateUnitDataList(const std::shared_ptr<const BattleSceneData> battleData);

	const LogKind m_kind;
};

#endif // !DEF_LOGELEMENT_H