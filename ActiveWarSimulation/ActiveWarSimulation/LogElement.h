#ifndef DEF_LOGELEMENT_H
#define DEF_LOGELEMENT_H

#include"Unit.h"
#include"BattleSceneData.h"
#include<list>
#include<memory>

//"BattleSceneData.h"��"ScoreSystem.h"��"LogElement.h"�͏z�Q��
struct BattleSceneData;

//�S�Ă̎�ނ̃��O�̊�{�ƂȂ����
class LogElement{
public:
	//���O�̎��
	enum class LogKind{
		e_wait
		,e_attack
	};
	//���j�b�g1�̂ɂ��Ẵf�[�^�`��
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
		//�ϐ���ǉ������ꍇ��==��R���X�g���N�^�̕ύX��Y�ꂸ�ɁI
		const int hp;
		const float op;
		const Vector2D pos;
		const Unit * const punit;
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