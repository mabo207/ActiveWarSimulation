#include"SelectSubmissionScene.h"
#include"DxLib.h"
#include"BattleSceneData.h"
#include"CommonConstParameter.h"
#include"GeneralPurposeResource.h"
#include"input.h"

#include"SwitchUnitScene.h"

//サブミッション一覧
#include"ArcherAttackDistance.h"
#include"MageAttackingOpponent.h"
#include"ArmerPosition.h"
#include"HealerPosition.h"
#include"ProtectFriend.h"
#include"IntensiveAttack.h"

//--------------------SelectSubmissionScene-----------------
const std::array<MyPolygon,SelectSubmissionScene::submissionCount> SelectSubmissionScene::submissionHitJudgeShape={
	MyPolygon::CreateRectangle(Vector2D(80.0f,40.0f),Vector2D(800.0f,460.0f),Shape::Fix::e_ignore)
	,MyPolygon::CreateRectangle(Vector2D(1040.0f,40.0f),Vector2D(800.0f,460.0f),Shape::Fix::e_ignore)
	,MyPolygon::CreateRectangle(Vector2D(80.0f,580.0f),Vector2D(800.0f,460.0f),Shape::Fix::e_ignore)
	,MyPolygon::CreateRectangle(Vector2D(1040.0f,580.0f),Vector2D(800.0f,460.0f),Shape::Fix::e_ignore)
};

SelectSubmissionScene::SelectSubmissionScene(const std::shared_ptr<BattleSceneData> &battleSceneData)
	:BattleSceneElement(SceneKind::e_selectSubmission)
	,m_battleSceneData(battleSceneData)
	,m_selectIndex(0)
{
	m_submissionList[0]=std::shared_ptr<SubmissionRuleBase>(new ArcherAttackDistance());
	m_submissionList[1]=std::shared_ptr<SubmissionRuleBase>(new ArmerPosition());
	m_submissionList[2]=std::shared_ptr<SubmissionRuleBase>(new HealerPosition());
	m_submissionList[3]=std::shared_ptr<SubmissionRuleBase>(new MageAttackingOpponent());
}

SelectSubmissionScene::~SelectSubmissionScene(){}

int SelectSubmissionScene::thisCalculate(){
	//サブミッションの選択
	const Vector2D mousePos=GetMousePointVector2D();
	for(size_t i=0;i<submissionCount;i++){
		if(submissionHitJudgeShape[i].VJudgePointInsideShape(mousePos)){
			m_selectIndex=i;
			break;
		}
	}
	//サブミッション決定判定
	if(keyboard_get(KEY_INPUT_Z)==1 || mouse_get(MOUSE_INPUT_LEFT)==1){
		//決定ボタンを押した
		if(submissionHitJudgeShape[m_selectIndex].VJudgePointInsideShape(mousePos)){
			//マウスがちゃんと項目に入っている
			PlaySoundMem(GeneralPurposeResource::decideSound,DX_PLAYTYPE_BACK,TRUE);//決定音を鳴らす
			return SceneKind::e_switch;//次のシーンへ
		}
	}
	return SceneKind::e_selectSubmission;
}

void SelectSubmissionScene::thisDraw()const{
	//バトル画面の描画
	m_battleSceneData->DrawField();//フィールドの描画
	m_battleSceneData->DrawUnit(true,std::set<const Unit *>{});//ユニットの描画
	m_battleSceneData->DrawHPGage();//全ユニットのHPゲージの描画
	//m_battleSceneData->DrawOrder(std::set<const BattleObject*>{});//ユニットのオーダー順番を描画(やるとm_operatedUnitを初期化してない影響でバグるのでやらない)
	//半透明黒描画
	{
		int mode,pal;
		GetDrawBlendMode(&mode,&pal);
		SetDrawBlendMode(DX_BLENDMODE_ALPHA,128);
		DrawBox(0,0,CommonConstParameter::gameResolutionX,CommonConstParameter::gameResolutionY,GetColor(0,0,0),TRUE);
		SetDrawBlendMode(mode,pal);
	}
	//サブミッションの描画
	for(size_t i=0;i<submissionCount;i++){
		//下地の描画
		const Vector2D pos=submissionHitJudgeShape[i].GetPosition();
		submissionHitJudgeShape[i].Draw(pos,Vector2D(),GetColor(64,192,64),TRUE,0.0f);
		if(i==m_selectIndex){
			//選択している項目には赤枠線
			submissionHitJudgeShape[i].Draw(pos,Vector2D(),GetColor(196,128,128),FALSE,10.0f);
		} else{
			//選択していない項目には灰黄色枠線
			submissionHitJudgeShape[i].Draw(pos,Vector2D(),GetColor(128,128,64),FALSE,10.0f);
		}
		//サブミッションの内容の描画
		DrawStringNewLineToHandle((int)pos.x,(int)pos.y,800,440,m_submissionList[i]->GetSubmissionExplanation().c_str(),GetColor(255,255,255),GeneralPurposeResource::popLargeFont,4);
	}
}

int SelectSubmissionScene::UpdateNextScene(int index){
	switch(index){
	case(SceneKind::e_switch):
		//ゲーム開始へ
		//サブミッション設定
		m_battleSceneData->m_scoreObserver->SetSubmissionRule(m_submissionList[m_selectIndex]);
		//次シーン作成
		m_nextScene=std::shared_ptr<BattleSceneElement>(new SwitchUnitScene(m_battleSceneData));
		break;
	}
	return index;
}

void SelectSubmissionScene::ReturnProcess(){
	//特に何もしない
}
