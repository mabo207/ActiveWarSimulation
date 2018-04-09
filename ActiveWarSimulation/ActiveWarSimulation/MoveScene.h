#ifndef DEF_MOVESCENE_H
#define DEF_MOVESCENE_H

#include"BattleSceneData.h"
#include"GameScene.h"
#include"Unit.h"

//�o�g���̈ړ���ʂɂ��Ă̋L�q
//COM�ɂ��ړ��͂������p�����č��΃��N����
class MoveScene:public GameScene{
	//�^�E�񋓑�
public:
	struct RouteInfo{
		const Vector2D pos;
		const float OP;
		RouteInfo(Vector2D i_pos,float i_OP):pos(i_pos),OP(i_OP){}
	};

	//�萔
protected:
	static const float routeFrequency;//OP���ǂ̂��炢����邽�т�m_route�ɋL�^���邩

public:

	//�ϐ�
protected:
	//�Q�[���̐i�s���Ǘ�����ϐ�


	//�Q�[���Ɏg�p����ϐ�
	std::shared_ptr<BattleSceneData> m_battleSceneData;//�Q�[�����S�̂Ŏg���f�[�^

	//���j�b�g�ړ���ʂł̂ݎg�p����ϐ�
	Unit *m_aimedUnit;//���݂̍U���Ώۃ��j�b�g(�ÓI�|�C���^)
	std::vector<RouteInfo> m_route;//���샆�j�b�g���ʂ����o�H

	//�֐�
protected:
	Vector2D CalculateInputVec()const;//�����^�[�����͓��͂���Ă�A�i���O�X�e�B�b�N�̃x�N�g����Ԃ��B�G�^�[������AI�ɏ]���Ĉړ������̃x�N�g����Ԃ��B
	bool PositionUpdate(const Vector2D inputVec);//���j�b�g�̈ʒu���X�V�Am_operateUnit�Ɉړ����삪������true��Ԃ��B
	void FinishUnitOperation();//���̃��j�b�g�ւ̑J�ڏ���
	void SetAimedUnit(float angle=0.0f,int turntimes=1);//m_aimedUnit��ύX����Bturntimes�������v���ɉ�]������
	void ProcessAttack();//�U���������s��
	bool JudgeAttackCommandUsable()const;//�U���R�}���h���g�p�\���ǂ���

public:
	MoveScene(std::shared_ptr<BattleSceneData> battleSceneData);
	~MoveScene();
	int Calculate();
	void Draw()const;
};


#endif // !DEF_MOVESCENE_H


#pragma once
