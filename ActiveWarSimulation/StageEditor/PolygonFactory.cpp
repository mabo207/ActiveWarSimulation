#include"DxLib.h"
#include"PolygonFactory.h"
#include"MyPolygon.h"
#include"Edge.h"
#include"EditActionSettings.h"
#include"BattleObject.h"
#include"StageEditor.h"

//-----------------------PolygonFactory::PolygonFactoryButton-----------------------
PolygonFactory::PolygonFactoryButton::PolygonFactoryButton(Vector2D point,Vector2D vec)
	:ShapeFactoryButton(point,vec){}

void PolygonFactory::PolygonFactoryButton::ButtonDraw(int font,int fillFlag)const{
	//�����������ă{�^���̕`��
	SetDrawBlendMode(DX_BLENDMODE_ALPHA,128);
	DrawBox((int)m_point.x,(int)m_point.y,(int)(m_point+m_vec).x,(int)(m_point+m_vec).y,GetColor(192,192,192),fillFlag);//���g(���傢���߂�����)
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND,255);
	//�g���̕`��
	DrawBox((int)m_point.x,(int)m_point.y,(int)(m_point+m_vec).x,(int)(m_point+m_vec).y,GetColor(192,192,64),FALSE);//�O�g
	//�����蔻��}�`�̕`��
	const float size=20.0f;
	DrawTriangleAA((m_point+m_vec/2).x-size,(m_point+m_vec/2).y-size,(m_point+m_vec/2).x+size,(m_point+m_vec/2).y+size,(m_point+m_vec/2).x+size-5.0f,(m_point+m_vec/2).y+5.0f,GetColor(255,255,255),TRUE);
	DrawTriangleAA((m_point+m_vec/2).x-size,(m_point+m_vec/2).y-size,(m_point+m_vec/2).x+size,(m_point+m_vec/2).y+size,(m_point+m_vec/2).x+5.0f,(m_point+m_vec/2).y+size-5.0f,GetColor(255,255,255),TRUE);
}

void PolygonFactory::PolygonFactoryButton::PushedProcess(EditActionSettings &settings)const{
	settings.m_pShapeFactory=std::shared_ptr<ShapeFactory>(new PolygonFactory(m_point,m_vec,GetColor(255,255,0)));
	//���ݑI�����Ă���I�u�W�F�N�g���A�����蔻��}�`��ύX����
	if(settings.m_pBattleObject.get()!=nullptr){
		settings.m_pBattleObject->ChangeShape(settings.m_pShapeFactory->CreateShape(settings.m_pBattleObject->getPos()));
	}
}

//-----------------------PolygonFactory-----------------------
const float PolygonFactory::pointCapacity=5.0f;

PolygonFactory::PolygonFactory(Vector2D buttonPos,Vector2D buttonSize,unsigned int lightcolor)
	:ShapeFactory(buttonPos,buttonSize,lightcolor){}

PolygonFactory::~PolygonFactory(){}

std::shared_ptr<Shape> PolygonFactory::CreateShape(Vector2D point)const{
	//�ЂƂ܂��ŏ��͐��������
	return std::shared_ptr<Shape>(new Edge(point,Vector2D(3.0f,3.0f),Shape::Fix::e_static));
}

EditPut::PosSetKind PolygonFactory::VPutAction(EditPut::PosSetKind pskind,Vector2D point,EditActionSettings &settings){
	if(pskind==EditPut::PosSetKind::BASENONEXIST){
		//�u���ꏊ�����߂Ă��鎞
		//�ʒu���m��
		settings.m_pBattleObject->Warp(point);
		//���_��������
		m_pointVec.clear();
		m_pointVec.push_back(point);
		//�}�`�̒��_�̌����
		return EditPut::PosSetKind::BASEEXIST;
	} else if(pskind==EditPut::PosSetKind::BASEEXIST){
		//�}�`�̒��_�����߂Ă��鎞
		if((point-settings.m_pBattleObject->getPos()).sqSize()>pointCapacity*pointCapacity){
			//�J�n�_���}�E�X�������ɂ��鎞�A�_��ǉ�����
			bool flag=true;
			for(const Vector2D v:m_pointVec){
				if((v-point).sqSize()==0.0f){
					//�����ꂩ�̓_�Əd�Ȃ��Ă���_�̒ǉ��͔F�߂Ȃ�
					flag=false;
					break;
				}
			}
			if(flag){
				//�_�̒ǉ�
				m_pointVec.push_back(point);
				//�}�`�̕ύX
				if(m_pointVec.size()==2){
					//�_��2�̎��͐����ɂȂ�
					settings.m_pBattleObject->ChangeShape(std::shared_ptr<Shape>(new Edge(m_pointVec[0],m_pointVec[1]-m_pointVec[0],settings.m_pBattleObject->GetFix())));
				} else if(m_pointVec.size()>=3){
					//�_��3�ȏ�̎��͑��p�`�ɂȂ�
					std::vector<Vector2D> noTop=m_pointVec;
					noTop.erase(noTop.begin());//�擪�͕ʂɗp�ӂ���̂ŏ���
					settings.m_pBattleObject->ChangeShape(std::shared_ptr<Shape>(new MyPolygon(m_pointVec[0],noTop,settings.m_pBattleObject->GetFix())));
				}
			}
			return EditPut::PosSetKind::BASEEXIST;//�}�`�̒��_�̌����
		} else{
			//�}�E�X���J�n�_�̋߂��ɂ��鎞�ɐ}�`���m�肷��			
			settings.PutObject(settings.m_pBattleObject->getPos());
			settings.m_pBattleObject->ChangeShape(CreateShape(point));//���p�`���̂܂܂ɂ���ƌ��Â炢�̂Ő����ɖ߂��Ă���
			m_pointVec.clear();//�`��ɉe�����o��̂ŃN���A���Ă���
			return EditPut::PosSetKind::BASENONEXIST;//�}�`�̈ʒu�̌����
		}
	}
	return EditPut::PosSetKind::NONEDIT;//��O�I�ȏ���

}

void PolygonFactory::VPutNotPressAction(EditPut::PosSetKind pskind,Vector2D point,EditActionSettings &settings)const{
	if(pskind==EditPut::PosSetKind::BASENONEXIST){
		//�u���ꏊ�����߂Ă��鎞
		settings.m_pBattleObject.get()->Warp(point);//�}�`�̈ʒu��ς���
	} else if(pskind==EditPut::PosSetKind::BASEEXIST){
		//�u���}�`�̒��_�����߂Ă��鎞
		if(settings.m_pBattleObject->GetHitJudgeShape()->GetType()==Shape::Type::e_edge){
			//m_pBattleObject�������ł����(�������̍ۂ͐����ł���)
			settings.m_pBattleObject->Resize(point);//�}�`�̑傫����ς���
		} else if(settings.m_pBattleObject->GetHitJudgeShape()->GetType()==Shape::Type::e_polygon){
			//���p�`�ɂȂ��Ă��鎞�͓��ɉ������Ȃ�
		}
	}
}

void PolygonFactory::FactoryDraw(const Vector2D adjust,const EditActionSettings &settings)const{
	//�ʏ�Ɠ������}�`��`��
	int mode,pal;
	GetDrawBlendMode(&mode,&pal);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA,128);
	if(settings.GetMPOriginObject()!=nullptr){
		settings.GetMPOriginObject()->VDraw(adjust);
	}
	//�ʏ�ƈقȂ�Am_pointVec��S�Č��񂾂��̂��A���݂̈ʒu�ƂƂ��ɕ\��
	const size_t size=m_pointVec.size();
	if(size>0){
		const unsigned int lineColor=GetColor(255,0,255);
		for(size_t i=0;i+1<size;i++){
			Vector2D p[2]={m_pointVec[i]+adjust,m_pointVec[i+1]+adjust};
			DrawLineAA(p[0].x,p[0].y,p[1].x,p[1].y,lineColor,1.0f);
		}
		const Vector2D mouse=GetMousePointVector2D();
		const Vector2D lastpoint=m_pointVec[size-1]+adjust;
		DrawLineAA(lastpoint.x,lastpoint.y,mouse.x,mouse.y,lineColor,1.0f);
		//�n�_�ɂ�capacity�̑傫���̉~��
		const Vector2D beginpoint=m_pointVec[0]+adjust;
		DrawCircleAA(beginpoint.x,beginpoint.y,pointCapacity,20,lineColor,TRUE);
	}
	//�ݒ�����ɖ߂�
	SetDrawBlendMode(mode,pal);
}
