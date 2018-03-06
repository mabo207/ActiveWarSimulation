#ifndef DEF_CIRCLE_H
#define DEF_CIRCLE_H

#include"Shape.h"

//�~��\���N���X
class Circle:public Shape{
	//�^�E�񋓑�

	//�萔

	//�ϐ�
protected:
	float m_r;//���a

	//�֐�
public:
	Circle(Vector2D position,float r,Fix::Kind fix);
	~Circle();
	std::shared_ptr<Shape> VCopy()const;//���e�������Ń|�C���^�̈ʒu�݂̂��قȂ�I�u�W�F�N�g�̃|�C���^��Ԃ�
	float GetR()const{
		return m_r;
	}
	void Draw(Vector2D adjust,unsigned int color,int fillFlag,float lineThickness=1.0f)const;
	Vector2D CalculatePushVec(const Shape *pShape)const;//pShape��this���d�Ȃ��Ă��邩���肵�A�����o���x�N�g����Ԃ��B�d�Ȃ��Ă��Ȃ��ꍇ��Vector2D(0,0)���Ԃ����B
	bool VJudgePointInsideShape(Vector2D point)const;//�}�`���ɓ_�����邩�ǂ����̔���ACalculatePushVec��p�����荂���Ɏ����ł���̂Ŋ֐��𕪂���
	Vector2D GetLeftTop()const;//����̍��W�����߂�
	Vector2D GetRightBottom()const;//�E���̍��W�����߂�
	//�G�f�B�^�p
	void Resize(Vector2D v);//�}�`��\������x�N�g����p���Đ}�`�̑傫����ύX����
	Vector2D GetRetResize()const;//Resize�̋t�֐��B����r��Resize����ƌ��݂̐}�`�ɂȂ�悤��r��Ԃ��B
	//�ǂݍ��݁E�����o���p�֐�
	void WriteOutShape(std::ofstream &ofs)const;//�����o���p�֐�
};

#endif // !DEF_CIRCLE_H
#pragma once
