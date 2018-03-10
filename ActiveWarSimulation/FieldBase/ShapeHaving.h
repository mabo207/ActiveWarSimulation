#ifndef DEF_SHAPEHAVING_H
#define DEF_SHAPEHAVING_H

#include<memory>
#include<vector>
#include"Shape.h"

//�����蔻�菈�������������z�N���X�A�u�����蔻�������Q���v�Ȃǂ���鎞�Ɋ��N���X�ɂ���Ɨǂ�
class ShapeHaving{
	//�萔
protected:
	static const float pushRate;

	//�ϐ�
protected:
	std::shared_ptr<Shape> m_hitJudgeShape;//�����蔻��̌`��

	//�֐�
public:
	ShapeHaving(std::shared_ptr<Shape> pShape);
	virtual ~ShapeHaving();

	//���z�֐�
	virtual void Move(Vector2D v);//�x�N�g��v����������(�o�g���̎����ŗp����)(�p����œ����蔻��}�`�𕡐��؂�ւ���̂ł���΃I�[�o�[���C�h����)
	virtual const Shape *GetHitJudgeShape()const;//m_hitJudgeShape��const�|�C���^���擾�A�����蔻�菈���ɗp����(�p����œ����蔻��}�`�𕡐��؂�ւ���̂ł���΃I�[�o�[���C�h����)
	virtual Shape::Fix::Kind SetFix(Shape::Fix::Kind fix)const;//m_hitJudgeShape.m_fix��ύX(�p����œ����蔻��}�`�𕡐��؂�ւ���̂ł���΃I�[�o�[���C�h����)

	//�ʏ�֐�
	void ShapeDraw(unsigned int color,int fillFlag,float linethickness=1.0f,Vector2D adjust=Vector2D())const;//�����蔻��`��Ɋ�Â����`�揈��(dx,dy�̓Y���̒l)
	void PosDraw(unsigned int color,int fillFlag,float size,Vector2D adjust=Vector2D())const;//m_Pos�̈ʒu�ɉ~��`�悷��B
	void Warp(Vector2D v);//�x�N�g��v�֓�����(�X�e�[�W�ҏW�ŗp����)
	void Resize(Vector2D v);//�ʒu�x�N�g��v���Q�l�ɑ傫����ύX����(�X�e�[�W�ҏW�ŗp����)(m_hitJudgeShape�̂��̂𒼐ڕҏW)
	void ChangeShape(std::shared_ptr<Shape> pShape);//�����蔻��}�`��ύX����
	bool JudgePointInsideShape(Vector2D point)const;
	float getTop()const;
	float getBottom()const;
	float getLeft()const;
	float getRight()const;
	Vector2D getPos()const;
	Vector2D getResizeVec()const;//m_hitJudgeShape�̂��̂𒼐ڗp����
	Shape::Fix::Kind GetFix()const;//GetHitJudgeShape().m_fix���擾
	const std::vector<const ShapeHaving *> InShapeHavingList(const ShapeHaving  * const * const pShapeHavingVec,const size_t vecSize)const;//pShapeVec�ɂ���}�`�̂���this���ɑ��݂���}�`��S�ă��X�g�A�b�v����
	void UpdatePosition(ShapeHaving * const * const pShapeHavingVec,const size_t vecSize,const size_t updateTimes);//�����蔻��ɂ��ʒu�␳��updateTimes�񂾂��s���B�S�Ă̐}�`�ɑ΂��܂Ƃ߂čs���̂��O��Bstd::vector�ŊǗ����Ă���ꍇ��std::vector::data()��p����B
	bool JudgeInShapeRect(const ShapeHaving *pShapeHaving)const;//�����`����ɂ���Ē��ɂ��邩�ǂ����𔻒肷��

};

#endif // !DEF_SHAPEHAVING_H

#pragma once
