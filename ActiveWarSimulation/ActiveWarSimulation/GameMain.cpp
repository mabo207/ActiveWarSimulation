#define _USE_MATH_DEFINES
#include<cmath>
#include<cassert>
#include"DxLib.h"
#include"input.h"
#include<memory>
#include"GraphicControl.h"
#include"ToolsLib.h"
#include"CommonConstParameter.h"
#include"GeneralPurposeResource.h"
#include"FilePath.h"
#include"BGMManager.h"

#include"TitleScene.h"
#include"FadeInScene.h"
#include"LoadingScene.h"

std::shared_ptr<GameScene> CreateStartScene(){
	const auto titleFactory=std::make_shared<TitleScene::TitleSceneFactory>();
	const auto fadeInFactory=std::make_shared<FadeInScene::FadeInSceneFactory>(titleFactory,15);
	return LoadingScene::LoadingSceneFactory(fadeInFactory).CreateCompleteScene();
}

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

		//BGM再生管理クラスの初期化
		BGMManager::Init();

		//共通素材のロード
		GeneralPurposeResource::LoadResource();

		{
			//場面変数
			std::shared_ptr<GameScene> pGameScene=CreateStartScene();

			//画面縮小することによる撮影をする際はSetMouseDispFlagをFALSEにしてコンパイル
			SetMouseDispFlag(TRUE);
			int mousePic=LoadGraphEX(FilePath::graphicDir+"mouseCursor.png");
			

			//デバッグ用、処理時間の計測と表示
			FpsMeasuring fpsMeasuring;
			bool debugDisp=false;

			//実行
			while(ScreenFlip() == 0 && ProcessMessage() == 0 && ClearDrawScreen() == 0) {
				//ゲーム本体
				//キー情報更新
				input_update();

				fpsMeasuring.Update();
				if(keyboard_get(KEY_INPUT_K)==60){
					debugDisp=!debugDisp;
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
					GeneralPurposeResource::ReleaseResource();//共有リソースの解放
					GraphicControler_End();//グラフィック管理クラスの解放
					FontControler_End();//フォント管理クラスの解放
					DeleteInputControler();//入力機構の解放
					GraphicControler_Init();
					FontControler_Init();
					GeneralPurposeResource::LoadResource();//共有リソースの取得
					InitInputControler();
					pGameScene=CreateStartScene();
					mousePic=LoadGraphEX(FilePath::graphicDir+"mouseCursor.png");//マウスの読み込みし直し
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
				if(debugDisp){
					const Vector2D mousePos=GetMousePointVector2D();
					printfDx("mouse : ( %.0f , %.0f )\n",mousePos.x,mousePos.y);
					printfDx("FPS : %.1f/60\n",fpsMeasuring.GetFps());
				}
				fpsMeasuring.RecordTime();

				pGameScene->Draw();

				if(GetMouseDispFlag()==FALSE){
					//マウス表示
					int x,y;
					GetMousePoint(&x,&y);
					DrawGraph(x,y,mousePic,TRUE);
				}
				
				if(debugDisp){ printfDx("Draw : %.1f[ms](/16.6)\n",fpsMeasuring.GetProcessedTime()*1000); }//fps表示
				
				//情報更新
				fpsMeasuring.RecordTime();

				int index=pGameScene->Calculate();

				//遷移処理
				if(index!=0){
					pGameScene=GameScene::GetNextScene(pGameScene);
					if(!pGameScene){
						//次の場面がなければ強制終了
						break;
					}
				}

				//終了検出
				if(keyboard_get(KEY_INPUT_ESCAPE)>0){
					//break;//Alt+F4でやってもらう
				}

				if(debugDisp){ printfDx("Update : %.1f[ms](/16.6)\n",fpsMeasuring.GetProcessedTime()*1000); }
			}
			DeleteGraphEX(mousePic);
		}

		//ここに来るまでにゲーム中で用いられていた変数は解放される

		//終了処理
		GeneralPurposeResource::ReleaseResource();//共通リソースの解放
		BGMManager::Release();//BGM再生管理クラスの解放
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

