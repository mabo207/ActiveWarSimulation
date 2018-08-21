#define _USE_MATH_DEFINES
#include"TitleScene.h"
#include"DxLib.h"
#include"GraphicControl.h"
#include"MyPolygon.h"
#include"input.h"
#include<cmath>
#include"StageSelectScene.h"
#include"BattleScene.h"

//-------------------TitleScene-------------------
std::string TitleScene::SelectItem::GetString(const Kind kind){
	switch(kind){
	case(e_stageSelect):
		return "STAGE SELECT";
	case(e_gameFinish):
		return "EXIT GAME";
	}
	return "";
}

std::shared_ptr<Shape> TitleScene::MakeHexagon(const Vector2D center,const float size)const{
	const size_t vecNum=6;
	Vector2D vec[vecNum];
	for(size_t i=0;i<vecNum;i++){
		vec[i]=Vector2D(size,0.0f).turn((float)(M_PI*2*i/vecNum+M_PI/6));
	}
	const Vector2D begin=center+vec[0];
	std::vector<Vector2D> point(vecNum-1,Vector2D());
	for(size_t i=0;i+1<vecNum;i++){
		point[i]=center+vec[i+1];
	}
	return std::shared_ptr<Shape>(new MyPolygon(begin,point,Shape::Fix::e_static));
}

const Vector2D TitleScene::strPos[TitleScene::SelectItem::COUNTER]={Vector2D(1440.0f,630.0f),Vector2D(1545.0f,820.0f)};

TitleScene::TitleScene()
	:MainControledGameScene()
	,m_backPic(LoadGraphEX("Graphic/nonfree/titleScene.png"))
	,m_titleLogo(LoadGraphEX("Graphic/titleLogo.png"))
	,m_itemFont(CreateFontToHandleEX("���C���I",24,1,-1))
	,m_mousePosJustBefore(GetMousePointVector2D())
	,m_selectItem(SelectItem::e_stageSelect)
	,m_flame(0)
	,m_nextScene(nullptr)
	,m_reqInfo(nullptr)
{
	//�����蔻��}�`�̗p��
	m_hitJudgeShapeVec[0]=MakeHexagon(strPos[0],120.0f);
	m_hitJudgeShapeVec[1]=MakeHexagon(strPos[1],120.0f);
}

TitleScene::~TitleScene(){
	DeleteGraphEX(m_backPic);
	DeleteGraphEX(m_titleLogo);
	DeleteFontToHandleEX(m_itemFont);
}

int TitleScene::thisCalculate(){
	//�t���[�����̍X�V
	m_flame++;

	//����ȓ��͏��̎擾
	const Vector2D mousePos=GetMousePointVector2D();//�}�E�X�ʒu
	const Vector2D stickInput=analogjoypad_get(DX_INPUT_PAD1);//�A�i���O�X�e�B�b�N�̌X��

	//�I�����ڂ̍X�V
	if((m_mousePosJustBefore-mousePos).sqSize()>4.0f){
		//�}�E�X���傫����������A�}�E�X�̈ʒu�����ƂɍX�V
		for(size_t i=0;i<SelectItem::COUNTER;i++){
			if(m_hitJudgeShapeVec[i]->VJudgePointInsideShape(mousePos)){
				//�}�`�S�Ăɓ_�̓���������s���A�����ɂ������}�`�ɑΉ����鍀�ڂɕύX����B
				//�ǂ̐}�`�ɂ��Ή����Ȃ�������ύX�͂��Ȃ�
				m_selectItem=static_cast<SelectItem::Kind>(i);
				break;
			}
		}
	}
/*	else if(stickInput!=Vector2D()){
		//�A�i���O�X�e�B�b�N�𓮂����Ă���΁A���̌��������ƂɍX�V(��߂��A�s���肷����)
		Vector2D backVec,nextVec;//���ڂ����ǂ�E�����ޓ��͕���
		if(m_selectItem==0){
			//�擪���ڂ̎��́A�擪���ڂƏI�[���ڂ����ԃx�N�g���̋t������backVec�Ƃ���
			backVec=-(m_hitJudgeShapeVec[(m_selectItem+SelectItem::COUNTER-1)%SelectItem::COUNTER]->GetPosition()-m_hitJudgeShapeVec[m_selectItem]->GetPosition());
		} else{
			backVec=m_hitJudgeShapeVec[(m_selectItem+SelectItem::COUNTER-1)%SelectItem::COUNTER]->GetPosition()-m_hitJudgeShapeVec[m_selectItem]->GetPosition();
		}
		if(m_selectItem+1==SelectItem::COUNTER){
			//�I�[���ڂ̎��́A�I�[���ڂƐ�[���ڂ����ԃx�N�g���̋t������nextVec�Ƃ���
			nextVec=-(m_hitJudgeShapeVec[(m_selectItem+1)%SelectItem::COUNTER]->GetPosition()-m_hitJudgeShapeVec[m_selectItem]->GetPosition());
		} else{
			nextVec=m_hitJudgeShapeVec[(m_selectItem+1)%SelectItem::COUNTER]->GetPosition()-m_hitJudgeShapeVec[m_selectItem]->GetPosition();
		}
		//���͕�����backVec,nextVec�ɋ߂��������ǂ����œ��͂����߂�
		//���e����60�x
		if(stickInput.dot(backVec)/stickInput.size()/backVec.size()>(float)std::cos(M_PI/3)){
			m_selectItem=static_cast<SelectItem::Kind>((m_selectItem+SelectItem::COUNTER-1)%SelectItem::COUNTER);
		} else if(stickInput.dot(nextVec)/stickInput.size()/nextVec.size()>(float)std::cos(M_PI/3)){
			m_selectItem=static_cast<SelectItem::Kind>((m_selectItem+1)%SelectItem::COUNTER);
		}
	}
//*/
	else{
		//�����łȂ��ꍇ�́A�L�[���͂ōs��
		if(keyboard_get(KEY_INPUT_UP)==1 || keyboard_get(KEY_INPUT_LEFT)==1){
			m_selectItem=static_cast<SelectItem::Kind>((m_selectItem+SelectItem::COUNTER-1)%SelectItem::COUNTER);
		} else if(keyboard_get(KEY_INPUT_DOWN)==1 || keyboard_get(KEY_INPUT_RIGHT)==1){
			m_selectItem=static_cast<SelectItem::Kind>((m_selectItem+1)%SelectItem::COUNTER);
		}
	}

	//�J�ړ��͏���
	if(keyboard_get(KEY_INPUT_Z)==1
		|| (mouse_get(MOUSE_INPUT_LEFT)==1 && m_hitJudgeShapeVec[m_selectItem]->VJudgePointInsideShape(mousePos))
		)
	{
		//����L�[���͂��A�{�^���̏�ō��N���b�N
		return m_selectItem;
	}

	//�}�E�X�ʒu�̍X�V
	m_mousePosJustBefore=mousePos;

	return SelectItem::COUNTER;//SelectItem::COUNTER�͌���ێ�
}

void TitleScene::thisDraw()const{
	//�w�i�̕`��
	DrawGraph(0,0,m_backPic,TRUE);
	//�^�C�g�����S�̕`��
	DrawGraph(0,0,m_titleLogo,TRUE);
	int verX,verY;
	GetGraphSize(m_titleLogo,&verX,&verY);
	DrawStringToHandle(verX,verY,"- C94 Trial Edition -",GetColor(255,255,255),m_itemFont);
	//���ڂ̕`��
	for(size_t i=0;i<SelectItem::COUNTER;i++){
		unsigned int inColor,frameColor,fontColor;
		int strDy=0;
		if(i!=m_selectItem){
			inColor=GetColor(0,128,200);
			frameColor=GetColor(0,64,128);
			fontColor=GetColor(0,0,0);
		} else{
			inColor=GetColor(0,64,128);
			frameColor=GetColor(0,128,200);
			fontColor=GetColor(255,255,255);
			strDy=std::abs((int)(std::cos(M_PI*2*m_flame/120)*5.0));
		}
		m_hitJudgeShapeVec[i]->Draw(Vector2D(),inColor,TRUE);
		m_hitJudgeShapeVec[i]->Draw(Vector2D(),frameColor,FALSE,3.0f);
		DrawStringCenterBaseToHandle((int)strPos[i].x,(int)strPos[i].y+strDy,SelectItem::GetString(static_cast<SelectItem::Kind>(i)).c_str(),fontColor,m_itemFont,true);
	}
}

int TitleScene::Calculate(){
	int index;
	if(m_nextScene.get()==nullptr){
		//���w���Ă����Ԃ��^�C�g����ʂł��鎞
		index=thisCalculate();//�X�V����
		//�J�ڏ���
		switch(index){
		case(SelectItem::e_gameFinish):
			//�O�̃N���X�ɂ��ǂ�B
			return 1;
		case(SelectItem::e_stageSelect):
			//�X�e�[�W�Z���N�g��ʂ�
			m_nextScene=std::shared_ptr<GameScene>(new StageSelectScene(&m_reqInfo));
			break;
		case(SelectItem::COUNTER):
			//����ێ�
			break;
		default:
			break;
		}
	} else{
		//����ȊO�̉�ʂ̏�Ԃł��鎞
		index=m_nextScene->Calculate();//�X�V����
		//�J�ڏ���
		switch(index){
		case(-1):
			//�O�̃N���X���狭���I������Ɠ`����Ă���
			return -1;
		case(-2):
			//�O�̃N���X���炱�̃N���X�ɖ߂�Ɠ`����Ă���
			m_nextScene=std::shared_ptr<GameScene>(nullptr);
			m_reqInfo=std::shared_ptr<MainControledGameScene::RequiredInfoToMakeClass>(nullptr);//���̃N���X���ǂ����邩�Ƃ������������Ă���
			break;
		}
	}
	return 0;
}

void TitleScene::Draw()const{
	if(m_nextScene.get()==nullptr){
		//���̃N���X�̏���
		thisDraw();
	} else{
		//���N���X�̏���
		m_nextScene->Draw();
	}
}

std::shared_ptr<MainControledGameScene> TitleScene::VGetNextMainControledScene()const{
	if(m_reqInfo.get()==nullptr){
		//���̃N���X�����Ȃ��ꍇ��nullptr��Ԃ�
	} else{
		//��񂪂���΁A���͍���
		if(m_reqInfo->GetKind()==RequiredInfoToMakeClass::e_battleScene){
			const BattleScene::RequiredInfoToMakeBattleScene *info=dynamic_cast<const BattleScene::RequiredInfoToMakeBattleScene *>(m_reqInfo.get());
			if(info!=nullptr){
				return std::shared_ptr<MainControledGameScene>(new BattleScene(info->m_stagename.c_str()));
			}
		}
	}
	return std::shared_ptr<MainControledGameScene>(nullptr);
}
