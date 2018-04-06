#include<vector>
#include"ToolsLib.h"
#include"DxLib.h"
#include<fstream>
#include<iostream>
#include<algorithm>
#include<time.h>
#include<Windows.h>
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
	:m_flame(0),m_recordTime(0){}

FpsMeasuring::~FpsMeasuring(){}

void FpsMeasuring::Update(){
	m_flame++;
	m_time[m_flame%fpsSize]=timeGetTime();
}

double FpsMeasuring::GetFps()const{
	double fps;
	double timeUpdateFpsSize=(double)(m_time[m_flame%fpsSize]-m_time[(m_flame+1)%fpsSize]);//fpsSize回の更新にどのくらいの時間がかかったか。単位はミリ秒
	if(timeUpdateFpsSize!=0.0){
		fps=fpsSize/timeUpdateFpsSize*1000;
	} else{
		fps=0.0;
	}
	return fps;
}

void FpsMeasuring::RecordTime(){
	m_recordTime=timeGetTime();
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

//数値変化を様々な式で管理するクラス
//---Easing---
Easing::Easing(int i_x,int i_endx,int i_maxflame,TYPE i_type,FUNCTION i_function,double i_degree)
	:flame(0),maxflame(i_maxflame),x(i_x),startx(i_x),endx(i_endx),type(i_type),function(i_function),degree(i_degree){}

void Easing::SetTarget(int i_endx,bool initflame){
	startx=x;
	if(initflame){
		//initflameがtrueの時のみflameを0に
		flame=0;
	}
	endx=i_endx;
}

void Easing::Update(){
	double ft;//増加割合
	const double fullRate=flame/maxflame;
	if(!GetEndFlag()){
		if(maxflame>0){
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
			}
		} else{
			ft=1.0;
		}
		x=startx+(int)((endx-startx)*ft);
	} else{
		x=endx;
	}
	flame++;
}

void Easing::EnforceEnd(){
	flame=maxflame;
	Update();
	flame=maxflame;//flameがmaxflameで止まるようにする。
}

void Easing::Retry(){
	x=startx;
	flame=0;
}

void Easing::Retry(int i_startx){
	startx=i_startx;
	Retry();
}

void Easing::SetMaxFlame(int flame,bool targetinitflag){
	maxflame=flame;
	if(targetinitflag){
		Retry(x);
	}else{
		flame=min(flame,maxflame);
	}
}

bool Easing::GetEndFlag()const{
	return (flame>=maxflame);
}

//位置を色々な式で管理するクラス
//---PositionControl---
void PositionControl::SetTarget(int i_endx,int i_endy,bool initflame){
	x.SetTarget(i_endx,initflame);
	y.SetTarget(i_endy,initflame);
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

void PositionControl::SetMaxFlame(int flame,bool targetinitflag){
	x.SetMaxFlame(flame,targetinitflag);
	y.SetMaxFlame(flame,targetinitflag);
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

int PositionComplexControl::GetMaxFlame()const{
	int maxflameX=0,maxflameY=0;
	for(const Easing &easing:x){
		maxflameX+=easing.GetMaxFlame();
	}
	for(const Easing &easing:y){
		maxflameY+=easing.GetMaxFlame();
	}
	return (std::max)(maxflameX,maxflameY);
}

//大きさ調整しつつ並べて表示する位置を計算するクラス
LiningupScalingMechanism::LiningupScalingMechanism(int x,int y,DIRECTION side,PositionControl initsize)
	:fixedside(side),startx(x),starty(y),size(initsize){}

LiningupScalingMechanism::~LiningupScalingMechanism(){}

void LiningupScalingMechanism::Update(){
	size.Update();
}

void LiningupScalingMechanism::SetScaling(int startdx,int startdy,int enddx,int enddy){
	size=PositionControl(startdx,startdy,size.GetMaxFlame(),size.GetType()
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
		//flame単位で返す
		return counter-startTimer;
	}
}

int Timer::GetLeftCounter(bool secondFlag)const{
	if(secondFlag){
		//秒単位で返す
		return (endTimer-startTimer)/fps-GetProcessCounter(true);
	}else{
		//flame単位で返す
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
		//flame単位で設定
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

//文字列の分割・結合を行うクラス
StringBuilder::StringBuilder(const std::string &str,char spliter,char beginer,char ender,bool deepen,bool setSplit)
	:m_spliter(spliter),m_beginer(beginer),m_ender(ender)
{
	//文字列として初期化すると決まっているのならそのようにして終了する
	if(!setSplit){
		m_str=str;
		m_splitFlag=false;
	} else{
		//分割読み込み処理を行う場合
		m_splitFlag=true;//この処理で読み込まれる場合は必ず区切られた複数文字列として読み込む
		//下準備
		std::string splitStr;//分割された文字列
		int tokenCount=0;//m_beginerを読み込んだ個数-m_enderを読み込んだ個数。負になる減少は無視する。
		bool setExist=false;//既に集合を読んだかどうか。区切り文字を読み取るたびにfalseにする。「aa(cc,(dd,ee))bb(ff)」のaa,bb,ffのようなバグデータを無視するために用いる。
		//新たなStringSpliterを生成する際の処理
		const auto pushFunc=[&]()->void{
			//deepenがfalseであるか、splitStrに集合が含まれていないならこれ以上深くならない
			//最終引数がtrueである限りこの処理は再帰的に呼び出されるが、いずれ(next!=ite)がfalseになる。（処理のたびに集合外のm_spliterと最外のm_beginer,m_enderがstrから消滅するため）
			m_vec.push_back(StringBuilder(splitStr,m_spliter,m_beginer,m_ender,true,deepen && setExist));
			splitStr.clear();
			setExist=false;
		};
		//strの先頭から末尾まで読み込み
		for(std::string::const_iterator it=str.begin(),ite=str.end();it!=ite;it++){
			if(*it==m_spliter && tokenCount<=0 && !splitStr.empty()){
				//区切り文字にたどり着いたらsplitStrを用いて新たなStringSpliterを作成する
				pushFunc();
			} else if(*it==m_beginer){
				//集合の先頭文字を見つけた場合は、tokenCountを1増やす
				if(!setExist){
					//splitStrを弄るのはまだ集合を読み込んでいない時のみ。
					if(tokenCount>0){
						//既に集合内である場合は、splitStrに文字を追加
						splitStr.push_back(m_beginer);
					} else{
						//集合外であるならここから集合が始まるので、splitStrに文字を追加しない。また、「aa(cc,(dd,ee))bb(ff)」のaa,bb,ffのようなバグデータを無視するためにaa部分は削除する。
						splitStr.clear();
					}
				}
				tokenCount++;
			} else if(*it==m_ender){
				//集合の終端文字を見つけた場合は、tokenCountを1減らす。
				tokenCount--;
				if(tokenCount>0){
					//集合の読み込みが終了しておらず、既存の完了した集合読み込みも存在しない場合は終端文字もsplitStrに追加する
					splitStr.push_back(m_ender);
				}else if(tokenCount==0){
					//集合の読み込みが終了した場合は、集合の読み込みフラグを操作する。
					setExist=true;
				} else{
					//tokenCountが負になるような終端文字の読み込みは無視する。
					tokenCount=0;
				}
			} else{
				//それ以外の文字は、splitStrに格納する
				splitStr.push_back(*it);
			}
		}
		//末尾まで読んだ後に、splitStrに文字列が残っている場合はそれを用いてStringSpliterに追加する
		pushFunc();
	}
}

StringBuilder::~StringBuilder(){}

std::string StringBuilder::GetString()const{
	if(m_splitFlag){
		//区切りがあるのであれば、区切り文字を追加しながらstringを作っていく。
		if(!m_vec.empty()){
			std::string str="";
			for(const StringBuilder &sb:m_vec){
				if(sb.GetSplitFlag()){
					//sbが分割されたものなら集合文字を入れる
					str+=m_beginer+sb.GetString()+m_ender+m_spliter;
				} else{
					//sbが分割されたものでないなら集合文字は入れない
					str+=sb.GetString()+m_spliter;
				}
			}
			//最後の区切り文字は抜く。strがemptyである事はないので例外処理は書かなくて良い。
			str.pop_back();
			return str;
		} else{
			return "";
		}
	} else{
		//区切りが無いのであればm_strをそのまま返す。
		return m_str;
	}
}

std::vector<StringBuilder> StringBuilder::GetVector()const{
	if(m_splitFlag){
		//区切りがあるのであれば、m_vecをそのまま返す
		return m_vec;
	} else{
		//区切りが無いのであればthisのみの配列として返す。
		return std::vector<StringBuilder>{*this};
	}
}

std::vector<std::string> StringBuilder::GetStringVector()const{
	if(m_splitFlag){
		//区切りがあるのであれば、m_vecの各要素のGetString()の配列を返す
		std::vector<std::string> v;
		for(const StringBuilder &sb:m_vec){
			v.push_back(sb.GetString());
		}
		return v;
	} else{
		//区切りが無いのであればthisのみのstringの配列として返す。
		return std::vector<std::string>{m_str};
	}
}
