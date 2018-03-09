#include"ShapeHaving.h"
#include"DxLib.h"

//----------------ShapeHaving-----------------
const float ShapeHaving::pushRate=0.4f;

ShapeHaving::ShapeHaving(std::shared_ptr<Shape> pShape):m_hitJudgeShape(pShape){}

ShapeHaving::~ShapeHaving(){}

void ShapeHaving::ShapeDraw(unsigned int color,int fillFlag,Vector2D adjust)const{
	m_hitJudgeShape->Draw(adjust,color,fillFlag);
}

void ShapeHaving::PosDraw(unsigned int color,int fillFlag,float size,Vector2D adjust)const{
	Vector2D pos=adjust+m_hitJudgeShape->GetPosition();
	DrawCircleAA(pos.x,pos.y,size,color,fillFlag);
}

void ShapeHaving::Move(Vector2D v){
	m_hitJudgeShape->Move(v);
}

void ShapeHaving::Warp(Vector2D v){
	m_hitJudgeShape->Move(v-m_hitJudgeShape->GetPosition());
}

void ShapeHaving::Resize(Vector2D v){
	//��_����}�E�X�����񂾃x�N�g���������ɓn��
	m_hitJudgeShape.get()->Resize(v-m_hitJudgeShape->GetPosition());
}

void ShapeHaving::ChangeShape(std::shared_ptr<Shape> pShape){
	m_hitJudgeShape=pShape;
}

bool ShapeHaving::JudgePointInsideShape(Vector2D point)const{
	return m_hitJudgeShape.get()->VJudgePointInsideShape(point);
}

float ShapeHaving::getTop()const {
	return m_hitJudgeShape->GetLeftTop().y;
}

float ShapeHaving::getBottom()const {
	return m_hitJudgeShape->GetRightBottom().y;
}

float ShapeHaving::getLeft()const {
	return m_hitJudgeShape->GetLeftTop().x;
}

float ShapeHaving::getRight()const {
	return m_hitJudgeShape->GetRightBottom().x;
}

Vector2D ShapeHaving::getPos()const {
	return m_hitJudgeShape->GetPosition();
}

const Shape *ShapeHaving::GetHitJudgeShape()const{
	return m_hitJudgeShape.get();
}

Shape::Fix::Kind ShapeHaving::GetFix()const{
	return m_hitJudgeShape->m_fix;
}

Shape::Fix::Kind ShapeHaving::SetFix(Shape::Fix::Kind fix)const{
	return (m_hitJudgeShape->m_fix=fix);
}

Vector2D ShapeHaving::getResizeVec()const{
	//m_hitJudgeShape�̂��̂̓I�u�W�F�N�g�̈ʒu���l�����Ă��Ȃ��̂ł���𑫂��ĕԂ�
	return m_hitJudgeShape.get()->GetRetResize()+m_hitJudgeShape->GetPosition();
}

const std::vector<const ShapeHaving *> ShapeHaving::InShapeHavingList(const ShapeHaving * const * const pShapeHavingVec,const size_t vecSize)const{
	std::vector<const ShapeHaving *> list;
	const Vector2D v=Vector2D();
	for(size_t i=0;i<vecSize;i++){
		if(m_hitJudgeShape->CalculatePushVec(pShapeHavingVec[i]->GetHitJudgeShape())!=v){
			list.push_back(pShapeHavingVec[i]);
		}
	}
	return list;
}

void ShapeHaving::UpdatePosition(ShapeHaving * const * const pShapeHavingVec,const size_t vecSize,const size_t updateTimes){
	if(m_hitJudgeShape->m_fix==Shape::Fix::e_dynamic){
		//m_fix�ɂ���ē������ǂ��������߂�
		//���̊֐����Ă΂ꂽ�ې}�`�����݂���͉̂��ړ���ł���B���ړ���ɂ����āA�����Əd�Ȃ��Ă���}�`��T���A�ʒu�����炷�B
		//�d�Ȃ��Ă���}�`��S�ă��X�g�A�b�v����
		std::vector<const ShapeHaving *> inShapeList;
		for(size_t i=0;i<vecSize;i++){
			//�����蔻��ɉe����^����}�`�ȊO�ɑ΂��ĉ����o���������s��
			if(pShapeHavingVec[i]->GetFix()!=Shape::Fix::e_ignore){
				//�}�`���҂�������߂钷���`�𗘗p���ĊȈՓI�ȓ����蔻����s��
				if(JudgeInShapeRect(pShapeHavingVec[i])){
					//�����o������}�`�ɒǉ�
					inShapeList.push_back(pShapeHavingVec[i]);
				}
			}
		}
		//�ȈՓI�����蔻��ɂ���Ĕ���ł���A�d�Ȃ��Ă���}�`�̊O�ɏo��悤�ɉ����o������������
		for(size_t i=0;i<updateTimes;i++){
			for(const ShapeHaving *pShapeHaving:inShapeList){
				//�e�}�`�ɑ΂��Ċ��S�ɉ����o���鋗�������߂�
				Vector2D pushVec=m_hitJudgeShape->CalculatePushVec(pShapeHaving->GetHitJudgeShape());
				//�����o�������̈��̊����̃x�N�g�����������o��
				this->Move(pushVec*ShapeHaving::pushRate);
			}
		}
	}
}

bool ShapeHaving::JudgeInShapeRect(const ShapeHaving *pShapeHaving)const{
	return this->getLeft()<=pShapeHaving->getRight() && this->getRight()>=pShapeHaving->getLeft() && this->getTop()<=pShapeHaving->getBottom() && this->getBottom()>=pShapeHaving->getTop();
}

