#include"DxLib.h"
#include"BattleSceneData.h"
#include"input.h"
#include"Edge.h"
#include<algorithm>

//----------------------BattleSceneData----------------------
BattleSceneData::BattleSceneData(const char *stagename)
	:m_Window(new Terrain(std::shared_ptr<Shape>(new Edge(Vector2D(0.0f,0.0f),Vector2D(1920.0f,1080.0f),Shape::Fix::e_ignore)),-1,0,true))
	,m_fpsMesuring(),m_operateUnit(nullptr),m_orderFont(CreateFontToHandle("Bell MT",32,2,DX_FONTTYPE_EDGE))
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
	//ファイルからステージのグラフィックデータの読み込み
	m_stageSize=Vector2D(1920.0f,1080.0f);//本来はステージの大きさはグラフィックデータの縦横の大きさで決める

	//ファイルからユニットを読み込み
	//読み込み方法が確立していないので暫定
	m_field.push_back(Unit::CreateMobUnit(Unit::Profession::e_lancer,1,Vector2D(786.0f,510.0f),Unit::Team::e_enemy));
	m_field.push_back(Unit::CreateMobUnit(Unit::Profession::e_lancer,1,Vector2D(1596.0f,165.0f),Unit::Team::e_enemy));
	m_field.push_back(Unit::CreateMobUnit(Unit::Profession::e_lancer,1,Vector2D(1740.0f,660.0f),Unit::Team::e_enemy));
	m_field.push_back(Unit::CreateMobUnit(Unit::Profession::e_armer,1,Vector2D(1536.0f,810.0f),Unit::Team::e_enemy));
	m_field.push_back(Unit::CreateMobUnit(Unit::Profession::e_lancer,3,Vector2D(224.0f,124.0f),Unit::Team::e_player));
	m_field.push_back(Unit::CreateMobUnit(Unit::Profession::e_archer,3,Vector2D(354.0f,294.0f),Unit::Team::e_player));
	m_field.push_back(Unit::CreateMobUnit(Unit::Profession::e_mage,3,Vector2D(225.0f,519.0f),Unit::Team::e_player));
	m_field.push_back(Unit::CreateMobUnit(Unit::Profession::e_armer,3,Vector2D(306.0f,441.0f),Unit::Team::e_player));
	m_field.push_back(Unit::CreateMobUnit(Unit::Profession::e_healer,3,Vector2D(106.0f,241.0f),Unit::Team::e_player));
	//m_unitListやm_operateUnitの初期化
	for(BattleObject *obj:m_field){
		if(obj->GetType()==BattleObject::Type::e_unit){
			m_unitList.push_back(dynamic_cast<Unit *>(obj));
		}
	}
	//m_unitListソートをし直す
	SortUnitList();
	//タイマーセット
	m_fpsMesuring.RecordTime();

}

BattleSceneData::~BattleSceneData(){
	DeleteFontToHandle(m_orderFont);
	//オブジェクト一覧を開放
	for(BattleObject *obj:m_field){
		delete obj;
	}
}

void BattleSceneData::UpdateFix(){
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

bool BattleSceneData::PositionUpdate(const Vector2D inputVec){
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
	//m_operateUnit->AddOP(-moveCost);//減少なのでcostをマイナスしたものを加算する
	m_operateUnit->ConsumeOPByCost(moveCost);

	return inputFlag;
}

void BattleSceneData::SortUnitList(){
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

void BattleSceneData::FinishUnitOperation(){
	//m_unitListソートをし直す
	SortUnitList();
	//先頭をm_operateUnitに格納
	m_operateUnit=m_unitList.front();
	//m_operateUnitのOPが最大になるようにm_unitList全員のOP値を変化
	const float plusOP=Unit::BattleStatus::maxOP-m_operateUnit->GetBattleStatus().OP;
	for(Unit *u:m_unitList){
		//u->AddOP(plusOP);
		u->SetOP(u->GetBattleStatus().OP+plusOP);
	}
	//m_operateUnitのOPを減らす(コストとして消費するので消費OP増加等の影響を受ける仕様となる)
	//m_operateUnit->AddOP(-Unit::reduceStartActionCost);
	m_operateUnit->ConsumeOPByCost(Unit::reduceStartActionCost);
	//当たり判定図形の変化
	UpdateFix();
	//タイマーセット
	m_fpsMesuring.RecordTime();

}

void BattleSceneData::DrawField(const std::set<const BattleObject *> &notDraw)const{
	for(const BattleObject *obj:m_field){
		if(m_Window->JudgeInShapeRect(obj)
			&& obj->GetType()!=BattleObject::Type::e_unit
			&& notDraw.find(obj)==notDraw.end())
		{
			//ウインドウに入っていない物は描画しない
			//ユニット(typeがe_unit)は描画しない
			//描画しないもの(notDrawに格納されているもの)は描画しない
			obj->VDraw();
		}
	}
}

void BattleSceneData::DrawUnit(bool infoDrawFlag,const std::set<const Unit *> &notDraw)const{
	for(const Unit *obj:m_unitList){
		if(m_Window->JudgeInShapeRect(obj)
			&& obj->GetFix()!=Shape::Fix::e_ignore
			&& notDraw.find(obj)==notDraw.end())
		{
			//ウインドウに入っていない物は描画しない
			//退却したユニット(m_fixがe_ignore)は描画しない
			//描画しないもの(notDrawに格納されているもの)は描画しない
			obj->DrawUnit(obj->getPos(),Vector2D(),infoDrawFlag);
		}
	}
}

void BattleSceneData::DrawHPGage()const{
	for(const Unit *unit:m_unitList){
		if(m_Window->JudgeInShapeRect(unit) && unit->GetFix()!=Shape::Fix::e_ignore){
			//ウインドウに入っていない物は描画しない
			//退却したユニット(typeがe_unitかつfixがe_ignore)は描画しない
			unit->DrawHPGage();
		}
	}
}

void BattleSceneData::DrawOrder()const{
	std::pair<int,int> windowSize=GetWindowResolution();
	auto calDrawPoint=[windowSize](size_t i){return Vector2D((i+1)*Unit::unitCircleSize*2.4f,(float)windowSize.second-Unit::unitCircleSize*1.1f);};
	//オーダー画面の背景を描画
	DrawBox(0,windowSize.second-(int)(Unit::unitCircleSize*1.5f),windowSize.first,windowSize.second,GetColor(128,128,128),TRUE);//背景の描画
	//ユニットのオーダー情報を順番に描画
	for(size_t i=0,size=m_unitList.size();i<size;i++){
		//ユニットアイコン(描画基準点は真ん中)
		const Vector2D centerPoint=calDrawPoint(i);
		m_unitList[i]->DrawFacePic(centerPoint);
		//残りOP
		const int x=(int)centerPoint.x,y=((int)centerPoint.y)-(int)(Unit::unitCircleSize)-20;
		DrawStringCenterBaseToHandle(x,y,std::to_string((int)m_unitList[i]->GetBattleStatus().OP).c_str(),GetColor(255,255,255),m_orderFont,true,GetColor(0,0,0));
	}
	//行動終了時のユニットのオーダー位置予測の矢印の描画
	const size_t arrowNum=2;
	Vector2D arrowPos[arrowNum]={calDrawPoint(0),calDrawPoint(0)};//矢印の先端位置(先頭:行動しない時 後ろ:行動する時)
	bool flag[arrowNum]={false,false};//位置設定が終わったか(先頭:行動しない時 後ろ:行動する時)
	float op[arrowNum]={m_operateUnit->GetBattleStatus().OP,m_operateUnit->ConsumeOPVirtualByCost(m_operateUnit->GetBattleStatus().weapon->GetCost())};//今の位置で行動終了した時のOP(先頭:行動しない時 後ろ:行動する時)
	for(size_t i=1,size=m_unitList.size();i+1<size;i++){
		
	}
}
