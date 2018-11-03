#ifndef DEF_SYSTEMMENU_H
#define DEF_SYSTEMMENU_H

#include"BattleSceneElement.h"
#include"BattleSceneData.h"
#include<memory>

class SystemMenu:public BattleSceneElement{
	//�^�E�񋓑�

	//�萔
protected:
	static const size_t indexCount=2;
	static const int contentX[indexCount],contentY[indexCount];
	static const int contentWidth,contentHeight;
	static const int sentenceX,sentenceY;
	static const std::string contentStr[indexCount];

	//�ϐ�
protected:
	//�Q�[���f�[�^(�`��ɕK�v)
	std::shared_ptr<const BattleSceneData> m_battleSceneData;
	
	//���ڑI��
	size_t m_index;
	Vector2D m_beforeFrameMousePos;//�O�t���[���̃}�E�X�̈ʒu

	//�摜�`��
	const int m_systemBoard;//�w�i�`��
	const int m_contentFont;

	//�֐�
protected:
	int thisCalculate();//0:���O��ʂɖ߂� 1~END-1:���̏�ʂɐi�� END:�o�g���p�[�g�I��
	void thisDraw()const;
	int UpdateNextScene(int index);//����ʂ̐ݒ�Bindex�ɖ߂肽�����A���ۂɖ߂�邩�͕s���Ȃ̂Ŏ��ۂ̖߂�ꏊ������int�l��Ԃ������B��{�I�ɕԂ�l��index�ɓ������Ȃ�B
	void ReturnProcess();//���̏�ʂɖ߂��Ă������̏���

public:
	SystemMenu(std::shared_ptr<BattleSceneData> battleSceneData);
	~SystemMenu();

};

#endif // !DEF_SYSTEMMENU_H
