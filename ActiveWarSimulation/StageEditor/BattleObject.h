#ifndef DEF_BATTLEOBJECT_H
#define DEF_BATTLEOBJECT_H

//�C���N���[�h�t�@�C��
#include<memory>
#include<iostream>
#include<fstream>
#include"ToolsLib.h"
#include"Shape.h"


//�ʂ̏ꏊ�Ő錾���Ă������ƁB
//struct Position2D;//��ʓI�Ȉʒu��\���\����


//�o�g����ʂɂ�����I�u�W�F�N�g�̊��N���X
class BattleObject {
	//�^
public:
	struct Type{
		//�I�u�W�F�N�g�̎��
		enum Kind{
			e_terrain,//��Q��
			e_unit,//���j�b�g
			END
		};
		static Kind link(int num){
			if(num>=0 && num<END){
				return static_cast<Kind>(num);
			}
			return END;
		}
		static std::string GetStr(Kind kind);
	};

	//�萔

	//�ϐ�
protected:
	Type::Kind m_type;//�I�u�W�F�N�g�̎��
	int m_gHandle;//�I�u�W�F�N�g�̃O���t�B�b�N�n���h��
	//protected�ɖ߂��܂���(getter�����������)
	//Vector2D m_pos;//�I�u�W�F�N�g�̈ʒu(m_hitJudgeShape������)
	std::shared_ptr<Shape> m_hitJudgeShape;//�����蔻��̌`��

public:
	//�␳����鑤���H(�v���C���[�L�������H)
	bool fixFlag;

	//�֐�
protected:
	//���z�֐�
	virtual void WriteOutObjectPeculiarInfo(std::ofstream &ofs)const=0;//�I�u�W�F�N�g���L�̏���Ԃ�

public:
	//�R���X�g���N�^�ƃf�X�g���N�^
	BattleObject(Type::Kind type,const std::shared_ptr<Shape> hitJudgeShape,int gHandle)
		:m_hitJudgeShape(hitJudgeShape),m_gHandle(gHandle),fixFlag(false){}
	virtual ~BattleObject() {}

	
	//���z�֐�
	void VDraw(Vector2D adjust=Vector2D(0.0f,0.0f))const;//�`�揈��(�ʒu���炵�܂߂�)
	virtual void VDraw(Vector2D point,Vector2D adjust)const=0;//�`�揈��(�C�Ӎ��W�ɕ`��)
	virtual void VHitProcess(const BattleObject *potherobj)=0;//�����ɓ����������̏���
	virtual std::shared_ptr<BattleObject> VCopy()const=0;//�����I�u�W�F�N�g�𕡐�����(�|�C���^�݈̂قȂ�)

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
	void WriteOutObjectWholeInfo(std::ofstream &ofs)const;//�I�u�W�F�N�g�S�̂̏�񏑂��o��

	//�����񂩂�BattleObject�����֐�
	static std::shared_ptr<BattleObject> CreateObject(const std::string &infostr);
};

#endif // !DEF_BATTLEOBJECT_H
