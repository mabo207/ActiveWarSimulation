#include"Edge.h"
#include"Circle.h"
#include"DxLib.h"
#include<cassert>

//--------------------Edge--------------------
Edge::Edge(Vector2D begin,Vector2D vec,Fix::Kind fix)
	:Shape(begin,Type::e_edge,fix),m_vec(vec){}

Edge::~Edge(){}

std::shared_ptr<Shape> Edge::VCopy()const{
	return std::shared_ptr<Shape>(new Edge(this->m_position,this->m_vec,this->m_fix));
}

Vector2D Edge::CalculateParpendicularVec(Vector2D point)const{
	//point���炱�̒����ɐ��������낵�����A��������point�Ɍ����������̐����x�N�g�������߂�
	float dot=m_vec.dot(point-GetBeginPoint());//�����̑��̈ʒu��m_position+Norm()*dot�ƂȂ�
	Vector2D parpendicularPoint=m_position+m_vec.norm()*dot;//�����̑��̈ʒu
	return parpendicularPoint-point;
}

void Edge::Draw(Vector2D point,Vector2D adjust,unsigned int color,int fillFlag,float lineThickness)const{
	Vector2D begin=point+adjust;
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

bool Edge::VJudgePointInsideShape(Vector2D point)const{
	//point�ւ�Edge��̍ŋߖT�_�����߂�
	float dot=m_vec.dot(point-GetBeginPoint());//�����̈ʒu��m_position+Norm()*dot/Size()�ƂȂ�
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
	//�ŋߖT�_�ւ̋�����5px�ȓ��Ȃ�_������ɂ���Ƃ݂Ȃ�
	const float capacity=5.0f;
	return (point-nearestPoint).sqSize()<=capacity*capacity;
}

Vector2D Edge::GetLeftTop()const{
	Vector2D ret=m_position;
	if(m_vec.x<0.0f){
		ret.x+=m_vec.x;
	}
	if(m_vec.y<0.0f){
		ret.y+=m_vec.y;
	}
	return ret;
}

Vector2D Edge::GetRightBottom()const{
	Vector2D ret=m_position;
	if(m_vec.x>0.0f){
		ret.x+=m_vec.x;
	}
	if(m_vec.y>0.0f){
		ret.y+=m_vec.y;
	}
	return ret;
}

Vector2D Edge::VGetNearEndpoint(Vector2D point,float capacity)const{
	const float sqCapacity=capacity*capacity;
	if((m_position-point).sqSize()<sqCapacity){
		//�n�_�ɋ߂��ꍇ
		return m_position;
	}
	if((m_position+m_vec-point).sqSize()<sqCapacity){
		//�I�_�ɋ߂��ꍇ
		return m_position+m_vec;
	}
	//�[�_�ɋ߂��Ȃ��Ȃ�point��Ԃ�
	return point;
}

void Edge::Resize(Vector2D v){
	//�x�N�g����v�ɂ���
	m_vec=v;
}

Vector2D Edge::GetRetResize()const{
	//Resize()�̋t�֐�
	return m_vec;
}

void Edge::WriteOutShape(std::ofstream &ofs)const{
	//"("��(���)��(�n�_�ʒu)��(�����x�N�g��)��(�����Œ�)��")"
	ofs<<beginer<<Type::GetStr(m_type)<<spliter;//ofs<<"(Edge,";
	ofs<<beginer<<m_position.x<<spliter<<m_position.y<<ender<<spliter;//ofs<<"("<<m_position.x<<","<<m_position.y<<"),";
	ofs<<Fix::GetStr(m_fix)<<spliter;//ofs<<Fix::GetStr(m_fix)<<",";
	ofs<<beginer<<m_vec.x<<spliter<<m_vec.y<<ender<<ender;//ofs<<"("<<m_vec.x<<","<<m_vec.y<<"))";	
}
