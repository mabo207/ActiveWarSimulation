#include"DxLib.h"
#include"FilePath.h"
#include"GraphicControl.h"
#include"BGMManager.h"
#include"CreditScene.h"
#include"CommonConstParameter.h"
#include"ToolsLib.h"
#include"input.h"

#include"TitleScene.h"

namespace {
	const int professionHeight=60;
	const int nameHeight=45;
	const int pairMergin=20;
	const int creditSpeed=3;
}

//-----------------------CreditScene-----------------
std::shared_ptr<GameScene> CreditScene::CreditSceneFactory::CreateIncompleteScene()const{
	return std::shared_ptr<GameScene>(new CreditScene());
}

CreditScene::CreditScene()
	:m_creditList{CreditPair("企画・プログラム・ドット絵など",std::vector<std::string>{"まーぼう"})
		,CreditPair("イラスト・キャラクターデザイン",std::vector<std::string>{"文苺"})
		,CreditPair("使用させていただいた素材やライブラリ",std::vector<std::string>{"DXライブラリ 様","ザ・マッチメイカァズ 様","フリーBGM・音楽素材MusMus 様","Wingless Seraph 様","あおいりい 様(りいポップ角 R)"})
		,CreditPair("協力",std::vector<std::string>{"東工大ロボット技術研究会の皆様","東工大デジタル創作同好会traPの皆様","大学の友人や先生の方々"})}
	,m_professionFont(LoadFontDataToHandleEX(FilePath::fontDir+"HugeGothicFont.dft",0))
	,m_nameFont(LoadFontDataToHandleEX(FilePath::fontDir+"LargeThickGothicFont.dft",0))
	,m_backPic(LoadGraphEX(FilePath::graphicDir+"nonfree/titleScene.png"))
	,m_bgm(Resource::BGM::Load("credit.txt"))
	,m_strY(CommonConstParameter::gameResolutionY)
	,m_strHeight(0)
{
	//m_strHeightの計算
	for(const CreditPair &pair:m_creditList){
		//役職の分の高さの加算
		m_strHeight+=professionHeight;
		//名前の分の高さの加算
		m_strHeight+=nameHeight*pair.nameVec.size();
		//マージンの高さの加算
		m_strHeight+=pairMergin;
	}
}

CreditScene::~CreditScene(){
	DeleteFontToHandleEX(m_professionFont);
	DeleteFontToHandleEX(m_nameFont);
	DeleteGraphEX(m_backPic);
	m_bgm.Delete();
	BGMManager::s_instance.value().Stop();//意図的に止める
}

void CreditScene::InitCompletely(){
	//特にすることがない
}

void CreditScene::Activate(){
	//bgm再生
	if(BGMManager::s_instance.has_value()){
		BGMManager::s_instance->PlayWithCopy(m_bgm);
	}
}

int CreditScene::Calculate(){
	if(keyboard_get(KEY_INPUT_Z)>0 || mouse_get(MOUSE_INPUT_LEFT)>0){
		//5倍速
		m_strY-=creditSpeed*5;
	} else{
		m_strY-=creditSpeed;
	}
	if(m_strY+m_strHeight<-300){
		return 1;
	}
	return 0;
}

void CreditScene::Draw()const{
	//背景の描画
	DrawGraph(0,0,m_backPic,TRUE);
	//黒被せ
	int mode,pal;
	GetDrawBlendMode(&mode,&pal);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA,128);
	DrawBox(0,0,CommonConstParameter::gameResolutionX,CommonConstParameter::gameResolutionY,GetColor(0,0,0),TRUE);
	SetDrawBlendMode(mode,pal);
	//文字列の描画
	int drawY=m_strY;
	for(const CreditPair &pair:m_creditList){
		//役職の描画
		DrawStringCenterBaseToHandle(CommonConstParameter::gameResolutionX/2,drawY,pair.profession.c_str(),GetColor(255,255,255),m_professionFont,false);
		drawY+=professionHeight;
		//名前の描画
		for(const std::string &name:pair.nameVec){
			DrawStringCenterBaseToHandle(CommonConstParameter::gameResolutionX/2,drawY,name.c_str(),GetColor(255,255,255),m_nameFont,false);
			drawY+=nameHeight;
		}
		//マージンの高さの加算
		drawY+=pairMergin;
	}
}

std::shared_ptr<GameScene> CreditScene::VGetNextScene(const std::shared_ptr<GameScene> &thisSharedPtr)const{
	const auto titleFactory=std::make_shared<TitleScene::TitleSceneFactory>();
	return CreateFadeOutInSceneCompletely(thisSharedPtr,titleFactory,15,15);
}
