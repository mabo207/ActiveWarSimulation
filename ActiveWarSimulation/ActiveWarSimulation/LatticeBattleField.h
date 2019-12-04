#ifndef DEF_LATTICEBATTLEFIELD_H
#define DEF_LATTICEBATTLEFIELD_H

//�X�e�[�W���i�q�_�Ƃ��đ���������
//BattleSceneData���o�R���Ďg��
#include<vector>
#include<memory>
#include"Unit.h"

class LatticeBattleField{
public:
	//�����}�b�v�쐬���ɗp����
	struct LatticeDistanceInfo{
		size_t index;//�i�q�_�ԍ�
		size_t from;//�ǂ̊i�q�_���瓞�B�ł��邩(�召�֌W�A���l�֌W�ɗp���Ȃ�)
		float dist;//index�̊i�q�_�܂ł̐��苗��
		LatticeDistanceInfo(size_t i_index,size_t i_from,float i_dist):index(i_index),from(i_from),dist(i_dist){}
		bool operator<(const LatticeDistanceInfo &otherobj)const;
		bool operator==(const LatticeDistanceInfo &otherobj)const;
	};
	//���̊i�q�_���ʂ��_���ǂ���
	enum class LatticePass{
		e_passable				//�ʂ��
		,e_unpassable			//�ʂ�Ȃ�
		,e_neighborUnpassable	//�אڊi�q�_���ʂ�Ȃ��̂Œʂ�邩������Ȃ�
	};

	static std::shared_ptr<LatticeBattleField> Create(const std::vector<BattleObject *> &field,const Vector2D stageSize,const Unit * const punit,bool unitExist);
	virtual ~LatticeBattleField();
	size_t GetXLatticeNum()const{
		return m_xLatticeNum;
	}
	size_t GetYLatticeNum()const{
		return m_yLatticeNum;
	}
	LatticePass GetLatticeInShapeAt(size_t index)const;
	size_t GetLatticeInShapeSize()const{
		return m_latticeInShape.size();
	}
	void BecomeImpassibleLattice(size_t index);//m_latticeInShape[index]��e_unpassable�ɂ��āA���̎��ӂ̊i�q�_�ɑ΂��鏈��������
	void BecomeImpassibleLattice(size_t x,size_t y);
	void BecomeImpassibleLattice(const Unit *punit,const Unit::Team::Kind operateTeam);//Unit������ʉߕs�\�ɂ���BUnit�ȊO��object�͑��샆�j�b�g�������O�����̔��肪�K�v�Ȃ̂ŁABattleObject�Ɋg�����邱�Ƃ͂��Ȃ��B��烍�O���͗p�B
	void CalculateLatticeDistanceInfo(std::vector<LatticeDistanceInfo> &retPal,const Vector2D startPos)const;//m_latticeInShape�����ɁA�e�i�q�_�ƃX�^�[�g�n�_�܂ł̋����E���[�g�Ɋւ������Ԃ�
	Vector2D CalculateLatticePointPos(size_t x,size_t y)const;
	Vector2D CalculateLatticePointPos(size_t index)const;
	std::vector<float> CalculateRouteDistance(const Vector2D start,const std::vector<Vector2D> &endVec)const;//�i�q�_����p�����I����l�������o�H������Ԃ��B�����}�b�v����������Ȃ��ōςނ悤�ɁA�ڕW�n�_���܂Ƃ߂ēn���B
	std::vector<float> CalculateRouteDistance(const std::vector<LatticeDistanceInfo> &distanceInfoVec,const std::vector<Vector2D> &endVec)const;//�����}�b�v�����ɍ쐬����Ă��鎞�̌o�H�����v�Z�B

	const static size_t latticeIntervalSize;//�i�q�_�͐����`��ɕ���ł���̂ŁA���̑傫��

private:
	LatticeBattleField(const std::vector<BattleObject *> &field,const Vector2D stageSize,const Unit * const punit,bool unitExist);
	void CalculateLatticeInShape(const std::vector<BattleObject *> &field,const Unit * const punit,bool unitExist);//unitExist��false�̎��A���j�b�g�����Ȃ����̂Ƃ��ċ����v�Z������

	std::vector<LatticePass> m_latticeInShape;
	const size_t m_xLatticeNum;
	const size_t m_yLatticeNum;
};

#endif // !DEF_LATTICEBATTLEFIELD_H