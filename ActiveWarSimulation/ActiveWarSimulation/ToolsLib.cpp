#include<vector>
#include"ToolsLib.h"
#include"DxLib.h"
#include<fstream>
#include<iostream>
#include<algorithm>
#include<time.h>
#include<Windows.h>
#include"FileRead.h"
#include"CommonConstParameter.h"
#pragma comment(lib, "winmm.lib")

#define INTMAXINDEX 12//int型の最大桁数+2('-'と'\0'のための+2)(int型の文字数)


//Vector2Dについての関数
float Vector2D::GetRadian()const{
	return std::atan2f(y,x);
}

Vector2D Vector2D::turn(float radian)const{
	float cos=std::cosf(radian),sin=std::sinf(radian);
	return Vector2D(x*cos-y*sin,x*sin+y*cos);	
}

Vector2D GetMousePointVector2D(){
	int x,y;
	GetMousePoint(&x,&y);
	return Vector2D((float)x,(float)y);
}

//FpeMeasuringについての関数
FpsMeasuring::FpsMeasuring()
	:m_frame(0),m_recordTime(0){}

FpsMeasuring::~FpsMeasuring(){}

void FpsMeasuring::Update(){
	m_frame++;
	m_time[m_frame%fpsSize]=timeGetTime();
}

double FpsMeasuring::GetFps()const{
	double fps;
	double timeUpdateFpsSize=(double)(m_time[m_frame%fpsSize]-m_time[(m_frame+1)%fpsSize]);//fpsSize回の更新にどのくらいの時間がかかったか。単位はミリ秒
	if(timeUpdateFpsSize!=0.0){
		fps=(fpsSize-1)/timeUpdateFpsSize*1000;//配列の間隔の個数が更新回数なので、fpsSizeから-1する。
	} else{
		fps=0.0;
	}
	return fps;
}

void FpsMeasuring::RecordTime(){
	m_recordFrame=m_frame;
	m_recordTime=timeGetTime();
}

size_t FpsMeasuring::GetProcessedFrame()const{
	return m_frame-m_recordFrame;
}

double FpsMeasuring::GetProcessedTime()const{
	return (double)(timeGetTime()-m_recordTime)/1000;
}

//描画関連
//画面全体を描画範囲にする
int SetDrawAllArea(){
	int dx,dy;
	GetScreenState(&dx,&dy,0);

	return SetDrawArea(0,0,dx,dy);
}

//文字列描画。\nで改行させる。また右端まで行ったら改行する。
//最後が\0で終わらない文字列だとフリーズまたはオーバーフローが起こる
int DrawStringNewLineToHandle(const int strX,const int strY,const int printableX,const int printableY,const int maxDX,const int maxDY,const int Color,const int Font,const int FontSize,const char *str){
	//描画可能範囲を設定
	SetDrawArea(printableX-1,printableY-1,printableX+maxDX+1,printableY+maxDY+1);//細かい部分で文字が切れることがあるので余裕を持たせる
																				 //文字列の長さを格納
	int strlength=strlen(str)+1;
	//それぞれの文字がどれだけ描画幅を使用するかを計算する
	int *indexlength=new int[strlength];
	for(int i=0;i<strlength;){
		//iは現在調べている文字の位置を示している
		char *partstr;
		if(str[i]>=0 || i+1>=strlength){//i文字目が1バイト文字なら
			partstr=new char[2];
			partstr[0]=str[i];
			partstr[1]='\0';//これがないと文字化けした文字列の長さを求めてしまう
			indexlength[i]=GetDrawStringWidthToHandle(partstr,2,Font);
			delete[] partstr;
			i+=1;
		} else{//2バイト文字なら
			partstr=new char[3];
			partstr[0]=str[i];
			partstr[1]=str[i+1];
			partstr[2]='\0';
			indexlength[i]=0;//2バイト文字の場合は2バイト目にのみ長さを格納する。
			indexlength[i+1]=GetDrawStringWidthToHandle(partstr,2,Font);
			delete[] partstr;
			i+=2;
		}
	}
	int linenum=0;//行数(0行目、1行目・・・)
	int totalstrlen=0;//i文字目までの描画上の長さの合計
	int totalstrdrawlen=0;//今までに描画したの文字列の描画上の長さの合計
	int nowstartindex=0;//今の行の描画の最初の文字の配列番号
	for(int i=0;i<strlength;i++){
		totalstrlen+=indexlength[i];
		//iまでの文字列を見る
		if(totalstrlen>maxDX+totalstrdrawlen || str[i]=='\n'){
			//文字列(iまで)が横にオーバーするまたはここで改行するなら
			//この行の文字列(endnumまで)を生成
			totalstrdrawlen=totalstrlen-indexlength[i];//描画はするのであらかじめ更新
			int endnum=i;//この行の文字列の最後の文字の直後の配列番号
			if(endnum>0 && str[endnum-1]<0 && indexlength[endnum-1]==0){//直前の配列番号の中身を見て、2バイト文字と判断したら
				endnum--;//最後の文字をひとつ前に持っていく
				totalstrlen-=indexlength[endnum];//変えた部分の長さも引いておく
			}
			char *STR=new char[endnum-nowstartindex+1];//末尾に終端文字を入れるために要素を1多くする
			for(int s=0;s<endnum-nowstartindex;s++){
				STR[s]=str[s+nowstartindex];
			}
			STR[endnum-nowstartindex]='\0';
			//文字列の描画
			if(strY+FontSize*(linenum+1)>printableY && strY+FontSize*linenum<printableY+maxDY){//範囲外なら描画しない
				DrawStringToHandle(strX,strY+FontSize*linenum,STR,Color,Font);
			}
			//文字列情報の更新
			linenum++;
			nowstartindex=endnum;
			if(str[endnum]=='\n'){
				//改行文字をみて先に進んだ場合
				nowstartindex++;//次の行は改行文字からスタートしない
				totalstrdrawlen+=indexlength[endnum];//改行文字分の長さは追加されてないので追加する
			}
			//if文終了処理
			delete[] STR;
			STR=NULL;
			continue;
		}
		if(i>=strlength-1){//配列が最後まで達したら
						   //残りの文字列の描画
			const char *pstr=&str[nowstartindex];
			if(strY+FontSize*(linenum+1)>printableY && strY+FontSize*linenum<printableY+maxDY){//範囲外なら描画しない
				DrawStringToHandle(strX,strY+FontSize*linenum,pstr,Color,Font);
			}
			//ループを抜ける支持

		}
	}
	SetDrawAllArea();//描画範囲設定の解除

					 //終了処理
	delete[] indexlength;
	//return 0;
	return (linenum+1)*FontSize;
}

int DrawStringNewLineToHandle(const int X,const int Y,const int printableX,const int printableY,const int maxDX,const int maxDY,const int Color,const int Font,const int FontSize,const std::string &str){
	//描画
	int returnnum=DrawStringNewLineToHandle(X,Y,printableX,printableY,maxDX,maxDY,Color,Font,FontSize,str.c_str());
	
	return returnnum;
}

int DrawStringNewLineToHandle(const int strX,const int strY,const int maxDX,const int maxDY,const char *str,const int Color,const int Font,int spaceBetweenLines){
	//フォントの大きさを計算
	const int fontsize=GetFontSizeToHandle(Font);
	//描画
	return DrawStringNewLineToHandle(strX,strY,strX,strY,maxDX,maxDY,Color,Font,fontsize+spaceBetweenLines,str);
}

//上の文字列描画の方式で、描画はせずに必要なY座標の幅のみ求める
int GetStringHeightNewLineToHandle(const int maxDX,const int font,const char *str){
	//変数初期化
	int strlength=strlen(str)+1;//strの文字列数を格納
	int *indexlength=new int[strlength];//strの各文字のX幅を求める
	for(int i=0;i<strlength;){
		//各strを調べて、文字列の長さを格納する
		if(str[i]<0 && i+1<strlength){//str[i]が2バイト文字でstr[i+1]が配列外参照でなければ
			char *ch=new char[3];
			ch[0]=str[i];
			ch[1]=str[i+1];
			ch[2]='\0';
			indexlength[i]=GetDrawStringWidthToHandle(ch,2,font);
			indexlength[i+1]=0;
			delete[] ch;
			i+=2;
		} else{//1バイト文字の場合
			char *ch=new char[2];
			ch[0]=str[i];
			ch[1]='\0';
			indexlength[i]=GetDrawStringWidthToHandle(ch,1,font);
			delete[] ch;
			i++;
		}
	}
	int fontsize=GetFontSizeToHandle(font);//フォントの大きさを格納
	int nowlinenum=1;//現在存在している行数
	int nowabovelineprintwidth=0;//現在の行より上に描画している文字列のX幅
	int nowprintwidth=0;//現在描画している文字列のX幅
						//行数計算
	for(int i=0;i<strlength;i++){
		if(nowprintwidth-nowabovelineprintwidth+indexlength[i]<=maxDX && str[i]!='\n'){
			//次の文字列を描画しても外にはみ出さない場合、更に改行文字でない場合
			nowprintwidth+=indexlength[i];//書く文字を追加
		} else{
			//はみ出す場合、または改行文字を発見した場合
			nowabovelineprintwidth=nowprintwidth;//現在までの文字列の幅を記録
			nowlinenum++;//行数のカウントを１つ増やす
			nowprintwidth+=indexlength[i];//書く文字を追加
		}
	}
	//終了処理
	delete[] indexlength;
	return nowlinenum*fontsize;
}

int GetStringHeightNewLineToHandle(const int maxDX,const int font,const std::string str){
	//string型を加工
	char *c_str=new char[str.size()];
	//strcpy(c_str,str.c_str());
	for(unsigned int i=0;i<str.size();i++){
		c_str[i]=str[i];
	}
	//計算
	int returnnum=GetStringHeightNewLineToHandle(maxDX,font,c_str);
	//終了処理
	delete[] c_str;
	c_str=NULL;

	return returnnum;
}

//拡大描画。位置指定ではなく大きさ指定で拡大率を指定。
int DrawExtendGraphSizeAssign(int x,int y,int dx,int dy,int GrHandle,int TransFlag){
	return DrawExtendGraph(x,y,x+dx,y+dy,GrHandle,TransFlag);
}

//中央揃えの文字列描画
int DrawStringCenterBaseToHandle(const int centerx,const int centery,const char *str,unsigned int color,int fonthandle,bool yposcenterbaseflag,unsigned int EdgeColor,int VerticalFlag){
	if(yposcenterbaseflag){
		return DrawStringToHandle(centerx-GetDrawStringWidthToHandle(str,strlen(str),fonthandle)/2,centery-GetFontSizeToHandle(fonthandle)/2
			,str,color,fonthandle,EdgeColor,VerticalFlag);
	} else{
		return DrawStringToHandle(centerx-GetDrawStringWidthToHandle(str,strlen(str),fonthandle)/2,centery,str,color,fonthandle,EdgeColor,VerticalFlag);
	}
}

//右揃えの文字列描画
int DrawStringRightJustifiedToHandle(int x,int y,const std::string &str,int color,int handle,unsigned int edgeColor,int verticalFlag){
	const char *const pc=str.c_str();
	int dx=GetDrawStringWidthToHandle(pc,str.size(),handle,verticalFlag);
	return DrawStringToHandle(x-dx,y,pc,color,handle,edgeColor,verticalFlag);
}

//int→string変換の際に、0詰めを行うようにする
std::string to_string_0d(int pal,unsigned int length){
	std::string str;
	str+=std::to_string(pal);
	str.reserve(length);
	for(unsigned int i=str.size();i<length;i++){
		str="0"+str;
	}
	return str;
}

//ファイルが存在するかどうかを調べる。実行ファイルの存在するディレクトリ上に存在するかを調べる。
bool JudgeFileExist(const std::string &str){
	//「ファイルが開ける」→「ファイルが存在する」という考え方。セキュリティ面の問題がある。
	std::ifstream ifs(str);
	bool flag=ifs.is_open();
	ifs.close();
	return flag;
}

//現在の背景色のカラーコードを取得する
unsigned int GetBackgroundColor(){
	int r,g,b;
	GetBackgroundColor(&r,&g,&b);
	return GetColor(r,g,b);
}

//反転色のカラーコードを取得する
unsigned int GetInvertedColor(unsigned int color){
	int r,g,b;
	GetColor2(color,&r,&g,&b);
	return GetColor(255-r,255-g,255-b);
}

//pointがp1,p2,p3による三角形の内部にあるかを判定
bool JudgeInTriangle(Vector2D point,Vector2D p1,Vector2D p2,Vector2D p3){
	//pointがp1,p2,p3による三角形の内部にあるかを判定
	Vector2D p[3]={p1,p2,p3};//処理しやすいように配列化
	//3辺のベクトルと3点からpointへのベクトルを計算
	Vector2D v[3];//3辺のベクトル
	Vector2D toPos[3];//3点からpointへのベクトル
	for(int i=0;i<3;i++){
		v[i]=p[(i+1)%3]-p[i];
		toPos[i]=point-p[i];
	}
	//i=0,1,2について、v[i]*toPos[i]の外積の正負が全て同じであれば三角形内部にあると判定できる。
	bool direction[3];
	for(int i=0;i<3;i++){
		direction[i]=(v[i].cross(toPos[i])>=0.0f);
	}
	return (direction[0] & direction[1] & direction[2])|(!direction[0] & !direction[1] & !direction[2]);
}

//内部で使用している描画解像度を取得する(ウインドウの拡大縮小で左右されないサイズが手に入る)
std::pair<int,int> GetWindowResolution(){
	//これだとフルスクリーンモードでは対応できないので、画面解像度の情報を保持するしかない
/*
	int dx,dy;
	double rateX,rateY;
	GetWindowSize(&dx,&dy);
	GetWindowSizeExtendRate(&rateX,&rateY);
	if(rateX==0.0 || rateY==0.0){
		return std::pair<int,int>(0,0);
	} else{
		return std::pair<int,int>((int)(std::ceil(dx/rateX)),(int)(std::ceil(dy/rateY)));
	}
//*/
	return std::pair<int,int>(CommonConstParameter::gameResolutionX,CommonConstParameter::gameResolutionY);
}

//マウスがウインドウ内に入っているかの判定
bool JudgeMouseInWindow(){
	std::pair<int,int> resolution=GetWindowResolution();
	int x,y;
	GetMousePoint(&x,&y);
	//ウインドウの大きさが分かれば、長方形と点の内部判定
	return (x>=0 && x<resolution.first && y>=0 && y<resolution.second);
}

//bgmを読み込む関数
int LoadBGMMem(const std::string &dirname,int BufferNum,int UnionHandle){
	//音を読み込む。
	int handle=LoadSoundMem((dirname+"bgm.ogg").c_str(),BufferNum,UnionHandle);
	//ループ位置の設定。info.csvに「ループ開始位置」「ループ終了位置」「音量%」が格納されている
	std::vector<std::vector<int>> info=CSVRead((dirname+"info.csv").c_str());
	if(!info.empty() && info[0].size()>=3){
		const int loopstart=info[0][0];
		const int loopend=info[0][1];
		const int volume=info[0][2];
		if(loopstart>=0){
			SetLoopStartSamplePosSoundMem(loopstart,handle);
		}
		if(loopend>=0){
			SetLoopSamplePosSoundMem(loopend,handle);
		}
		ChangeVolumeSoundMem(volume,handle);
	}
	return handle;
}

//グラデーション有りの四角形描画(x1<x2,y1<y2でないと正しく動作しない)
void DrawBoxGradation(int x1,int y1,int x2,int y2,unsigned int leftUpColor,unsigned int rightDownColor,bool horizontal){
	//水平方向または垂直方向のみのグラデーション
	if(horizontal){
		//水平方向
		DrawBoxGradation(x1,y1,x2,y2,leftUpColor,leftUpColor,rightDownColor,rightDownColor);
	} else{
		//垂直方向
		DrawBoxGradation(x1,y1,x2,y2,leftUpColor,rightDownColor,leftUpColor,rightDownColor);
	}
}

void DrawBoxGradation(int x1,int y1,int x2,int y2,unsigned int leftUpColor,unsigned int leftDownColor,unsigned int rightUpColor,unsigned int rightDownColor){
	//頂点色の取得
	int vertexColor[4][3];//0:(x1,y1) 1:(x1,y2) 2:(x2,y1) 3:(x2,y2)
	GetColor2(leftUpColor,&vertexColor[0][0],&vertexColor[0][1],&vertexColor[0][2]);
	GetColor2(leftDownColor,&vertexColor[1][0],&vertexColor[1][1],&vertexColor[1][2]);
	GetColor2(rightUpColor,&vertexColor[2][0],&vertexColor[2][1],&vertexColor[2][2]);
	GetColor2(rightDownColor,&vertexColor[3][0],&vertexColor[3][1],&vertexColor[3][2]);
	//四角形の描画
	if(x1<x2 && y1<y2){
		for(int y=y1;y<y2;y++){
			for(int x=x1;x<x2;x++){
				//(x,y)に描画する色は
				/*color=
					vertexColor[0]*(x2-x)/(x2-x1)*(y2-y)/(y2-y1)
					+vertexColor[1]*(x2-x)/(x2-x1)*(y-y1)/(y2-y1)
					+vertexColor[2]*(x-x1)/(x2-x1)*(y2-y)/(y2-y1)
					+vertexColor[3]*(x-x1)/(x2-x1)*(y-y1)/(y2-y1)
				//*/
				int colorValue[3];
				for(size_t i=0;i<3;i++){
					colorValue[i]=((vertexColor[0][i]*(x2-x)*(y2-y))+(vertexColor[1][i]*(x2-x)*(y-y1))+(vertexColor[2][i]*(x-x1)*(y2-y))+(vertexColor[3][i]*(x-x1)*(y-y1)))/(y2-y1)/(x2-x1);
				}
				DrawPixel(x,y,GetColor(colorValue[0],colorValue[1],colorValue[2]));
			}
		}
	} else{
		//0除算や方向でミスっている場合は普通のDrawBoxをする
		DrawBox(x1,y1,x2,y2,leftUpColor,TRUE);
	}
}

//数値変化を様々な式で管理するクラス
//---Easing---
Easing::Easing(int i_x,int i_endx,int i_maxframe,TYPE i_type,FUNCTION i_function,double i_degree)
	:frame(0),maxframe(i_maxframe),startx(i_x),endx(i_endx),type(i_type),function(i_function),degree(i_degree){}

int Easing::GetX()const{
	if(!GetEndFlag()){
		//動作が終わっていない場合は、割合を計算する
		return startx+(int)((endx-startx)*GetRate());
	} else{
		//動作が終わっている場合は、endxと分かりきっている
		return endx;
	}
}

double Easing::GetRate()const{
	double ft;//返す値
	const double fullRate=(double)frame/maxframe;
	if(!GetEndFlag()){
		if(maxframe>0){
			switch(function){
			case(FUNCTION_LINER):
				ft=1.0*fullRate;
				break;
			case(FUNCTION_QUAD):
				if(type==TYPE_IN){
					ft=1.0*fullRate*fullRate;
				} else if(type==TYPE_OUT){
					ft=1.0*fullRate*(2.0-fullRate);
				} else{
					if(fullRate<0.5){
						ft=2.0*fullRate*fullRate;
					} else{
						ft=2.0*fullRate*(2.0-fullRate)-1.0;
					}
				}
				break;
			case(FUNCTION_EXPO):
				if(type==TYPE_IN){
					ft=pow(2.0,degree*(1.0*fullRate-1.0));
				} else if(type==TYPE_OUT){
					ft=1.0-pow(2.0,-degree*fullRate);
				} else if(type==TYPE_INOUT){
					if(fullRate<0.5){
						ft=pow(2.0,degree*(fullRate*2.0-1.0))/2.0;
					} else{
						ft=1.0-pow(2.0,-degree*(fullRate*2.0-1.0))/2.0;
					}
				}
				break;
			case(FUNCTION_BACK):
				if(degree!=0.5){
					//0.5になると0除算が起きるので別処理をする
					if(type==TYPE_IN){
						ft=1.0/(1.0-degree*2.0)*fullRate*(fullRate-degree*2.0);
					} else if(type==TYPE_OUT){
						const double deg=1.0-degree;
						ft=1.0/(1.0-deg*2.0)*fullRate*(fullRate-deg*2.0);
					} else{
						if(fullRate<0.5){
							ft=2.0/(1.0-degree*2.0)*fullRate*(fullRate-degree);
						} else{
							const double deg=1.0-degree;
							ft=2.0/(1.0-degree*2.0)*(fullRate-0.5)*(fullRate-0.5-degree)+0.5;
						}
					}
				} else{
					//linerにする
					ft=1.0*fullRate;
				}
				break;
			}
		} else{
			ft=1.0;
		}
	} else{
		ft=1.0;
	}
	return ft;
}

void Easing::SetTarget(int i_endx,bool initframe){
	startx=GetX();
	if(initframe){
		//initframeがtrueの時のみframeを0に
		frame=0;
	}
	endx=i_endx;
}

void Easing::Update(){
	frame++;
}

void Easing::EnforceEnd(){
	frame=maxframe;
	Update();
	frame=maxframe;//frameがmaxframeで止まるようにする。
}

void Easing::Retry(){
	frame=0;
}

void Easing::Retry(int i_startx){
	startx=i_startx;
	Retry();
}

void Easing::SetMaxFrame(int frame,bool targetinitflag){
	maxframe=frame;
	if(targetinitflag){
		Retry(GetX());
	}else{
		frame=min(frame,maxframe);
	}
}

bool Easing::GetEndFlag()const{
	return (frame>=maxframe);
}

//位置を色々な式で管理するクラス
//---PositionControl---
void PositionControl::SetTarget(int i_endx,int i_endy,bool initframe){
	x.SetTarget(i_endx,initframe);
	y.SetTarget(i_endy,initframe);
}

void PositionControl::Update(){
	x.Update();
	y.Update();
}

void PositionControl::EnforceEnd(){
	x.EnforceEnd();
	y.EnforceEnd();
}

void PositionControl::Retry(){
	x.Retry();
	y.Retry();
}

void PositionControl::Retry(int i_startx,int i_starty){
	x.Retry(i_startx);
	y.Retry(i_starty);
}

void PositionControl::SetMaxFrame(int frame,bool targetinitflag){
	x.SetMaxFrame(frame,targetinitflag);
	y.SetMaxFrame(frame,targetinitflag);
}

bool PositionControl::GetEndFlag()const{
	//xもyも同じフレーム管理なので、yのGetEndFlagもxのGetEndFlagも同じ
	return x.GetEndFlag();
}

//位置を複数の式で管理するクラス
//---PositionComplexControl---
PositionComplexControl::PositionComplexControl(const std::vector<PositionControl> &controlgroup)
	:indexX(0),indexY(0)
{
	//サイズ確保
	const size_t size=controlgroup.size();
	x.reserve(size);
	y.reserve(size);
	//形成
	for(const PositionControl &control:controlgroup){
		x.push_back(control.GetEasingX());
		y.push_back(control.GetEasingY());
	}

}

void PositionComplexControl::Update(){
	x.at(indexX).Update();
	y.at(indexY).Update();
	//indexの更新
	if(x.at(indexX).GetEndFlag() && indexX+1<x.size()){
		indexX++;
	}
	if(y.at(indexY).GetEndFlag() && indexY+1<y.size()){
		indexY++;
	}
}

void PositionComplexControl::EnforceEnd(){
	for(size_t size=x.size();;indexX++){
		x.at(indexX).EnforceEnd();
		if(indexX+1>=size){
			break;
		}
	}
	for(size_t size=y.size();;indexY++){
		y.at(indexY).EnforceEnd();
		if(indexY+1>=size){
			break;
		}
	}
}

void PositionComplexControl::Retry(){
	indexX=0;
	indexY=0;
	for(Easing &e:x){
		e.Retry();
	}
	for(Easing &e:y){
		e.Retry();
	}

}

void PositionComplexControl::Retry(int i_startx,int i_starty){
	indexX=0;
	indexY=0;
	for(Easing &e:x){
		e.Retry();
	}
	for(Easing &e:y){
		e.Retry();
	}
	x.at(indexX).Retry(i_startx);
	y.at(indexY).Retry(i_starty);
}

bool PositionComplexControl::GetEndFlag()const{
	//xもyも同じフレーム管理なので、yのGetEndFlagもxのGetEndFlagも同じ
	return x.back().GetEndFlag() && y.back().GetEndFlag();
}

int PositionComplexControl::GetMaxFrame()const{
	int maxframeX=0,maxframeY=0;
	for(const Easing &easing:x){
		maxframeX+=easing.GetMaxFrame();
	}
	for(const Easing &easing:y){
		maxframeY+=easing.GetMaxFrame();
	}
	return (std::max)(maxframeX,maxframeY);
}

//大きさ調整しつつ並べて表示する位置を計算するクラス
LiningupScalingMechanism::LiningupScalingMechanism(int x,int y,DIRECTION side,PositionControl initsize)
	:fixedside(side),startx(x),starty(y),size(initsize){}

LiningupScalingMechanism::~LiningupScalingMechanism(){}

void LiningupScalingMechanism::Update(){
	size.Update();
}

void LiningupScalingMechanism::SetScaling(int startdx,int startdy,int enddx,int enddy){
	size=PositionControl(startdx,startdy,size.GetMaxFrame(),size.GetType()
		,size.GetFunction(),size.GetDegree());
	size.SetTarget(enddx,enddy,true);
}

void LiningupScalingMechanism::EnforceEnd(){
	size.EnforceEnd();
}

void LiningupScalingMechanism::Retry(){
	size.Retry();
}

int LiningupScalingMechanism::GetX(int n,int expandingn,int reducingn)const{
	int x=startx;
	switch(fixedside){
	case(UP):
		//上辺を揃えて横並びなので、n番目まで全て足し合わせる
	case(UNDER):
		//下辺を揃え横並びなので、n番目まで足し合わせる
		for(int i=0;i<n;i++){
			if(i==expandingn){
				x+=GetExpandingSizeX();
			} else if(i==reducingn){
				x+=GetReducingSizeX();
			} else{
				x+=GetNormalSizeX();
			}
		}
		break;
	case(LEFT):
		//左辺を揃えて縦並びなので、そのまま返す
		break;
	case(RIGHT):
		//右辺を揃えて縦並びなので、n番目の横幅を引いて返す
		if(n==expandingn){
			x-=GetExpandingSizeX();
		} else if(n==reducingn){
			x-=GetReducingSizeX();
		} else{
			x-=GetNormalSizeX();
		}
		break;
	default:
		break;
	}
	return x;
}

int LiningupScalingMechanism::GetY(int n,int expandingn,int reducingn)const{
	int y=starty;
	switch(fixedside){
	case(LEFT):
		//左辺を揃えて横並びなので、n番目まで全て足し合わせる
	case(RIGHT):
		//右辺を揃え横並びなので、n番目まで足し合わせる
		for(int i=0;i<n;i++){
			if(i==expandingn){
				y+=GetExpandingSizeY();
			} else if(i==reducingn){
				y+=GetReducingSizeY();
			} else{
				y+=GetNormalSizeY();
			}
		}
		break;
	case(UP):
		//上辺を揃えて縦並びなので、そのまま返す
		break;
	case(UNDER):
		//下辺を揃えて縦並びなので、n番目の横幅を引いて返す
		if(n==expandingn){
			y-=GetExpandingSizeY();
		} else if(n==reducingn){
			y-=GetReducingSizeY();
		} else{
			y-=GetNormalSizeY();
		}
		break;
	default:
		break;
	}
	return y;
}

int LiningupScalingMechanism::GetNormalSizeX()const{
	return size.GetstartX();
}

int LiningupScalingMechanism::GetNormalSizeY()const{
	return size.GetstartY();
}

int LiningupScalingMechanism::GetExpandingSizeX()const{
	return size.GetX();
}

int LiningupScalingMechanism::GetExpandingSizeY()const{
	return size.GetY();
}

int LiningupScalingMechanism::GetReducingSizeX()const{
	return size.GetendX()+size.GetstartX()-size.GetX();
}

int LiningupScalingMechanism::GetReducingSizeY()const{
	return size.GetendY()+size.GetstartY()-size.GetY();
}

//フレーム数を数えるためのクラスTimer
Timer::Timer(int i_fps)
	:fps(max(1,i_fps)),counter(0),startTimer(0),endTimer(0){}

Timer::~Timer(){}

int Timer::GetProcessCounter(bool secondFlag)const{
	if(secondFlag){
		//秒単位で返す
		return (counter-startTimer)/fps;
	}else{
		//frame単位で返す
		return counter-startTimer;
	}
}

int Timer::GetLeftCounter(bool secondFlag)const{
	if(secondFlag){
		//秒単位で返す
		return (endTimer-startTimer)/fps-GetProcessCounter(true);
	}else{
		//frame単位で返す
		return endTimer-counter;
	}
}

bool Timer::JudgeEnd()const{
	return counter>=endTimer;
}

bool Timer::SetTimer(int timeLength,bool secondFlag){
	startTimer=counter;
	if(secondFlag){
		//秒単位で設定
		endTimer=startTimer+timeLength*fps;
	} else{
		//frame単位で設定
		endTimer=startTimer+timeLength;
	}
	return true;//失敗しないのでtrueを返す。
}

void Timer::Update(){
	counter++;
}

void Timer::EnforceEnd(){
	counter=endTimer;
}
