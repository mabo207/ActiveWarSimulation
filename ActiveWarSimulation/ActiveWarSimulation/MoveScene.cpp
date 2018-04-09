#include"DxLib.h"
#include"MoveScene.h"
#include"input.h"
#include"Edge.h"
#include<algorithm>

//----------------------MoveScene----------------------
const float MoveScene::routeFrequency=1.0f;

MoveScene::MoveScene(std::shared_ptr<BattleSceneData> battleSceneData)
	:GameScene(),m_battleSceneData(battleSceneData)
{
	//m_aimedUnit���̏�����
	FinishUnitOperation();
}

MoveScene::~MoveScene(){}

Vector2D MoveScene::CalculateInputVec()const{
	Vector2D moveVec;
	if(m_battleSceneData->m_operateUnit->GetBattleStatus().team==Unit::Team::e_player){
		//�v���C���[���쎞
		moveVec=analogjoypad_get(DX_INPUT_PAD1);
	} else{
		//�R���s���[�^���쎞�AAI�����������߂�
		//�^�[���J�n����1�b�o������ЂƂ܂��ŋߖT���j�b�g�ɒP���ɋ߂Â�
		if(m_battleSceneData->m_fpsMesuring.GetProcessedTime()>1.0){
			const Unit *nearestUnit=nullptr;
			for(const Unit *pu:m_battleSceneData->m_unitList){
				if(pu->GetBattleStatus().team!=m_battleSceneData->m_operateUnit->GetBattleStatus().team){
					if(nearestUnit==nullptr){
						nearestUnit=pu;
					} else if((pu->getPos()-m_battleSceneData->m_operateUnit->getPos()).sqSize()<(nearestUnit->getPos()-m_battleSceneData->m_operateUnit->getPos()).sqSize()){
						nearestUnit=pu;
					}
				}
			}
			if(nearestUnit!=nullptr){
				moveVec=nearestUnit->getPos()-m_battleSceneData->m_operateUnit->getPos();
			}
		}
	}
	return moveVec;
}

bool MoveScene::PositionUpdate(const Vector2D inputVec){
	//�o�g���f�[�^�̍X�V
	bool inputFlag=m_battleSceneData->PositionUpdate(inputVec);

	//�U���Ώۃ��j�b�g�̍X�V(�ړ����Ȃ��Ă�(=inputFlag��false�ł�)���j�b�g�̈ʒu�͓����\��������̂Ŗ����[�v��������)
	bool changeAimedUnitFlag;//�Ώۃ��j�b�g�̕ύX���s����
	float aimedUnitAngle;//�Ώۃ��j�b�g�̂�������
	if(m_aimedUnit!=nullptr){
		//�O�̃t���[���őΏۃ��j�b�g�������ꍇ
		if(m_battleSceneData->m_operateUnit->JudgeAttackable(m_aimedUnit)){
			//���݂̑Ώۃ��j�b�g�����샆�j�b�g�̍U���͈͂ɓ����Ă��鎞�͓��ɉ������Ȃ�
			changeAimedUnitFlag=false;
		} else{
			//���݂̑Ώۃ��j�b�g�����샆�j�b�g�̍U���͈͂ɓ����Ă��Ȃ�����
			changeAimedUnitFlag=true;
			aimedUnitAngle=(m_aimedUnit->getPos()-m_battleSceneData->m_operateUnit->getPos()).GetRadian();
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
	if(m_route.empty() || m_route.back().OP-m_battleSceneData->m_operateUnit->GetBattleStatus().OP>routeFrequency){
		//routeFrequency���ȑO�̌v���ȍ~�ɏ����OP���z�����猻�݈ʒu�ƌ��݂�OP��m_route�Ɋi�[
		m_route.push_back(RouteInfo(m_battleSceneData->m_operateUnit->getPos(),m_battleSceneData->m_operateUnit->GetBattleStatus().OP));
	}

	return inputFlag;
}

void MoveScene::FinishUnitOperation(){
	//�o�g���f�[�^�̍X�V
	m_battleSceneData->FinishUnitOperation();
	//m_aimedUnit�̏�����
	SetAimedUnit(0.0f,0);
	//m_route�̏�����
	m_route.clear();

}

void MoveScene::SetAimedUnit(float angle,int turntimes){
	//�͈͓��̃��j�b�g�ꗗ�̍쐬
	std::vector<Unit *> list;
	for(Unit *pUnit:m_battleSceneData->m_unitList){
		if(m_battleSceneData->m_operateUnit->JudgeAttackable(pUnit)){
			//�قȂ�`�[������苗�����ɂ���Βǉ�
			list.push_back(pUnit);
		}
	}
	//�U���Ώۃ��j�b�g��ݒ�
	m_aimedUnit=nullptr;//�ЂƂ܂�nullptr��
	if(!list.empty()){
		//��r�֐��̍쐬�ƕ��בւ��Bm_operateUnit����̃x�N�g���̌����Ń\�[�g����
		const Vector2D pos=m_battleSceneData->m_operateUnit->getPos();
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

void MoveScene::ProcessAttack(){
	//�R�X�g�̏���
	m_battleSceneData->m_operateUnit->AddOP(m_battleSceneData->m_operateUnit->CalculateAddOPNormalAttack());
	//���샆�j�b�g���Ώۃ��j�b�g�ւ̍U�����̌v�Z
	Weapon::AttackInfo attackinfo=m_battleSceneData->m_operateUnit->GetBattleStatus().weapon->GetAttackInfo(m_battleSceneData->m_operateUnit,m_aimedUnit);
	//���샆�j�b�g���Ώۃ��j�b�g�ւ̍U������
	int aimedHP=m_aimedUnit->AddHP(-attackinfo.damage);
	if(aimedHP<=0){
		//�Ώۃ��j�b�g��HP��0�ȉ��Ȃ�A�X�e�[�W���烆�j�b�g����菜��
		m_aimedUnit->SetFix(Shape::Fix::e_ignore);//�����蔻��̑Ώۂ����菜��
												  //m_unitList����m_aimedUnit����菜��
		for(std::vector<Unit *>::const_iterator it=m_battleSceneData->m_unitList.begin(),ite=m_battleSceneData->m_unitList.end();it!=ite;it++){
			if(*it==m_aimedUnit){
				m_battleSceneData->m_unitList.erase(it);
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

bool MoveScene::JudgeAttackCommandUsable()const{
	return m_aimedUnit!=nullptr && m_battleSceneData->m_operateUnit->GetBattleStatus().OP+m_battleSceneData->m_operateUnit->CalculateAddOPNormalAttack()>=0;
}

int MoveScene::Calculate(){
	if(m_battleSceneData->m_operateUnit->GetBattleStatus().team==Unit::Team::e_player){
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

			} else if(keyboard_get(KEY_INPUT_A)==1 && JudgeAttackCommandUsable()){
				//�U���R�}���h�g�p�\�̎��̂݁A�_���̃L�����̕ύX(�����v���)
				float angle;
				if(m_aimedUnit!=nullptr){
					angle=(m_aimedUnit->getPos()-m_battleSceneData->m_operateUnit->getPos()).GetRadian();
				} else{
					angle=0.0f;
				}
				SetAimedUnit(angle,-1);
			} else if(keyboard_get(KEY_INPUT_S)==1 && JudgeAttackCommandUsable()){
				//�U���R�}���h�g�p�\�̎��̂݁A�_���̃L�����̕ύX(���v���)
				float angle;
				if(m_aimedUnit!=nullptr){
					angle=(m_aimedUnit->getPos()-m_battleSceneData->m_operateUnit->getPos()).GetRadian();
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
						m_battleSceneData->m_operateUnit->Warp(info.pos);
						//OP���񕜂�����
						m_battleSceneData->m_operateUnit->AddOP(info.OP-m_battleSceneData->m_operateUnit->GetBattleStatus().OP);
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
		const Vector2D beforeVec=m_battleSceneData->m_operateUnit->getPos();
		PositionUpdate(CalculateInputVec());
		const float moveSqLength=(beforeVec-m_battleSceneData->m_operateUnit->getPos()).sqSize();
		const double processedTime=m_battleSceneData->m_fpsMesuring.GetProcessedTime();
		if(m_battleSceneData->m_fpsMesuring.GetProcessedTime()>1.0){
			//1�b�o������s������
			if(JudgeAttackCommandUsable()){
				//�U���Ώۂ����݂��AOP������Ă���ꍇ�̂ݍU���������s��
				ProcessAttack();//�U������
				FinishUnitOperation();//�s���I������
			} else if(m_battleSceneData->m_operateUnit->GetBattleStatus().OP<2.0f || processedTime>10.0 || (moveSqLength<0.1f && processedTime>2.0)){
				//�ړ��ł��Ȃ��Ȃ�����A�܂���10�b�o������A�܂��ړ����������Ȃ��ꍇ�͑ҋ@
				FinishUnitOperation();
			}
		}
	}
	return 0;
}

void MoveScene::Draw()const{
	//�o�H�̕`��
	for(size_t i=0,max=m_route.size();i+1<max;i++){
		DrawLineAA(m_route[i].pos.x,m_route[i].pos.y,m_route[i+1].pos.x,m_route[i+1].pos.y,GetColor(255,255,0),1.0f);
	}

	//�t�B�[���h�̕`��
	m_battleSceneData->DrawField(std::set<const BattleObject *>{m_battleSceneData->m_operateUnit,m_aimedUnit});

	//�_���Ă��郆�j�b�g�̕`��
	if(m_aimedUnit!=nullptr){
		m_aimedUnit->BattleObject::VDraw();
		if(JudgeAttackCommandUsable()){
			//�U���\�Ȃ�}�[�J�[��`��
			Vector2D pos=m_aimedUnit->getPos();
			DrawTriangleAA(pos.x-15.0f,pos.y-60.0f,pos.x+15.0f,pos.y-60.0f,pos.x,pos.y-30.0f,GetColor(0,255,0),TRUE);
		}
	}

	//���쒆���j�b�g�̕`��
	m_battleSceneData->m_operateUnit->BattleObject::VDraw();
	m_battleSceneData->m_operateUnit->DrawMoveInfo();//�ړ����̕`��
	Vector2D pos=m_battleSceneData->m_operateUnit->getPos();
	DrawTriangleAA(pos.x-15.0f,pos.y-60.0f,pos.x+15.0f,pos.y-60.0f,pos.x,pos.y-30.0f,GetColor(255,255,0),TRUE);


	//�S���j�b�g��HP�Q�[�W�̕`��
	m_battleSceneData->DrawHPGage();

	//���j�b�g�̃I�[�_�[���Ԃ�`��
	m_battleSceneData->DrawOrder();

}
