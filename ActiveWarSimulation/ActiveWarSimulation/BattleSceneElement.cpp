#include"BattleSceneElement.h"

//---------------BattleSceneElement-----------------
BattleSceneElement::BattleSceneElement(SceneKind::Kind sceneKind)
	:m_nextScene(nullptr),m_sceneKind(sceneKind){}

BattleSceneElement::~BattleSceneElement(){}

BattleSceneElement::SceneKind::Kind BattleSceneElement::GetTailSceneKind()const{
	//末尾にある場面クラスのm_sceneKindを得る
	if(m_nextScene.get()!=nullptr){
		return m_nextScene->GetTailSceneKind();
	} else{
		return GetSceneKind();
	}
}

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
	if(index==SceneKind::END){
		//ゲームプレイが終わる時
		m_nextScene.reset();
	} else if(m_nextScene.get()==nullptr){
		//このクラス内で処理をしていた場合
		if(index!=m_sceneKind){
			//このクラス自身の処理からm_nextSceneへ進む場合
			//この書き方だとm_sceneKindが同じである場面に進めない事になるが…問題ないか
			ret=UpdateNextScene(index);
		} else if(index==0){
			//直前場面に戻る場合
			//Calaulate()が0を返せば何もせずとも直前に戻る
		}
	} else{
		//m_nextScene以降で処理をしていた場合
		if(index!=m_nextScene->GetTailSceneKind()){
			if(index==0 || index==m_sceneKind){
				//この場面に戻ってきた場合
				ReturnProcess();
				m_nextScene.reset();
				ret=m_sceneKind;//返り値はこのクラス自身の種類値にする
			} else{
				//次のクラスの処理からindexに当たる場面まで一気に戻る場合
				ReturnProcess();
				m_nextScene.reset();
			}
		}
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
