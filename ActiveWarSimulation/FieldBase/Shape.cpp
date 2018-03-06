#include"Shape.h"
#include"Circle.h"
#include"Edge.h"

//---------------------Shape::Type---------------------
std::string Shape::Type::GetStr(Kind kind){
	switch(kind){
	case(e_circle):
		return "Circle";
	case(e_edge):
		return "Edge";
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
const float Shape::pushRate=0.4f;
const char Shape::spliter=',';
const char Shape::beginer='(';
const char Shape::ender=')';

Shape::Shape(Vector2D position,Type::Kind type,Fix::Kind fix)
	:m_position(position),m_type(type),m_fix(fix){}

Shape::~Shape(){}

void Shape::Move(Vector2D displacement){
	m_position+=displacement;
}

const std::vector<std::shared_ptr<const Shape>> Shape::InShapeList(const std::vector<std::shared_ptr<const Shape>> &pShapeVec)const{
	std::vector<std::shared_ptr<const Shape>> list;
	const Vector2D v=Vector2D();
	for(const std::shared_ptr<const Shape> &pShape:pShapeVec){
		if(CalculatePushVec(pShape.get())!=v){
			list.push_back(pShape);
		}
	}
	return list;
}

void Shape::Update(const std::vector<std::shared_ptr<Shape>> &pShapeVec,size_t updateTimes){
	if(m_fix==Fix::e_dynamic){
		//m_fix�ɂ���ē������ǂ��������߂�
		//���̊֐����Ă΂ꂽ�ې}�`�����݂���͉̂��ړ���ł���B���ړ���ɂ����āA�����Əd�Ȃ��Ă���}�`��T���A�ʒu�����炷�B
		//�܂��d�Ȃ��Ă���}�`��S�ă��X�g�A�b�v����
		std::vector<std::shared_ptr<const Shape>> inShapeList;
		for(const std::shared_ptr<const Shape> &pShape:pShapeVec){
			//�����蔻��ɉe����^����}�`�ȊO�ɑ΂��ĉ����o���������s��
			if(pShape->m_fix!=Fix::e_ignore){
				//�}�`���҂�������߂钷���`�𗘗p���ĊȈՓI�ȓ����蔻����s��
				if(JudgeInShapeRect(pShape.get())){
					//�����o������}�`�ɒǉ�
					inShapeList.push_back(pShape);
				}
			}
		}
		//�ȈՓI�����蔻��ɂ���Ĕ���ł���A�d�Ȃ��Ă���}�`�̊O�ɏo��悤�ɉ����o������������
		for(size_t i=0;i<updateTimes;i++){
			for(const std::shared_ptr<const Shape> &pShape:inShapeList){
				//�e�}�`�ɑ΂��Ċ��S�ɉ����o���鋗�������߂�
				Vector2D pushVec=CalculatePushVec(pShape.get());
				//�����o�������̈��̊����̃x�N�g�����������o��
				m_position+=pushVec*Shape::pushRate;
			}
		}
	}
}

bool Shape::JudgeInShapeRect(const Shape *pShape)const{
	return this->GetLeftTop().x<=pShape->GetRightBottom().x && this->GetRightBottom().x>=pShape->GetLeftTop().x && this->GetLeftTop().y<=pShape->GetRightBottom().y && this->GetRightBottom().y>=pShape->GetLeftTop().y;
}

std::shared_ptr<Shape> Shape::CreateShape(const std::string &infostr){
	//�u���,�ʒu,�����Œ�,�}�`���v�̏��B���ꂼ��̗v�f�𒊏o����B
	//�S��()���W�������A,����؂蕶���Ȃ̂ňꊇ�ŕ�������B
	StringBuilder sb(infostr,spliter,beginer,ender,true,true);
	//str�̉��߁Bsb.m_vec[0]:�I�u�W�F�N�g�̎�� sb.m_vec[1]:�����蔻��}�`�̏��((x,y)�`��) sb.m_vec[2]:�����Œ� sb.m_vec[3]:�}�`���
	std::shared_ptr<Shape> pShape(nullptr);
	try{
		//�ʒu�̐���(���ʏ���)
		Vector2D pos(std::stof(sb.m_vec.at(1).m_vec.at(0).GetString()),std::stof(sb.m_vec.at(1).m_vec.at(1).GetString()));
		//�����Œ�̐���
		const std::string fixName=sb.m_vec.at(2).GetString();
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
		if(sb.m_vec.at(0).GetString()==Type::GetStr(Type::e_circle)){
			//�~�̐������s���Bm_vec[3]�̒��g�͔��a�̒l�̂݁B
			float r=std::stof(sb.m_vec.at(3).GetString());
			pShape=std::shared_ptr<Shape>(new Circle(pos,r,fix));
		} else if(sb.m_vec.at(0).GetString()==Type::GetStr(Type::e_edge)){
			//�����̐������s���Bm_vec[3]�̒��g��(x,y)�`���̍��W�̂݁B
			Vector2D vec(std::stof(sb.m_vec.at(3).m_vec.at(0).GetString()),std::stof(sb.m_vec.at(3).m_vec.at(1).GetString()));
			pShape=std::shared_ptr<Shape>(new Edge(pos,vec,fix));
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
