#ifndef DEF_TUTORIALBATTLESCENEDATA_H
#define DEF_TUTORIALBATTLESCENEDATA_H

#include"BattleSceneData.h"
#include"BattleSceneElement.h"

struct TutorialBattleSceneData:public BattleSceneData{
	//�񋓑́E�^
public:
	//�`���[�g���A���f�[�^�̊��N���X�AMoveScene�ɂ���f�[�^�𐔑����p����K�v������̂ł��̃N���X�Ƀ`���[�g���A���ۑ�N���A�𔻒肷��֐��͗p�ӂ��Ȃ�
	struct TutorialBase{
		enum class TutorialKind{
			e_move
			,e_normalAttack
			,e_wait
		};
		const TutorialKind m_kind;
		TutorialBase(TutorialKind kind):m_kind(kind){}
		virtual ~TutorialBase(){}
	};
	//�ړ��Ɋւ���`���[�g���A���f�[�^���Ǘ�����N���X
	struct MoveTutorial:public TutorialBase{
		const std::shared_ptr<Shape> m_moveTutorialArea;//�ǂ��Ɉړ�����Ηǂ����i�ړ����j�b�g�̒��S�_�����̐}�`�̒��ɓ���Ηǂ��j
		MoveTutorial(std::shared_ptr<Shape> moveTutorialArea);
		~MoveTutorial();
	};
	//�U���Ɋւ���`���[�g���A���f�[�^���Ǘ�����N���X
	struct AttackTutorial:public TutorialBase{
		const Unit *m_targetUnit;//�ǂ̃��j�b�g���U������΂悢��
		AttackTutorial(const Unit *targetUnit);
		~AttackTutorial();
	};
	//�ҋ@�Ɋւ���`���[�g���A�����Ǘ�����N���X
	struct WaitTutorial:public TutorialBase{
		WaitTutorial();
		~WaitTutorial();
	};

	//�萔

	//�ϐ�
public:
	std::vector<std::shared_ptr<TutorialBase>> m_tutorialData;//�ǂ̂悤�ȃ`���[�g���A����񋟂��邩

	//�֐�
protected:

public:
	TutorialBattleSceneData();
	~TutorialBattleSceneData();


};


#endif // !DEF_TUTORIALBATTLESCENEDATA_H