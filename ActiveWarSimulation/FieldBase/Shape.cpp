#include"Shape.h"

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
