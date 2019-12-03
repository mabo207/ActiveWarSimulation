#define _USE_MATH_DEFINES
#include"DxLib.h"
#include"input.h"
#include"SubmissionReflectionScene.h"
#include"StageClearScene.h"
#include"ToolsLib.h"
#include"CommonConstParameter.h"
#include"LogElement.h"
#include"GeneralPurposeResource.h"
#include"GraphicControl.h"
#include"FilePath.h"
#include"WaitLog.h"
#include"AttackLog.h"
//リフレクション活動一覧
#include"LineDraw.h"
#include"ObjectClick.h"
#include"SelectOne.h"
#include"MoveSimulation.h"
#include"ReadExplanation.h"
//仮想図形構築のために必要なもの
#include"Edge.h"
#include"MyPolygon.h"
#include"Circle.h"
#include<cmath>

namespace {
	const int lineWidth=5;
	const unsigned int merginColor=GetColor(128,128,255);
	const float twoMinimapRate=0.5f;
	const float oneMinimapRate=0.8f;
	const Vector2D minimapPos[2]={Vector2D(20.0f,300.0f),Vector2D(980.0f,300.0f)};
	const int minimapX[2]={(int)minimapPos[0].x,(int)minimapPos[1].x};
	const int minimapY[2]={(int)minimapPos[0].y,(int)minimapPos[1].y};
}
//---------------SubmissionReflectionScene::MinimapDrawInfo-----------------
SubmissionReflectionScene::MinimapDrawInfo::MinimapDrawInfo(const std::shared_ptr<const LogElement> &log,Unit::Team::Kind phase)
	:pOperateUnit(nullptr),pAttackedUnit(nullptr)
{
	if(log){
		//logの有効性判定
		//関数の作成
		const std::shared_ptr<const WaitLog> waitLog=std::dynamic_pointer_cast<const WaitLog>(log);
		const std::shared_ptr<const AttackLog> attackLog=std::dynamic_pointer_cast<const AttackLog>(log);
		std::function<const Unit *()> getOperateUnit,getAttackedUnit;
		std::function<const std::vector<RouteInfo>()> getRoute;
		if(log->GetLogKind()==LogElement::LogKind::e_wait && waitLog){
			//待機ログの場合
			getOperateUnit=[&waitLog](){return waitLog->GetOperateUnitData().punit;};
			getRoute=[&waitLog](){return waitLog->GetRoute();};
		} else if(log->GetLogKind()==LogElement::LogKind::e_attack && attackLog){
			//攻撃ログの場合
			getOperateUnit=[&attackLog](){return attackLog->GetOperateUnitData().punit;};
			getAttackedUnit=[&attackLog](){return attackLog->GetAimedUnit();};
			getRoute=[&attackLog](){return attackLog->GetRoute();};
		}
		//盤面のユニット一覧の作成
		const size_t maxSize=log->m_unitDataList.size();
		size_t operateIndex=maxSize,attackedIndex=maxSize,index=0;//pOperateUnitとpAttackedUnitの探索に用いる
		for(const LogElement::UnitLogData &logData:log->m_unitDataList){
			//ユニットデータのコピー（加工をするため）
			Unit u=*logData.punit;
			//ユニットデータの加工
			u.Warp(logData.pos);
			u.AddHP(logData.hp-u.GetBattleStatus().HP);
			u.SetOP(logData.op);
			u.SetPenetratable(phase);
			u.SetFix(Shape::Fix::e_static);
			//operateIndex,attackedIndexの探索、関数が存在するかどうか気をつける
			if(getOperateUnit && logData.punit==getOperateUnit()){
				operateIndex=index;
				u.SetPenetratable(u.GetBattleStatus().team);//行動ユニットについては、侵入不可範囲を常に広げない状態にする（見づらいため）
			}
			if(getAttackedUnit && logData.punit==getAttackedUnit()){
				attackedIndex=index;
			}
			//格納
			unitList.push_back(u);
			//探索用indexの更新
			index++;
		}
		//pOperateUnitとpAttackedUnitの探索(これ以降unitListの操作は行われない)
		if(operateIndex<maxSize){
			pOperateUnit=&unitList[operateIndex];
		}
		if(attackedIndex<maxSize){
			pAttackedUnit=&unitList[attackedIndex];
		}
		//移動経路の作成
		if(getRoute){
			for(const RouteInfo &r:getRoute()){
				route.push_back(r);
			}
		}
	}
}

SubmissionReflectionScene::MinimapDrawInfo::~MinimapDrawInfo(){}

//---------------SubmissionReflectionScene::ExtendDraw------------------
void SubmissionReflectionScene::ExtendDraw::DrawMinimap()const{
	m_drawFunc(m_startPos.GetX(),m_startPos.GetY(),(float)(1.0*m_width.GetX()/CommonConstParameter::mapSizeX));
}

//---------------SubmissionReflectionScene-----------------
SubmissionReflectionScene::SubmissionReflectionScene(const std::shared_ptr<BattleSceneData> &battleSceneData,const std::shared_ptr<BattleSceneElement> &clearScene)
	:BattleSceneElement(SceneKind::e_submissionReflection)
	,m_battleSceneData(battleSceneData)
	,m_clearScene(clearScene)
	,m_operateCursor(LoadGraphEX(FilePath::graphicDir+"operatedCursor.png"))
	,m_predictNumberFont(CreateFontToHandleEX("メイリオ",56,8,DX_FONTTYPE_EDGE,-1,4))
	,m_predictExplainFont(CreateFontToHandleEX("メイリオ",20,2,DX_FONTTYPE_EDGE,-1,2))
	,m_nowWork(std::shared_ptr<ReflectionWork::Base>(),std::shared_ptr<MinimapLayoutBase>())
{
	//m_goodLogInfo,m_badLogInfoの初期化
	const WholeReflectionInfo reflectionInfo=m_battleSceneData->m_scoreObserver->GetSubmission().GetReflectionInfo();
	//射手の安全地帯についてのワークなので、敵フェーズの侵入不可範囲設定にする
	const Unit::Team::Kind phaseSetting=Unit::Team::e_enemy;//サブミッションによってここのフェーズ設定が決められる
	m_goodLogInfo.emplace(reflectionInfo.m_goodLog.second,phaseSetting);
	m_badLogInfo.emplace(reflectionInfo.m_badLog.second,phaseSetting);
	//m_workMethodListの初期化
	InitReflectionWork();
}

SubmissionReflectionScene::~SubmissionReflectionScene(){
	DeleteGraphEX(m_operateCursor);
	DeleteFontToHandleEX(m_predictNumberFont);
	DeleteFontToHandleEX(m_predictExplainFont);
}

void SubmissionReflectionScene::DrawResizedMap(int x,int y,const MinimapDrawInfo &minimapInfo,const float rate)const{
	//マージンの描画
	const int minimapWidth=(int)(CommonConstParameter::mapSizeX*rate);
	const int minimapHeight=(int)(CommonConstParameter::mapSizeY*rate);
	DrawBox(x-lineWidth,y-lineWidth,x+minimapWidth+lineWidth,y+minimapHeight+lineWidth,merginColor,TRUE);
	const Vector2D startPos((float)x,(float)y);
	//BattleSceneDataの描画関数は拡大縮小描画に対応していないので、独自に実装する
	//背景描画
	DrawExtendGraphExRateAssign(startPos.x,startPos.y,rate,m_battleSceneData->m_mapPic,TRUE);
	//ルートの描画
	for(size_t i=0,siz=minimapInfo.GetRoute().size();i+1<siz;i++){
		//経路の線分の座標を取得
		Vector2D pos[2]={minimapInfo.GetRoute()[i].pos,minimapInfo.GetRoute()[i+1].pos};
		//ミニマップ用に位置を加工
		for(Vector2D &p:pos){
			p=startPos+p*rate;
		}
		//線分を描画
		DrawLineAA(pos[0].x,pos[0].y,pos[1].x,pos[1].y,GetColor(255,255,0),2.0f);
	}
	//ユニットの描画
	for(size_t i=1,siz=minimapInfo.GetUnitList().size();i<siz+1;i++){
		const size_t index=i%siz;
		//ウインドウに入っていない物は描画しない
		//当たり判定図形のm_fix判定は行わない、そもそも死んでいるかどうかの判定なので、「実際のマップでは死んでいるが仮想マップでは生きているようにしたい」という要望には対応できないから
		//先頭ユニットは最後に描画する
		if(m_battleSceneData->m_mapRange->JudgeInShapeRect(&minimapInfo.GetUnitList()[index])){
			//アクション範囲の描画フラグ
			bool actionRangeDraw,animationFlag;
			if(index==0){
				//先頭ユニットのだけ描画フラグを立てる
				actionRangeDraw=true;
				animationFlag=true;
			} else{
				actionRangeDraw=false;
				animationFlag=false;
			}
			//退却したユニット(m_fixがe_ignore)は描画しない
			minimapInfo.GetUnitList()[index].DrawUnit(minimapInfo.GetUnitList()[index].getPos(),startPos,rate,0,animationFlag,true,actionRangeDraw);
		}
	}
	//ユニットのHPゲージの描画
	for(size_t i=1,siz=minimapInfo.GetUnitList().size();i<siz+1;i++){
		const size_t index=i%siz;
		if(m_battleSceneData->m_mapRange->JudgeInShapeRect(&minimapInfo.GetUnitList()[index])){
			//ウインドウに入っていない物は描画しない
			minimapInfo.GetUnitList()[index].DrawHPGage(minimapInfo.GetUnitList()[index].getPos(),startPos,rate);
		}
	}
	//操作ユニットのマーカーを表示
	if(minimapInfo.GetOperateUnit()!=nullptr){
		float dx,dy;
		GetGraphSizeF(m_operateCursor,&dx,&dy);
		const Vector2D pos=(minimapInfo.GetOperateUnit()->getPos()+Vector2D(0.0f,-Unit::unitCircleSize+10.0f))*rate+startPos-Vector2D(dx/2.0f,dy);
		DrawGraph((int)pos.x,(int)pos.y,m_operateCursor,TRUE);
	}
	//ダメージの表示
	if(minimapInfo.GetOperateUnit()!=nullptr && minimapInfo.GetAttackedUnit()!=nullptr){
		//攻撃時は、攻撃相手の上にダメージを表示
		const Vector2D pos=minimapInfo.GetAttackedUnit()->getPos()*rate+startPos;
		const int centerX=(int)(pos.x),centerY=(int)(pos.y);
		minimapInfo.GetOperateUnit()->GetBattleStatus().weapon->DrawExtendPredict((int)pos.x,(int)pos.y,rate,m_predictExplainFont,m_predictNumberFont,minimapInfo.GetOperateUnit(),minimapInfo.GetAttackedUnit());
	}
}

void SubmissionReflectionScene::DrawTwoMinimap()const{
	if(m_goodLogInfo.has_value()){
		DrawResizedMap(minimapX[0],minimapY[0],m_goodLogInfo.value(),twoMinimapRate);
	}
	if(m_badLogInfo.has_value()){
		DrawResizedMap(minimapX[1],minimapY[1],m_badLogInfo.value(),twoMinimapRate);
	}
}

void SubmissionReflectionScene::SetUnitPenetratable(Unit::Team::Kind phase){
	if(m_goodLogInfo.has_value()){
		for(size_t i=0,siz=m_goodLogInfo->GetUnitList().size();i<siz;i++){
			Unit *pu=m_goodLogInfo->GetUnitListPtr(i);
			if(pu!=m_goodLogInfo->GetOperateUnit() && pu!=nullptr){
				//操作ユニットは視認性向上のために変更できないようにする
				pu->SetPenetratable(phase);
			}
		}
	}
	if(m_badLogInfo.has_value()){
		for(size_t i=0,siz=m_badLogInfo->GetUnitList().size();i<siz;i++){
			Unit *pu=m_badLogInfo->GetUnitListPtr(i);
			if(pu!=m_badLogInfo->GetOperateUnit() && pu!=nullptr){
				//操作ユニットは視認性向上のために変更できないようにする
				pu->SetPenetratable(phase);
			}
		}
	}
}

int SubmissionReflectionScene::thisCalculate(){
	//更新処理
	//ワーク入力処理
	if(m_nowWork.reflection){
		m_nowWork.reflection->Update();
	}
	if(m_nowWork.minimap){
		//マップ描画処理の更新
		m_nowWork.minimap->Update();
	}
	if(m_nowWork.reflection->WorkClear()){
		//ワークをクリアしたら
		if(m_nextWorkMethod==m_workMethodList.end()){
			//最後のワークだったら
			return SceneKind::e_clear;
		} else{
			//まだワークが残っているなら
			m_nowWork=(*m_nextWorkMethod)();//ワーク更新
			m_nextWorkMethod++;//次のワーク作成方法を更新
		}
	}

	return SceneKind::e_submissionReflection;
}

void SubmissionReflectionScene::thisDraw()const{
	//文章の描画
	if(m_nowWork.reflection){
		DrawStringToHandle(30,30,m_nowWork.reflection->GetQuestion().c_str(),GetColor(255,255,255),GeneralPurposeResource::popLargeFont);
	} else{
		DrawStringToHandle(30,30,"どちらの方が良いとされる行動でしょうか？",GetColor(255,255,255),GeneralPurposeResource::popLargeFont);
	}
	//比較マップの描画
	if(m_nowWork.minimap){
		m_nowWork.minimap->DrawMinimap();
	}
	//ワークについての描画
	if(m_nowWork.reflection){
		m_nowWork.reflection->WorkDraw();
	}
}

int SubmissionReflectionScene::UpdateNextScene(int index){
	switch(index){
	case(SceneKind::e_clear):
		m_nextScene=m_clearScene;
		m_clearScene.reset();//m_nextSceneにコピーしたので、所有権を放棄
		break;
	}
	return index;
}

void SubmissionReflectionScene::ReturnProcess(){
	//特に何もしない
}

void SubmissionReflectionScene::InitReflectionWork(){
	//ワーク一覧の作成
	const auto twoMinimapDrawFunc=[this](){
		DrawTwoMinimap();
	};
	const auto oneMinimapDrawFunc=[this](){
		if(m_badLogInfo.has_value()){
			DrawResizedMap(minimapX[0],minimapY[0],m_badLogInfo.value(),oneMinimapRate);
		}
	};
	AddAreaClickWork(std::vector<ShapeClickWorkInfo>{ShapeClickWorkInfo(&m_goodLogInfo,minimapPos[0],twoMinimapRate),ShapeClickWorkInfo(&m_badLogInfo,minimapPos[1],twoMinimapRate)}
		,std::shared_ptr<MinimapLayoutBase>(new NormalDraw(twoMinimapDrawFunc))
		,Unit::Team::e_enemy);
	AddMoveSimulationWork(Unit::Team::e_player);//プレイヤーユニットを動かすため、e_playerを指定
	AddAreaClickWork(std::vector<ShapeClickWorkInfo>{ShapeClickWorkInfo(&m_badLogInfo,minimapPos[0],oneMinimapRate)}
		,std::shared_ptr<MinimapLayoutBase>(new NormalDraw(oneMinimapDrawFunc))
		,Unit::Team::e_enemy);
	//ワーク設定
	m_nextWorkMethod=m_workMethodList.begin();
	m_nowWork=(*m_nextWorkMethod)();
	m_nextWorkMethod++;
}

//ワーク作成関数
void SubmissionReflectionScene::AddDrawLineWork(Unit::Team::Kind phase){
	//攻撃ユニットと被攻撃ユニットを結ぶ線を引くワーク
	const auto lineWorkMethod=[this,phase](){
		//ユニットの侵入可否を敵フェーズ用に設定しておく
		this->SetUnitPenetratable(phase);
		//線分の位置の設定
		const Vector2D goodLogStart=minimapPos[0]+m_goodLogInfo->GetOperateUnit()->getPos()*twoMinimapRate;
		const Vector2D goodLogEnd=minimapPos[0]+m_goodLogInfo->GetAttackedUnit()->getPos()*twoMinimapRate;
		const Vector2D badLogStart=minimapPos[1]+m_badLogInfo->GetOperateUnit()->getPos()*twoMinimapRate;
		const Vector2D badLogEnd=minimapPos[1]+m_badLogInfo->GetAttackedUnit()->getPos()*twoMinimapRate;
		const std::shared_ptr<ReflectionWork::Base> work=std::shared_ptr<ReflectionWork::Base>(new ReflectionWork::LineDraw(
			{Edge(goodLogStart,goodLogEnd-goodLogStart,Shape::Fix::e_ignore)
			,Edge(badLogStart,badLogEnd-badLogStart,Shape::Fix::e_ignore)}
			,"攻撃したキャラと敵との距離を線を引いて確認してみよう！"
		));
		//マップの描画の仕方を設定
		const auto drawFunc=[this](){
			DrawTwoMinimap();
		};
		const std::shared_ptr<MinimapLayoutBase> minimap=std::shared_ptr<MinimapLayoutBase>(new NormalDraw(drawFunc));
		return WorkInfo(work,minimap);
	};
	m_workMethodList.push_back(lineWorkMethod);
}

void SubmissionReflectionScene::AddShapeClickWork(const std::function<std::shared_ptr<const Shape>(Vector2D,Vector2D)> conditionShapeFunc
	,std::vector<ShapeClickWorkInfo> minimapInfo
	,const std::shared_ptr<MinimapLayoutBase> minimapLayout
	,Unit::Team::Kind phase)
{
	const auto clickWorkMethod=[minimapInfo,conditionShapeFunc,minimapLayout,phase,this](){
		//準備
		//ユニットの侵入可否を敵フェーズ用に設定しておく
		this->SetUnitPenetratable(phase);
		std::vector<std::shared_ptr<const Shape>> shapeList;
		//関数の作成
		const auto addFunc=[&shapeList,this](std::shared_ptr<Shape> &addShape,const Vector2D &minimapPosition,const std::shared_ptr<const Shape> &conditionShape,const std::shared_ptr<const Shape> &attackedUnitShape,float rate){
			//地図に合うように加工
			const Vector2D pos=addShape->GetPosition();//現在位置、縮小マップ上の位置を指定するためにこれを用いて移動させないといけない
			addShape->Move(minimapPosition+pos*rate-pos);
			addShape->Resize(addShape->GetRetResize()*rate);
			//条件付き追加
			if(conditionShape->JudgeCross(addShape.get()) || conditionShape->JudgeInShape(addShape.get())){
				//「addShapeがconditionShape内に完全に入っている」もしくは「交点を持つ」場合のみクリック図形リストに追加
				if(!addShape->JudgeInShape(attackedUnitShape.get())){
					//ただし、ユニットをまるっと中に入れている図形（外壁）は除外（外壁はクリックUIを作るのが困難かつ障害物として認識されづらいから）
					shapeList.push_back(addShape);
				}
			}
		};
		const auto addMinimapObject=[&addFunc,&conditionShapeFunc](std::shared_ptr<Shape> &shape,const ShapeClickWorkInfo &minimapInfo){
			//ユニット同士を結ぶ線分の端点
			const Vector2D p0=minimapInfo.startPos+minimapInfo.drawInfo->value().GetAttackedUnit()->getPos()*minimapInfo.rate,
				p1=minimapInfo.startPos+minimapInfo.drawInfo->value().GetOperateUnit()->getPos()*minimapInfo.rate;
			//条件図形の作成
			const std::shared_ptr<const Shape> conditionShape=conditionShapeFunc(p0,p1);
			//攻撃されたユニットの当たり判定図形の作成
			std::shared_ptr<Shape> attackedUnitShape=minimapInfo.drawInfo->value().GetOperateUnit()->GetHitJudgeShape()->VCopy();
			const Vector2D pos=attackedUnitShape->GetPosition();
			attackedUnitShape->Move(minimapInfo.startPos+pos*minimapInfo.rate-pos);
			attackedUnitShape->Resize(attackedUnitShape->GetRetResize()*minimapInfo.rate);
			//図形を作成して条件次第でshapeをリストに追加
			addFunc(shape,minimapInfo.startPos,conditionShape,attackedUnitShape,minimapInfo.rate);
		};
		//ユニットデータ以外の障害物の格納
		for(const BattleObject *object:m_battleSceneData->m_field){
			if(object->GetType()!=BattleObject::Type::e_unit){
				//当たり判定図形を引き出して追加
				for(const ShapeClickWorkInfo &info:minimapInfo){
					addMinimapObject(object->GetHitJudgeShape()->VCopy(),info);
				}
			}
		}
		//ユニットデータの格納
		for(const ShapeClickWorkInfo &info:minimapInfo){
			for(size_t i=0,siz=info.drawInfo->value().GetUnitList().size();i<siz;i++){
				//当たり判定図形を引き出して追加
				if(&info.drawInfo->value().GetUnitList()[i]!=info.drawInfo->value().GetOperateUnit() && &info.drawInfo->value().GetUnitList()[i]!=info.drawInfo->value().GetAttackedUnit()){
					addMinimapObject(info.drawInfo->value().GetUnitList()[i].GetHitJudgeShape()->VCopy(),info);
				}
			}
		}
		//ワークの作成
		const std::shared_ptr<ReflectionWork::Base> clickWork=std::shared_ptr<ReflectionWork::Base>(new ReflectionWork::ObjectClick(shapeList,"攻撃した敵の移動を邪魔している障害物をクリックしてみよう！"));
		return WorkInfo(clickWork,minimapLayout);
	};
	m_workMethodList.push_back(clickWorkMethod);
	//解説ワークの作成
	const auto explanationWorkMethod=[minimapInfo,minimapLayout,this](){
		std::vector<std::pair<std::shared_ptr<const Shape>,unsigned int>> assistList;
		//敵の移動範囲に入っている格子点を全て補助関数に加える
		minimapInfo[0].drawInfo->value().GetUnitListPtr(0);
		for(const ShapeClickWorkInfo &mapinfo:minimapInfo){
			std::vector<BattleObject *> field;
			std::shared_ptr<LatticeBattleField> lField;
			std::vector<LatticeBattleField::LatticeDistanceInfo> dField;
			float moveDistance;
			const unsigned int pointColor=GetColor(0,128,64);
			//フィールドの作成
			for(BattleObject *obj:m_battleSceneData->m_field){
				//全ての障害物を追加
				if(obj->GetType()==BattleObject::Type::e_terrain){
					field.push_back(obj);
				}
			}
			for(size_t i=0;i<mapinfo.drawInfo->value().GetUnitList().size();i++){
				Unit *pu=mapinfo.drawInfo->value().GetUnitListPtr(i);
				if(pu!=mapinfo.drawInfo->value().GetAttackedUnit() && pu!=mapinfo.drawInfo->value().GetOperateUnit()){
					field.push_back(pu);
				}
			}
			//格子点マップの作成
			lField=LatticeBattleField::Create(field,m_battleSceneData->m_stageSize,mapinfo.drawInfo->value().GetOperateUnit(),true);
			//距離マップの作成
			lField->CalculateLatticeDistanceInfo(dField,mapinfo.drawInfo->value().GetAttackedUnit()->getPos());
			//格子点の追加
			moveDistance=mapinfo.drawInfo->value().GetOperateUnit()->GetMaxMoveDistance();
			for(const LatticeBattleField::LatticeDistanceInfo &info:dField){
				if(info.dist<moveDistance){
					const Vector2D pos=lField->CalculateLatticePointPos(info.index)*mapinfo.rate+mapinfo.startPos;
					assistList.push_back(std::make_pair(std::shared_ptr<const Shape>(new Circle(pos,2.0f,Shape::Fix::e_ignore)),pointColor));
				}
			}
		}
		//ワーク作成
		const std::shared_ptr<ReflectionWork::Base> explanationWork(new ReflectionWork::ReadExplanation(assistList,m_nowWork.reflection,"点で表現された敵の動く範囲を見てみると、\n大きく空いている隙間を縫って射手に近づいてくる事がわかります。"));
		return WorkInfo(explanationWork,minimapLayout);
	};
	m_workMethodList.push_back(explanationWorkMethod);
}

void SubmissionReflectionScene::AddLineClickWork(std::vector<ShapeClickWorkInfo> &minimapInfo
	,const std::shared_ptr<MinimapLayoutBase> &minimapLayout
	,Unit::Team::Kind phase)
{
	//攻撃ユニットと被攻撃ユニットを結ぶ線上の障害物をクリックするワーク
	//図形作成関数の作成
	const auto createFunc=[](Vector2D p0,Vector2D p1){
		return std::shared_ptr<Shape>(new Edge(p0,p1-p0,Shape::Fix::e_dynamic));
	};
	//ワーク作成
	AddShapeClickWork(createFunc,minimapInfo,minimapLayout,phase);
}

void SubmissionReflectionScene::AddAreaClickWork(std::vector<ShapeClickWorkInfo> &minimapInfo
	,const std::shared_ptr<MinimapLayoutBase> &minimapLayout
	,Unit::Team::Kind phase)
{
	//攻撃ユニットと被攻撃ユニットを結ぶ線分を対角線とした菱形領域の障害物をクリックするワーク
	//図形作成関数の作成
	const auto createFunc=[this](Vector2D p0,Vector2D p1){
		//p0p1に垂直なベクトルで、p0p1の中点からhだけ進んだ所にある点をp2とすると|p0p2|+|p1p2|がpAttackedUnitの移動距離になるようなベクトル
		Vector2D h=(p1-p0).turn((float)M_PI_2);
		const float unitMoveDistance=m_goodLogInfo->GetAttackedUnit()->GetMaxMoveDistance();
		h=h.norm()*std::powf(unitMoveDistance*unitMoveDistance-h.sqSize(),0.5f)*0.5f;
		return std::shared_ptr<Shape>(new MyPolygon(p0-h,{p0+h,p1+h,p1-h},Shape::Fix::e_dynamic));
	};
	//ワーク作成
	AddShapeClickWork(createFunc,minimapInfo,minimapLayout,phase);
}

void SubmissionReflectionScene::AddSelectOneWork(Unit::Team::Kind phase){
	const auto selectWorkMethod=[this,phase](){
		//ユニットの侵入可否を敵フェーズ用に設定しておく
		this->SetUnitPenetratable(phase);
		//ワークの作成
		const float minimapWidth=(float)CommonConstParameter::mapSizeX*twoMinimapRate;
		const float minimapHeight=(float)CommonConstParameter::mapSizeY*twoMinimapRate;
		const std::shared_ptr<Shape> correct(new MyPolygon(MyPolygon::CreateRectangle(minimapPos[0],Vector2D(minimapWidth,minimapHeight),Shape::Fix::e_static)));
		const std::shared_ptr<Shape> incorrect(new MyPolygon(MyPolygon::CreateRectangle(minimapPos[1],Vector2D(minimapWidth,minimapHeight),Shape::Fix::e_static)));
		const std::shared_ptr<ReflectionWork::Base> work=std::shared_ptr<ReflectionWork::Base>(new ReflectionWork::SelectOne(correct,{incorrect},"どちらの方が適した行動でしょうか？"));
		//マップの描画の仕方を設定
		const auto drawFunc=[this](){
			DrawTwoMinimap();
		};
		const std::shared_ptr<MinimapLayoutBase> minimap=std::shared_ptr<MinimapLayoutBase>(new NormalDraw(drawFunc));
		return WorkInfo(work,minimap);
	};
	m_workMethodList.push_back(selectWorkMethod);
}

void SubmissionReflectionScene::AddMoveSimulationWork(Unit::Team::Kind phase){
	//攻撃キャラの位置を変えてみて評価がどうなるかをシミュレーション学習してみるワーク
	if(m_badLogInfo.has_value()){
		const auto simulationWorkMethod=[this,phase](){
			//ユニットの侵入可否を味方フェーズ用に設定しておく
			this->SetUnitPenetratable(phase);
			//フィールドの作成
			std::vector<BattleObject *> field;
			for(BattleObject *obj:m_battleSceneData->m_field){
				//全ての障害物を追加
				if(obj->GetType()==BattleObject::Type::e_terrain){
					field.push_back(obj);
				}
			}
			for(size_t i=0;i<m_badLogInfo->GetUnitList().size();i++){
				field.push_back(m_badLogInfo->GetUnitListPtr(i));
			}
			//クリア後に行う処理を記述
			const auto clearProcess=[this](){
				//ワークを追加

			};
			//ワークの設定
			const std::shared_ptr<ReflectionWork::Base> work=std::shared_ptr<ReflectionWork::Base>(new
				ReflectionWork::MoveSimulation(field
					,m_badLogInfo->GetOperateUnit()
					,m_battleSceneData->m_stageSize
					,m_badLogInfo->GetAttackedUnit()
					,minimapPos[0]
					,oneMinimapRate
					,m_battleSceneData->m_scoreObserver->GetSubmission().GetRule()
					,"シミュレーション学習"));
			//マップの描画の仕方を設定
			const Easing::TYPE type=Easing::TYPE_OUT;
			const Easing::FUNCTION function=Easing::FUNCTION_QUAD;
			const double degree=4.0;
			const int maxFrame=10;
			const auto drawFunc=[this](int x,int y,float rate){
				if(m_badLogInfo.has_value()){
					DrawResizedMap(x,y,m_badLogInfo.value(),rate);
				}
			};
			const std::shared_ptr<MinimapLayoutBase> minimap=std::shared_ptr<MinimapLayoutBase>(new ExtendDraw(drawFunc
				,PositionControl(minimapPos[1],minimapPos[0],maxFrame,type,function,degree)
				,Easing((int)(CommonConstParameter::mapSizeX*twoMinimapRate),(int)(CommonConstParameter::mapSizeX*oneMinimapRate),maxFrame,type,function,degree)));
			return WorkInfo(work,minimap);
		};
		m_workMethodList.push_back(simulationWorkMethod);
	}
}
