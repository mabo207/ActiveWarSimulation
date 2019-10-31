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
	std::shared_ptr<Shape> VCopy()const;//���e�������Ń|�C���^�̈ʒu�݂̂��قȂ�I�u�W�F�N�g�̃|�C���^��Ԃ�
	Vector2D GetBeginPoint()const{
		return m_position;
	}
	Vector2D GetEndPoint()const{
		//�I�_�̈ʒu��Ԃ�
		return m_position+m_vec;
	}
	Vector2D CalculatePushVec(const Shape *pShape)const;//pShape��this���d�Ȃ��Ă��邩���肵�A�����o���x�N�g����Ԃ��B�d�Ȃ��Ă��Ȃ��ꍇ��Vector2D(0,0)���Ԃ����B
	void BrokenDraw(Vector2D adjust,unsigned int color,float lineThickness,float drawSize,float eraseSize)const;//�j���`��
	void BrokenDraw(Vector2D point,Vector2D adjust,float exRate,unsigned int color,float lineThickness,float drawSize,float eraseSize)const;//�j���`��

	//�������z�֐��̃I�[�o�[���C�h
	void Draw(Vector2D point,Vector2D adjust,float exRate,unsigned int color,int fillFlag,float lineThickness)const;
	bool PushParentObj(const Shape *pShape,ShapeHaving *parentObj,float pushRate)const;//this��pShape���d�Ȃ��Ă��邩���肵�A�d�Ȃ��Ă���ꍇ��parentObj���ړ�����true��Ԃ��B
	bool JudgeInShape(const Shape *pShape)const;//this����pShape�����邩�ǂ����̔���
	Vector2D GetLeftTop()const;//����̍��W�����߂�
	Vector2D GetRightBottom()const;//�E���̍��W�����߂�
	void RecordLatticePointInShape(std::vector<int> &latticeInShape,const size_t xNum,const size_t yNum,const size_t squareWidth,const size_t squareHeight,int index)const;//���̐}�`�����ɂ���i�q�_�̔z���S��index�ɂ���
	//�G�f�B�^�p
	bool VJudgePointInsideShape(Vector2D point)const;//�}�`���ɓ_�����邩�ǂ����̔���ACalculatePushVec��p�����荂���Ɏ����ł���̂Ŋ֐��𕪂���
	Vector2D VGetNearEndpoint(Vector2D point,float capacity)const;//point���[�_�ɋ߂�(������capacity�ȓ�)�ꍇ�A���̒[�_��Ԃ�
	void Resize(Vector2D v);//�}�`��\������x�N�g����p���Đ}�`�̑傫����ύX����
	Vector2D GetRetResize()const;//Resize�̋t�֐��B����r��Resize����ƌ��݂̐}�`�ɂȂ�悤��r��Ԃ��B
	//�ǂݍ��݁E�����o���p�֐�
	void WriteOutShape(std::ofstream &ofs)const;//�����o���p�֐�
};


#endif // !DEF_EDGE_H
#pragma once
