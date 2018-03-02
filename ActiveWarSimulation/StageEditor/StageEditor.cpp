#define _USE_MATH_DEFINES	//math.hの定数を使うために必須

#include<math.h>

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

#include"ConstPosSet.h"

#include"ScrollBar.h"

//定数の定義
const int StageEditor::mapSizeX = 800;
const int StageEditor::mapSizeY = 600;
const int StageEditor::leftUpPosX = 25;
const int StageEditor::leftUpPosY = 25;
const int StageEditor::buttonWidth = 400;
const int StageEditor::buttonHeight=(leftUpPosY*2+mapSizeY)/4;
const int StageEditor::shapeButtonHeightNum=1;
const int StageEditor::shapeButtonWidthNum=2;
const int StageEditor::shapeButtonHeight=StageEditor::buttonHeight/2;
const int StageEditor::shapeButtonWidth=StageEditor::buttonWidth;
const int StageEditor::posButtonWidth=StageEditor::buttonWidth;
const int StageEditor::posButtonHeight=StageEditor::buttonHeight/2;
const int StageEditor::posButtonWidthNum=3;
const int StageEditor::posButtonHeightNum=1;
const std::string StageEditor::actButtonStr[actButtonHeightNum*actButtonWidthNum]={"put","remove","move","expand"};
const int StageEditor::baseSize=32;

//関数定義
//動的関数
StageEditor::StageEditor()
	:m_actionSettings(
		std::shared_ptr<EditAction>(nullptr)
		,std::shared_ptr<BattleObject>(new Terrain(std::shared_ptr<Shape>(new Circle(Vector2D(0.0f,0.0f),20.0f,Shape::Fix::e_static)),-1,0,GetColor(128,128,128),false))
		,std::shared_ptr<ShapeFactory>(nullptr)
		,std::shared_ptr<PosSetting>(nullptr))
{
	//ボタン一覧
	
	//最初から押されているようにするボタンをリストアップしながら行う
	std::shared_ptr<ButtonHaving::Button> pPutButton,pRectangleFactoryButton,pPosSettingButton;
	
	//上スクロールボタン
	m_buttons.push_back(std::shared_ptr<ButtonHaving::Button>(new ScrollBar::ScrollButton(
		Vector2D(0,0)
		,Vector2D((float)(leftUpPosX*2+mapSizeX),0)
		,Vector2D((float)(leftUpPosX+mapSizeX),(float)leftUpPosY)
		,Vector2D((float)leftUpPosX,(float)leftUpPosY)
		,Vector2D(0,-1)
	)));
	//左スクロールボタン
	m_buttons.push_back(std::shared_ptr<ButtonHaving::Button>(new ScrollBar::ScrollButton(
		Vector2D(0,0)
		,Vector2D((float)leftUpPosX,(float)leftUpPosY)
		,Vector2D((float)leftUpPosX,(float)(leftUpPosY+mapSizeY))
		,Vector2D(0,(float)(leftUpPosY*2+mapSizeY))
		,Vector2D(-1,0)
	)));
	//右スクロールボタン
	m_buttons.push_back(std::shared_ptr<ButtonHaving::Button>(new ScrollBar::ScrollButton(
		Vector2D((float)(leftUpPosX*2+mapSizeX),0)
		,Vector2D((float)(leftUpPosX*2+mapSizeX),(float)(leftUpPosY*2+mapSizeY))
		,Vector2D((float)(leftUpPosX+mapSizeX),(float)(leftUpPosY+mapSizeY))
		,Vector2D((float)(leftUpPosX+mapSizeX),(float)leftUpPosY)
		,Vector2D(1,0)
	)));
	//下スクロールボタン
	m_buttons.push_back(std::shared_ptr<ButtonHaving::Button>(new ScrollBar::ScrollButton(
		Vector2D((float)leftUpPosX,(float)(leftUpPosY+mapSizeY))
		,Vector2D((float)(leftUpPosX+mapSizeX),(float)(leftUpPosY+mapSizeY))
		,Vector2D((float)(leftUpPosX*2+mapSizeX),(float)(leftUpPosY*2+mapSizeY))
		,Vector2D(0,(float)(leftUpPosY*2+mapSizeY))
		,Vector2D(0,1)
	)));

	//putボタン
	m_buttons.push_back(std::shared_ptr<ButtonHaving::Button>(new EditPut::EditPutButton(
		Vector2D((float)(leftUpPosX*2+mapSizeX+buttonWidth/actButtonWidthNum*0),0)
		,Vector2D((float)(buttonWidth/actButtonWidthNum),(float)(buttonHeight/actButtonHeightNum))
	)));
	pPutButton=m_buttons.back();//最初から押されているようにするボタン
	//removeボタン
	m_buttons.push_back(std::shared_ptr<ButtonHaving::Button>(new EditRemove::EditRemoveButton(
		Vector2D((float)(leftUpPosX*2+mapSizeX+buttonWidth/actButtonWidthNum*1),0)
		,Vector2D((float)(buttonWidth/actButtonWidthNum),(float)(buttonHeight/actButtonHeightNum))
	)));
	//moveボタン
	m_buttons.push_back(std::shared_ptr<ButtonHaving::Button>(new EditMove::EditMoveButton(
		Vector2D((float)(leftUpPosX*2+mapSizeX+buttonWidth/actButtonWidthNum*0),(float)(buttonHeight/actButtonHeightNum*1))
		,Vector2D((float)(buttonWidth/actButtonWidthNum),(float)(buttonHeight/actButtonHeightNum))
	)));
	//resizeボタン
	m_buttons.push_back(std::shared_ptr<ButtonHaving::Button>(new EditResize::EditResizeButton(
		Vector2D((float)(leftUpPosX*2+mapSizeX+buttonWidth/actButtonWidthNum*1),(float)(buttonHeight/actButtonHeightNum*1))
		,Vector2D((float)(buttonWidth/actButtonWidthNum),(float)(buttonHeight/actButtonHeightNum))
	)));

	//CircleFactoryボタン
	m_buttons.push_back(std::shared_ptr<ButtonHaving::Button>(new CircleFactory::CircleFactoryButton(
		Vector2D(
		(float)(leftUpPosX*2+mapSizeX+shapeButtonWidth/shapeButtonWidthNum*0)
			,(float)(buttonHeight+shapeButtonHeight/shapeButtonHeightNum*0)
		)
		,Vector2D((float)(shapeButtonWidth/shapeButtonWidthNum),(float)(shapeButtonHeight/shapeButtonHeightNum))
	)));
	pRectangleFactoryButton=m_buttons.back();//最初から押されているようにする

	//EdgeFactoryボタン
	m_buttons.push_back(std::shared_ptr<ButtonHaving::Button>(new EdgeFactory::EdgeFactoryButton(
		Vector2D(
		(float)(leftUpPosX*2+mapSizeX+shapeButtonWidth/shapeButtonWidthNum*1)
			,(float)(buttonHeight+shapeButtonHeight/shapeButtonHeightNum*0)
		)
		,Vector2D((float)(shapeButtonWidth/shapeButtonWidthNum),(float)(shapeButtonHeight/shapeButtonHeightNum))
	)));
	
	//1px位置調整ボタン
	m_buttons.push_back(std::shared_ptr<ButtonHaving::Button>(new ConstPosSet::ConstPosSetButton(
		Vector2D(
		(float)(leftUpPosX*2+mapSizeX+posButtonWidth/posButtonWidthNum*0)
			,(float)(buttonHeight+shapeButtonHeight+posButtonHeight/posButtonHeightNum*0)
		)
		,Vector2D((float)(posButtonWidth/posButtonWidthNum),(float)(posButtonHeight/posButtonHeightNum))
		,1
	)));
	pPosSettingButton=m_buttons.back();

	//32px位置調整ボタン
	m_buttons.push_back(std::shared_ptr<ButtonHaving::Button>(new ConstPosSet::ConstPosSetButton(
		Vector2D(
		(float)(leftUpPosX*2+mapSizeX+posButtonWidth/posButtonWidthNum*1)
			,(float)(buttonHeight+shapeButtonHeight+posButtonHeight/posButtonHeightNum*0)
		)
		,Vector2D((float)(posButtonWidth/posButtonWidthNum),(float)(posButtonHeight/posButtonHeightNum))
		,baseSize
	)));


	//最初から押されているようにするボタンを押す(順番に注意！)
	pRectangleFactoryButton->PushedProcess(m_actionSettings);
	pPutButton->PushedProcess(m_actionSettings);
	pPosSettingButton->PushedProcess(m_actionSettings);
	
	//フォント
	m_font=CreateFontToHandle("メイリオ",16,1);

	
}

StageEditor::~StageEditor() {
	//フォント
	DeleteFontToHandle(m_font);
}

//マウスを左クリックした時の動作群
void StageEditor::ProcessMapPush(int mouseX,int mouseY){
	//ゲーム画面上の座標に変換(残りは描画補正値m_adjustのみを適用すればよい)
	mouseX-=leftUpPosX;
	mouseY-=leftUpPosY;
	//動作
	m_actionSettings.PracticeEdit(Vector2D((float)mouseX,(float)mouseY));
}

//常に行う編集行為
void StageEditor::NonPressEdit(int mouseX,int mouseY){
	//ゲーム画面上の座標に変換(残りは描画補正値m_adjustのみを適用すればよい)
	mouseX-=leftUpPosX;
	mouseY-=leftUpPosY;
	//動作
	m_actionSettings.PracticeNonPressEdit(Vector2D((float)mouseX,(float)mouseY));
}

//毎ループ動作部分の関数
int StageEditor::Calculate() {
	//マウスの位置計算
	int mouseX, mouseY;
	GetMousePoint(&mouseX, &mouseY);
	//マウス入力受付
	NonPressEdit(mouseX,mouseY);

	//ボタン群の入力判定
	for(std::shared_ptr<ButtonHaving::Button> &pb:m_buttons){
		if(pb.get()->JudgeButtonPushed() && pb.get()->JudgeInButton(GetMousePointVector2D())){
			pb.get()->PushedProcess(m_actionSettings);
			break;
		}
	}

	//左クリックをされたら
	if(mouseX>=leftUpPosX && mouseX<leftUpPosX+mapSizeX && mouseY>=leftUpPosY && mouseY<leftUpPosY+mapSizeY){
		//マップ画面内にマウスがある場合
		if(mouse_get(MOUSE_INPUT_LEFT)==1) {
			ProcessMapPush(mouseX,mouseY);
		}
	}

	//キーボード入力受付
	if(keyboard_get(KEY_INPUT_NUMPADENTER) == 1){
		return -1;
	}
	return 0;
}

void StageEditor::Draw() {
	//デバッグ描画
	clsDx();
	Vector2D v=GetMousePointVector2D();
	printfDx("(%f,%f)\n",v.x,v.y);//素の座標
	v=m_actionSettings.m_pPosSetting->CalculatePos(v,m_actionSettings);
	printfDx("(%f,%f)\n",v.x,v.y);//位置調整後の座標
	for(auto o:*m_actionSettings.GetPMObject()){
		printfDx("%d\n",o.get());
	}
	printfDx("m_pBattleObject:%d\n",m_actionSettings.m_pBattleObject.get());

	//マップの描画
	//マップ描画出来る範囲を制限
	SetDrawArea(leftUpPosX,leftUpPosY,leftUpPosX+mapSizeX, leftUpPosY + mapSizeY);
	//マップ全体の描画
	bool firstflag=true;
	Vector2D mouse=GetMousePointVector2D()-Vector2D((float)leftUpPosX,(float)leftUpPosY)+m_actionSettings.GetMAdjust();//マウスの位置(補正値を考慮しマップ上の座標で表す)
	Vector2D adjust=Vector2D((float)leftUpPosX,(float)leftUpPosY)-m_actionSettings.GetMAdjust();//描画の全体調整位置
	//現在の編集対象図形を描画
	if(m_actionSettings.m_pBattleObject.get()!=nullptr){
		int mode,pal;
		GetDrawBlendMode(&mode,&pal);
		SetDrawBlendMode(DX_BLENDMODE_ALPHA,128);
		m_actionSettings.m_pBattleObject.get()->VDraw(adjust);
		SetDrawBlendMode(mode,pal);
	}
	//マップに存在しているものを全て描画
	for (const std::shared_ptr<BattleObject> &obj : *m_actionSettings.GetPMObject()) {
		//obj.get()->VDraw(leftUpPosX-(int)m_actionSettings.GetMAdjust().x,leftUpPosY-(int)m_actionSettings.GetMAdjust().y);
		obj.get()->VDraw(adjust);
		//マウスが被っている図形には黄色い枠を描画しフォーカスを表現
		if(firstflag && obj.get()->JudgePointInsideShape(mouse)){
			obj.get()->ShapeDraw(GetColor(255,255,0),FALSE,adjust);
			obj.get()->PosDraw(GetColor(255,255,0),TRUE,2.0f,adjust);
			firstflag=false;
		}
		//選択中の図形については赤い枠を描画しフォーカスを表現
		if(obj.get()==m_actionSettings.m_pBattleObject.get()){
			obj.get()->ShapeDraw(GetColor(255,0,0),FALSE,adjust);
			obj.get()->PosDraw(GetColor(255,0,0),TRUE,2,adjust);
		}
	}
	
	//編集前のBattleObjectをマップに仮想的に描画
	if(m_actionSettings.GetMPOriginObject()!=nullptr){
		int mode,pal;
		GetDrawBlendMode(&mode,&pal);
		SetDrawBlendMode(DX_BLENDMODE_ALPHA,128);
		m_actionSettings.GetMPOriginObject()->VDraw(adjust);
		SetDrawBlendMode(mode,pal);
	}
	SetDrawAreaFull();

	//位置設定ガイドの描画
	m_actionSettings.DrawPosSettingGuide(leftUpPosX,leftUpPosY,mapSizeX,mapSizeY);

	//入力されている動作のボタンの描画
	m_actionSettings.DrawEditButtonPushed();

	//入力されている図形設定ボタンの描画
	m_actionSettings.DrawShapeFactoryButtonPushed();
	
	//入力されている位置設定ボタンの描画
	m_actionSettings.DrawPosSettingButtonPushed();
	
	//ボタン群の描画
	for(std::shared_ptr<ButtonHaving::Button> &pb:m_buttons){
		pb.get()->ButtonDraw(m_font,TRUE);
	}


}
