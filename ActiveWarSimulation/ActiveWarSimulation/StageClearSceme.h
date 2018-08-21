#ifndef DEF_STAGECLEARSCENE_H
#define DEF_STAGECLEARSCENE_H

#include"BattleSceneElement.h"
#include"BattleSceneData.h"

//ステージクリア（勝ち負け関係なし）の場面
class StageClearScene:public BattleSceneElement{
	//型・列挙体
	
	//定数

	//変数
protected:
	const bool m_winFlag;//クリアしたかどうか。
	const std::string m_explain;//補足情報
	int m_flame;

	//ゲームの進行データ
	std::shared_ptr<BattleSceneData> m_battleSceneData;

	//グラフィック系
	const int m_stageClearBox;//ステージクリア状況を描画するボックス
	const int m_clearFont;
	const int m_explainFont;

	//関数
protected:
	
	//仮想関数のオーバーライド
	int thisCalculate();//0:直前場面に戻る 1~END-1:その場面に進む END:バトルパート終了
	void thisDraw()const;
	int UpdateNextScene(int index);//次場面の設定。indexに戻りたいが、実際に戻れるかは不明なので実際の戻り場所を示すint値を返し直す。基本的に返り値はindexに等しくなる。
	void ReturnProcess();//この場面に戻ってきた時の処理

public:
	StageClearScene(std::shared_ptr<BattleSceneData> battleSceneData,bool winFlag,const std::string &explain);
	~StageClearScene();
};


#endif // !DEF_STAGECLEARSCENE_H
