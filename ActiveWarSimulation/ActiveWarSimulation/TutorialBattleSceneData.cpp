#include"DxLib.h"
#include"TutorialBattleSceneData.h"
#include"Circle.h"
#include"CommonConstParameter.h"
#include"ToolsLib.h"
#include"GraphicControl.h"

//----------------TutorialBattleSceneData::MoveTutorial----------------------
TutorialBattleSceneData::MoveTutorial::MoveTutorial(std::shared_ptr<Shape> moveTutorialArea)
	:TutorialBase(TutorialKind::e_move)
	,m_moveTutorialArea(moveTutorialArea)
{}

TutorialBattleSceneData::MoveTutorial::~MoveTutorial(){}

void TutorialBattleSceneData::MoveTutorial::DrawSupplement(int font)const{
	int width=650,height=250;
	const int x=CommonConstParameter::gameResolutionX-width,y=0;
	//黒バックを半透明で描画
	int mode,pal;
	GetDrawBlendMode(&mode,&pal);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA,128);
	DrawBox(x,y,x+width,y+height,GetColor(0,0,0),TRUE);
	SetDrawBlendMode(mode,pal);
	//文字列の描画
	DrawStringNewLineToHandle(x+5,y+5,width-10,height-10
		,"移動\n"
		"　マウス　　　：左クリックでその場所に向かって移動\n"
		"　ゲームパッド：左ジョイスティック or 十字キー\n"
		"　キーボード　：上下左右キー\n\n"
		"移動のやり直し\n"
		"　マウス　　　：右クリック\n"
		"　ゲームパッド：ボタン３\n"
		"　キーボード　：Xキー"
		,GetColor(255,255,255),font,2);
}

//----------------TutorialBattleSceneData::AttackTutorial----------------------
TutorialBattleSceneData::AttackTutorial::AttackTutorial(const Unit *targetUnit)
	:TutorialBase(TutorialKind::e_normalAttack)
	,m_targetUnit(targetUnit)
{}

TutorialBattleSceneData::AttackTutorial::~AttackTutorial(){}

void TutorialBattleSceneData::AttackTutorial::DrawSupplement(int font)const{
	int width=650,height=250;
	const int x=CommonConstParameter::gameResolutionX-width,y=0;
	//黒バックを半透明で描画
	int mode,pal;
	GetDrawBlendMode(&mode,&pal);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA,128);
	DrawBox(x,y,x+width,y+height,GetColor(0,0,0),TRUE);
	SetDrawBlendMode(mode,pal);
	//文字列の描画
	DrawStringNewLineToHandle(x+5,y+5,width-10,height-10
		,"攻撃\n"
		"　マウス　　　：攻撃したいユニットを左クリック\n"
		"　ゲームパッド：ボタン４\n"
		"　キーボード　：Zキー\n\n"
		"攻撃対象の変更\n"
		"　マウス　　　：攻撃したいユニットにマウスを合わせる\n"
		"　ゲームパッド：ボタン７・ボタン８\n"
		"　キーボード　：Aキー・Sキー"
		,GetColor(255,255,255),font,2);
}

//----------------TutorialBattleSceneData::WaitTutorial----------------------
TutorialBattleSceneData::WaitTutorial::WaitTutorial()
	:TutorialBase(TutorialKind::e_wait)
{}

TutorialBattleSceneData::WaitTutorial::~WaitTutorial(){}

void TutorialBattleSceneData::WaitTutorial::DrawSupplement(int font)const{
	int width=650,height=120;
	const int x=CommonConstParameter::gameResolutionX-width,y=0;
	//黒バックを半透明で描画
	int mode,pal;
	GetDrawBlendMode(&mode,&pal);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA,128);
	DrawBox(x,y,x+width,y+height,GetColor(0,0,0),TRUE);
	SetDrawBlendMode(mode,pal);
	//文字列の描画
	DrawStringNewLineToHandle(x+5,y+5,width-10,height-10
		,"待機\n"
		"　マウス　　　：右下のNEXTボタンを左クリック\n"
		"　ゲームパッド：ボタン２\n"
		"　キーボード　：Vキー"
		,GetColor(255,255,255),font,2);
}

//----------------TutorialBattleSceneData-----------------------
TutorialBattleSceneData::TutorialBattleSceneData()
	:BattleSceneData("tutorial",PlayMode::e_tutorial)
	,m_tutorialFont(CreateFontToHandleEX("メイリオ",24,1,-1))
{
	//チュートリアルデータの読み込み

	m_tutorialData.push_back(std::shared_ptr<TutorialBase>(new MoveTutorial(std::shared_ptr<Shape>(new Circle(Vector2D(395.0f,425.0f),45.0f,Shape::Fix::e_ignore)))));
	m_tutorialData.push_back(std::shared_ptr<TutorialBase>(new AttackTutorial(m_unitList[1])));
	m_tutorialData.push_back(std::shared_ptr<TutorialBase>(new WaitTutorial()));
}

TutorialBattleSceneData::~TutorialBattleSceneData()
{
}
