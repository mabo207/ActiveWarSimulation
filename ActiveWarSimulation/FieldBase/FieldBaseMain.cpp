#define _USE_MATH_DEFINES
#include<cmath>
#include<cassert>
#include"DxLib.h"
#include"input.h"
#include"ToolsLib.h"

#include"Circle.h"
#include"Edge.h"

int WINAPI WinMain(HINSTANCE,HINSTANCE,LPSTR,int){
	try{
		//dx���C�u�����̏�����
		//��ʃ��[�h�̐ݒ�(�ꉞ����Ȋ���)
		SetGraphMode(1280,720,16);
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

		//���͋@�\�̏�����
		InitInputControler();

		//fps�v���@�\�̒�`
		FpsMeasuring fpsMeasuring;

		//�f���p�ϐ�
		std::shared_ptr<Shape> pMoveCircle(new Circle(Vector2D(100.0f,300.0f),30.0f,Shape::Fix::e_dynamic));
		std::vector<std::shared_ptr<Shape>> field={pMoveCircle};
		Vector2D epos(800.0f,300.0f),evec(80.0f,20.0f);
		for(int i=0,max=100;i<max;i++){
			//��Q���̒ǉ�
			field.push_back(std::shared_ptr<Shape>(new Circle(Vector2D(120.0f*(float)i-2.0f/2.0f*(float)(i*i),150.0f),30.0f,Shape::Fix::e_static)));
			field.push_back(std::shared_ptr<Shape>(new Edge(epos,evec,Shape::Fix::e_static)));
			epos+=evec;
			evec=evec.turn((float)(M_PI*2/max));
		}
		
		//���s
		while(ScreenFlip() == 0 && ProcessMessage() == 0 && ClearDrawScreen() == 0) {
			//�Q�[���{��
			//�L�[���X�V
			input_update();
			clsDx();

			//fps�v��
			fpsMeasuring.Update();
			printfDx("FPS : %.1f/60\n",fpsMeasuring.GetFps());

			//�`��
			fpsMeasuring.RecordTime();
			for(const std::shared_ptr<const Shape> &pShape:field){
				pShape->Draw(Vector2D(0.0f,0.0f),GetColor(255,255,255),TRUE,1.0f);
			}
			pMoveCircle->Draw(Vector2D(0.0f,0.0f),GetColor(255,0,0),TRUE,1.0f);
			printfDx("Draw : %.1f[ms](/16.6)\n",fpsMeasuring.GetProcessedTime()*1000);

			//���X�V
			fpsMeasuring.RecordTime();
			int index=0;
			const float speed=3.0f;//�I�u�W�F�N�g�̈ړ����x
			const int moveCount=5;//�ړ��񐔂̕�����
			const int judgeCount=3;//1�ړ����̓����蔻����s��
			for(int j=0;j<moveCount;j++){
				//1�t���[������5��ړ�
				//�ړ��������������ɂ��鎖�ŁA�ړ����x�����コ���邱�Ƃ��ł���
				if(fpsMeasuring.GetFlame()%1==0){
					//1��̈ړ����������������Ȃ��ƃM���M���ʂ�Ȃ��ꏊ���ʂ��悤�ɂȂ��Ă��܂�
					Vector2D v=GetMousePointVector2D()-pMoveCircle->GetPosition();
					pMoveCircle->Move(v.norm()*std::fminf((float)(speed/moveCount),v.size()));
				}
				for(int i=0;i<judgeCount;i++){
					//1�t���[�����ɕ����񓖂��蔻�菈�����s���ƁA�������d���Ȃ����ɃI�u�W�F�N�g�̈ړ����x���グ�邱�Ƃ��ł���
					for(const std::shared_ptr<Shape> &pShape:field){
						//�����蔻��n�̏���
						pShape->Update(field);
					}
				}
			}

			//�I�����o
			if(keyboard_get(KEY_INPUT_ESCAPE)>0 || index!=0){
				break;
			}
			printfDx("Update : %.1f[ms](/16.6)\n",fpsMeasuring.GetProcessedTime()*1000);
		}

		//�I������
		DeleteInputControler();//���͋@�\�̉��
		DxLib_End();


		return 0;
	} catch(const std::exception &e){
		assert(e.what());
		return 1;
	}
}

