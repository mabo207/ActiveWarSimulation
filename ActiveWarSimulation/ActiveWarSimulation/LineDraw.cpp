#include"LineDraw.h"
#include"DxLib.h"
#include"input.h"
#include"ToolsLib.h"

namespace {
	const float acceptableDistance=5.0f;
	const float sqAcceptableDistance=acceptableDistance*acceptableDistance;
}

//---------------ReflectionWork::LineDraw--------------
ReflectionWork::LineDraw::LineDraw(const std::vector<Edge> &edgeList)
	:Base(Base::Kind::e_lineDraw)
	,m_inputing(false)
	,m_inputIndex(0)//�e�L�g�[�ŗǂ�
	,m_inputStart(true)//�e�L�g�[�ŗǂ�
{
	//m_lineList�̏�����
	const size_t listSize=edgeList.size();
	m_lineList.reserve(listSize);
	for(size_t i=0;i<listSize;i++){
		m_lineList.push_back(edgeList[i]);
	}
}

ReflectionWork::LineDraw::~LineDraw(){}

void ReflectionWork::LineDraw::Update(){
	const Vector2D mouse=GetMousePointVector2D();
	const int left=mouse_get(MOUSE_INPUT_LEFT);
	if(m_inputing){
		//���炩�̐�����͒�
		//�}�E�X�𗣂������̔���
		if(left==0){
			//�ڕW�_�̌v�Z
			Vector2D target;
			if(m_inputStart){
				//start�n�_�ł���ΖڕW�_��end
				target=m_lineList[m_inputIndex].edge.GetEndPoint();
			} else{
				//end�n�_�ł���ΖڕW�_��start
				target=m_lineList[m_inputIndex].edge.GetBeginPoint();
			}
			//�����ƈ��������̔����m_lineList�̍X�V
			if((target-mouse).sqSize()<=sqAcceptableDistance){
				m_lineList[m_inputIndex].drawInfo=true;
			}
			//m_inputing�̍X�V
			m_inputing=false;
		} else{
			//�܂����͒�(�������Ȃ�)
		}
	} else{
		//����T���Ă���
		if(left==1){
			//�N���b�N����
			m_inputIndex=0;//������
			for(const size_t listSize=m_lineList.size();m_inputIndex<listSize;m_inputIndex++){
				//�Y���������T��
				if((m_lineList[m_inputIndex].edge.GetBeginPoint()-mouse).sqSize()<=sqAcceptableDistance){
					//�n�_����͂���
					m_inputing=true;
					m_inputStart=true;
					break;
				} else if((m_lineList[m_inputIndex].edge.GetEndPoint()-mouse).sqSize()<=sqAcceptableDistance){
					//�I�_����͂���
					m_inputing=true;
					m_inputStart=false;
					break;
				}
			}
		}
	}
}

bool ReflectionWork::LineDraw::WorkClear()const{
	//m_lineList[i].drawInfo���S��true�ł��鎞����true��Ԃ�
	for(const LineDrawInfo &line:m_lineList){
		if(!line.drawInfo){
			return false;
		}
	}
	return true;
}

void ReflectionWork::LineDraw::WorkDraw()const{
	for(const LineDrawInfo &info:m_lineList){
		if(info.drawInfo){
			//���ɓ��͍ς݂̒����͎�����
			info.edge.Shape::Draw(Vector2D(),GetColor(255,128,128),TRUE,5.0f);
		} else{
			//�܂����͂��Ă��Ȃ������͔j����
			info.edge.BrokenDraw(Vector2D(),GetColor(255,128,128),5.0f,8.0f,8.0f);
		}
	}
}
