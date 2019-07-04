#include"DxLib.h"
#include"SystemMenu.h"
#include"input.h"
#include"GraphicControl.h"
#include"GeneralPurposeResource.h"
#include"CommonConstParameter.h"
#include"FilePath.h"

//-------------------SystemMenu-----------------
const int SystemMenu::contentX[SystemMenu::indexCount]={720,1200};
const int SystemMenu::contentY[SystemMenu::indexCount]={630,630};
const int SystemMenu::contentWidth=400;
const int SystemMenu::contentHeight=100;
const int SystemMenu::sentenceX=CommonConstParameter::gameResolutionX/2;
const int SystemMenu::sentenceY=CommonConstParameter::gameResolutionY/2-90;
const std::string SystemMenu::contentStr[SystemMenu::indexCount]={"つづける！","あきらめる"};

SystemMenu::SystemMenu(std::shared_ptr<BattleSceneData> battleSceneData)
	:BattleSceneElement(SceneKind::e_systemMenu)
	,m_battleSceneData(battleSceneData)
	,m_index(0)
	,m_systemBoard(LoadGraphEX(FilePath::graphicDir+"systemBoard.png"))
	,m_contentFont(CreateFontToHandleEX("メイリオ",48,7,DX_FONTTYPE_ANTIALIASING_EDGE_4X4,-1,3))
{}

SystemMenu::~SystemMenu(){
	DeleteGraphEX(m_systemBoard);
	DeleteFontToHandleEX(m_contentFont);
}

int SystemMenu::thisCalculate(){
	//マウスの位置を取得
	const Vector2D mouse=GetMousePointVector2D();

	//選択項目の更新
	bool inRectPushMoment=false;//文字上で左クリックしたかどうかの判定。ここでやっておくほうが処理が簡潔
	const size_t beforeIndex=m_index;
	if(keyboard_get(KEY_INPUT_LEFT)==1 || keyboard_get(KEY_INPUT_UP)==1){
		//←↑キーで１つ前に
		m_index=(m_index+indexCount-1)%indexCount;
	} else if(keyboard_get(KEY_INPUT_RIGHT)==1 || keyboard_get(KEY_INPUT_DOWN)==1){
		//→↓キーで１つ後に
		m_index=(m_index+1)%indexCount;
	} else{
		//マウスによる更新判定
		const int mouseX=(int)mouse.x,mouseY=(int)mouse.y;
		for(size_t i=0;i<indexCount;i++){
			const int dx=mouseX-contentX[i],dy=mouseY-contentY[i];
			if(dx>-contentWidth/2 && dx<contentWidth/2 && dy>-contentHeight/2 && dy<contentHeight/2){
				//文章を表示する長方形内に入っているかどうか
				inRectPushMoment=(mouse_get(MOUSE_INPUT_LEFT)==1);//文字上で左クリックしたか
				if((mouse-m_beforeFrameMousePos).sqSize()>=25.0f){
					//マウスをある程度動かしているなら、マウスによる更新判定をする
					m_index=i;
					break;
				}
			}
		}
	}
	if(m_index!=beforeIndex){
		//項目が変わっていたら音を鳴らす
		PlaySoundMem(GeneralPurposeResource::selectSound,DX_PLAYTYPE_BACK,TRUE);
	}

	//場面遷移
	bool backFlag=false;
	if(keyboard_get(KEY_INPUT_Z)==1 || inRectPushMoment){
		//項目を決定した場合
		switch(m_index){
		case(0):
			//続ける
			backFlag=true;
			break;
		case(1):
			//ギブアップする
			PlaySoundMem(GeneralPurposeResource::decideSound,DX_PLAYTYPE_BACK,TRUE);
			return SceneKind::END;
			break;
		}
	}
	if(backFlag 
		|| keyboard_get(KEY_INPUT_SPACE)==1 
		|| keyboard_get(KEY_INPUT_X)==1
		|| mouse_get(MOUSE_INPUT_RIGHT)==1
		)
	{
		//項目決定によって前に戻るか、戻るボタンを押したら
		PlaySoundMem(GeneralPurposeResource::decideSound,DX_PLAYTYPE_BACK,TRUE);
		return 0;
	}

	//前フレームのマウスの位置を更新
	m_beforeFrameMousePos=mouse;

	return SceneKind::e_systemMenu;
}

void SystemMenu::thisDraw()const{
	//バトル画面を暗く表示
	m_battleSceneData->DrawField();//フィールドの描画
	m_battleSceneData->DrawUnit(true);//ユニットの描画
	m_battleSceneData->DrawHPGage();//全ユニットのHPゲージの描画
	m_battleSceneData->DrawOrder();//ユニットのオーダー順番を描画
	int mode,pal;
	GetDrawBlendMode(&mode,&pal);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA,128);
	DrawBox(0,0,CommonConstParameter::gameResolutionX,CommonConstParameter::gameResolutionY,GetColor(0,0,0),TRUE);
	SetDrawBlendMode(mode,pal);
	//システムメニュー描画
	DrawRotaGraph(CommonConstParameter::gameResolutionX/2,CommonConstParameter::gameResolutionY/2,1.0,0.0,m_systemBoard,TRUE);
	DrawStringCenterBaseToHandle(sentenceX,sentenceY,"バトルをつづける？",GetColor(255,255,255),m_contentFont,true,GetColor(0,0,0));
	for(size_t i=0;i<indexCount;i++){
		unsigned int inColor,edgeColor;
		if(i==m_index){
			inColor=GetColor(255,255,255);
			edgeColor=GetColor(0,0,0);
		} else{
			inColor=GetColor(128,128,128);
			edgeColor=GetColor(0,0,0);
		}
		DrawStringCenterBaseToHandle(contentX[i],contentY[i],contentStr[i].c_str(),inColor,m_contentFont,true,edgeColor);
	}
}

int SystemMenu::UpdateNextScene(int index){
	//特に何もしない
	return index;
}

void SystemMenu::ReturnProcess(){
	//特に何もしない
}

