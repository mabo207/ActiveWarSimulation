#include"input.h"
#include"DxLib.h"
#include"ComputerMoveScene.h"

//---------------ComputerMoveScene------------------
ComputerMoveScene::ComputerMoveScene(std::shared_ptr<BattleSceneData> battleSceneData)
	:MoveScene(battleSceneData){}

Vector2D ComputerMoveScene::CalculateInputVec()const{
	Vector2D moveVec;
	//コンピュータ操作時、AIが方向を決める
	//ターン開始から1秒経ったらひとまず最近傍ユニットに単純に近づく
	if(m_battleSceneData->m_fpsMesuring.GetProcessedTime()>1.0){
		const Unit *nearestUnit=nullptr;
		for(const Unit *pu:m_battleSceneData->m_unitList){
			if(pu->GetBattleStatus().team!=m_battleSceneData->m_operateUnit->GetBattleStatus().team){
				if(nearestUnit==nullptr){
					nearestUnit=pu;
				} else if((pu->getPos()-m_battleSceneData->m_operateUnit->getPos()).sqSize()<(nearestUnit->getPos()-m_battleSceneData->m_operateUnit->getPos()).sqSize()){
					nearestUnit=pu;
				}
			}
		}
		if(nearestUnit!=nullptr){
			moveVec=nearestUnit->getPos()-m_battleSceneData->m_operateUnit->getPos();
		}
	}
	return moveVec;
}

int ComputerMoveScene::thisCalculate(){
	//m_operateUnitの位置更新
	const Vector2D beforeVec=m_battleSceneData->m_operateUnit->getPos();
	PositionUpdate(CalculateInputVec());
	const float moveSqLength=(beforeVec-m_battleSceneData->m_operateUnit->getPos()).sqSize();
	const double processedTime=m_battleSceneData->m_fpsMesuring.GetProcessedTime();
	if(m_battleSceneData->m_fpsMesuring.GetProcessedTime()>1.0){
		//1秒経ったら行動する
		if(JudgeAttackCommandUsable()){
			//攻撃対象が存在し、OPが足りている場合のみ攻撃処理を行う
			//FinishUnitOperation();//行動終了処理(あとで)
			return SceneKind::e_attackNormal;//攻撃場面へ
		} else if(m_battleSceneData->m_operateUnit->GetBattleStatus().OP<2.0f || processedTime>10.0 || (moveSqLength<0.1f && processedTime>2.0)){
			//移動できなくなったら、または10秒経ったら、また移動距離が少ない場合は待機
			FinishUnitOperation();
			return 0;
		}
	}
	return SceneKind::e_move;
}
