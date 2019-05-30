#include"DemoScene.h"
#include"DemoSwitchUnitScene.h"
#include"input.h"
#include"DxLib.h"
#include"GraphicControl.h"
#include"ToolsLib.h"
#include"CommonConstParameter.h"

//--------------------DemoScene::DemoSceneFactory-------------------
DemoScene::DemoSceneFactory::DemoSceneFactory()
	:SceneFactory()
{}

DemoScene::DemoSceneFactory::~DemoSceneFactory(){}

std::shared_ptr<GameScene> DemoScene::DemoSceneFactory::CreateScene()const{
	return std::shared_ptr<GameScene>(new DemoScene());
}

//--------------------DemoScene-------------------
DemoScene::DemoScene()
	:BattleScene("demo","デモステージ",3)
	,m_frame(0)
	,m_font(CreateFontToHandleEX("メイリオ",36,5,DX_FONTTYPE_ANTIALIASING_EDGE,-1,3))
{
	//m_sceneDataの初期化をやり直す、BattleSceneのコンストラクタ内ではDemoSceneの関数は呼び出せない
	ResetGame();
}

DemoScene::~DemoScene(){
	DeleteFontToHandleEX(m_font);
}

std::shared_ptr<BattleSceneElement> DemoScene::VGetSwitchUnitScene()const{
	return std::shared_ptr<BattleSceneElement>(new DemoSwitchUnitScene(m_battleSceneData));
}

int DemoScene::Calculate(){
	//従来のCalculate()をする
	int index=BattleScene::Calculate();
	m_frame++;
	//クリックまたは決定キーの入力を検知し、検知したらデモを終わらせる
	if(keyboard_get(KEY_INPUT_Z)==1 || mouse_get(MOUSE_INPUT_LEFT)==1){
		return -2;
	}
	return index;
}

void DemoScene::Draw()const{
	//従来のDraw
	BattleScene::Draw();
	//注意書き
	if((m_frame/120)%2==0){
		DrawStringCenterBaseToHandle(CommonConstParameter::gameResolutionX/2,CommonConstParameter::gameResolutionY/2,"Zキー、4ボタン、決定ボタンまたはマウスを左クリックするとタイトル画面に戻ります。",GetColor(255,255,255),m_font,true,GetColor(0,0,0));
	}
}
