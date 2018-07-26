#ifndef DEF_RESEARCHSCENE_H
#define DEF_RESEARCHSCENE_H

#include"BattleSceneElement.h"
#include"BattleSceneData.h"
#include"Unit.h"

//�}�b�v�̏��𒲂ׂ郂�[�h�̏�ʕϐ�
class ResearchScene:public BattleSceneElement{
	//�^�E�񋓑�

	//�萔
	static const int explainFontSize;

	//�ϐ�
protected:

	//�Q�[���Ɏg�p����ϐ�
	std::shared_ptr<BattleSceneData> m_battleSceneData;//�Q�[�����S�̂Ŏg���f�[�^

	//�������[�h�ŗp����ϐ�
	const Unit *m_researchUnit;//����\�����郆�j�b�g
	Vector2D m_pointerVec;//���ׂĂ���ꏊ

	const int m_palFont;//�p�����[�^�\���̂��߂̃t�H���g
	const int m_nameFont;//���O�\���̂��߂̃t�H���g
	const int m_explainFont;//�������\���̂��߂̃t�H���g
	const int m_palBackPic;//�p�����[�^�\���̂��߂̔w�i�G


	//�֐�
protected:
	void UpdatePointer();//���ׂ�ӏ��̍X�V

	//���z�֐��I�[�o�[���C�h
	int thisCalculate();//0:���O��ʂɖ߂� 1~END-1:���̏�ʂɐi�� END:�o�g���p�[�g�I��
	void thisDraw()const;
	int UpdateNextScene(int index);//����ʂ̐ݒ�Bindex�ɖ߂肽�����A���ۂɖ߂�邩�͕s���Ȃ̂Ŏ��ۂ̖߂�ꏊ������int�l��Ԃ������B��{�I�ɕԂ�l��index�ɓ������Ȃ�B
	void ReturnProcess();//���̏�ʂɖ߂��Ă������̏���

public:
	ResearchScene(std::shared_ptr<BattleSceneData> battleSceneData);
	~ResearchScene();

};


#endif // !DEF_RESEARCHSCENE_H
