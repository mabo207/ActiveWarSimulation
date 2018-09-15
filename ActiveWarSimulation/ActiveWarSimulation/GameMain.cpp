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
		//dxライブラリの初期化
		//画面モードの設定(一応こんな感じ)
		SetGraphMode(CommonConstParameter::gameResolutionX,CommonConstParameter::gameResolutionY,16);
		//タイトルメニュー文字
		SetMainWindowText("ActiveWarSimulation");
		//ウインドウサイズの変更
		SetWindowSizeExtendRate(1.0);
		//ウインドウサイズの変更をできるようにする
		SetWindowSizeChangeEnableFlag(TRUE);
		//アイコンの設定
		SetWindowIconID(101);
		//非アクティブ状態での処理の続行のフラグ
		SetAlwaysRunFlag(FALSE);
		//背景色は黒より少しだけ白っぽくする
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

		//グラフィック管理クラスの初期化
		GraphicControler_Init();
		FontControler_Init();

		//入力機構の初期化
		InitInputControler();

		{
			//場面変数
			//std::shared_ptr<MainControledGameScene> pGameScene(new MainControledFadeInOutGameScene(new TitleScene());
			std::shared_ptr<MainControledGameScene> pGameScene(new MainControledFadeInOutGameScene(std::shared_ptr<MainControledGameScene>(new TitleScene()),0x03,15));
			
			FpsMeasuring fpsMeasuring;
			bool fpsdisp=false;

			//実行
			while(ScreenFlip() == 0 && ProcessMessage() == 0 && ClearDrawScreen() == 0) {
				//ゲーム本体
				//キー情報更新
				input_update();

				fpsMeasuring.Update();
				if(keyboard_get(KEY_INPUT_K)==60){
					fpsdisp=!fpsdisp;
				}

				if(keyboard_get(KEY_INPUT_F1)==60){
					//F1長押しで、ウインドウモードとフルスクリーンモードの切り替え。タイトル画面まで強制的に戻す
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
					//グラフィック系の読み込み直し
					GraphicControler_End();//グラフィック管理クラスの解放
					FontControler_End();//フォント管理クラスの解放
					DeleteInputControler();//入力機構の解放
					GraphicControler_Init();
					FontControler_Init();
					InitInputControler();
					pGameScene=std::shared_ptr<MainControledGameScene>(new MainControledFadeInOutGameScene(std::shared_ptr<MainControledGameScene>(new TitleScene()),0x03,15));
					SetMouseDispFlag(TRUE);//ウインドウモードだとマウスが見えなくなるので設定
				} else if(keyboard_get(KEY_INPUT_F2)==60){
					//F2長押しで、ウインドウサイズを1.0倍に
					SetWindowSizeExtendRate(1.0);
				} else if(keyboard_get(KEY_INPUT_F3)==60){
					//F3長押しで、ウインドウサイズを1.0倍に
					SetWindowSizeExtendRate(0.5);
				}

				//描画
				clsDx();
				if(fpsdisp){ printfDx("FPS : %.1f/60\n",fpsMeasuring.GetFps()); }
				fpsMeasuring.RecordTime();

				pGameScene->Draw();

				if(fpsdisp){ printfDx("Draw : %.1f[ms](/16.6)\n",fpsMeasuring.GetProcessedTime()*1000); }

				//情報更新
				fpsMeasuring.RecordTime();

				int index=pGameScene->Calculate();

				//遷移処理
				if(index!=0){
					//std::shared_ptr<MainControledGameScene> pNextScene=pGameScene->VGetNextMainControledScene();
					std::shared_ptr<MainControledGameScene> pNextActivateScene=pGameScene->VGetNextMainControledScene();
					if(pNextActivateScene.get()!=nullptr){
						//次の場面があれば、その場面へ遷移
						std::shared_ptr<MainControledGameScene> pNextScene(new MainControledFadeInOutGameScene(pNextActivateScene,0x03,15));
						if(pNextScene.get()!=nullptr){
							//次の場面の生成に成功すれば
							pGameScene=pNextScene;
						} else{
							//失敗したら強制終了
							break;
						}
					} else{
						//次の場面がなければ強制終了
						break;
					}
				}

				//終了検出
				if(keyboard_get(KEY_INPUT_ESCAPE)>0){
					//break;//Alt+F4でやってもらう
				}

				if(fpsdisp){ printfDx("Update : %.1f[ms](/16.6)\n",fpsMeasuring.GetProcessedTime()*1000); }
			}
		}

		//ここに来るまでにゲーム中で用いられていた変数は解放される

		//終了処理
		DeleteInputControler();//入力機構の解放
		GraphicControler_End();//グラフィック管理クラスの解放
		FontControler_End();//フォント管理クラスの解放
		DxLib_End();


		return 0;
	} catch(const std::exception &e){
		assert(e.what());
		return 1;
	}
}

