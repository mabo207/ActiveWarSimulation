#include"Edge.h"
#include"Circle.h"
#include"MyPolygon.h"
#include"DxLib.h"
#include<cassert>
#include"ToolsLib.h"

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

void Edge::Draw(Vector2D point,Vector2D adjust,float exRate,unsigned int color,int fillFlag,float lineThickness)const{
	Vector2D begin=point*exRate+adjust;
	Vector2D end=begin+m_vec*exRate;
	DrawLineAA(begin.x,begin.y,end.x,end.y,color,lineThickness*exRate);
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
			const Edge *pedge=dynamic_cast<const Edge *>(pShape);
			if(pedge!=nullptr){
				//���̎����ł͐��Ɛ��̓����蔻��͂ł��Ȃ��B�Ȃ��A��������Ȃ�ړ��O��̈ʒu��������K�v������B
				//�ЂƂ܂������������ǂ����̔��肪�ł���悤�ɁAVector2D(0.001f,0.001f)���������o�����Ƃɂ���B
				if(this->m_vec!=pedge->m_vec){
					//���s�łȂ��ꍇ
					//�A���������������Ηǂ�
					//�g��Ȃ��̂Ŏ������Ȃ�
				} else{
					//���s�ł���ꍇ
					//�g��Ȃ��̂Ŏ������Ȃ�
				}
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

void Edge::BrokenDraw(Vector2D adjust,unsigned int color,float lineThickness,float drawSize,float eraseSize)const{
	BrokenDraw(GetPosition(),adjust,1.0f,color,lineThickness,drawSize,eraseSize);
}

void Edge::BrokenDraw(Vector2D point,Vector2D adjust,float exRate,unsigned int color,float lineThickness,float drawSize,float eraseSize)const{
	Vector2D begin=point*exRate+adjust;
	Vector2D end=begin+m_vec*exRate;
	float thickness=lineThickness*exRate;
	if(thickness<1.0f){
		thickness=1.0f;
	}
	DrawBrokenLineAA(begin,end,color,thickness,drawSize,eraseSize);
}

bool Edge::PushParentObj(const Shape *pShape,ShapeHaving *parentObj,float pushRate)const{
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
					ret=e.PushParentObj(pShape,parentObj,pushRate) | ret;//�����ɂ�鉟���o������+�����������ǂ����̍X�V����
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

bool Edge::JudgeInShape(const Shape *pShape)const{
	//�����̏ꍇ�͉����o�����������߂���̂ł����p����
	return CalculatePushVec(pShape)!=Vector2D();
}

bool Edge::JudgeCross(const Shape *pShape)const{
	const Type::Kind kind=pShape->GetType();
	if(kind==Type::e_circle){
		const Circle *pCircle=dynamic_cast<const Circle *>(pShape);
		if(pCircle!=nullptr){
			//�����Ɖ~�̌�������́A�~�̒��S���璼����̓_�̋������~�̔��aR���傫���E���������؂�ւ�邩�ǂ����Ŕ���ł���
			//�~�̒��S��������ւ̍ŋߖT�_���ǂ����ŏꍇ��������
			const Vector2D center=pCircle->GetPosition();
			const float dot=m_vec.dot(center-GetBeginPoint());//�����̈ʒu��m_position+Norm()*dot/Size()�ƂȂ�
			float sqR=pCircle->GetR();
			sqR*=sqR;
			if(dot<0.0f){
				//�����O�ɐ����̑�������A�ŋߖT�_��m_position�ɂȂ�
				//2�[�_�Ɖ~�̒��S�̋����̊֌W�����߂�Ηǂ�
				const float nearestSqLength=(GetBeginPoint()-center).sqSize();
				const float farestSqLength=(GetEndPoint()-center).sqSize();
				return (nearestSqLength-sqR)*(farestSqLength-sqR)<=0.0f;
			} else if(dot<=m_vec.sqSize()){
				//�������ɐ����̑�������ꍇ�B
				//�u�����̑���1�̒[�_�̋����֌W�v�Ɓu�����̑��Ƃ���1�̒[�_�̋����֌W�v�̂Q�𒲂ׂ�Ηǂ�
				const float beginSqLength=(GetBeginPoint()-center).sqSize();
				const float vecSqSize=m_vec.sqSize();
				const float nearestSqLength=(vecSqSize*beginSqLength-dot*dot)/vecSqSize;
				const float endSqLength=(GetEndPoint()-center).sqSize();
				return ((nearestSqLength-sqR)*(beginSqLength-sqR)<=0.0f || (nearestSqLength-sqR)*(endSqLength-sqR)<=0.0f);
			} else{
				//�����O�ɐ����̑�������A�ŋߖT�_��m_position+m_vec�ɂȂ�
				//2�[�_�Ɖ~�̒��S�̋����̊֌W�����߂�Ηǂ�
				const float farestSqLength=(GetBeginPoint()-center).sqSize();
				const float nearestSqLength=(GetEndPoint()-center).sqSize();
				return (nearestSqLength-sqR)*(farestSqLength-sqR)<=0.0f;
			}
		}
	} else if(kind==Type::e_edge){
		const Edge *pEdge=dynamic_cast<const Edge *>(pShape);
		if(pEdge!=nullptr){
			//this->m_position=(x1,y1),this->m_vec=(x2,y2),pEdge->m_position=(x3,y3),pEdge->m_vec=(x4,y4)�ƒu���B
			//(x2*y4-x4*y2)==0�̎��X����0�Ȃ̂ŕ��s�ł���A���̒l�͌�ŗp����
			const float inclination=this->m_vec.x*pEdge->m_vec.y-pEdge->m_vec.x*this->m_vec.y;
			if(inclination==0.0f){
				//���s�̏ꍇ
				//���꒼����ɑ��݂��邩�̔��������(2��position���m�����񂾒�����this�����s���ǂ����Ŕ��肷��΂悢)
				//p1=this->GetBeginPoint(),p2=this->GetEndPoint(),q1=pEdge->GetBeginPoint(),q2=pEdge->GetEndPoint()�ł���
				const Vector2D p1p2=pEdge->GetBeginPoint()-this->GetBeginPoint();
				if(m_vec.x*p1p2.y==p1p2.x*m_vec.y){
					//���꒼����ɂ������ꍇ�A���ς�p���Ĕ���
					const Vector2D p1q2=pEdge->GetEndPoint()-this->GetBeginPoint();
					const Vector2D q1p2=pEdge->GetBeginPoint()-this->GetEndPoint();
					const Vector2D q1q2=pEdge->GetEndPoint()-this->GetEndPoint();
					if(p1p2.dot(m_vec)<0.0f && p1q2.dot(m_vec)<0.0f){
						//pEdge�̗��[�_��this��begin���m_vec�Ƌt���ɂ��鎞
						return true;
					} else if(q1p2.dot(m_vec)>0.0f && q1q2.dot(m_vec)>0.0f){
						//pEdge�̗��[�_��this��end���m_vec�Ɠ������ɂ��鎞
						return true;
					}
				}
			} else{
				//���s�łȂ��ꍇ�A�����̌�_�����߂Ă���炪������ɑ��݂��邩�����߂�
				//��_�̍��W��(x,y)=(x1,y1)+m*(x2,y2)=(x3,y3)+n*(x4,y4) (m,n��float)�ƂȂ�
				//0<=m,n<=1�ł���Ό�_����������ɑ��݂��邱�ƂɂȂ�
				//�������������ƁAm*(x2*y4-x4*y2)=y4*(x3-x1)-x4(y3-y1),n*(x2*y4-x4*y2)=y2*(x3-x1)-x2*(y3-y1)�ƂȂ�B
				const float m=(pEdge->m_vec.y*(pEdge->m_position.x-this->m_position.x)-pEdge->m_vec.x*(pEdge->m_position.y-this->m_position.y))/inclination;
				const float n=(this->m_vec.y*(pEdge->m_position.x-this->m_position.x)-this->m_vec.x*(pEdge->m_position.y-this->m_position.y))/inclination;
				return m>=0.0f && m<=1.0f && n>=0.0f && n<=1.0f;
			}
		}
	} else if(kind==Type::e_polygon){
		//Polygon���ɏ�����C����
		return pShape->JudgeCross(this);
	}
	return false;
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

void Edge::RecordLatticePointInShape(std::vector<int> &latticeInShape,const size_t xNum,const size_t yNum,const size_t squareWidth,const size_t squareHeight,int index)const{
	//�������͊i�q�_�������s��Ȃ��i�s���Ȃ��j�̂ŁA���̂܂ܕ��u
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
	//"("��(���)��(�n�_�ʒu)��(�����Œ�)��(�����x�N�g��)��")"
	ofs<<beginer<<Type::GetStr(m_type)<<spliter;//ofs<<"(Edge,";
	ofs<<beginer<<m_position.x<<spliter<<m_position.y<<ender<<spliter;//ofs<<"("<<m_position.x<<","<<m_position.y<<"),";
	ofs<<Fix::GetStr(m_fix)<<spliter;//ofs<<Fix::GetStr(m_fix)<<",";
	ofs<<beginer<<m_vec.x<<spliter<<m_vec.y<<ender<<ender;//ofs<<"("<<m_vec.x<<","<<m_vec.y<<"))";	
}
