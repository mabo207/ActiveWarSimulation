#ifndef DEF_SHAPE_H
#define DEF_SHAPE_H

#include"ToolsLib.h"
#include<vector>
#include<memory>
#include<iostream>
#include<fstream>
#include"StringBuilder.h"

class ShapeHaving;//�z�Q�Ɣ���

//�����蔻��}�`���w���������z�֐�
class Shape{
	//�񋓑́E�^
public:
	struct Type{
		enum Kind{
			e_circle,
			e_edge,
			e_polygon,
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
	static const char spliter,beginer,ender;//�f�[�^�ǂݍ��݂̍ۂɎg����؂蕶����

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
	void Draw(Vector2D adjust,unsigned int color,int fillFlag,float lineThickness=1.0f)const;
	//�������z�֐�
	virtual std::shared_ptr<Shape> VCopy()const=0;//���e�������Ń|�C���^�̈ʒu�݂̂��قȂ�I�u�W�F�N�g�̃|�C���^��Ԃ�
	virtual void Draw(Vector2D point,Vector2D adjust,unsigned int color,int fillFlag,float lineThickness=1.0f)const=0;
	//virtual Vector2D CalculatePushVec(const Shape *pShape)const=0;//pShape��this���d�Ȃ��Ă��邩���肵�A�����o���x�N�g����Ԃ��B�d�Ȃ��Ă��Ȃ��ꍇ��Vector2D(0,0)���Ԃ����B
	virtual bool PushParentObj(const Shape *pShape,ShapeHaving *parentObj,float pushRate)const=0;//this��pShape���d�Ȃ��Ă��邩���肵�A�d�Ȃ��Ă���ꍇ��parentObj���ړ�����true��Ԃ��B
	virtual bool JudgeInShape(const Shape *pShape)const=0;//this����pShape�����邩�ǂ����̔���
	virtual Vector2D GetLeftTop()const=0;//����̍��W�����߂�
	virtual Vector2D GetRightBottom()const=0;//�E���̍��W�����߂�
	virtual void RecordLatticePointInShape(std::vector<int> &latticeInShape,const size_t xNum,const size_t yNum,const size_t squareWidth,const size_t squareHeight,int index)const=0;//���̐}�`�����ɂ���i�q�_�̔z���S��index�ɂ���
	//�G�f�B�^�p�̏������z�֐�
	virtual bool VJudgePointInsideShape(Vector2D point)const=0;//�}�`���ɓ_�����邩�ǂ����̔���ACalculatePushVec��p�����荂���Ɏ����ł���̂Ŋ֐��𕪂���
	virtual Vector2D VGetNearEndpoint(Vector2D point,float capacity)const=0;//point���[�_�ɋ߂�(������capacity�ȓ�)�ꍇ�A���̒[�_��Ԃ�
	virtual void Resize(Vector2D v)=0;//�}�`��\������x�N�g����p���Đ}�`�̑傫����ύX����
	virtual Vector2D GetRetResize()const=0;//Resize�̋t�֐��B����r��Resize����ƌ��݂̐}�`�ɂȂ�悤��r��Ԃ��B
	//�ǂݍ��݁E�����o���p���z�֐�
	virtual void WriteOutShape(std::ofstream &ofs)const=0;//�����o���p�֐�
	static std::shared_ptr<Shape> CreateShape(StringBuilder &info);//�����񂩂�Shape�𐶐�����
};

#endif // !DEF_SHAPE_H
#pragma once
