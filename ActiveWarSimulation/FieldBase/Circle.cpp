#include<cassert>
#include"Circle.h"
#include"Edge.h"
#include"MyPolygon.h"
#include"DxLib.h"

//--------------------Circle--------------------
Circle::Circle(Vector2D position,float r,Fix::Kind fix)
	:Shape(position,Type::e_circle,fix),m_r(r){}

Circle::~Circle(){}

std::shared_ptr<Shape> Circle::VCopy()const{
	return std::shared_ptr<Shape>(new Circle(this->m_position,this->m_r,this->m_fix));
}

void Circle::Draw(Vector2D point,Vector2D adjust,unsigned int color,int fillFlag,float lineThickness)const{
	Vector2D v=point+adjust;
	DrawCircleAA(v.x,v.y,m_r,36,color,fillFlag,lineThickness);
}

Vector2D Circle::CalculatePushVec(const Shape *pShape)const{
	Vector2D ret;
	//pShape�̎�ނɂ���ď������@���ς��
	switch(pShape->GetType()){
	case Type::e_circle:
		{
			//pShape��Circle�ɃL���X�g����
			const Circle *pcircle=dynamic_cast<const Circle *>(pShape);
			if(pcircle!=nullptr){
				//�~�Ɖ~�͒��S�Ԃ̋����ɂ���Ĕ��肪�\
				Vector2D v=this->m_position-pcircle->m_position;//�Փˑ��肩�玩���Ɍ����������ɉ����o��
				float pushLength=this->GetR()+pcircle->GetR()-v.size();
				if(pushLength<=0.0f){
					//�����߂����������Ȃ�A�d�Ȃ��Ă��Ȃ�����Ԃ�
					ret=Vector2D();
				} else{
					//�����߂����������Ȃ�Av��pushLength�ɂȂ�܂ŏk�߂�B
					//�~�̒��S���d�Ȃ�Ɖ����o���x�N�g����0�ɂȂ�̂͒��߂�i���x���������Ă���΂Ȃ�Ƃ��Ȃ邪�j�B
					ret=v.norm()*pushLength;
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
			//pShape��Circle�ɃL���X�g����
			const Edge *pedge=dynamic_cast<const Edge *>(pShape);
			if(pedge!=nullptr){
				//Edge�̕��ŏ�������B�����ɒ��ӁA�t�����ɂ���B
				ret=-pedge->CalculatePushVec(this);
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

bool Circle::PushParentObj(const Shape *pShape,ShapeHaving *parentObj,float pushRate)const{
	//�����ƂԂ���������̌��pShape�������ɂ���ĉ����o���������قȂ�
	switch(pShape->GetType()){
	case(Type::e_circle):
	case(Type::e_edge):
		{
			//���肪�~�������̏ꍇ�͉����o�����������߂���̂ŁA��������̂܂܎g��
			//�����o�������̌v�Z
			Vector2D pushVec=this->CalculatePushVec(pShape);
			//�����o�������̈��̊����̃x�N�g�����������o��
			parentObj->Move(pushVec*pushRate);
			//�����o�������̒�����true��false���𔻒f���ĕԂ�
			return pushVec.sqSize()!=0.0f;
		}
	case(Type::e_polygon):
		{
			//���肪���p�`�̏ꍇ�́A�����ɕ������ď������s��
			const MyPolygon *ppolygon=dynamic_cast<const MyPolygon *>(pShape);
			if(ppolygon!=nullptr){
				bool ret=false;
				Vector2D begin=ppolygon->GetPosition();
				for(const Vector2D &vec:ppolygon->GetAllEdgeVecs()){
					//�S�Ă̐����ɑ΂��Ă̏���
					Edge e(begin,vec,ppolygon->m_fix);
					ret=this->PushParentObj(&e,parentObj,pushRate) | ret;//�����ɂ�鉟���o������+�����������ǂ����̍X�V����
					begin+=vec;
				}
				return ret;
			} else{
				//�����ɗ��邱�Ƃ���������A�Ȃ񂩃o�O���Ă�
				assert(false);
			}
		}
	}
	return false;
}

bool Circle::JudgeInShape(const Shape *pShape)const{
	//�����̏ꍇ�͉����o�����������߂���̂ł����p����
	return CalculatePushVec(pShape)!=Vector2D();
}

bool Circle::VJudgePointInsideShape(Vector2D point)const{
	//���S��point�܂ł̋�����m_r�𒲂ׂ�Ηǂ�
	return (m_position-point).sqSize()<m_r*m_r;
}

Vector2D Circle::GetLeftTop()const{
	return Vector2D(m_position.x-m_r,m_position.y-m_r);
}

Vector2D Circle::GetRightBottom()const{
	return Vector2D(m_position.x+m_r,m_position.y+m_r);
}

Vector2D Circle::VGetNearEndpoint(Vector2D point,float capacity)const{
	//�~�ɒ[�_�͖����̂�point�����̂܂ܕԂ�
	return point;
}

void Circle::Resize(Vector2D v){
	//���am_r��v�̒����ɂ���
	m_r=v.size();
}

Vector2D Circle::GetRetResize()const{
	//����m_r�̃x�N�g����Ԃ��΂Ȃ�ł��悢
	return Vector2D(m_r,0);
}

void Circle::WriteOutShape(std::ofstream &ofs)const{
	//"("��(���)��(�n�_�ʒu)��(�����Œ�)��(���a)��")"
	ofs<<beginer<<Type::GetStr(m_type)<<spliter;//ofs<<"(Circle,";
	ofs<<beginer<<m_position.x<<spliter<<m_position.y<<ender<<spliter;//ofs<<"("<<m_position.x<<","<<m_position.y<<"),";
	ofs<<Fix::GetStr(m_fix)<<spliter;//ofs<<Fix::GetStr(m_fix)<<",";
	ofs<<m_r<<ender;//ofs<<m_r<<")";
}
