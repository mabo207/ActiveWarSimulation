#define _USE_MATH_DEFINES	//math.h�̒萔���g�����߂ɕK�{

#include<math.h>
#include<fstream>

#include"DxLib.h"
#include"StageEditor.h"
#include"input.h"

#include"Terrain.h"
#include"Circle.h"
#include"Edge.h"

#include"EditPut.h"
#include"EditRemove.h"
#include"EditMove.h"
#include"EditResize.h"

#include"CircleFactory.h"
#include"EdgeFactory.h"
#include"PolygonFactory.h"

#include"ConstPosSet.h"

#include"CommonConstParameter.h"

#include"ScrollBar.h"

//�萔�̒�`
const int StageEditor::mapSizeX = 1920;
const int StageEditor::mapSizeY = 1080;
const int StageEditor::leftUpPosX = 25;
const int StageEditor::leftUpPosY = 25;
const int StageEditor::buttonWidth = 400;
const int StageEditor::buttonHeight=(leftUpPosY*2+mapSizeY)/4;
const int StageEditor::shapeButtonHeightNum=1;
const int StageEditor::shapeButtonWidthNum=3;
const int StageEditor::shapeButtonHeight=StageEditor::buttonHeight/3;
const int StageEditor::shapeButtonWidth=StageEditor::buttonWidth;
const int StageEditor::posButtonWidth=StageEditor::buttonWidth;
const int StageEditor::posButtonHeight=StageEditor::buttonHeight/2;
const int StageEditor::posButtonWidthNum=3;
const int StageEditor::posButtonHeightNum=1;
const std::string StageEditor::actButtonStr[actButtonHeightNum*actButtonWidthNum]={"put","remove","move","expand"};
const int StageEditor::baseSize=CommonConstParameter::unitCircleSize;

//�֐���`
//���I�֐�
StageEditor::StageEditor()
	:m_actionSettings(
		std::shared_ptr<EditAction>(nullptr)
		,std::shared_ptr<BattleObject>(new Terrain(std::shared_ptr<Shape>(new Circle(Vector2D(0.0f,0.0f),20.0f,Shape::Fix::e_static)),-1,GetColor(128,128,128),false))
		,std::shared_ptr<ShapeFactory>(nullptr)
		,std::shared_ptr<PosSetting>(nullptr))
{
	//�{�^���ꗗ
	
	//�ŏ����牟����Ă���悤�ɂ���{�^�������X�g�A�b�v���Ȃ���s��
	std::shared_ptr<ButtonHaving::Button> pPutButton,pRectangleFactoryButton,pPosSettingButton;
	
	//��X�N���[���{�^��
	m_buttons.push_back(std::shared_ptr<ButtonHaving::Button>(new ScrollBar::ScrollButton(
		Vector2D(0,0)
		,Vector2D((float)(leftUpPosX*2+mapSizeX),0)
		,Vector2D((float)(leftUpPosX+mapSizeX),(float)leftUpPosY)
		,Vector2D((float)leftUpPosX,(float)leftUpPosY)
		,Vector2D(0,-1)
	)));
	//���X�N���[���{�^��
	m_buttons.push_back(std::shared_ptr<ButtonHaving::Button>(new ScrollBar::ScrollButton(
		Vector2D(0,0)
		,Vector2D((float)leftUpPosX,(float)leftUpPosY)
		,Vector2D((float)leftUpPosX,(float)(leftUpPosY+mapSizeY))
		,Vector2D(0,(float)(leftUpPosY*2+mapSizeY))
		,Vector2D(-1,0)
	)));
	//�E�X�N���[���{�^��
	m_buttons.push_back(std::shared_ptr<ButtonHaving::Button>(new ScrollBar::ScrollButton(
		Vector2D((float)(leftUpPosX*2+mapSizeX),0)
		,Vector2D((float)(leftUpPosX*2+mapSizeX),(float)(leftUpPosY*2+mapSizeY))
		,Vector2D((float)(leftUpPosX+mapSizeX),(float)(leftUpPosY+mapSizeY))
		,Vector2D((float)(leftUpPosX+mapSizeX),(float)leftUpPosY)
		,Vector2D(1,0)
	)));
	//���X�N���[���{�^��
	m_buttons.push_back(std::shared_ptr<ButtonHaving::Button>(new ScrollBar::ScrollButton(
		Vector2D((float)leftUpPosX,(float)(leftUpPosY+mapSizeY))
		,Vector2D((float)(leftUpPosX+mapSizeX),(float)(leftUpPosY+mapSizeY))
		,Vector2D((float)(leftUpPosX*2+mapSizeX),(float)(leftUpPosY*2+mapSizeY))
		,Vector2D(0,(float)(leftUpPosY*2+mapSizeY))
		,Vector2D(0,1)
	)));

	//put�{�^��
	m_buttons.push_back(std::shared_ptr<ButtonHaving::Button>(new EditPut::EditPutButton(
		Vector2D((float)(leftUpPosX*2+mapSizeX+buttonWidth/actButtonWidthNum*0),0)
		,Vector2D((float)(buttonWidth/actButtonWidthNum),(float)(buttonHeight/actButtonHeightNum))
	)));
	pPutButton=m_buttons.back();//�ŏ����牟����Ă���悤�ɂ���{�^��
	//remove�{�^��
	m_buttons.push_back(std::shared_ptr<ButtonHaving::Button>(new EditRemove::EditRemoveButton(
		Vector2D((float)(leftUpPosX*2+mapSizeX+buttonWidth/actButtonWidthNum*1),0)
		,Vector2D((float)(buttonWidth/actButtonWidthNum),(float)(buttonHeight/actButtonHeightNum))
	)));
	//move�{�^��
	m_buttons.push_back(std::shared_ptr<ButtonHaving::Button>(new EditMove::EditMoveButton(
		Vector2D((float)(leftUpPosX*2+mapSizeX+buttonWidth/actButtonWidthNum*0),(float)(buttonHeight/actButtonHeightNum*1))
		,Vector2D((float)(buttonWidth/actButtonWidthNum),(float)(buttonHeight/actButtonHeightNum))
	)));
	//resize�{�^��
	m_buttons.push_back(std::shared_ptr<ButtonHaving::Button>(new EditResize::EditResizeButton(
		Vector2D((float)(leftUpPosX*2+mapSizeX+buttonWidth/actButtonWidthNum*1),(float)(buttonHeight/actButtonHeightNum*1))
		,Vector2D((float)(buttonWidth/actButtonWidthNum),(float)(buttonHeight/actButtonHeightNum))
	)));

	//CircleFactory�{�^��
	m_buttons.push_back(std::shared_ptr<ButtonHaving::Button>(new CircleFactory::CircleFactoryButton(
		Vector2D(
		(float)(leftUpPosX*2+mapSizeX+shapeButtonWidth/shapeButtonWidthNum*0)
			,(float)(buttonHeight+shapeButtonHeight/shapeButtonHeightNum*0)
		)
		,Vector2D((float)(shapeButtonWidth/shapeButtonWidthNum),(float)(shapeButtonHeight/shapeButtonHeightNum))
	)));
	pRectangleFactoryButton=m_buttons.back();//�ŏ����牟����Ă���悤�ɂ���

	//EdgeFactory�{�^��
	m_buttons.push_back(std::shared_ptr<ButtonHaving::Button>(new EdgeFactory::EdgeFactoryButton(
		Vector2D(
		(float)(leftUpPosX*2+mapSizeX+shapeButtonWidth/shapeButtonWidthNum*1)
			,(float)(buttonHeight+shapeButtonHeight/shapeButtonHeightNum*0)
		)
		,Vector2D((float)(shapeButtonWidth/shapeButtonWidthNum),(float)(shapeButtonHeight/shapeButtonHeightNum))
	)));
	
	//PolygonFactory�{�^��
	m_buttons.push_back(std::shared_ptr<ButtonHaving::Button>(new PolygonFactory::PolygonFactoryButton(
		Vector2D(
		(float)(leftUpPosX*2+mapSizeX+shapeButtonWidth/shapeButtonWidthNum*2)
			,(float)(buttonHeight+shapeButtonHeight/shapeButtonHeightNum*0)
		)
		,Vector2D((float)(shapeButtonWidth/shapeButtonWidthNum),(float)(shapeButtonHeight/shapeButtonHeightNum))
	)));

	//1px�ʒu�����{�^��
	m_buttons.push_back(std::shared_ptr<ButtonHaving::Button>(new ConstPosSet::ConstPosSetButton(
		Vector2D(
		(float)(leftUpPosX*2+mapSizeX+posButtonWidth/posButtonWidthNum*0)
			,(float)(buttonHeight+shapeButtonHeight+posButtonHeight/posButtonHeightNum*0)
		)
		,Vector2D((float)(posButtonWidth/posButtonWidthNum),(float)(posButtonHeight/posButtonHeightNum))
		,1
	)));
	pPosSettingButton=m_buttons.back();

	//45px�ʒu�����{�^��
	m_buttons.push_back(std::shared_ptr<ButtonHaving::Button>(new ConstPosSet::ConstPosSetButton(
		Vector2D(
		(float)(leftUpPosX*2+mapSizeX+posButtonWidth/posButtonWidthNum*1)
			,(float)(buttonHeight+shapeButtonHeight+posButtonHeight/posButtonHeightNum*0)
		)
		,Vector2D((float)(posButtonWidth/posButtonWidthNum),(float)(posButtonHeight/posButtonHeightNum))
		,baseSize
	)));


	//�ŏ����牟����Ă���悤�ɂ���{�^��������(���Ԃɒ��ӁI)
	pRectangleFactoryButton->PushedProcess(m_actionSettings);
	pPutButton->PushedProcess(m_actionSettings);
	pPosSettingButton->PushedProcess(m_actionSettings);
	
	//�t�H���g
	m_font=CreateFontToHandle("���C���I",16,1);

	//�w�i�O���t�B�b�N
	m_mapPic=LoadGraph("Savedata/stage.png");

	//�}�`�̓ǂݍ���
	m_actionSettings.ReadStage("Savedata/stage.txt");
}

StageEditor::~StageEditor() {
	//�t�H���g
	DeleteFontToHandle(m_font);
	//�摜
	DeleteGraph(m_mapPic);
}

//�}�E�X�����N���b�N�������̓���Q
void StageEditor::ProcessMapPush(int mouseX,int mouseY){
	//�Q�[����ʏ�̍��W�ɕϊ�(�c��͕`��␳�lm_adjust�݂̂�K�p����΂悢)
	mouseX-=leftUpPosX;
	mouseY-=leftUpPosY;
	//����
	m_actionSettings.PracticeEdit(Vector2D((float)mouseX,(float)mouseY));
}

//��ɍs���ҏW�s��
void StageEditor::NonPressEdit(int mouseX,int mouseY){
	//�Q�[����ʏ�̍��W�ɕϊ�(�c��͕`��␳�lm_adjust�݂̂�K�p����΂悢)
	mouseX-=leftUpPosX;
	mouseY-=leftUpPosY;
	//����
	m_actionSettings.PracticeNonPressEdit(Vector2D((float)mouseX,(float)mouseY));
}

//�����[�v���암���̊֐�
int StageEditor::Calculate() {
	//�}�E�X�̈ʒu�v�Z
	int mouseX, mouseY;
	GetMousePoint(&mouseX, &mouseY);
	//�}�E�X���͎�t
	NonPressEdit(mouseX,mouseY);

	//�{�^���Q�̓��͔���
	for(std::shared_ptr<ButtonHaving::Button> &pb:m_buttons){
		if(pb.get()->JudgeButtonPushed() && pb.get()->JudgeInButton(GetMousePointVector2D())){
			pb.get()->PushedProcess(m_actionSettings);
			break;
		}
	}

	//���N���b�N�����ꂽ��
	if(mouseX>=leftUpPosX && mouseX<leftUpPosX+mapSizeX && mouseY>=leftUpPosY && mouseY<leftUpPosY+mapSizeY){
		//�}�b�v��ʓ��Ƀ}�E�X������ꍇ
		if(mouse_get(MOUSE_INPUT_LEFT)==1) {
			ProcessMapPush(mouseX,mouseY);
		}
	}

	//�E�N���b�N���ꂽ��
	m_actionSettings.UpdateMouseObjectDepth(mouse_get(MOUSE_INPUT_RIGHT));//�����ĂȂ����ł��X�V���邱�Ƃ�����̂ŁA�����Ă���t���[������n���B

	//�L�[�{�[�h���͎�t
	if(keyboard_get(KEY_INPUT_S)==10){
		//S�L�[�������ŕۑ�
		m_actionSettings.WriteOutStage("SaveData/stage.txt");
	} else if(keyboard_get(KEY_INPUT_R)==10){
		//R�L�[�������œǂݍ���
		m_actionSettings.ReadStage("SaveData/stage.txt");
	} else if(keyboard_get(KEY_INPUT_NUMPADENTER) == 1){
		//Enter�L�[���͂ŃG�f�B�^���I��
		return -1;
	}
	return 0;
}

void StageEditor::Draw() {
	//�f�o�b�O�`��
	clsDx();
	Vector2D v=GetMousePointVector2D();
	//printfDx("(%f,%f)\n",v.x,v.y);//�f�̍��W
	v=m_actionSettings.m_pPosSetting->CalculatePos(v-Vector2D((float)leftUpPosX,(float)leftUpPosY)+m_actionSettings.GetMAdjust(),m_actionSettings);
	//printfDx("(%f,%f)\n",v.x,v.y);//�ʒu������̍��W
	for(auto o:*m_actionSettings.GetPMObject()){
		printfDx("%d\n",o.get());
	}
	//printfDx("m_pBattleObject:%d\n",m_actionSettings.m_pBattleObject.get());

	//�}�b�v�̕`��
	//�}�b�v�`��o����͈͂𐧌�
	SetDrawArea(leftUpPosX,leftUpPosY,leftUpPosX+mapSizeX, leftUpPosY + mapSizeY);
	//�}�b�v�S�̂̕`��
	bool firstflag=true;
	Vector2D mouse=GetMousePointVector2D()-Vector2D((float)leftUpPosX,(float)leftUpPosY)+m_actionSettings.GetMAdjust();//�}�E�X�̈ʒu(�␳�l���l�����}�b�v��̍��W�ŕ\��)
	Vector2D adjust=Vector2D((float)leftUpPosX,(float)leftUpPosY)-m_actionSettings.GetMAdjust();//�`��̑S�̒����ʒu
	DrawGraph((int)adjust.x,(int)adjust.y,m_mapPic,TRUE);
	//���݂̕ҏW�Ώې}�`��`��
	if(m_actionSettings.m_pBattleObject.get()!=nullptr){
		int mode,pal;
		GetDrawBlendMode(&mode,&pal);
		SetDrawBlendMode(DX_BLENDMODE_ALPHA,128);
		m_actionSettings.m_pBattleObject.get()->VDraw(adjust);
		SetDrawBlendMode(mode,pal);
	}
	//�}�b�v�ɑ��݂��Ă�����̂�S�ĕ`��
	const BattleObject *mouseObj=m_actionSettings.GetMousePointedObjectPointer(mouse);
	for (const std::shared_ptr<BattleObject> &obj : *m_actionSettings.GetPMObject()) {
		obj.get()->VDraw(adjust);
		//�}�E�X������Ă���}�`�ɂ͉��F���g��`�悵�t�H�[�J�X��\��
		if(firstflag && obj.get()==mouseObj){
			obj.get()->ShapeDraw(GetColor(255,255,0),FALSE,1.0f,adjust);
			obj.get()->PosDraw(GetColor(255,255,0),TRUE,2.0f,adjust);
			firstflag=false;
		}
		//�I�𒆂̐}�`�ɂ��Ă͐Ԃ��g��`�悵�t�H�[�J�X��\��
		if(obj.get()==m_actionSettings.m_pBattleObject.get()){
			obj.get()->ShapeDraw(GetColor(255,0,0),FALSE,1.0f,adjust);
			obj.get()->PosDraw(GetColor(255,0,0),TRUE,2.0f,adjust);
		}
	}
	
	//�ҏW�̂��߂̃f�[�^��`��B�}�`�`�悪��
	m_actionSettings.m_pEditAction->ActionDraw(adjust,m_actionSettings);
/*	
	//�ҏW�O��BattleObject���}�b�v�ɉ��z�I�ɕ`��
	if(m_actionSettings.GetMPOriginObject()!=nullptr){
		int mode,pal;
		GetDrawBlendMode(&mode,&pal);
		SetDrawBlendMode(DX_BLENDMODE_ALPHA,128);
		m_actionSettings.GetMPOriginObject()->VDraw(adjust);
		SetDrawBlendMode(mode,pal);
	}
//*/

	//AI����̍ۂ̎Q�l�ƂȂ郆�j�b�g�̔��a�Ԋu�̊i�q�_�̕`��
	{
		int init[2]={(int)m_actionSettings.GetMAdjust().x,(int)m_actionSettings.GetMAdjust().y};
		for(int &pal:init){
			//x,y�̕`�揉���ʒu�𒲐�����
			if(pal<0){
				pal=baseSize-((-pal)%baseSize);
			} else{
				pal=pal%baseSize;
			}
			//���̂܂܂��ƁApal�́um_adjust���獶�������pal�������ꂽ�ꏊ�Ɋi�q�_�����݂���v�Ƃ����l�̂܂܂Ȃ̂ŁA�}�b�v��ōł�����ɕ`�悳���ꏊ�������l�ɂ��Ă����B
			pal=baseSize-pal;
		}
		for(int y=init[1]+leftUpPosY;y<leftUpPosY+mapSizeY;y+=baseSize){
			for(int x=init[0]+leftUpPosX;x<leftUpPosX+mapSizeX;x+=baseSize){
				DrawCircle(x,y,2,GetColor(255,64,64),TRUE);
			}
		}
	}

	SetDrawAreaFull();

	//�ʒu�ݒ�K�C�h�̕`��
	m_actionSettings.DrawPosSettingGuide(leftUpPosX,leftUpPosY,mapSizeX,mapSizeY);

	//���͂���Ă��铮��̃{�^���̕`��
	m_actionSettings.DrawEditButtonPushed();

	//���͂���Ă���}�`�ݒ�{�^���̕`��
	m_actionSettings.DrawShapeFactoryButtonPushed();
	
	//���͂���Ă���ʒu�ݒ�{�^���̕`��
	m_actionSettings.DrawPosSettingButtonPushed();
	
	//�{�^���Q�̕`��
	for(std::shared_ptr<ButtonHaving::Button> &pb:m_buttons){
		pb.get()->ButtonDraw(m_font,TRUE);
	}


}
