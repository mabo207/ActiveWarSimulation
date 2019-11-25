#ifndef DEF_LOGELEMENT_H
#define DEF_LOGELEMENT_H

#include<memory>
#include<functional>
#include<vector>
#include"ToolsLib.h"

struct BattleSceneData;
class Unit;

//�S�Ă̎�ނ̃��O�̊�{�ƂȂ����
class LogElement{
public:
	//���O�̎��
	enum class LogKind{
		e_wait
		,e_attack
		,e_init
		,e_finish
	};
	//���j�b�g1�̂ɂ��Ẵf�[�^�`��
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
		//�ϐ���ǉ������ꍇ��==��R���X�g���N�^�̕ύX��Y�ꂸ�ɁI
		const int hp;
		const float op;
		const Vector2D pos;
		const Unit * const punit;
	};

	LogKind GetLogKind()const{
		return m_kind;
	}
	const UnitLogData GetOperateUnitData()const;
	bool JudgeEveryUnitData(const std::function<bool(const UnitLogData &)> &func,bool all)const;//all��true�Ȃ�u�S�Đ����v���Afalse�Ȃ�u�������Ă�����̂�����v(exist)�𒲂ׂ�B
	const UnitLogData FindUnitData(const Unit *punit)const;

	const std::vector<UnitLogData> m_unitDataList;

protected:
	//battleData�́A�K�v�Ȓl�����o�������Ȃ̂�std::shared_ptr<>�ł���K�v�͂Ȃ��B
	LogElement(LogKind kind,const BattleSceneData * const battleData);
	LogElement(LogKind kind,const std::vector<Unit *> &unitList);
	virtual ~LogElement(){}

private:
	static std::vector<UnitLogData> CreateUnitDataList(const std::vector<Unit *> &unitList);

	const LogKind m_kind;
};

#endif // !DEF_LOGELEMENT_H