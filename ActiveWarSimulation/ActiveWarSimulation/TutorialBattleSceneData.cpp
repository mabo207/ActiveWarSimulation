#include"DxLib.h"
#include"TutorialBattleSceneData.h"
#include"Circle.h"
#include"CommonConstParameter.h"
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

//----------------TutorialBattleSceneData::ExplainTutorial----------------------
TutorialBattleSceneData::ExplainTutorial::ExplainTutorial(const char *gFileName)
	:TutorialBase(TutorialKind::e_explain)
	,m_gHandle(LoadGraphEX(gFileName))
	,m_pos(CommonConstParameter::gameResolutionX*3/2,CommonConstParameter::gameResolutionX/2,CommonConstParameter::gameResolutionY/2,CommonConstParameter::gameResolutionY/2,15,Easing::TYPE_IN,Easing::FUNCTION_EXPO,1.0)
	,m_secondMoveFlag(false)
{}

TutorialBattleSceneData::ExplainTutorial::~ExplainTutorial(){
	DeleteGraphEX(m_gHandle);
}

void TutorialBattleSceneData::ExplainTutorial::DrawSupplement(int font)const{
	int width,height;
	GetGraphSize(m_gHandle,&width,&height);
	DrawGraph(m_pos.GetX()-width/2,m_pos.GetY()-height/2,m_gHandle,TRUE);
}

void TutorialBattleSceneData::ExplainTutorial::SetSecondMoveFlag(){
	m_pos=PositionControl(m_pos.GetX(),m_pos.GetendX()*2-m_pos.GetstartX(),m_pos.GetY(),m_pos.GetendY()*2-m_pos.GetstartY(),m_pos.GetMaxFrame(),Easing::TYPE_OUT,m_pos.GetFunction(),m_pos.GetDegree());
	m_secondMoveFlag=true;
}

//----------------TutorialBattleSceneData-----------------------
TutorialBattleSceneData::TutorialBattleSceneData()
	:BattleSceneData("tutorial",PlayMode::e_tutorial)
	,m_tutorialFont(CreateFontToHandleEX("メイリオ",24,1,-1))
{
	//チュートリアルデータの読み込み

	m_tutorialData.push_back(std::shared_ptr<TutorialBase>(new ExplainTutorial(("Stage/"+m_stageName+"/nonfree/tutorial1.png").c_str())));
	m_tutorialData.push_back(std::shared_ptr<TutorialBase>(new ExplainTutorial(("Stage/"+m_stageName+"/nonfree/tutorial2.png").c_str())));
	m_tutorialData.push_back(std::shared_ptr<TutorialBase>(new MoveTutorial(std::shared_ptr<Shape>(new Circle(Vector2D(395.0f,425.0f),45.0f,Shape::Fix::e_ignore)))));
	m_tutorialData.push_back(std::shared_ptr<TutorialBase>(new ExplainTutorial(("Stage/"+m_stageName+"/nonfree/tutorial3.png").c_str())));
	m_tutorialData.push_back(std::shared_ptr<TutorialBase>(new AttackTutorial(m_unitList[1])));
	m_tutorialData.push_back(std::shared_ptr<TutorialBase>(new ExplainTutorial(("Stage/"+m_stageName+"/nonfree/tutorial6.png").c_str())));
	m_tutorialData.push_back(std::shared_ptr<TutorialBase>(new WaitTutorial()));
}

TutorialBattleSceneData::~TutorialBattleSceneData()
{
}
