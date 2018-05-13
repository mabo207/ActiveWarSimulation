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
			//fps計測機構の定義
			FpsMeasuring fpsMeasuring;

			//デモ用変数
			BattleObject *pMoveCircle(new Terrain(std::shared_ptr<Shape>(new Circle(Vector2D(100.0f,300.0f),30.0f,Shape::Fix::e_dynamic)),-1,GetColor(255,0,0),false));
			std::vector<BattleObject *> field={pMoveCircle};
			//ファイルを開きすべての文字列を書き出す
			std::ifstream ifs("../StageEditor/SaveData/stage2.txt");
			if(!ifs){
				return -1;
			}
			std::string str;//書き出し先
			while(true){
				char ch;
				ch=ifs.get();
				//ファイルの終端でwhileから脱出
				if(ch==EOF){
					break;
				}
				str.push_back(ch);
			}
			//オブジェクト群は{}で囲まれ\nで区切られているので、１階層だけ分割読み込みして、オブジェクトを生成する
			StringBuilder sb(str,'\n','{','}',false,true);
			for(const StringBuilder &ssb:sb.m_vec){
				BattleObject *pb=BattleObject::CreateRawObject(ssb.GetString());
				if(pb!=nullptr){
					field.push_back(pb);
				}
			}
			//ウインドウの設定
			std::shared_ptr<ShapeHaving> pSharedWindow(new Terrain(std::shared_ptr<Shape>(new Edge(Vector2D(0.0f,0.0f),Vector2D(1280.0f,720.0f),Shape::Fix::e_ignore)),-1,0,false));//画面を表すベクトル
			const ShapeHaving *pWindow=pSharedWindow.get();

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
				for(const BattleObject *pObject:field){
					if(pObject->JudgeInShapeRect(pWindow)){
						pObject->VDraw(Vector2D(0.0f,0.0f));
					}
				}
				pMoveCircle->VDraw(Vector2D(0.0f,0.0f));
				printfDx("Draw : %.1f[ms](/16.6)\n",fpsMeasuring.GetProcessedTime()*1000);

				//位置更新
				fpsMeasuring.RecordTime();
				int index=0;
				const float speed=3.0f;//オブジェクトの移動速度
				const size_t moveCount=5;//移動回数の分割数
				const size_t judgeCount=3;//1移動内の当たり判定実行回数
				for(size_t i=0;i<moveCount;i++){
					//1フレーム内に5回移動
					//移動処理を小分けにする事で、移動速度を向上させることができる
					if(fpsMeasuring.GetFlame()%1==0){
						//1回の移動距離を小さくしないとギリギリ通れない場所が通れるようになってしまう
						Vector2D v=GetMousePointVector2D()-pMoveCircle->getPos();
						pMoveCircle->Move(v.norm()*std::fminf((float)(speed/moveCount),v.size()));
					}
					//1フレーム内に複数回当たり判定処理を行うと、処理が重くなる代わりにオブジェクトの移動速度を上げることができる
					for(BattleObject *pObject:field){
						//当たり判定系の処理
						pObject->UpdatePosition(pointer_array_cast<ShapeHaving>(field.data()),field.size(),judgeCount);
					}
				}

				//終了検出
				if(keyboard_get(KEY_INPUT_ESCAPE)>0 || index!=0){
					break;
				}
				printfDx("Update : %.1f[ms](/16.6)\n",fpsMeasuring.GetProcessedTime()*1000);
			}
			//終了処理
			//fieldの開放
			for(BattleObject *pobj:field){
				delete pobj;
			}
		}

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

