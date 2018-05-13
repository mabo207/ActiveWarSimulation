#include"MyPolygon.h"
#include"DxLib.h"
#include<cassert>
#include<exception>

//--------------------MyPolygon--------------------
MyPolygon::MyPolygon(Vector2D begin,std::vector<Vector2D> points,Fix::Kind fix)
	:Shape(begin,Type::e_polygon,fix)
{
	//m_edgeVecs�̏�����
	size_t size=points.size();
	if(size<2){
		//���v���_����3����������瑽�p�`�ł͖����̂ŗ�O���΂�
		throw std::invalid_argument("You make polygon with "+std::to_string(size+1)+" points.");
	}
	Vector2D edgevec=points.at(0)-m_position;
	m_edgeVecs.push_back(edgevec);
	for(size_t i=0;i+1<size;i++){
		edgevec=points.at(i+1)-points.at(i);
		m_edgeVecs.push_back(edgevec);
	}
}

MyPolygon::~MyPolygon(){}

std::vector<Vector2D> MyPolygon::GetAllEdgeVecs()const{
	std::vector<Vector2D> ret=m_edgeVecs;
	Vector2D addVec(0.0f,0.0f);
	for(const Vector2D &e:m_edgeVecs){
		addVec-=e;
	}
	ret.push_back(addVec);
	return ret;
}

std::shared_ptr<Shape> MyPolygon::VCopy()const{
	return std::shared_ptr<Shape>(new MyPolygon(*this));
}

void MyPolygon::Draw(Vector2D point,Vector2D adjust,unsigned int color,int fillFlag,float lineThickness)const{
	//���𖞂������ǂ����ŏ�����ς���B�������Ȃ��ꍇ�͎O�p�`���������Ȃ��ėǂ��̂ō������ł��邽��
	if(fillFlag==TRUE){
		//���𖞂����ꍇ
		Vector2D pos=point+adjust,next;
		for(const Vector2D &edge:GetAllEdgeVecs()){
			//�n�_���������x�n�_�ɖ߂�܂ł̑S�Ă̕ӂ̕`��
			next=pos+edge;
			DrawLineAA(pos.x,pos.y,next.x,next.y,GetColor(168,128,128),lineThickness);
			pos=next;
		}
	} else{
		//�ӂ̂ݕ`�悷��ꍇ
		Vector2D pos=point+adjust,next;
		for(const Vector2D &edge:GetAllEdgeVecs()){
			//�n�_���������x�n�_�ɖ߂�܂ł̑S�Ă̕ӂ̕`��
			next=pos+edge;
			DrawLineAA(pos.x,pos.y,next.x,next.y,GetColor(168,128,128),lineThickness);
			pos=next;
		}
	}
}

bool MyPolygon::PushParentObj(const Shape *pShape,ShapeHaving *parentObj,float pushRate)const{
	//�����o�������̌v�Z�͂ł��Ȃ����A�ӂ��S�Đ����Ȃ̂őS�Ă̕ӂ�Edge�N���X�ɂ�����Ƃ̉����o���������s�킹��
	bool ret=false;
	Vector2D begin=m_position;
	for(const Vector2D &vec:GetAllEdgeVecs()){
		//�S�Ă̐����ɑ΂��Ă̏���
		Edge e(begin,vec,this->m_fix);
		ret=e.PushParentObj(pShape,parentObj,pushRate) | ret;//�����ɂ�鉟���o������+�����������ǂ����̍X�V����
		begin+=vec;
	}
	return ret;
}

bool MyPolygon::JudgeInShape(const Shape *pShape)const{
	//�O�p�`�������s���A�����ł����S�Ă̎O�p�`�ɂ��ē������菈�����s��

	return false;
}

Vector2D MyPolygon::GetLeftTop()const{
	Vector2D leftTop=m_position,point=m_position;
	for(const Vector2D &edge:m_edgeVecs){
		//�S�Ă̒��_�ɂ��čŏ��l�����߂�
		point+=edge;
		leftTop.x=std::fminf(leftTop.x,point.x);
		leftTop.y=std::fminf(leftTop.y,point.y);
	}
	return leftTop;
}

Vector2D MyPolygon::GetRightBottom()const{
	Vector2D rightBottom=m_position,point=m_position;
	for(const Vector2D &edge:m_edgeVecs){
		//�S�Ă̒��_�ɂ��čő�l�����߂�
		point+=edge;
		rightBottom.x=std::fmaxf(rightBottom.x,point.x);
		rightBottom.y=std::fmaxf(rightBottom.y,point.y);
	}
	return rightBottom;
}

bool MyPolygon::VJudgePointInsideShape(Vector2D point)const{
	//�O�p�`�������s���A�����ł����S�Ă̎O�p�`�ɂ��ē������菈�����s��

	return false;
}

Vector2D MyPolygon::VGetNearEndpoint(Vector2D point,float capacity)const{
	//�S�Ă̒��_�ɂ��ċ߂����ǂ����𔻒肷��
	Vector2D vertex=m_position;
	const float sqcapacity=capacity*capacity;
	for(const Vector2D &edge:GetAllEdgeVecs()){
		//�n�_�̎��̓_����T�����A�Ō�͎n�_�𔻒肷��
		vertex+=edge;
		if((point-vertex).sqSize()<=sqcapacity){
			//�߂��[�_���������炻�̍��W��Ԃ�
			return vertex;
		}
	}
	//�ǂ̒��_�ɂ��߂��Ȃ��Ȃ�point��Ԃ�
	return point;
}

void MyPolygon::Resize(Vector2D v){
	//MyPolygon��1�ڂ̕ӂ�v�ƂȂ�悤�ɁA�g��k�������łȂ���]���ł���悤�ɂ���B
	/*m_edgeVecs[0]��v���r���A�S�Ă�m_edgeVecs�ɑ΂���m_edgeVecs[0]����v�ւ̊g�嗦�Ɖ�]�p�x��K�p����
	//�ϊ��s������̂܂܋��߂����������Ȃ̂ł�����v�Z��Resize����B
	//v2=Av1�ƂȂ�A�͎��̂悤�Ɍv�Z�ł���B(v1=(x1,y1),v2=(x2,y2))
	//A=(a,-b)
	//  (b,a) (a=(x1x2+y1y2)/(x1^2+y1^2),b=(x1y2-x2y1)/(x1^2+y1^2))
	//*/
	Vector2D v1=m_edgeVecs.at(0);
	const float a=(v1.x*v.x+v1.y*v.y)/(v1.x*v1.x+v1.y*v1.y)
		,b=(v1.x*v.y-v.x*v1.y)/(v1.x*v1.x+v1.y*v1.y);
	for(Vector2D &edgeVec:m_edgeVecs){
		//�e�ӂɑ΂��ĉ�]�s���K�p
		edgeVec.x=a*edgeVec.x-b*edgeVec.y;
		edgeVec.y=b*edgeVec.x+a*edgeVec.y;
	}
}

Vector2D MyPolygon::GetRetResize()const{
	//���S�Ɍ��݂̌`���Č����邱�Ƃ͂ł��Ȃ��B�g�嗦�Ɖ�]�p�x����v�����邾���Ȃ�ŏ��̕ӂ�v�Ƃ��ėp����΂悢�B
	return m_edgeVecs[0];
}

void MyPolygon::WriteOutShape(std::ofstream &ofs)const{
	//"("��(���)��(�n�_�ʒu)��(�����Œ�)��((�e�ӂ̕����x�N�g��)��...)��")"
	ofs<<beginer<<Type::GetStr(m_type)<<spliter;//ofs<<"(Polygon,";
	ofs<<beginer<<m_position.x<<spliter<<m_position.y<<ender<<spliter;//ofs<<"("<<m_position.x<<","<<m_position.y<<"),";
	ofs<<Fix::GetStr(m_fix)<<spliter;//ofs<<Fix::GetStr(m_fix)<<",";
	//�S�Ă̕ӂ̍��W��()�őS�̂��͂��Ă���o��
	ofs<<beginer;//ofs<<"("
	//�n�_�����������_���W��S�ďo��
	Vector2D vertex=m_position;
	for(size_t i=0,size=m_edgeVecs.size();i<size;i++){
		//���_���W�̌v�Z
		vertex+=m_edgeVecs.at(i);
		//���W��(x,y)�̌`���ŏo��
		ofs<<beginer<<vertex.x<<spliter<<vertex.y<<ender;//ofs<<"(x,y)"
		if(i+1!=size){
			ofs<<spliter;//�Ō�̕ӂłȂ���΋�؂蕶�����o�͂���
		}
	}
	ofs<<ender<<ender;//ofs<<"))";	
}
