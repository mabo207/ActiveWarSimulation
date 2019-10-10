#include"ScoreObserver.h"
#include<algorithm>
#include<cassert>
#include<map>
#include"BattleSceneData.h"

#include"WaitLog.h"
#include"AttackLog.h"
#include"FinishLog.h"
#include"InitLog.h"

#include"ArcherAttackDistance.h"
#include"MageAttackingOpponent.h"
#include"ArmerPosition.h"
#include"HealerPosition.h"
#include"ProtectFriend.h"
#include"IntensiveAttack.h"

//--------------ScoreObserver::Bonus-----------------
ScoreObserver::Bonus::Bonus(const std::string &explain,int score)
	:m_score(score)
	,m_explain(explain)
{}

ScoreObserver::Bonus::~Bonus(){}

//---------------ScoreObserver::LatticeBonusData----------------
ScoreObserver::LatticeBonusData::LatticeBonusData()
	:m_playerCount(0)
	,m_totalPlayerRate(0.0)
	,m_enemyCount(0)
	,m_totalEnemyRate(0.0)
{}

ScoreObserver::LatticeBonusData::~LatticeBonusData(){}

void ScoreObserver::LatticeBonusData::InputData(const BattleSceneData * const battleData,const LogElement::UnitLogData &operateUnitData){
	if(operateUnitData.punit!=nullptr){
		//格子点情報の獲得
		const auto latticeField=battleData->CalculateLatticeBattleField(true);//ユニットを含めた格子点情報でok
		std::vector<LatticeBattleField::LatticeDistanceInfo> distanceInfo;
		latticeField->CalculateLatticeDistanceInfo(distanceInfo,operateUnitData.punit->getPos());
		//長さRについて「半径Rの円の中にある格子点の数」「距離がR以内にある格子点の数」を数える
		//この数値は要調整
		const float circleSizeF=Unit::unitCircleSize*5.0f;//狭めに絞って、自分の近くの密集度を測定する
		const size_t circleSizeI=(size_t)circleSizeF;
		size_t inCircleCount=0,arrivableCount=0;
		//円内にある格子点の数を数え上げ。円が画面外に入り格子点が少なくなる事を防ぐため、円の半径によって一定に定まる計算方法を採用する。
		for(size_t y=0;y<=circleSizeI;y+=LatticeBattleField::latticeIntervalSize){
			for(size_t x=0;x<=circleSizeI;x+=LatticeBattleField::latticeIntervalSize){
				if(x*x+y*y<=circleSizeI*circleSizeI){
					inCircleCount++;
				}
			}
		}
		//上の計算で1/4円の数え上げができたため、軸に沿った２つの部分の内１つを除いて４倍すれば数え上げ終了。
		inCircleCount=(inCircleCount-circleSizeI/LatticeBattleField::latticeIntervalSize)*4+1;
		//実質距離R以内のものを数え上げ
		for(const auto &info:distanceInfo){
			if(latticeField->GetLatticeInShapeAt(info.index)!=LatticeBattleField::LatticePass::e_unpassable
				&& info.dist<=circleSizeF)
			{
				//入れない部分は除くように
				arrivableCount++;
			}
		}
		//プレイヤーか敵かを見て記録
		if(inCircleCount>0){
			const double rate=1.0*arrivableCount/inCircleCount;
			switch(operateUnitData.punit->GetBattleStatus().team){
			case(Unit::Team::e_player):
				m_playerCount++;
				m_totalPlayerRate+=rate;
				break;
			case(Unit::Team::e_enemy):
				m_enemyCount++;
				m_totalEnemyRate+=rate;
				break;
			}
		}
	}
}

ScoreObserver::Bonus ScoreObserver::LatticeBonusData::GetPlayerBonus()const{
	if(m_playerCount>0){
		const double rate=m_totalPlayerRate/m_playerCount;
		if(rate>=0.50){
			//この数値は要調整
			return Bonus("広所確保",3000);
		}
	}
	return Bonus("",0);
}

ScoreObserver::Bonus ScoreObserver::LatticeBonusData::GetEnemyBonus()const{
	if(m_enemyCount>0){
		const double rate=m_totalEnemyRate/m_enemyCount;
		if(rate<=0.35){
			//この数値は要調整
			return Bonus("敵に狭所を押し付けた",3000);
		}
	}
	return Bonus("",0);
}

//---------------ScoreObserver--------------
void ScoreObserver::InitUpdate(const BattleSceneData * const battleData){
	m_logList.clear();
	m_logList.push_back(std::make_shared<InitLog>(battleData));
}

void ScoreObserver::FinishUpdate(const BattleSceneData * const battleData){
	m_logList.push_back(std::make_shared<FinishLog>(battleData));
	//クリア難易度もここで測定する
	m_stageLevel=battleData->m_stageLevel;
	//サブミッションの総括的振り返りをする
	m_submission.WholeLookBack();
}

void ScoreObserver::AttackUpdate(const BattleSceneData * const battleData,const Unit * const aimedUnit){
	const std::shared_ptr<LogElement> logData=std::make_shared<AttackLog>(battleData,aimedUnit);
	m_latticeBonusData.InputData(battleData,logData->GetOperateUnitData());
	m_logList.push_back(logData);
	//サブミッション更新
	if(m_submission.JudgeEvaluatedOrder(battleData)){
		m_submission.RubricEvaluate(battleData);
	}
}

void ScoreObserver::ResearchUpdate(){
	m_researchCount++;
}

void ScoreObserver::WaitUpdate(const BattleSceneData * const battleData){
	const std::shared_ptr<LogElement> logData=std::make_shared<WaitLog>(battleData);
	m_latticeBonusData.InputData(battleData,logData->GetOperateUnitData());
	m_logList.push_back(logData);
	//サブミッション更新
	if(m_submission.JudgeEvaluatedOrder(battleData)){
		m_submission.RubricEvaluate(battleData);
	}
}

void ScoreObserver::CancelUpdate(){
	m_cancelCount++;
}

std::shared_ptr<ScoreObserver::ScoreExpression> ScoreObserver::GetScoreExpression(bool winFlag)const{
	//スコア計算方法がベタ書きされている、リファクタリングの方法求む
	std::vector<Bonus> bonus;
	//基本スコア
	int turnScore=0;//ターン数
	int liveScore=0;//生存数
	//よく使われる特徴的なログデータ
	const FinishLog * finishLog=nullptr;
	const InitLog *initLog=nullptr;
	if(!m_logList.empty()){
		const LogElement *ptr=m_logList.at(0).get();
		initLog=dynamic_cast<const InitLog *>(ptr);
		ptr=m_logList.back().get();
		finishLog=dynamic_cast<const FinishLog *>(ptr);
	}
	if(initLog==nullptr || finishLog==nullptr){
		//ログの取り方がおかしい、エラー
		assert(false);
		return std::shared_ptr<ScoreExpression>(nullptr);
	}
	//共通して用いられるデータ
	size_t totalPlayerUnitCount=0,livePlayerUnitCount=0;//プレイヤーユニットの合計と生存数
	size_t totalEnemyUnitCount=0,liveEnemyUnitCount=0;//敵ユニットの合計と生存数
	const auto judgeExistProfession=[](const LogElement::UnitLogData &logData,Unit::Profession::Kind profession){
		return (logData.punit!=nullptr
			&& logData.punit->GetBaseStatus().profession==profession
			&& logData.punit->GetBattleStatus().team==Unit::Team::e_player);
	};
	//クリア難易度
	{
		if(winFlag){
			//勝利している時のみボーナス付与
			if(m_stageLevel==StageLevel::e_easy){
				bonus.push_back(Bonus("難易度EASYをクリア",5000));
			} else if(m_stageLevel==StageLevel::e_normal){
				bonus.push_back(Bonus("難易度NORMALをクリア",20000));
			} else if(m_stageLevel==StageLevel::e_hard){
				bonus.push_back(Bonus("難易度HARDをクリア",35000));
			} else if(m_stageLevel==StageLevel::e_lunatic){
				bonus.push_back(Bonus("難易度LUNATICをクリア",50000));
			}
		}
	}
	//生存数(味方のついでに敵も計算)
	{
		//生存数からの得点計算(寝返りを想定していない)
		for(const LogElement::UnitLogData &unitData:initLog->m_unitDataList){
			if(unitData.punit!=nullptr){
				switch(unitData.punit->GetBattleStatus().team){
				case(Unit::Team::e_player):
					totalPlayerUnitCount++;
					break;
				case(Unit::Team::e_enemy):
					totalEnemyUnitCount++;
					break;
				}
			}
		}
		for(const LogElement::UnitLogData &unitData:finishLog->m_unitDataList){
			if(unitData.punit!=nullptr){
				switch(unitData.punit->GetBattleStatus().team){
				case(Unit::Team::e_player):
					livePlayerUnitCount++;
					break;
				case(Unit::Team::e_enemy):
					liveEnemyUnitCount++;
					break;
				}
			}
		}
		if(totalPlayerUnitCount>0){
			liveScore=40000*livePlayerUnitCount/totalPlayerUnitCount;
		} else{
			liveScore=0;
		}
		//生存数のボーナス得点
		if(totalPlayerUnitCount-livePlayerUnitCount==0){
			bonus.push_back(Bonus("全員生存",7000));
		} else if(livePlayerUnitCount==1){
			bonus.push_back(Bonus("ほぼ引き分け",200));
		}
	}
	//クリアターン数からの得点計算
	const int clearTurn=finishLog->GetClearTurn();
	//クリアターン数
	{
		if(winFlag){
			//勝利している時のみスコア計算
			turnScore=std::max(0,20-clearTurn)*2000;
			//クリアターン数のボーナス得点
			if(clearTurn<(int)(totalEnemyUnitCount)){
				bonus.push_back(Bonus("神速進軍",6000));
			} else if(clearTurn<(int)(totalEnemyUnitCount+2)){
				bonus.push_back(Bonus("速攻進軍",3000));
			} else if(clearTurn>10){
				bonus.push_back(Bonus("ノロノロ進軍",100));
			}
		}
	}
	//被ダメージ%推移
	{
		std::map<const Unit *,double> minRateMap;//ユニットと最小HP割合を記録する
		for(const std::shared_ptr<LogElement> logData:m_logList){
			for(const LogElement::UnitLogData &unitData:logData->m_unitDataList){
				if(unitData.punit!=nullptr && unitData.punit->GetBattleStatus().team==Unit::Team::e_player){
					const double hpRate=1.0*unitData.hp/unitData.punit->GetBaseStatus().maxHP;
					std::map<const Unit *,double>::iterator it=minRateMap.find(unitData.punit);
					if(it!=minRateMap.end()){
						//既に見つけたことのあるユニットであれば、最小HP割合の更新処理
						if(hpRate<it->second){
							it->second=hpRate;
						}
					} else{
						//まだ記録をつけたことがないなら、追加
						minRateMap.insert(std::pair<const Unit *,double>(unitData.punit,hpRate));
					}
				}
			}
		}
		if(totalPlayerUnitCount-livePlayerUnitCount==0){
			//全員生存時のみ評価項目の計算
			double minMinRate=1.0,maxMinRate=0.0;//全味方ユニットの最小HP割合について最小値と最大値を求める
			for(const std::pair<const Unit *,double> &pair:minRateMap){
				if(minMinRate>pair.second){
					minMinRate=pair.second;
				}
				if(maxMinRate<pair.second){
					maxMinRate=pair.second;
				}
			}
			//最小HP割合が求まれば、それに応じたボーナスを出す
			if(minMinRate>=1.00){
				bonus.push_back(Bonus("ノーダメージ",8000));
			} else if(minMinRate>=0.75){
				bonus.push_back(Bonus("HP75%キープ",4000));
			} else if(minMinRate>=0.50){
				bonus.push_back(Bonus("HP50%キープ",2000));
			} else if(maxMinRate<=0.25){
				bonus.push_back(Bonus("ギリギリの戦い",500));
			}
		}
	}
	//兵士関連
	{
		size_t moveCount=0;//手番がきた回数
		float totalWalkOP=0.0f;//移動に使った合計OP
		int attackCount=0;//攻撃回数
		for(const std::shared_ptr<const LogElement> logData:m_logList){
			const LogElement::UnitLogData operateData=logData->GetOperateUnitData();
			if(operateData.punit!=nullptr
				&& operateData.punit->GetBattleStatus().team==Unit::Team::e_player
				&& operateData.punit->GetBaseStatus().profession==Unit::Profession::e_soldier)
			{
				switch(logData->GetLogKind()){
				case(LogElement::LogKind::e_attack):
					attackCount++;
				case(LogElement::LogKind::e_wait):
					moveCount++;
					//ログ出力の後に行動分のOP消費が行われるので、待機と攻撃のどちらも同じ処理で良い
					totalWalkOP+=Unit::BattleStatus::maxOP-Unit::reduceStartActionCost-operateData.op;
					break;
				}
			}
		}
		const auto judgeExistSoldier=[&judgeExistProfession](const LogElement::UnitLogData &logData){
			return judgeExistProfession(logData,Unit::Profession::e_soldier);
		};
		if(initLog->JudgeEveryUnitData(judgeExistSoldier,false)){
			//味方に兵士がいる場合のみボーナス追加処理をする
			//平均移動OP
			if(moveCount>0){
				double averageWalkOP=totalWalkOP/moveCount;
				if(averageWalkOP>80.0){
					bonus.push_back(Bonus("遊撃のプロフェッショナル",2000));
				} else if(averageWalkOP>60.0){
					bonus.push_back(Bonus("戦士が積極的に移動した",1300));
				}
			}
			//攻撃回数
			if(attackCount>=finishLog->GetClearTurn()){
				bonus.push_back(Bonus("戦士が積極的に攻撃した",3000));
			}
		}
	}
	//重装兵関連
	{
		size_t magicAttackedCount=0;//魔法で重装兵に攻撃された回数
		size_t magicTotalCount=0;//部隊全体で魔法で攻撃された回数
		size_t physicAttackedCount=0;//物理で重装兵に攻撃された回数
		size_t physicTotalCount=0;//部隊全体で物理で攻撃された回数
		for(const std::shared_ptr<const LogElement> logData:m_logList){
			if(logData->GetLogKind()==LogElement::LogKind::e_attack){
				const std::shared_ptr<const AttackLog> attackLog=std::dynamic_pointer_cast<const AttackLog>(logData);
				if(attackLog.get()!=nullptr
					&& attackLog->GetOperateUnitData().punit!=nullptr
					&& attackLog->GetAimedUnit()!=nullptr
					&& attackLog->GetAimedUnit()->GetBattleStatus().team==Unit::Team::e_player)
				{
					switch(attackLog->GetOperateUnitData().punit->GetBattleStatus().weapon->GetActionKind()){
					case(DamageCalculator::Kind::e_physicalAttack):
						physicTotalCount++;
						if(attackLog->GetAimedUnit()->GetBaseStatus().profession==Unit::Profession::e_armer){
							physicAttackedCount++;
						}
						break;
					case(DamageCalculator::Kind::e_magicAttack):
						magicTotalCount++;
						if(attackLog->GetAimedUnit()->GetBaseStatus().profession==Unit::Profession::e_armer){
							magicAttackedCount++;
						}
						break;
					}
				}
			}
		}
		const auto judgeExistArmer=[&judgeExistProfession](const LogElement::UnitLogData &logData){
			return judgeExistProfession(logData,Unit::Profession::e_armer);
		};
		if(initLog->JudgeEveryUnitData(judgeExistArmer,false)){
			//味方に重装兵がいる場合のみボーナス処理をする
			//魔法被弾回数(敵に魔道士がいる場合のみ)
			const auto judgeExistEnemyMage=[](const LogElement::UnitLogData &unitData){
				return (unitData.punit!=nullptr
					&& unitData.punit->GetBattleStatus().team==Unit::Team::e_enemy
					&& unitData.punit->GetBaseStatus().profession==Unit::Profession::e_mage);
			};
			if(initLog->JudgeEveryUnitData(judgeExistEnemyMage,false)){
				if(magicAttackedCount==0){
					bonus.push_back(Bonus("重装兵で魔法攻撃を受けない",2000));
				} else if(magicAttackedCount==magicTotalCount){
					bonus.push_back(Bonus("重装兵が魔法で弱点を突かれ続けた",100));
				}
			}
			//物理被弾回数
			if(physicTotalCount>0){
				double rate=1.0*physicAttackedCount/physicTotalCount;
				if(rate>=0.75){
					bonus.push_back(Bonus("重装兵が鉄壁と化していた",2000));
				} else if(rate>=0.50){
					bonus.push_back(Bonus("重装兵がたくさんの物理攻撃を引き受けた",1400));
				}
			}
		}
	}
	//射手関連
	{
		size_t attackCount=0;//合計攻撃回数
		size_t effectiveAttackCount=0;//効果的な攻撃の回数（射手・魔道士・衛生兵狙い）
		float totalAttackLengthRate=0.0f;//攻撃射程割合の総和
		const auto judgeEffective=[](const Unit *punit){
			if(punit!=nullptr && punit->GetBattleStatus().team==Unit::Team::e_enemy){
				switch(punit->GetBaseStatus().profession){
				case(Unit::Profession::e_archer):
				case(Unit::Profession::e_mage):
				case(Unit::Profession::e_healer):
					return true;
				}
			}
			return false;
		};
		for(const std::shared_ptr<LogElement> logData:m_logList){
			if(logData->GetLogKind()==LogElement::LogKind::e_attack){
				const std::shared_ptr<const AttackLog> attackLog=std::dynamic_pointer_cast<const AttackLog>(logData);
				if(attackLog.get()!=nullptr
					&& attackLog->GetOperateUnitData().punit!=nullptr
					&& attackLog->GetOperateUnitData().punit->GetBattleStatus().team==Unit::Team::e_player
					&& attackLog->GetOperateUnitData().punit->GetBaseStatus().profession==Unit::Profession::e_archer
					&& attackLog->GetAimedUnit()!=nullptr)
				{
					attackCount++;
					//効果的な攻撃であればeffectiveAttackCountを加算
					if(judgeEffective(attackLog->GetAimedUnit())){
						effectiveAttackCount++;
					}
					//攻撃射程割合を調べる
					const Vector2D vec=attackLog->GetOperateUnitData().punit->getPos()-attackLog->GetAimedUnit()->getPos();
					const float maxLength=attackLog->GetOperateUnitData().punit->GetBattleStatus().weapon->GetLength();
					totalAttackLengthRate+=(vec.size()-Unit::unitCircleSize)/maxLength;
				}
			}
		}
		const auto judgeExistArcher=[&judgeExistProfession](const LogElement::UnitLogData &logData){
			return judgeExistProfession(logData,Unit::Profession::e_archer);
		};
		if(initLog->JudgeEveryUnitData(judgeExistArcher,false)){
			//アーチャーが自軍にいればボーナス計算
			//攻撃射程
			if(attackCount>0){
				const double averageRate=totalAttackLengthRate/attackCount;
				if(averageRate>=0.75){
					bonus.push_back(Bonus("ロングレンジスナイパーの異名を持つ射手",2000));
				} else if(averageRate>=0.50){
					bonus.push_back(Bonus("射手が射程を活かした攻撃に徹していた",1200));
				}
			}
			//狙う兵種
			size_t effectiveUnitCount=0;//効果的な攻撃相手の数
			for(const LogElement::UnitLogData &logData:initLog->m_unitDataList){
				if(judgeEffective(logData.punit)){
					effectiveUnitCount++;
				}
			}
			if(effectiveAttackCount>0 && effectiveAttackCount>=effectiveUnitCount){
				//効果的な攻撃相手に１回ずつ攻撃しているなら（ただし、厳密にはせず回数の合計だけみる）
				bonus.push_back(Bonus("弱点狙いの射手",1800));
			}
		}
	}
	//魔道士関連
	{
		size_t armerAttackCount=0;//重装兵攻撃回数
		size_t nearAttackedCount=0;//近接攻撃被弾回数
		for(const std::shared_ptr<const LogElement> logData:m_logList){
			if(logData->GetLogKind()==LogElement::LogKind::e_attack){
				const std::shared_ptr<const AttackLog> attackLog=std::dynamic_pointer_cast<const AttackLog>(logData);
				if(attackLog.get()!=nullptr
					&& attackLog->GetOperateUnitData().punit!=nullptr
					&& attackLog->GetAimedUnit()!=nullptr)
				{
					//重装兵への攻撃
					if(attackLog->GetOperateUnitData().punit->GetBattleStatus().team==Unit::Team::e_player
						&& attackLog->GetOperateUnitData().punit->GetBaseStatus().profession==Unit::Profession::e_mage
						&& attackLog->GetAimedUnit()->GetBattleStatus().team==Unit::Team::e_enemy
						&& attackLog->GetAimedUnit()->GetBaseStatus().profession==Unit::Profession::e_armer)
					{
						armerAttackCount++;
					}
					//近接攻撃の被弾
					if(attackLog->GetOperateUnitData().punit->GetBattleStatus().team==Unit::Team::e_enemy
						&& attackLog->GetAimedUnit()->GetBattleStatus().team==Unit::Team::e_player
						&& attackLog->GetAimedUnit()->GetBaseStatus().profession==Unit::Profession::e_mage)
					{
						switch(attackLog->GetOperateUnitData().punit->GetBaseStatus().profession){
						case(Unit::Profession::e_soldier):
						case(Unit::Profession::e_armer):
							nearAttackedCount++;
							break;
						}
					}
				}
			}
		}
		const auto judgeExistMage=[&judgeExistProfession](const LogElement::UnitLogData &logData){
			return judgeExistProfession(logData,Unit::Profession::e_mage);
		};
		if(initLog->JudgeEveryUnitData(judgeExistMage,false)){
			//魔道士が自軍にいる場合はボーナス処理をする
			//重装兵への攻撃回数
			size_t armerCount=0;//敵重装兵の数
			for(const LogElement::UnitLogData &unitData:initLog->m_unitDataList){
				if(unitData.punit!=nullptr
					&& unitData.punit->GetBaseStatus().profession==Unit::Profession::e_armer
					&& unitData.punit->GetBattleStatus().team==Unit::Team::e_enemy)
				{
					armerCount++;
				}
			}
			if(armerCount>0){
				const double average=1.0*armerAttackCount/armerCount;
				if(average>=1.5){
					bonus.push_back(Bonus("重装兵キラーの魔道士",3000));
				} else if(average>=1.0){
					bonus.push_back(Bonus("魔道士で重装兵狙い",1800));
				}
			}
			//近接攻撃被弾回数
			if(nearAttackedCount<=0){
				bonus.push_back(Bonus("魔道士が近接攻撃を受けない",1600));
			}
		}
	}
	//衛生兵関連
	{
		size_t physicAttackedCount=0;//物理攻撃被弾回数
		int totalRecoverPoint=0;//合計回復量
		int totalDamage=0;//合計被ダメージ
		for(std::vector<std::shared_ptr<LogElement>>::const_iterator it=m_logList.begin(),ite=m_logList.end();it!=ite;it++){
			if((*it)->GetLogKind()==LogElement::LogKind::e_attack){
				const std::shared_ptr<const AttackLog> attackLog=std::dynamic_pointer_cast<AttackLog>(*it);
				if(attackLog.get()!=nullptr
					&& attackLog->GetOperateUnitData().punit!=nullptr
					&& attackLog->GetAimedUnit()!=nullptr)
				{
					//物理攻撃被弾
					if(attackLog->GetOperateUnitData().punit->GetBattleStatus().team==Unit::Team::e_enemy
						&& attackLog->GetOperateUnitData().punit->GetBattleStatus().weapon->GetActionKind()==DamageCalculator::Kind::e_physicalAttack
						&& attackLog->GetAimedUnit()->GetBattleStatus().team==Unit::Team::e_player
						&& attackLog->GetAimedUnit()->GetBaseStatus().profession==Unit::Profession::e_healer)
					{
						physicAttackedCount++;
					}
					//次の２つは次のログデータを用いて計算するので、その検査をする
					std::vector<std::shared_ptr<LogElement>>::const_iterator next=it;
					next++;
					if(next!=ite){
						//次のデータがあるなら、回復についてのデータを計算できる
						const LogElement::UnitLogData afterData=(*next)->FindUnitData(attackLog->GetAimedUnit());
						int afterHP;
						if(afterData.punit!=nullptr){
							//次のログにaimedUnitが存在していたら、そのログデータを用いる
							afterHP=afterData.hp;
						} else{
							//存在していないということは、HPが0になって死んだということ。
							afterHP=0;
						}
						//回復量
						if(attackLog->GetOperateUnitData().punit->GetBattleStatus().team==Unit::Team::e_player
							&& attackLog->GetOperateUnitData().punit->GetBaseStatus().profession==Unit::Profession::e_healer
							&& attackLog->GetOperateUnitData().punit->GetBattleStatus().weapon->GetActionKind()==DamageCalculator::Kind::e_recover
							&& attackLog->GetAimedUnit()->GetBattleStatus().team==Unit::Team::e_player)
						{
							totalRecoverPoint+=(afterHP-attackLog->GetAimedUnitData().hp);
						}
						//被ダメージ
						if(attackLog->GetOperateUnitData().punit->GetBattleStatus().team==Unit::Team::e_enemy
							&& attackLog->GetAimedUnit()->GetBattleStatus().team==Unit::Team::e_player)
						{
							switch(attackLog->GetOperateUnitData().punit->GetBattleStatus().weapon->GetActionKind()){
							case(DamageCalculator::Kind::e_physicalAttack):
							case(DamageCalculator::Kind::e_magicAttack):
								totalDamage+=(attackLog->GetAimedUnitData().hp-afterHP);
								break;
							}
						}
					}
				}
			}
		}
		const auto judgeExistHealer=[&judgeExistProfession](const LogElement::UnitLogData &logData){
			return judgeExistProfession(logData,Unit::Profession::e_healer);
		};
		if(initLog->JudgeEveryUnitData(judgeExistHealer,false)){
			//味方に衛生兵がいる場合、ボーナス処理をする
			//物理攻撃被弾回数
			if(physicAttackedCount==0){
				bonus.push_back(Bonus("衛生兵が物理攻撃を受けない",3000));
			}
			//回復量
			if(totalDamage>0){
				const double rate=1.0*totalRecoverPoint/totalDamage;
				if(rate>=0.75){
					bonus.push_back(Bonus("回復のスペシャリスト",1600));
				} else if(rate>=0.50){
					bonus.push_back(Bonus("衛生兵がたくさん味方を癒した",900));
				}
			}
		}
	}
	//位置取り関連
	{
		//味方の位置取り
		Bonus playerInfo=m_latticeBonusData.GetPlayerBonus();
		if(playerInfo.GetScore()!=0){
			bonus.push_back(playerInfo);
		}
		//敵の位置取り
		Bonus enemyInfo=m_latticeBonusData.GetEnemyBonus();
		if(enemyInfo.GetScore()!=0){
			bonus.push_back(enemyInfo);
		}
	}
	//敵ユニットを撃破するまでのアクション密度
	{
		std::map<const Unit *,std::pair<bool,std::pair<size_t,size_t>>> enemyAttackTendency;//順番に「どのユニットか」「計測中か」「何回攻撃されたか」「行動が何回起こったか」
		for(const LogElement::UnitLogData &unitData:initLog->m_unitDataList){
			if(unitData.punit!=nullptr
				&& unitData.punit->GetBattleStatus().team==Unit::Team::e_enemy)
			{
				enemyAttackTendency.insert(std::make_pair(unitData.punit,std::make_pair(false,std::make_pair(0,0))));
			}
		}
		for(const std::shared_ptr<const LogElement> &logData:m_logList){
			if(logData->GetOperateUnitData().punit!=nullptr
				&& logData->GetOperateUnitData().punit->GetBattleStatus().team==Unit::Team::e_player
				&& logData->GetOperateUnitData().punit->GetBattleStatus().weapon->GetActionKind()!=DamageCalculator::Kind::e_recover)
			{
				//行動ユニットが「回復役以外の味方」の手番のみ調べる
				if(logData->GetLogKind()==LogElement::LogKind::e_wait){
					for(const LogElement::UnitLogData &unitData:logData->m_unitDataList){
						const auto it=enemyAttackTendency.find(unitData.punit);
						if(it!=enemyAttackTendency.end() && it->second.first){
							//itのユニットは計測中かつ何もされていないので、行動が起こった回数のみ加算
							//なお、itのユニットの消滅は調べる必要がない。m_unitDataListに存在しなくなるため、加算されなくなるから。
							it->second.second.second++;
						}
					}
				} else if(logData->GetLogKind()==LogElement::LogKind::e_attack){
					const std::shared_ptr<const AttackLog> attackLog=std::dynamic_pointer_cast<const AttackLog>(logData);
					if(attackLog.get()!=nullptr
						&& attackLog->GetAimedUnit()!=nullptr)
					{
						for(const LogElement::UnitLogData &unitData:logData->m_unitDataList){
							const auto it=enemyAttackTendency.find(unitData.punit);
							if(it!=enemyAttackTendency.end()){
								//itのユニットについて、攻撃密度の処理をしていく
								//計測状態更新
								if(!it->second.first){
									//まだ計測中でないなら
									if(it->first==attackLog->GetAimedUnit()){
										//初めてアクションを受けた場合、計測開始にして最初の処理をする
										it->second.first=true;
									}
								}
								//回数加算処理(状態更新を反映させるので、else ifにはしない！)
								if(it->second.first){
									//計測中であれば
									if(it->first==attackLog->GetAimedUnit()){
										//アクションを受けた場合、攻撃された回数と行動回数を加算
										it->second.second.first++;
										it->second.second.second++;
									} else{
										//アクションを受けなかったときは、行動回数のみ加算
										it->second.second.second++;
									}
								}
							}
						}
					}
				}
			}
		}
		//平均密度を求める
		size_t enemyCount=0;
		double totalEachRate=0.0;//それぞれの敵ユニットに対して(攻撃された回数)/(行動回数)を計算して合計を求める。こいつの平均値で評価する。
		for(auto it=enemyAttackTendency.begin(),ite=enemyAttackTendency.end();it!=ite;it++){
			if(it->second.second.second>0){
				enemyCount++;
				totalEachRate+=1.0*it->second.second.first/it->second.second.second;
			}
		}
		//ボーナス処理
		if(enemyCount>0){
			const double rate=totalEachRate/enemyCount;
			if(rate>=0.80){
				bonus.push_back(Bonus("集中攻撃をとても心がけた",3500));
			} else if(rate>=0.60){
				bonus.push_back(Bonus("集中攻撃を心がけた",2600));
			}
		}
	}
	//味方ユニットの待機回数とアクション回数
	{
		size_t orderCount=0;//手番の合計回数
		size_t actionCount=0;//アクション回数
		for(const std::shared_ptr<const LogElement> &logData:m_logList){
			if(logData->GetOperateUnitData().punit!=nullptr
				&& logData->GetOperateUnitData().punit->GetBattleStatus().team==Unit::Team::e_player)
			{
				switch(logData->GetLogKind()){
				case(LogElement::LogKind::e_attack):
					actionCount++;
				case(LogElement::LogKind::e_wait):
					orderCount++;
					break;
				}
			}
		}
		//ボーナス計算
		if(orderCount>0){
			const double rate=1.0*actionCount/orderCount;
			if(rate>=0.80){
				bonus.push_back(Bonus("待機行動を殆ど行わなかった",2400));
			} else if(rate>=0.60){
				bonus.push_back(Bonus("待機行動が控えめだった",1300));
			}
		}
	}
	//移動キャンセル回数
	{
		if(m_cancelCount==0){
			bonus.push_back(Bonus("ノー移動キャンセル",2200));
		}
	}
	//合計移動距離
	{
		double totalMoveDistance=0.0;
		for(const std::shared_ptr<const LogElement> &logData:m_logList){
			switch(logData->GetLogKind()){
			case(LogElement::LogKind::e_wait):
			case(LogElement::LogKind::e_attack):
			{
				if(logData->GetOperateUnitData().punit!=nullptr
					&& logData->GetOperateUnitData().punit->GetBattleStatus().team==Unit::Team::e_player)
				{
					const float moveOP=Unit::BattleStatus::maxOP-Unit::reduceStartActionCost-logData->GetOperateUnitData().op;
					const int movePal=logData->GetOperateUnitData().punit->GetBaseStatus().move;
					totalMoveDistance+=moveOP*movePal;
				}
			}
			break;
			}
		}
		//ボーナス処理
		if(totalMoveDistance==0.0f){
			bonus.push_back(Bonus("移動せずにクリア",20000));
		}
	}
	//探索使用回数
	{
		if(m_researchCount==0){
			bonus.push_back(Bonus("偵察いらずの用兵術",3000));
		} else if(m_researchCount>=10){
			bonus.push_back(Bonus("偵察のプロフェッショナル",400));
		}
	}

	//ボーナスを全て計算したら、スコア計算
	int score=turnScore+liveScore;
	for(const Bonus &b:bonus){
		score+=b.GetScore();
	}
	
	return std::make_shared<ScoreExpression>(bonus,livePlayerUnitCount,totalPlayerUnitCount,totalEnemyUnitCount-liveEnemyUnitCount,totalEnemyUnitCount,clearTurn,score);
}

std::shared_ptr<const LogElement> ScoreObserver::GetLatestLog()const{
	if(m_logList.empty()){
		//ログが存在しない場合は、無を返す
		return std::shared_ptr<const LogElement>();
	} else{
		return m_logList.back();
	}
}

ScoreObserver::ScoreObserver()
	:m_researchCount(0)
	,m_cancelCount(0)
	,m_logList()
	,m_latticeBonusData()
	,m_submission()
{
	//ひとまずここで初期化しておく
	m_submission.InitRubric(std::shared_ptr<SubmissionRuleBase>(new ProtectFriend()));
}

ScoreObserver::~ScoreObserver(){}