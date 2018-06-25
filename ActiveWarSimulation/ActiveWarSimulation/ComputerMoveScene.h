#ifndef DEF_COMPUTERMOVESCENE_H
#define DEF_COMPUTERMOVESCENE_H

#include"MoveScene.h"
#include<vector>

//AI�����j�b�g�𓮂����ۂɎg���֐�
class ComputerMoveScene:public MoveScene{
	//�^�E�񋓑�
protected:
	//�����}�b�v�쐬���ɗp����
	struct LatticeDistanceInfo{
		size_t index;//�i�q�_�ԍ�
		size_t from;//�ǂ̊i�q�_���瓞�B�ł��邩(�召�֌W�A���l�֌W�ɗp���Ȃ�)
		float dist;//index�̊i�q�_�܂ł̐��苗��
		LatticeDistanceInfo(size_t i_index,size_t i_from,float i_dist):index(i_index),from(i_from),dist(i_dist){}
		bool operator<(const LatticeDistanceInfo &otherobj)const;
		bool operator==(const LatticeDistanceInfo &otherobj)const;
	};

	//�萔
protected:
	static const size_t squareSize;//�i�q�_�̊Ԋu�B���j�b�g�̑傫���ȏ�łȂ��ƒʂ�Ȃ��ꏊ��ʂ낤�Ƃ���o�O���������鋰�ꂪ����

	//�ϐ�
protected:
	//�i�q�_�̒�`�̂��߂̏��
	std::vector<int> m_latticeInShape;//���ׂĂ̊i�q�_�ɂ����āA���j�b�g���ʂ��ꏊ���ǂ������i�[���Ă���B0:�ʂ�� 1:�}�`���Œʂ�Ȃ� 2:�אڊi�q�_��1�Ȃ̂Œʂ�Ȃ��\��������
	const size_t m_xLatticeNum,m_yLatticeNum;//�i�q�_�����E�c�ɉ����݂��邩�B�X�e�[�W�̊O�ɂ����Ă�����Ƃ��Ă͖��Ȃ��B�v�Z���y�ɍs�����߂ɗp����B

	//AI�̂��߂̏��
	std::vector<Vector2D> m_latticeRoute;//�ʂ�i�q�_

	std::vector<LatticeDistanceInfo> distvec;//�f�o�b�O�p���������ϐ�

	//�֐�
protected:
	std::pair<size_t,Vector2D> DecideTargetPoint(const std::vector<LatticeDistanceInfo> &distanceInfo)const;//�ڕW�Ƃ���i�q�_�Ƃ��̊i�q�_�ɓ����������Ƃɑ_���ʒu�����߂�
	void CalculateLatticeDistanceInfo(std::vector<LatticeDistanceInfo> &latticeDistanceInfo)const;//�e�i�q�_�܂ł̋����ƍs�������v�Z����֐��B���ʂ������̂ŏ��̊i�[��������œn���B
	void CalculateLatticeRoute();//m_latticeInShape����m_latticeRoute���v�Z����

	//���z�֐��̃I�[�o�[���C�h
	Vector2D CalculateInputVec()const;

public:
	ComputerMoveScene(std::shared_ptr<BattleSceneData> battleSceneData);
	~ComputerMoveScene();

	//���z�֐��̃I�[�o�[���C�h
	int thisCalculate();
	void thisDraw()const;

};

#endif // !DEF_COMPUTERMOVESCENE_H
#pragma once
