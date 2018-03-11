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
	m_field.push_back(new Unit(Vector2D(296.0f,196.0f),-1,Unit::Team::e_player));
	m_field.push_back(new Unit(Vector2D(524.0f,340.0f),-1,Unit::Team::e_enemy));
	//m_unitListやm_operateUnitの初期化
	for(BattleObject *obj:m_field){
		if(obj->GetType()==BattleObject::Type::e_unit){
			m_unitList.push_back(dynamic_cast<Unit *>(obj));
		}
	}
	FinishUnitOperation();
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

bool BattleScene::PositionUpdate(){
	//各値の定義
	const float speed=(float)(m_operateUnit->GetBaseStatus().move);//オブジェクトの移動速度
	const size_t moveCount=5;//移動回数の分割数
	const size_t judgeCount=3;//1移動内の当たり判定実行回数
	const Vector2D beforePos=m_operateUnit->getPos();//移動前の位置を取得
	//移動方向の計算
	Vector2D moveVec=analogjoypad_get(DX_INPUT_PAD1);
	bool inputFlag;
	if(moveVec.sqSize()==0.0f){
		inputFlag=false;
	} else{
		inputFlag=true;
		moveVec=moveVec.norm()*std::fminf((float)(speed/moveCount),moveVec.size());
	}
	//位置更新作業
	for(size_t i=0;i<moveCount;i++){
		//1フレーム内に5回移動
		//移動処理を小分けにする事で、移動速度を向上させることができる
		//1回の移動距離を小さくしないとギリギリ通れない場所が通れるようになってしまう
		if(inputFlag){
			m_operateUnit->Move(moveVec);
		}
		//1フレーム内に複数回当たり判定処理を行うと、処理が重くなる代わりにオブジェクトの移動速度を上げることができる
		for(BattleObject *pObject:m_field){
			//当たり判定系の処理
			pObject->UpdatePosition(pointer_array_cast<ShapeHaving>(m_field.data()),m_field.size(),judgeCount);
		}
	}
	//移動距離の計測とOP減少
	const float moveCost=(m_operateUnit->getPos()-beforePos).size()/speed;
	m_operateUnit->AddOP(-moveCost);//現象なのでcostをマイナスしたものを加算する

	return inputFlag;
}

void BattleScene::SortUnitList(){
	std::vector<Unit *> list=m_unitList;//元の配列をコピー
	m_unitList.clear();
	//m_unitListにソート結果を格納。O(n^2)の実装なので直せるのなら直したいが、同じOPのオブジェクトは前後で同じ順番にしたい。
	while(!list.empty()){
		std::vector<Unit *>::const_iterator maxit=list.begin();
		//itにOP最大のオブジェクトの内、先頭に近いものを格納
		for(std::vector<Unit *>::const_iterator it=list.begin(),ite=list.end();it!=ite;it++){
			if((*maxit)->GetBattleStatus().OP<(*it)->GetBattleStatus().OP){
				//itの方が大きい場合
				maxit=it;
			}
		}
		//m_unitListに格納し、listから削除
		m_unitList.push_back(*maxit);
		list.erase(maxit);
	}
}

void BattleScene::FinishUnitOperation(){
	//m_unitListソートをし直す
	SortUnitList();
	//先頭をm_operateUnitに格納
	m_operateUnit=m_unitList.front();
	//m_operateUnitのOPが最大になるようにm_unitList全員のOP値を変化
	const float plusOP=Unit::BattleStatus::maxOP-m_operateUnit->GetBattleStatus().OP;
	for(Unit *u:m_unitList){
		u->AddOP(plusOP);
	}
	//当たり判定図形の変化
	UpdateFix();
}

int BattleScene::Calculate(){
	//m_operateUnitの位置更新
	if(PositionUpdate()){

	} else{
		//移動操作をしなかった時はその他の入力を受け付ける
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
			FinishUnitOperation();
		} else if(keyboard_get(KEY_INPUT_D)==1){
			//移動やり直し

		}
	}
	return 0;
}

void BattleScene::Draw()const{
	//フィールドの描画
	for(const BattleObject *obj:m_field){
		if(obj!=m_operateUnit && m_Window->JudgeInShapeRect(obj)){
			//操作中ユニットは最後に描画
			//ウインドウに入っていない物は描画しない
			obj->VDraw();
		}
	}
	//操作中ユニットの描画
	m_operateUnit->BattleObject::VDraw();
	Vector2D pos=m_operateUnit->getPos();
	DrawTriangleAA(pos.x-15.0f,pos.y-60.0f,pos.x+15.0f,pos.y-60.0f,pos.x,pos.y-30.0f,GetColor(255,255,0),TRUE);

	//ユニット情報をデバッグ出力
	int i=0;
	for(const Unit *u:m_unitList){
		printfDx("(Unit[%d])HP:%d OP:%3.3f pos:(%.3f,%.3f)\n",i,u->GetBattleStatus().HP,u->GetBattleStatus().OP,u->getPos().x,u->getPos().y);
		i++;
	}
}
