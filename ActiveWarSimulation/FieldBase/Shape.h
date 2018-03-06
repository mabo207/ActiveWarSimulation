#ifndef DEF_SHAPE_H
#define DEF_SHAPE_H

#include"ToolsLib.h"
#include<vector>
#include<memory>
#include<iostream>
#include<fstream>

//�����蔻��}�`���w���������z�֐�
class Shape{
	//�񋓑́E�^
public:
	struct Type{
		enum Kind{
			e_circle,
			e_edge,
			END
		};
		static Kind link(int num){
			if(num>=0 && num<END){
				return static_cast<Kind>(num);
			}
			return END;
		}
	};
	struct Fix{
		//���I�u�W�F�N�g�ړ����͏Փ˔��肪����ƕK���ړ��ꏊ���␳�����
		enum Kind{
			e_dynamic,//���I�u�W�F�N�g�̈ړ����ɉe����^����A���I�u�W�F�N�g�ړ����ɂ����ďՓ˔��肪����ƈʒu���␳�����
			e_static,//���I�u�W�F�N�g�̈ړ����ɉe����^����A���I�u�W�F�N�g�ړ����ɂ����ďՓ˔��肪�����Ă��ʒu���␳����Ȃ�
			e_ignore,//���I�u�W�F�N�g�̈ړ����ɉe����^���Ȃ�
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
public:
	static const float pushRate;//�����蔻�菈�����s���ہA�����o�������̊���

	//�ϐ�
protected:
	Vector2D m_position;//�}�`�̈ʒu
	Type::Kind m_type;//�}�`�̎��

public:
	Fix::Kind m_fix;//�}�`���Œ肳��Ă��邩�ǂ���(�����炭�e�L�����̍s��������邽�тɐݒ肵�������)

	//�֐�
protected:

public:
	Shape(Vector2D position,Type::Kind type,Fix::Kind fix);
	virtual ~Shape();
	Vector2D GetPosition()const{
		return m_position;
	}
	Type::Kind GetType()const{
		return m_type;
	}
	void Move(Vector2D displacement);//�I�u�W�F�N�g���ړ�������B���t���[�������Ĉʒu�␳���s�����߁A�����蔻��ɂ��ʒu�̕␳��Move()�ł͂Ȃ�Update()�ɂ���čs��
	const std::vector<std::shared_ptr<const Shape>> InShapeList(const std::vector<std::shared_ptr<const Shape>> &pShapeVec)const;//pShapeVec�ɂ���}�`�̂���this���ɑ��݂���}�`��S�ă��X�g�A�b�v����
	void Update(const std::vector<std::shared_ptr<Shape>> &pShapeVec);//�����蔻��ɂ��ʒu�␳���s���B�S�Ă̐}�`�ɑ΂��܂Ƃ߂čs���̂��O��B
	//�������z�֐�
	virtual std::shared_ptr<Shape> VCopy()const=0;//���e�������Ń|�C���^�̈ʒu�݂̂��قȂ�I�u�W�F�N�g�̃|�C���^��Ԃ�
	virtual void Draw(Vector2D adjust,unsigned int color,int fillFlag,float lineTickness=1.0f)const=0;
	virtual Vector2D CalculatePushVec(const Shape *pShape)const=0;//pShape��this���d�Ȃ��Ă��邩���肵�A�����o���x�N�g����Ԃ��B�d�Ȃ��Ă��Ȃ��ꍇ��Vector2D(0,0)���Ԃ����B
	virtual Vector2D GetLeftTop()const=0;//����̍��W�����߂�
	virtual Vector2D GetRightBottom()const=0;//�E���̍��W�����߂�
	//�G�f�B�^�p�̏������z�֐�
	virtual bool VJudgePointInsideShape(Vector2D point)const=0;//�}�`���ɓ_�����邩�ǂ����̔���ACalculatePushVec��p�����荂���Ɏ����ł���̂Ŋ֐��𕪂���
	virtual void Resize(Vector2D v)=0;//�}�`��\������x�N�g����p���Đ}�`�̑傫����ύX����
	virtual Vector2D GetRetResize()const=0;//Resize�̋t�֐��B����r��Resize����ƌ��݂̐}�`�ɂȂ�悤��r��Ԃ��B
	//�ǂݍ��݁E�����o���p���z�֐�
	virtual void WriteOutShape(std::ofstream &ofs)const=0;//�����o���p�֐�
	static std::shared_ptr<Shape> CreateShape(const std::string &infostr);//�����񂩂�Shape�𐶐�����
};

#endif // !DEF_SHAPE_H
#pragma once
