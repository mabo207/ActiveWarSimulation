#define _USE_MATH_DEFINES
#include<cmath>
#include<cassert>
#include"DxLib.h"
#include"input.h"
#include"ToolsLib.h"
#include"GraphicControl.h"

#include"Terrain.h"
#include"Circle.h"
#include"Edge.h"

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
			//fps�v���@�\�̒�`
			FpsMeasuring fpsMeasuring;

			//�f���p�ϐ�
			BattleObject *pMoveCircle(new Terrain(std::shared_ptr<Shape>(new Circle(Vector2D(100.0f,300.0f),30.0f,Shape::Fix::e_dynamic)),-1,GetColor(255,0,0),false));
			std::vector<BattleObject *> field={pMoveCircle};
			//�t�@�C�����J�����ׂĂ̕�����������o��
			std::ifstream ifs("../StageEditor/SaveData/stage2.txt");
			if(!ifs){
				return -1;
			}
			std::string str;//�����o����
			while(true){
				char ch;
				ch=ifs.get();
				//�t�@�C���̏I�[��while����E�o
				if(ch==EOF){
					break;
				}
				str.push_back(ch);
			}
			//�I�u�W�F�N�g�Q��{}�ň͂܂�\n�ŋ�؂��Ă���̂ŁA�P�K�w���������ǂݍ��݂��āA�I�u�W�F�N�g�𐶐�����
			StringBuilder sb(str,'\n','{','}',false,true);
			for(const StringBuilder &ssb:sb.m_vec){
				BattleObject *pb=BattleObject::CreateRawObject(ssb.GetString());
				if(pb!=nullptr){
					field.push_back(pb);
				}
			}
			//�E�C���h�E�̐ݒ�
			std::shared_ptr<ShapeHaving> pSharedWindow(new Terrain(std::shared_ptr<Shape>(new Edge(Vector2D(0.0f,0.0f),Vector2D(1280.0f,720.0f),Shape::Fix::e_ignore)),-1,0,false));//��ʂ�\���x�N�g��
			const ShapeHaving *pWindow=pSharedWindow.get();

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
				for(const BattleObject *pObject:field){
					if(pObject->JudgeInShapeRect(pWindow)){
						pObject->VDraw(Vector2D(0.0f,0.0f));
					}
				}
				pMoveCircle->VDraw(Vector2D(0.0f,0.0f));
				printfDx("Draw : %.1f[ms](/16.6)\n",fpsMeasuring.GetProcessedTime()*1000);

				//�ʒu�X�V
				fpsMeasuring.RecordTime();
				int index=0;
				const float speed=3.0f;//�I�u�W�F�N�g�̈ړ����x
				const size_t moveCount=5;//�ړ��񐔂̕�����
				const size_t judgeCount=3;//1�ړ����̓����蔻����s��
				for(size_t i=0;i<moveCount;i++){
					//1�t���[������5��ړ�
					//�ړ��������������ɂ��鎖�ŁA�ړ����x�����コ���邱�Ƃ��ł���
					if(fpsMeasuring.GetFlame()%1==0){
						//1��̈ړ����������������Ȃ��ƃM���M���ʂ�Ȃ��ꏊ���ʂ��悤�ɂȂ��Ă��܂�
						Vector2D v=GetMousePointVector2D()-pMoveCircle->getPos();
						pMoveCircle->Move(v.norm()*std::fminf((float)(speed/moveCount),v.size()));
					}
					//1�t���[�����ɕ����񓖂��蔻�菈�����s���ƁA�������d���Ȃ����ɃI�u�W�F�N�g�̈ړ����x���グ�邱�Ƃ��ł���
					for(BattleObject *pObject:field){
						//�����蔻��n�̏���
						pObject->UpdatePosition(pointer_array_cast<ShapeHaving>(field.data()),field.size(),judgeCount);
					}
				}

				//�I�����o
				if(keyboard_get(KEY_INPUT_ESCAPE)>0 || index!=0){
					break;
				}
				printfDx("Update : %.1f[ms](/16.6)\n",fpsMeasuring.GetProcessedTime()*1000);
			}
			//�I������
			//field�̊J��
			for(BattleObject *pobj:field){
				delete pobj;
			}
		}

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

