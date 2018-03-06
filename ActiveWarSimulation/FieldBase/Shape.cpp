#include"Shape.h"

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

void Shape::Update(const std::vector<std::shared_ptr<Shape>> &pShapeVec){
	if(m_fix==Fix::e_dynamic){
		//m_fix�ɂ���ē������ǂ��������߂�
		//���̊֐����Ă΂ꂽ�ې}�`�����݂���͉̂��ړ���ł���B���ړ���ɂ����āA�����Əd�Ȃ��Ă���}�`��T���A�ʒu�����炷�B
		//�d�Ȃ��Ă���}�`�̊O�ɏo��悤�ɉ����o������������
		const float pushRate=Shape::pushRate;//�}�`���d�Ȃ�Ȃ��Ȃ�̂ɂ҂�����̋����ɑ΂��A�����o�������̊����B1�t���[�����Ŋ��S�ɉ����o���ƁA�}�`�ɋ��܂������ɕs���R�ȋ���������B
		for(const std::shared_ptr<const Shape> &pShape:pShapeVec){
			//�����蔻��ɉe����^����}�`�ȊO�ɑ΂��ĉ����o���������s��
			if(pShape->m_fix!=Fix::e_ignore){
				//�e�}�`�ɑ΂��Ċ��S�ɉ����o���鋗�������߂�
				Vector2D pushVec=CalculatePushVec(pShape.get());
				//�����o�������̈��̊����̃x�N�g�����������o��
				m_position+=pushVec*pushRate;
			}
		}
	}
}

std::shared_ptr<Shape> Shape::CreateShape(const std::string &infostr){
	//�u���,�ʒu,�����Œ�,�}�`���v�̏��B���ꂼ��̗v�f�𒊏o����B
	std::vector<std::string> strVec;//������������
	char ch;//�ǂݍ���1�������ꎞ�i�[����
	int tokenCount=0;//�ǂݍ���(�̌�-�ǂݍ���)�̌��B0���傫���Ȃ�str�ɒǉ��B0�����ɂ͂Ȃ�Ȃ��悤�ɂ���B
	std::string str;//�ǂݍ���()��������
	str.reserve(40);//�������x�𑬂߂邽�߂�reserve�͂��Ă����B�E�l�Z�ɂȂ�B
	for(std::string::const_iterator it=infostr.begin(),ite=infostr.end();it!=ite;it++){
		ch=*it;
		//�g�[�N����'(',')',','��3��
		if(ch==',' && tokenCount<=0){
			//�I�u�W�F�N�g�ǂݍ��݊J�n�A������()���̂��͖̂�������
			
		} else if(ch==')'){
			//�g�[�N����count�𒲐�
			if(tokenCount>=0){
				//���̌��ɂȂ�g�[�N���͖�������
				tokenCount--;
				if(tokenCount>0){
					//�g�[�N����count�����Ȃ�ǂݍ��݂𑱂���
					str.push_back(ch);
				} else{
					//�g�[�N����count��0�ɂȂ�����()���ǂݍ��݂͏I���AstrVec�Ɋi�[
					strVec.push_back(str);
					str.clear();
				}
			}
		} else{
			//tokenCount��0���傫���Ȃ�str�ɒǉ�
			if(tokenCount>0){
				str.push_back(ch);
			}
		}
	}
	//str�̉��߁BstrVec[0]:�I�u�W�F�N�g�̎�� strVec[1]:�����蔻��}�`�̏��

	return std::shared_ptr<Shape>(nullptr);
}
