#include"DxLib.h"
#include"BattleScene.h"
#include"input.h"
#include"Edge.h"
#include<algorithm>

//----------------------BattleScene----------------------
const float BattleScene::routeFrequency=1.0f;

BattleScene::BattleScene(const char *stagename)
	:GameScene(),m_Window(new Terrain(std::shared_ptr<Shape>(new Edge(Vector2D(0.0f,0.0f),Vector2D(1280.0f,720.0f),Shape::Fix::e_ignore)),-1,0,true))
	,m_fpsMesuring()
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
	m_field.push_back(Unit::CreateMobUnit(Unit::Profession::e_lancer,1,Vector2D(196.0f,196.0f),Unit::Team::e_player));
	m_field.push_back(Unit::CreateMobUnit(Unit::Profession::e_lancer,1,Vector2D(1024.0f,540.0f),Unit::Team::e_enemy));
	m_field.push_back(Unit::CreateMobUnit(Unit::Profession::e_archer,1,Vector2D(296.0f,196.0f),Unit::Team::e_player));
	m_field.push_back(Unit::CreateMobUnit(Unit::Profession::e_lancer,1,Vector2D(524.0f,340.0f),Unit::Team::e_enemy));
	m_field.push_back(Unit::CreateMobUnit(Unit::Profession::e_armer,1,Vector2D(196.0f,256.0f),Unit::Team::e_player));
	m_field.push_back(Unit::CreateMobUnit(Unit::Profession::e_lancer,1,Vector2D(624.0f,340.0f),Unit::Team::e_enemy));
	m_field.push_back(Unit::CreateMobUnit(Unit::Profession::e_mage,1,Vector2D(100.0f,346.0f),Unit::Team::e_player));
	m_field.push_back(Unit::CreateMobUnit(Unit::Profession::e_lancer,1,Vector2D(1060.0f,440.0f),Unit::Team::e_enemy));
	//m_unitListやm_operateUnitの初期化
	for(BattleObject *obj:m_field){
		if(obj->GetType()==BattleObject::Type::e_unit){
			m_unitList.push_back(dynamic_cast<Unit *>(obj));
		}
	}
	FinishUnitOperation();

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

Vector2D BattleScene::CalculateInputVec()const{
	Vector2D moveVec;
	if(m_operateUnit->GetBattleStatus().team==Unit::Team::e_player){
		//プレイヤー操作時
		moveVec=analogjoypad_get(DX_INPUT_PAD1);
	} else{
		//コンピュータ操作時、AIが方向を決める
		//ターン開始から1秒経ったらひとまず最近傍ユニットに単純に近づく
		if(m_fpsMesuring.GetProcessedTime()>1.0){
			const Unit *nearestUnit=nullptr;
			for(const Unit *pu:m_unitList){
				if(pu->GetBattleStatus().team!=m_operateUnit->GetBattleStatus().team){
					if(nearestUnit==nullptr){
						nearestUnit=pu;
					} else if((pu->getPos()-m_operateUnit->getPos()).sqSize()<(nearestUnit->getPos()-m_operateUnit->getPos()).sqSize()){
						nearestUnit=pu;
					}
				}
			}
			if(nearestUnit!=nullptr){
				moveVec=nearestUnit->getPos()-m_operateUnit->getPos();
			}
		}
	}
	return moveVec;
}

bool BattleScene::PositionUpdate(const Vector2D inputVec){
	//各値の定義
	const float speed=(float)(m_operateUnit->GetBaseStatus().move);//オブジェクトの移動速度
	const size_t moveCount=5;//移動回数の分割数
	const size_t judgeCount=3;//1移動内の当たり判定実行回数
	const Vector2D beforePos=m_operateUnit->getPos();//移動前の位置を取得
	Vector2D moveVec;
	//移動ベクトルの計算
	bool inputFlag=false;//移動の入力があったかどうか
	if(m_operateUnit->GetBattleStatus().OP>0.0f){
		//OPが足りないと動けない
		if(inputVec.sqSize()==0.0f){
			inputFlag=false;
		} else{
			inputFlag=true;
			moveVec=inputVec.norm()*std::fminf((float)(speed/moveCount),inputVec.size());
		}
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
	m_operateUnit->AddOP(-moveCost);//減少なのでcostをマイナスしたものを加算する

	//攻撃対象ユニットの更新(移動しなくても(=inputFlagがfalseでも)ユニットの位置は動く可能性があるので毎ループ処理する)
	bool changeAimedUnitFlag;//対象ユニットの変更を行うか
	float aimedUnitAngle;//対象ユニットのいた方向
	if(m_aimedUnit!=nullptr){
		//前のフレームで対象ユニットがいた場合
		if(m_operateUnit->JudgeAttackable(m_aimedUnit)){
			//現在の対象ユニットが操作ユニットの攻撃範囲に入っている時は特に何もしない
			changeAimedUnitFlag=false;
		} else{
			//現在の対象ユニットが操作ユニットの攻撃範囲に入っていない時は
			changeAimedUnitFlag=true;
			aimedUnitAngle=(m_aimedUnit->getPos()-m_operateUnit->getPos()).GetRadian();
		}
	} else{
		//前のフレームで対象ユニットがいなかった場合
		changeAimedUnitFlag=true;
		aimedUnitAngle=0.0f;
	}
	if(changeAimedUnitFlag){
		//対象ユニットの変更
		SetAimedUnit(aimedUnitAngle,0);
	}
	//m_routeの追加
	if(m_route.empty() || m_route.back().OP-m_operateUnit->GetBattleStatus().OP>routeFrequency){
		//routeFrequencyを以前の計測以降に消費したOPが越したら現在位置と現在のOPをm_routeに格納
		m_route.push_back(RouteInfo(m_operateUnit->getPos(),m_operateUnit->GetBattleStatus().OP));
	}

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
	//m_operateUnitのOPを一定値減らす
	m_operateUnit->AddOP(-Unit::reduceStartActionCost);
	//当たり判定図形の変化
	UpdateFix();
	//m_aimedUnitの初期化
	SetAimedUnit(0.0f,0);
	//m_routeの初期化
	m_route.clear();
	//タイマーセット
	m_fpsMesuring.RecordTime();

}

void BattleScene::SetAimedUnit(float angle,int turntimes){
	//範囲内のユニット一覧の作成
	std::vector<Unit *> list;
	for(Unit *pUnit:m_unitList){
		if(m_operateUnit->JudgeAttackable(pUnit)){
			//異なるチームかつ一定距離内にいれば追加
			list.push_back(pUnit);
		}
	}
	//攻撃対象ユニットを設定
	m_aimedUnit=nullptr;//ひとまずnullptrに
	if(!list.empty()){
		//比較関数の作成と並べ替え。m_operateUnitからのベクトルの向きでソートする
		const Vector2D pos=m_operateUnit->getPos();
		auto f=[&pos](Unit *lobj,Unit *robj)->bool{
			const float langle=(lobj->getPos()-pos).GetRadian(),rangle=(robj->getPos()-pos).GetRadian();
			return langle<rangle;
		};
		std::sort(list.begin(),list.end(),f);
		//aimedUnitAngleに時計回り方向に近いユニットを探す(turntimesが1であるユニットの検出)
		std::vector<Unit *>::const_iterator it=list.begin(),ite=list.end();
		for(;it!=ite;it++){
			if(angle<((*it)->getPos()-pos).GetRadian()){
				//初めてaimedUnitAngleを超えた所にいるオブジェクトを初期オブジェクトとする
				break;
			}
		}
		if(it==list.end()){
			it=list.begin();
		}
		//turntimes回の回転に合わせる。現時点ではturntimes=1のユニットが求められている。
		if(turntimes>0){
			//正の方向の回転は、iteに達したらlist.begin()に戻す事だけ気をつければ良い
			for(int i=1;i<turntimes;i++){
				it++;
				if(it==ite){
					it=list.begin();
				}
			}
		} else{
			//0以下の回転は、angleにぴったり合わさった場合とそうでない場合で異なる
			std::vector<Unit *>::const_iterator beforeit=it;//turntimes=1の直前のイテレータ
			if(beforeit==list.begin()){
				beforeit=list.end();
			}
			beforeit--;
			//itをturntimes=0の場所に合わせる
			if(angle==((*beforeit)->getPos()-pos).GetRadian()){
				//angleにぴったり合わさったユニットが存在する場合
				it=beforeit;//itをturntimes=1のユニットより1つ前に戻す
			}
			//turntimes回の回転に合わせる
			for(int i=0;i>turntimes;i--){
				if(it==list.begin()){
					it=list.end();
				}
				it--;
			}
		}
		//求めたイテレータに格納されているユニットをm_aimedUnitに格納する
		m_aimedUnit=*it;
	}
}

void BattleScene::ProcessAttack(){
	//コストの消費
	m_operateUnit->AddOP(m_operateUnit->CalculateAddOPNormalAttack());
	//操作ユニット→対象ユニットへの攻撃情報の計算
	Weapon::AttackInfo attackinfo=m_operateUnit->GetBattleStatus().weapon->GetAttackInfo(m_operateUnit,m_aimedUnit);
	//操作ユニット→対象ユニットへの攻撃処理
	int aimedHP=m_aimedUnit->AddHP(-attackinfo.damage);
	if(aimedHP<=0){
		//対象ユニットのHPが0以下なら、ステージからユニットを取り除く
		m_aimedUnit->SetFix(Shape::Fix::e_ignore);//当たり判定の対象から取り除く
		//m_unitListからm_aimedUnitを取り除く
		for(std::vector<Unit *>::const_iterator it=m_unitList.begin(),ite=m_unitList.end();it!=ite;it++){
			if(*it==m_aimedUnit){
				m_unitList.erase(it);
				break;
			}
		}
		//m_aimedUnitをnullに
		m_aimedUnit=nullptr;
	} else{
		//対象ユニットが生き残っているなら反撃処理を行う
		//未実装
	}
}

bool BattleScene::JudgeAttackCommandUsable()const{
	return m_aimedUnit!=nullptr && m_operateUnit->GetBattleStatus().OP+m_operateUnit->CalculateAddOPNormalAttack()>=0;
}

int BattleScene::Calculate(){
	if(m_operateUnit->GetBattleStatus().team==Unit::Team::e_player){
		//味方操作時
		//m_operateUnitの位置更新
		if(PositionUpdate(CalculateInputVec())){
			//位置更新をした時の処理

		} else{
			//移動操作をしなかった時はその他の入力を受け付ける
			if(keyboard_get(KEY_INPUT_Z)==1){
				//攻撃
				if(JudgeAttackCommandUsable()){
					//攻撃対象が存在し、OPが足りている場合のみ攻撃処理を行う
					ProcessAttack();//攻撃処理
					FinishUnitOperation();//行動終了処理
				}
			} else if(keyboard_get(KEY_INPUT_D)==1){
				//必殺技

			} else if(keyboard_get(KEY_INPUT_A)==1){
				//狙いのキャラの変更(反時計回り)
				float angle;
				if(m_aimedUnit!=nullptr){
					angle=(m_aimedUnit->getPos()-m_operateUnit->getPos()).GetRadian();
				} else{
					angle=0.0f;
				}
				SetAimedUnit(angle,-1);
			} else if(keyboard_get(KEY_INPUT_S)==1){
				//狙いのキャラの変更(時計回り)
				float angle;
				if(m_aimedUnit!=nullptr){
					angle=(m_aimedUnit->getPos()-m_operateUnit->getPos()).GetRadian();
				} else{
					angle=0.0f;
				}
				SetAimedUnit(angle,1);
			} else if(keyboard_get(KEY_INPUT_C)==1){
				//アイテムの使用

			} else if(keyboard_get(KEY_INPUT_V)==1){
				//待機
				FinishUnitOperation();
			} else if(keyboard_get(KEY_INPUT_X)==1 || keyboard_get(KEY_INPUT_X)>30){
				//移動やり直し(m_route.back()の1つ前の場所に戻す。back()の位置は現在位置の可能性が高いため)
				if(!m_route.empty()){
					m_route.pop_back();
					if(!m_route.empty()){
						//もう要素がなければpop_back()をしない
						RouteInfo info=m_route.back();
						m_route.pop_back();
						//ユニットを移動させる
						m_operateUnit->Warp(info.pos);
						//OPを回復させる
						m_operateUnit->AddOP(info.OP-m_operateUnit->GetBattleStatus().OP);
					}
					//位置更新を行う
					PositionUpdate(Vector2D());
				}
			}
		}
	} else{
		//敵操作時
		//味方操作時
		//m_operateUnitの位置更新
		const Vector2D beforeVec=m_operateUnit->getPos();
		PositionUpdate(CalculateInputVec());
		const float moveSqLength=(beforeVec-m_operateUnit->getPos()).sqSize();
		const double processedTime=m_fpsMesuring.GetProcessedTime();
		if(m_fpsMesuring.GetProcessedTime()>1.0){
			//1秒経ったら行動する
			if(JudgeAttackCommandUsable()){
				//攻撃対象が存在し、OPが足りている場合のみ攻撃処理を行う
				ProcessAttack();//攻撃処理
				FinishUnitOperation();//行動終了処理
			} else if(m_operateUnit->GetBattleStatus().OP<2.0f || processedTime>10.0 || (moveSqLength<0.1f && processedTime>2.0)){
				//移動できなくなったら、または10秒経ったら、また移動距離が少ない場合は待機
				FinishUnitOperation();
			}
		}
	}
	return 0;
}

void BattleScene::Draw()const{
	//フィールドの描画
	for(const BattleObject *obj:m_field){
		if((obj!=m_operateUnit || obj!=m_aimedUnit) && m_Window->JudgeInShapeRect(obj) && !(obj->GetFix()==Shape::Fix::e_ignore && obj->GetType()==BattleObject::Type::e_unit)){
			//操作中ユニットと攻撃対象ユニットは最後に描画
			//ウインドウに入っていない物は描画しない
			//退却したユニット(typeがe_unitかつfixがe_ignore)は描画しない
			obj->VDraw();
		}
	}

	//狙っているユニットの描画
	if(JudgeAttackCommandUsable()){
		m_aimedUnit->BattleObject::VDraw();
		Vector2D pos=m_aimedUnit->getPos();
		DrawTriangleAA(pos.x-15.0f,pos.y-60.0f,pos.x+15.0f,pos.y-60.0f,pos.x,pos.y-30.0f,GetColor(0,255,0),TRUE);
	}

	//経路の描画
	for(size_t i=0,max=m_route.size();i+1<max;i++){
		DrawLineAA(m_route[i].pos.x,m_route[i].pos.y,m_route[i+1].pos.x,m_route[i+1].pos.y,GetColor(255,255,0),1.0f);
	}

	//操作中ユニットの描画
	m_operateUnit->BattleObject::VDraw();
	m_operateUnit->DrawMoveInfo();//移動情報の描画
	Vector2D pos=m_operateUnit->getPos();
	DrawTriangleAA(pos.x-15.0f,pos.y-60.0f,pos.x+15.0f,pos.y-60.0f,pos.x,pos.y-30.0f,GetColor(255,255,0),TRUE);


	//全ユニットのHPゲージの描画
	for(const Unit *unit:m_unitList){
		if(m_Window->JudgeInShapeRect(unit) && unit->GetFix()!=Shape::Fix::e_ignore){
			//ウインドウに入っていない物は描画しない
			//退却したユニット(typeがe_unitかつfixがe_ignore)は描画しない
			unit->DrawHPGage();
		}
	}

	//ユニット情報をデバッグ出力
	int i=0;
	for(const Unit *u:m_unitList){
		printfDx("(Unit[%d])HP:%d OP:%3.3f pos:(%.3f,%.3f)\n",i,u->GetBattleStatus().HP,u->GetBattleStatus().OP,u->getPos().x,u->getPos().y);
		i++;
	}
}
