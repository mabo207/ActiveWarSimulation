#include"ResearchScene.h"
#include"DxLib.h"
#include"input.h"

//-------------------ResearchScene---------------------
ResearchScene::ResearchScene(std::shared_ptr<BattleSceneData> battleSceneData)
	:BattleSceneElement(SceneKind::e_research)
	,m_palFont(CreateFontToHandle("メイリオ",16,1,-1))
	,m_palBackPic(-1)
	,m_battleSceneData(battleSceneData)
{
	//操作ユニット等の初期化
	UpdatePointer();
}

ResearchScene::~ResearchScene(){
	DeleteFontToHandle(m_palFont);
	DeleteGraph(m_palBackPic);
}

void ResearchScene::UpdatePointer(){
	//ひとまずマウスを使って操作するのを想定する
	m_pointerVec=GetMousePointVector2D();
	//ポインターの位置にいるユニットをm_researchUnitに格納する
	m_researchUnit=m_battleSceneData->GetUnitPointer(m_pointerVec);
}

int ResearchScene::thisCalculate(){
	//ポインターの更新
	UpdatePointer();

	//遷移処理
	if(keyboard_get(KEY_INPUT_F)==1){
		return 0;//マップ調べモード切替ボタン入力で直前場面へ
	}

	return SceneKind::e_research;
}

void ResearchScene::thisDraw()const{
	//フィールドの描画
	m_battleSceneData->DrawField();

	//ユニットの描画
	m_battleSceneData->DrawUnit(true,std::set<const Unit *>{m_battleSceneData->m_operateUnit,m_researchUnit});

	//調べているユニットについての描画
	if(m_researchUnit!=nullptr){
		m_researchUnit->BattleObject::VDraw();
		m_researchUnit->GetHitJudgeShape()->Draw(Vector2D(),GetColor(255,255,255),FALSE);//調べてるよってことを強調
	}

	//操作中ユニットの描画(m_researchUnitとかぶっていたら描画する必要はない)
	if(m_battleSceneData->m_operateUnit!=m_researchUnit){
		m_battleSceneData->m_operateUnit->BattleObject::VDraw();
		Vector2D pos=m_battleSceneData->m_operateUnit->getPos();
		DrawTriangleAA(pos.x-15.0f,pos.y-60.0f,pos.x+15.0f,pos.y-60.0f,pos.x,pos.y-30.0f,GetColor(255,255,0),TRUE);
	}

	//全ユニットのHPゲージの描画
	m_battleSceneData->DrawHPGage();

	//ユニットのオーダー順番を描画
	m_battleSceneData->DrawOrder();

}

int ResearchScene::UpdateNextScene(int index){
	//特に何もしない
	return index;
}

void ResearchScene::ReturnProcess(){
	//特に何もしない
}
