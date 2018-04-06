#include"DxLib.h"
#include"BattleSceneData.h"
#include"input.h"
#include"Edge.h"
#include<algorithm>

//----------------------BattleSceneData----------------------
BattleSceneData::BattleSceneData(const char *stagename)
	:m_Window(new Terrain(std::shared_ptr<Shape>(new Edge(Vector2D(0.0f,0.0f),Vector2D(1960.0f,1080.0f),Shape::Fix::e_ignore)),-1,0,true))
	,m_fpsMesuring(),m_operateUnit(nullptr)
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
	m_field.push_back(Unit::CreateMobUnit(Unit::Profession::e_lancer,3,Vector2D(524.0f,324.0f),Unit::Team::e_player));
	m_field.push_back(Unit::CreateMobUnit(Unit::Profession::e_archer,3,Vector2D(354.0f,294.0f),Unit::Team::e_player));
	m_field.push_back(Unit::CreateMobUnit(Unit::Profession::e_mage,3,Vector2D(225.0f,519.0f),Unit::Team::e_player));
	m_field.push_back(Unit::CreateMobUnit(Unit::Profession::e_armer,3,Vector2D(306.0f,441.0f),Unit::Team::e_player));
	m_field.push_back(Unit::CreateMobUnit(Unit::Profession::e_lancer,1,Vector2D(786.0f,510.0f),Unit::Team::e_enemy));
	m_field.push_back(Unit::CreateMobUnit(Unit::Profession::e_lancer,1,Vector2D(1596.0f,165.0f),Unit::Team::e_enemy));
	m_field.push_back(Unit::CreateMobUnit(Unit::Profession::e_lancer,1,Vector2D(1740.0f,660.0f),Unit::Team::e_enemy));
	m_field.push_back(Unit::CreateMobUnit(Unit::Profession::e_armer,1,Vector2D(1536.0f,810.0f),Unit::Team::e_enemy));
	//m_unitList��m_operateUnit�̏�����
	for(BattleObject *obj:m_field){
		if(obj->GetType()==BattleObject::Type::e_unit){
			m_unitList.push_back(dynamic_cast<Unit *>(obj));
		}
	}
	//m_unitList�\�[�g��������
	SortUnitList();
	//�^�C�}�[�Z�b�g
	m_fpsMesuring.RecordTime();

}

BattleSceneData::~BattleSceneData(){
	//�I�u�W�F�N�g�ꗗ���J��
	for(BattleObject *obj:m_field){
		delete obj;
	}
}

void BattleSceneData::UpdateFix(){
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

bool BattleSceneData::PositionUpdate(const Vector2D inputVec){
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

	return inputFlag;
}

void BattleSceneData::SortUnitList(){
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

void BattleSceneData::FinishUnitOperation(){
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
	//�^�C�}�[�Z�b�g
	m_fpsMesuring.RecordTime();

}

void BattleSceneData::DrawField(const std::set<const BattleObject *> &notDraw)const{
	for(const BattleObject *obj:m_field){
		if(m_Window->JudgeInShapeRect(obj)
			&& !(obj->GetFix()==Shape::Fix::e_ignore && obj->GetType()==BattleObject::Type::e_unit)
			&& notDraw.find(obj)==notDraw.end())
		{
			//�E�C���h�E�ɓ����Ă��Ȃ����͕`�悵�Ȃ�
			//�ދp�������j�b�g(type��e_unit����fix��e_ignore)�͕`�悵�Ȃ�
			//�`�悵�Ȃ����j�b�g(notDraw�Ɋi�[����Ă������)�͕`�悵�Ȃ�
			obj->VDraw();
		}
	}
}

void BattleSceneData::DrawHPGage()const{
	for(const Unit *unit:m_unitList){
		if(m_Window->JudgeInShapeRect(unit) && unit->GetFix()!=Shape::Fix::e_ignore){
			//�E�C���h�E�ɓ����Ă��Ȃ����͕`�悵�Ȃ�
			//�ދp�������j�b�g(type��e_unit����fix��e_ignore)�͕`�悵�Ȃ�
			unit->DrawHPGage();
		}
	}
}

void BattleSceneData::DrawOrder()const{
	int windowdx,windowdy;
	GetWindowSize(&windowdx,&windowdy);
	DrawBox(0,windowdy-(int)(Unit::unitCircleSize*1.5f),windowdx,windowdy,GetColor(128,128,128),TRUE);//�w�i�̕`��
	for(size_t i=0,size=m_unitList.size();i<size;i++){
		m_unitList[i]->DrawFacePic(Vector2D((i+1)*Unit::unitCircleSize*2.4f,(float)windowdy-Unit::unitCircleSize*1.1f));
	}
}
