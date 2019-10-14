#include"BattleScene.h"
#include"input.h"
#include"DxLib.h"
#include"CommonConstParameter.h"
#include"BGMManager.h"

#include"SwitchUnitInitOrderScene.h"
#include"SubmissionSwitchUnitScene.h"

#include"StageSelectScene.h"
#include"CreditScene.h"

//----------------------BattleScene::BattleSceneFactory----------------------
BattleScene::BattleSceneFactory::BattleSceneFactory(const std::string &stageDirName,const std::string &title,const StageLevel level)
	:SceneFactory()
	,m_stageDirName(stageDirName)
	,m_title(title)
	,m_level(level)
{}

BattleScene::BattleSceneFactory::~BattleSceneFactory(){}

std::shared_ptr<GameScene> BattleScene::BattleSceneFactory::CreateIncompleteScene()const{
	return std::shared_ptr<BattleScene>(new BattleScene(m_stageDirName,m_title,m_level));
}

//----------------------BattleScene----------------------
const int BattleScene::resetInterval=60;

BattleScene::BattleScene(std::shared_ptr<BattleSceneData> battleSceneData)
	:GameScene()
	,m_resetFlag(false)
	,m_resetFrame(0)
	,m_battleSceneData(battleSceneData)
{}

BattleScene::BattleScene(const std::string &stageDirName,const std::string &titleName,const StageLevel stageLevel)
	:BattleScene(std::shared_ptr<BattleSceneData>(new BattleSceneData(stageDirName,titleName,stageLevel))){}

BattleScene::~BattleScene(){
	//m_battleSceneDataにあるシーン終了時に行う処理群の一括処理をする
	m_battleSceneData->RunSceneEndProcess();
	//BGMを止める
	if(BGMManager::s_instance.has_value()){
		BGMManager::s_instance->Stop();
	}
}

void BattleScene::InitCompletely(){
	//m_battleSceneDataはグラフィックとともに初期化するべきデータが多いので、ここで初期化処理は行わずにコンストラクタで全て初期化する
}

void BattleScene::Activate(){
	//bgm再生
	//m_battleSceneData->m_mapBGM.SetAndPlay(DX_PLAYTYPE_LOOP,TRUE);
	if(BGMManager::s_instance.has_value()){
		BGMManager::s_instance->PlayWithCopy(m_battleSceneData->m_mapBGM);
	}
	//m_sceneDataの初期化、最初はユニット切り替え(m_battleSceneDataの初期化が終わった状態でこの処理はしたいのでActivate内で行う)
	m_sceneData=VGetSwitchUnitScene();
}

std::shared_ptr<BattleSceneElement> BattleScene::VGetSwitchUnitScene()const{
	//すごーい汚いけど、デモでない場合はここでサブミッション選択場面を生成する。
	//return std::shared_ptr<BattleSceneElement>(new SwitchUnitInitOrderScene(m_battleSceneData));
	return std::shared_ptr<BattleSceneElement>(new SubmissionSwitchUnitScene(m_battleSceneData));
}

void BattleScene::ResetGame(){
	m_battleSceneData=std::shared_ptr<BattleSceneData>(new BattleSceneData(m_battleSceneData->m_stageDirName,m_battleSceneData->m_stageTitleName,m_battleSceneData->m_stageLevel));//バトルデータを変える
	m_sceneData=VGetSwitchUnitScene();//クラスを変える
	if(BGMManager::s_instance.has_value()){
		//BGMを最初から再生しなおす。そのためには、一旦BGMを止めてから、Play()を呼ぶ必要がある
		BGMManager::s_instance->Stop();
		BGMManager::s_instance->PlayWithCopy(m_battleSceneData->m_mapBGM);
	}
}

int BattleScene::Calculate(){
	//更新
	if(!m_resetFlag){
		//ゲーム処理を行う
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
		} else if(index==BattleSceneElement::SceneKind::e_gameReset){
			//リセット場面に移行する
			m_resetFlag=true;
		}
	} else{
		//リセット場面中
		m_resetFrame++;
		if(m_resetFrame==resetInterval/2){
			//リセット処理を行う
			ResetGame();
		} else if(m_resetFrame==resetInterval){
			//リセット場面を終了する
			m_resetFrame=0;
			m_resetFlag=false;
		}
	}

	return 0;
}

void BattleScene::Draw()const{
	//描画
	if(m_sceneData.get()!=nullptr){
		m_sceneData->Draw();
	}
	//リセット中は暗転するようにする
	if(m_resetFlag){
		int mode,pal;
		GetDrawBlendMode(&mode,&pal);
		SetDrawBlendMode(DX_BLENDMODE_ALPHA,255*m_resetFrame*(resetInterval-m_resetFrame)/(resetInterval/2)/(resetInterval/2));
		DrawBox(0,0,CommonConstParameter::gameResolutionX,CommonConstParameter::gameResolutionY,GetColor(0,0,0),TRUE);
		SetDrawBlendMode(mode,pal);
	}
}

std::shared_ptr<GameScene> BattleScene::VGetNextScene(const std::shared_ptr<GameScene> &thisSharedPtr)const{
	//ゲームプレイが終わった時は、基本的にはステージセレクト画面へ
	if(!m_battleSceneData->m_gotoCredit){
		const auto stageSelectFactory=std::make_shared<StageSelectScene::StageSelectSceneFactory>();
		return CreateFadeOutInSceneCompletely(thisSharedPtr,stageSelectFactory,15,15);
	} else{
		//最終ステージクリア時のみ、クレジットへ
		const auto creditFactory=std::make_shared<CreditScene::CreditSceneFactory>();
		return CreateFadeOutInSceneCompletely(thisSharedPtr,creditFactory,15,15);
	}
}
