#include<iostream>
#include<cassert>
#include"DxLib.h"
#include"GraphicControl.h"


/*GraphicControlClassを用いるための関数*/
static GraphicControlClass *GraphicControler=NULL;

void GraphicControler_Init(){
	GraphicControler=new GraphicControlClass;
	int i=0;
}

void GraphicControler_End(){
	if(GraphicControler!=NULL){
		delete GraphicControler;
		GraphicControler=NULL;
	}
}

int LoadGraphEX(std::string filename){
	if(GraphicControler==NULL){
		return -1;
	}
	return GraphicControler->LoadGraphEX(filename);
}

int LoadDivGraphEX(std::string filename,int Allnum,int XNum,int YNum,int XSize,int YSize,int *HandleBuf){
	if(GraphicControler==NULL){
		return -1;
	}
	return GraphicControler->LoadDivGraphEX(filename,Allnum,XNum,YNum,XSize,YSize,HandleBuf);
}

int CopyGraph(int GRHandle){
	return GraphicControler->CopyGraph(GRHandle);
}

int DeleteGraphEX(int GRHandle){
	if(GraphicControler==NULL){
		return -1;
	}
	return GraphicControler->DeleteGraphEX(GRHandle);
}

/*GraphicControlClass*///内部定義したGraphicData方の戻り値の書き方に注意！！
//m_Graphicの中から欲しいデータを探す(同じ名前のものは一番配列番号の小さいものが返ってくる)
std::vector<GraphicControlClass::GraphicData>::iterator GraphicControlClass::GraphicSearch(int GRHandle){
	//イテレータを用意
	std::vector<GraphicData>::iterator pGraphic=m_Graphic.begin();
	for(;pGraphic!=m_Graphic.end();pGraphic++){
		if(pGraphic->m_handle==GRHandle){
			return pGraphic;
		}
	}
	//何も示さないイテレータはEndのイテレータ。エラー時はm_Graphic.End()を返す
	return m_Graphic.end();
}

std::vector<GraphicControlClass::GraphicData>::iterator GraphicControlClass::GraphicSearch(std::string GRName){
	//イテレータを用意
	std::vector<GraphicData>::iterator pGraphic=m_Graphic.begin();
	for(;pGraphic!=m_Graphic.end();pGraphic++){
		if(pGraphic->m_name==GRName){
			return pGraphic;
		}
	}
	//何も示さないイテレータはEndのイテレータ。エラー時はm_Graphic.End()を返す
	return m_Graphic.end();
}

//グラフィックの読み込み
int GraphicControlClass::LoadGraphEX(std::string filename){
	//まず同じデータがないかを確認する。
	std::vector<GraphicData>::iterator pData=GraphicSearch(filename);
	if( pData != m_Graphic.end() ){//同じデータがあるなら
		pData->m_number++;//グラフィックデータの用いられている箇所を増やす。
		return pData->m_handle;//グラフィックハンドルを返す
	}
	//同じデータがなければ末尾にデータを追加
	const int GRHandle=LoadGraph(filename.c_str());
	if(GRHandle!=-1){//エラーが起きていなければ
		m_Graphic.push_back(GraphicData(GRHandle,filename,1));//m_Graphicにデータを追加。1箇所で用いられている
	}
	return GRHandle;
}

//グラフィックの分割読み込み
int GraphicControlClass::LoadDivGraphEX(std::string filename,int Allnum,int XNum,int YNum,int XSize,int YSize,int *HandleBuf){
	//分割数が0以下なら失敗
	if(Allnum<=0){
		return -1;
	}
	int flag=0;//成功かどうかのフラグ
	//名前と分割数でダブリの存在を検索
	std::vector<DivGraphicData>::iterator pData=m_DivGraphic.begin();
	for(;pData!=m_DivGraphic.end();pData++){
		if(pData->m_name==filename && pData->m_divNUM==Allnum){//被っていたら
			break;//イテレータをそのままにして返す
		}
	}
	//ダブリ存在時
	if(pData!=m_DivGraphic.end()){
		//m_DivGraphicの編集
		pData->m_number++;
		//m_Graphicの編集
		for(int i=0;i<Allnum;i++){
			std::vector<GraphicData>::iterator pGdata=GraphicSearch(pData->m_handle[i]);
			if(pGdata!=m_Graphic.end()){//ちゃんとi番目のデータが存在しているならば
				HandleBuf[i]=pData->m_handle[i];//HandleBufにハンドルを入れてあげる
				pGdata->m_number++;//使われている回数を増やす
			}else{
				HandleBuf[i]=-1;//i番目だけ読み込み失敗
				flag=-1;//全体としては失敗
				assert(1==0);
			}
		}
	}
	//ダブリ非存在時
	else{
		flag=LoadDivGraph(filename.c_str(),Allnum,XNum,YNum,XSize,YSize,HandleBuf);//分割読み込みを行う
		if(flag==0){//読み込み成功時
			//m_DivGraphicの編集
			/*int *pointer=new int [Allnum];
			for(int i=0;i<Allnum;i++){
				pointer[i]=HandleBuf[i];
			}*/
			m_DivGraphic.push_back(DivGraphicData(NULL,filename,1,Allnum));
			m_DivGraphic.back().m_handle=new int [Allnum];
			for(int i=0;i<Allnum;i++){
				m_DivGraphic.back().m_handle[i]=HandleBuf[i];
			}
			//m_Graphicの編集
			for(int i=0;i<Allnum;i++){
				m_Graphic.push_back(GraphicData(HandleBuf[i],filename,1));
			}
		}
	}
	return flag;
}

//グラフィックの複製
int GraphicControlClass::CopyGraph(int GRHandle){
	//該当するグラフィックの検索
	std::vector<GraphicData>::iterator Data=GraphicSearch(GRHandle);
	//Dataが意味のないイテレータを指している時はエラーとして処理。
	if(Data==m_Graphic.end()){
		return -1;
	}else{
		//そうでない場合、使う箇所のカウントを1増やせば良い
		Data->m_number++;
	}
	return Data->m_handle;
}

//グラフィックの削除
int GraphicControlClass::DeleteGraphEX(int GRHandle){
	int returnnum=0;//戻り値
	//m_Graphicから探す
	std::vector<GraphicData>::iterator pData=GraphicSearch(GRHandle);
	if(pData==m_Graphic.end()){//データが見つからなかった場合
		returnnum=DeleteGraph(GRHandle);//とりあえずDeleteGraphと同じ働きをするようにする
	}else{
		//データが見つかった場合
		std::string GRName=pData->m_name;//後々名前は用いるので保存
		pData->m_number--;//使われている箇所を1減らす
		if(pData->m_number <= 0){//使われている場所がなければ
			//その場所を破棄する
			returnnum=DeleteGraph(pData->m_handle);//グラフィックデータの削除
			m_Graphic.erase(pData);//std::vectorの要素の削除
			m_Graphic.shrink_to_fit();//並び替え
			//削除作業を終えた後、m_DivGraphicの編集
			//削除したデータについて、それが分割読み込みしていた時、
			//それと同じ名前のグラフィックが存在していなければm_DivGraphから取り除く
			//for(std::vector<DivGraphicData>::iterator pd=m_DivGraphic.begin();pd!=m_DivGraphic.end();pd++){
			std::vector<DivGraphicData>::iterator pd=m_DivGraphic.begin();
			while(true){
				std::vector<DivGraphicData>::iterator pdtmp=m_DivGraphic.end();
				if(pd==m_DivGraphic.end()){
					break;
				}
				bool eraseflag=false;//要素を消すかどうか
				//まず名前の一致を検索
				if(pd->m_name==GRName){
					//次にデータの消失しているかを検索
					eraseflag=true;//とりあえず消すフラグを立てておき
					for(int i=0;i<pd->m_divNUM;i++){
						if(GraphicSearch(pd->m_handle[i])!=m_Graphic.end()){//グラフィックが存在しているならば
							eraseflag=false;//条件に合わなければ消さない事にする
							break;
						}
					}
					if(eraseflag){//trueのままならば(全て消えているならば)
						//m_DivGraphicから要素を消す
						pd=m_DivGraphic.erase(pd);
						//m_DivGraphic.shrink_to_fit();
						int hhhh=0;
					}
				}
				if(!eraseflag){//m_DivGraphic.erase()をしてない時は次のイテレータを示すにはイテレータを動かさないといけない
					pd++;//whileループの終りでpdを次のイテレータに動かす
				}
			}
		}
	}
	
	return returnnum;
}

//全てのグラフィックデータの削除
int GraphicControlClass::InitGraphEX(){
	while(!m_Graphic.empty()){
		DeleteGraph(m_Graphic.begin()->m_handle);
		m_Graphic.erase(m_Graphic.begin());
	}
	m_Graphic.shrink_to_fit();
	return 0;
}

GraphicControlClass::GraphicData::GraphicData(int handle,std::string name,int number)
	:m_handle(handle),m_name(name),m_number(number){}

GraphicControlClass::GraphicData::~GraphicData(){}

GraphicControlClass::GraphicControlClass(){

}

GraphicControlClass::~GraphicControlClass(){
	//全てのグラフィックデータを解放する
	GraphicControler->InitGraphEX();
}

//------------------FontControlClassとそれを扱うための関数------------------
static FontControlClass *pFontControler=nullptr;

//クラスの関数
FontControlClass::FontData::FontData(std::string i_fontname,int i_size,int i_thick,int i_fonttype)
	:fontname(i_fontname),size(i_size),thick(i_thick),fonttype(i_fonttype){}

bool FontControlClass::FontData::operator<(const FontData &otherobj)const{
	//size
	if(size<otherobj.size){
		return true;
	} else if(size>otherobj.size){
		return false;
	}
	//thick
	if(thick<otherobj.thick){
		return true;
	} else if(thick>otherobj.thick){
		return false;
	}
	//fonttype
	if(fonttype<otherobj.fonttype){
		return true;
	} else if(fonttype>otherobj.fonttype){
		return false;
	}
	//fontname
	if(fontname<otherobj.fontname){
		return true;
	} else if(fontname>otherobj.fontname){
		return false;
	}

	return false;
}

bool FontControlClass::FontData::operator==(const FontData &otherobj)const{
	return (fontname==otherobj.fontname && size==otherobj.size && thick==otherobj.thick && fonttype==otherobj.fonttype);
}

FontControlClass::FontControlClass(){}

FontControlClass::~FontControlClass(){
	//m_fontを全て削除する
	for(const std::pair<FontData,MapValue> &data:m_font){
		DeleteFontToHandle(data.second.handle);
	}
}

int FontControlClass::CreateFontToHandleEX(std::string fontname,int size,int thick,int fonttype){
	//同一フォントが存在しているかの調査
	FontData fontdata(fontname,size,thick,fonttype);
	std::map<FontData,MapValue>::iterator it=m_font.find(fontdata);
	//フォントの作成処理
	int handle;
	if(it!=m_font.end()){
		//既にフォントが作られている場合
		it->second.count++;
		handle=it->second.handle;
	} else{
		//まだフォントが作られていない場合
		handle=CreateFontToHandle(fontname.c_str(),size,thick,fonttype);
		if(handle!=-1){
			//フォント生成が成功した場合はm_fontに情報を格納
			m_font.insert(std::pair<FontData,MapValue>(fontdata,MapValue(handle,1)));
		}
	}
	return handle;
}

int FontControlClass::DeleteFontToHandleEX(int handle){
	//handleの検索
	bool flag=false;//handleをDelete下かどうか
	int ret=-1;
	for(std::map<FontData,MapValue>::iterator it=m_font.begin(),ite=m_font.end();it!=ite;it++){
		if(it->second.handle==handle){
			//削除処理
			it->second.count--;//使用箇所が1減る
			if(it->second.count<=0){
				//使用箇所がなくなったらフォントを削除し、m_fontからデータを取り除く
				ret=DeleteFontToHandle(handle);
				m_font.erase(it);
				flag=true;
				break;
			}
		}
	}
	//handleがデータ一覧に存在していなければ、削除のみ行う
	if(!flag){
		ret=DeleteFontToHandle(handle);
	}
	return ret;

}

//関数群
void FontControler_Init(){
	pFontControler=new FontControlClass();
}

void FontControler_End(){
	if(pFontControler!=nullptr){
		delete pFontControler;
		pFontControler=nullptr;
	}
}

int CreateFontToHandleEX(std::string fontname,int size,int thick,int fonttype){
	int handle;
	if(pFontControler!=nullptr){
		handle=pFontControler->CreateFontToHandleEX(fontname,size,thick,fonttype);
	} else{
		handle=CreateFontToHandle(fontname.c_str(),size,thick,fonttype);
	}
	return handle;
}

int DeleteFontToHandleEX(int handle){
	int ret;
	if(pFontControler!=nullptr){
		ret=pFontControler->DeleteFontToHandleEX(handle);
	} else{
		ret=DeleteFontToHandle(handle);
	}
	return ret;
}

