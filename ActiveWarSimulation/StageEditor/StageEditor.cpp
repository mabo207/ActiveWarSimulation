#define _USE_MATH_DEFINES	//math.hの定数を使うために必須

#include<math.h>
#include<fstream>

#include"DxLib.h"
#include"StageEditor.h"
#include"input.h"
#include"CommonConstParameter.h"

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

#include"SelectLevel.h"

#include"SettingFunction.h"

#include"ScrollBar.h"

//定数の定義
namespace {
	//マップの表示部分の大きさ
	const int mapSizeX = CommonConstParameter::mapSizeX;
	const int mapSizeY = CommonConstParameter::mapSizeY;
	//マップの左上の座標
	const int leftUpPosX = 25;
	const int leftUpPosY = 25;
	//「動作」ボタンの縦横の個数
	const size_t actButtonWidthNum=2;
	const size_t actButtonHeightNum=3;
	//「動作」ボタン部分全体での横幅,縦幅
	const int buttonWidth = 400;
	const int buttonHeight=300;
	//「図形設定」ボタンの縦横の個数
	const size_t shapeButtonWidthNum=3;
	const size_t shapeButtonHeightNum=3;
	//「図形設定」ボタン部分全体での横幅,縦幅
	const int shapeButtonWidth=buttonWidth;
	const int shapeButtonHeight=240;
	//「位置設定」ボタンの縦横の個数
	const size_t posButtonWidthNum=3;
	const size_t posButtonHeightNum=1;
	//「位置設定」ボタン部分全体での横幅,縦幅
	const int posButtonWidth=buttonWidth;
	const int posButtonHeight=100;
	//「レベル設定」ボタンの縦横の個数
	const size_t levelButtonWidthNum=StageLevel::levelCount;
	const size_t levelButtonHeightNum=1;
	//「レベル設定」ボタン部分全体での横幅,縦幅
	const int levelButtonWidth=buttonWidth;
	const int levelButtonHeight=120;
	//「入出力」ボタンの縦横の個数
	const size_t readWriteButtonWidthNum=3;
	const size_t readWriteButtonHeightNum=1;
	//「入出力」ボタン部分全体での横幅,縦幅
	const int readWriteButtonWidth=buttonWidth;
	const int readWriteButtonHeight=120;
	//エディタで作られる物のサイズの基準の大きさ・基本単位
	const int baseSize=CommonConstParameter::unitCircleSize;
}

//関数定義
//静的関数
int StageEditor::GetEditorSizeX(){
	return leftUpPosX*2+mapSizeX+buttonWidth;
}

int StageEditor::GetEditorSizeY(){
	return leftUpPosY*2+mapSizeY;
}

//動的関数
StageEditor::StageEditor()
	:m_actionSettings(
		std::shared_ptr<EditAction>(nullptr)
		,std::shared_ptr<BattleObject>(new Terrain(std::shared_ptr<Shape>(new Circle(Vector2D(0.0f,0.0f),20.0f,Shape::Fix::e_static)),-1,GetColor(128,128,128),false))
		,std::shared_ptr<ShapeFactory>(nullptr)
		,std::shared_ptr<PosSetting>(nullptr))
{
	//ボタン一覧
	
	//最初から押されているようにするボタンをリストアップしながら行う
	std::shared_ptr<ButtonHaving::Button> pPutButton,pRectangleFactoryButton,pPosSettingButton,pSelectLevelButton;
	
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
	const float buttonsLeftEdge=(float)(leftUpPosX*2+mapSizeX);
	float buttonY=0.0f;
	m_buttons.push_back(std::shared_ptr<ButtonHaving::Button>(new EditPut::EditPutButton(
		Vector2D(buttonsLeftEdge+(float)(buttonWidth/actButtonWidthNum*0),buttonY)
		,Vector2D((float)(buttonWidth/actButtonWidthNum),(float)(buttonHeight/actButtonHeightNum))
	)));
	pPutButton=m_buttons.back();//最初から押されているようにするボタン
	//removeボタン
	m_buttons.push_back(std::shared_ptr<ButtonHaving::Button>(new EditRemove::EditRemoveButton(
		Vector2D(buttonsLeftEdge+(float)(buttonWidth/actButtonWidthNum*1),buttonY)
		,Vector2D((float)(buttonWidth/actButtonWidthNum),(float)(buttonHeight/actButtonHeightNum))
	)));
	buttonY+=(float)(buttonHeight/actButtonHeightNum*1);
	//moveボタン
	m_buttons.push_back(std::shared_ptr<ButtonHaving::Button>(new EditMove::EditMoveButton(
		Vector2D(buttonsLeftEdge+(float)(buttonWidth/actButtonWidthNum*0),buttonY)
		,Vector2D((float)(buttonWidth/actButtonWidthNum),(float)(buttonHeight/actButtonHeightNum))
	)));
	//resizeボタン
	m_buttons.push_back(std::shared_ptr<ButtonHaving::Button>(new EditResize::EditResizeButton(
		Vector2D(buttonsLeftEdge+(float)(buttonWidth/actButtonWidthNum*1),buttonY)
		,Vector2D((float)(buttonWidth/actButtonWidthNum),(float)(buttonHeight/actButtonHeightNum))
	)));
	buttonY+=(float)(buttonHeight/actButtonHeightNum*1);
	//UnitEditボタン
	m_buttons.push_back(std::shared_ptr<ButtonHaving::Button>(new EditUnitParameter::EditUnitParameterButton(
		Vector2D(buttonsLeftEdge+(float)(buttonWidth/actButtonWidthNum*0),buttonY)
		,Vector2D((float)(buttonWidth/actButtonWidthNum),(float)(buttonHeight/actButtonHeightNum))
	)));
	buttonY+=(float)(buttonHeight/actButtonHeightNum*1);

	//CircleFactoryボタン
	m_buttons.push_back(std::shared_ptr<ButtonHaving::Button>(new CircleFactory::CircleFactoryButton(
		Vector2D(buttonsLeftEdge+(float)(shapeButtonWidth/shapeButtonWidthNum*0),buttonY)
		,Vector2D((float)(shapeButtonWidth/shapeButtonWidthNum),(float)(shapeButtonHeight/shapeButtonHeightNum))
	)));
	pRectangleFactoryButton=m_buttons.back();//最初から押されているようにする
	//EdgeFactoryボタン
	m_buttons.push_back(std::shared_ptr<ButtonHaving::Button>(new EdgeFactory::EdgeFactoryButton(
		Vector2D(buttonsLeftEdge+(float)(shapeButtonWidth/shapeButtonWidthNum*1),buttonY)
		,Vector2D((float)(shapeButtonWidth/shapeButtonWidthNum),(float)(shapeButtonHeight/shapeButtonHeightNum))
	)));
	//PolygonFactoryボタン
	m_buttons.push_back(std::shared_ptr<ButtonHaving::Button>(new PolygonFactory::PolygonFactoryButton(
		Vector2D(buttonsLeftEdge+(float)(shapeButtonWidth/shapeButtonWidthNum*2),buttonY)
		,Vector2D((float)(shapeButtonWidth/shapeButtonWidthNum),(float)(shapeButtonHeight/shapeButtonHeightNum))
	)));
	buttonY+=+(float)(shapeButtonHeight/shapeButtonHeightNum*1);
	//戦士作成ボタン
	m_buttons.push_back(std::shared_ptr<ButtonHaving::Button>(new UnitFactory::UnitFactoryButton(
		Vector2D(buttonsLeftEdge+(float)(shapeButtonWidth/shapeButtonWidthNum*0),buttonY)
		,Vector2D((float)(shapeButtonWidth/shapeButtonWidthNum),(float)(shapeButtonHeight/shapeButtonHeightNum))
		,Unit::Profession::e_soldier
	)));
	//重装兵作成ボタン
	m_buttons.push_back(std::shared_ptr<ButtonHaving::Button>(new UnitFactory::UnitFactoryButton(
		Vector2D(buttonsLeftEdge+(float)(shapeButtonWidth/shapeButtonWidthNum*1),buttonY)
		,Vector2D((float)(shapeButtonWidth/shapeButtonWidthNum),(float)(shapeButtonHeight/shapeButtonHeightNum))
		,Unit::Profession::e_armer
	)));
	//射手作成ボタン
	m_buttons.push_back(std::shared_ptr<ButtonHaving::Button>(new UnitFactory::UnitFactoryButton(
		Vector2D(buttonsLeftEdge+(float)(shapeButtonWidth/shapeButtonWidthNum*2),buttonY)
		,Vector2D((float)(shapeButtonWidth/shapeButtonWidthNum),(float)(shapeButtonHeight/shapeButtonHeightNum))
		,Unit::Profession::e_archer
	)));
	buttonY+=+(float)(shapeButtonHeight/shapeButtonHeightNum*1);
	//魔道士作成ボタン
	m_buttons.push_back(std::shared_ptr<ButtonHaving::Button>(new UnitFactory::UnitFactoryButton(
		Vector2D(buttonsLeftEdge+(float)(shapeButtonWidth/shapeButtonWidthNum*0),buttonY)
		,Vector2D((float)(shapeButtonWidth/shapeButtonWidthNum),(float)(shapeButtonHeight/shapeButtonHeightNum))
		,Unit::Profession::e_mage
	)));
	//衛生兵作成ボタン
	m_buttons.push_back(std::shared_ptr<ButtonHaving::Button>(new UnitFactory::UnitFactoryButton(
		Vector2D(buttonsLeftEdge+(float)(shapeButtonWidth/shapeButtonWidthNum*1),buttonY)
		,Vector2D((float)(shapeButtonWidth/shapeButtonWidthNum),(float)(shapeButtonHeight/shapeButtonHeightNum))
		,Unit::Profession::e_healer
	)));
	buttonY+=+(float)(shapeButtonHeight/shapeButtonHeightNum*1);

	//1px位置調整ボタン
	m_buttons.push_back(std::shared_ptr<ButtonHaving::Button>(new ConstPosSet::ConstPosSetButton(
		Vector2D(buttonsLeftEdge+(float)(posButtonWidth/posButtonWidthNum*0),buttonY)
		,Vector2D((float)(posButtonWidth/posButtonWidthNum),(float)(posButtonHeight/posButtonHeightNum))
		,1
	)));
	pPosSettingButton=m_buttons.back();
	//45px位置調整ボタン
	m_buttons.push_back(std::shared_ptr<ButtonHaving::Button>(new ConstPosSet::ConstPosSetButton(
		Vector2D(buttonsLeftEdge+(float)(posButtonWidth/posButtonWidthNum*1),buttonY)
		,Vector2D((float)(posButtonWidth/posButtonWidthNum),(float)(posButtonHeight/posButtonHeightNum))
		,baseSize
	)));
	buttonY+=+(float)(posButtonHeight/posButtonHeightNum);

	//easyボタン
	m_buttons.push_back(std::shared_ptr<ButtonHaving::Button>(new SelectLevel::SelectLevelButton(
		Vector2D(buttonsLeftEdge+(float)(levelButtonWidth/levelButtonWidthNum*0),buttonY)
		,Vector2D((float)(levelButtonWidth/levelButtonWidthNum),(float)(levelButtonHeight/levelButtonHeightNum))
		,StageLevel::e_easy
	)));
	pSelectLevelButton=m_buttons.back();
	//normalボタン
	m_buttons.push_back(std::shared_ptr<ButtonHaving::Button>(new SelectLevel::SelectLevelButton(
		Vector2D(buttonsLeftEdge+(float)(levelButtonWidth/levelButtonWidthNum*1),buttonY)
		,Vector2D((float)(levelButtonWidth/levelButtonWidthNum),(float)(levelButtonHeight/levelButtonHeightNum))
		,StageLevel::e_normal
	)));
	//hardボタン
	m_buttons.push_back(std::shared_ptr<ButtonHaving::Button>(new SelectLevel::SelectLevelButton(
		Vector2D(buttonsLeftEdge+(float)(levelButtonWidth/levelButtonWidthNum*2),buttonY)
		,Vector2D((float)(levelButtonWidth/levelButtonWidthNum),(float)(levelButtonHeight/levelButtonHeightNum))
		,StageLevel::e_hard
	)));
	//lunaticボタン
	m_buttons.push_back(std::shared_ptr<ButtonHaving::Button>(new SelectLevel::SelectLevelButton(
		Vector2D(buttonsLeftEdge+(float)(levelButtonWidth/levelButtonWidthNum*3),buttonY)
		,Vector2D((float)(levelButtonWidth/levelButtonWidthNum),(float)(levelButtonHeight/levelButtonHeightNum))
		,StageLevel::e_lunatic
	)));
	buttonY+=(float)(levelButtonHeight/levelButtonHeightNum);
	
	//ReadStageボタン
	m_buttons.push_back(std::shared_ptr<ButtonHaving::Button>(new SettingFunction::SettingFunctionButton(
		Vector2D(buttonsLeftEdge+(float)(readWriteButtonWidth/readWriteButtonWidthNum*0),buttonY)
		,Vector2D((float)(readWriteButtonWidth/readWriteButtonWidthNum),(float)(readWriteButtonHeight/readWriteButtonHeightNum))
		,"ReadStageAndUnit"
		,[](EditActionSettings &settings){
			settings.InitObjects();
			settings.ReadStage("SaveData/stage.txt");
			settings.ReadUnit();
		}
	)));
	m_buttons.push_back(std::shared_ptr<ButtonHaving::Button>(new SettingFunction::SettingFunctionButton(
		Vector2D(buttonsLeftEdge+(float)(readWriteButtonWidth/readWriteButtonWidthNum*1),buttonY)
		,Vector2D((float)(readWriteButtonWidth/readWriteButtonWidthNum),(float)(readWriteButtonHeight/readWriteButtonHeightNum))
		,"WriteStage"
		,[](EditActionSettings &settings){settings.WriteOutStage("SaveData/stage.txt");}
	)));
	m_buttons.push_back(std::shared_ptr<ButtonHaving::Button>(new SettingFunction::SettingFunctionButton(
		Vector2D(buttonsLeftEdge+(float)(readWriteButtonWidth/readWriteButtonWidthNum*2),buttonY)
		,Vector2D((float)(readWriteButtonWidth/readWriteButtonWidthNum),(float)(readWriteButtonHeight/readWriteButtonHeightNum))
		,"WriteUnit"
		,[](EditActionSettings &settings){settings.WriteOutUnit();}
	)));
	buttonY+=(float)(readWriteButtonHeight/readWriteButtonHeightNum);

	//最初から押されているようにするボタンを押す(順番に注意！)
	pRectangleFactoryButton->PushedProcess(m_actionSettings);
	pPutButton->PushedProcess(m_actionSettings);
	pPosSettingButton->PushedProcess(m_actionSettings);
	pSelectLevelButton->PushedProcess(m_actionSettings);
	
	//フォント
	m_font=CreateFontToHandle("メイリオ",16,1);

	//背景グラフィック
	m_mapPic=LoadGraph("Savedata/stage.png");

	//図形の読み込み
	m_actionSettings.InitObjects();
	m_actionSettings.ReadStage("Savedata/stage.txt");
	m_actionSettings.ReadUnit();
}

StageEditor::~StageEditor() {
	//フォント
	DeleteFontToHandle(m_font);
	//画像
	DeleteGraph(m_mapPic);
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

//ユニット情報の描画
void StageEditor::DrawUnitInfo(const std::shared_ptr<const Unit> &punit,const Vector2D unitDrawPos)const{
	DrawUnitInfo(punit,unitDrawPos,false,0);
}

void StageEditor::DrawUnitInfo(const std::shared_ptr<const Unit> &punit,const Vector2D unitDrawPos,bool drawEditItem,size_t index)const{
	if(punit){
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
		const int strSpace=2;
		const unsigned int backColor=GetColor(0,128,64);
		const unsigned int strColor=GetColor(255,255,255);
		//能力値についての文字列作成
		//能力値の描画
		std::string drawStr;
		drawStr.reserve(300);
		drawStr+="Lv: "+std::to_string(punit->GetBaseStatus().lv)+'\n';
		drawStr+="Team: "+Unit::Team::GetName(punit->GetBattleStatus().team)+'\n';
		drawStr+="Profession: "+Unit::Profession::GetName(punit->GetBaseStatus().profession)+'\n';
		drawStr+="AItype: "+Unit::AIType::GetName(punit->GetBattleStatus().aitype)+'\n';
		drawStr+="AIgroup: "+std::to_string(punit->GetBattleStatus().aiGroup)+'\n';
		drawStr+="HP: "+std::to_string(punit->GetBattleStatus().HP)+" / "+std::to_string(punit->GetBaseStatus().maxHP)+'\n';
		drawStr+="Weapon: "+punit->GetBattleStatus().weapon->GetName()+'\n';
		//以下、編集できないもの
		drawStr+="\nPOW: "+std::to_string(punit->GetBaseStatus().power)+'\n';
		drawStr+="DEF: "+std::to_string(punit->GetBaseStatus().def)+'\n';
		drawStr+="MPOW: "+std::to_string(punit->GetBaseStatus().mpower)+'\n';
		drawStr+="MDEF: "+std::to_string(punit->GetBaseStatus().mdef)+'\n';
		drawStr+=punit->GetBattleStatus().weapon->GetEffectivenessString(punit.get());
		//下地の描画
		DrawTriangle(unitX+baseSize,unitY,left,unitY-10,left,unitY+10,backColor,TRUE);
		DrawBox(left,top,left+boxWidth,top+boxHeight,backColor,TRUE);
		//能力値の描画
		DrawStringNewLineToHandle(left+mergin,top+mergin,boxWidth-mergin*2,boxHeight-mergin*2,drawStr.c_str(),strColor,m_font,strSpace);
		if(drawEditItem){
			//編集項目の描画
			const int strLineHeight=GetFontSizeToHandle(m_font)+strSpace;
			const int selectBoxY=top+mergin+strLineHeight*index;
			DrawBox(left+mergin,selectBoxY,left+boxWidth-mergin,selectBoxY+strLineHeight,GetColor(255,255,0),FALSE);
		}
	}
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

	//右クリックされたら
	m_actionSettings.UpdateMouseObjectDepth(mouse_get(MOUSE_INPUT_RIGHT));//押してない時でも更新することがあるので、押しているフレーム数を渡す。

	//キーボード入力受付
	if(keyboard_get(KEY_INPUT_S)==10){
		//Sキー長押しで地形を保存
		m_actionSettings.WriteOutStage("SaveData/stage.txt");
	} else if(keyboard_get(KEY_INPUT_U)==10){
		//Uキー長押しでユニットを保存
		m_actionSettings.WriteOutUnit();
	} else if(keyboard_get(KEY_INPUT_R)==10){
		//Rキー長押しで読み込み
		m_actionSettings.InitObjects();
		m_actionSettings.ReadStage("SaveData/stage.txt");
		m_actionSettings.ReadUnit();
	} else if(keyboard_get(KEY_INPUT_NUMPADENTER) == 1){
		//Enterキー入力でエディタを終了
		return -1;
	} else{
		auto func=[](int pal){
			return (pal==1 || (pal>=30 && pal%6==0));
		};
		const bool up=func(keyboard_get(KEY_INPUT_UP));
		const bool down=func(keyboard_get(KEY_INPUT_DOWN));
		const bool left=func(keyboard_get(KEY_INPUT_LEFT));
		const bool right=func(keyboard_get(KEY_INPUT_RIGHT));
		const std::shared_ptr<EditUnitParameter> editUnit=std::dynamic_pointer_cast<EditUnitParameter>(m_actionSettings.m_pEditAction);
		if(editUnit){
			//現在EditUnitParameterをしているなら、キーボードの入力を反映させる
			editUnit->EditParameter(up,down,left,right);
		}
	}
	return 0;
}

void StageEditor::Draw() {
	//デバッグ描画
	clsDx();
	Vector2D v=GetMousePointVector2D();
	//printfDx("(%f,%f)\n",v.x,v.y);//素の座標
	v=m_actionSettings.m_pPosSetting->CalculatePos(v-Vector2D((float)leftUpPosX,(float)leftUpPosY)+m_actionSettings.GetMAdjust(),m_actionSettings);
	printfDx("(%f,%f)\n",v.x,v.y);//位置調整後の座標
	for(auto o:*m_actionSettings.GetPMObject()){
		printfDx("%d\n",o.get());
	}
	//printfDx("m_pBattleObject:%d\n",m_actionSettings.m_pBattleObject.get());

	//マップの描画
	//マップ描画出来る範囲を制限
	SetDrawArea(leftUpPosX,leftUpPosY,leftUpPosX+mapSizeX, leftUpPosY + mapSizeY);
	//マップ全体の描画
	bool firstflag=true;
	Vector2D mouse=GetMousePointVector2D()-Vector2D((float)leftUpPosX,(float)leftUpPosY)+m_actionSettings.GetMAdjust();//マウスの位置(補正値を考慮しマップ上の座標で表す)
	Vector2D adjust=Vector2D((float)leftUpPosX,(float)leftUpPosY)-m_actionSettings.GetMAdjust();//描画の全体調整位置
	DrawGraph((int)adjust.x,(int)adjust.y,m_mapPic,TRUE);
	//現在の編集対象図形を描画
	if(m_actionSettings.m_pBattleObject.get()!=nullptr){
		int mode,pal;
		GetDrawBlendMode(&mode,&pal);
		SetDrawBlendMode(DX_BLENDMODE_ALPHA,128);
		m_actionSettings.m_pBattleObject.get()->VDraw(adjust);
		SetDrawBlendMode(mode,pal);
	}
	//マップに存在しているものを全て描画
	const BattleObject *mouseObj=m_actionSettings.GetMousePointedObjectPointer(mouse);
	for (const std::shared_ptr<BattleObject> &obj : *m_actionSettings.GetPMObject()) {
		obj.get()->VDraw(adjust);
		//マウスが被っている図形には黄色い枠を描画しフォーカスを表現
		if(firstflag && obj.get()==mouseObj){
			obj.get()->ShapeDraw(GetColor(255,255,0),FALSE,1.0f,adjust);
			obj.get()->PosDraw(GetColor(255,255,0),TRUE,2.0f,adjust);
			firstflag=false;
		}
		//選択中の図形については赤い枠を描画しフォーカスを表現
		if(obj.get()==m_actionSettings.m_pBattleObject.get()){
			obj.get()->ShapeDraw(GetColor(255,0,0),FALSE,1.0f,adjust);
			obj.get()->PosDraw(GetColor(255,0,0),TRUE,2.0f,adjust);
		}
	}
	
	//編集のためのデータを描画。図形描画が主
	m_actionSettings.m_pEditAction->ActionDraw(adjust,m_actionSettings);
/*	
	//編集前のBattleObjectをマップに仮想的に描画
	if(m_actionSettings.GetMPOriginObject()!=nullptr){
		int mode,pal;
		GetDrawBlendMode(&mode,&pal);
		SetDrawBlendMode(DX_BLENDMODE_ALPHA,128);
		m_actionSettings.GetMPOriginObject()->VDraw(adjust);
		SetDrawBlendMode(mode,pal);
	}
//*/

	//AI操作の際の参考となるユニットの半径間隔の格子点の描画
	{
		int init[2]={(int)m_actionSettings.GetMAdjust().x,(int)m_actionSettings.GetMAdjust().y};
		for(int &pal:init){
			//x,yの描画初期位置を調整する
			if(pal<0){
				pal=baseSize-((-pal)%baseSize);
			} else{
				pal=pal%baseSize;
			}
			//このままだと、palは「m_adjustから左上方向にpalだけ離れた場所に格子点が存在する」という値のままなので、マップ上で最も左上に描画される場所を示す値にしておく。
			pal=baseSize-pal;
		}
		for(int y=init[1]+leftUpPosY;y<leftUpPosY+mapSizeY;y+=baseSize){
			for(int x=init[0]+leftUpPosX;x<leftUpPosX+mapSizeX;x+=baseSize){
				DrawCircle(x,y,2,GetColor(255,64,64),TRUE);
			}
		}
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
	
	//入力されているレベル設定ボタンの描画
	m_actionSettings.m_pSelectLevel->LightUpButton();

	//入力されている出入力ボタンの描画
	if(m_actionSettings.m_pSettingFunction){
		m_actionSettings.m_pSettingFunction->LightUpButton();
	}

	//ボタン群の描画
	for(std::shared_ptr<ButtonHaving::Button> &pb:m_buttons){
		pb.get()->ButtonDraw(m_font,TRUE);
	}

	//現在EditUnitParameterをしているなら、ユニットの能力値を表示させる
	{
		const std::shared_ptr<EditUnitParameter> editUnitAction=std::dynamic_pointer_cast<EditUnitParameter>(m_actionSettings.m_pEditAction);
		if(editUnitAction){
			//編集対象がUnitでないと描画しない
			const std::shared_ptr<const Unit> punit=editUnitAction->GetEditResult();
			if(punit){
				punit->BattleObject::VDraw(adjust);//編集後のユニットの様子も表示
				const Vector2D unitDrawPos=punit->getPos()+adjust;
				DrawUnitInfo(punit,unitDrawPos,true,editUnitAction->GetEditIndex());
			}
		}
	}
}
