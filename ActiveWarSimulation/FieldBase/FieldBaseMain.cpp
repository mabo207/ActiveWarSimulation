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

		//fps計測機構の定義
		FpsMeasuring fpsMeasuring;

		//デモ用変数
		std::shared_ptr<Shape> pMoveCircle(new Circle(Vector2D(100.0f,300.0f),30.0f,Shape::Fix::e_dynamic));
		std::vector<std::shared_ptr<Shape>> field={pMoveCircle};
		Vector2D epos(800.0f,300.0f),evec(80.0f,20.0f);
		for(int i=0,max=100;i<max;i++){
			//障害物の追加
			field.push_back(std::shared_ptr<Shape>(new Circle(Vector2D(120.0f*(float)i-2.0f/2.0f*(float)(i*i),150.0f),30.0f,Shape::Fix::e_static)));
			field.push_back(std::shared_ptr<Shape>(new Edge(epos,evec,Shape::Fix::e_static)));
			epos+=evec;
			evec=evec.turn((float)(M_PI*2/max));
		}
		
		//実行
		while(ScreenFlip() == 0 && ProcessMessage() == 0 && ClearDrawScreen() == 0) {
			//ゲーム本体
			//キー情報更新
			input_update();
			clsDx();

			//fps計測
			fpsMeasuring.Update();
			printfDx("FPS : %.1f/60\n",fpsMeasuring.GetFps());

			//描画
			fpsMeasuring.RecordTime();
			for(const std::shared_ptr<const Shape> &pShape:field){
				pShape->Draw(Vector2D(0.0f,0.0f),GetColor(255,255,255),TRUE,1.0f);
			}
			pMoveCircle->Draw(Vector2D(0.0f,0.0f),GetColor(255,0,0),TRUE,1.0f);
			printfDx("Draw : %.1f[ms](/16.6)\n",fpsMeasuring.GetProcessedTime()*1000);

			//情報更新
			fpsMeasuring.RecordTime();
			int index=0;
			const float speed=3.0f;//オブジェクトの移動速度
			const int moveCount=5;//移動回数の分割数
			const int judgeCount=3;//1移動内の当たり判定実行回数
			for(int j=0;j<moveCount;j++){
				//1フレーム内に5回移動
				//移動処理を小分けにする事で、移動速度を向上させることができる
				if(fpsMeasuring.GetFlame()%1==0){
					//1回の移動距離を小さくしないとギリギリ通れない場所が通れるようになってしまう
					Vector2D v=GetMousePointVector2D()-pMoveCircle->GetPosition();
					pMoveCircle->Move(v.norm()*std::fminf((float)(speed/moveCount),v.size()));
				}
				for(int i=0;i<judgeCount;i++){
					//1フレーム内に複数回当たり判定処理を行うと、処理が重くなる代わりにオブジェクトの移動速度を上げることができる
					for(const std::shared_ptr<Shape> &pShape:field){
						//当たり判定系の処理
						pShape->Update(field);
					}
				}
			}

			//終了検出
			if(keyboard_get(KEY_INPUT_ESCAPE)>0 || index!=0){
				break;
			}
			printfDx("Update : %.1f[ms](/16.6)\n",fpsMeasuring.GetProcessedTime()*1000);
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

