#ifndef DEF_CIRCLE_H
#define DEF_CIRCLE_H

#include"Shape.h"
#include"ShapeHaving.h"

//�~��\���N���X
class Circle:public Shape{
	//�^�E�񋓑�

	//�萔

	//�ϐ�
protected:
	float m_r;//���a

	//�֐�
protected:
	Vector2D CalculatePushVec(const Shape *pShape)const;//pShape��this���d�Ȃ��Ă��邩���肵�A�����o���x�N�g����Ԃ��B�d�Ȃ��Ă��Ȃ��ꍇ��Vector2D(0,0)���Ԃ����B

public:
	Circle(Vector2D position,float r,Fix::Kind fix);
	~Circle();
	std::shared_ptr<Shape> VCopy()const;//���e�������Ń|�C���^�̈ʒu�݂̂��قȂ�I�u�W�F�N�g�̃|�C���^��Ԃ�
	float GetR()const{
		return m_r;
	}
	
	//�������z�֐��̃I�[�o�[���C�h
	void Draw(Vector2D point,Vector2D adjust,float exRate,unsigned int color,int fillFlag,float lineThickness=1.0f)const;
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

#endif // !DEF_CIRCLE_H
#pragma once
