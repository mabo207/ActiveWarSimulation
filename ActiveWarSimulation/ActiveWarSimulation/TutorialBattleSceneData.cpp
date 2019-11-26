#include"DxLib.h"
#include"TutorialBattleSceneData.h"
#include"Circle.h"
#include"CommonConstParameter.h"
#include"GraphicControl.h"
#include"BattleSceneData.h"
#include"FileRead.h"
#include"StringBuilder.h"
#include"FilePath.h"

//----------------TutorialBattleSceneData::MoveTutorial----------------------
const float TutorialBattleSceneData::MoveTutorial::minDisplayPopOP=60.0f;

TutorialBattleSceneData::MoveTutorial::MoveTutorial(std::shared_ptr<Shape> moveTutorialArea)
	:TutorialBase(TutorialKind::e_move)
	,m_moveTutorialArea(moveTutorialArea)
	,m_displayPopFlag(false)
{}

TutorialBattleSceneData::MoveTutorial::~MoveTutorial(){}

void TutorialBattleSceneData::MoveTutorial::DrawSupplement(int font)const{
	const int width=650,height=290;
	const int x=CommonConstParameter::gameResolutionX-width,y=((int)BattleSceneData::mapDrawSize.y)-height;
	//黒バックを半透明で描画
	int mode,pal;
	GetDrawBlendMode(&mode,&pal);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA,128);
	DrawBox(x,y,x+width,y+height,GetColor(0,0,0),TRUE);
	SetDrawBlendMode(mode,pal);
	//文字列の描画
	DrawStringNewLineToHandle(x+5,y+5,width-10,height-10
		,"紫に点滅している場所に行こう！\n\n"
		"移動\n"
		"　マウス　　　：左クリックでその場所に向かって移動\n"
		"　ゲームパッド：左ジョイスティック or 十字キー\n"
		"　キーボード　：上下左右キー\n\n"
		"移動のやり直し\n"
		"　マウス　　　：右クリック\n"
		"　ゲームパッド：ボタン３\n"
		"　キーボード　：Xキー"
		,GetColor(255,255,255),font,2);
}

std::shared_ptr<TutorialBattleSceneData::TutorialBase> TutorialBattleSceneData::TutorialBase::Create(StringBuilder &info,const BattleSceneData &gameData){
	//データを分割
	info.Split(',','(',')');
	//チュートリアルデータを作成
	try{
		if(info.m_vec.size()>=2){
			if(info.m_vec[0].GetString()=="move"){
				//MoveTutorialは、到達地点を表す図形が格納されている
				const std::shared_ptr<Shape> pShape=Shape::CreateShape(info.m_vec[1]);
				if(pShape.get()!=nullptr){
					return std::shared_ptr<TutorialBase>(new MoveTutorial(pShape));
				}
			} else if(info.m_vec[0].GetString()=="attack"){
				//AttackTutorialは、初期化された時のユニットの配列番号が格納されている
				const size_t index=std::stoi(info.m_vec[1].GetString().c_str());
				if(index<gameData.m_unitList.size()){
					return std::shared_ptr<TutorialBase>(new AttackTutorial(gameData.m_unitList[index]));
				}
			} else if(info.m_vec[0].GetString()=="wait"){
				//WaitTutorialには特に設定する項目はない
				return std::shared_ptr<TutorialBase>(new WaitTutorial());
			} else if(info.m_vec[0].GetString()=="explain"){
				//ExplainTutorialには画像ファイル名が入っている
				return std::shared_ptr<TutorialBase>(new ExplainTutorial((FilePath::stageDir+gameData.m_stageDirName+info.m_vec[1].GetString()).c_str()));
			} else if(info.m_vec[0].GetString()=="blank"){
				//BlankTutorialには何もしない行動の回数が入っている
				const int count=std::stoi(info.m_vec[1].GetString().c_str());
				if(count>0){
					return std::shared_ptr<TutorialBase>(new BlankTutorial(count));
				}
			}
		}
	} catch(const std::invalid_argument &){
		//数値じゃない値が検出された場合
	} catch(const std::out_of_range &){
		//範囲外の値が検出された場合
	}
	//作成失敗時
	return std::shared_ptr<TutorialBase>(nullptr);
}

//----------------TutorialBattleSceneData::AttackTutorial----------------------
TutorialBattleSceneData::AttackTutorial::AttackTutorial(const Unit *targetUnit)
	:TutorialBase(TutorialKind::e_normalAttack)
	,m_targetUnit(targetUnit)
{}

TutorialBattleSceneData::AttackTutorial::~AttackTutorial(){}

void TutorialBattleSceneData::AttackTutorial::DrawSupplement(int font)const{
	const int width=650,height=290;
	const int x=CommonConstParameter::gameResolutionX-width,y=((int)BattleSceneData::mapDrawSize.y)-height;
	//黒バックを半透明で描画
	int mode,pal;
	GetDrawBlendMode(&mode,&pal);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA,128);
	DrawBox(x,y,x+width,y+height,GetColor(0,0,0),TRUE);
	SetDrawBlendMode(mode,pal);
	//文字列の描画
	DrawStringNewLineToHandle(x+5,y+5,width-10,height-10
		,"矢印で指定されたユニットを攻撃しよう！\n\n"
		"攻撃\n"
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
	const int width=650,height=195;
	const int x=CommonConstParameter::gameResolutionX-width,y=((int)BattleSceneData::mapDrawSize.y)-height;
	//黒バックを半透明で描画
	int mode,pal;
	GetDrawBlendMode(&mode,&pal);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA,128);
	DrawBox(x,y,x+width,y+height,GetColor(0,0,0),TRUE);
	SetDrawBlendMode(mode,pal);
	//文字列の描画
	DrawStringNewLineToHandle(x+5,y+5,width-10,height-10
		,"その場で待機してみよう。\n\n"
		"待機\n"
		"　マウス　　　：マウスホイールを押す\n"
		"　　　　または　右下のNEXTボタンを左クリック\n"
		"　ゲームパッド：ボタン２\n"
		"　キーボード　：Vキー"
		,GetColor(255,255,255),font,2);
}

//----------------TutorialBattleSceneData::ExplainTutorial----------------------
TutorialBattleSceneData::ExplainTutorial::ExplainTutorial(const char *gFileName)
	:TutorialBase(TutorialKind::e_explain)
	,m_gHandle(LoadGraphEX(gFileName))
	//,m_pos(CommonConstParameter::gameResolutionX*2,CommonConstParameter::gameResolutionX/2,CommonConstParameter::gameResolutionY/2,CommonConstParameter::gameResolutionY/2,30,Easing::TYPE_OUT,Easing::FUNCTION_BACK,0.2)
	,m_rate(0,1000,25,Easing::TYPE_OUT,Easing::FUNCTION_BACK,0.2)
	,m_cooltime(0)
	,m_secondMoveFlag(false)
{}

TutorialBattleSceneData::ExplainTutorial::~ExplainTutorial(){
	DeleteGraphEX(m_gHandle);
}

void TutorialBattleSceneData::ExplainTutorial::DrawSupplement(int font)const{
	int width,height;
	GetGraphSize(m_gHandle,&width,&height);
	const double rate=(m_secondMoveFlag?1.0-m_rate.GetRate():m_rate.GetRate());
	width=(int)(width*rate);
	height=(int)(height*rate);
	//DrawGraph(m_pos.GetX()-width/2,m_pos.GetY()-height/2,m_gHandle,TRUE);
	DrawExtendGraph((CommonConstParameter::gameResolutionX-width)/2,(CommonConstParameter::gameResolutionY-height)/2,(CommonConstParameter::gameResolutionX+width)/2,(CommonConstParameter::gameResolutionY+height)/2,m_gHandle,TRUE);
}

void TutorialBattleSceneData::ExplainTutorial::SetSecondMoveFlag(){
	//m_pos=PositionControl(m_pos.GetX(),m_pos.GetendX()*2-m_pos.GetstartX(),m_pos.GetY(),m_pos.GetendY()*2-m_pos.GetstartY(),m_pos.GetMaxFrame(),Easing::TYPE_IN,m_pos.GetFunction(),m_pos.GetDegree());
	m_rate=Easing(m_rate.GetX(),0,m_rate.GetMaxFrame(),Easing::TYPE_IN,m_rate.GetFunction(),m_rate.GetDegree());
	m_secondMoveFlag=true;
}

//----------------TutorialBattleSceneData::BlankTutorial----------------------
TutorialBattleSceneData::BlankTutorial::BlankTutorial(int count)
	:TutorialBase(TutorialKind::e_blank)
	,m_leftCount(count)
{}

void TutorialBattleSceneData::BlankTutorial::DrawSupplement(int font)const{}

//----------------TutorialBattleSceneData-----------------------
TutorialBattleSceneData::TutorialBattleSceneData(const std::string &stageDirName,const std::string &titleName,const StageLevel stageLevel)
	:BattleSceneData(stageDirName,titleName,stageLevel,PlayMode::e_tutorial)
{
	//チュートリアルデータの読み込み
	//オブジェクト群は{}で囲まれ\nで区切られているので、１階層だけ分割読み込みして、オブジェクトを生成する
	try{
		StringBuilder sb(FileStrRead((FilePath::stageDir+m_stageDirName+"/tutorialList.txt").c_str()),'\n','{','}');
		for(StringBuilder &ssb:sb.m_vec){
			std::shared_ptr<TutorialBase> pt=TutorialBase::Create(ssb,*this);//sb,ssbは変更される
			if(pt.get()!=nullptr){
				m_tutorialData.push_back(pt);
			}
		}
	} catch(const FileOpenFailedException &){
		//特に何もしない
	}
}

TutorialBattleSceneData::~TutorialBattleSceneData(){}
