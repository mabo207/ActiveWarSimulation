#include"MyPolygon.h"
#include"DxLib.h"
#include<cassert>
#include<exception>
#include<list>
#include<array>
#include<iterator>

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
	//m_triangleSet�̏�����
	DivideTriangle(&m_triangleSet);
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

void MyPolygon::DivideTriangle(std::vector<std::array<size_t,3>> *triangleSet)const{
	//�g���֐�
	auto FuncNextIt=[](std::list<std::pair<Vector2D,size_t>>::iterator it,std::list<std::pair<Vector2D,size_t>> &list){
		it++;
		if(it==list.end()){
			it=list.begin();
		}
		return it;
	};
	auto FuncBackIt=[](std::list<std::pair<Vector2D,size_t>>::iterator it,std::list<std::pair<Vector2D,size_t>> &list){
		if(it==list.begin()){
			it=list.end();
		}
		it--;
		return it;
	};
	//�O�p�`�������s���A�����ł����S�Ă̎O�p�`�ɂ��ē������菈�����s��
	triangleSet->clear();
	//�O�p�`����������
	std::list<std::pair<Vector2D,size_t>> vertexList={std::pair<Vector2D,size_t>(m_position,0)};//�����Ă��Ȃ����_�ꗗ�B�����l��Polygon�̑S�Ă̒��_�B���X�g�̒����폜����s�ׂ������̂őo�������X�g�̕�����|�ɍ����Ă���B
	for(size_t i=0,size=m_edgeVecs.size();i<size;i++){
		vertexList.push_back(std::pair<Vector2D,size_t>(vertexList.back().first+m_edgeVecs[i],i+1));
	}
	size_t leftCount=vertexList.size();//�c���
	//�O�p�`�������s
	while(leftCount>3){
		//�c�蒸�_����3�ȉ��ɂȂ�܂Œ��_��1����菜��
		//���_����ł������_�����o��
		std::list<std::pair<Vector2D,size_t>>::iterator baseIt=vertexList.begin();
		for(std::list<std::pair<Vector2D,size_t>>::iterator it=vertexList.begin(),ite=vertexList.end();it!=ite;it++){
			if(it->first.sqSize()>baseIt->first.sqSize()){
				baseIt=it;
			}
		}
		//���̓_�Ɨאڂ���2�_�ɂ��O�p�`�ɂ��āA�O�ς����߂Ă���
		//�ŉ��_����Ƃ���O�p�`�͕K�����p�`�������܂ނ��߁A�O�p�`�̌����������O�ς̌�����p����΁A�u�O�p�`�����p�`�̊O���ɑ��݂���v�Ƃ����p�^�[����e����B
		std::list<std::pair<Vector2D,size_t>>::iterator backIt,nextIt;
		const float baseCross=((FuncBackIt(baseIt,vertexList)->first)-(baseIt->first)).cross((FuncNextIt(baseIt,vertexList)->first)-(baseIt->first));
		//�ŉ��_����baseIt��++����������ɏ��ԂɎO�p�`�����ł��邩���Ă���
		while(true){
			//�O�p�`�̌����̏������N���A���Ă��邩
			const float cross=((FuncBackIt(baseIt,vertexList)->first)-(baseIt->first)).cross((FuncNextIt(baseIt,vertexList)->first)-(baseIt->first));
			if(cross*baseCross>0.0f){
				//�O�p�`�����ɑ��̑��p�`�_���Ȃ����𔻒�
				std::list<std::pair<Vector2D,size_t>>::iterator it=FuncNextIt(FuncNextIt(baseIt,vertexList),vertexList);
				bool noexist=true;
				for(;it!=FuncBackIt(baseIt,vertexList);it=FuncNextIt(it,vertexList)){
					if(JudgeInTriangle(it->first,FuncBackIt(baseIt,vertexList)->first,baseIt->first,FuncNextIt(baseIt,vertexList)->first)){
						//�O�ς̕������ǂ�������Ȃ�_�������ɂ���̂ŁA�����ɂ��锻�������for���[�v�I��
						noexist=false;
						break;
					}
				}
				if(noexist){
					//�O�p�`�����ł���ꍇ�̓��[�v��E�o
					triangleSet->push_back({FuncBackIt(baseIt,vertexList)->second,baseIt->second,FuncNextIt(baseIt,vertexList)->second});
					vertexList.erase(baseIt);
					leftCount--;
					break;
				}
			}
			//�����ł��Ȃ��ꍇ�͎��̓_��
			baseIt=FuncNextIt(baseIt,vertexList);
		}
	}
	//�Ō�ɗ]����3�_�̎O�p�`���i�[
	std::array<size_t,3> tri;
	std::list<std::pair<Vector2D,size_t>>::iterator it=vertexList.begin();
	for(size_t i=0;i<3;i++,it++){
		tri[i]=it->second;
	}
	triangleSet->push_back(tri);
}

void MyPolygon::CalculateAllPointPosition(std::vector<Vector2D> *vertexPos)const{
	vertexPos->clear();
	vertexPos->reserve(m_edgeVecs.size()+1);//�������̂���
	vertexPos->push_back(m_position);
	Vector2D v=m_position;
	for(const Vector2D &vec:m_edgeVecs){
		v+=vec;
		vertexPos->push_back(v);
	}
}

void MyPolygon::Draw(Vector2D point,Vector2D adjust,unsigned int color,int fillFlag,float lineThickness)const{
	//���𖞂������ǂ����ŏ�����ς���B�������Ȃ��ꍇ�͎O�p�`���������Ȃ��ėǂ��̂ō������ł��邽��
	if(fillFlag==TRUE){
		//���𖞂����ꍇ
		//�O�p�`����
		//std::vector<std::array<size_t,3>> triangleSet;
		//DivideTriangle(&triangleSet);

		//���������O�p�`�̏W�����ł����̂ŁA�S�Ă̎O�p�`�ɑ΂��ē���������s���Ă���
		//�ė��p�����l���āA�O�ł͒��_�ԍ��ŎO�p�`�W����\�����āA������ł܂����W�ꗗ������Ă���
		std::vector<Vector2D> vertexVec;
		CalculateAllPointPosition(&vertexVec);
		//�S�Ă̕����O�p�`�ɑ΂��āA������������s
		unsigned int c=VJudgePointInsideShape(GetMousePointVector2D())?GetColor(168,128,128):GetColor(128,168,128);
		for(size_t i=0,size=m_triangleSet.size();i<size;i++){
			DrawTriangleAA(vertexVec[m_triangleSet[i][0]].x,vertexVec[m_triangleSet[i][0]].y,
				vertexVec[m_triangleSet[i][1]].x,vertexVec[m_triangleSet[i][1]].y,
				vertexVec[m_triangleSet[i][2]].x,vertexVec[m_triangleSet[i][2]].y,
				c,TRUE,lineThickness);
			DrawTriangleAA(vertexVec[m_triangleSet[i][0]].x,vertexVec[m_triangleSet[i][0]].y,
				vertexVec[m_triangleSet[i][1]].x,vertexVec[m_triangleSet[i][1]].y,
				vertexVec[m_triangleSet[i][2]].x,vertexVec[m_triangleSet[i][2]].y,
				color,FALSE,lineThickness);
		}

	} else{
		//�ӂ̂ݕ`�悷��ꍇ
		Vector2D pos=point+adjust,next;
		for(const Vector2D &edge:GetAllEdgeVecs()){
			//�n�_���������x�n�_�ɖ߂�܂ł̑S�Ă̕ӂ̕`��
			next=pos+edge;
			DrawLineAA(pos.x,pos.y,next.x,next.y,color,lineThickness);
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
	std::vector<Vector2D> vertexVec;
	CalculateAllPointPosition(&vertexVec);
	for(const std::array<size_t,3> &index:m_triangleSet){
		if(JudgeInTriangle(point,vertexVec[index[0]],vertexVec[index[1]],vertexVec[index[2]])){
			//�ǂꂩ1�̎O�p�`�ɓ����Ă���Α��p�`�����ɓ����Ă���
			return true;
		}
	}

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
