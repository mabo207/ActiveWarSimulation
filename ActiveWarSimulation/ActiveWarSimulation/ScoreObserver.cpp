#include"ScoreObserver.h"
#include<algorithm>
#include<cassert>
#include<map>

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
			bonus.push_back(std::pair<std::string,int>("神速進軍",3000));
		} else if(clearTurn<7){
			bonus.push_back(std::pair<std::string,int>("速攻進軍",2000));
		} else if(clearTurn>15){
			bonus.push_back(std::pair<std::string,int>("ノロノロ進軍",100));
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
			bonus.push_back(std::pair<std::string,int>("全員生存",3000));
		} else if(livePlayerUnitCount==1){
			bonus.push_back(std::pair<std::string,int>("ほぼ引き分け",200));
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
				if(minMinRate<pair.second){
					minMinRate=pair.second;
				}
				if(maxMinRate>pair.second){
					maxMinRate=pair.second;
				}
			}
			//最小HP割合が求まれば、それに応じたボーナスを出す
			if(minMinRate>=1.00){
				bonus.push_back(std::pair<std::string,int>("ノーダメージ",5000));
			} else if(minMinRate>=0.75){
				bonus.push_back(std::pair<std::string,int>("HP75%キープ",2000));
			} else if(minMinRate>=0.50){
				bonus.push_back(std::pair<std::string,int>("HP50%キープ",1000));
			} else if(maxMinRate<=0.25){
				bonus.push_back(std::pair<std::string,int>("ギリギリの戦い",500));
			}
		}
	}
	//兵士関連
	{
		size_t moveCount=0;//手番がきた回数
		float totalWalkOP=0.0f;//移動に使った合計OP
		int attackCount=0;//攻撃回数
		for(const std::shared_ptr<LogElement> logData:m_logList){
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
					totalWalkOP+=Unit::BattleStatus::maxOP-operateData.op;
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
					bonus.push_back(std::pair<std::string,int>("遊撃のプロフェッショナル",1500));
				} else if(averageWalkOP>60.0){
					bonus.push_back(std::pair<std::string,int>("遊撃兵士",1000));
				}
			}
			//攻撃回数
			if(attackCount>=finishLog->GetClearTurn()){
				bonus.push_back(std::pair<std::string,int>("生粋のアタッカー",2000));
			}
		}
	}
	//重装兵関連
	{
		size_t magicAttackedCount=0;//魔法で重装兵に攻撃された回数
		size_t magicTotalCount=0;//部隊全体で魔法で攻撃された回数
		size_t physicAttackedCount=0;//物理で重装兵に攻撃された回数
		size_t physicTotalCount=0;//部隊全体で物理で攻撃された回数
		for(const std::shared_ptr<LogElement> logData:m_logList){
			if(logData->GetLogKind()==LogElement::LogKind::e_attack){
				const std::shared_ptr<AttackLog> attackLog=std::dynamic_pointer_cast<AttackLog>(logData);
				if(attackLog.get()!=nullptr
					&& attackLog->GetOperateUnitData().punit!=nullptr
					&& attackLog->GetAimedUnitData().punit!=nullptr
					&& attackLog->GetAimedUnitData().punit->GetBattleStatus().team==Unit::Team::e_player)
				{
					switch(attackLog->GetOperateUnitData().punit->GetBattleStatus().weapon->GetActionKind()){
					case(WeaponActionKind::e_physicalAttack):
						physicTotalCount++;
						if(attackLog->GetAimedUnitData().punit->GetBaseStatus().profession==Unit::Profession::e_armer){
							physicAttackedCount++;
						}
						break;
					case(WeaponActionKind::e_magicAttack):
						magicTotalCount++;
						if(attackLog->GetAimedUnitData().punit->GetBaseStatus().profession==Unit::Profession::e_armer){
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
			//魔法被弾回数
			if(magicAttackedCount==0){
				bonus.push_back(std::pair<std::string,int>("重装兵で魔法攻撃を受けない",2000));
			} else if(magicAttackedCount==magicTotalCount){
				bonus.push_back(std::pair<std::string,int>("重装兵が弱点を突かれ続けた",100));
			}
			//物理被弾回数
			if(physicTotalCount>0){
				double rate=1.0*physicAttackedCount/physicTotalCount;
				if(rate>=0.75){
					bonus.push_back(std::pair<std::string,int>("物理鉄壁",1500));
				} else if(rate>=0.50){
					bonus.push_back(std::pair<std::string,int>("物理壁",1000));
				}
			}
		}
	}
	//射手関連

	return "";
}

ScoreObserver::ScoreObserver()
	:m_researchCount(0)
	,m_cancelCount(0)
	,m_logList()
{}

ScoreObserver::~ScoreObserver(){}