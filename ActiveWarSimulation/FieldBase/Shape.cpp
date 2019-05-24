#include"Shape.h"
#include"Circle.h"
#include"Edge.h"
#include"MyPolygon.h"
#include"StringBuilder.h"

//---------------------Shape::Type---------------------
std::string Shape::Type::GetStr(Kind kind){
	switch(kind){
	case(e_circle):
		return "Circle";
	case(e_edge):
		return "Edge";
	case(e_polygon):
		return "Polygon";
	default:
		return "";
	}
}

//---------------------Shape::Fix---------------------
std::string Shape::Fix::GetStr(Kind kind){
	switch(kind){
	case(e_dynamic):
		return "dynamic";
	case(e_static):
		return "static";
	case(e_ignore):
		return "ignore";
	default:
		return "";
	}
}

//---------------------Shape---------------------
const char Shape::spliter=',';
const char Shape::beginer='(';
const char Shape::ender=')';

Shape::Shape(Vector2D position,Type::Kind type,Fix::Kind fix)
	:m_position(position),m_type(type),m_fix(fix){}

Shape::~Shape(){}

void Shape::Move(Vector2D displacement){
	m_position+=displacement;
}

void Shape::Draw(Vector2D adjust,unsigned int color,int fillFlag,float lineThickness)const{
	Draw(m_position,adjust,color,fillFlag,lineThickness);
}

std::shared_ptr<Shape> Shape::CreateShape(StringBuilder &info){
	//�u���,�ʒu,�����Œ�,�}�`���v�̏��B���ꂼ��̗v�f�𒊏o����B
	//�S��()���W�������A,����؂蕶���Ȃ̂ňꊇ�ŕ�������B
	info.Split(spliter,beginer,ender);
	//str�̉��߁Binfo.m_vec[0]:�I�u�W�F�N�g�̎�� info.m_vec[1]:�����蔻��}�`�̏��((x,y)�`��) info.m_vec[2]:�����Œ� info.m_vec[3]:�}�`���
	std::shared_ptr<Shape> pShape(nullptr);
	try{
		//�ʒu�̐���(���ʏ���)
		Vector2D pos(std::stof(info.m_vec.at(1).m_vec.at(0).GetString()),std::stof(info.m_vec.at(1).m_vec.at(1).GetString()));
		//�����Œ�̐���
		const std::string fixName=info.m_vec.at(2).GetString();
		Fix::Kind fix;
		if(fixName==Fix::GetStr(Fix::e_dynamic)){
			fix=Fix::e_dynamic;
		} else if(fixName==Fix::GetStr(Fix::e_ignore)){
			fix=Fix::e_ignore;
		} else if(fixName==Fix::GetStr(Fix::e_static)){
			fix=Fix::e_static;
		} else{
			//�ϊ��ł��Ȃ��̂ŗ�O����
			throw std::invalid_argument("");
		}
		//�}�`�e���̏���
		if(info.m_vec.at(0).GetString()==Type::GetStr(Type::e_circle)){
			//�~�̐������s���Bm_vec[3]�̒��g�͔��a�̒l�̂݁B
			float r=std::stof(info.m_vec.at(3).GetString());
			pShape=std::shared_ptr<Shape>(new Circle(pos,r,fix));
		} else if(info.m_vec.at(0).GetString()==Type::GetStr(Type::e_edge)){
			//�����̐������s���Bm_vec[3]�̒��g��(x,y)�`���̍��W�̂݁B
			Vector2D vec(std::stof(info.m_vec.at(3).m_vec.at(0).GetString()),std::stof(info.m_vec.at(3).m_vec.at(1).GetString()));
			pShape=std::shared_ptr<Shape>(new Edge(pos,vec,fix));
		} else if(info.m_vec.at(0).GetString()==Type::GetStr(Type::e_polygon)){
			//���p�`�̐������s���Bm_vec[3]�̒��g��()�őS�̂��͂��A���̒���(x,y)�`���̍��W���i�[����Ă���
			const size_t size=info.m_vec.at(3).m_vec.size();
			std::vector<Vector2D> points;
			points.reserve(size);
			for(size_t i=0;i<size;i++){
				//���_���W��S�Ċi�[
				points.push_back(Vector2D(std::stof(info.m_vec.at(3).m_vec.at(i).m_vec.at(0).GetString()),std::stof(info.m_vec.at(3).m_vec.at(i).m_vec.at(1).GetString())));
			}
			pShape=std::shared_ptr<Shape>(new MyPolygon(pos,points,fix));
		}
	} catch(std::invalid_argument){
		//string��float�̏��������~�X������
		//string��Fix::Kind�̕ϊ����~�X������
	} catch(std::out_of_range){
		//string�����l�̕ϊ��ɂ����Ĕ͈͊O�ɂȂ��Ă��܂������B
		//�܂��z��O�Q�Ƃ�������
	}
	return pShape;
}
