#include"input.h"
#include"DxLib.h"
#include"ComputerMoveScene.h"
#include"CommonConstParameter.h"

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

//---------------ComputerMoveScene::AttackFailedInfo------------------
ComputerMoveScene::AttackFailedInfo::AttackFailedInfo()
	:m_count(0),m_frame(0)
{}

bool ComputerMoveScene::AttackFailedInfo::JudgeAttackProcessProceed()const{
	return (m_count==0 || m_frame>6);//6フレームは移動に当てる
}

bool ComputerMoveScene::AttackFailedInfo::JudgeRetry()const{
	return (m_count<1);//やり直し回数は1回
}

void ComputerMoveScene::AttackFailedInfo::RetryProcess(){
	m_count++;
	m_frame=0;
}

//---------------ComputerMoveScene------------------
const size_t ComputerMoveScene::squareSize=CommonConstParameter::unitCircleSize;

ComputerMoveScene::ComputerMoveScene(std::shared_ptr<BattleSceneData> battleSceneData)
	:MoveScene(battleSceneData)
	,m_xLatticeNum((size_t)(battleSceneData->m_stageSize.x)/squareSize+1)
	,m_yLatticeNum((size_t)(battleSceneData->m_stageSize.y)/squareSize+1)
	,m_actionWaiting(false)
	,m_nextScene(SceneKind::e_move)
	,m_aimChangeFrame(0)
	,m_waitingFrame(0)
	,m_attackFailedInfo()
{
	//格子点の数だけ配列を確保
	m_latticeInShape=std::vector<int>(m_xLatticeNum*m_yLatticeNum,0);
	//操作するユニットが通れない場所に1を格納していく
	//「障害物の中＝中にいないとユニットは通れない」という考え方を用いる
	for(const BattleObject *object:m_battleSceneData->m_field){
		if(object==m_battleSceneData->m_operateUnit){
			//操作ユニットは通れるか否かに影響を与えない

		} else if(object->GetFix()==Shape::Fix::e_ignore){
			//当たり判定のないオブジェクトは通れるか否かに影響を与えない

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
				ImpassableLatticeInShape(x,y);//m_latticeInShape[index]を1とする処理も含むが、元々1なので問題ない
			}
		}
	}

	//通る格子点の計算
	CalculateLatticeRoute();
}

ComputerMoveScene::~ComputerMoveScene(){}

Vector2D ComputerMoveScene::CalculateLatticePointPos(size_t x,size_t y)const{
	return Vector2D((float)(x*squareSize),(float)(y*squareSize));
}

Vector2D ComputerMoveScene::CalculateLatticePointPos(size_t index)const{
	return CalculateLatticePointPos(index%m_xLatticeNum,index/m_xLatticeNum);
}

std::pair<std::pair<size_t,Vector2D>,Unit *> ComputerMoveScene::DecideTargetPoint(const std::vector<LatticeDistanceInfo> &distanceInfo)const{
	//狙うユニットを決める
	Unit *targetUnit=nullptr;//この関数内では変更されない
	float point=-50000.0f;
	for(Unit *pu:m_battleSceneData->m_unitList){
		if(m_battleSceneData->m_operateUnit->GetBattleStatus().weapon->JudgeWeild(m_battleSceneData->m_operateUnit,pu)){
			//攻撃対象となるユニットを見つけたら
			if(targetUnit==nullptr){
				//まだ候補がいない場合は
				targetUnit=pu;//そのユニットを候補にする
				point=CalculateEvaluate(targetUnit,distanceInfo);//そのユニットの攻撃対象としての評価値を計算する
			} else{
				//既に候補がいる場合は評価値を計算する
				const float tmpPoint=CalculateEvaluate(pu,distanceInfo);
				if(tmpPoint>point){
					//評価値が高ければいま検索しているユニットを攻撃対象とする
					targetUnit=pu;
					point=tmpPoint;
				}
			}
		}
	}
	//格子点探し
	const size_t vecSize=m_latticeInShape.size();
	Vector2D targetPointVec=m_battleSceneData->m_operateUnit->getPos();//targetに相当する座標まで到達した際にどの座標に向かって動くか。初期値は「その場で待機する」
	size_t target=vecSize;
	if(targetUnit!=nullptr){
		//狙うユニットを決めたら、そのユニットを攻撃できる格子点を探す
		//貪欲法でいく
		Unit copiedUnit(*dynamic_cast<Unit *>(m_battleSceneData->m_operateUnit->VCopy().get()));//作業用にコピーを作る
		for(size_t y=0;y<m_yLatticeNum;y++){
			for(size_t x=0;x<m_xLatticeNum;x++){
				const size_t index=x+y*m_xLatticeNum;
				if(distanceInfo[index].dist>=0.0f && (target>=vecSize || distanceInfo[index]<distanceInfo[target])){
					//既に別のより近い目標地点候補が存在するなら新しい目標地点になる事はない
					const Vector2D pos=CalculateLatticePointPos(x,y);
					copiedUnit.Warp(pos);
					if(copiedUnit.JudgeAttackable(targetUnit)){
						//攻撃可能ならこの位置を暫定の目標地点とする
						target=index;
					}
				}
			}
		}
		if(target!=vecSize){
			targetPointVec=targetUnit->getPos();//暫定で最終目標座標が狙っているユニットになる（迎撃型AIであったりするとここが後で変わる）
		}
	} else{
		//狙うユニットが無いなら、その場で待機する
		//(target=vecSize,targetPointVec=m_battleSceneData->m_operateUnit->getPos()のまま)
	}

	//狙うユニットと目的地が決まったので、AIのルールに従ってそこに行くかどうか決める
	switch(m_battleSceneData->m_operateUnit->GetBattleStatus().aitype){
	case(Unit::AIType::e_intercept):
	case(Unit::AIType::e_linkageIntercept):
		//迎撃型AIと連動迎撃型AI
		if(target>=vecSize || point<0.0f){
			//目的地が１回の移動で届かない場合は、その場で待機する
			//pointが0以上である事が１回の移動で届く事の必要十分条件となっている
			target=vecSize;
			targetPointVec=m_battleSceneData->m_operateUnit->getPos();
		}
		break;
	case(Unit::AIType::e_assult):
		//突撃型AI
		//求めた目的地まで進む(targetに変更は無し)
		break;
	}

	//移動するかどうかによるAIの変化処理
	if(target!=vecSize){
		//行動する場合、自分に紐づいている連動迎撃型AIのキャラクタのAIを突撃型に変更する必要がある
		const int group=m_battleSceneData->m_operateUnit->GetBattleStatus().aiGroup;
		for(Unit *pu:m_battleSceneData->m_unitList){
			//全てのユニットに対して検索
			if(pu->GetBattleStatus().aiLinkage.count(group)!=0){
				//group値がpuのaiLinkageに存在していて、かつpuが連動迎撃型AIである場合は、puはm_operateUnitに連動して動くグループであるので突撃型になる
				//puが連動迎撃型であるかどうかの判定は、Unit::BecomeAssultAI()中で判定する。こっちのほうがコーディングのミスは減ると思う。
				pu->BecomeAssultAI();
			}
		}
	}

	return std::pair<std::pair<size_t,Vector2D>,Unit *>(std::pair<size_t,Vector2D>(target,targetPointVec),targetUnit);
}

void ComputerMoveScene::CalculateLatticeDistanceInfo(std::vector<LatticeDistanceInfo> &latticeDistanceInfo)const{
	//事前準備
	const size_t latticeNum=m_latticeInShape.size();//何度も使うので予め計算しておく
	latticeDistanceInfo=std::vector<LatticeDistanceInfo>(latticeNum,LatticeDistanceInfo(latticeNum,latticeNum,-0.1f));//初期化
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

}

void ComputerMoveScene::CalculateLatticeRoute(){
	m_latticeRoute.clear();
	//各格子点への距離マップの作成
	const size_t latticeNum=m_latticeInShape.size();
	std::vector<LatticeDistanceInfo> latticeDistanceInfo;//各格子点に進むための最小距離と、どの格子点から移動すればよいかが格納されている。
	CalculateLatticeDistanceInfo(latticeDistanceInfo);//距離マップ計算関数

	distvec=latticeDistanceInfo;//デバッグ用

	//目標地点の決定
	const std::pair<std::pair<size_t,Vector2D>,Unit *> targetPoint=DecideTargetPoint(latticeDistanceInfo);//ここはAIの行動傾向によって異なるので
	m_targetUnit=targetPoint.second;

	//ルートの選定と格納
	m_latticeRoute.push_back(std::pair<size_t,Vector2D>(latticeNum,targetPoint.first.second));//最終目標の格子点についた後に向かう位置を最初に格納しておく（これは恐らくユニット内部で入れない）
	for(size_t point=targetPoint.first.first;point<latticeNum;point=latticeDistanceInfo[point].from){
		//latticeDistanceInfo[point].fromを辿っていけば最短距離となる
		Vector2D v;
		if(point<latticeNum){
			//格子点が存在するならその位置に
			v=CalculateLatticePointPos(point);
		} else{
			//格子点が存在しないならその場に(point==latticeNumの時のみ。これは「操作ユニットの位置にいること」を表す)
			v=m_battleSceneData->m_operateUnit->getPos();
		}
		m_latticeRoute.insert(m_latticeRoute.begin(),std::pair<size_t,Vector2D>(point,v));
	}

}

Vector2D ComputerMoveScene::CalculateInputVec()const{
	Vector2D moveVec;
	//コンピュータ操作時、AIが方向を決める
	//ターン開始から1秒経ったら動く
	if(m_battleSceneData->m_fpsMesuring.GetProcessedTime()>1.0){
		//m_latticeRouteの先頭に向かって動く
		if(!m_latticeRoute.empty()){
			moveVec=m_latticeRoute.front().second-m_battleSceneData->m_operateUnit->getPos();
		}
	}
	return moveVec;
}

float ComputerMoveScene::CalculateEvaluate(const Unit *punit,const std::vector<LatticeDistanceInfo> &distanceInfo)const{
	//const float sqDistance=-(punit->getPos()-m_battleSceneData->m_operateUnit->getPos()).sqSize();//ユニット間の距離の2乗値。
	//punitに攻撃が届く位置までの距離についての評価値
	//distanceEvaluateの値域
	//	distanceEvaluate=0:届く
	//	distanceEvaluate<=m_operateUnit->GetMoveDistance():届かない
	float distanceEvaluate=-50000.0f;//ひとまず酷い点数をつける
	if(punit->GetHitJudgeShape()->GetType()==Shape::Type::e_circle){
		const Circle *pcircle=dynamic_cast<const Circle *>(punit->GetUnitCircleShape());//作業用のユニット図形(内部)の確保
		if(pcircle!=nullptr){
			Circle c(pcircle->GetPosition(),pcircle->GetR()+m_battleSceneData->m_operateUnit->GetBattleStatus().weapon->GetLength()+squareSize,pcircle->m_fix);//攻撃可能範囲(ただし、位置によっては格子点を検出できないため、squareSizeだけ余分に広い範囲で格子点を探す)
			//攻撃可能範囲の中にある格子点の中で最近点を求める
			const size_t size=distanceInfo.size();
			size_t nearestIndex=size;//これがsizeである間は
			for(size_t i=0;i<size;i++){
				if(m_latticeInShape[i]==0 && distanceInfo[i].dist>=0.0f && c.VJudgePointInsideShape(CalculateLatticePointPos(i)) && (nearestIndex>=size || distanceInfo[i].dist<distanceInfo[nearestIndex].dist)){
					//点iが侵入可能かつ到達可能で、攻撃可能範囲内にあり、尚且つ現在の最近点より近いのであれば
					nearestIndex=i;//最近点の更新
				}
			}
			if(nearestIndex<size){
				//最近点が存在するならば(=何度か動けばpunitに攻撃できるのであれば)まともな点数をつける
				if(m_battleSceneData->m_operateUnit->GetMoveDistance()<distanceInfo[nearestIndex].dist){
					//1ターンの間にたどり着ける場所でないなら、距離が遠くなるほどに点数を下げる
					distanceEvaluate=-distanceInfo[nearestIndex].dist;
				} else{
					//1ターンの間にたどり着ける場所であるなら、一律でそこそこ良い点をつける
					distanceEvaluate=0.0f;
				}
			}

		}
	}

	//行動した際の影響度についての評価値
	//actionEvaluateの値域は、1ターンの間で目標ユニットにactionができない場合はtotalが負の値になるようにするためにこうする
	//	actionEvaluate<=m_operateUnit->GetMoveDistance()
	const float actionEvaluate=CalculateActionEvaluate(punit);

	//合計の評価値を返す
	return distanceEvaluate+actionEvaluate;
}

float ComputerMoveScene::CalculateActionEvaluate(const Unit *punit)const{
	//行動した際の影響度についての評価値
	float actionEvaluate=-50000.0f;
	//2ユニットが敵同士だとして評価値を計算し、味方であれば最後に評価値の正負を反転させる
	Weapon::AttackInfo attackinfo=m_battleSceneData->m_operateUnit->GetBattleStatus().weapon->GetAttackInfo(m_battleSceneData->m_operateUnit,punit);
	actionEvaluate=m_battleSceneData->m_operateUnit->GetMoveDistance()*std::fminf(1.0f,(float)(1.0*attackinfo.damage/punit->GetBattleStatus().HP));//基本的にピンチの敵に攻撃・ピンチの味方に回復をした方が評価が高い。正規化も行う。
	if(Unit::Team::JudgeFriend(m_battleSceneData->m_operateUnit->GetBattleStatus().team,punit->GetBattleStatus().team)){
		actionEvaluate=-actionEvaluate;
	}
	return actionEvaluate;
}

void ComputerMoveScene::ImpassableLatticeInShape(const size_t index){
	const size_t x=index%m_xLatticeNum,y=index/m_xLatticeNum;
	ImpassableLatticeInShape(x,y);
}

void ComputerMoveScene::ImpassableLatticeInShape(const size_t x,const size_t y){
	const size_t index=x+y*m_xLatticeNum;
	if(x<m_xLatticeNum && y<m_yLatticeNum){
		m_latticeInShape[index]=1;
	}
	//全ての1である格子点に隣接する格子点を2にする。
	auto Input2Only0=[](int &a){if(a==0){ a=2; }};
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

int ComputerMoveScene::BranchingWaitingProcess(){
	//自分が今いる地点から行動対象にできるキャラがいるかを調べる。複数いる場合は評価値の高いキャラを対象とする。
	float evaluate=-50000.0f;
	Unit *pActedUnit=nullptr;
	for(Unit *pUnit:m_battleSceneData->m_unitList){
		if(JudgeBecomeAimedUnit(pUnit)){
			//行動対象にできた場合は、更新するか判定して場合によっては更新する
			const float pal=CalculateActionEvaluate(pUnit);
			if(pActedUnit==nullptr || evaluate<pal){
				pActedUnit=pUnit;
				evaluate=pal;
			}
		}
	}
	//攻撃するか待機するかの分岐
	if(pActedUnit==nullptr || evaluate<=0.0){
		//行動対象がいなければ待機
		//行動に意味がなくても待機
		FinishUnitOperation();
		return 0;
	} else{
		//行動対象がいればそのユニットを対象にして行動場面へ
		m_aimedUnit=pActedUnit;
		return SceneKind::e_attackNormal;
	}
}

int ComputerMoveScene::thisCalculate(){
	//フレーム関係の更新
	m_aimChangeFrame++;
	m_attackFailedInfo.m_frame++;
	//m_operateUnitの位置更新
	const Vector2D beforeVec=m_battleSceneData->m_operateUnit->getPos();
	PositionUpdate(CalculateInputVec());
	const float moveSqLength=(beforeVec-m_battleSceneData->m_operateUnit->getPos()).sqSize();
	const size_t processedFrame=m_battleSceneData->m_fpsMesuring.GetProcessedFrame();
	if(!m_actionWaiting){
		//行動までの待ち時間を待っている状態ではない時
		if(processedFrame>45){
			//1秒経ったら行動する
			if(JudgeAttackCommandUsable() && m_aimedUnit==m_targetUnit && m_attackFailedInfo.JudgeAttackProcessProceed()){
				//m_aimerUnitがAIが決めていた攻撃対象に一致した時、攻撃処理を行う
				//ただし、m_attackFailedInfoから分かる再移動の際は規定フレームを待つ
				m_nextScene=BranchingWaitingProcess();
				m_actionWaiting=true;
			} else if(m_targetUnit!=nullptr && m_battleSceneData->m_operateUnit->JudgeAttackable(m_targetUnit) && m_aimedUnit!=m_targetUnit){
				//AIが決めていた攻撃対象が攻撃範囲内にいるが、m_aimedUnitがそれに一致しないときは、攻撃対象を動かす
				if(m_aimChangeFrame>5){
					//ただし、5フレームほど間隔を持たせる
					SetAimedUnit(1);
					PlaySoundMem(m_battleSceneData->m_aimchangeSound,DX_PLAYTYPE_BACK,TRUE);//狙い切り替えなのでその効果音を
					m_aimChangeFrame=0;
				}
			} else if(m_battleSceneData->m_operateUnit->GetBattleStatus().OP<2.0f
				|| processedFrame>600//不具合があった時にゲームに戻れるようにするため
				)
			{
				//移動できなくなったら、または10秒経ったら待機
				m_nextScene=BranchingWaitingProcess();
				m_actionWaiting=true;
			} else if((moveSqLength<0.1f && processedFrame>120)){
				//移動距離も少ない場合は移動先の変更
				if(m_latticeRoute.size()<2){
					//進む場所がないまたは最後の1点にたどり着かずに止まっている場合は待機でよい
					//最後の1点の場合も待機を行う理由は、先頭点を進入不可にする事でルート変更を行うが、最後の1点は大抵は元々進入不可でルートが変わらず、無限ループとなってしまうから。
					m_nextScene=BranchingWaitingProcess();
					m_actionWaiting=true;
				} else{
					//経由点で行き詰まっている場合、そのルートは間違っていると考えられるのでルート変更をする
					if(m_latticeRoute.front().first<m_latticeInShape.size()){
						ImpassableLatticeInShape(m_latticeRoute.front().first);//今通ろうとしている格子点を通れなくする
						CalculateLatticeRoute();//ルート変更処理
					}
				}
			} else if(!m_latticeRoute.empty() && m_battleSceneData->m_operateUnit->JudgePointInsideShape(m_latticeRoute.front().second)){
				//m_latticeRouteの先頭の点がユニットの当たり判定図形に入ったらそこまでは簡単に動けるということなので移動先を変える
				m_latticeRoute.erase(m_latticeRoute.begin());
			}
		}
	} else{
		//待ち時間を待っている時
		m_waitingFrame++;
		if(m_waitingFrame>6){
			//0.1秒待ってから行動へ
			if(m_nextScene==SceneKind::e_attackNormal && !JudgeAttackCommandUsable()){
				//攻撃選択の場合、JudgeAttackCommandUsable()をする。図形押し出し処理の影響で、「攻撃できると思ったらできない」が発生する事があるため。
				//こうなるパターンは様々あるため、１度だけ再探索を行わせ、それでもダメなら待機する処理にする。
				if(m_attackFailedInfo.JudgeRetry()){
					//規定回数内の失敗の時、やり直しをする
					m_actionWaiting=!m_actionWaiting;
					m_waitingFrame=0;
					m_attackFailedInfo.RetryProcess();
				}else{
					//やり直し回数をオーバーしたら、もう何もせずにターン終了
					FinishUnitOperation();//再現が難しすぎるので、自然発生的に発生するのを待つ
					m_battleSceneData->m_fpsMesuring.RecordTime();
					return 0;
				}
			} else{
				//問題なく行動できるなら、行動処理に移行する
				m_battleSceneData->m_fpsMesuring.RecordTime();
				return m_nextScene;
			}
		}
	}
	return SceneKind::e_move;
}

void ComputerMoveScene::thisDraw()const{
	//プレイヤーのと同じ描画処理
	MoveScene::thisDraw();

	//デバッグ用描画
	if(m_battleSceneData->m_drawObjectShapeFlag){
		//格子点を描画
		for(size_t i=0,size=m_latticeInShape.size();i<size;i++){
			const int x=(i%m_xLatticeNum)*squareSize,y=(i/m_xLatticeNum)*squareSize;
			unsigned int color;
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
			DrawCircleAA(m_latticeRoute.front().second.x,m_latticeRoute.front().second.y,5,15,GetColor(255,255,0),FALSE);
			for(size_t i=0,size=m_latticeRoute.size();i+1<size;i++){
				DrawCircleAA(m_latticeRoute[i+1].second.x,m_latticeRoute[i+1].second.y,5,15,GetColor(255,255,0),FALSE);
				DrawLineAA(m_latticeRoute[i].second.x,m_latticeRoute[i].second.y,m_latticeRoute[i+1].second.x,m_latticeRoute[i+1].second.y,GetColor(255,255,0));
			}
		}
	}
}
