#ifndef DEF_TUTORIALBATTLESCENEDATA_H
#define DEF_TUTORIALBATTLESCENEDATA_H

#include"BattleSceneData.h"
#include"BattleSceneElement.h"
#include"ToolsLib.h"

struct TutorialBattleSceneData:public BattleSceneData{
	//�񋓑́E�^
public:
	//�`���[�g���A���f�[�^�̊��N���X�AMoveScene�ɂ���f�[�^�𐔑����p����K�v������̂ł��̃N���X�Ƀ`���[�g���A���ۑ�N���A�𔻒肷��֐��͗p�ӂ��Ȃ�
	struct TutorialBase{
		enum class TutorialKind{
			e_move
			,e_normalAttack
			,e_wait
			,e_explain
		};
		const TutorialKind m_kind;
		TutorialBase(TutorialKind kind):m_kind(kind){}
		virtual ~TutorialBase(){}
		virtual void DrawSupplement(int font)const=0;//�⑫������`�悷��
	};
	//�ړ��Ɋւ���`���[�g���A���f�[�^���Ǘ�����N���X
	struct MoveTutorial:public TutorialBase{
		const std::shared_ptr<Shape> m_moveTutorialArea;//�ǂ��Ɉړ�����Ηǂ����i�ړ����j�b�g�̒��S�_�����̐}�`�̒��ɓ���Ηǂ��j
		MoveTutorial(std::shared_ptr<Shape> moveTutorialArea);
		~MoveTutorial();
		void DrawSupplement(int font)const;
	};
	//�U���Ɋւ���`���[�g���A���f�[�^���Ǘ�����N���X
	struct AttackTutorial:public TutorialBase{
		const Unit *m_targetUnit;//�ǂ̃��j�b�g���U������΂悢��
		AttackTutorial(const Unit *targetUnit);
		~AttackTutorial();
		void DrawSupplement(int font)const;
	};
	//�ҋ@�Ɋւ���`���[�g���A�����Ǘ�����N���X
	struct WaitTutorial:public TutorialBase{
		WaitTutorial();
		~WaitTutorial();
		void DrawSupplement(int font)const;
	};
	//������\������`���[�g���A�����Ǘ�����N���X
	struct ExplainTutorial:public TutorialBase{
		//PositionControl m_pos;//�����G�̕`�撆�S�ʒu(�����ň�U��~������X�V�����)
		Easing m_rate;//�����G�̊g�嗦(1000���ő�)(�����ň�U��~������X�V�����)
		const int m_gHandle;//�����G�̃f�[�^
		bool m_secondMoveFlag;//2��ڂ̈ړ����n�܂������ǂ���(�`���[�g���A���I���̔���ɗp����)
		ExplainTutorial(const char *gFileName);
		~ExplainTutorial();
		void DrawSupplement(int font)const;//�����G�̕`����s��
		void SetSecondMoveFlag();//2��ڂ̈ړ����n�߂���悤�ɐݒ肷��
	};


	//�萔

	//�ϐ�
public:
	std::vector<std::shared_ptr<TutorialBase>> m_tutorialData;//�ǂ̂悤�ȃ`���[�g���A����񋟂��邩

	//�t�H���g
	const int m_tutorialFont;//�`���[�g���A���̕⑫�����ɗp����t�H���g

	//�֐�
protected:

public:
	TutorialBattleSceneData();
	~TutorialBattleSceneData();


};


#endif // !DEF_TUTORIALBATTLESCENEDATA_H
