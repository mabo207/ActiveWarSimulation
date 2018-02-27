#include<cassert>
#include"Circle.h"
#include"DxLib.h"

//--------------------Circle--------------------
Circle::Circle(Vector2D position,float r,Fix::Kind fix)
	:Shape(position,Type::e_circle,fix),m_r(r){}

Circle::~Circle(){}

std::shared_ptr<Shape> Circle::VCopy()const{
	return std::shared_ptr<Shape>(new Circle(this->m_position,this->m_r,this->m_fix));
}

void Circle::Draw(Vector2D adjust,unsigned int color,int fillFlag,float lineThickness)const{
	Vector2D v=m_position+adjust;
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
		//Edge�̕��ŏ�������B�����ɒ��ӁA�t�����ɂ���B
		ret=-pShape->CalculatePushVec(this);
		break;
	default:
		ret=Vector2D();
		break;
	}
	return ret;
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

void Circle::Resize(Vector2D v){
	//���am_r��v�̒����ɂ���
	m_r=v.size();
}

Vector2D Circle::GetRetResize()const{
	//����m_r�̃x�N�g����Ԃ��΂Ȃ�ł��悢
	return Vector2D(m_r,0);
}
