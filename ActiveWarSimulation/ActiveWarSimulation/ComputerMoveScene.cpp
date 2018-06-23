#include"input.h"
#include"DxLib.h"
#include"ComputerMoveScene.h"

//---------------ComputerMoveScene------------------
const size_t ComputerMoveScene::squareSize=(size_t)Unit::unitCircleSize;

ComputerMoveScene::ComputerMoveScene(std::shared_ptr<BattleSceneData> battleSceneData)
	:MoveScene(battleSceneData)
	,m_xLatticeNum((size_t)(battleSceneData->m_stageSize.x)/squareSize+1)
	,m_yLatticeNum((size_t)(battleSceneData->m_stageSize.y)/squareSize+1)
{
	//格子点の数だけ配列を確保
	m_latticeInShape=std::vector<int>(m_xLatticeNum*m_yLatticeNum,0);
	//操作するユニットが通れない場所に1を格納していく
	//「障害物の中＝中にいないとユニットは通れない」という考え方を用いる
	for(const BattleObject *object:m_battleSceneData->m_field){
		
		if(object==m_battleSceneData->m_operateUnit){
			//操作ユニットは通れるか否かに影響を与えない

		} else if(object->JudgePointInsideShape(m_battleSceneData->m_operateUnit->getPos())){
			//object内部にユニットがいるなら、その外部を通れないようにする
			std::vector<int> info(m_latticeInShape.size(),1);//通れるかの情報を一旦格納する。通れるところのみを0とする。
			object->GetHitJudgeShape()->RecordLatticePointInShape(info,m_xLatticeNum,m_yLatticeNum,squareSize,squareSize,0);
			for(size_t i=0,size=m_latticeInShape.size();i<size;i++){
				if(info[i]==1){
					//通れないと分かった場所を通れなくさせる
					m_latticeInShape[i]=1;
				}
			}
		} else{
			//object外部にユニットがいるなら、その内部は通れないようにする
			object->GetHitJudgeShape()->RecordLatticePointInShape(m_latticeInShape,m_xLatticeNum,m_yLatticeNum,squareSize,squareSize,1);
		}
	}
	//1の隣に2を置く
	auto Input2Only0=[](int &a){if(a==0){ a=2; }};
	for(size_t y=0;y<m_yLatticeNum;y++){
		for(size_t x=0;x<m_xLatticeNum;x++){
			const size_t index=x+y*m_xLatticeNum;
			if(m_latticeInShape[index]==1){
				//全ての1である格子点に隣接する格子点を2にする。
				if(y>0){
					//上の隣接点
					Input2Only0(m_latticeInShape[index-m_xLatticeNum]);
				}
				if(x>0){
					//左の隣接点
					Input2Only0(m_latticeInShape[index-1]);
				}
				if(x+1<m_xLatticeNum){
					//上の隣接点
					Input2Only0(m_latticeInShape[index+1]);
				}
				if(y+1<m_yLatticeNum){
					//上の隣接点
					Input2Only0(m_latticeInShape[index+m_xLatticeNum]);
				}
			}
		}
	}
}

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

void ComputerMoveScene::thisDraw()const{
	//プレイヤーのと同じ描画処理
	MoveScene::thisDraw();

	//デバッグ用描画
	//格子点を描画
	for(size_t i=0,size=m_latticeInShape.size();i<size;i++){
		const int x=(i%m_xLatticeNum)*squareSize,y=(i/m_xLatticeNum)*squareSize;
		unsigned int color;
		switch(m_latticeInShape[i]){
		case(0):
			color=GetColor(255,255,0);
			break;
		case(1):
			color=GetColor(0,255,255);
			break;
		case(2):
			color=GetColor(255,0,255);
			break;
		}
		DrawCircleAA((float)(x),(float)(y),2,6,color,TRUE);
	}
}
