#include"Weapon.h"
#include"Unit.h"
#include<algorithm>

//---------------Weapon----------------
const float Weapon::closeAttackLength=Unit::rivalInpenetratableCircleSize*1.15f;
const float Weapon::openAttackLength=Weapon::closeAttackLength*2.0f;
const float Weapon::longAttackLength=Weapon::closeAttackLength*3.0f;

DamageCalculator::Kind Weapon::GetActionKind()const{
	return m_calculator->GetKind();
}

Weapon::AttackInfo Weapon::GetAttackInfo(const Unit *attacker,const Unit *defender)const{
	AttackInfo info;
	//各情報の計算
	info.damage=m_calculator->VCalculateDamage(attacker,defender);
	//結果を返す
	return info;
}

bool Weapon::JudgeWeild(const Unit *attacker,const Unit *defender)const{
	return m_calculator->VJudgeWeild(attacker,defender);
}

std::string Weapon::GetEffectivenessString(const Unit *attacker)const{
	//1行目に武器攻撃力と合計攻撃力（効果によって文面が異なる）、2行目に射程とコストを出力する。
	std::string ret;
	ret+=m_calculator->VGetPowerString(attacker);
	ret+="\n";
	ret+="射程："+std::to_string((int)m_length)+"　コスト："+std::to_string((int)m_cost);
	return ret;
}

void Weapon::DrawPredict(const int centerX,const int centerY,const int explainFont,const int numberFont,const Unit *attacker,const Unit *defender)const{
	//DamageCalculatorに投げる
	m_calculator->VDrawPredict(centerX,centerY,explainFont,numberFont,attacker,defender);
}

void Weapon::DrawExtendPredict(const int centerX,const int centerY,const double exRate,const int explainFont,const int numberFont,const Unit *attacker,const Unit *defender)const{
	//DamageCalculatorに投げる
	m_calculator->VDrawExtendPredict(centerX,centerY,exRate,explainFont,numberFont,attacker,defender);
}

//以下静的変数
const std::map<std::string,std::shared_ptr<Weapon>> Weapon::s_weaponMap=Weapon::InitWeaponMap();

std::pair<std::string,std::shared_ptr<Weapon>> Weapon::CreateWeaponMapElement(const std::shared_ptr<Weapon> &weapon){
	return std::pair<std::string,std::shared_ptr<Weapon>>(weapon->GetResisterName(),weapon);
}

std::map<std::string,std::shared_ptr<Weapon>> Weapon::InitWeaponMap(){
	std::map<std::string,std::shared_ptr<Weapon>> map;
	//以下列挙してmapに格納していく
	map.insert(CreateWeaponMapElement(std::shared_ptr<Weapon>(new Weapon("青銅の剣","sword_bronze",Kind::e_sword,5,Weapon::closeAttackLength,50.0f,std::shared_ptr<DamageCalculator>(new PhysicalCalculator()),"低価格な入門用の剣。",AttackInfo(0)))));
	map.insert(CreateWeaponMapElement(std::shared_ptr<Weapon>(new Weapon("鉄の剣","sword_iron",Kind::e_sword,6,Weapon::closeAttackLength,50.0f,std::shared_ptr<DamageCalculator>(new PhysicalCalculator()),"誰でも扱いやすい標準的な剣。",AttackInfo(0)))));
	map.insert(CreateWeaponMapElement(std::shared_ptr<Weapon>(new Weapon("鋼の剣","sword_steel",Kind::e_sword,7,Weapon::closeAttackLength,50.0f,std::shared_ptr<DamageCalculator>(new PhysicalCalculator()),"少し扱いづらいが威力のある剣。",AttackInfo(0)))));
	map.insert(CreateWeaponMapElement(std::shared_ptr<Weapon>(new Weapon("銀の剣","sword_silver",Kind::e_sword,8,Weapon::closeAttackLength,50.0f,std::shared_ptr<DamageCalculator>(new PhysicalCalculator()),"高価で切れ味バツグンな剣。",AttackInfo(0)))));
	map.insert(CreateWeaponMapElement(std::shared_ptr<Weapon>(new Weapon("青銅の槍","lance_bronze",Kind::e_lance,5,Weapon::closeAttackLength,50.0f,std::shared_ptr<DamageCalculator>(new PhysicalCalculator()),"低価格な入門用の槍。",AttackInfo(0)))));
	map.insert(CreateWeaponMapElement(std::shared_ptr<Weapon>(new Weapon("鉄の槍","lance_iron",Kind::e_lance,6,Weapon::closeAttackLength,50.0f,std::shared_ptr<DamageCalculator>(new PhysicalCalculator()),"誰でも扱いやすい標準的な槍。",AttackInfo(0)))));
	map.insert(CreateWeaponMapElement(std::shared_ptr<Weapon>(new Weapon("鋼の槍","lance_steel",Kind::e_lance,7,Weapon::closeAttackLength,50.0f,std::shared_ptr<DamageCalculator>(new PhysicalCalculator()),"少し扱いづらいが威力のある槍。",AttackInfo(0)))));
	map.insert(CreateWeaponMapElement(std::shared_ptr<Weapon>(new Weapon("銀の槍","lance_silver",Kind::e_lance,8,Weapon::closeAttackLength,50.0f,std::shared_ptr<DamageCalculator>(new PhysicalCalculator()),"高価でとても鋭利な槍。",AttackInfo(0)))));
	map.insert(CreateWeaponMapElement(std::shared_ptr<Weapon>(new Weapon("青銅の弓","bow_bronze",Kind::e_bow,3,Weapon::longAttackLength,50.0f,std::shared_ptr<DamageCalculator>(new PhysicalCalculator()),"低価格な入門用の弓。",AttackInfo(0)))));
	map.insert(CreateWeaponMapElement(std::shared_ptr<Weapon>(new Weapon("鉄の弓","bow_iron",Kind::e_bow,4,Weapon::longAttackLength,50.0f,std::shared_ptr<DamageCalculator>(new PhysicalCalculator()),"誰でも扱いやすい標準的な弓。",AttackInfo(0)))));
	map.insert(CreateWeaponMapElement(std::shared_ptr<Weapon>(new Weapon("鋼の弓","bow_steel",Kind::e_bow,5,Weapon::longAttackLength,50.0f,std::shared_ptr<DamageCalculator>(new PhysicalCalculator()),"少し扱いづらいが威力のある弓。",AttackInfo(0)))));
	map.insert(CreateWeaponMapElement(std::shared_ptr<Weapon>(new Weapon("銀の弓","bow_silver",Kind::e_bow,6,Weapon::longAttackLength,50.0f,std::shared_ptr<DamageCalculator>(new PhysicalCalculator()),"反発力が強い高価な弓。",AttackInfo(0)))));
	map.insert(CreateWeaponMapElement(std::shared_ptr<Weapon>(new Weapon("ファイアーの書","book_fire",Kind::e_book,5,Weapon::openAttackLength,60.0f,std::shared_ptr<DamageCalculator>(new MagicCalculator()),"炎の基本が詰まった入門の魔道書。",AttackInfo(0)))));
	map.insert(CreateWeaponMapElement(std::shared_ptr<Weapon>(new Weapon("フレイムの書","book_flame",Kind::e_book,6,Weapon::openAttackLength,60.0f,std::shared_ptr<DamageCalculator>(new MagicCalculator()),"誰でも扱いやすい炎の魔道書。",AttackInfo(0)))));
	map.insert(CreateWeaponMapElement(std::shared_ptr<Weapon>(new Weapon("ブレイズの書","book_blaze",Kind::e_book,7,Weapon::openAttackLength,60.0f,std::shared_ptr<DamageCalculator>(new MagicCalculator()),"少し読むのが難しい炎の魔道書。",AttackInfo(0)))));
	map.insert(CreateWeaponMapElement(std::shared_ptr<Weapon>(new Weapon("ボルガノの書","book_volcano",Kind::e_book,8,Weapon::openAttackLength,60.0f,std::shared_ptr<DamageCalculator>(new MagicCalculator()),"炎のエキスパート向けの魔道書。",AttackInfo(0)))));
	map.insert(CreateWeaponMapElement(std::shared_ptr<Weapon>(new Weapon("ヒールの杖","rod_heal",Kind::e_rod,5,Weapon::closeAttackLength,50.0f,std::shared_ptr<RecoverCalculator>(new RecoverCalculator()),"ちょっとした怪我に使える市販の杖。\n【特殊効果】行動開始時に装備キャラのHPを1回復",AttackInfo(-1)))));
	map.insert(CreateWeaponMapElement(std::shared_ptr<Weapon>(new Weapon("レストの杖","rod_rest",Kind::e_rod,7,Weapon::closeAttackLength,50.0f,std::shared_ptr<RecoverCalculator>(new RecoverCalculator()),"そばにいる人に癒しを与える杖。\n【特殊効果】行動開始時に装備キャラのHPを1回復",AttackInfo(-1)))));
	map.insert(CreateWeaponMapElement(std::shared_ptr<Weapon>(new Weapon("キュアの杖","rod_cure",Kind::e_rod,9,Weapon::closeAttackLength,50.0f,std::shared_ptr<RecoverCalculator>(new RecoverCalculator()),"回復の力を強める不思議な杖。\n【特殊効果】行動開始時に装備キャラのHPを2回復",AttackInfo(-2)))));
	map.insert(CreateWeaponMapElement(std::shared_ptr<Weapon>(new Weapon("リカバーの杖","rod_recover",Kind::e_rod,11,Weapon::closeAttackLength,50.0f,std::shared_ptr<RecoverCalculator>(new RecoverCalculator()),"ケガがすぐ治ると評判の杖。\n【特殊効果】行動開始時に装備キャラのHPを2回復",AttackInfo(-2)))));
	//mapを返す
	return map;
}

const std::shared_ptr<Weapon> Weapon::GetWeapon(const std::string &name){
	std::map<std::string,std::shared_ptr<Weapon>>::const_iterator it=s_weaponMap.find(name);
	if(it!=s_weaponMap.end()){
		return it->second;
	} else{
		return nullptr;
	}
}

std::vector<std::shared_ptr<Weapon>> Weapon::GetKindVecSorted(const Kind weaponKind){
	//特定の武器種の武器を探す
	std::vector<std::shared_ptr<Weapon>> vec;
	for(std::map<std::string,std::shared_ptr<Weapon>>::const_iterator it=s_weaponMap.begin(),ite=s_weaponMap.end();it!=ite;it++){
		if(it->second->GetKind()==weaponKind){
			vec.push_back(it->second);
		}
	}
	//ソートする
	const auto compare=[](const std::shared_ptr<Weapon> &left,const std::shared_ptr<Weapon> &right){
		//コストの比較
		const float leftCost=left->GetCost(),rightCost=right->GetCost();
		if(leftCost<rightCost){
			return true;
		} else if(leftCost>rightCost){
			return false;
		}
		//射程の比較
		const float leftLength=left->GetLength(),rightLength=right->GetLength();
		if(leftLength<rightLength){
			return true;
		} else if(leftLength>rightLength){
			return false;
		}
		//最後に威力の比較
		return (left->GetPower()<right->GetPower());
	};
	std::sort(vec.begin(),vec.end(),compare);
	//返す
	return vec;
}
