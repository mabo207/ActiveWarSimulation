#ifndef DEF_MOVESCENE_H
#define DEF_MOVESCENE_H

#include"BattleSceneData.h"
#include"BattleSceneElement.h"
#include"Unit.h"

//�o�g���̈ړ���ʂɂ��Ă̋L�q
//COM�ɂ��ړ��͂������p�����č��΃��N����
class MoveScene:public BattleSceneElement{
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

	//�O���t�B�b�N�֘A
	static const size_t attackedCursorPicNum=4;//������
	int m_attackedCursor[attackedCursorPicNum];//m_aimedUnit�̏�ɏ悹��J�[�\���̕`��
	int m_operatedCursor[attackedCursorPicNum];//m_operatedUnit�̏�ɏ悹��J�[�\���̕`��

	//�֐�
protected:
	bool PositionUpdate(const Vector2D inputVec);//���j�b�g�̈ʒu���X�V�Am_operateUnit�Ɉړ����삪������true��Ԃ��B
	void FinishUnitOperation();//���̃��j�b�g�ւ̑J�ڏ���
	void SetAimedUnit(int turntimes);//m_aimedUnit��m_operatedUnit�̈ʒu����angle�������I�ɋ��߂�SetAimedUnit
	void SetAimedUnit(float angle,int turntimes);//m_aimedUnit��ύX����Bturntimes�������v���ɉ�]������
	bool JudgeAttackCommandUsable()const;//�U���R�}���h���g�p�\���ǂ���
	bool JudgeBecomeAimedUnit(const Unit *punit)const;//punit��m_aimedUnit�ɂȂ邱�Ƃ��o���邩�ǂ����𔻒肷��

	//���z�֐��̃I�[�o�[���C�h
	//int thisCalculate();//�v���C���[���G���ňقȂ�̂�
	virtual void thisDraw()const;//AI�̃f�o�b�O�̂��߂̕`�悪�ł���悤�ɂ���
	int UpdateNextScene(int index);
	void ReturnProcess();

	//���z�֐�
	virtual Vector2D CalculateInputVec()const=0;//�����^�[�����͓��͂���Ă�A�i���O�X�e�B�b�N�̃x�N�g����Ԃ��B�G�^�[������AI�ɏ]���Ĉړ������̃x�N�g����Ԃ��B

public:
	MoveScene(std::shared_ptr<BattleSceneData> battleSceneData);
	virtual ~MoveScene();
};


#endif // !DEF_MOVESCENE_H


#pragma once
