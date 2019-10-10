#ifndef DEF_SELECTSUBMISSIONSCENE_H
#define DEF_SELECTSUBMISSIONSCENE_H

#include"BattleSceneElement.h"
#include"BattleSceneData.h"
#include"MyPolygon.h"
#include<array>
#include"SubmissionRuleBase.h"
#include<memory>

//サブミッション設定場面

class SelectSubmissionScene:public BattleSceneElement{
public:
	explicit SelectSubmissionScene(const std::shared_ptr<BattleSceneData> &battleSceneData);
	~SelectSubmissionScene();

protected:
	//仮想関数オーバーライド
	int thisCalculate();//0:直前場面に戻る 1~END-1:その場面に進む END:バトルパート終了
	void thisDraw()const;
	int UpdateNextScene(int index);//次場面の設定。indexに戻りたいが、実際に戻れるかは不明なので実際の戻り場所を示すint値を返し直す。基本的に返り値はindexに等しくなる。
	void ReturnProcess();//この場面に戻ってきた時の処理

private:
	//静的変数
	static const size_t submissionCount=4;//選択できるサブミッションの個数
	static const std::array<MyPolygon,submissionCount> submissionHitJudgeShape;
	//動的変数
	const std::shared_ptr<BattleSceneData> m_battleSceneData;//バトルのデータ
	size_t m_selectIndex;//選択しているサブミッションの配列番号
	std::array<std::shared_ptr<SubmissionRuleBase>,submissionCount> m_submissionList;
};

#endif // !DEF_SELECTSUBMISSIONSCENE_H
