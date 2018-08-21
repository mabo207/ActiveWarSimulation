#ifndef DEF_STAGECLEARSCENE_H
#define DEF_STAGECLEARSCENE_H

#include"BattleSceneElement.h"
#include"BattleSceneData.h"

//�X�e�[�W�N���A�i���������֌W�Ȃ��j�̏��
class StageClearScene:public BattleSceneElement{
	//�^�E�񋓑�
	
	//�萔

	//�ϐ�
protected:
	const bool m_winFlag;//�N���A�������ǂ����B
	const std::string m_explain;//�⑫���
	int m_flame;

	//�Q�[���̐i�s�f�[�^
	std::shared_ptr<BattleSceneData> m_battleSceneData;

	//�O���t�B�b�N�n
	const int m_stageClearBox;//�X�e�[�W�N���A�󋵂�`�悷��{�b�N�X
	const int m_clearFont;
	const int m_explainFont;

	//�֐�
protected:
	
	//���z�֐��̃I�[�o�[���C�h
	int thisCalculate();//0:���O��ʂɖ߂� 1~END-1:���̏�ʂɐi�� END:�o�g���p�[�g�I��
	void thisDraw()const;
	int UpdateNextScene(int index);//����ʂ̐ݒ�Bindex�ɖ߂肽�����A���ۂɖ߂�邩�͕s���Ȃ̂Ŏ��ۂ̖߂�ꏊ������int�l��Ԃ������B��{�I�ɕԂ�l��index�ɓ������Ȃ�B
	void ReturnProcess();//���̏�ʂɖ߂��Ă������̏���

public:
	StageClearScene(std::shared_ptr<BattleSceneData> battleSceneData,bool winFlag,const std::string &explain);
	~StageClearScene();
};


#endif // !DEF_STAGECLEARSCENE_H
