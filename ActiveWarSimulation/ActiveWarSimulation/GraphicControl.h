#ifndef DEF_GRAPHICCONTROL_H
#define DEF_GRAPHICCONTROL_H

#include<vector>
#include<string>
#include<map>

//重複したグラフィック読み込みを防止するためのグラフィック読み込み管理クラス
//分割読み込みしたものの一部を消すみたいなことをするデータはここでは管理しない
class GraphicControlClass{
	//型宣言
protected:
	//(分割読み込みしない)グラフィック型(分割読み込み時は分割されたものが入る)
	struct GraphicData{
		int m_handle;//データハンドル
		std::string m_name;//ロード元の名前
		int m_number;//このグラフィックデータが何箇所で用いられているか
		GraphicData(int handle,std::string name,int number);
		~GraphicData();
	};
	//分割読み込みするグラフィック型(いらない)
	struct DivGraphicData{
		int *m_handle;//データハンドル(動的確保)
		std::string m_name;//ロード元の名前
		int m_number;//このグラフィックデータが何箇所で用いられているか
		int m_divNUM;//分割数(データハンドル数)
		DivGraphicData(int *handle,std::string name,int number,int divNUM)
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
	std::vector<GraphicData> m_Graphic;
	std::vector<DivGraphicData> m_DivGraphic;//いらない

	//関数
protected:
	std::vector<GraphicData>::iterator GraphicSearch(int GRHandle);
	std::vector<GraphicData>::iterator GraphicSearch(std::string GRName);


public:
	GraphicControlClass();
	~GraphicControlClass();
	int LoadGraphEX(std::string filename);//(戻り値)-1:エラー発生 -1以外:グラフィックハンドル
	int LoadDivGraphEX(std::string filename,int Allnum,int XNum,int YNum,int XSize,int YSize,int *HandleBuf);//(戻り値)-1:エラー発生 0:成功
	int CopyGraph(int GRHandle);//(戻り値)-1:エラー発生 それ以外:グラフィックハンドル//グラフィックデータを複製する(内部的には使われている箇所を1増やすだけ)
	int DeleteGraphEX(int GRHandle);//(戻り値)-1:エラー発生 0:成功
	int InitGraphEX();//全てのグラフィックデータを削除する
};

void GraphicControler_Init();

void GraphicControler_End();

int LoadGraphEX(std::string filename);

int LoadDivGraphEX(std::string filename,int Allnum,int XNum,int YNum,int XSize,int YSize,int *HandleBuf);

int CopyGraph(int GRHandle);

int DeleteGraphEX(int GRHandle);

//重複したフォント読み込みを防止するためのフォント読み込み管理クラス
class FontControlClass{
	//型・列挙体
private:
	struct FontData{
		std::string fontname;
		int size;
		int thick;
		int fonttype;
		FontData(std::string i_fontname,int i_size,int i_thick,int i_fonttype);
		bool operator<(const FontData &otherobj)const;
		bool operator==(const FontData &otherobj)const;
	};
	struct MapValue{
		int handle;
		int count;
		MapValue(int i_handle,int i_count):handle(i_handle),count(i_count){}
	};
	
	//変数
private:
	std::map<FontData,MapValue> m_font;//フォントデータと個数とハンドル

	//関数
private:

public:
	FontControlClass();
	~FontControlClass();
	int CreateFontToHandleEX(std::string fontname,int size,int thick,int fonttype=-1);
	int DeleteFontToHandleEX(int handle);
};

void FontControler_Init();

void FontControler_End();

int CreateFontToHandleEX(std::string fontname,int size,int thick,int fonttype=-1);

int DeleteFontToHandleEX(int handle);

#endif
