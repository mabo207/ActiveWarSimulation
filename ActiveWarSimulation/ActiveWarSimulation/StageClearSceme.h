#ifndef DEF_STAGECLEARSCENE_H
#define DEF_STAGECLEARSCENE_H

#include"BattleSceneElement.h"
#include"BattleSceneData.h"

//ステージクリア（勝ち負け関係なし）の場面
class StageClearScene:public BattleSceneElement{
	//型・列挙体
	
	//定数
protected:
	static const int bonusFontSize;
	static const int scoreFontSize;

	//変数
protected:
	const bool m_winFlag;//クリアしたかどうか。
	int m_frame;

	//ゲームの進行データ
	std::shared_ptr<BattleSceneData> m_battleSceneData;
	const std::shared_ptr<ScoreObserver::ScoreExpression> m_scoreExpression;//スコア表示のためのデータ

	//グラフィック系
	const std::pair<int,int> m_backPic;//背景のハンドルとその透明度
	const std::pair<int,int> m_bonusBar;//ボーナス描画領域のハンドルとその透明度
	const std::pair<int,int> m_turnBar;//ターン数描画領域のハンドルとその透明度
	const std::pair<int,int> m_survivalBar;//生存数描画領域のハンドルとその透明度
	const int m_resultBarPic;//勝った・負けたのバーの描画ハンドル
	const int m_scoreBarPic;//合計スコア描画領域のハンドル

	//フォント系
	const int m_bonusFont;//ボーナス項目表示のフォント
	const int m_scoreFont;//スコア数値表示のフォント

	//関数
protected:
	
	//仮想関数のオーバーライド
	int thisCalculate();//0:直前場面に戻る 1~END-1:その場面に進む END:バトルパート終了
	void thisDraw()const;
	int UpdateNextScene(int index);//次場面の設定。indexに戻りたいが、実際に戻れるかは不明なので実際の戻り場所を示すint値を返し直す。基本的に返り値はindexに等しくなる。
	void ReturnProcess();//この場面に戻ってきた時の処理

public:
	StageClearScene(std::shared_ptr<BattleSceneData> battleSceneData,bool winFlag);
	~StageClearScene();
};


#endif // !DEF_STAGECLEARSCENE_H
