#define _USE_MATH_DEFINES
#include<cmath>
#include<cassert>
#include"DxLib.h"
#include"input.h"
#include<memory>
#include"GraphicControl.h"
#include"ToolsLib.h"

#include"BattleScene.h"
#include"TitleScene.h"

int WINAPI WinMain(HINSTANCE,HINSTANCE,LPSTR,int){
	try{
		//dx���C�u�����̏�����
		//��ʃ��[�h�̐ݒ�(�ꉞ����Ȋ���)
		SetGraphMode(1920,1080,16);
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
			//std::shared_ptr<GameScene> pGameScene(new BattleScene("tutorial1"));
			std::shared_ptr<GameScene> pGameScene(new TitleScene());

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

				//�`��
				clsDx();
				if(fpsdisp){ printfDx("FPS : %.1f/60\n",fpsMeasuring.GetFps()); }
				fpsMeasuring.RecordTime();

				pGameScene->Draw();

				if(fpsdisp){ printfDx("Draw : %.1f[ms](/16.6)\n",fpsMeasuring.GetProcessedTime()*1000); }

				//���X�V
				fpsMeasuring.RecordTime();

				int index=pGameScene->Calculate();

				//�I�����o
				if(keyboard_get(KEY_INPUT_ESCAPE)>0 || index!=0){
					break;
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

