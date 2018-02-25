#ifndef DEF_EDGE_H
#define DEF_EDGE_H

#include"Shape.h"

//������\���N���X
class Edge:public Shape{
	//�^�E�񋓑�

	//�萔

	//�ϐ�
protected:
	Vector2D m_vec;//�n�_����I�_�֌������x�N�g��

	//�֐�
protected:
	Vector2D CalculateParpendicularVec(Vector2D point)const;//point���炱�̒����ɐ��������낵�����A��������point�Ɍ����������̐����x�N�g�������߂�

public:
	Edge(Vector2D begin,Vector2D vec,Fix::Kind fix);
	~Edge();
	Vector2D GetBeginPoint()const{
		return m_position;
	}
	Vector2D GetEndPoint()const{
		//�I�_�̈ʒu��Ԃ�
		return m_position+m_vec;
	}
	void Draw(Vector2D adjust,unsigned int color,int fillFlag,float lineThickness=1.0f)const;
	Vector2D CalculatePushVec(const Shape *pShape)const;//pShape��this���d�Ȃ��Ă��邩���肵�A�����o���x�N�g����Ԃ��B�d�Ȃ��Ă��Ȃ��ꍇ��Vector2D(0,0)���Ԃ����B
};


#endif // !DEF_EDGE_H
#pragma once
