#include"input.h"
#include"DxLib.h"
#include"ComputerMoveScene.h"
#include"CommonConstParameter.h"

#include"Circle.h"


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
ComputerMoveScene::ComputerMoveScene(std::shared_ptr<BattleSceneData> battleSceneData)
	:MoveScene(battleSceneData)
	,m_latticeField(battleSceneData->CalculateLatticeBattleField(true))
	,m_actionWaiting(false)
	,m_nextScene(SceneKind::e_move)
	,m_aimChangeFrame(0)
	,m_waitingFrame(0)
	,m_attackFailedInfo()
{
	//通る格子点の計算
	CalculateLatticeRoute();
}

ComputerMoveScene::~ComputerMoveScene(){}

std::pair<std::pair<size_t,Vector2D>,Unit *> ComputerMoveScene::DecideTargetPoint(const std::vector<LatticeBattleField::LatticeDistanceInfo> &distanceInfo)const{
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
	const size_t vecSize=m_latticeField->GetLatticeInShapeSize();
	Vector2D targetPointVec=m_battleSceneData->m_operateUnit->getPos();//targetに相当する座標まで到達した際にどの座標に向かって動くか。初期値は「その場で待機する」
	size_t target=vecSize;
	if(targetUnit!=nullptr){
		//狙うユニットを決めたら、そのユニットを攻撃できる格子点を探す
		//貪欲法でいく
		Unit copiedUnit(*dynamic_cast<Unit *>(m_battleSceneData->m_operateUnit->VCopy().get()));//作業用にコピーを作る
		for(size_t y=0;y<m_latticeField->GetYLatticeNum();y++){
			for(size_t x=0;x<m_latticeField->GetXLatticeNum();x++){
				const size_t index=x+y*m_latticeField->GetXLatticeNum();
				if(distanceInfo[index].dist>=0.0f && (target>=vecSize || distanceInfo[index]<distanceInfo[target])){
					//既に別のより近い目標地点候補が存在するなら新しい目標地点になる事はない
					const Vector2D pos=m_latticeField->CalculateLatticePointPos(x,y);
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
	case(Unit::AIType::e_wait):
		//待機型AI
		//必ずその場で待機する
		target=vecSize;
		targetPointVec=m_battleSceneData->m_operateUnit->getPos();
		break;
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

void ComputerMoveScene::CalculateLatticeRoute(){
	m_latticeRoute.clear();
	//各格子点への距離マップの作成
	const size_t latticeNum=m_latticeField->GetLatticeInShapeSize();
	std::vector<LatticeBattleField::LatticeDistanceInfo> latticeDistanceInfo;//各格子点に進むための最小距離と、どの格子点から移動すればよいかが格納されている。
	m_latticeField->CalculateLatticeDistanceInfo(latticeDistanceInfo,m_battleSceneData->m_operateUnit->getPos());//距離マップ計算関数

	distvec=latticeDistanceInfo;//デバッグ用

	//目標地点の決定
	const std::pair<std::pair<size_t,Vector2D>,Unit *> targetPoint=DecideTargetPoint(latticeDistanceInfo);//ここはAIの行動傾向によって異なるので
	m_targetUnit=targetPoint.second;
	size_t targetPointIndex=targetPoint.first.first;//目標地点の格子点番号
	Vector2D targetPointVec=targetPoint.first.second;//目標座標
	if(targetPointIndex==latticeNum && m_battleSceneData->m_operateUnit->GetBattleStatus().aitype==Unit::AIType::e_assult){
		//突撃型かつ目標地点が現在位置である場合、詰まりが発生している。そのため、途中まで進めるように再度計算をする。
		//ユニットの当たり判定を無視した目標地点の計算を行う
		const std::shared_ptr<LatticeBattleField> virtualLatticeField=m_battleSceneData->CalculateLatticeBattleField(false);//ユニットが誰もいない地形で計算をして比較しながらルート検索をすれば、詰まる地点までは進める
		const size_t virtualLatticeNum=virtualLatticeField->GetLatticeInShapeSize();
		std::vector<LatticeBattleField::LatticeDistanceInfo> virtualLatticeDistanceInfo;
		virtualLatticeField->CalculateLatticeDistanceInfo(virtualLatticeDistanceInfo,m_battleSceneData->m_operateUnit->getPos());
		const std::pair<std::pair<size_t,Vector2D>,Unit *> virtualTargetPoint=DecideTargetPoint(virtualLatticeDistanceInfo);
		//目標地点計算(これでも探索に失敗すると、その場で動かなくなる。それ自体は構わない。)
		for(size_t point=virtualTargetPoint.first.first;point<latticeNum;point=virtualLatticeDistanceInfo[point].from){
			//latticeDistanceInfo[point].fromを辿っていけば最短距離となる
			if(latticeDistanceInfo[point].from<latticeNum){
				//pointにたどり着く事ができるならば(pointにたどり着く事ができる格子点の存在が見つけられれば良い)
				targetPointIndex=point;
				targetPointVec=m_latticeField->CalculateLatticePointPos(point);//その格子点の位置を最終目標地点とする
				break;
			}
		}
	}

	//ルートの選定と格納
	m_latticeRoute.push_back(std::pair<size_t,Vector2D>(latticeNum,targetPointVec));//最終目標の格子点についた後に向かう位置を最初に格納しておく（これは恐らくユニット内部で入れない）
	for(size_t point=targetPointIndex;point<latticeNum;point=latticeDistanceInfo[point].from){
		//latticeDistanceInfo[point].fromを辿っていけば最短距離となる
		Vector2D v;
		if(point<latticeNum){
			//格子点が存在するならその位置に
			v=m_latticeField->CalculateLatticePointPos(point);
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
	//ターン開始から1秒経ったら動く、また待機までの待ち時間(=m_actionWaitingがtrueの時)は移動しない
	if(m_battleSceneData->m_fpsMesuring.GetProcessedTime()>1.0 && !m_actionWaiting){
		//m_latticeRouteの先頭に向かって動く
		if(!m_latticeRoute.empty()){
			moveVec=m_latticeRoute.front().second-m_battleSceneData->m_operateUnit->getPos();
		}
	}
	return moveVec;
}

float ComputerMoveScene::CalculateEvaluate(const Unit *punit,const std::vector<LatticeBattleField::LatticeDistanceInfo> &distanceInfo)const{
	//const float sqDistance=-(punit->getPos()-m_battleSceneData->m_operateUnit->getPos()).sqSize();//ユニット間の距離の2乗値。
	//punitに攻撃が届く位置までの距離についての評価値
	//distanceEvaluateの値域
	//	distanceEvaluate=0:届く
	//	distanceEvaluate<=m_operateUnit->GetMoveDistance():届かない
	float distanceEvaluate=-50000.0f;//ひとまず酷い点数をつける
	if(punit->GetHitJudgeShape()->GetType()==Shape::Type::e_circle){
		const Circle *pcircle=dynamic_cast<const Circle *>(punit->GetUnitCircleShape());//作業用のユニット図形(内部)の確保
		if(pcircle!=nullptr){
			Circle c(pcircle->GetPosition(),pcircle->GetR()+m_battleSceneData->m_operateUnit->GetBattleStatus().weapon->GetLength()+LatticeBattleField::latticeIntervalSize,pcircle->m_fix);//攻撃可能範囲(ただし、位置によっては格子点を検出できないため、LatticeBattleField::latticeIntervalSizeだけ余分に広い範囲で格子点を探す)
			//攻撃可能範囲の中にある格子点の中で最近点を求める
			const size_t size=distanceInfo.size();
			size_t nearestIndex=size;//これがsizeである間は
			for(size_t i=0;i<size;i++){
				if(m_latticeField->GetLatticeInShapeAt(i)==LatticeBattleField::LatticePass::e_passable
					&& distanceInfo[i].dist>=0.0f
					&& c.VJudgePointInsideShape(m_latticeField->CalculateLatticePointPos(i))
					&& (nearestIndex>=size || distanceInfo[i].dist<distanceInfo[nearestIndex].dist))
				{
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
					if(m_latticeRoute.front().first<m_latticeField->GetLatticeInShapeSize()){
						m_latticeField->BecomeImpassibleLattice(m_latticeRoute.front().first);//今通ろうとしている格子点を通れなくする
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
		for(size_t i=0,size=m_latticeField->GetLatticeInShapeSize();i<size;i++){
			const int x=(i%m_latticeField->GetXLatticeNum())*LatticeBattleField::latticeIntervalSize,y=(i/m_latticeField->GetXLatticeNum())*LatticeBattleField::latticeIntervalSize;
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
