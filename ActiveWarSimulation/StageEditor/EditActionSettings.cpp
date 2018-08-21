#include<math.h>
#include"EditActionSettings.h"

#include"DxLib.h"
#include"StageEditor.h"
#include"EditAction.h"
#include"ShapeFactory.h"
#include"PosSetting.h"

#include"Terrain.h"
#include"Circle.h"

EditActionSettings::EditActionSettings(std::shared_ptr<EditAction> pEditAction, std::shared_ptr<BattleObject> pBattleObject,std::shared_ptr<ShapeFactory> pShapeFactory,std::shared_ptr<PosSetting> pPosSetting)
	:m_adjust(0,0),m_pEditAction(pEditAction),m_pBattleObject(pBattleObject),m_pShapeFactory(pShapeFactory),m_pPosSetting(pPosSetting),m_pOriginObject(std::shared_ptr<BattleObject>(nullptr))
	,m_mouseObjectDepth(0,Vector2D(0.0f,0.0f))
{
	
}

EditActionSettings::~EditActionSettings() {}

std::vector<std::shared_ptr<BattleObject>>::const_iterator EditActionSettings::GetMousePointedObject(Vector2D point)const{
	std::vector<std::shared_ptr<BattleObject>>::const_iterator it=m_objects.begin(),ite=m_objects.end();
	size_t count=0;
	for(;it!=ite;it++) {
		//�}�E�X������Ă���}�`�ɂ͘g��`�悵�t�H�[�J�X��\���B������m_mouseobjectDepth.first�R�ڂɌ��������̂̂�
		if(it->get()->JudgePointInsideShape(point)){
			if(count==m_mouseObjectDepth.first){
				break;
			} else{
				//���ɒB���Ă��Ȃ��ꍇ�͂��̃C�e���[�^��Ԃ��Ȃ�
				count++;
			}
		}
	}
	return it;
}

const BattleObject *EditActionSettings::GetMousePointedObjectPointer(Vector2D point)const{
	std::vector<std::shared_ptr<BattleObject>>::const_iterator it=GetMousePointedObject(point);
	if(it==m_objects.end()){
		return nullptr;
	}
	return it->get();
}

void EditActionSettings::PracticeEdit(Vector2D point){
	//�}�E�X���W���}�b�v�����W�ɕϊ����A�X�Ɉʒu�������Ă���ҏW
	m_pEditAction.get()->VProcessAction(m_pPosSetting->CalculatePos(point+m_adjust,*this),*this);
}

void EditActionSettings::PracticeNonPressEdit(Vector2D point){
	//�}�E�X���W���}�b�v�����W�ɕϊ����A�X�Ɉʒu�������Ă���ҏW
	m_pEditAction.get()->VNonPressEditing(m_pPosSetting->CalculatePos(point+m_adjust,*this),*this);
}

void EditActionSettings::PushScrollBar(float scrollpx,float maxX,float maxY,int mouseX,int mouseY,int leftUpPosX,int leftUpPosY,int mapSizeX,int mapSizeY){
	if(mouseY*leftUpPosX-mouseX*leftUpPosY<0 && (mouseY-leftUpPosY)*leftUpPosX+(mouseX-(mapSizeX+leftUpPosX))*leftUpPosY<0){
		//�㕔��(1��艺�A2��艺)
		m_adjust.y=fmax((float)0,m_adjust.y-scrollpx);
	} else if(mouseY*leftUpPosX-mouseX*leftUpPosY>=0 && (mouseY-(leftUpPosY+mapSizeY))*leftUpPosX+(mouseX-leftUpPosX)*leftUpPosY<0){
		//������(1����A2��艺)
		m_adjust.x=fmax((float)0,m_adjust.x-scrollpx);
	} else if((mouseY-leftUpPosY)*leftUpPosX+(mouseX-(mapSizeX+leftUpPosX))*leftUpPosY>=0 && (mouseY-(leftUpPosY+mapSizeY))*leftUpPosX-(mouseX-(mapSizeX+leftUpPosX))*leftUpPosY<0){
		//�E����(1��艺�A2����)
		m_adjust.x=fmin(maxX-mapSizeX,m_adjust.x+scrollpx);
	} else{
		//������(1����A2����)
		m_adjust.y=fmin(maxY-mapSizeY,m_adjust.y+scrollpx);
	}
}

void EditActionSettings::PushScrollBar(Vector2D move){
	//�ő�l�ŏ��l���l������
	Vector2D a=m_adjust+move;
	float maxX=2000-(float)StageEditor::mapSizeX,maxY=2000-(float)StageEditor::mapSizeY;
	m_adjust=Vector2D(
		a.x>=0 ? (a.x<maxX ? a.x : maxX) : 0
		,a.y>=0 ? (a.y<maxY ? a.y : maxY) : 0
	);
}

void EditActionSettings::DrawEditButtonPushed()const{
	m_pEditAction.get()->DrawPushedButton();
}

void EditActionSettings::DrawShapeFactoryButtonPushed()const{
	m_pShapeFactory->DrawPushedButtonLight();
}

void EditActionSettings::DrawPosSettingButtonPushed()const{
	m_pPosSetting->DrawPushedButtonLight();
}

void EditActionSettings::DrawPosSettingGuide(int leftUpPosX,int leftUpPosY,int mapSizeX,int mapSizeY)const{
	m_pPosSetting->DrawGuide(leftUpPosX,leftUpPosY,mapSizeX,mapSizeY,m_adjust);
}

void EditActionSettings::PutObject(Vector2D point){
	if(m_pBattleObject.get()!=nullptr){
		//�I�u�W�F�N�g�����݂̃}�E�X�̈ʒu�ɍ��킹�Ă���ݒu
		m_pBattleObject.get()->Warp(point);
		m_objects.push_back(m_pBattleObject);
		//m_pBattleObject�����̂܂܂ɂ���Ɠ����|�C���^�̃I�u�W�F�N�g���Ⴄ�ꏊ�ɒu�����Ƃ��Ă��܂��̂ŁA�|�C���^�͐V��������
		m_pBattleObject=m_pBattleObject->VCopy();
	}
}

void EditActionSettings::RemoveObject(Vector2D point){
	//��菜���I�u�W�F�N�g��T��
	std::vector<std::shared_ptr<BattleObject>>::const_iterator it=GetMousePointedObject(point);
	if(it!=m_objects.end()){
		m_objects.erase(it);
	}
}

void EditActionSettings::SetEditObject(Vector2D point){
	std::vector<std::shared_ptr<BattleObject>>::const_iterator it=GetMousePointedObject(point);
	if(it!=m_objects.end()){
		m_pBattleObject=*it;
		m_pOriginObject=it->get()->VCopy();
	}
}

void EditActionSettings::CancelEditing(){
	//�G�f�B�^�ŕύX�ł���͈̂ʒu�Ƒ傫���̂�
	if(m_pBattleObject.get()!=nullptr && m_pOriginObject.get()!=nullptr){
		m_pBattleObject.get()->Warp(m_pOriginObject.get()->getPos());
		m_pBattleObject.get()->Resize(m_pOriginObject.get()->getResizeVec());
	}
}

void EditActionSettings::InitEditObject(){
	m_pBattleObject=std::shared_ptr<BattleObject>(nullptr);
	m_pOriginObject=std::shared_ptr<BattleObject>(nullptr);
}

void EditActionSettings::UpdateMouseObjectDepth(const int keyinputright){
	const Vector2D vec=GetMousePointVector2D();//�}�E�X�̈ʒu�̓}�b�v��ł���K�v�͂Ȃ�
	if(keyinputright==1){
		//�E�N���b�N�ŉ񐔂𑝂₷
		m_mouseObjectDepth.first++;
		m_mouseObjectDepth.second=vec;
	}else if((vec-m_mouseObjectDepth.second).sqSize()>=100.0f){
		//�O�ɉE�N���b�N�����ʒu����10px�ȏ㗣��Ă����
		m_mouseObjectDepth.first=0;
		m_mouseObjectDepth.second=vec;
	}
}

//����f�[�^�̏����o��
void EditActionSettings::WriteOutStage(const char *filename)const{
	/*
	�`����
	{(�I�u�W�F�N�g���),(�}�`���)}
	{(Terrain),(Edge,(200,20),(30,10))}
	*/
	//�t�@�C�����J��
	std::ofstream ofs(filename,std::ios_base::trunc);
	if(!ofs){
		return;
	}
	//�S�ẴI�u�W�F�N�g���������o��
	for(const std::shared_ptr<BattleObject> &pObj:m_objects){
		pObj->WriteOutObjectWholeInfo(ofs);
	}
}

//�X�e�[�W�̓ǂݍ���
void EditActionSettings::ReadStage(const char *filename){
	m_objects.clear();
	//�t�@�C�����J�����ׂĂ̕�����������o��
	std::ifstream ifs(filename);
	std::string str;//�����o����
	while(true){
		char ch;
		ch=ifs.get();
		//�t�@�C���̏I�[��while����E�o
		if(ch==EOF){
			break;
		}
		str.push_back(ch);
	}
	//�I�u�W�F�N�g�Q��{}�ň͂܂�\n�ŋ�؂��Ă���̂ŁA�P�K�w���������ǂݍ��݂��āA�I�u�W�F�N�g�𐶐�����
	StringBuilder sb(str,'\n','{','}',false,true);
	for(const StringBuilder &ssb:sb.m_vec){
		std::shared_ptr<BattleObject> pb=BattleObject::CreateObject(ssb.GetString());
		if(pb.get()!=nullptr){
			m_objects.push_back(pb);
		}
	}
}

