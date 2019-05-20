#ifndef DEF_STAGECLEARSCENE_H
#define DEF_STAGECLEARSCENE_H

#include"BattleSceneElement.h"
#include"BattleSceneData.h"

//�X�e�[�W�N���A�i���������֌W�Ȃ��j�̏��
class StageClearScene:public BattleSceneElement{
	//�^�E�񋓑�
	
	//�萔
protected:
	static const int bonusFontSize;
	static const int scoreFontSize;

	//�ϐ�
protected:
	const bool m_winFlag;//�N���A�������ǂ����B
	int m_frame;

	//�Q�[���̐i�s�f�[�^
	std::shared_ptr<BattleSceneData> m_battleSceneData;
	const std::shared_ptr<ScoreObserver::ScoreExpression> m_scoreExpression;//�X�R�A�\���̂��߂̃f�[�^

	//�O���t�B�b�N�n
	const std::pair<int,int> m_backPic;//�w�i�̃n���h���Ƃ��̓����x
	const std::pair<int,int> m_bonusBar;//�{�[�i�X�`��̈�̃n���h���Ƃ��̓����x
	const std::pair<int,int> m_turnBar;//�^�[�����`��̈�̃n���h���Ƃ��̓����x
	const std::pair<int,int> m_survivalBar;//�������`��̈�̃n���h���Ƃ��̓����x
	const int m_resultBarPic;//�������E�������̃o�[�̕`��n���h��
	const int m_scoreBarPic;//���v�X�R�A�`��̈�̃n���h��

	//�t�H���g�n
	const int m_bonusFont;//�{�[�i�X���ڕ\���̃t�H���g
	const int m_scoreFont;//�X�R�A���l�\���̃t�H���g

	//�֐�
protected:
	
	//���z�֐��̃I�[�o�[���C�h
	int thisCalculate();//0:���O��ʂɖ߂� 1~END-1:���̏�ʂɐi�� END:�o�g���p�[�g�I��
	void thisDraw()const;
	int UpdateNextScene(int index);//����ʂ̐ݒ�Bindex�ɖ߂肽�����A���ۂɖ߂�邩�͕s���Ȃ̂Ŏ��ۂ̖߂�ꏊ������int�l��Ԃ������B��{�I�ɕԂ�l��index�ɓ������Ȃ�B
	void ReturnProcess();//���̏�ʂɖ߂��Ă������̏���

public:
	StageClearScene(std::shared_ptr<BattleSceneData> battleSceneData,bool winFlag);
	~StageClearScene();
};


#endif // !DEF_STAGECLEARSCENE_H