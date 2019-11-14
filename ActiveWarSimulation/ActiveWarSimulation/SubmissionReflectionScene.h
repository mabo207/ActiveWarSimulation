#ifndef DEF_SUBMISSIONREFLECTIONSCENE_H
#define DEF_SUBMISSIONREFLECTIONSCENE_H

#include"BattleSceneElement.h"
#include"BattleSceneData.h"
#include<optional>
#include<functional>
#include"ReflectionWorkBase.h"

//サブミッション設定時、リザルト画面の前に振り返りを行う場面
class SubmissionReflectionScene:public BattleSceneElement{
public:
	SubmissionReflectionScene(const std::shared_ptr<BattleSceneData> &battleSceneData,const std::shared_ptr<BattleSceneElement> &clearScene);
	~SubmissionReflectionScene();

private:
	//型
	struct MinimapDrawInfo{
		//ミニマップ描画に必要な情報群
		MinimapDrawInfo(const std::shared_ptr<const LogElement> &log,Unit::Team::Kind phase);
		~MinimapDrawInfo();
		const std::vector<Unit>& GetUnitList()const{ return unitList; }//unitListのgetter
		const std::vector<RouteInfo>& GetRoute()const{ return route; }
		Unit *GetOperateUnit()const{ return pOperateUnit; }
		Unit *GetAttackedUnit()const{ return pAttackedUnit; }
	private:
		//情報群(これらの位置情報は、基準位置調整や拡大率の影響を受けていない、ログそのままの情報である)
		std::vector<Unit> unitList;//pOperateUnitとpAttackedUnitがこの要素を指している都合上、外部からの変更を防ぎたいのでprivate
		std::vector<RouteInfo> route;//pOperateUnitの移動経路
		//unitListの中身のどれかを指すポインタ
		Unit *pOperateUnit;
		Unit *pAttackedUnit;//nullptrならアクション情報の描画を行わない
	};
	//ミニマップの描画の仕方を記録するための構造体
	class MinimapLayoutBase{
	public:
		MinimapLayoutBase(){}
		virtual ~MinimapLayoutBase()=default;
		virtual void DrawMinimap()const=0;
		virtual void Update(){}//何か情報更新が必要あれば行うが、通常は何も更新する必要がない
	};
	class NormalDraw:public MinimapLayoutBase{
	public:
		//普通に描画する
		NormalDraw(const std::function<void()> &drawFunc):m_drawFunc(drawFunc){}
		~NormalDraw()=default;
		void DrawMinimap()const{
			m_drawFunc();
		}
	private:
		const std::function<void()> m_drawFunc;
	};
	class ExtendDraw:public MinimapLayoutBase{
	public:
		//拡大しながら描画する
		ExtendDraw(const std::function<void(int,int,float)> &drawFunc,const PositionControl &startPos,const Easing &width)
			:m_drawFunc(drawFunc)
			,m_startPos(startPos)
			,m_width(width)
		{}
		~ExtendDraw()=default;
		void DrawMinimap()const;
		void Update(){
			m_startPos.Update();
			m_width.Update();
		}
	private:
		const std::function<void(int,int,float)> m_drawFunc;
		PositionControl m_startPos;
		Easing m_width;
	};

	//関数
	void DrawResizedMap(int x,int y,const MinimapDrawInfo &minimapInfo,const float rate)const;//簡易拡大縮小マップを描画する
	void InitReflectionWork();//リフレクションワークを作成する処理(描画位置の調整のための情報がこのクラス内に存在するので、WholeReflectionInfoクラスでなくこのクラス内に処理を記述する)
	void DrawTwoMinimap()const;//2つならべて普通にミニマップを描画する
	//仮想関数のオーバーライド
	int thisCalculate();//0:直前場面に戻る 1~END-1:その場面に進む END:バトルパート終了
	void thisDraw()const;
	int UpdateNextScene(int index);//次場面の設定。indexに戻りたいが、実際に戻れるかは不明なので実際の戻り場所を示すint値を返し直す。基本的に返り値はindexに等しくなる。
	void ReturnProcess();//この場面に戻ってきた時の処理
	//ワーク作成関数
	void SetDrawLineWork();
	void SetClickWork(const std::function<std::shared_ptr<const Shape>(Vector2D,Vector2D)> &conditionShapeFunc);
	void SetLineClickWork();
	void SetAreaClickWork();
	void SetSelectOneWork();
	void SetMoveSimulationWork();

	//変数
	const std::shared_ptr<BattleSceneData> m_battleSceneData;
	std::shared_ptr<BattleSceneElement> m_clearScene;//前クラスで作成したClearSceneを一時的に保存するための変数
	std::optional<MinimapDrawInfo> m_goodLogInfo,m_badLogInfo;//初期化をコンストラクタの{}内で行いたいので遅延の必要がある
	std::shared_ptr<ReflectionWork::Base> m_reflectionWork;//現在行っているリフレクションワーク
	std::shared_ptr<MinimapLayoutBase> m_layoutInfo;//ミニマップをどう表示するか

	const int m_operateCursor;//操作ユニットを指し示すためのマーカー
	const int m_predictNumberFont;
	const int m_predictExplainFont;
};

#endif // !DEF_SUBMISSIONREFLECTIONSCENE_H
