#define _USE_MATH_DEFINES
#include<cmath>
#include<cassert>
#include"DxLib.h"
#include"input.h"
#include<memory>
#include"GraphicControl.h"
#include"ToolsLib.h"
#include"CommonConstParameter.h"

#include"TitleScene.h"

int WINAPI WinMain(HINSTANCE,HINSTANCE,LPSTR,int){
	try{
		//dx���C�u�����̏�����
		//��ʃ��[�h�̐ݒ�(�ꉞ����Ȋ���)
		SetGraphMode(CommonConstParameter::gameResolutionX,CommonConstParameter::gameResolutionY,16);
		//�^�C�g�����j���[����
		SetMainWindowText("ActiveWarSimulation");
		//�E�C���h�E�T�C�Y�̕ύX
		SetWindowSizeExtendRate(1.0);
		//�E�C���h�E�T�C�Y�̕ύX���ł���悤�ɂ���
		SetWindowSizeChangeEnableFlag(TRUE);
		//�A�C�R���̐ݒ�
		SetWindowIconID(101);
		//��A�N�e�B�u��Ԃł̏����̑��s�̃t���O
		SetAlwaysRunFlag(FALSE);
		//�w�i�F�͍���菭�����������ۂ�����
		SetBackgroundColor(32,32,32);

		if(ChangeWindowMode(TRUE) != 0) {
			throw(std::runtime_error("ChangeWindowMode(TRUE) failed."));
		}
		if(DxLib_Init() != 0) {
			throw(std::runtime_error("DxLib_Init() failed."));
		}
		if(SetDrawScreen(DX_SCREEN_BACK) != 0) {
			DxLib_End();
			throw(std::runtime_error("SetDrawScreen(DX_SCREEN_BACK) failed."));
		}

		//�O���t�B�b�N�Ǘ��N���X�̏�����
		GraphicControler_Init();
		FontControler_Init();

		//���͋@�\�̏�����
		InitInputControler();

		{
			//��ʕϐ�
			//std::shared_ptr<MainControledGameScene> pGameScene(new MainControledFadeInOutGameScene(new TitleScene());
			std::shared_ptr<MainControledGameScene> pGameScene(new MainControledFadeInOutGameScene(std::shared_ptr<MainControledGameScene>(new TitleScene()),0x03,15));
			
			FpsMeasuring fpsMeasuring;
			bool fpsdisp=false;

			//���s
			while(ScreenFlip() == 0 && ProcessMessage() == 0 && ClearDrawScreen() == 0) {
				//�Q�[���{��
				//�L�[���X�V
				input_update();

				fpsMeasuring.Update();
				if(keyboard_get(KEY_INPUT_K)==60){
					fpsdisp=!fpsdisp;
				}

				if(keyboard_get(KEY_INPUT_F1)==60){
					//F1�������ŁA�E�C���h�E���[�h�ƃt���X�N���[�����[�h�̐؂�ւ��B�^�C�g����ʂ܂ŋ����I�ɖ߂�
					int mode=GetWindowModeFlag();
					if(mode==TRUE){
						mode=FALSE;
					} else{
						mode=TRUE;
					}
					if(ChangeWindowMode(mode) != 0) {
						throw(std::runtime_error("ChangeWindowMode(mode) failed."));
					}
					if(SetDrawScreen(DX_SCREEN_BACK) != 0) {
						DxLib_End();
						throw(std::runtime_error("SetDrawScreen(DX_SCREEN_BACK) failed."));
					}
					//�O���t�B�b�N�n�̓ǂݍ��ݒ���
					GraphicControler_End();//�O���t�B�b�N�Ǘ��N���X�̉��
					FontControler_End();//�t�H���g�Ǘ��N���X�̉��
					DeleteInputControler();//���͋@�\�̉��
					GraphicControler_Init();
					FontControler_Init();
					InitInputControler();
					pGameScene=std::shared_ptr<MainControledGameScene>(new MainControledFadeInOutGameScene(std::shared_ptr<MainControledGameScene>(new TitleScene()),0x03,15));
					SetMouseDispFlag(TRUE);//�E�C���h�E���[�h���ƃ}�E�X�������Ȃ��Ȃ�̂Őݒ�
				} else if(keyboard_get(KEY_INPUT_F2)==60){
					//F2�������ŁA�E�C���h�E�T�C�Y��1.0�{��
					SetWindowSizeExtendRate(1.0);
				} else if(keyboard_get(KEY_INPUT_F3)==60){
					//F3�������ŁA�E�C���h�E�T�C�Y��1.0�{��
					SetWindowSizeExtendRate(0.5);
				}

				//�`��
				clsDx();
				if(fpsdisp){ printfDx("FPS : %.1f/60\n",fpsMeasuring.GetFps()); }
				fpsMeasuring.RecordTime();

				pGameScene->Draw();

				if(fpsdisp){ printfDx("Draw : %.1f[ms](/16.6)\n",fpsMeasuring.GetProcessedTime()*1000); }

				//���X�V
				fpsMeasuring.RecordTime();

				int index=pGameScene->Calculate();

				//�J�ڏ���
				if(index!=0){
					//std::shared_ptr<MainControledGameScene> pNextScene=pGameScene->VGetNextMainControledScene();
					std::shared_ptr<MainControledGameScene> pNextActivateScene=pGameScene->VGetNextMainControledScene();
					if(pNextActivateScene.get()!=nullptr){
						//���̏�ʂ�����΁A���̏�ʂ֑J��
						std::shared_ptr<MainControledGameScene> pNextScene(new MainControledFadeInOutGameScene(pNextActivateScene,0x03,15));
						if(pNextScene.get()!=nullptr){
							//���̏�ʂ̐����ɐ��������
							pGameScene=pNextScene;
						} else{
							//���s�����狭���I��
							break;
						}
					} else{
						//���̏�ʂ��Ȃ���΋����I��
						break;
					}
				}

				//�I�����o
				if(keyboard_get(KEY_INPUT_ESCAPE)>0){
					//break;//Alt+F4�ł���Ă��炤
				}

				if(fpsdisp){ printfDx("Update : %.1f[ms](/16.6)\n",fpsMeasuring.GetProcessedTime()*1000); }
			}
		}

		//�����ɗ���܂łɃQ�[�����ŗp�����Ă����ϐ��͉�������

		//�I������
		DeleteInputControler();//���͋@�\�̉��
		GraphicControler_End();//�O���t�B�b�N�Ǘ��N���X�̉��
		FontControler_End();//�t�H���g�Ǘ��N���X�̉��
		DxLib_End();


		return 0;
	} catch(const std::exception &e){
		assert(e.what());
		return 1;
	}
}

