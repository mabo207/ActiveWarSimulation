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
#include"EditUnitParameter.h"

#include"CircleFactory.h"
#include"EdgeFactory.h"
#include"PolygonFactory.h"
#include"UnitFactory.h"

#include"ConstPosSet.h"

#include"CommonConstParameter.h"

#include"ScrollBar.h"

//�萔�̒�`
namespace {
	//�}�b�v�̕\�������̑傫��
	const int mapSizeX = CommonConstParameter::mapSizeX;
	const int mapSizeY = CommonConstParameter::mapSizeY;
	//�}�b�v�̍���̍��W
	const int leftUpPosX = 25;
	const int leftUpPosY = 25;
	//�u����v�{�^���̏c���̌�
	const size_t actButtonWidthNum=2;
	const size_t actButtonHeightNum=3;
	//�u����v�{�^�������S�̂ł̉���,�c��
	const int buttonWidth = 400;
	const int buttonHeight=300;
	//�u�}�`�ݒ�v�{�^���̏c���̌�
	const size_t shapeButtonWidthNum=3;
	const size_t shapeButtonHeightNum=3;
	//�u�}�`�ݒ�v�{�^�������S�̂ł̉���,�c��
	const int shapeButtonWidth=buttonWidth;
	const int shapeButtonHeight=240;
	//�u�ʒu�ݒ�v�{�^���̏c���̌�
	const int posButtonWidthNum=3;
	const int posButtonHeightNum=1;
	//�u�ʒu�ݒ�v�{�^�������S�̂ł̉���,�c��
	const int posButtonWidth=buttonWidth;
	const int posButtonHeight=100;
	//�G�f�B�^�ō���镨�̃T�C�Y�̊�̑傫���E��{�P��
	const int baseSize=CommonConstParameter::unitCircleSize;
}

//�֐���`
//�ÓI�֐�
int StageEditor::GetEditorSizeX(){
	return leftUpPosX*2+mapSizeX+buttonWidth;
}

int StageEditor::GetEditorSizeY(){
	return leftUpPosY*2+mapSizeY;
}

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
	const float buttonsLeftEdge=(float)(leftUpPosX*2+mapSizeX);
	float buttonY=0.0f;
	m_buttons.push_back(std::shared_ptr<ButtonHaving::Button>(new EditPut::EditPutButton(
		Vector2D(buttonsLeftEdge+(float)(buttonWidth/actButtonWidthNum*0),buttonY)
		,Vector2D((float)(buttonWidth/actButtonWidthNum),(float)(buttonHeight/actButtonHeightNum))
	)));
	pPutButton=m_buttons.back();//�ŏ����牟����Ă���悤�ɂ���{�^��
	//remove�{�^��
	m_buttons.push_back(std::shared_ptr<ButtonHaving::Button>(new EditRemove::EditRemoveButton(
		Vector2D(buttonsLeftEdge+(float)(buttonWidth/actButtonWidthNum*1),buttonY)
		,Vector2D((float)(buttonWidth/actButtonWidthNum),(float)(buttonHeight/actButtonHeightNum))
	)));
	buttonY+=(float)(buttonHeight/actButtonHeightNum*1);
	//move�{�^��
	m_buttons.push_back(std::shared_ptr<ButtonHaving::Button>(new EditMove::EditMoveButton(
		Vector2D(buttonsLeftEdge+(float)(buttonWidth/actButtonWidthNum*0),buttonY)
		,Vector2D((float)(buttonWidth/actButtonWidthNum),(float)(buttonHeight/actButtonHeightNum))
	)));
	//resize�{�^��
	m_buttons.push_back(std::shared_ptr<ButtonHaving::Button>(new EditResize::EditResizeButton(
		Vector2D(buttonsLeftEdge+(float)(buttonWidth/actButtonWidthNum*1),buttonY)
		,Vector2D((float)(buttonWidth/actButtonWidthNum),(float)(buttonHeight/actButtonHeightNum))
	)));
	buttonY+=(float)(buttonHeight/actButtonHeightNum*1);
	//UnitEdit�{�^��
	m_buttons.push_back(std::shared_ptr<ButtonHaving::Button>(new EditUnitParameter::EditUnitParameterButton(
		Vector2D(buttonsLeftEdge+(float)(buttonWidth/actButtonWidthNum*0),buttonY)
		,Vector2D((float)(buttonWidth/actButtonWidthNum),(float)(buttonHeight/actButtonHeightNum))
	)));
	buttonY+=(float)(buttonHeight/actButtonHeightNum*1);

	//CircleFactory�{�^��
	m_buttons.push_back(std::shared_ptr<ButtonHaving::Button>(new CircleFactory::CircleFactoryButton(
		Vector2D(buttonsLeftEdge+(float)(shapeButtonWidth/shapeButtonWidthNum*0),buttonY)
		,Vector2D((float)(shapeButtonWidth/shapeButtonWidthNum),(float)(shapeButtonHeight/shapeButtonHeightNum))
	)));
	pRectangleFactoryButton=m_buttons.back();//�ŏ����牟����Ă���悤�ɂ���
	//EdgeFactory�{�^��
	m_buttons.push_back(std::shared_ptr<ButtonHaving::Button>(new EdgeFactory::EdgeFactoryButton(
		Vector2D(buttonsLeftEdge+(float)(shapeButtonWidth/shapeButtonWidthNum*1),buttonY)
		,Vector2D((float)(shapeButtonWidth/shapeButtonWidthNum),(float)(shapeButtonHeight/shapeButtonHeightNum))
	)));
	//PolygonFactory�{�^��
	m_buttons.push_back(std::shared_ptr<ButtonHaving::Button>(new PolygonFactory::PolygonFactoryButton(
		Vector2D(buttonsLeftEdge+(float)(shapeButtonWidth/shapeButtonWidthNum*2),buttonY)
		,Vector2D((float)(shapeButtonWidth/shapeButtonWidthNum),(float)(shapeButtonHeight/shapeButtonHeightNum))
	)));
	buttonY+=+(float)(shapeButtonHeight/shapeButtonHeightNum*1);
	//��m�쐬�{�^��
	m_buttons.push_back(std::shared_ptr<ButtonHaving::Button>(new UnitFactory::UnitFactoryButton(
		Vector2D(buttonsLeftEdge+(float)(shapeButtonWidth/shapeButtonWidthNum*0),buttonY)
		,Vector2D((float)(shapeButtonWidth/shapeButtonWidthNum),(float)(shapeButtonHeight/shapeButtonHeightNum))
		,Unit::Profession::e_soldier
	)));
	//�d�����쐬�{�^��
	m_buttons.push_back(std::shared_ptr<ButtonHaving::Button>(new UnitFactory::UnitFactoryButton(
		Vector2D(buttonsLeftEdge+(float)(shapeButtonWidth/shapeButtonWidthNum*1),buttonY)
		,Vector2D((float)(shapeButtonWidth/shapeButtonWidthNum),(float)(shapeButtonHeight/shapeButtonHeightNum))
		,Unit::Profession::e_armer
	)));
	//�ˎ�쐬�{�^��
	m_buttons.push_back(std::shared_ptr<ButtonHaving::Button>(new UnitFactory::UnitFactoryButton(
		Vector2D(buttonsLeftEdge+(float)(shapeButtonWidth/shapeButtonWidthNum*2),buttonY)
		,Vector2D((float)(shapeButtonWidth/shapeButtonWidthNum),(float)(shapeButtonHeight/shapeButtonHeightNum))
		,Unit::Profession::e_archer
	)));
	buttonY+=+(float)(shapeButtonHeight/shapeButtonHeightNum*1);
	//�����m�쐬�{�^��
	m_buttons.push_back(std::shared_ptr<ButtonHaving::Button>(new UnitFactory::UnitFactoryButton(
		Vector2D(buttonsLeftEdge+(float)(shapeButtonWidth/shapeButtonWidthNum*0),buttonY)
		,Vector2D((float)(shapeButtonWidth/shapeButtonWidthNum),(float)(shapeButtonHeight/shapeButtonHeightNum))
		,Unit::Profession::e_mage
	)));
	//�q�����쐬�{�^��
	m_buttons.push_back(std::shared_ptr<ButtonHaving::Button>(new UnitFactory::UnitFactoryButton(
		Vector2D(buttonsLeftEdge+(float)(shapeButtonWidth/shapeButtonWidthNum*1),buttonY)
		,Vector2D((float)(shapeButtonWidth/shapeButtonWidthNum),(float)(shapeButtonHeight/shapeButtonHeightNum))
		,Unit::Profession::e_healer
	)));
	buttonY+=+(float)(shapeButtonHeight/shapeButtonHeightNum*1);

	//1px�ʒu�����{�^��
	m_buttons.push_back(std::shared_ptr<ButtonHaving::Button>(new ConstPosSet::ConstPosSetButton(
		Vector2D(buttonsLeftEdge+(float)(posButtonWidth/posButtonWidthNum*0),buttonY)
		,Vector2D((float)(posButtonWidth/posButtonWidthNum),(float)(posButtonHeight/posButtonHeightNum))
		,1
	)));
	pPosSettingButton=m_buttons.back();

	//45px�ʒu�����{�^��
	m_buttons.push_back(std::shared_ptr<ButtonHaving::Button>(new ConstPosSet::ConstPosSetButton(
		Vector2D(buttonsLeftEdge+(float)(posButtonWidth/posButtonWidthNum*1),buttonY)
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
	m_actionSettings.InitObjects();
	m_actionSettings.ReadStage("Savedata/stage.txt");
	m_actionSettings.ReadUnit("Savedata/unitlist.txt");
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
		m_actionSettings.WriteOutUnit("SaveData/unitlist.txt");
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
	printfDx("(%f,%f)\n",v.x,v.y);//�ʒu������̍��W
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

	//�ҏW�Ώۂ�Unit�ł���΁A���̔\�͒l���`��
	if(m_actionSettings.m_pBattleObject && m_actionSettings.m_pBattleObject->GetType()==BattleObject::Type::e_unit){
		const std::shared_ptr<const Unit> punit=std::dynamic_pointer_cast<const Unit>(m_actionSettings.m_pBattleObject);
		if(punit){
			const Vector2D unitDrawPos=punit->getPos()+adjust;
			const int unitX=(int)unitDrawPos.x,unitY=(int)unitDrawPos.y;
			const int left=unitX+baseSize+20;
			int top=unitY-leftUpPosY;
			const int boxWidth=300;
			const int boxHeight=350;
			if(top<0){
				top=0;
			} else if(top+boxHeight>GetEditorSizeY()){
				top=GetEditorSizeY()-boxHeight;
			}
			const int mergin=5;
			const unsigned int backColor=GetColor(0,128,64);
			const unsigned int strColor=GetColor(255,255,255);
			//���n�̕`��
			DrawTriangle(unitX+baseSize,unitY,left,unitY-10,left,unitY+10,backColor,TRUE);
			DrawBox(left,top,left+boxWidth,top+boxHeight,backColor,TRUE);
			//�\�͒l�̕`��
			std::string drawStr;
			drawStr.reserve(300);
			drawStr+="Lv: "+std::to_string(punit->GetBaseStatus().lv)+'\n';
			drawStr+="Team: "+std::to_string(punit->GetBattleStatus().team)+'\n';
			drawStr+="Profession: "+std::to_string(punit->GetBaseStatus().profession)+'\n';
			drawStr+="AItype: "+std::to_string(punit->GetBattleStatus().aitype)+'\n';
			drawStr+="AIgroup: "+std::to_string(punit->GetBattleStatus().aiGroup)+'\n';
			drawStr+="HP: "+std::to_string(punit->GetBattleStatus().HP)+" / "+std::to_string(punit->GetBaseStatus().maxHP)+'\n';
			//�ȉ��A�ҏW�ł��Ȃ�����
			drawStr+="\nPOW: "+std::to_string(punit->GetBaseStatus().power)+'\n';
			drawStr+="DEF: "+std::to_string(punit->GetBaseStatus().def)+'\n';
			drawStr+="MPOW: "+std::to_string(punit->GetBaseStatus().mpower)+'\n';
			drawStr+="MDEF: "+std::to_string(punit->GetBaseStatus().mdef)+'\n';
			drawStr+="Weapon: "+punit->GetBattleStatus().weapon->GetName()+'\n';
			drawStr+=punit->GetBattleStatus().weapon->GetEffectivenessString(punit.get());
			DrawStringNewLineToHandle(left+mergin,top+mergin,boxWidth-mergin*2,boxHeight-mergin*2,drawStr.c_str(),strColor,m_font,2);
		}
	}
}
