#include"DxLib.h"
#include"BattleScene.h"
#include"input.h"
#include"Edge.h"

//----------------------BattleScene----------------------
BattleScene::BattleScene(const char *stagename)
	:GameScene(),m_Window(new Terrain(std::shared_ptr<Shape>(new Edge(Vector2D(0.0f,0.0f),Vector2D(1280.0f,720.0f),Shape::Fix::e_ignore)),-1,0,true))
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
	m_field.push_back(new Unit(Vector2D(196.0f,196.0f),-1,Unit::Team::e_player));
	m_field.push_back(new Unit(Vector2D(1024.0f,540.0f),-1,Unit::Team::e_enemy));
	m_field.push_back(new Unit(Vector2D(296.0f,196.0f),-1,Unit::Team::e_player));
	m_field.push_back(new Unit(Vector2D(524.0f,340.0f),-1,Unit::Team::e_enemy));
	//m_unitList��m_operateUnit�̏�����
	for(BattleObject *obj:m_field){
		if(obj->GetType()==BattleObject::Type::e_unit){
			m_unitList.push_back(dynamic_cast<Unit *>(obj));
		}
	}
	FinishUnitOperation();
	//�����蔻����̍X�V
	UpdateFix();
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

bool BattleScene::PositionUpdate(){
	//�e�l�̒�`
	const float speed=(float)(m_operateUnit->GetBaseStatus().move);//�I�u�W�F�N�g�̈ړ����x
	const size_t moveCount=5;//�ړ��񐔂̕�����
	const size_t judgeCount=3;//1�ړ����̓����蔻����s��
	const Vector2D beforePos=m_operateUnit->getPos();//�ړ��O�̈ʒu���擾
	//�ړ������̌v�Z
	Vector2D moveVec=analogjoypad_get(DX_INPUT_PAD1);
	bool inputFlag;
	if(moveVec.sqSize()==0.0f){
		inputFlag=false;
	} else{
		inputFlag=true;
		moveVec=moveVec.norm()*std::fminf((float)(speed/moveCount),moveVec.size());
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
	m_operateUnit->AddOP(-moveCost);//���ۂȂ̂�cost���}�C�i�X�������̂����Z����

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
	//�����蔻��}�`�̕ω�
	UpdateFix();
}

int BattleScene::Calculate(){
	//m_operateUnit�̈ʒu�X�V
	if(PositionUpdate()){

	} else{
		//�ړ���������Ȃ��������͂��̑��̓��͂��󂯕t����
		if(keyboard_get(KEY_INPUT_Z)==1){
			//�U��

		} else if(keyboard_get(KEY_INPUT_X)==1){
			//�K�E�Z

		} else if(keyboard_get(KEY_INPUT_A)==1){
			//�_���̃L�����̕ύX(�����v���)

		} else if(keyboard_get(KEY_INPUT_S)==1){
			//�_���̃L�����̕ύX(���v���)

		} else if(keyboard_get(KEY_INPUT_C)==1){
			//�A�C�e���̎g�p

		} else if(keyboard_get(KEY_INPUT_V)==1){
			//�ҋ@
			FinishUnitOperation();
		} else if(keyboard_get(KEY_INPUT_D)==1){
			//�ړ���蒼��

		}
	}
	return 0;
}

void BattleScene::Draw()const{
	//�t�B�[���h�̕`��
	for(const BattleObject *obj:m_field){
		if(obj!=m_operateUnit && m_Window->JudgeInShapeRect(obj)){
			//���쒆���j�b�g�͍Ō�ɕ`��
			//�E�C���h�E�ɓ����Ă��Ȃ����͕`�悵�Ȃ�
			obj->VDraw();
		}
	}
	//���쒆���j�b�g�̕`��
	m_operateUnit->BattleObject::VDraw();
	Vector2D pos=m_operateUnit->getPos();
	DrawTriangleAA(pos.x-15.0f,pos.y-60.0f,pos.x+15.0f,pos.y-60.0f,pos.x,pos.y-30.0f,GetColor(255,255,0),TRUE);

	//���j�b�g�����f�o�b�O�o��
	int i=0;
	for(const Unit *u:m_unitList){
		printfDx("(Unit[%d])HP:%d OP:%3.3f pos:(%.3f,%.3f)\n",i,u->GetBattleStatus().HP,u->GetBattleStatus().OP,u->getPos().x,u->getPos().y);
		i++;
	}
}
