#include"DxLib.h"
#include"SystemMenu.h"
#include"input.h"
#include"GraphicControl.h"
#include"GeneralPurposeResource.h"
#include"CommonConstParameter.h"
#include"FilePath.h"

//-------------------SystemMenu-----------------
const int SystemMenu::contentX[SystemMenu::indexCount]={720,1200};
const int SystemMenu::contentY[SystemMenu::indexCount]={630,630};
const int SystemMenu::contentWidth=400;
const int SystemMenu::contentHeight=100;
const int SystemMenu::sentenceX=CommonConstParameter::gameResolutionX/2;
const int SystemMenu::sentenceY=CommonConstParameter::gameResolutionY/2-90;
const std::string SystemMenu::contentStr[SystemMenu::indexCount]={"�Â���I","������߂�"};

SystemMenu::SystemMenu(std::shared_ptr<BattleSceneData> battleSceneData)
	:BattleSceneElement(SceneKind::e_systemMenu)
	,m_battleSceneData(battleSceneData)
	,m_index(0)
	,m_systemBoard(LoadGraphEX(FilePath::graphicDir+"systemBoard.png"))
	,m_contentFont(CreateFontToHandleEX("���C���I",48,7,DX_FONTTYPE_ANTIALIASING_EDGE_4X4,-1,3))
{}

SystemMenu::~SystemMenu(){
	DeleteGraphEX(m_systemBoard);
	DeleteFontToHandleEX(m_contentFont);
}

int SystemMenu::thisCalculate(){
	//�}�E�X�̈ʒu���擾
	const Vector2D mouse=GetMousePointVector2D();

	//�I�����ڂ̍X�V
	bool inRectPushMoment=false;//������ō��N���b�N�������ǂ����̔���B�����ł���Ă����ق����������Ȍ�
	const size_t beforeIndex=m_index;
	if(keyboard_get(KEY_INPUT_LEFT)==1 || keyboard_get(KEY_INPUT_UP)==1){
		//�����L�[�łP�O��
		m_index=(m_index+indexCount-1)%indexCount;
	} else if(keyboard_get(KEY_INPUT_RIGHT)==1 || keyboard_get(KEY_INPUT_DOWN)==1){
		//�����L�[�łP���
		m_index=(m_index+1)%indexCount;
	} else{
		//�}�E�X�ɂ��X�V����
		const int mouseX=(int)mouse.x,mouseY=(int)mouse.y;
		for(size_t i=0;i<indexCount;i++){
			const int dx=mouseX-contentX[i],dy=mouseY-contentY[i];
			if(dx>-contentWidth/2 && dx<contentWidth/2 && dy>-contentHeight/2 && dy<contentHeight/2){
				//���͂�\�����钷���`���ɓ����Ă��邩�ǂ���
				inRectPushMoment=(mouse_get(MOUSE_INPUT_LEFT)==1);//������ō��N���b�N������
				if((mouse-m_beforeFrameMousePos).sqSize()>=25.0f){
					//�}�E�X��������x�������Ă���Ȃ�A�}�E�X�ɂ��X�V���������
					m_index=i;
					break;
				}
			}
		}
	}
	if(m_index!=beforeIndex){
		//���ڂ��ς���Ă����特��炷
		PlaySoundMem(GeneralPurposeResource::selectSound,DX_PLAYTYPE_BACK,TRUE);
	}

	//��ʑJ��
	bool backFlag=false;
	if(keyboard_get(KEY_INPUT_Z)==1 || inRectPushMoment){
		//���ڂ����肵���ꍇ
		switch(m_index){
		case(0):
			//������
			backFlag=true;
			break;
		case(1):
			//�M�u�A�b�v����
			PlaySoundMem(GeneralPurposeResource::decideSound,DX_PLAYTYPE_BACK,TRUE);
			return SceneKind::END;
			break;
		}
	}
	if(backFlag 
		|| keyboard_get(KEY_INPUT_SPACE)==1 
		|| keyboard_get(KEY_INPUT_X)==1
		|| mouse_get(MOUSE_INPUT_RIGHT)==1
		)
	{
		//���ڌ���ɂ���đO�ɖ߂邩�A�߂�{�^������������
		PlaySoundMem(GeneralPurposeResource::decideSound,DX_PLAYTYPE_BACK,TRUE);
		return 0;
	}

	//�O�t���[���̃}�E�X�̈ʒu���X�V
	m_beforeFrameMousePos=mouse;

	return SceneKind::e_systemMenu;
}

void SystemMenu::thisDraw()const{
	//�o�g����ʂ��Â��\��
	m_battleSceneData->DrawField();//�t�B�[���h�̕`��
	m_battleSceneData->DrawUnit(true);//���j�b�g�̕`��
	m_battleSceneData->DrawHPGage();//�S���j�b�g��HP�Q�[�W�̕`��
	m_battleSceneData->DrawOrder();//���j�b�g�̃I�[�_�[���Ԃ�`��
	int mode,pal;
	GetDrawBlendMode(&mode,&pal);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA,128);
	DrawBox(0,0,CommonConstParameter::gameResolutionX,CommonConstParameter::gameResolutionY,GetColor(0,0,0),TRUE);
	SetDrawBlendMode(mode,pal);
	//�V�X�e�����j���[�`��
	DrawRotaGraph(CommonConstParameter::gameResolutionX/2,CommonConstParameter::gameResolutionY/2,1.0,0.0,m_systemBoard,TRUE);
	DrawStringCenterBaseToHandle(sentenceX,sentenceY,"�o�g�����Â���H",GetColor(255,255,255),m_contentFont,true,GetColor(0,0,0));
	for(size_t i=0;i<indexCount;i++){
		unsigned int inColor,edgeColor;
		if(i==m_index){
			inColor=GetColor(255,255,255);
			edgeColor=GetColor(0,0,0);
		} else{
			inColor=GetColor(128,128,128);
			edgeColor=GetColor(0,0,0);
		}
		DrawStringCenterBaseToHandle(contentX[i],contentY[i],contentStr[i].c_str(),inColor,m_contentFont,true,edgeColor);
	}
}

int SystemMenu::UpdateNextScene(int index){
	//���ɉ������Ȃ�
	return index;
}

void SystemMenu::ReturnProcess(){
	//���ɉ������Ȃ�
}

