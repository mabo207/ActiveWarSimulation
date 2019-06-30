#ifndef DEF_GGEDITOR_H
#define DEF_GGEDITOR_H

#include<string>
#include<vector>
#include<memory>
#include"EditActionSettings.h"
#include"ToolsLib.h"
#include"ButtonHaving.h"

//�X�e�[�W�G�f�B�^���g
class StageEditor {
	//�񋓑́E�^
protected:
	
	//�萔
public:

	//�ϐ�
protected:
	EditActionSettings m_actionSettings;//���ݍs�����Ƃ��Ă���ҏW�s�ׂƕҏW�f�[�^�̂܂Ƃ�

	//�{�^���ꗗ
	std::vector<std::shared_ptr<ButtonHaving::Button>> m_buttons;
	
	//�`��p�f�[�^
	int m_font;//�t�H���g
	int m_mapPic;//�w�i�O���t�B�b�N


	//�֐�
protected:
	//�}�E�X�����N���b�N�������̓���
	void ProcessMapPush(int mouseX,int mouseY);//�}�b�v��ʂ����������̓���
	//��ɍs���ҏW�s��
	void NonPressEdit(int mouseX,int mouseY);
	
public:
	//�R���X�g���N�^�ƃf�X�g���N�^
	StageEditor();
	~StageEditor();

	//�����[�v���암��
	int Calculate();
	void Draw();

	//�ÓI�֐�
private:
public:
	static int GetEditorSizeX();
	static int GetEditorSizeY();
};

#endif // !DEF_GGEDITOR_H
#pragma once
