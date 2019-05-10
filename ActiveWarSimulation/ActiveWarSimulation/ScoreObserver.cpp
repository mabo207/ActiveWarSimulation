#include"ScoreObserver.h"
#include<algorithm>
#include<cassert>
#include<map>
#include"BattleSceneData.h"

#include"WaitLog.h"
#include"AttackLog.h"
#include"FinishLog.h"
#include"InitLog.h"

void ScoreObserver::InitUpdate(const BattleSceneData * const battleData){
	m_logList.clear();
	m_logList.push_back(std::make_shared<InitLog>(battleData));
}

void ScoreObserver::FinishUpdate(const BattleSceneData * const battleData){
	m_logList.push_back(std::make_shared<FinishLog>(battleData));
}

void ScoreObserver::AttackUpdate(const BattleSceneData * const battleData,const Unit * const aimedUnit){
	m_logList.push_back(std::make_shared<AttackLog>(battleData,aimedUnit));
}

void ScoreObserver::ResearchUpdate(){
	m_researchCount++;
}

void ScoreObserver::WaitUpdate(const BattleSceneData * const battleData){
	m_logList.push_back(std::make_shared<WaitLog>(battleData));
}

void ScoreObserver::CancelUpdate(){
	m_cancelCount++;
}

std::string ScoreObserver::GetScoreExplain()const{
	//スコア計算方法がベタ書きされている、リファクタリングの方法求む
	std::vector<std::pair<std::string,int>> bonus;
	//基本スコア
	std::pair<std::string,int> turnScore;//ターン数
	std::pair<std::string,int> liveScore;//生存数
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
		return "";
	}
	//共通して用いられるデータ
	size_t totalPlayerUnitCount=0,livePlayerUnitCount=0;//プレイヤーユニットの合計と生存数
	const auto judgeExistProfession=[](const LogElement::UnitLogData &logData,Unit::Profession::Kind profession){
		return (logData.punit!=nullptr
			&& logData.punit->GetBaseStatus().profession==profession
			&& logData.punit->GetBattleStatus().team==Unit::Team::e_player);
	};
	//クリアターン数
	{
		//クリアターン数からの得点計算
		const int clearTurn=finishLog->GetClearTurn();
		turnScore.first="クリアターン数： "+std::to_string(clearTurn);
		turnScore.second=std::max(0,20-clearTurn)*500;
		//クリアターン数のボーナス得点
		if(clearTurn<5){
			bonus.push_back(std::make_pair("神速進軍",3000));
		} else if(clearTurn<7){
			bonus.push_back(std::make_pair("速攻進軍",2000));
		} else if(clearTurn>15){
			bonus.push_back(std::make_pair("ノロノロ進軍",100));
		}
	}
	//生存数
	{
		//生存数からの得点計算(寝返りを想定していない)
		size_t totalPlayerUnitCount=0;
		for(const LogElement::UnitLogData &unitData:initLog->m_unitDataList){
			if(unitData.punit!=nullptr && unitData.punit->GetBattleStatus().team==Unit::Team::e_player){
				totalPlayerUnitCount++;
			}
		}
		size_t livePlayerUnitCount=0;
		for(const LogElement::UnitLogData &unitData:finishLog->m_unitDataList){
			if(unitData.punit!=nullptr && unitData.punit->GetBattleStatus().team==Unit::Team::e_player){
				livePlayerUnitCount++;
			}
		}
		liveScore.first="生存数： "+std::to_string(livePlayerUnitCount)+" / "+std::to_string(totalPlayerUnitCount);
		if(totalPlayerUnitCount>0){
			liveScore.second=10000*livePlayerUnitCount/totalPlayerUnitCount;
		} else{
			liveScore.second=0;
		}
		//生存数のボーナス得点
		if(totalPlayerUnitCount-livePlayerUnitCount==0){
			bonus.push_back(std::make_pair("全員生存",3000));
		} else if(livePlayerUnitCount==1){
			bonus.push_back(std::make_pair("ほぼ引き分け",200));
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
				bonus.push_back(std::make_pair("ノーダメージ",5000));
			} else if(minMinRate>=0.75){
				bonus.push_back(std::make_pair("HP75%キープ",2000));
			} else if(minMinRate>=0.50){
				bonus.push_back(std::make_pair("HP50%キープ",1000));
			} else if(maxMinRate<=0.25){
				bonus.push_back(std::make_pair("ギリギリの戦い",500));
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
					bonus.push_back(std::make_pair("遊撃のプロフェッショナル",1500));
				} else if(averageWalkOP>60.0){
					bonus.push_back(std::make_pair("遊撃兵士",1000));
				}
			}
			//攻撃回数
			if(attackCount>=finishLog->GetClearTurn()){
				bonus.push_back(std::make_pair("生粋のアタッカー",2000));
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
					bonus.push_back(std::make_pair("重装兵で魔法攻撃を受けない",2000));
				} else if(magicAttackedCount==magicTotalCount){
					bonus.push_back(std::make_pair("重装兵が弱点を突かれ続けた",100));
				}
			}
			//物理被弾回数
			if(physicTotalCount>0){
				double rate=1.0*physicAttackedCount/physicTotalCount;
				if(rate>=0.75){
					bonus.push_back(std::make_pair("物理鉄壁",1500));
				} else if(rate>=0.50){
					bonus.push_back(std::make_pair("物理壁",1000));
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
					bonus.push_back(std::make_pair("狙撃手",2000));
				} else if(averageRate>=0.50){
					bonus.push_back(std::make_pair("後衛",1000));
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
				bonus.push_back(std::make_pair("弱点狙いの射手",1500));
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
					bonus.push_back(std::make_pair("重装兵キラー",2000));
				} else if(average>=1.0){
					bonus.push_back(std::make_pair("魔道士で重装兵狙い",1000));
				}
			}
			//近接攻撃被弾回数
			if(nearAttackedCount<=0){
				bonus.push_back(std::make_pair("魔道士が近接攻撃を受けない",1500));
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
				bonus.push_back(std::make_pair("衛生兵が物理攻撃を受けない",2000));
			}
			//回復量
			if(totalDamage>0){
				const double rate=1.0*totalRecoverPoint/totalDamage;
				if(rate>=0.75){
					bonus.push_back(std::make_pair("回復のスペシャリスト",1500));
				} else if(rate>=0.50){
					bonus.push_back(std::make_pair("回復役",1000));
				}
			}
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
				bonus.push_back(std::make_pair("集中攻撃をとても心がけた",2500));
			} else if(rate>=0.60){
				bonus.push_back(std::make_pair("集中攻撃を心がけた",1500));
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
				bonus.push_back(std::make_pair("超アクティブバトル",2400));
			} else if(rate>=0.60){
				bonus.push_back(std::make_pair("アクティブバトル",1300));
			}
		}
	}
	//移動キャンセル回数
	{
		if(m_cancelCount==0){
			bonus.push_back(std::make_pair("ノーキャンセル",1200));
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
			bonus.push_back(std::make_pair("移動せずにクリア",6000));
		}
	}
	//探索使用回数
	{
		if(m_researchCount==0){
			bonus.push_back(std::make_pair("偵察いらず",3000));
		} else if(m_researchCount>=10){
			bonus.push_back(std::make_pair("偵察のプロフェッショナル",400));
		}
	}

	//ボーナスを全て計算したら、スコア計算
	int score=turnScore.second+liveScore.second;
	for(const std::pair<std::string,int> &pair:bonus){
		score+=pair.second;
	}
	//説明文の表示
	std::string explain;
	explain+="トータルスコア："+std::to_string(score)+"\n\n";
	explain+=turnScore.first+"　　　"+std::to_string(turnScore.second)+"\n";
	explain+=liveScore.first+"　　　"+std::to_string(liveScore.second)+"\n\n";
	explain+="BONUS SCORE\n";
	for(const std::pair<std::string,int> &pair:bonus){
		explain+=pair.first+"　　　"+std::to_string(pair.second)+"\n";
	}

	return explain;
}

ScoreObserver::ScoreObserver()
	:m_researchCount(0)
	,m_cancelCount(0)
	,m_logList()
{}

ScoreObserver::~ScoreObserver(){}