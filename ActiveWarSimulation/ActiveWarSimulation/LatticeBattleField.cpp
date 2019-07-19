#include"LatticeBattleField.h"
#include"BattleSceneData.h"
#include"CommonConstParameter.h"

//---------------LatticeBattleField::LatticeDistanceInfo------------------
bool LatticeBattleField::LatticeDistanceInfo::operator<(const LatticeDistanceInfo &otherobj)const{
	//評価の優先順位は「距離」→「インデックス」の順。近いところから処理していきたいので
	if(this->dist<otherobj.dist){
		return true;
	} else if(this->dist>otherobj.dist){
		return false;
	}
	return (this->index<otherobj.index);
}

bool LatticeBattleField::LatticeDistanceInfo::operator==(const LatticeDistanceInfo &otherobj)const{
	return (this->index==otherobj.index) && (this->dist==otherobj.dist);
}

//-----------------------LatticeBattleField-------------------------
const size_t LatticeBattleField::latticeIntervalSize=CommonConstParameter::unitCircleSize;

std::shared_ptr<LatticeBattleField> LatticeBattleField::Create(const BattleSceneData &battleData,const Unit * const punit,bool unitExist){
	return std::shared_ptr<LatticeBattleField>(new LatticeBattleField(battleData,punit,unitExist));
}

LatticeBattleField::~LatticeBattleField(){}

LatticeBattleField::LatticePass LatticeBattleField::GetLatticeInShapeAt(size_t index)const{
	if(index<m_xLatticeNum*m_yLatticeNum){
		return m_latticeInShape[index];
	} else{
		//配列外参照した時は、存在しない点が通ろうとしているのでunpassableを返す
		return LatticePass::e_unpassable;
	}
}

void LatticeBattleField::BecomeImpassibleLattice(size_t index){
	if(m_xLatticeNum>0){
		BecomeImpassibleLattice(index%m_xLatticeNum,index/m_xLatticeNum);
	}
}

void LatticeBattleField::BecomeImpassibleLattice(size_t x,size_t y){
	const size_t index=x+y*m_xLatticeNum;
	if(x<m_xLatticeNum && y<m_yLatticeNum){
		m_latticeInShape[index]=LatticePass::e_unpassable;
	}
	//全てのunpassableである格子点に隣接する格子点をneighborUnpassableにする。
	const auto BecomeNeighborUnpassable=[](LatticePass &pass){
		if(pass!=LatticePass::e_unpassable){
			//既にunpassableである場所はneighborUnpassableにしてはいけない
			pass=LatticePass::e_neighborUnpassable;
		}
	};
	if(y>0){
		//上の隣接点
		BecomeNeighborUnpassable(m_latticeInShape[index-m_xLatticeNum]);
	}
	if(x>0){
		//左の隣接点
		BecomeNeighborUnpassable(m_latticeInShape[index-1]);
	}
	if(x+1<m_xLatticeNum){
		//上の隣接点
		BecomeNeighborUnpassable(m_latticeInShape[index+1]);
	}
	if(y+1<m_yLatticeNum){
		//上の隣接点
		BecomeNeighborUnpassable(m_latticeInShape[index+m_xLatticeNum]);
	}
}

void LatticeBattleField::BecomeImpassibleLattice(const Unit *punit,const Unit::Team::Kind operateTeam){
	const size_t vecSize=m_xLatticeNum*m_yLatticeNum;
	std::vector<int> info(vecSize,0);//通れるかの情報を一旦格納する。通れないところのみを1とする。
	//Teamを用いてpunitの当たり判定図形を再構築
	const Shape *shape=punit->GetHitJudgeShape(Unit::Team::JudgeFriend(punit->GetBattleStatus().team,operateTeam));
	//当たり判定図形からunpassableな格子点を追加、既存のunpasssable格子点追加関数を用いて楽をする
	shape->RecordLatticePointInShape(info,m_xLatticeNum,m_yLatticeNum,latticeIntervalSize,latticeIntervalSize,1);
	for(size_t i=0,size=vecSize;i<size;i++){
		if(info[i]==1){
			//通れないと分かった場所を通れなくさせる
			BecomeImpassibleLattice(i);
		}
	}
}

void LatticeBattleField::CalculateLatticeDistanceInfo(std::vector<LatticeDistanceInfo> &retPal,const Vector2D startPos)const{
	//事前準備
	const size_t latticeNum=m_latticeInShape.size();//何度も使うので予め計算しておく
	retPal=std::vector<LatticeDistanceInfo>(latticeNum,LatticeDistanceInfo(latticeNum,latticeNum,-0.1f));//初期化
	std::set<LatticeDistanceInfo> latticeSet;//探索していく格子点のリスト。近い順から探すためにsetを用いる。
	const auto SearchUpdate=[&latticeSet,&retPal](const size_t from,const float dist,const size_t x,const size_t y,const size_t width,const size_t height,const std::vector<LatticePass> &latticeInShape){
		//retPalを更新し、latticeSetに格納する。
		if(x>=0 && x<width && y>=0 && y<height){
			size_t index=x+y*width;
			if(latticeInShape[index]!=LatticePass::e_unpassable && (retPal[index].dist<0.0f || dist<retPal[index].dist)){
				//進入不可ではなく、なおかつ今見つけたルートよりも短いルートが発見されていなければ
				//距離マップの更新を行う
				LatticeDistanceInfo info(index,from,dist);
				retPal[index]=info;
				if(latticeInShape[index]==LatticePass::e_passable){
					//探索する格子点の追加は、m_latticeInShape[index]が通れる場合のみ行う
					latticeSet.insert(info);
				}
			}
		}
	};
	//初期位置に近い4点を探す
	const size_t initX=(size_t)(startPos.x/latticeIntervalSize),initY=(size_t)(startPos.y/latticeIntervalSize);
	SearchUpdate(latticeNum,(startPos-Vector2D((float)(initX*latticeIntervalSize),(float)(initY*latticeIntervalSize))).size(),initX,initY,m_xLatticeNum,m_yLatticeNum,m_latticeInShape);
	SearchUpdate(latticeNum,(startPos-Vector2D((float)((initX+1)*latticeIntervalSize),(float)(initY*latticeIntervalSize))).size(),initX+1,initY,m_xLatticeNum,m_yLatticeNum,m_latticeInShape);
	SearchUpdate(latticeNum,(startPos-Vector2D((float)(initX*latticeIntervalSize),(float)((initY+1)*latticeIntervalSize))).size(),initX,initY+1,m_xLatticeNum,m_yLatticeNum,m_latticeInShape);
	SearchUpdate(latticeNum,(startPos-Vector2D((float)((initX+1)*latticeIntervalSize),(float)((initY+1)*latticeIntervalSize))).size(),initX+1,initY+1,m_xLatticeNum,m_yLatticeNum,m_latticeInShape);
	//通れる可能性のある部分を全て探索
	const int dx[8]={-1,0,1,-1,1,-1,0,1};
	const int dy[8]={-1,-1,-1,0,0,1,1,1};
	const float slantingdist=Vector2D((float)(latticeIntervalSize),(float)(latticeIntervalSize)).size();
	const float horizondist=(float)latticeIntervalSize;
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

Vector2D LatticeBattleField::CalculateLatticePointPos(size_t x,size_t y)const{
	return Vector2D((float)(x*latticeIntervalSize),(float)(y*latticeIntervalSize));
}

Vector2D LatticeBattleField::CalculateLatticePointPos(size_t index)const{
	return CalculateLatticePointPos(index%m_xLatticeNum,index/m_xLatticeNum);
}

std::vector<float> LatticeBattleField::CalculateRouteDistance(const Vector2D start,const std::vector<Vector2D> &endVec)const{
	//距離マップを作成
	std::vector<LatticeDistanceInfo> distanceInfoVec;
	CalculateLatticeDistanceInfo(distanceInfoVec,start);
	//各格子点に対して距離を求める
	std::vector<float> retPal;
	const auto CalcMinDistance=[&distanceInfoVec,this](size_t x,size_t y,float minDistance){
		if(x>=0 && x<m_xLatticeNum && y>=0 && y<m_yLatticeNum){
			const float dist=distanceInfoVec[x+y*m_xLatticeNum].dist;
			if(minDistance<0.0f || dist<minDistance){
				return dist;
			}
		}
		return minDistance;
	};
	for(const Vector2D &end:endVec){
		//endの周辺の格子点までの距離を求め,最小距離のものをendまでのルート距離とする
		const size_t endLeftUpX=(size_t)(end.x/latticeIntervalSize),endLeftUpY=(size_t)(end.y/latticeIntervalSize);
		float minDistance=-0.1f;
		minDistance=CalcMinDistance(endLeftUpX,endLeftUpY,minDistance);
		minDistance=CalcMinDistance(endLeftUpX+1,endLeftUpY,minDistance);
		minDistance=CalcMinDistance(endLeftUpX,endLeftUpY+1,minDistance);
		minDistance=CalcMinDistance(endLeftUpX+1,endLeftUpY+1,minDistance);
		retPal.push_back(minDistance);
	}
	return retPal;
}

LatticeBattleField::LatticeBattleField(const BattleSceneData &battleData,const Unit * const punit,bool unitExist)
	:m_xLatticeNum((size_t)(battleData.m_stageSize.x)/latticeIntervalSize+1)
	,m_yLatticeNum((size_t)(battleData.m_stageSize.y)/latticeIntervalSize+1)
{
	CalculateLatticeInShape(battleData,punit,unitExist);
}

void LatticeBattleField::CalculateLatticeInShape(const BattleSceneData &battleData,const Unit * const punit,bool unitExist){
	const size_t vecSize=m_xLatticeNum*m_yLatticeNum;
	//最初に全ての格子点が通れるとして初期化する
	m_latticeInShape=std::vector<LatticePass>(vecSize,LatticePass::e_passable);
	//操作するユニットが通れない場所にe_unpassableを格納していく
	//「障害物の中＝中にいないとユニットは通れない」という考え方を用いる
	for(const BattleObject *object:battleData.m_field){
		if(object==punit){
			//操作ユニットは通れるか否かに影響を与えない

		} else if(object->GetFix()==Shape::Fix::e_ignore){
			//当たり判定のないオブジェクトは通れるか否かに影響を与えない

		} else if(!unitExist && object->GetType()==BattleObject::Type::e_unit){
			//unitExistがfalseの時、ユニットは通れるか否かに影響を与えない

		} else if(punit!=nullptr && object->JudgePointInsideShape(punit->getPos())){
			//object内部にユニットがいるなら、その外部を通れないようにする
			std::vector<int> info(vecSize,1);//通れるかの情報を一旦格納する。通れるところのみを0とする。
			object->GetHitJudgeShape()->RecordLatticePointInShape(info,m_xLatticeNum,m_yLatticeNum,latticeIntervalSize,latticeIntervalSize,0);
			for(size_t i=0,size=vecSize;i<size;i++){
				if(info[i]==1){
					//通れないと分かった場所を通れなくさせる
					m_latticeInShape[i]=LatticePass::e_unpassable;
				}
			}
		} else{
			//object外部にユニットがいるなら、その内部は通れないようにする
			std::vector<int> info(vecSize,0);//通れるかの情報を一旦格納する。通れないところのみを1とする。
			object->GetHitJudgeShape()->RecordLatticePointInShape(info,m_xLatticeNum,m_yLatticeNum,latticeIntervalSize,latticeIntervalSize,1);
			for(size_t i=0,size=vecSize;i<size;i++){
				if(info[i]==1){
					//通れないと分かった場所を通れなくさせる
					m_latticeInShape[i]=LatticePass::e_unpassable;
				}
			}
		}
	}
	//unpassableの隣にneighborUnpassableを置く
	for(size_t y=0;y<m_yLatticeNum;y++){
		for(size_t x=0;x<m_xLatticeNum;x++){
			const size_t index=x+y*m_xLatticeNum;
			if(m_latticeInShape[index]==LatticePass::e_unpassable){
				//全てのunpassableである格子点に隣接する格子点をneighborUnpassableにする。
				BecomeImpassibleLattice(x,y);//m_latticeInShape[index]をunpassableとする処理も含むが、元々unpassableなので問題ない
			}
		}
	}
}