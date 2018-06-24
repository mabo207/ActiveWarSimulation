#include"input.h"
#include"DxLib.h"
#include"ComputerMoveScene.h"

#include"Circle.h"


//---------------ComputerMoveScene::LatticeDistanceInfo------------------
bool ComputerMoveScene::LatticeDistanceInfo::operator<(const LatticeDistanceInfo &otherobj)const{
	//評価の優先順位は「距離」→「インデックス」の順。近いところから処理していきたいので
	if(this->dist<otherobj.dist){
		return true;
	} else if(this->dist>otherobj.dist){
		return false;
	}
	return (this->index<otherobj.index);
}

bool ComputerMoveScene::LatticeDistanceInfo::operator==(const LatticeDistanceInfo &otherobj)const{
	return (this->index==otherobj.index) && (this->dist==otherobj.dist);
}


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

	//各格子点への距離マップの作成
	const size_t latticeNum=m_latticeInShape.size();
	std::vector<LatticeDistanceInfo> latticeDistanceInfo(latticeNum,LatticeDistanceInfo(latticeNum,latticeNum,-0.1f));//各格子点に進むための最小距離と、どの格子点から移動すればよいかが格納されている。
	std::set<LatticeDistanceInfo> latticeSet;//探索していく格子点のリスト。近い順から探すためにsetを用いる。
	auto SearchUpdate=[&latticeSet,&latticeDistanceInfo](const size_t from,const float dist,const size_t x,const size_t y,const size_t width,const size_t height,const std::vector<int> &latticeInShape){
		//latticeDistanceInfoを更新し、latticeSetに格納する。
		if(x>=0 && x<width && y>=0 && y<height){
			size_t index=x+y*width;
			int po=latticeInShape[index];
			if(latticeInShape[index]!=1 && (latticeDistanceInfo[index].dist<0.0f || dist<latticeDistanceInfo[index].dist)){
				//進入不可ではなく、なおかつ今見つけたルートよりも短いルートが発見されていなければ
				//距離マップの更新を行う
				LatticeDistanceInfo info(index,from,dist);
				latticeDistanceInfo[index]=info;
				if(latticeInShape[index]==0){
					//探索する格子点の追加は、m_latticeInShape[index]が0（すなわち通れる場所）を表す
					latticeSet.insert(info);
				}
			}
		}
	};
	//初期位置に近い4点を探す
	const size_t initX=(size_t)(m_battleSceneData->m_operateUnit->getPos().x/squareSize),initY=(size_t)(m_battleSceneData->m_operateUnit->getPos().y/squareSize);
	SearchUpdate(latticeNum,(m_battleSceneData->m_operateUnit->getPos()-Vector2D((float)(initX*squareSize),(float)(initY*squareSize))).size(),initX,initY,m_xLatticeNum,m_yLatticeNum,m_latticeInShape);
	SearchUpdate(latticeNum,(m_battleSceneData->m_operateUnit->getPos()-Vector2D((float)((initX+1)*squareSize),(float)(initY*squareSize))).size(),initX+1,initY,m_xLatticeNum,m_yLatticeNum,m_latticeInShape);
	SearchUpdate(latticeNum,(m_battleSceneData->m_operateUnit->getPos()-Vector2D((float)(initX*squareSize),(float)((initY+1)*squareSize))).size(),initX,initY+1,m_xLatticeNum,m_yLatticeNum,m_latticeInShape);
	SearchUpdate(latticeNum,(m_battleSceneData->m_operateUnit->getPos()-Vector2D((float)((initX+1)*squareSize),(float)((initY+1)*squareSize))).size(),initX+1,initY+1,m_xLatticeNum,m_yLatticeNum,m_latticeInShape);
	//通れる可能性のある部分を全て探索
	const int dx[8]={-1,0,1,-1,1,-1,0,1};
	const int dy[8]={-1,-1,-1,0,0,1,1,1};
	const float slantingdist=Vector2D((float)(squareSize),(float)(squareSize)).size();
	const float horizondist=(float)squareSize;
	const float dis[8]={slantingdist,horizondist,slantingdist,horizondist,horizondist,slantingdist,horizondist,slantingdist};
	while(!latticeSet.empty()){
		std::set<LatticeDistanceInfo>::iterator beginIt=latticeSet.begin();
		LatticeDistanceInfo beginInfo(*beginIt);
		latticeSet.erase(beginIt);//探索を今後行わないのでsetから取り除いておく
		//周囲8個の格子点について更新を行う
		const size_t x=beginInfo.index%m_xLatticeNum,y=beginInfo.index/m_xLatticeNum;
		for(size_t i=0;i<8;i++){
			SearchUpdate(beginInfo.index,beginInfo.dist+dis[i],x+dx[i],y+dy[i],m_xLatticeNum,m_yLatticeNum,m_latticeInShape);
		}
	}


	distvec=latticeDistanceInfo;//デバッグ用

	//目標地点の決定
	const std::pair<size_t,Vector2D> targetPoint=DecideTargetPoint(latticeDistanceInfo);//ここはAIの行動傾向によって異なるので

	//ルートの選定と格納
	m_latticeRoute.push_back(targetPoint.second);//最終目標の格子点についた後に向かう位置を最初に格納しておく（これは恐らくユニット内部で入れない）
	for(size_t point=targetPoint.first;point<latticeNum;point=latticeDistanceInfo[point].from){
		//latticeDistanceInfo[point].fromを辿っていけば最短距離となる
		Vector2D v;
		if(point<latticeNum){
			//格子点が存在するならその位置に
			v=Vector2D((float)((point%m_xLatticeNum)*squareSize),(float)((point/m_xLatticeNum)*squareSize));
		} else{
			//格子点が存在しないならその場に(point==latticeNumの時のみ。これは「操作ユニットの位置にいること」を表す)
			v=m_battleSceneData->m_operateUnit->getPos();
		}
		m_latticeRoute.insert(m_latticeRoute.begin(),v);
	}

}

ComputerMoveScene::~ComputerMoveScene(){}

std::pair<size_t,Vector2D> ComputerMoveScene::DecideTargetPoint(const std::vector<LatticeDistanceInfo> &distanceInfo)const{
	const Unit *targetUnit=nullptr;
	for(const Unit *pu:m_battleSceneData->m_unitList){
		if(pu->GetBattleStatus().team!=m_battleSceneData->m_operateUnit->GetBattleStatus().team){
			if(targetUnit==nullptr){
				targetUnit=pu;
			} else if((pu->getPos()-m_battleSceneData->m_operateUnit->getPos()).sqSize()<(targetUnit->getPos()-m_battleSceneData->m_operateUnit->getPos()).sqSize()){
				targetUnit=pu;
			}
		}
	}
	//格子点探し
	const size_t vecSize=m_latticeInShape.size();
	if(targetUnit!=nullptr){
		//狙うユニットを決めたら、そのユニットを攻撃できる格子点を探す
		//貪欲法でいく
		Unit copiedUnit(*dynamic_cast<Unit *>(m_battleSceneData->m_operateUnit->VCopy().get()));//作業用にコピーを作る
		size_t target=vecSize;
		for(size_t y=0;y<m_yLatticeNum;y++){
			for(size_t x=0;x<m_xLatticeNum;x++){
				const size_t index=x+y*m_xLatticeNum;
				if(distanceInfo[index].dist>=0.0f && (target>=vecSize || distanceInfo[index]<distanceInfo[target])){
					//既に別のより近い目標地点候補が存在するなら新しい目標地点になる事はない
					const Vector2D pos((float)(x*squareSize),(float)(y*squareSize));
					copiedUnit.Warp(pos);
					if(copiedUnit.JudgeAttackable(targetUnit)){
						//攻撃可能ならこの位置を暫定の目標地点とする
						target=index;
					}
				}
			}
		}
		if(target!=vecSize){
			return std::pair<size_t,Vector2D>(target,targetUnit->getPos());
		}
	} else{
		//狙うユニットが無いなら、その場で待機する
	}
	return std::pair<size_t,Vector2D>(vecSize,m_battleSceneData->m_operateUnit->getPos());
}

Vector2D ComputerMoveScene::CalculateInputVec()const{
	Vector2D moveVec;
	//コンピュータ操作時、AIが方向を決める
	//ターン開始から1秒経ったら動く
	if(m_battleSceneData->m_fpsMesuring.GetProcessedTime()>1.0){
/*		//ひとまず最近傍ユニットに単純に近づく
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
//*/
		//m_latticeRouteの先頭に向かって動く
		if(!m_latticeRoute.empty()){
			moveVec=m_latticeRoute.front()-m_battleSceneData->m_operateUnit->getPos();
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
	if(processedTime>1.0){
		//1秒経ったら行動する
		if(JudgeAttackCommandUsable()){
			//攻撃対象が存在し、OPが足りている場合のみ攻撃処理を行う
			//FinishUnitOperation();//行動終了処理(あとで)
			return SceneKind::e_attackNormal;//攻撃場面へ
		} else if(m_battleSceneData->m_operateUnit->GetBattleStatus().OP<2.0f
//			|| processedTime>10.0
			|| keyboard_get(KEY_INPUT_Q)==1
			|| (moveSqLength<0.1f && m_latticeRoute.size()<2 && processedTime>2.0))
		{
			//移動できなくなったら、または10秒経ったら、またもう進む場所がないまたは最後の1点にたどり着かず移動距離も少ない場合は待機
			FinishUnitOperation();
			return 0;
		} else if(!m_latticeRoute.empty() && m_battleSceneData->m_operateUnit->JudgePointInsideShape(m_latticeRoute.front())){
			//m_latticeRouteの先頭の点がユニットの当たり判定図形に入ったらそこまでは簡単に動けるということなので移動先を変える
			m_latticeRoute.erase(m_latticeRoute.begin());
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
/*
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
//*/
		if(distvec[i].dist<0.0f){
			color=GetColor(0,0,255);
		} else{
			int level=(int)distvec[i].dist/3;
			color=GetColor(level%256,255-level%256,0);
		}
		DrawCircleAA((float)(x),(float)(y),2,6,color,TRUE);
	}
	//ルートを描画
	if(!m_latticeRoute.empty()){
		DrawCircleAA(m_latticeRoute.front().x,m_latticeRoute.front().y,5,15,GetColor(255,255,0),FALSE);
		for(size_t i=0,size=m_latticeRoute.size();i+1<size;i++){
			DrawCircleAA(m_latticeRoute[i+1].x,m_latticeRoute[i+1].y,5,15,GetColor(255,255,0),FALSE);
			DrawLineAA(m_latticeRoute[i].x,m_latticeRoute[i].y,m_latticeRoute[i+1].x,m_latticeRoute[i+1].y,GetColor(255,255,0));
		}
	}
}
