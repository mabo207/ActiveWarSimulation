#include"input.h"
#include"DxLib.h"
#include"ComputerMoveScene.h"

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

