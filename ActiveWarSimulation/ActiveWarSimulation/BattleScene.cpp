#include"DxLib.h"
#include"BattleScene.h"
#include"input.h"
#include"Edge.h"
#include<algorithm>

//----------------------BattleScene----------------------
const float BattleScene::routeFrequency=1.0f;

BattleScene::BattleScene(const char *stagename)
	:GameScene(),m_Window(new Terrain(std::shared_ptr<Shape>(new Edge(Vector2D(0.0f,0.0f),Vector2D(1280.0f,720.0f),Shape::Fix::e_ignore)),-1,0,true))
	,m_fpsMesuring()
{
	//�t�@�C������X�e�[�W��ǂݍ���
	//�t�@�C�����J�����ׂĂ̕�����������o��
	std::ifstream ifs("SaveData/stage.txt");
	if(!ifs){

	} else{
		std::string str;//�����o����
		while(true){
			char ch;
			ch=ifs.get();
			//�t�@�C���̏I�[��while����E�o
			if(ch==EOF){
				break;
			}
			str.push_back(ch);
		}
		//�I�u�W�F�N�g�Q��{}�ň͂܂�\n�ŋ�؂��Ă���̂ŁA�P�K�w���������ǂݍ��݂��āA�I�u�W�F�N�g�𐶐�����
		StringBuilder sb(str,'\n','{','}',false,true);
		for(const StringBuilder &ssb:sb.m_vec){
			BattleObject *pb=BattleObject::CreateRawObject(ssb.GetString());
			if(pb!=nullptr){
				m_field.push_back(pb);
			}
		}
	}
	//�t�@�C�����烆�j�b�g��ǂݍ���
	//�ǂݍ��ݕ��@���m�����Ă��Ȃ��̂Ŏb��
	m_field.push_back(Unit::CreateMobUnit(Unit::Profession::e_lancer,1,Vector2D(196.0f,196.0f),Unit::Team::e_player));
	m_field.push_back(Unit::CreateMobUnit(Unit::Profession::e_lancer,1,Vector2D(1024.0f,540.0f),Unit::Team::e_enemy));
	m_field.push_back(Unit::CreateMobUnit(Unit::Profession::e_archer,1,Vector2D(296.0f,196.0f),Unit::Team::e_player));
	m_field.push_back(Unit::CreateMobUnit(Unit::Profession::e_lancer,1,Vector2D(524.0f,340.0f),Unit::Team::e_enemy));
	m_field.push_back(Unit::CreateMobUnit(Unit::Profession::e_armer,1,Vector2D(196.0f,256.0f),Unit::Team::e_player));
	m_field.push_back(Unit::CreateMobUnit(Unit::Profession::e_lancer,1,Vector2D(624.0f,340.0f),Unit::Team::e_enemy));
	m_field.push_back(Unit::CreateMobUnit(Unit::Profession::e_mage,1,Vector2D(100.0f,346.0f),Unit::Team::e_player));
	m_field.push_back(Unit::CreateMobUnit(Unit::Profession::e_lancer,1,Vector2D(1060.0f,440.0f),Unit::Team::e_enemy));
	//m_unitList��m_operateUnit�̏�����
	for(BattleObject *obj:m_field){
		if(obj->GetType()==BattleObject::Type::e_unit){
			m_unitList.push_back(dynamic_cast<Unit *>(obj));
		}
	}
	FinishUnitOperation();

}

BattleScene::~BattleScene(){
	//�I�u�W�F�N�g�ꗗ���J��
	for(BattleObject *obj:m_field){
		delete obj;
	}
}

void BattleScene::UpdateFix(){
	//m_field�̊e�I�u�W�F�N�g��m_fix���X�V����
	//e_ignore�͂��̂܂܁A����ȊO�͑S��e_static�ɂ��Am_operateUnit������e_dynamic�ɂ���
	for(BattleObject *obj:m_field){
		if(obj->GetFix()!=Shape::Fix::e_ignore){
			obj->SetFix(Shape::Fix::e_static);
			if(obj->GetType()==BattleObject::Type::e_unit){
				//unit�ɑ΂��Ă�Penetratable���X�V����
				dynamic_cast<Unit *>(obj)->SetPenetratable(m_operateUnit->GetBattleStatus().team);
			}
		}
	}
	m_operateUnit->SetFix(Shape::Fix::e_dynamic);
}

Vector2D BattleScene::CalculateInputVec()const{
	Vector2D moveVec;
	if(m_operateUnit->GetBattleStatus().team==Unit::Team::e_player){
		//�v���C���[���쎞
		moveVec=analogjoypad_get(DX_INPUT_PAD1);
	} else{
		//�R���s���[�^���쎞�AAI�����������߂�
		//�^�[���J�n����1�b�o������ЂƂ܂��ŋߖT���j�b�g�ɒP���ɋ߂Â�
		if(m_fpsMesuring.GetProcessedTime()>1.0){
			const Unit *nearestUnit=nullptr;
			for(const Unit *pu:m_unitList){
				if(pu->GetBattleStatus().team!=m_operateUnit->GetBattleStatus().team){
					if(nearestUnit==nullptr){
						nearestUnit=pu;
					} else if((pu->getPos()-m_operateUnit->getPos()).sqSize()<(nearestUnit->getPos()-m_operateUnit->getPos()).sqSize()){
						nearestUnit=pu;
					}
				}
			}
			if(nearestUnit!=nullptr){
				moveVec=nearestUnit->getPos()-m_operateUnit->getPos();
			}
		}
	}
	return moveVec;
}

bool BattleScene::PositionUpdate(const Vector2D inputVec){
	//�e�l�̒�`
	const float speed=(float)(m_operateUnit->GetBaseStatus().move);//�I�u�W�F�N�g�̈ړ����x
	const size_t moveCount=5;//�ړ��񐔂̕�����
	const size_t judgeCount=3;//1�ړ����̓����蔻����s��
	const Vector2D beforePos=m_operateUnit->getPos();//�ړ��O�̈ʒu���擾
	Vector2D moveVec;
	//�ړ��x�N�g���̌v�Z
	bool inputFlag=false;//�ړ��̓��͂����������ǂ���
	if(m_operateUnit->GetBattleStatus().OP>0.0f){
		//OP������Ȃ��Ɠ����Ȃ�
		if(inputVec.sqSize()==0.0f){
			inputFlag=false;
		} else{
			inputFlag=true;
			moveVec=inputVec.norm()*std::fminf((float)(speed/moveCount),inputVec.size());
		}
	}
	//�ʒu�X�V���
	for(size_t i=0;i<moveCount;i++){
		//1�t���[������5��ړ�
		//�ړ��������������ɂ��鎖�ŁA�ړ����x�����コ���邱�Ƃ��ł���
		//1��̈ړ����������������Ȃ��ƃM���M���ʂ�Ȃ��ꏊ���ʂ��悤�ɂȂ��Ă��܂�
		if(inputFlag){
			m_operateUnit->Move(moveVec);
		}
		//1�t���[�����ɕ����񓖂��蔻�菈�����s���ƁA�������d���Ȃ����ɃI�u�W�F�N�g�̈ړ����x���グ�邱�Ƃ��ł���
		for(BattleObject *pObject:m_field){
			//�����蔻��n�̏���
			pObject->UpdatePosition(pointer_array_cast<ShapeHaving>(m_field.data()),m_field.size(),judgeCount);
		}
	}
	//�ړ������̌v����OP����
	const float moveCost=(m_operateUnit->getPos()-beforePos).size()/speed;
	m_operateUnit->AddOP(-moveCost);//�����Ȃ̂�cost���}�C�i�X�������̂����Z����

	//�U���Ώۃ��j�b�g�̍X�V(�ړ����Ȃ��Ă�(=inputFlag��false�ł�)���j�b�g�̈ʒu�͓����\��������̂Ŗ����[�v��������)
	bool changeAimedUnitFlag;//�Ώۃ��j�b�g�̕ύX���s����
	float aimedUnitAngle;//�Ώۃ��j�b�g�̂�������
	if(m_aimedUnit!=nullptr){
		//�O�̃t���[���őΏۃ��j�b�g�������ꍇ
		if(m_operateUnit->JudgeAttackable(m_aimedUnit)){
			//���݂̑Ώۃ��j�b�g�����샆�j�b�g�̍U���͈͂ɓ����Ă��鎞�͓��ɉ������Ȃ�
			changeAimedUnitFlag=false;
		} else{
			//���݂̑Ώۃ��j�b�g�����샆�j�b�g�̍U���͈͂ɓ����Ă��Ȃ�����
			changeAimedUnitFlag=true;
			aimedUnitAngle=(m_aimedUnit->getPos()-m_operateUnit->getPos()).GetRadian();
		}
	} else{
		//�O�̃t���[���őΏۃ��j�b�g�����Ȃ������ꍇ
		changeAimedUnitFlag=true;
		aimedUnitAngle=0.0f;
	}
	if(changeAimedUnitFlag){
		//�Ώۃ��j�b�g�̕ύX
		SetAimedUnit(aimedUnitAngle,0);
	}
	//m_route�̒ǉ�
	if(m_route.empty() || m_route.back().OP-m_operateUnit->GetBattleStatus().OP>routeFrequency){
		//routeFrequency���ȑO�̌v���ȍ~�ɏ����OP���z�����猻�݈ʒu�ƌ��݂�OP��m_route�Ɋi�[
		m_route.push_back(RouteInfo(m_operateUnit->getPos(),m_operateUnit->GetBattleStatus().OP));
	}

	return inputFlag;
}

void BattleScene::SortUnitList(){
	std::vector<Unit *> list=m_unitList;//���̔z����R�s�[
	m_unitList.clear();
	//m_unitList�Ƀ\�[�g���ʂ��i�[�BO(n^2)�̎����Ȃ̂Œ�����̂Ȃ璼���������A����OP�̃I�u�W�F�N�g�͑O��œ������Ԃɂ������B
	while(!list.empty()){
		std::vector<Unit *>::const_iterator maxit=list.begin();
		//it��OP�ő�̃I�u�W�F�N�g�̓��A�擪�ɋ߂����̂��i�[
		for(std::vector<Unit *>::const_iterator it=list.begin(),ite=list.end();it!=ite;it++){
			if((*maxit)->GetBattleStatus().OP<(*it)->GetBattleStatus().OP){
				//it�̕����傫���ꍇ
				maxit=it;
			}
		}
		//m_unitList�Ɋi�[���Alist����폜
		m_unitList.push_back(*maxit);
		list.erase(maxit);
	}
}

void BattleScene::FinishUnitOperation(){
	//m_unitList�\�[�g��������
	SortUnitList();
	//�擪��m_operateUnit�Ɋi�[
	m_operateUnit=m_unitList.front();
	//m_operateUnit��OP���ő�ɂȂ�悤��m_unitList�S����OP�l��ω�
	const float plusOP=Unit::BattleStatus::maxOP-m_operateUnit->GetBattleStatus().OP;
	for(Unit *u:m_unitList){
		u->AddOP(plusOP);
	}
	//m_operateUnit��OP�����l���炷
	m_operateUnit->AddOP(-Unit::reduceStartActionCost);
	//�����蔻��}�`�̕ω�
	UpdateFix();
	//m_aimedUnit�̏�����
	SetAimedUnit(0.0f,0);
	//m_route�̏�����
	m_route.clear();
	//�^�C�}�[�Z�b�g
	m_fpsMesuring.RecordTime();

}

void BattleScene::SetAimedUnit(float angle,int turntimes){
	//�͈͓��̃��j�b�g�ꗗ�̍쐬
	std::vector<Unit *> list;
	for(Unit *pUnit:m_unitList){
		if(m_operateUnit->JudgeAttackable(pUnit)){
			//�قȂ�`�[������苗�����ɂ���Βǉ�
			list.push_back(pUnit);
		}
	}
	//�U���Ώۃ��j�b�g��ݒ�
	m_aimedUnit=nullptr;//�ЂƂ܂�nullptr��
	if(!list.empty()){
		//��r�֐��̍쐬�ƕ��בւ��Bm_operateUnit����̃x�N�g���̌����Ń\�[�g����
		const Vector2D pos=m_operateUnit->getPos();
		auto f=[&pos](Unit *lobj,Unit *robj)->bool{
			const float langle=(lobj->getPos()-pos).GetRadian(),rangle=(robj->getPos()-pos).GetRadian();
			return langle<rangle;
		};
		std::sort(list.begin(),list.end(),f);
		//aimedUnitAngle�Ɏ��v�������ɋ߂����j�b�g��T��(turntimes��1�ł��郆�j�b�g�̌��o)
		std::vector<Unit *>::const_iterator it=list.begin(),ite=list.end();
		for(;it!=ite;it++){
			if(angle<((*it)->getPos()-pos).GetRadian()){
				//���߂�aimedUnitAngle�𒴂������ɂ���I�u�W�F�N�g�������I�u�W�F�N�g�Ƃ���
				break;
			}
		}
		if(it==list.end()){
			it=list.begin();
		}
		//turntimes��̉�]�ɍ��킹��B�����_�ł�turntimes=1�̃��j�b�g�����߂��Ă���B
		if(turntimes>0){
			//���̕����̉�]�́Aite�ɒB������list.begin()�ɖ߂��������C������Ηǂ�
			for(int i=1;i<turntimes;i++){
				it++;
				if(it==ite){
					it=list.begin();
				}
			}
		} else{
			//0�ȉ��̉�]�́Aangle�ɂ҂����荇�킳�����ꍇ�Ƃ����łȂ��ꍇ�ňقȂ�
			std::vector<Unit *>::const_iterator beforeit=it;//turntimes=1�̒��O�̃C�e���[�^
			if(beforeit==list.begin()){
				beforeit=list.end();
			}
			beforeit--;
			//it��turntimes=0�̏ꏊ�ɍ��킹��
			if(angle==((*beforeit)->getPos()-pos).GetRadian()){
				//angle�ɂ҂����荇�킳�������j�b�g�����݂���ꍇ
				it=beforeit;//it��turntimes=1�̃��j�b�g���1�O�ɖ߂�
			}
			//turntimes��̉�]�ɍ��킹��
			for(int i=0;i>turntimes;i--){
				if(it==list.begin()){
					it=list.end();
				}
				it--;
			}
		}
		//���߂��C�e���[�^�Ɋi�[����Ă��郆�j�b�g��m_aimedUnit�Ɋi�[����
		m_aimedUnit=*it;
	}
}

void BattleScene::ProcessAttack(){
	//�R�X�g�̏���
	m_operateUnit->AddOP(m_operateUnit->CalculateAddOPNormalAttack());
	//���샆�j�b�g���Ώۃ��j�b�g�ւ̍U�����̌v�Z
	Weapon::AttackInfo attackinfo=m_operateUnit->GetBattleStatus().weapon->GetAttackInfo(m_operateUnit,m_aimedUnit);
	//���샆�j�b�g���Ώۃ��j�b�g�ւ̍U������
	int aimedHP=m_aimedUnit->AddHP(-attackinfo.damage);
	if(aimedHP<=0){
		//�Ώۃ��j�b�g��HP��0�ȉ��Ȃ�A�X�e�[�W���烆�j�b�g����菜��
		m_aimedUnit->SetFix(Shape::Fix::e_ignore);//�����蔻��̑Ώۂ����菜��
		//m_unitList����m_aimedUnit����菜��
		for(std::vector<Unit *>::const_iterator it=m_unitList.begin(),ite=m_unitList.end();it!=ite;it++){
			if(*it==m_aimedUnit){
				m_unitList.erase(it);
				break;
			}
		}
		//m_aimedUnit��null��
		m_aimedUnit=nullptr;
	} else{
		//�Ώۃ��j�b�g�������c���Ă���Ȃ甽���������s��
		//������
	}
}

bool BattleScene::JudgeAttackCommandUsable()const{
	return m_aimedUnit!=nullptr && m_operateUnit->GetBattleStatus().OP+m_operateUnit->CalculateAddOPNormalAttack()>=0;
}

int BattleScene::Calculate(){
	if(m_operateUnit->GetBattleStatus().team==Unit::Team::e_player){
		//�������쎞
		//m_operateUnit�̈ʒu�X�V
		if(PositionUpdate(CalculateInputVec())){
			//�ʒu�X�V���������̏���

		} else{
			//�ړ���������Ȃ��������͂��̑��̓��͂��󂯕t����
			if(keyboard_get(KEY_INPUT_Z)==1){
				//�U��
				if(JudgeAttackCommandUsable()){
					//�U���Ώۂ����݂��AOP������Ă���ꍇ�̂ݍU���������s��
					ProcessAttack();//�U������
					FinishUnitOperation();//�s���I������
				}
			} else if(keyboard_get(KEY_INPUT_D)==1){
				//�K�E�Z

			} else if(keyboard_get(KEY_INPUT_A)==1){
				//�_���̃L�����̕ύX(�����v���)
				float angle;
				if(m_aimedUnit!=nullptr){
					angle=(m_aimedUnit->getPos()-m_operateUnit->getPos()).GetRadian();
				} else{
					angle=0.0f;
				}
				SetAimedUnit(angle,-1);
			} else if(keyboard_get(KEY_INPUT_S)==1){
				//�_���̃L�����̕ύX(���v���)
				float angle;
				if(m_aimedUnit!=nullptr){
					angle=(m_aimedUnit->getPos()-m_operateUnit->getPos()).GetRadian();
				} else{
					angle=0.0f;
				}
				SetAimedUnit(angle,1);
			} else if(keyboard_get(KEY_INPUT_C)==1){
				//�A�C�e���̎g�p

			} else if(keyboard_get(KEY_INPUT_V)==1){
				//�ҋ@
				FinishUnitOperation();
			} else if(keyboard_get(KEY_INPUT_X)==1 || keyboard_get(KEY_INPUT_X)>30){
				//�ړ���蒼��(m_route.back()��1�O�̏ꏊ�ɖ߂��Bback()�̈ʒu�͌��݈ʒu�̉\������������)
				if(!m_route.empty()){
					m_route.pop_back();
					if(!m_route.empty()){
						//�����v�f���Ȃ����pop_back()�����Ȃ�
						RouteInfo info=m_route.back();
						m_route.pop_back();
						//���j�b�g���ړ�������
						m_operateUnit->Warp(info.pos);
						//OP���񕜂�����
						m_operateUnit->AddOP(info.OP-m_operateUnit->GetBattleStatus().OP);
					}
					//�ʒu�X�V���s��
					PositionUpdate(Vector2D());
				}
			}
		}
	} else{
		//�G���쎞
		//�������쎞
		//m_operateUnit�̈ʒu�X�V
		const Vector2D beforeVec=m_operateUnit->getPos();
		PositionUpdate(CalculateInputVec());
		const float moveSqLength=(beforeVec-m_operateUnit->getPos()).sqSize();
		const double processedTime=m_fpsMesuring.GetProcessedTime();
		if(m_fpsMesuring.GetProcessedTime()>1.0){
			//1�b�o������s������
			if(JudgeAttackCommandUsable()){
				//�U���Ώۂ����݂��AOP������Ă���ꍇ�̂ݍU���������s��
				ProcessAttack();//�U������
				FinishUnitOperation();//�s���I������
			} else if(m_operateUnit->GetBattleStatus().OP<2.0f || processedTime>10.0 || (moveSqLength<0.1f && processedTime>2.0)){
				//�ړ��ł��Ȃ��Ȃ�����A�܂���10�b�o������A�܂��ړ����������Ȃ��ꍇ�͑ҋ@
				FinishUnitOperation();
			}
		}
	}
	return 0;
}

void BattleScene::Draw()const{
	//�t�B�[���h�̕`��
	for(const BattleObject *obj:m_field){
		if((obj!=m_operateUnit || obj!=m_aimedUnit) && m_Window->JudgeInShapeRect(obj) && !(obj->GetFix()==Shape::Fix::e_ignore && obj->GetType()==BattleObject::Type::e_unit)){
			//���쒆���j�b�g�ƍU���Ώۃ��j�b�g�͍Ō�ɕ`��
			//�E�C���h�E�ɓ����Ă��Ȃ����͕`�悵�Ȃ�
			//�ދp�������j�b�g(type��e_unit����fix��e_ignore)�͕`�悵�Ȃ�
			obj->VDraw();
		}
	}

	//�_���Ă��郆�j�b�g�̕`��
	if(JudgeAttackCommandUsable()){
		m_aimedUnit->BattleObject::VDraw();
		Vector2D pos=m_aimedUnit->getPos();
		DrawTriangleAA(pos.x-15.0f,pos.y-60.0f,pos.x+15.0f,pos.y-60.0f,pos.x,pos.y-30.0f,GetColor(0,255,0),TRUE);
	}

	//�o�H�̕`��
	for(size_t i=0,max=m_route.size();i+1<max;i++){
		DrawLineAA(m_route[i].pos.x,m_route[i].pos.y,m_route[i+1].pos.x,m_route[i+1].pos.y,GetColor(255,255,0),1.0f);
	}

	//���쒆���j�b�g�̕`��
	m_operateUnit->BattleObject::VDraw();
	m_operateUnit->DrawMoveInfo();//�ړ����̕`��
	Vector2D pos=m_operateUnit->getPos();
	DrawTriangleAA(pos.x-15.0f,pos.y-60.0f,pos.x+15.0f,pos.y-60.0f,pos.x,pos.y-30.0f,GetColor(255,255,0),TRUE);


	//�S���j�b�g��HP�Q�[�W�̕`��
	for(const Unit *unit:m_unitList){
		if(m_Window->JudgeInShapeRect(unit) && unit->GetFix()!=Shape::Fix::e_ignore){
			//�E�C���h�E�ɓ����Ă��Ȃ����͕`�悵�Ȃ�
			//�ދp�������j�b�g(type��e_unit����fix��e_ignore)�͕`�悵�Ȃ�
			unit->DrawHPGage();
		}
	}

	//���j�b�g�����f�o�b�O�o��
	int i=0;
	for(const Unit *u:m_unitList){
		printfDx("(Unit[%d])HP:%d OP:%3.3f pos:(%.3f,%.3f)\n",i,u->GetBattleStatus().HP,u->GetBattleStatus().OP,u->getPos().x,u->getPos().y);
		i++;
	}
}
