#include"Edge.h"
#include"Circle.h"
#include"DxLib.h"
#include<cassert>

//--------------------Edge--------------------
Edge::Edge(Vector2D begin,Vector2D vec,Fix::Kind fix)
	:Shape(begin,Type::e_edge,fix),m_vec(vec){}

Edge::~Edge(){}

Vector2D Edge::CalculateParpendicularVec(Vector2D point)const{
	//point���炱�̒����ɐ��������낵�����A��������point�Ɍ����������̐����x�N�g�������߂�
	float dot=m_vec.dot(point-GetBeginPoint());//�����̑��̈ʒu��m_position+Norm()*dot�ƂȂ�
	Vector2D parpendicularPoint=m_position+m_vec.norm()*dot;//�����̑��̈ʒu
	return parpendicularPoint-point;
}

void Edge::Draw(Vector2D adjust,unsigned int color,int fillFlag,float lineThickness)const{
	Vector2D begin=m_position+adjust;
	Vector2D end=begin+m_vec;
	DrawLineAA(begin.x,begin.y,end.x,end.y,color,lineThickness);
}

Vector2D Edge::CalculatePushVec(const Shape *pShape)const{
	Vector2D ret;
	//pShape�̎�ނɂ���ď������@���ς��
	switch(pShape->GetType()){
	case Type::e_circle:
		{
			//pShape��Circle�ɃL���X�g����
			const Circle *pcircle=dynamic_cast<const Circle *>(pShape);
			if(pcircle!=nullptr){
				//�����Ɖ~�͉~�̒��S�������ɉ��낵�������̑��̈ʒu��p���Ĕ���\
				//�~�̒��S��������ւ̍ŋߖT�_�����߂�
				float dot=m_vec.dot(pcircle->GetPosition()-GetBeginPoint());//�����̈ʒu��m_position+Norm()*dot/Size()�ƂȂ�
				Vector2D nearestPoint;//�~�̒��S�ɍł��߂�������̓_
				if(dot<0.0f){
					//�����O�ɐ����̑�������A�ŋߖT�_��m_position�ɂȂ�
					nearestPoint=GetBeginPoint();
				} else if(dot<=m_vec.sqSize()){
					//�������ɐ����̑�������ꍇ�B
					nearestPoint=m_position+m_vec*dot/m_vec.sqSize();
				} else{
					//�����O�ɐ����̑�������A�ŋߖT�_��m_position+m_vec�ɂȂ�
					nearestPoint=GetEndPoint();
				}
				//�ŋߖT�_�Ɖ~�̓����蔻����s���B�~����ŋߖT�_�������o���B
				Vector2D hVec=nearestPoint-pcircle->GetPosition();//�~���S����ŋߖT�_�ւ̃x�N�g��
				float pushLength=pcircle->GetR()-hVec.size();
				if(pushLength>0.0f){
					//�ŋߖT�_�Ɖ~�̋��������a�ɓ������Ȃ�悤�ɉ����o���x�N�g�����`
					ret=hVec.norm()*pushLength;
				} else{
					//pushLength<0�Ȃ牟���o�����Ȃ�
					ret=Vector2D();
				}
			} else{
				//�����ɗ��邱�Ƃ͖����͂��B��芸�����������ĂȂ����Ƃɂ���B
				assert(false);
				ret=Vector2D();
			}
		}
	break;
	case Type::e_edge:
		{
			//pShape��Edge�ɃL���X�g����
			const Edge *pcircle=dynamic_cast<const Edge *>(pShape);
			if(pcircle!=nullptr){
				//���̎����ł͐��Ɛ��̓����蔻��͂ł��Ȃ��B�Ȃ��A��������Ȃ�ړ��O��̈ʒu��������K�v������B
				//�g��Ȃ��̂Ŏ������Ȃ��B
			} else{
				//�����ɗ��邱�Ƃ͖����͂��B��芸�����������ĂȂ����Ƃɂ���B
				assert(false);
				ret=Vector2D();
			}
		}
		break;
	default:
		ret=Vector2D();
		break;
	}
	return ret;
}
