#ifndef DEF_BATTLEOBJECT_H
#define DEF_BATTLEOBJECT_H

//�C���N���[�h�t�@�C��
#include<memory>
#include<iostream>
#include<fstream>
#include"ToolsLib.h"
#include"ShapeHaving.h"


//�o�g����ʂɂ�����I�u�W�F�N�g�̊��N���X
class BattleObject:public ShapeHaving{
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

public:

	//�֐�
protected:
	//���z�֐�
	virtual void WriteOutObjectPeculiarInfo(std::ofstream &ofs)const=0;//�I�u�W�F�N�g���L�̏���Ԃ�

public:
	//�R���X�g���N�^�ƃf�X�g���N�^
	BattleObject(Type::Kind type,const std::shared_ptr<Shape> hitJudgeShape,int gHandle)
		:ShapeHaving(hitJudgeShape),m_gHandle(gHandle),m_type(type){}
	virtual ~BattleObject() {}

	
	//���z�֐�
	void VDraw(Vector2D adjust=Vector2D(0.0f,0.0f))const;//�`�揈��(�ʒu���炵�܂߂�)
	virtual void VDraw(Vector2D point,Vector2D adjust)const=0;//�`�揈��(�C�Ӎ��W�ɕ`��)
	virtual void VHitProcess(const BattleObject *potherobj)=0;//�����ɓ����������̏���
	virtual std::shared_ptr<BattleObject> VCopy()const=0;//�����I�u�W�F�N�g�𕡐�����(�|�C���^�݈̂قȂ�)

	//�ʏ�֐�
	void WriteOutObjectWholeInfo(std::ofstream &ofs)const;//�I�u�W�F�N�g�S�̂̏�񏑂��o��
	Type::Kind GetType()const{
		return m_type;
	}

	//�����񂩂�BattleObject�����֐�
	static std::shared_ptr<BattleObject> CreateObject(const std::string &infostr);
	static BattleObject *CreateRawObject(const std::string &infostr);//new�Ő��|�C���^�ō��Bshared_ptr�ɓ˂����܂Ȃ��ꍇ�͎����Ń������������B
};

#endif // !DEF_BATTLEOBJECT_H
