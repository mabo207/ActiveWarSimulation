#include"BattleScene.h"
#include"SwitchUnitScene.h"
#include"TitleScene.h"
#include"input.h"
#include"DxLib.h"

//----------------------BattleScene----------------------
BattleScene::BattleScene(const char *stagename)
	:MainControledGameScene(),m_battleSceneData(new BattleSceneData(stagename))
{
	//bgm再生
	PlaySoundMem(m_battleSceneData->m_mapBGM,DX_PLAYTYPE_LOOP,TRUE);
	//m_sceneDataの初期化、最初はひとまず移動で
	m_sceneData=std::shared_ptr<BattleSceneElement>(new SwitchUnitScene(m_battleSceneData));
}

BattleScene::~BattleScene(){}

int BattleScene::Calculate(){
	//更新
	m_battleSceneData->m_fpsMesuring.Update();//タイマーの更新
	int index;
	if(m_sceneData.get()!=nullptr){
		index=m_sceneData->Calculate();
	} else{
		index=-2;
	}
	//状態遷移
	if(index==BattleSceneElement::SceneKind::END
		|| keyboard_get(KEY_INPUT_DELETE)==60
		)
	{
		//このクラスを監視するクラスにゲームプレイ場面の終了を伝える
		return -2;
	}

	return 0;
}

void BattleScene::Draw()const{
	//描画
	if(m_sceneData.get()!=nullptr){
		m_sceneData->Draw();
	}
}

std::shared_ptr<MainControledGameScene> BattleScene::VGetNextMainControledScene()const{
	//ゲームプレイが終わった時は、タイトル画面へ
	return std::shared_ptr<MainControledGameScene>(new TitleScene());
}
