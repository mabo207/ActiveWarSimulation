#define _USE_MATH_DEFINES
#include<cmath>
#include<cassert>
#include"DxLib.h"
#include"input.h"
#include<memory>
#include"GraphicControl.h"
#include"ToolsLib.h"
#include"CommonConstParameter.h"
#include"GeneralPurposeResourceManager.h"

#include"TitleScene.h"

int WINAPI WinMain(HINSTANCE,HINSTANCE,LPSTR,int){
	try{
		//dxライブラリの初期化
		//画面モードの設定(一応こんな感じ)
		SetGraphMode(CommonConstParameter::gameResolutionX,CommonConstParameter::gameResolutionY,16);
		//タイトルメニュー文字
		SetMainWindowText("ActiveWarSimulation [F2長押し:画面サイズ1倍 / F3長押し:画面サイズ0.5倍 / F1長押し:ウインドウ・フルスクリーン切替]");
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

		//共通素材のロード
		GeneralPurposeResourceManager::LoadResource();

		{
			//場面変数
			//std::shared_ptr<MainControledGameScene> pGameScene(new MainControledFadeInOutGameScene(new TitleScene());
			std::shared_ptr<MainControledGameScene> pGameScene(new MainControledFadeInOutGameScene(TitleScene::TitleSceneFactory().CreateScene(),0x03,15));

			//画面縮小することによる撮影をする際はSetMouseDispFlagをFALSEにしてコンパイル
			SetMouseDispFlag(TRUE);
			int mousePic=LoadGraphEX("Graphic/mouseCursor.png");
			

			//デバッグ用、処理時間の計測と表示
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
					const int mouseDispFlag=GetMouseDispFlag();
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
					pGameScene=std::shared_ptr<MainControledGameScene>(new MainControledFadeInOutGameScene(TitleScene::TitleSceneFactory().CreateScene(),0x03,15));
					mousePic=LoadGraphEX("Graphic/mouseCursor.png");//マウスの読み込みし直し
					SetMouseDispFlag(mouseDispFlag);
				} else if(keyboard_get(KEY_INPUT_F2)==60){
					//F2長押しで、ウインドウサイズを1.0倍に
					const int mouseDispFlag=GetMouseDispFlag();
					SetWindowSizeExtendRate(1.0);
					SetMouseDispFlag(mouseDispFlag);
				} else if(keyboard_get(KEY_INPUT_F3)==60){
					//F3長押しで、ウインドウサイズを1.0倍に
					const int mouseDispFlag=GetMouseDispFlag();
					SetWindowSizeExtendRate(0.5);
					SetMouseDispFlag(mouseDispFlag);
				}

				//描画
				clsDx();
				if(fpsdisp){ printfDx("FPS : %.1f/60\n",fpsMeasuring.GetFps()); }
				fpsMeasuring.RecordTime();

				pGameScene->Draw();

				if(GetMouseDispFlag()==FALSE){
					//マウス表示
					int x,y;
					GetMousePoint(&x,&y);
					DrawGraph(x,y,mousePic,TRUE);
				}
				
				if(fpsdisp){ printfDx("Draw : %.1f[ms](/16.6)\n",fpsMeasuring.GetProcessedTime()*1000); }//fps表示
				
				//情報更新
				fpsMeasuring.RecordTime();

				int index=pGameScene->Calculate();

				//遷移処理
				if(index!=0){
					//std::shared_ptr<MainControledGameScene> pNextScene=pGameScene->VGetNextMainControledScene();
					std::shared_ptr<MainControledGameScene> pNextActivateScene=pGameScene->VGetNextMainControledScene();
					if(pNextActivateScene){
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
			DeleteGraphEX(mousePic);
		}

		//ここに来るまでにゲーム中で用いられていた変数は解放される

		//終了処理
		GeneralPurposeResourceManager::ReleaseResource();//共通リソースの解放
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

