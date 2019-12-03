#ifndef DEF_REFLECTIONWORK_MOVESIMULATION_H
#define DEF_REFLECTIONWORK_MOVESIMULATION_H

#include"ReflectionWorkBase.h"
#include"Unit.h"
#include"LogElement.h"
#include"SubmissionRuleBase.h"

namespace ReflectionWork{
	//�L�����𓮂����Ȃ���V�~�����[�V�����w�K���Ă݂�N���X
	class MoveSimulation:public Base{
	public:
		//�֐�
		MoveSimulation(const std::vector<BattleObject *> &field
			,Unit * const operateUnit
			,const Vector2D stageSize
			,const Unit * const attackedUnit
			,const Vector2D startPos
			,const float mapRate
			,const std::shared_ptr<const SubmissionRuleBase> &rule
			,const std::string &question);
		~MoveSimulation();
		//���z�֐��̃I�[�o�[���C�h
		void Update();
		bool WorkClear()const;
		void WorkDraw()const;

	private:
		//�֐�
		std::shared_ptr<LogElement> CreateLog()const;//���݂̃V�~�����[�V�����w�K�̏�Ԃ����O�`���ɂ���֐�
		
		//�ϐ�
		bool m_clear;//�������I����������Ǘ�����
		SubmissionEvaluation m_evaluate;//���݂̕]��
		//�o�g���}�b�v��\�����邽�߂ɕK�v�ȏ��
		const std::vector<BattleObject *> m_field;//�t�B�[���h
		const Vector2D m_stageSize;//�X�e�[�W�̑傫��
		//���������߂ɕK�v�ȏ��
		Unit *m_operateUnit;//���샆�j�b�g
		const Vector2D m_startPos;//�}�b�v�E��̈ʒu
		const float m_mapRate;//�}�b�v�̔{��
		//���O���쐬���邽�߂ɕK�v�ȏ��
		const Unit *m_attackedUnit;//�U��������郆�j�b�g
		const std::shared_ptr<const SubmissionRuleBase> m_rule;//�T�u�~�b�V�����]����@
		//�O���t�B�b�N�ϐ�
		const int m_font;
	};
}


#endif // !DEF_REFLECTIONWORK_MOVESIMULATION_H