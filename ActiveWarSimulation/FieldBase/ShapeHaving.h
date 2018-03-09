#ifndef DEF_SHAPEHAVING_H
#define DEF_SHAPEHAVING_H

#include<memory>
#include<vector>
#include"Shape.h"

//�����蔻�菈�������������z�N���X�A�u�����蔻�������Q���v�Ȃǂ���鎞�Ɋ��N���X�ɂ���Ɨǂ�
class ShapeHaving{
	//�ϐ�
protected:
	std::shared_ptr<Shape> m_hitJudgeShape;//�����蔻��̌`��

	//�֐�
public:
	ShapeHaving(std::shared_ptr<Shape> pShape);
	virtual ~ShapeHaving();

	//�ʏ�֐�
	void ShapeDraw(unsigned int color,int fillFlag,Vector2D adjust=Vector2D())const;//�����蔻��`��Ɋ�Â����`�揈��(dx,dy�̓Y���̒l)
	void PosDraw(unsigned int color,int fillFlag,float size,Vector2D adjust=Vector2D())const;//m_Pos�̈ʒu�ɉ~��`�悷��B
	void Move(Vector2D v);//�x�N�g��v����������(�o�g���̎����ŗp����)
	void Warp(Vector2D v);//�x�N�g��v�֓�����(�X�e�[�W�ҏW�ŗp����)
	void Resize(Vector2D v);//�ʒu�x�N�g��v���Q�l�ɑ傫����ύX����(�X�e�[�W�ҏW�ŗp����)
	void ChangeShape(std::shared_ptr<Shape> pShape);//�����蔻��}�`��ύX����
	bool JudgePointInsideShape(Vector2D point)const;
	float getTop()const;
	float getBottom()const;
	float getLeft()const;
	float getRight()const;
	Vector2D getPos()const;
	Vector2D getResizeVec()const;
	std::shared_ptr<Shape> getHitJudgeShape()const;

};

#endif // !DEF_SHAPEHAVING_H

#pragma once
