#ifndef DEF_EDITACTIONSETTINGS_H
#define DEF_EDITACTIONSETTINGS_H

#include<memory>
#include<vector>
#include"ToolsLib.h"

//プロトタイプ宣言
class EditAction;
class BattleObject;
class PosSetting;
class ShapeFactory;

//ステージ編集をおこなう際の「何を行うか」の設定を集約したクラス
class EditActionSettings {
	//列挙体・型

	//定数

	//変数
protected:
	//編集データ
	std::vector<std::shared_ptr<BattleObject>> m_objects;//マップ上に設置しているオブジェクト一覧
	Vector2D m_adjust;//描画の左右補正値(実際のステージのm_adjustの座標点を左上に合わせてエディタに表示)
	std::pair<size_t,Vector2D> m_mouseObjectDepth;//編集行為を行う際、マウス点を内部点として持つような図形の何個目を編集するか。右クリックでdepthを増やし、右クリックを押した位置から10px離れると0になる。

	//編集やり直し用データ(編集データにこの変数使って編集してやれば治るよ)
	std::shared_ptr<BattleObject> m_pOriginObject;//編集前のオブジェクトのコピー

public:
	//行為についてのデータ(StageEditor内で自由に変更できる)
	std::shared_ptr<EditAction> m_pEditAction;//編集行為
	std::shared_ptr<BattleObject> m_pBattleObject;//編集対象
	std::shared_ptr<ShapeFactory> m_pShapeFactory;//EditPut時に置く図形
	std::shared_ptr<PosSetting> m_pPosSetting;//位置合わせの手法

	//関数
protected:
	std::vector<std::shared_ptr<BattleObject>>::const_iterator GetMousePointedObject(Vector2D point)const;//pointを含む図形を返す

public:
	//コンストラクタとデストラクタ
	EditActionSettings(std::shared_ptr<EditAction> pEditAction,std::shared_ptr<BattleObject> pBattleObject,std::shared_ptr<ShapeFactory> pShapeFactory,std::shared_ptr<PosSetting> pPosSetting);
	~EditActionSettings();
	//メンバ変数のコピーを返す関数
	Vector2D GetMAdjust()const{
		return m_adjust;
	}
	const std::vector<std::shared_ptr<BattleObject>> *GetPMObject()const{
		return &m_objects;
	}
	const BattleObject *GetMPOriginObject()const{
		return m_pOriginObject.get();
	}
	//現在行おうとしている編集を実行する
	void PracticeEdit(Vector2D point);
	//常に行っている編集を実行する
	void PracticeNonPressEdit(Vector2D point);
	//普通の関数
	void PushScrollBar(float scrollpx,float maxX,float maxY,int mouseX,int mouseY,int leftUpPosX,int leftUpPosY,int mapSizeX,int mapSizeY);//マップのスクロールバーを押した時の処理
	void PushScrollBar(Vector2D move);
	void DrawEditButtonPushed()const;//現在選択されている編集ボタンに対して押されている様子を描画する
	void DrawShapeFactoryButtonPushed()const;//現在選択されている図形設定ボタンに対して押されている様子を描画する
	void DrawPosSettingButtonPushed()const;//現在選択されている位置設定ボタンに対して押されている様子を描画する
	void DrawPosSettingGuide(int leftUpPosX,int leftUpPosY,int mapSizeX,int mapSizeY)const;//位置設定ガイドの描画
	void PutObject(Vector2D point);//現在マウスを指している位置にオブジェクトを設置する
	void RemoveObject(Vector2D point);//現在マウスを指している位置にあるオブジェクトを取り除く(vectorの頭の方から取り除かれる)
	void SetEditObject(Vector2D point);//現在マウスを指している位置にあるオブジェクトを編集対象に設定する
	void CancelEditing();//編集をキャンセルする。編集対象を編集の変更前の状態に戻す
	void InitEditObject();//編集行為を行う際の初期化。m_pBattleObjectとm_pOriginObjectをnullptrに。
	const BattleObject *GetMousePointedObjectPointer(Vector2D point)const;//イテレータを返すGetMousePointedObject()をクラス外部から使えるようにした
	void UpdateMouseObjectDepth(const int keyinputright);//m_mouseObjectDepthを更新する
	void RemoveOriginObject();//m_pOriginObjectをm_objectsから削除する
	//m_objectsの初期化
	void InitObjects();
	//制作データの書き出し
	void WriteOutStage(const char *filename)const;
	//ユニットデータの書き出し
	void WriteOutUnit(const char *filename)const;
	//ステージの読み込み
	void ReadStage(const char *filename);
	//ユニットの読み込み
	void ReadUnit(const char *filename);
};

#endif // !DEF_EDITACTIONSETTINGS_H
#pragma once
