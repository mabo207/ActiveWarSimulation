#include"BattleScene.h"
#include"MoveScene.h"

//----------------------BattleScene----------------------
BattleScene::BattleScene(const char *stagename)
	:GameScene(),m_battleSceneData(new BattleSceneData(stagename))
{
	//m_sceneDataの初期化、最初はひとまず移動で
	m_sceneData=std::shared_ptr<GameScene>(new MoveScene(m_battleSceneData));
}

BattleScene::~BattleScene(){}

int BattleScene::Calculate(){
	//更新
	int index;
	if(m_sceneData.get()!=nullptr){
		index=m_sceneData->Calculate();
	} else{
		index=1;
	}
	//状態遷移
	if(index!=0){
		//このクラスを監視するクラスにゲームプレイ場面の終了を伝える
		return 1;
	}

	return 0;
}

void BattleScene::Draw()const{
	//描画
	if(m_sceneData.get()!=nullptr){
		m_sceneData->Draw();
	}
}
