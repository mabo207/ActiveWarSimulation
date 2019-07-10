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
	//���ɉ������Ȃ�
}

void EditUnitParameter::VProcessAction(Vector2D point,EditActionSettings &settings){
	if(VGetPosSetKind(settings)==NONEDIT){
		//�ҏW�Ώۂ����܂��Ă��Ȃ��ꍇ�Apoint�̒n�_�ɂ���Unit��T��
		settings.SetEditObject(point);
		if(settings.m_pBattleObject.get()!=nullptr && settings.m_pBattleObject->GetType()!=BattleObject::Type::e_unit){
			//�񃆃j�b�g�͕ҏW�Ώۂɂ��Ȃ�
			settings.InitEditObject();
		} else{
			//m_editResult�̐ݒ�
			m_editResult=std::dynamic_pointer_cast<Unit>(settings.m_pBattleObject);
			//index��������
			m_editIndex=0;
		}
	} else{
		//�ҏW�Ώۂ����܂��Ă���ꍇ�A�ҏW���I��������̂Ƃ���
		//���j�b�g��u��������
		settings.ReplaceBattleObject(m_editResult);
		settings.InitEditObject();
		m_editResult=std::shared_ptr<Unit>(nullptr);
	}
}

EditAction::PosSetKind EditUnitParameter::VGetPosSetKind(const EditActionSettings &settings)const{
	if(settings.m_pBattleObject){
		//�ҏW�Ώۂ����܂��Ă���ꍇ
		return BASEEXIST;
	} else{
		//�ҏW�Ώۂ����܂��Ă��Ȃ��ꍇ
		return NONEDIT;
	}
}

void EditUnitParameter::EditParameter(bool up,bool down,bool left,bool right){
	std::shared_ptr<Unit> punit=m_editResult;
	if(!punit){
		//�ҏW�ł��Ȃ��ꍇ��return
		return;
	}
	//�L�[�{�[�h���͂ŁA�ҏW���ڂ�������
	if(up){
		m_editIndex=(m_editIndex+editItemNum-1)%editItemNum;
	} else if(down){
		m_editIndex=(m_editIndex+1)%editItemNum;
	} else if(left || right){
		const int gap=(left?-1:1);
		bool reduceHP=false;//HP�����炷��Ƃ����邩�ǂ���
		bool searchWeapon=false;//�����I�тȂ�����Ƃ����邩�ǂ���
		int lv=punit->GetBaseStatus().lv;
		Unit::Profession::Kind profession=punit->GetBaseStatus().profession;
		Unit::AIType::Kind aiType=punit->GetBattleStatus().aitype;
		int aiGroup=punit->GetBattleStatus().aiGroup;
		int hp=punit->GetBattleStatus().HP;
		Unit::Team::Kind team=punit->GetBattleStatus().team;
		std::string weaponResisterName=punit->GetBattleStatus().weapon->GetResisterName();
		int weaponSelectGap=0;
		//���ڂ̕ҏW�̎d���͍��ڂɂ���ĕω�����
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
			//weapon(���킪�ς���Ă����킪�ς��̂ŁA��ŕҏW)
			weaponSelectGap=gap;
			searchWeapon=true;
		}
		//���O�̌���i�悭�p���閽���p�^�[����u���Ă����j
		std::string name="Mob";
		if(team==Unit::Team::e_player){
			//�����̏ꍇ�͌ŗL����
			switch(profession){
			case Unit::Profession::e_soldier:
				name="�A�C���X";
				break;
			case Unit::Profession::e_archer:
				name="�c���@�C";
				break;
			case Unit::Profession::e_armer:
				name="�t�B�[�A";
				break;
			case Unit::Profession::e_mage:
				name="�h���C";
				break;
			case Unit::Profession::e_healer:
				name="�t���t";
				break;
			}
		} else{
			//�G�̏ꍇ�̓��u�̖��O
			name="�G��";
		}
		//����̑I��
		if(searchWeapon){
			//���탊�X�g���擾
			const std::vector<std::shared_ptr<Weapon>> weaponList=Weapon::GetKindVecSorted(EditActionSettings::ProfessionToWeaponKind(profession));
			//���ݑI�����Ă��镐���index��T��
			const size_t siz=weaponList.size();
			size_t index;
			for(index=0;index<siz;index++){
				if(weaponList[index]->GetResisterName()==weaponResisterName){
					break;
				}
			}
			//����o�^����Ԃ�
			if(index<siz){
				//���݂̕��햼��weaponList�ɂ������ꍇ��weaponSelectGap�ɏ]���ĕԂ�
				weaponResisterName=weaponList[(index+weaponSelectGap)%siz]->GetResisterName();
			} else{
				//���݂̕��햼��weaponList�ɂȂ������ꍇ��weaponList�̐擪��Ԃ�
				weaponResisterName=weaponList.front()->GetResisterName();
			}
		}
		//�ҏW���e�𔽉f
		m_editResult=std::shared_ptr<Unit>(Unit::CreateMobUnit(name,profession,lv,weaponResisterName,punit->getPos(),team,aiType,aiGroup,punit->GetBattleStatus().aiLinkage));
		if(reduceHP){
			//AI,�`�[��,HP��ҏW�����ۂ́AHP�����炷����������
			m_editResult->AddHP(hp-m_editResult->GetBattleStatus().HP);//HP�̏�����
		}
	}
}
