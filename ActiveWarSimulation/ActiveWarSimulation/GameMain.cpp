#define _USE_MATH_DEFINES
#include<cmath>
#include<cassert>
#include"DxLib.h"
#include"input.h"

int WINAPI WinMain(HINSTANCE,HINSTANCE,LPSTR,int){
	try{
		//dxライブラリの初期化
		//画面モードの設定(一応こんな感じ)
		SetGraphMode(1280,720,16);
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

		//入力機構の初期化
		InitInputControler();

		//デモ用変数

		//実行
		while(ScreenFlip() == 0 && ProcessMessage() == 0 && ClearDrawScreen() == 0) {
			//ゲーム本体
			//キー情報更新
			input_update();

			//描画
			clsDx();

			//情報更新
			int index=0;

			//終了検出
			if(keyboard_get(KEY_INPUT_ESCAPE)>0 || index!=0){
				break;
			}
		}

		//終了処理
		DeleteInputControler();//入力機構の解放
		DxLib_End();


		return 0;
	} catch(const std::exception &e){
		assert(e.what());
		return 1;
	}
}

