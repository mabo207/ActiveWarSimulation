#ifndef DEF_LATTICEBATTLEFIELD_H
#define DEF_LATTICEBATTLEFIELD_H

//�X�e�[�W���i�q�_�Ƃ��đ���������
//BattleSceneData���o�R���Ďg��
#include<vector>
#include<memory>
#include"Unit.h"
struct BattleSceneData;

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

	static std::shared_ptr<LatticeBattleField> Create(const BattleSceneData &battleData);
	static std::shared_ptr<LatticeBattleField> Create(const BattleSceneData &battleData,const Unit * const punit);
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
	void CalculateLatticeDistanceInfo(std::vector<LatticeDistanceInfo> &retPal,const Vector2D startPos)const;//m_latticeInShape�����ɁA�e�i�q�_�ƃX�^�[�g�n�_�܂ł̋����E���[�g�Ɋւ������Ԃ�
	Vector2D CalculateLatticePointPos(size_t x,size_t y)const;
	Vector2D CalculateLatticePointPos(size_t index)const;

	const static size_t latticeIntervalSize;//�i�q�_�͐����`��ɕ���ł���̂ŁA���̑傫��

private:
	LatticeBattleField(const BattleSceneData &battleData,const Unit * const punit);
	void CalculateLatticeInShape(const BattleSceneData &battleData,const Unit * const punit);

	std::vector<LatticePass> m_latticeInShape;
	const size_t m_xLatticeNum;
	const size_t m_yLatticeNum;
};

#endif // !DEF_LATTICEBATTLEFIELD_H