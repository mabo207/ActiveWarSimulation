#include"ResearchScene.h"
#include"DxLib.h"
#include"GraphicControl.h"
#include"CommonConstParameter.h"

//-------------------ResearchScene---------------------
const int ResearchScene::explainFontSize=16;

ResearchScene::ResearchScene(std::shared_ptr<BattleSceneData> battleSceneData)
	:BattleSceneElement(SceneKind::e_research)
	,m_palFont(CreateFontToHandleEX("メイリオ",28,1,-1))
	,m_nameFont(CreateFontToHandleEX("メイリオ",32,1,-1))
	,m_explainFont(CreateFontToHandleEX("メイリオ",explainFontSize,1,-1))
	,m_palBackPic(LoadGraphEX("Graphic/researchInfoBack.png"))
	,m_battleSceneData(battleSceneData)
	,m_researchPic(LoadGraphEX("Graphic/operatedCursor.png"))
	,m_mousePosJustBefore(GetMousePointVector2D())
	,m_pointerVec(battleSceneData->m_operateUnit->getPos())
	,m_moveButton(1620,980,80,80,LoadGraphEX("Graphic/backButton.png"))
{
	//操作ユニット等の初期化
	UpdatePointer();
}

ResearchScene::~ResearchScene(){
	DeleteFontToHandleEX(m_palFont);
	DeleteFontToHandleEX(m_nameFont);
	DeleteFontToHandleEX(m_explainFont);
	DeleteGraphEX(m_palBackPic);
	DeleteGraphEX(m_researchPic);
}

void ResearchScene::UpdatePointer(){
	//ひとまずマウスを使って操作するのを想定する
	const Vector2D mouseVec=GetMousePointVector2D();
	if(keyboard_get(KEY_INPUT_A)==1 || keyboard_get(KEY_INPUT_S)==1){
		//A:オーダーの末尾から先頭へ調べるユニットを進める
		//S:オーダーの先頭から末尾へ調べるユニットを進める
		//今何番目のユニットを調べているかを判定
		size_t researchIndex=0;
		const size_t size=m_battleSceneData->m_unitList.size();
		for(;researchIndex<size;researchIndex++){
			if(m_battleSceneData->m_unitList[researchIndex]==m_researchUnit){
				break;
			}
		}
		//オーダー見て進める
		size_t index=researchIndex;//進めた先。sizeならオーダーの誰でもないことを意味する。
		if(keyboard_get(KEY_INPUT_A)==1){
			index=(researchIndex+size)%(size+1);//もどる
		} else if(keyboard_get(KEY_INPUT_S)==1){
			index=(researchIndex+1)%(size+1);//すすむ
		}
		//進めた先のキャラの位置まで調べる場所を動かす
		if(index<size){
			m_pointerVec=m_battleSceneData->m_unitList[index]->getPos();
		} else{
			//誰も調べないという状態になった場合は
			//特に何もしない
		}
	}else{
		//ユニット切り替えを用いなかった時は、マウス・ジョイスティック・キー入力で調べる位置を細かく移動
		if(JudgeMouseInWindow() && (mouseVec-m_mousePosJustBefore).sqSize()>1.0f){
			//マウスの移動が大きいならば、マウスの位置を調査場所とする
			//マウスがウインドウに入っていない時は更新しないほうが無難
			//誤差許容が大きすぎるとマウスを動かしてもカーソルが動かず気持ち悪い
			m_pointerVec=mouseVec;
		} else if(analogjoypad_get(DX_INPUT_PAD1)!=Vector2D()){
			//ジョイスティックが動いたならば、それを参考に調査場所を移動する
			Vector2D stick=analogjoypad_get(DX_INPUT_PAD1);
			//アナログスティックの物理的なズレ等によるstickの微入力を除く
			const float gap=50.0f;
			if(std::abs(stick.x)<gap){
				stick.x=0.0f;
			}
			if(std::abs(stick.y)<gap){
				stick.y=0.0f;
			}
			m_pointerVec+=stick/64.0f;
		} else{
			//これ以外はキー入力
			//キーボード入力しているなら、それを用いる
			Vector2D moveVec=Vector2D();
			if(keyboard_get(KEY_INPUT_UP)>4){
				moveVec+=Vector2D(0.0f,-15.0f);
			} else if(keyboard_get(KEY_INPUT_UP)>0){
				moveVec+=Vector2D(0.0f,-1.0f);
			}
			if(keyboard_get(KEY_INPUT_LEFT)>4){
				moveVec+=Vector2D(-15.0f,0.0f);
			} else if(keyboard_get(KEY_INPUT_LEFT)>0){
				moveVec+=Vector2D(-1.0f,0.0f);
			}
			if(keyboard_get(KEY_INPUT_RIGHT)>4){
				moveVec+=Vector2D(15.0f,0.0f);
			} else if(keyboard_get(KEY_INPUT_RIGHT)>0){
				moveVec+=Vector2D(1.0f,0.0f);
			}
			if(keyboard_get(KEY_INPUT_DOWN)>4){
				moveVec+=Vector2D(0.0f,15.0f);
			} else if(keyboard_get(KEY_INPUT_DOWN)>0){
				moveVec+=Vector2D(0.0f,1.0f);
			}
			m_pointerVec+=moveVec;
		}
	}
	//マウスの直前位置の更新
	m_mousePosJustBefore=mouseVec;
	//ポインターの位置にいるユニットをm_researchUnitに格納する
	m_researchUnit=m_battleSceneData->GetUnitPointer(m_pointerVec);
}

int ResearchScene::thisCalculate(){
	//ポインターの更新
	UpdatePointer();

	//遷移処理
	if(keyboard_get(KEY_INPUT_F)==1 || keyboard_get(KEY_INPUT_X)==1 || m_moveButton.JudgePressMoment()){
		return 0;//マップ調べモード切替ボタンまたは戻るボタン入力で直前場面へ
	}

	return SceneKind::e_research;
}

void ResearchScene::thisDraw()const{
	//フィールドの描画
	m_battleSceneData->DrawField();

	//ユニットの描画
	m_battleSceneData->DrawUnit(true,std::set<const Unit *>{m_battleSceneData->m_operateUnit,m_researchUnit});

	//調べているユニットについての描画
	if(m_researchUnit!=nullptr){
		m_researchUnit->BattleObject::VDraw();
		m_researchUnit->GetHitJudgeShape()->Draw(Vector2D(),GetColor(255,255,255),FALSE);//調べてるよってことを強調
		m_researchUnit->DrawMaxMoveInfo();//最大移動距離を表示
	}

	//操作中ユニットの描画(m_researchUnitとかぶっていたら描画する必要はない)
	if(m_battleSceneData->m_operateUnit!=m_researchUnit){
		m_battleSceneData->m_operateUnit->BattleObject::VDraw();
		Vector2D pos=m_battleSceneData->m_operateUnit->getPos();
		//DrawTriangleAA(pos.x-15.0f,pos.y-60.0f,pos.x+15.0f,pos.y-60.0f,pos.x,pos.y-30.0f,GetColor(255,255,0),TRUE);
	}


	//全ユニットのHPゲージの描画
	m_battleSceneData->DrawHPGage();

	//ユニットのオーダー順番を描画
	m_battleSceneData->DrawOrder(std::set<const BattleObject*>{m_researchUnit});

	//移動シーンに戻るボタンの描画
	m_moveButton.DrawButton();

	//調べている場所がどこかを描画
	{
		//矢印指ししてさらに強調
		const Vector2D pos=m_pointerVec;
		float dx,dy;
		GetGraphSizeF(m_researchPic,&dx,&dy);
		DrawGraph((int)(pos.x-dx/2.0f),(int)(pos.y-dy),m_researchPic,TRUE);
	}

	//パラメータの描画
	if(m_researchUnit!=nullptr){
		//パラメータの表示
		int gx=1200,gy=140;//パラメータ画面全体の描画位置
		if(m_researchUnit->getPos().x<CommonConstParameter::gameResolutionX/2){
			//ユニットの位置が画面左半分にいるなら
			gx=1200;
		} else{
			//右半分なら
			gx=120;
		}
		DrawGraph(gx,gy,m_palBackPic,TRUE);
		const Unit::BaseStatus base=m_researchUnit->GetBaseStatus();
		const Unit::BattleStatus battle=m_researchUnit->GetBattleStatus();
		//ネームプレート部分
		m_researchUnit->DrawFacePic(Vector2D((float)(gx+70),(float)(gy+60)));
		DrawStringToHandle(gx+150,gy+12,base.name.c_str(),GetColor(255,255,255),m_nameFont);
		DrawFormatStringToHandle(gx+180,gy+65,GetColor(255,255,255),m_palFont,"%s Lv%d",Unit::Profession::GetName(base.profession).c_str(),base.lv);
		//パラメータ部分
		DrawFormatStringToHandle(gx+68,gy+159,GetColor(255,255,255),m_palFont,"%d/%d",battle.HP,base.maxHP);
		DrawFormatStringToHandle(gx+68,gy+203,GetColor(255,255,255),m_palFont,"%.0f/%.0f",battle.OP,battle.maxOP);
		DrawFormatStringToHandle(gx+94,gy+247,GetColor(255,255,255),m_palFont,"%d",base.power);
		DrawFormatStringToHandle(gx+94,gy+291,GetColor(255,255,255),m_palFont,"%d",base.def);
		DrawFormatStringToHandle(gx+94,gy+335,GetColor(255,255,255),m_palFont,"%d",base.mpower);
		DrawFormatStringToHandle(gx+94,gy+379,GetColor(255,255,255),m_palFont,"%d",base.mdef);
		DrawFormatStringToHandle(gx+94,gy+423,GetColor(255,255,255),m_palFont,"%d",base.speed);
		DrawFormatStringToHandle(gx+94,gy+467,GetColor(255,255,255),m_palFont,"%d",base.move);
		//装備説明部分
		//通常装備
		DrawStringToHandle(gx+261,gy+159,m_researchUnit->GetBattleStatus().weapon->GetName().c_str(),GetColor(255,255,255),m_palFont);
		DrawStringNewLineToHandle(gx+272,gy+192,310,36,m_researchUnit->GetBattleStatus().weapon->GetExplain().c_str(),GetColor(255,255,255),m_explainFont,2);
		DrawStringNewLineToHandle(gx+272,gy+240,310,36,m_researchUnit->GetBattleStatus().weapon->GetEffectivenessString(m_researchUnit).c_str(),GetColor(255,255,255),m_explainFont,2);
	}
}

int ResearchScene::UpdateNextScene(int index){
	//特に何もしない
	return index;
}

void ResearchScene::ReturnProcess(){
	//特に何もしない
}
