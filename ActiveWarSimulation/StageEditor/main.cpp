#include<iostream>
#include<memory>
#include"DxLib.h"
#include"input.h"
#include"GraphicControl.h"
#include"StageEditor.h"

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	//��ʃ��[�h�̐ݒ�
	SetGraphMode(StageEditor::leftUpPosX*2+StageEditor::mapSizeX+StageEditor::buttonWidth, StageEditor::leftUpPosY * 2 + StageEditor::mapSizeY,16);
	//�^�C�g�����j���[����
	SetMainWindowText("StageEditor");
	//�E�C���h�E�T�C�Y�̕ύX
	SetWindowSizeExtendRate(1.0);
	//�E�C���h�E�T�C�Y�̕ύX���ł���悤�ɂ��Ȃ�
	SetWindowSizeChangeEnableFlag(FALSE);
	//�A�C�R���̐ݒ�
	SetWindowIconID(101);

	if (ChangeWindowMode(TRUE) != 0) {
		return 0;
	}
	if (DxLib_Init() != 0) {
		return 0;
	}
	if (SetDrawScreen(DX_SCREEN_BACK) != 0) {
		DxLib_End();
		return 0;
	}

	//�O���t�B�b�N�Ǘ��N���X�̏�����
	GraphicControler_Init();
	FontControler_Init();

	//���͂ɂ��Ă̏�����
	InitInputControler();

	std::shared_ptr<StageEditor> ggEditor(new StageEditor());

	while (ScreenFlip() == 0 && ProcessMessage() == 0 && ClearDrawScreen() == 0) {
		//�Q�[���{��
		//�L�[���X�V
		input_update();
		//�`��
		ggEditor->Draw();
		//�v�Z����
		int index = ggEditor->Calculate();
		//�I�����o
		if(index<0){
			break;
		}
	}

	//���͊֘A�̃������̈�J��
	DeleteInputControler();
	GraphicControler_End();//�O���t�B�b�N�Ǘ��N���X�̉��
	FontControler_End();//�t�H���g�Ǘ��N���X�̉��

	DxLib_End();//DX���C�u�����I������
	return 0;
}
