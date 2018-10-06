#ifndef DEF_SWITCHUNITSCENE_H
#define DEF_SWITCHUNITSCENE_H

#include"BattleSceneElement.h"
#include"BattleSceneData.h"
#include"ToolsLib.h"

//選択するユニットを切り替えるシーン
class SwitchUnitScene:public BattleSceneElement{
	//型・列挙体
	enum class JudgeEnd{
		e_notEnd
		,e_playerWin
		,e_playerLose
	};

	//定数

	//変数
protected:
	//勝敗判定
	JudgeEnd m_judgeEnd;

	//ゲームの進行データ
	std::shared_ptr<BattleSceneData> m_battleSceneData;

	//タイマー


	//関数
protected:
	virtual std::shared_ptr<BattleSceneElement> GetPlayerOperateClass()const;//プレイヤーキャラを操作するクラスを手に入れる。デフォルトだと自操作クラスだが、継承してデモ用切り替えクラスを作成する場合はAI操作クラスにする。

	//仮想関数のオーバーライド
	int thisCalculate();//0:直前場面に戻る 1~END-1:その場面に進む END:バトルパート終了
	void thisDraw()const;
	int UpdateNextScene(int index);//次場面の設定。indexに戻りたいが、実際に戻れるかは不明なので実際の戻り場所を示すint値を返し直す。基本的に返り値はindexに等しくなる。
	void ReturnProcess();//この場面に戻ってきた時の処理

public:
	SwitchUnitScene(std::shared_ptr<BattleSceneData> battleSceneData);
	virtual ~SwitchUnitScene();
};


#endif // !DEF_SWITCHUNITSCENE_H
#pragma once
