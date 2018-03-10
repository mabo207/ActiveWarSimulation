#include"DxLib.h"
#include"BattleScene.h"
#include"input.h"
#include"Edge.h"

//----------------------BattleScene----------------------
BattleScene::BattleScene(const char *stagename)
	:GameScene(),m_Window(new Terrain(std::shared_ptr<Shape>(new Edge(Vector2D(0.0f,0.0f),Vector2D(1280.0f,720.0f),Shape::Fix::e_ignore)),-1,0,true))
{
	//ファイルからステージを読み込み
	//ファイルを開きすべての文字列を書き出す
	std::ifstream ifs("SaveData/stage.txt");
	if(!ifs){

	} else{
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
				m_field.push_back(pb);
			}
		}
	}
	//ファイルからユニットを読み込み
	//読み込み方法が確立していないので暫定
	m_field.push_back(new Unit(Vector2D(196.0f,196.0f),-1,Unit::Team::e_player));
	m_field.push_back(new Unit(Vector2D(1024.0f,540.0f),-1,Unit::Team::e_enemy));
	//最初に操作するユニットの選択
	for(BattleObject *obj:m_field){
		if(obj->GetType()==BattleObject::Type::e_unit){
			m_operateUnit=dynamic_cast<Unit *>(obj);
			break;
		}
	}
	//当たり判定情報の更新
	UpdateFix();
}

BattleScene::~BattleScene(){
	//オブジェクト一覧を開放
	for(BattleObject *obj:m_field){
		delete obj;
	}
}

void BattleScene::UpdateFix(){
	//m_fieldの各オブジェクトのm_fixを更新する
	//e_ignoreはそのまま、それ以外は全てe_staticにし、m_operateUnitだけはe_dynamicにする
	for(BattleObject *obj:m_field){
		if(obj->GetFix()!=Shape::Fix::e_ignore){
			obj->SetFix(Shape::Fix::e_static);
			if(obj->GetType()==BattleObject::Type::e_unit){
				//unitに対してはPenetratableも更新する
				dynamic_cast<Unit *>(obj)->SetPenetratable(m_operateUnit->GetBattleStatus().team);
			}
		}
	}
	m_operateUnit->SetFix(Shape::Fix::e_dynamic);
}

int BattleScene::Calculate(){
	//m_operateUnitの位置更新
	const float speed=(float)(m_operateUnit->GetBaseStatus().move);//オブジェクトの移動速度
	const size_t moveCount=5;//移動回数の分割数
	const size_t judgeCount=3;//1移動内の当たり判定実行回数
	const Vector2D beforePos=m_operateUnit->getPos();
	for(size_t i=0;i<moveCount;i++){
		//1フレーム内に5回移動
		//移動処理を小分けにする事で、移動速度を向上させることができる
		//1回の移動距離を小さくしないとギリギリ通れない場所が通れるようになってしまう
		Vector2D v=analogjoypad_get(DX_INPUT_PAD1);
		m_operateUnit->Move(v.norm()*std::fminf((float)(speed/moveCount),v.size()));
		//1フレーム内に複数回当たり判定処理を行うと、処理が重くなる代わりにオブジェクトの移動速度を上げることができる
		for(BattleObject *pObject:m_field){
			//当たり判定系の処理
			pObject->UpdatePosition(pointer_array_cast<ShapeHaving>(m_field.data()),m_field.size(),judgeCount);
		}
	}
	//移動距離の計測と行動終了判定
	const float moveCost=(m_operateUnit->getPos()-beforePos).size()/speed;
	//その他の入力を受け付ける
	if(keyboard_get(KEY_INPUT_Z)==1){
		//攻撃

	} else if(keyboard_get(KEY_INPUT_X)==1){
		//必殺技

	} else if(keyboard_get(KEY_INPUT_A)==1){
		//狙いのキャラの変更(反時計回り)

	} else if(keyboard_get(KEY_INPUT_S)==1){
		//狙いのキャラの変更(時計回り)

	} else if(keyboard_get(KEY_INPUT_C)==1){
		//アイテムの使用

	} else if(keyboard_get(KEY_INPUT_V)==1){
		//待機
		//次のキャラクターに操作を移す
		for(BattleObject *obj:m_field){
			if(obj->GetType()==BattleObject::Type::e_unit && obj!=m_operateUnit){
				m_operateUnit=dynamic_cast<Unit *>(obj);
				break;
			}
		}
		//ユニットの当たり判定図形を変化させる
		UpdateFix();
	} else if(keyboard_get(KEY_INPUT_D)==1){
		//移動やり直し

	}
	return 0;
}

void BattleScene::Draw()const{
	//フィールドの描画
	for(const BattleObject *obj:m_field){
		if(obj!=m_operateUnit){
			//操作中ユニットは最後に描画
			obj->VDraw();
		}
	}
	//操作中ユニットの描画
	m_operateUnit->BattleObject::VDraw();
}
