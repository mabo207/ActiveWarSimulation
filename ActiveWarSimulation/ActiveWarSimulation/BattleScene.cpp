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
	//�ŏ��ɑ��삷�郆�j�b�g�̑I��
	for(BattleObject *obj:m_field){
		if(obj->GetType()==BattleObject::Type::e_unit){
			m_operateUnit=dynamic_cast<Unit *>(obj);
			break;
		}
	}
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

int BattleScene::Calculate(){
	//m_operateUnit�̈ʒu�X�V
	const float speed=(float)(m_operateUnit->GetBaseStatus().move);//�I�u�W�F�N�g�̈ړ����x
	const size_t moveCount=5;//�ړ��񐔂̕�����
	const size_t judgeCount=3;//1�ړ����̓����蔻����s��
	const Vector2D beforePos=m_operateUnit->getPos();
	for(size_t i=0;i<moveCount;i++){
		//1�t���[������5��ړ�
		//�ړ��������������ɂ��鎖�ŁA�ړ����x�����コ���邱�Ƃ��ł���
		//1��̈ړ����������������Ȃ��ƃM���M���ʂ�Ȃ��ꏊ���ʂ��悤�ɂȂ��Ă��܂�
		Vector2D v=analogjoypad_get(DX_INPUT_PAD1);
		m_operateUnit->Move(v.norm()*std::fminf((float)(speed/moveCount),v.size()));
		//1�t���[�����ɕ����񓖂��蔻�菈�����s���ƁA�������d���Ȃ����ɃI�u�W�F�N�g�̈ړ����x���グ�邱�Ƃ��ł���
		for(BattleObject *pObject:m_field){
			//�����蔻��n�̏���
			pObject->UpdatePosition(pointer_array_cast<ShapeHaving>(m_field.data()),m_field.size(),judgeCount);
		}
	}
	//�ړ������̌v���ƍs���I������
	const float moveCost=(m_operateUnit->getPos()-beforePos).size()/speed;
	//���̑��̓��͂��󂯕t����
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
		//���̃L�����N�^�[�ɑ�����ڂ�
		for(BattleObject *obj:m_field){
			if(obj->GetType()==BattleObject::Type::e_unit && obj!=m_operateUnit){
				m_operateUnit=dynamic_cast<Unit *>(obj);
				break;
			}
		}
		//���j�b�g�̓����蔻��}�`��ω�������
		UpdateFix();
	} else if(keyboard_get(KEY_INPUT_D)==1){
		//�ړ���蒼��

	}
	return 0;
}

void BattleScene::Draw()const{
	//�t�B�[���h�̕`��
	for(const BattleObject *obj:m_field){
		if(obj!=m_operateUnit){
			//���쒆���j�b�g�͍Ō�ɕ`��
			obj->VDraw();
		}
	}
	//���쒆���j�b�g�̕`��
	m_operateUnit->BattleObject::VDraw();
}
