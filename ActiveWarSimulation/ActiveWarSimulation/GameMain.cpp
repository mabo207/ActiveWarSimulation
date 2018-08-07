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
		//dxライブラリの初期化
		//画面モードの設定(一応こんな感じ)
		SetGraphMode(1920,1080,16);
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
			//std::shared_ptr<GameScene> pGameScene(new BattleScene("tutorial1"));
			std::shared_ptr<GameScene> pGameScene(new TitleScene());

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

				//描画
				clsDx();
				if(fpsdisp){ printfDx("FPS : %.1f/60\n",fpsMeasuring.GetFps()); }
				fpsMeasuring.RecordTime();

				pGameScene->Draw();

				if(fpsdisp){ printfDx("Draw : %.1f[ms](/16.6)\n",fpsMeasuring.GetProcessedTime()*1000); }

				//情報更新
				fpsMeasuring.RecordTime();

				int index=pGameScene->Calculate();

				//終了検出
				if(keyboard_get(KEY_INPUT_ESCAPE)>0 || index!=0){
					break;
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

