#ifndef DEF_GRAPHICCONTROL_H
#define DEF_GRAPHICCONTROL_H

#include<vector>
#include<string>

using namespace std;

//分割読み込みしたものの一部を消すみたいなことをするデータはここでは管理しない
class GraphicControlClass{
	//型宣言
protected:
	//(分割読み込みしない)グラフィック型(分割読み込み時は分割されたものが入る)
	struct GraphicData{
		int m_handle;//データハンドル
		string m_name;//ロード元の名前
		int m_number;//このグラフィックデータが何箇所で用いられているか
		GraphicData(int handle,string name,int number);
		~GraphicData();
	};
	//分割読み込みするグラフィック型(いらない)
	struct DivGraphicData{
		int *m_handle;//データハンドル(動的確保)
		string m_name;//ロード元の名前
		int m_number;//このグラフィックデータが何箇所で用いられているか
		int m_divNUM;//分割数(データハンドル数)
		DivGraphicData(int *handle,string name,int number,int divNUM)
			:m_handle(handle),m_name(name),m_number(number),m_divNUM(divNUM)
		{
			int i=0;
		}
		~DivGraphicData(){
			if(m_handle!=NULL){
				delete [] m_handle;
				m_handle=NULL;
			}
		}
		DivGraphicData(const DivGraphicData &otherobj)
			:m_handle(NULL),m_name(otherobj.m_name),m_number(otherobj.m_number),m_divNUM(otherobj.m_divNUM)
		{
			//m_handleは動的確保した配列なので、コピーの際は新しくメモリを確保してあげる必要がある
			if(otherobj.m_handle!=NULL){//NULLでなければコピーするものがある
				m_handle=new int [m_divNUM];
				for(int i=0;i<m_divNUM;i++){
					m_handle[i]=otherobj.m_handle[i];
				}
			}
		}
		DivGraphicData operator=(const DivGraphicData &otherobj){//代入演算子=のオーバーロード
			return DivGraphicData(otherobj);
		}
	};
	
	//変数
protected:
	vector<GraphicData> m_Graphic;
	vector<DivGraphicData> m_DivGraphic;//いらない

	//関数
protected:
	vector<GraphicData>::iterator GraphicSearch(int GRHandle);
	vector<GraphicData>::iterator GraphicSearch(string GRName);


public:
	GraphicControlClass();
	~GraphicControlClass();
	int LoadGraphEX(string filename);//(戻り値)-1:エラー発生 -1以外:グラフィックハンドル
	int LoadDivGraphEX(string filename,int Allnum,int XNum,int YNum,int XSize,int YSize,int *HandleBuf);//(戻り値)-1:エラー発生 0:成功
	int CopyGraph(int GRHandle);//(戻り値)-1:エラー発生 それ以外:グラフィックハンドル//グラフィックデータを複製する(内部的には使われている箇所を1増やすだけ)
	int DeleteGraphEX(int GRHandle);//(戻り値)-1:エラー発生 0:成功
	int InitGraphEX();//全てのグラフィックデータを削除する
};

void GraphicControler_Init();

void GraphicControler_End();

int LoadGraphEX(string filename);

int LoadDivGraphEX(string filename,int Allnum,int XNum,int YNum,int XSize,int YSize,int *HandleBuf);

int CopyGraph(int GRHandle);

int DeleteGraphEX(int GRHandle);

#endif