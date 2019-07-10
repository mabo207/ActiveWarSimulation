#include"EditUnitParameter.h"
#include"DxLib.h"
#include"EditActionSettings.h"

//----------------------EditUnitParameter::EditUnitParameterButton-----------------------
EditUnitParameter::EditUnitParameterButton::EditUnitParameterButton(Vector2D point,Vector2D vec)
	:EditActionButton(point,vec,"EditUnit")
{}

void EditUnitParameter::EditUnitParameterButton::PushedProcess(EditActionSettings &settings)const{
	settings.CancelEditing();
	settings.m_pEditAction=std::shared_ptr<EditAction>(new EditUnitParameter(
		(int)m_point.x
		,(int)m_point.y
		,(int)m_vec.x
		,(int)m_vec.y
		,GetColor(255,255,0)));;
	settings.InitEditObject();
}

//----------------------EditUnitParameter-----------------------
const size_t EditUnitParameter::editItemNum=7;

EditUnitParameter::EditUnitParameter(int buttonX,int buttonY,int buttonDX,int buttonDY,unsigned int pushedColor)
	:EditAction(buttonX,buttonY,buttonDX,buttonDY,pushedColor)
	,m_editIndex(0)
{}

EditUnitParameter::~EditUnitParameter(){}

void EditUnitParameter::VNonPressEditing(Vector2D,EditActionSettings &)const{
	//特に何もしない
}

void EditUnitParameter::VProcessAction(Vector2D point,EditActionSettings &settings){
	if(VGetPosSetKind(settings)==NONEDIT){
		//編集対象が決まっていない場合、pointの地点にあるUnitを探す
		settings.SetEditObject(point);
		if(settings.m_pBattleObject.get()!=nullptr && settings.m_pBattleObject->GetType()!=BattleObject::Type::e_unit){
			//非ユニットは編集対象にしない
			settings.InitEditObject();
		} else{
			//m_editResultの設定
			m_editResult=std::dynamic_pointer_cast<Unit>(settings.m_pBattleObject);
			//indexを初期化
			m_editIndex=0;
		}
	} else{
		//編集対象が決まっている場合、編集が終わったものとする
		//ユニットを置き換える
		settings.ReplaceBattleObject(m_editResult);
		settings.InitEditObject();
		m_editResult=std::shared_ptr<Unit>(nullptr);
	}
}

EditAction::PosSetKind EditUnitParameter::VGetPosSetKind(const EditActionSettings &settings)const{
	if(settings.m_pBattleObject){
		//編集対象が決まっている場合
		return BASEEXIST;
	} else{
		//編集対象が決まっていない場合
		return NONEDIT;
	}
}

void EditUnitParameter::EditParameter(bool up,bool down,bool left,bool right){
	std::shared_ptr<Unit> punit=m_editResult;
	if(!punit){
		//編集できない場合はreturn
		return;
	}
	//キーボード入力で、編集項目をいじる
	if(up){
		m_editIndex=(m_editIndex+editItemNum-1)%editItemNum;
	} else if(down){
		m_editIndex=(m_editIndex+1)%editItemNum;
	} else if(left || right){
		const int gap=(left?-1:1);
		bool reduceHP=false;//HPを減らす作業をするかどうか
		bool searchWeapon=false;//武器を選びなおす作業をするかどうか
		int lv=punit->GetBaseStatus().lv;
		Unit::Profession::Kind profession=punit->GetBaseStatus().profession;
		Unit::AIType::Kind aiType=punit->GetBattleStatus().aitype;
		int aiGroup=punit->GetBattleStatus().aiGroup;
		int hp=punit->GetBattleStatus().HP;
		Unit::Team::Kind team=punit->GetBattleStatus().team;
		std::string weaponResisterName=punit->GetBattleStatus().weapon->GetResisterName();
		int weaponSelectGap=0;
		//項目の編集の仕方は項目によって変化する
		if(m_editIndex==0){
			//LV
			const int tmp=lv+gap;
			if(tmp>0){
				lv=tmp;
			}
		} else if(m_editIndex==1){
			//team
			team=Unit::Team::link((team+Unit::Team::END+gap)%Unit::Team::END);
			reduceHP=true;
		} else if(m_editIndex==2){
			//profession
			profession=Unit::Profession::link((profession+Unit::Profession::END+gap)%Unit::Profession::END);
			searchWeapon=true;
		} else if(m_editIndex==3){
			//AItype
			aiType=Unit::AIType::link((aiType+Unit::AIType::END+gap)%Unit::AIType::END);
			reduceHP=true;
		} else if(m_editIndex==4){
			//AIgroup
			const int tmp=aiGroup+gap;
			if(tmp>=0){
				aiGroup=tmp;
			}
			reduceHP=true;
		} else if(m_editIndex==5){
			//initHP
			const int tmp=hp+gap;
			if(tmp>0 && tmp<=punit->GetBaseStatus().maxHP){
				hp=tmp;
			}
			reduceHP=true;
		} else if(m_editIndex==6){
			//weapon(兵種が変わっても武器が変わるので、後で編集)
			weaponSelectGap=gap;
			searchWeapon=true;
		}
		//名前の決定（よく用いる命名パターンを置いておく）
		std::string name="Mob";
		if(team==Unit::Team::e_player){
			//味方の場合は固有名詞
			switch(profession){
			case Unit::Profession::e_soldier:
				name="アインス";
				break;
			case Unit::Profession::e_archer:
				name="ツヴァイ";
				break;
			case Unit::Profession::e_armer:
				name="フィーア";
				break;
			case Unit::Profession::e_mage:
				name="ドライ";
				break;
			case Unit::Profession::e_healer:
				name="フンフ";
				break;
			}
		} else{
			//敵の場合はモブの名前
			name="敵兵";
		}
		//武器の選択
		if(searchWeapon){
			//武器リストを取得
			const std::vector<std::shared_ptr<Weapon>> weaponList=Weapon::GetKindVecSorted(EditActionSettings::ProfessionToWeaponKind(profession));
			//現在選択している武器のindexを探す
			const size_t siz=weaponList.size();
			size_t index;
			for(index=0;index<siz;index++){
				if(weaponList[index]->GetResisterName()==weaponResisterName){
					break;
				}
			}
			//武器登録名を返す
			if(index<siz){
				//現在の武器名がweaponListにあった場合はweaponSelectGapに従って返す
				weaponResisterName=weaponList[(index+weaponSelectGap)%siz]->GetResisterName();
			} else{
				//現在の武器名がweaponListになかった場合はweaponListの先頭を返す
				weaponResisterName=weaponList.front()->GetResisterName();
			}
		}
		//編集内容を反映
		m_editResult=std::shared_ptr<Unit>(Unit::CreateMobUnit(name,profession,lv,weaponResisterName,punit->getPos(),team,aiType,aiGroup,punit->GetBattleStatus().aiLinkage));
		if(reduceHP){
			//AI,チーム,HPを編集した際は、HPを減らす処理をする
			m_editResult->AddHP(hp-m_editResult->GetBattleStatus().HP);//HPの初期化
		}
	}
}
