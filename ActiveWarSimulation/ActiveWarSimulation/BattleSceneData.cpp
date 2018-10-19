#include"DxLib.h"
#include"BattleSceneData.h"
#include"input.h"
#include"Edge.h"
#include<algorithm>
#include"GraphicControl.h"
#include"ToolsLib.h"
#include"FileRead.h"
#include"GameScene.h"
#include"CommonConstParameter.h"
#include<math.h>

//----------------------BattleSceneData----------------------
const Vector2D BattleSceneData::mapDrawSize=Vector2D((float)CommonConstParameter::gameResolutionX,900.0f);
const Vector2D BattleSceneData::uiDrawSize=Vector2D(mapDrawSize.x,(float)CommonConstParameter::gameResolutionX-BattleSceneData::mapDrawSize.y);

BattleSceneData::BattleSceneData(const std::string &stagename)
	:m_mapRange(new Terrain(std::shared_ptr<Shape>(new Edge(Vector2D(0.0f,0.0f),mapDrawSize,Shape::Fix::e_ignore)),-1,0,true))
	,m_fpsMesuring(),m_operateUnit(nullptr)
	,m_totalOP(0.0f)
	,m_stageName(stagename)
//	,m_orderFont(CreateFontToHandle("04かんじゅくゴシック",24,4,DX_FONTTYPE_EDGE,-1,2))
	,m_orderFont(LoadFontDataToHandleEX("Font/OrderPalFont.dft",2))
	,m_mapPic(LoadGraphEX(("Stage/"+std::string(stagename)+"/nonfree/map.png").c_str())),m_drawObjectShapeFlag(false)
	,m_mapBGM(LoadBGMMem("Sound/bgm/nonfree/stage1/"))
	,m_aimchangeSound(LoadSoundMem("Sound/effect/nonfree/aimchange.ogg"))
	,m_attackSound(LoadSoundMem("Sound/effect/nonfree/damage.ogg"))
	,m_healSound(LoadSoundMem("Sound/effect/nonfree/recover.ogg"))
	,m_footSound(LoadSoundMem("Sound/effect/nonfree/foot.ogg"))
{
	//グラフィックデータの読み込み
	LoadDivGraphEX("Graphic/drawOrderHelp.png",drawOrderHelpNum,1,drawOrderHelpNum,90,15,m_drawOrderHelp);

	//ファイルからステージを読み込み
	const std::string stagedir("Stage/"+std::string(stagename)+"/");
	//ファイルを開きすべての文字列を書き出す
	std::ifstream ifs((stagedir+"stage.txt").c_str());
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
	m_stageSize=mapDrawSize;//本来はステージの大きさはグラフィックデータの縦横の大きさで決める

	//ファイルからユニットを読み込み
	StringBuilder unitlist(FileStrRead((stagedir+"unitlist.txt").c_str()),'\n','{','}',false,true);
	for(const StringBuilder &unitdata:unitlist.m_vec){
		const StringBuilder sb(unitdata.GetString(),',','(',')',true,true);
		//まずモブ用の設定をするか固定ユニット用の設定をするかを判定する
		bool uniqueFlag=false;
		for(const StringBuilder &ssb:sb.m_vec){
			if(ssb.m_vec.size()>=2 && ssb.m_vec[0].GetString()=="definition"){
				//設定方法はdefinitionに記載されている
				uniqueFlag=(ssb.m_vec[1].GetString()=="unique");
				break;
			}
		}
		//モブか固定かで分岐
		if(uniqueFlag){
			//固定ユニット(未実装)

		} else{
			//モブ
			//各値を宣言。設定したかどうかをpairのsecondに格納
			std::pair<std::string,bool> name;
			name.second=false;
			std::pair<Vector2D,bool> pos;
			pos.second=false;
			std::pair<int,bool> lv;
			lv.second=false;
			std::pair<Unit::Profession::Kind,bool> prof;
			prof.second=false;
			std::pair<Unit::Team::Kind,bool> team;
			team.second=false;
			//各値の読み取り
			for(const StringBuilder &ssb:sb.m_vec){
				if(!ssb.m_vec.empty()){
					//先頭文字列があることを保障
					if(ssb.m_vec[0].GetString()=="name" && ssb.m_vec.size()>=2){
						name.first=ssb.m_vec[1].GetString();
						name.second=true;
					} else if(ssb.m_vec[0].GetString()=="profession" && ssb.m_vec.size()>=2){
						prof.first=Unit::Profession::link(std::atoi(ssb.m_vec[1].GetString().c_str()));
						prof.second=true;
					} else if(ssb.m_vec[0].GetString()=="lv" && ssb.m_vec.size()>=2){
						lv.first=std::atoi(ssb.m_vec[1].GetString().c_str());
						lv.second=true;
					} else if(ssb.m_vec[0].GetString()=="pos" && ssb.m_vec.size()>=3){
						pos.first=Vector2D((float)std::atoi(ssb.m_vec[1].GetString().c_str()),(float)std::atoi(ssb.m_vec[2].GetString().c_str()));
						pos.second=true;
					} else if(ssb.m_vec[0].GetString()=="team" && ssb.m_vec.size()>=2){
						team.first=Unit::Team::link(std::atoi(ssb.m_vec[1].GetString().c_str()));
						team.second=true;
					}
				}
			}
			//各値からユニットを格納
			if(name.second && prof.second && lv.second && pos.second && team.second){
				m_field.push_back(Unit::CreateMobUnit(name.first,prof.first,lv.first,pos.first,team.first));
			}
		}
	}
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
	//グラフィック開放
	DeleteGraphEX(m_mapPic);
	for(size_t i=0;i<drawOrderHelpNum;i++){
		DeleteGraphEX(m_drawOrderHelp[i]);
	}
	//サウンド開放
	StopSoundMem(m_mapBGM);
	DeleteSoundMem(m_mapBGM);
	DeleteSoundMem(m_aimchangeSound);
	DeleteSoundMem(m_attackSound);
	DeleteSoundMem(m_healSound);
	DeleteSoundMem(m_footSound);
	//フォント開放
	DeleteFontToHandleEX(m_orderFont);
	//オブジェクト一覧を開放
	for(BattleObject *obj:m_field){
		delete obj;
	}
}

float BattleSceneData::CalculateOperateUnitFinishOP()const{
	return CalculateOperateUnitFinishOP(m_operateUnit->GetBattleStatus().OP);
}

float BattleSceneData::CalculateOperateUnitFinishOP(float op)const{
	//2番目の先頭ユニットと比較する
	if(m_unitList.size()<2 || m_unitList[1]->GetBattleStatus().OP>op){
		//1体しかユニットがいなければ補正は行わない
		//次の操作ユニットのOPよりopが小さいなら補正は行わない
		return op;
	} else{
		//そうでないなら補正を行う
		return m_unitList[1]->GetBattleStatus().OP-1.0f;
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
	//現在のユニットを一番後ろまで持っていく
	//現在のユニットと同一OPのユニットを、現在のユニットより先に動かせるようにするため
	for(std::vector<Unit *>::const_iterator it=list.begin(),ite=list.end();it!=ite;it++){
		//初期化の時などm_operateUnitがnullptrの時は後ろに持っていくのは起こらない
		if((*it)==m_operateUnit){
			list.erase(it);
			list.push_back(m_operateUnit);
			break;
		}
	}
	//m_unitListはclearして、listから順番にどんどん格納していく
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
	//次の操作ユニットがm_operateUnitのままにならないようにOPを補正する
	//初期化の際(m_operateUnitがnullptrである)にも用いられるので、nullptrの時は補正は必要ないのでしない。
	if(m_operateUnit!=nullptr){
		m_operateUnit->SetOP(CalculateOperateUnitFinishOP());
	}
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
	m_totalOP+=plusOP;//消費したOPにplusOPを加算。
	//m_operateUnitのOPを減らす(コストとして消費するので消費OP増加等の影響を受ける仕様となる)
	//m_operateUnit->AddOP(-Unit::reduceStartActionCost);
	m_operateUnit->ConsumeOPByCost(Unit::reduceStartActionCost);
	//当たり判定図形の変化
	UpdateFix();
	//タイマーセット
	m_fpsMesuring.RecordTime();

}

Unit *BattleSceneData::GetUnitPointer(Vector2D pos)const{
	for(Unit *pu:m_unitList){
		if(pu->JudgePointInsideShape(pos)){
			return pu;
		}
	}
	return nullptr;
}

void BattleSceneData::DrawField(const std::set<const BattleObject *> &notDraw)const{
	//背景データをそのまま描画
	//背景をアニメーションするとかなったらここで処理を記述
	DrawGraph(0,0,m_mapPic,TRUE);
	//フィールドオブジェクトの当たり判定図形の描画（デバッグ機能としてデフォルトはOFF、コマンド入力でONになる）
	if(m_drawObjectShapeFlag){
		//数値情報をprintfDx()
		{
			Vector2D v=GetMousePointVector2D();
			int wx,wy;
			GetWindowSize(&wx,&wy);
			printfDx("mouse:(%f,%f)\nwindowsize:(%d,%d)\nresolution:(%d,%d)\n",v.x,v.y,wx,wy,CommonConstParameter::gameResolutionX,CommonConstParameter::gameResolutionY);
		}
		//当たり判定図形の描画
		for(const BattleObject *obj:m_field){
			if(m_mapRange->JudgeInShapeRect(obj)
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
}

void BattleSceneData::DrawUnit(bool infoDrawFlag,const std::set<const Unit *> &notDraw)const{
	for(const Unit *obj:m_unitList){
		if(m_mapRange->JudgeInShapeRect(obj)
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
		if(m_mapRange->JudgeInShapeRect(unit) && unit->GetFix()!=Shape::Fix::e_ignore){
			//ウインドウに入っていない物は描画しない
			//退却したユニット(typeがe_unitかつfixがe_ignore)は描画しない
			unit->DrawHPGage();
		}
	}
}

void BattleSceneData::DrawOrder(const std::set<const BattleObject *> &lineDraw)const{
	const float opStrDy=20.0f;//OPの数字の描画にウインドウ下端から何px使用するか
	auto calDrawPoint=[opStrDy](size_t i){
		//描画位置をそのまま計算する関数
		float topOpenWidth;//先頭キャラ（＝操作ユニット）だけ離して描画されるようにするための値
		if(i==0){
			topOpenWidth=0.0f;
		} else{
			topOpenWidth=Unit::unitCircleSize;
		}
		return Vector2D((i+1)*Unit::unitCircleSize*2.4f+topOpenWidth,(float)CommonConstParameter::gameResolutionY-Unit::unitCircleSize*1.1f-opStrDy);
	};
	
	//オーダー画面の背景を描画
	//DrawBox(0,windowSize.second-(int)(Unit::unitCircleSize*1.5f),windowSize.first,windowSize.second,GetColor(128,128,128),TRUE);//背景の描画
	

	//ユニットのオーダー情報を順番に描画（タイマーも描画）
	size_t unitListIndex=0;//ユニットリストの参照番号
	bool turnTimerDrawFlag=false;//既にturnTimerを描画したか
	std::vector<float> opVec(m_unitList.size()+1);//OPの値一覧
	for(size_t i=0,size=m_unitList.size();i<size+1;i++){
		const Vector2D centerPoint=calDrawPoint(i);//オーダー画面に乗っけられているユニットアイコンの中心位置
		const float turnTimerOP=fmodf(m_totalOP,Unit::BattleStatus::maxOP);//ターン計測タイマーの表示OP
		float opPal;//アイコンの下に描画するOPの値
		//ユニットを描画するか、タイマーを描画するかでこの先は変化する
		if(turnTimerDrawFlag
			|| unitListIndex==0
			|| (unitListIndex<size && m_unitList[unitListIndex]->GetBattleStatus().OP>=turnTimerOP)
			)
		{
			//既にタイマーを描画している場合、先頭ユニットを描画していない場合は必ずユニット描画。また、turnTimerOPより大きいOPのユニットはタイマーより先に描画
			//マウスが重なっていれば、対応キャラまで線を伸ばす
			//線を伸ばすキャラのリストであるlineDrawにm_unitList[unitListIndex]が入っていても線を伸ばす
			if((GetMousePointVector2D()-centerPoint).sqSize()<Unit::unitCircleSize*Unit::unitCircleSize || lineDraw.find(m_unitList[unitListIndex])!=lineDraw.end()){
				const Vector2D unitDrawPos=m_unitList[unitListIndex]->getPos()-Vector2D();
				//線の描画
				DrawLineAA(centerPoint.x,centerPoint.y,unitDrawPos.x,unitDrawPos.y,GetColor(196,196,196),3.0f);
				//線の中を通る線分のアニメーション
				const int animeDuration=30;//アニメーションの長さ
				const int startDuration=45;//アニメーションが起こる間隔
				const int t=m_fpsMesuring.GetFlame()%startDuration;
				const int lineLength=5;//線分の長さはこのフレーム分
				if(t<animeDuration+lineLength){
					//線分の終端が出現してから始点が消失するまで線分の描画を行うので、AnimeDuration+lineLengthの長さだけ線分は描画する
					if(t<lineLength){
						//始点が出現していない場合
						const Vector2D v=(centerPoint*(float)(animeDuration-t)+unitDrawPos*(float)t)/(float)animeDuration;//終点位置
						DrawLineAA(centerPoint.x,centerPoint.y,v.x,v.y,GetColor(255,255,255));//始点の位置はオーダー描画の位置とする
					} else if(t<animeDuration){
						//始点終点ともに出現している場合
						const Vector2D v1=(centerPoint*(float)(animeDuration+lineLength-t)+unitDrawPos*(float)(t-lineLength))/(float)animeDuration;//始点位置
						const Vector2D v2=(centerPoint*(float)(animeDuration-t)+unitDrawPos*(float)t)/(float)animeDuration;//終点位置
						DrawLineAA(v1.x,v1.y,v2.x,v2.y,GetColor(255,255,255));//始点と終点を結ぶ
					} else{
						//終点が消失している場合
						const Vector2D v=(centerPoint*(float)(animeDuration+lineLength-t)+unitDrawPos*(float)(t-lineLength))/(float)animeDuration;//始点位置
						DrawLineAA(v.x,v.y,unitDrawPos.x,unitDrawPos.y,GetColor(255,255,255));//終点の位置はユニットのマップ描画位置とする
					}
				}
			}
			//ユニットアイコン(描画基準点は真ん中)
			m_unitList[unitListIndex]->DrawFacePic(centerPoint);
			//opPalの初期化
			opPal=m_unitList[unitListIndex]->GetBattleStatus().OP;
			//unitListIndexのインクリメント
			unitListIndex++;
		} else{
			//タイマーアイコンの描画
			DrawCircleAA(centerPoint.x,centerPoint.y,30.0f,9,GetColor(128,128,0),TRUE);
			//opPalの初期化
			opPal=turnTimerOP;
			//もうタイマーを描画しないようにする
			turnTimerDrawFlag=true;
		}
		//残りOP
		const int x=(int)centerPoint.x,y=(int)(CommonConstParameter::gameResolutionY-opStrDy);
		DrawStringCenterBaseToHandle(x,y,std::to_string((int)opPal).c_str(),GetColor(255,255,255),m_orderFont,true,GetColor(0,0,0));
		opVec[i]=opPal;
	}

	//行動終了時のユニットのオーダー位置予測の矢印の描画位置計算
	const size_t arrowNum=drawOrderHelpNum;
	Vector2D arrowPos[arrowNum]={calDrawPoint(0),calDrawPoint(0)};//矢印の先端位置(先頭:行動しない時 後ろ:行動する時)
	float op[arrowNum]={CalculateOperateUnitFinishOP(),CalculateOperateUnitFinishOP(m_operateUnit->ConsumeOPVirtualByCost(m_operateUnit->GetBattleStatus().weapon->GetCost()))};//今の位置で行動終了した時のOP(先頭:行動しない時 後ろ:行動する時)
	const size_t listsize=opVec.size();
	for(size_t j=0;j<arrowNum;j++){
		bool flag=false;//位置設定が終わったか
		for(size_t i=1;i+1<listsize;i++){
			if(!flag && op[j]<=opVec[i] && op[j]>opVec[i+1]){
				//ユニットが入るであろう区間を求める(既に求まっている場合:flag[j]==trueは計算の必要はない)
				//等号位置に注意！現在のユニットと同一OPのユニットを、現在のユニットより先に動かせるようにするようにしているから、次ユニットよりOPが等しい場合はその区間は間違っているようにしないといけない。
				//また、同一OPのユニットが存在する場合は、等号を条件に入れないと入るべき区間を見逃す可能性があるので等号は入れる。
				const float pal=(opVec[i]-op[j])/(opVec[i]-opVec[i+1]);//条件式より0除算にならないことが保証されている
				arrowPos[j]=calDrawPoint(i)*(1.0f-pal)+calDrawPoint(i+1)*pal;//内分点の算出式:p=ap0+bp1(a+b=1,a>=0,b>=0)
				//計算し終わったからflagをtrueにして計算終了
				flag=true;
				break;
			}
		}
		//区間が見つからなかったら一番後ろということ
		if(!flag){
			arrowPos[j]=calDrawPoint(listsize);
			arrowPos[j].x+=(float)(j*8);//ズレさせてあげることで2矢印が完全に被る事を防ぐ
		}
	}
	//矢印を描画
/*
	for(size_t j=0;j<2;j++){
		const float width[2]={8.0f,6.0f};
		const unsigned int color[2]={GetColor(0,0,0),GetColor(255,255,255)};//下地は黒、上は白
		for(size_t i=0;i<arrowNum;i++){
			const float height=Unit::unitCircleSize*0.6f;
			const Vector2D v=calDrawPoint(0)-Vector2D(0.0f,width[0]*2.0f+Unit::unitCircleSize*0.9f);//先頭ユニットの描画基準位置(下地・中抜き部分ともに同じ位置にしたいのでwidth[0]を用いる)
			DrawBoxAA(v.x-width[j],v.y-height-width[j],v.x+width[j],v.y,color[j],TRUE);
			DrawBoxAA(v.x-width[j],v.y-height-width[j],arrowPos[i].x+width[j],v.y-height+width[j],color[j],TRUE);
			DrawBoxAA(arrowPos[i].x-width[j],v.y-height-width[j],arrowPos[i].x+width[j],v.y,color[j],TRUE);
			DrawTriangleAA(arrowPos[i].x,v.y+width[j]*2.0f,arrowPos[i].x-width[j]*2.0f,v.y,arrowPos[i].x+width[j]*2.0f,v.y,color[j],TRUE);
			if(j==0){
				//矢印の分岐の上にヘルプ描画
				int dx,dy;
				GetGraphSize(m_drawOrderHelp[i],&dx,&dy);
				DrawGraph((int)arrowPos[i].x-dx/2,(int)(v.y-height-width[j])-dy*(drawOrderHelpNum-i),m_drawOrderHelp[i],TRUE);
			}
		}
	}
//*/
	const unsigned int color[2][2]={{GetColor(100,162,234),GetColor(231,121,24)},{GetColor(255,255,255),GetColor(255,255,255)}};//下地は黒、上は白
	for(size_t j=0;j<2;j++){
		//奥と手前側に矢印を描画(j=0が奥の色付き、j=1が手前の白)
		const float width[2]={8.0f,4.0f};
		for(size_t i=0;i<arrowNum;i++){
			const float height=15.0f;
			const Vector2D v=calDrawPoint(0)-Vector2D(0.0f,width[0]*2.0f+Unit::unitCircleSize*0.9f);//先頭ユニットの描画基準位置(下地・中抜き部分ともに同じ位置にしたいのでwidth[0]を用いる)
			DrawBoxAA(arrowPos[i].x-width[j],v.y-height-width[j],arrowPos[i].x+width[j],v.y,color[j][i],TRUE);
			DrawTriangleAA(arrowPos[i].x,v.y+width[j]*2.0f,arrowPos[i].x-width[j]*2.0f,v.y,arrowPos[i].x+width[j]*2.0f,v.y,color[j][i],TRUE);
			if(j==0){
				//矢印の分岐の上にヘルプ描画
				int dx,dy;
				GetGraphSize(m_drawOrderHelp[i],&dx,&dy);
				DrawGraph((int)arrowPos[i].x-dx/2,(int)(v.y-height-width[j])-dy*(drawOrderHelpNum-i),m_drawOrderHelp[i],TRUE);
			}
		}
	}
}

bool BattleSceneData::JudgeMousePushInsideMapDrawZone(int mouseCode,bool continuousFlag){
	const Vector2D mouse=GetMousePointVector2D();
	if(mouse.x>=0.0f && mouse.x<mapDrawSize.x && mouse.y>=0.0f && mouse.y<mapDrawSize.y){
		//マップ描画部分にマウスがあるか
		const int f=mouse_get(mouseCode);
		if(continuousFlag){
			//trueなら押しているかどうかを
			return f>0;
		} else{
			//falseなら押した瞬間かどうかを
			return f==1;
		}
	}
	return false;
}
