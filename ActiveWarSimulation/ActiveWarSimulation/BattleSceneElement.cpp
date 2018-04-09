#include"BattleSceneElement.h"

//---------------BattleSceneElement-----------------
BattleSceneElement::BattleSceneElement(SceneKind::Kind sceneKind)
	:m_nextScene(nullptr),m_sceneKind(sceneKind){}

BattleSceneElement::~BattleSceneElement(){}

int BattleSceneElement::Calculate(){
	//更新処理
	int index;
	if(m_nextScene.get()==nullptr){
		index=thisCalculate();
	} else{
		index=m_nextScene->Calculate();
	}
	//場面遷移
	int ret=index;
	if(index==0){
		//この場面に戻ってきた場合
		ReturnProcess();
		m_nextScene.reset();
	} else if(index==SceneKind::END){
		//ゲームプレイが終わる時
		m_nextScene.reset();
	} else if(index!=m_sceneKind && m_nextScene.get()==nullptr){
		//このクラス自身の処理からm_nextSceneへ進む場合
		//この書き方だとm_sceneKindが同じである場面に進めない事になるが…問題ないか
		ret=UpdateNextScene(index);
	}
	return ret;
}

void BattleSceneElement::Draw()const{
	//描画処理
	if(m_nextScene.get()==nullptr){
		thisDraw();
	} else{
		m_nextScene->Draw();
	}

}
