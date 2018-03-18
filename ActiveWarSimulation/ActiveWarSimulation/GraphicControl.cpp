#include<iostream>
#include<cassert>
#include"DxLib.h"
#include"GraphicControl.h"


using namespace std;



/*GraphicControlClassを用いるための関数*/
static GraphicControlClass *GraphicControler=NULL;

void GraphicControler_Init(){
	GraphicControler=new GraphicControlClass;
	int i=0;
}

void GraphicControler_End(){
	if(GraphicControler!=NULL){
		GraphicControler->InitGraphEX();
		delete GraphicControler;
		GraphicControler=NULL;
	}
}

int LoadGraphEX(string filename){
	if(GraphicControler==NULL){
		return -1;
	}
	return GraphicControler->LoadGraphEX(filename);
}

int LoadDivGraphEX(string filename,int Allnum,int XNum,int YNum,int XSize,int YSize,int *HandleBuf){
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
vector<GraphicControlClass::GraphicData>::iterator GraphicControlClass::GraphicSearch(int GRHandle){
	//イテレータを用意
	vector<GraphicData>::iterator pGraphic=m_Graphic.begin();
	for(;pGraphic!=m_Graphic.end();pGraphic++){
		if(pGraphic->m_handle==GRHandle){
			return pGraphic;
		}
	}
	//何も示さないイテレータはEndのイテレータ。エラー時はm_Graphic.End()を返す
	return m_Graphic.end();
}

vector<GraphicControlClass::GraphicData>::iterator GraphicControlClass::GraphicSearch(string GRName){
	//イテレータを用意
	vector<GraphicData>::iterator pGraphic=m_Graphic.begin();
	for(;pGraphic!=m_Graphic.end();pGraphic++){
		if(pGraphic->m_name==GRName){
			return pGraphic;
		}
	}
	//何も示さないイテレータはEndのイテレータ。エラー時はm_Graphic.End()を返す
	return m_Graphic.end();
}

//グラフィックの読み込み
int GraphicControlClass::LoadGraphEX(string filename){
	//まず同じデータがないかを確認する。
	vector<GraphicData>::iterator pData=GraphicSearch(filename);
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
int GraphicControlClass::LoadDivGraphEX(string filename,int Allnum,int XNum,int YNum,int XSize,int YSize,int *HandleBuf){
	//分割数が0以下なら失敗
	if(Allnum<=0){
		return -1;
	}
	int flag=0;//成功かどうかのフラグ
	//名前と分割数でダブリの存在を検索
	vector<DivGraphicData>::iterator pData=m_DivGraphic.begin();
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
			vector<GraphicData>::iterator pGdata=GraphicSearch(pData->m_handle[i]);
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
	vector<GraphicData>::iterator Data=GraphicSearch(GRHandle);
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
	vector<GraphicData>::iterator pData=GraphicSearch(GRHandle);
	if(pData==m_Graphic.end()){//データが見つからなかった場合
		returnnum=DeleteGraph(GRHandle);//とりあえずDeleteGraphと同じ働きをするようにする
	}else{
		//データが見つかった場合
		string GRName=pData->m_name;//後々名前は用いるので保存
		pData->m_number--;//使われている箇所を1減らす
		if(pData->m_number <= 0){//使われている場所がなければ
			//その場所を破棄する
			returnnum=DeleteGraph(pData->m_handle);//グラフィックデータの削除
			m_Graphic.erase(pData);//vectorの要素の削除
			m_Graphic.shrink_to_fit();//並び替え
			//削除作業を終えた後、m_DivGraphicの編集
			//削除したデータについて、それが分割読み込みしていた時、
			//それと同じ名前のグラフィックが存在していなければm_DivGraphから取り除く
			//for(vector<DivGraphicData>::iterator pd=m_DivGraphic.begin();pd!=m_DivGraphic.end();pd++){
			vector<DivGraphicData>::iterator pd=m_DivGraphic.begin();
			while(true){
				vector<DivGraphicData>::iterator pdtmp=m_DivGraphic.end();
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

GraphicControlClass::GraphicData::GraphicData(int handle,string name,int number)
	:m_handle(handle),m_name(name),m_number(number){}

GraphicControlClass::GraphicData::~GraphicData(){}

GraphicControlClass::GraphicControlClass(){

}

GraphicControlClass::~GraphicControlClass(){}
