#ifndef DEF_SUBMISSIONREFLECTIONSCENE_H
#define DEF_SUBMISSIONREFLECTIONSCENE_H

#include"BattleSceneElement.h"
#include"BattleSceneData.h"
#include<optional>
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
		const std::vector<Unit>& GetUnitList()const{ return unitList; };//unitListのgetter
		//unitListの中身のどれかを指すポインタ
		const Unit *pOperateUnit;
		const Unit *pAttackedUnit;//nullptrならアクション情報の描画を行わない
		//描画するユニットの一覧
	private:
		std::vector<Unit> unitList;//pOperateUnitとpAttackedUnitがこの要素を指している都合上、外部からの変更を防ぎたいのでprivate
	};

	//関数
	void DrawResizedMap(int x,int y,const MinimapDrawInfo &minimapInfo)const;//簡易拡大縮小マップを描画する
	void InitReflectionWork();//リフレクションワークを作成する処理(描画位置の調整のための情報がこのクラス内に存在するので、WholeReflectionInfoクラスでなくこのクラス内に処理を記述する)
	//仮想関数のオーバーライド
	int thisCalculate();//0:直前場面に戻る 1~END-1:その場面に進む END:バトルパート終了
	void thisDraw()const;
	int UpdateNextScene(int index);//次場面の設定。indexに戻りたいが、実際に戻れるかは不明なので実際の戻り場所を示すint値を返し直す。基本的に返り値はindexに等しくなる。
	void ReturnProcess();//この場面に戻ってきた時の処理
	//ワーク作成関数
	std::shared_ptr<ReflectionWork::Base> CreateDrawLineWork()const;
	std::shared_ptr<ReflectionWork::Base> CreateClickWork(const std::function<std::shared_ptr<const Shape>(Vector2D,Vector2D)> &conditionShapeFunc)const;
	std::shared_ptr<ReflectionWork::Base> CreateLineClickWork()const;
	std::shared_ptr<ReflectionWork::Base> CreateAreaClickWork()const;
	std::shared_ptr<ReflectionWork::Base> CreateSelectOneWork()const;

	//変数
	const std::shared_ptr<BattleSceneData> m_battleSceneData;
	std::shared_ptr<BattleSceneElement> m_clearScene;//前クラスで作成したClearSceneを一時的に保存するための変数
	std::optional<MinimapDrawInfo> m_goodLogInfo,m_badLogInfo;//初期化をコンストラクタの{}内で行いたいので遅延の必要がある
	std::shared_ptr<ReflectionWork::Base> m_reflectionWork;//現在行っているリフレクションワーク

	const int m_operateCursor;//操作ユニットを指し示すためのマーカー
	const int m_predictNumberFont;
	const int m_predictExplainFont;
};

#endif // !DEF_SUBMISSIONREFLECTIONSCENE_H
