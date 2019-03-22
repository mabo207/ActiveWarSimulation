#define _USE_MATH_DEFINES
#include"TitleScene.h"
#include"DxLib.h"
#include"GraphicControl.h"
#include"MyPolygon.h"
#include"input.h"
#include<cmath>
#include"StageSelectScene.h"
#include"BattleScene.h"
#include"DemoScene.h"
#include"TutorialScene.h"
#include"GeneralPurposeResourceManager.h"
#include"CommonConstParameter.h"

//-------------------TitleScene-------------------
std::string TitleScene::SelectItem::GetString(const Kind kind){
	switch(kind){
	case(e_stageSelect):
		return "STAGE SELECT";
	case(e_tutorial):
		return "TUTORIAL";
	case(e_tutorial_2):
		return "TUTORIAL2";
	case(e_demo):
		return "DEMO PLAY";
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

const Vector2D TitleScene::strPos[TitleScene::SelectItem::COUNTER]={
	Vector2D(1450.0f,790.0f)
	,Vector2D(1530.0f,920.0f)
	,Vector2D(1610.0f,790.0f)
	,Vector2D(1690.0f,920.0f)
	,Vector2D(1770.0f,790.0f)
};

TitleScene::TitleScene()
	:MainControledGameScene()
	,m_backPic(LoadGraphEX("Graphic/nonfree/titleScene.png"))
	,m_itemFont(CreateFontToHandleEX("メイリオ",16,1,-1))
	,m_bgm(LoadBGMMem("Sound/bgm/nonfree/title/"))
	,m_aimchangeSound(LoadSoundMem("Sound/effect/nonfree/aimchange.ogg"))
	,m_mousePosJustBefore(GetMousePointVector2D())
	,m_selectItem(SelectItem::e_stageSelect)
	,m_frame(0)
	,m_nextScene(nullptr)
	,m_reqInfo(nullptr)
{
	//当たり判定図形の用意
	for(size_t i=0;i<SelectItem::COUNTER;i++){
		m_hitJudgeShapeVec[i]=MakeHexagon(strPos[i],80.0f);
	}
	//bgm再生
	PlaySoundMem(m_bgm,DX_PLAYTYPE_LOOP,TRUE);
}

TitleScene::~TitleScene(){
	//グラフィック解放
	DeleteGraphEX(m_backPic);
	DeleteFontToHandleEX(m_itemFont);
	//サウンド解放
	StopSoundMem(m_bgm);
	DeleteSoundMem(m_bgm);
	DeleteSoundMem(m_aimchangeSound);
}

int TitleScene::thisCalculate(){
	//フレーム数の更新
	m_frame++;

	//特殊な入力情報の取得
	const Vector2D mousePos=GetMousePointVector2D();//マウス位置
	const Vector2D stickInput=analogjoypad_get(DX_INPUT_PAD1);//アナログスティックの傾き

	//選択項目の更新
	const size_t beforeSelectItem=m_selectItem;//効果音を鳴らすかの判定に用いる
	if((m_mousePosJustBefore-mousePos).sqSize()>4.0f){
		//マウスが大きく動いたら、マウスの位置をもとに更新
		for(size_t i=0;i<SelectItem::COUNTER;i++){
			if(m_hitJudgeShapeVec[i]->VJudgePointInsideShape(mousePos)){
				//図形全てに点の内部判定を行い、内部にあった図形に対応する項目に変更する。
				//どの図形にも対応しなかったら変更はしない
				m_selectItem=static_cast<SelectItem::Kind>(i);//変更
				break;
			}
		}
	}
/*	else if(stickInput!=Vector2D()){
		//アナログスティックを動かしていれば、その向きをもとに更新(やめた、不安定すぎる)
		Vector2D backVec,nextVec;//項目がもどる・すすむ入力方向
		if(m_selectItem==0){
			//先頭項目の時は、先頭項目と終端項目を結ぶベクトルの逆方向をbackVecとする
			backVec=-(m_hitJudgeShapeVec[(m_selectItem+SelectItem::COUNTER-1)%SelectItem::COUNTER]->GetPosition()-m_hitJudgeShapeVec[m_selectItem]->GetPosition());
		} else{
			backVec=m_hitJudgeShapeVec[(m_selectItem+SelectItem::COUNTER-1)%SelectItem::COUNTER]->GetPosition()-m_hitJudgeShapeVec[m_selectItem]->GetPosition();
		}
		if(m_selectItem+1==SelectItem::COUNTER){
			//終端項目の時は、終端項目と先端項目を結ぶベクトルの逆方向をnextVecとする
			nextVec=-(m_hitJudgeShapeVec[(m_selectItem+1)%SelectItem::COUNTER]->GetPosition()-m_hitJudgeShapeVec[m_selectItem]->GetPosition());
		} else{
			nextVec=m_hitJudgeShapeVec[(m_selectItem+1)%SelectItem::COUNTER]->GetPosition()-m_hitJudgeShapeVec[m_selectItem]->GetPosition();
		}
		//入力方向がbackVec,nextVecに近い方向かどうかで入力を決める
		//許容差は60度
		if(stickInput.dot(backVec)/stickInput.size()/backVec.size()>(float)std::cos(M_PI/3)){
			m_selectItem=static_cast<SelectItem::Kind>((m_selectItem+SelectItem::COUNTER-1)%SelectItem::COUNTER);
		} else if(stickInput.dot(nextVec)/stickInput.size()/nextVec.size()>(float)std::cos(M_PI/3)){
			m_selectItem=static_cast<SelectItem::Kind>((m_selectItem+1)%SelectItem::COUNTER);
		}
	}
//*/
	else{
		//そうでない場合は、キー入力で行う
		if(keyboard_get(KEY_INPUT_UP)==1 || keyboard_get(KEY_INPUT_LEFT)==1){
			m_selectItem=static_cast<SelectItem::Kind>((m_selectItem+SelectItem::COUNTER-1)%SelectItem::COUNTER);
		} else if(keyboard_get(KEY_INPUT_DOWN)==1 || keyboard_get(KEY_INPUT_RIGHT)==1){
			m_selectItem=static_cast<SelectItem::Kind>((m_selectItem+1)%SelectItem::COUNTER);
		}
	}
	if(beforeSelectItem!=m_selectItem){
		//選択項目の変更が起きているならば効果音再生
		PlaySoundMem(m_aimchangeSound,DX_PLAYTYPE_BACK,TRUE);
	}

	//遷移入力処理
	if(keyboard_get(KEY_INPUT_Z)==1
		|| (mouse_get(MOUSE_INPUT_LEFT)==1 && m_hitJudgeShapeVec[m_selectItem]->VJudgePointInsideShape(mousePos))
		)
	{
		//決定キー入力か、ボタンの上で左クリック
		PlaySoundMem(GeneralPurposeResourceManager::decideSound,DX_PLAYTYPE_BACK,TRUE);//効果音再生
		return m_selectItem;
	}

	//マウス位置の更新
	m_mousePosJustBefore=mousePos;

	return SelectItem::COUNTER;//SelectItem::COUNTERは現状維持
}

void TitleScene::thisDraw()const{
	//背景の描画
	DrawGraph(0,0,m_backPic,TRUE);
	//バージョン情報
	const std::string VERSION_STRING="- GAME^3 9th Trial Edition -";
	const int verX=CommonConstParameter::gameResolutionX-GetDrawStringWidthToHandle(VERSION_STRING.c_str(),VERSION_STRING.size(),m_itemFont);
	const int verY=CommonConstParameter::gameResolutionY-GetFontSizeToHandle(m_itemFont);
	DrawStringToHandle(verX,verY,VERSION_STRING.c_str(),GetColor(0,0,0),m_itemFont);
	//項目の描画
	for(size_t i=0;i<SelectItem::COUNTER;i++){
		unsigned int inColor,frameColor,fontColor;
		int strDy=0;
		if(i!=m_selectItem){
			inColor=GetColor(224,224,224);
			frameColor=GetColor(32,32,32);
			fontColor=GetColor(0,0,0);
		} else{
			inColor=GetColor(32,32,32);
			frameColor=GetColor(224,224,224);
			fontColor=GetColor(255,255,255);
			strDy=std::abs((int)(std::cos(M_PI*2*m_frame/120)*5.0));
		}
		m_hitJudgeShapeVec[i]->Draw(Vector2D(),inColor,TRUE);
		m_hitJudgeShapeVec[i]->Draw(Vector2D(),frameColor,FALSE,3.0f);
		DrawStringCenterBaseToHandle((int)strPos[i].x,(int)strPos[i].y+strDy,SelectItem::GetString(static_cast<SelectItem::Kind>(i)).c_str(),fontColor,m_itemFont,true);
	}
}

int TitleScene::Calculate(){
	int index;
	if(m_nextScene.get()==nullptr){
		//今指している状態がタイトル画面である時
		index=thisCalculate();//更新処理
		//遷移処理
		switch(index){
		case(SelectItem::e_gameFinish):
			//前のクラスにもどる。
			return 1;
		case(SelectItem::e_stageSelect):
			//ステージセレクト画面へ
			m_nextScene=std::shared_ptr<GameScene>(new StageSelectScene(&m_reqInfo));
			break;
		case(SelectItem::e_demo):
			//デモ画面へ
			return 1;
			break;
		case(SelectItem::e_tutorial):
			//チュートリアル画面へ
			return 1;
			break;
		case(SelectItem::e_tutorial_2):
			//チュートリアル画面2へ
			return 1;
			break;
		case(SelectItem::COUNTER):
			//現状維持
			break;
		default:
			break;
		}
	} else{
		//それ以外の画面の状態である時
		index=m_nextScene->Calculate();//更新処理
		//遷移処理
		switch(index){
		case(-1):
			//前のクラスから強制終了しろと伝わってきた
			return -1;
		case(-2):
			//前のクラスからこのクラスに戻ると伝わってきた
			m_nextScene=std::shared_ptr<GameScene>(nullptr);
			m_reqInfo=std::shared_ptr<MainControledGameScene::RequiredInfoToMakeClass>(nullptr);//次のクラスをどうするかという情報も消しておく
			break;
		}
	}
	return 0;
}

void TitleScene::Draw()const{
	if(m_nextScene.get()==nullptr){
		//このクラスの処理
		thisDraw();
	} else{
		//他クラスの処理
		m_nextScene->Draw();
	}
}

std::shared_ptr<MainControledGameScene> TitleScene::VGetNextMainControledScene()const{
	switch(m_selectItem){
	case(SelectItem::e_stageSelect):
		if(m_reqInfo.get()==nullptr){
			//次のクラスが作れない場合はnullptrを返す
		} else{
			//情報があれば、物は作れる
			if(m_reqInfo->GetKind()==RequiredInfoToMakeClass::e_battleScene){
				const BattleScene::RequiredInfoToMakeBattleScene *info=dynamic_cast<const BattleScene::RequiredInfoToMakeBattleScene *>(m_reqInfo.get());
				if(info!=nullptr){
					return std::shared_ptr<MainControledGameScene>(new BattleScene(info->m_stagename.c_str()));
				}
			}
		}
		break;
	case(SelectItem::e_demo):
		return std::shared_ptr<MainControledGameScene>(new DemoScene());
		break;
	case(SelectItem::e_tutorial):
		return std::shared_ptr<MainControledGameScene>(new TutorialScene("tutorial"));
		break;
	case(SelectItem::e_tutorial_2):
		return std::shared_ptr<MainControledGameScene>(new TutorialScene("tutorial_2"));
		break;
	}
	return std::shared_ptr<MainControledGameScene>(nullptr);
}
