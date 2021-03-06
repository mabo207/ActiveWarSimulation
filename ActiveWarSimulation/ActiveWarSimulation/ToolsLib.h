#ifndef DEF_TOOLSLIB_H
#define DEF_TOOLSLIB_H

//インクルード
#include<string>
#include<vector>
#include<math.h>
#include<type_traits>

//一般的に用いることができる便利関数・構造体をここに書く
//位置についての構造体
class Vector2D{
public:
	//変数
	float x, y;
	//関数
	Vector2D(): x(0),y(0){}//既定のコンストラクタ
	Vector2D(float i_x, float i_y) : x(i_x), y(i_y) {}
	//加減算
	const Vector2D operator+(const Vector2D &otherobj)const {
		return Vector2D(x+otherobj.x,y+otherobj.y);
	}
	void operator+=(const Vector2D &otherobj){
		*this=(*this)+otherobj;
	}
	const Vector2D operator-(const Vector2D &otherobj)const {
		return Vector2D(x - otherobj.x, y - otherobj.y);
	}
	void operator-=(const Vector2D &otherobj){
		(*this)=(*this)-otherobj;
	}
	//定数倍
	const Vector2D operator*(float aMag)const{
		return Vector2D(x*aMag,y*aMag);
	}
	void operator*=(float aMag){
		(*this)=(*this)*aMag;
	}
	const Vector2D operator/(float aMag)const {
		return Vector2D(x/aMag,y/aMag);
	}
	void operator/=(float aMag){
		(*this)=(*this)/aMag;
	}
	//その他の演算子の定義
	const Vector2D operator-()const{
		return Vector2D(-x,-y);
	}
	bool operator==(const Vector2D &otherobj)const{
		return (this->x==otherobj.x) && (this->y==otherobj.y);
	}
	bool operator!=(const Vector2D &otherobj)const{
		return !((*this)==otherobj);
	}
	//内積
	float dot(const Vector2D &otherobj)const {
		return x*otherobj.x + y*otherobj.y;
	}
	//外積
	float cross(const Vector2D &otherobj)const {
		return x*otherobj.y - otherobj.x*y;
	}
	//長さ
	float size()const {
		return sqrt(x*x + y*y);
	}
	//サイズの二乗を返す
	float sqSize()const {
		return x*x + y*y;
	}
	Vector2D norm()const {
		float siz = size();
		if(siz!=0.0){
			return Vector2D(x/siz,y/siz);
		}
		return Vector2D(0.0f,0.0f);
	}
	//角度を返す(単位はラジアン)
	float GetRadian()const;
	//時計回りに回転させた時のベクトルを返す(角度の単位はラジアン)
	Vector2D turn(float radian)const;
};

//マウスの位置をVector2D型で返す関数
Vector2D GetMousePointVector2D();

//fps関連
class FpsMeasuring{
	//定数
private:
	static const size_t fpsSize=60+1;//60回の更新にかかった時間を図るには、60個前の時間が必要。間隔の個数を60にするため、1つ多く用意する。

	//変数
private:
	size_t m_frame;
	size_t m_recordFrame;//処理にかかったフレーム数を計測するための変数。あると便利である。
	unsigned long m_time[fpsSize];
	unsigned long m_recordTime;//処理時間を計測するための変数

	//関数
private:

public:
	FpsMeasuring();
	~FpsMeasuring();
	void Update();
	size_t GetFrame()const{
		return m_frame;
	}
	double GetFps()const;
	void RecordTime();//現在の時刻をm_recordTimeに記録する
	size_t GetProcessedFrame()const;//m_recordFrameから経過したフレーム数を返す
	double GetProcessedTime()const;//m_recordTimeから経過した時間を返す
};

//描画関連
//画面全体を描画範囲にする
int SetDrawAllArea();

//文字列描画。\nで改行させる。また右端まで行ったら改行する。
//最後が\0で終わらない文字列だとフリーズまたはオーバーフローが起こる
int DrawStringNewLineToHandle(const int strX,const int strY,const int printableX,const int printableY,const int maxDX,const int maxDY,const int Color,const int Font,const int FontSize,const char *str);

int DrawStringNewLineToHandle(const int strX,const int strY,const int printableX,const int printableY,const int maxDX,const int maxDY,const int Color,const int Font,const int FontSize,const std::string &str);

int DrawStringNewLineToHandle(const int strX,const int strY,const int maxDX,const int maxDY,const char *str,const int Color,const int Font,int spaceBetweenLines=0);//普段使いはこれでok。printableX,printableYによる切れた文字とか使わない場面も多い。

//上の文字列描画の方式で、描画はせずに必要なY座標の幅のみ求める
int GetStringHeightNewLineToHandle(const int maxDX,const int font,const char *str);

int GetStringHeightNewLineToHandle(const int maxDX,const int font,const std::string str);

//拡大描画。位置指定ではなく大きさ指定で拡大率を指定。
int DrawExtendGraphSizeAssign(int x,int y,int dx,int dy,int GrHandle,int TransFlag);

//中央の描画位置を指定した文字列描画
int DrawStringCenterBaseToHandle(const int centerx,const int centery,const char *str,unsigned int color,int fonthandle,bool yposcenterbaseflag,unsigned int EdgeColor=0U,int VerticalFlag=0);

//右寄せの文字列描画
int DrawStringRightJustifiedToHandle(int x,int y,const std::string &str,int color,int handle,unsigned int edgeColor=0U,int verticalFlag=0);

//int→string変換の際に、0詰めを行うようにする
std::string to_string_0d(int pal,unsigned int length);

//ファイルが存在するかどうかを調べる。実行ファイルの存在するディレクトリ上に存在するかを調べる。
bool JudgeFileExist(const std::string &str);

//現在の背景色のカラーコードを取得する
unsigned int GetBackgroundColor();

//反転色のカラーコードを取得する
unsigned int GetInvertedColor(unsigned int color);

//pointがp1,p2,p3による三角形の内部にあるかを判定
bool JudgeInTriangle(Vector2D point,Vector2D p1,Vector2D p2,Vector2D p3);

//内部で使用している描画解像度を取得する(ウインドウの拡大縮小で左右されないサイズが手に入る)
std::pair<int,int> GetWindowResolution();

//マウスがウインドウ内に入っているかの判定
bool JudgeMouseInWindow();

//グラデーション有りの四角形描画(x1<x2,y1<y2でないと正しく動作しない)
void DrawBoxGradation(int x1,int y1,int x2,int y2,unsigned int leftUpColor,unsigned int rightDownColor,bool horizontal);

void DrawBoxGradation(int x1,int y1,int x2,int y2,unsigned int leftUpColor,unsigned int leftDownColor,unsigned int rightUpColor,unsigned int rightDownColor);

//継承クラスのポインタのポインタを基底クラスのポインタのポインタにキャストする関数。継承先ポインタで作った配列を基底クラスのポインタで作った配列に変えたい時に使う
template<typename T,typename FROM> std::enable_if_t<std::is_base_of_v<T,FROM>,T**> pointer_array_cast(FROM **arr){
	//TがFROMを継承していないとコンパイルエラーになる。
	//std::enable_if_t<bool Condition,class T> --- Conditionがtrueの時のみ型Tを表す
	//std::is_base_of_v<class Base,class Derived> --- DerivedがBaseを継承しているか
	//std::is_convertible_v<class From,class To> --- FromからToへ暗黙的に変換が可能かどうか
	return reinterpret_cast<T**>(arr);
}

//継承クラスのポインタのポインタを基底クラスのポインタのポインタにキャストする関数。継承先ポインタで作った配列を基底クラスのポインタで作った配列に変えたい時に使う
template<typename T,typename FROM> std::enable_if_t<std::is_convertible_v<FROM*,T*>,T**> pointer_array_cast2(FROM **arr){
	//TがFROMを継承していないとコンパイルエラーになる。
	//std::enable_if_t<bool Condition,class T> --- Conditionがtrueの時のみ型Tを表す
	//std::is_base_of_v<class Base,class Derived> --- DerivedがBaseを継承しているか
	//std::is_convertible_v<class From,class To> --- FromからToへ暗黙的に変換が可能かどうか
	return reinterpret_cast<T**>(arr);
}

//数値の変化を様々な式で管理するクラス
class Easing{
	//列挙体
public:
	enum TYPE{
		TYPE_IN,
		TYPE_OUT,
		TYPE_INOUT
	};
	enum FUNCTION{
		FUNCTION_LINER,
		FUNCTION_QUAD,
		FUNCTION_EXPO,
		FUNCTION_BACK
	};
	//変数
protected:
	int frame,maxframe;//フレーム数の管理。frameはmaxframeを超えて増加する。
	int startx,endx;//数値xの管理
	TYPE type;//変化形式
	FUNCTION function;//使用する関数
	double degree;//変化度合い
	//関数
public:
	explicit Easing(int i_x=0,int i_maxframe=0,TYPE i_type=TYPE_IN,FUNCTION i_function=FUNCTION_LINER,double i_degree=0.0)
		:Easing(i_x,i_x,i_maxframe,i_type,i_function,i_degree){}
	Easing(int i_x,int i_endx,int i_maxframe,TYPE i_type,FUNCTION i_function,double i_degree);
	virtual ~Easing(){}//デストラクタ
	virtual void Update();//位置更新
	void SetTarget(int i_endx,bool initframe);//目標位置を決める
	void EnforceEnd();//強制的に動作後にする
	void Retry();//動作をリセットしてやり直す
	void Retry(int i_startx);//動作をリセットしてやり直す。スタート位置も変える
	int GetX()const;
	int GetstartX()const{
		return startx;
	}
	int GetendX()const{
		return endx;
	}
	int GetFrame()const{
		return frame;
	}
	virtual int GetMaxFrame()const{
		return maxframe;
	}
	FUNCTION GetFunction()const{
		return function;
	}
	TYPE GetType()const{
		return type;
	}
	double GetDegree()const{
		return degree;
	}
	double GetRate()const;
	void SetMaxFrame(int frame,bool targetinitflag);
	virtual bool GetEndFlag()const;//動作が終了しているかを判定する
};

//位置を色々な式で管理するクラス
class PositionControl{
	//列挙体
public:
	//変数
protected:
	Easing x,y;
	//関数
public:
	explicit PositionControl(int i_x=0,int i_y=0,int i_maxframe=0,Easing::TYPE i_type=Easing::TYPE_IN,Easing::FUNCTION i_function=Easing::FUNCTION_LINER,double i_degree=0.0)
		:PositionControl(i_x,i_x,i_y,i_y,i_maxframe,i_type,i_function,i_degree){}//位置の初期化（最初のみ）
	PositionControl(int i_x,int i_endx,int i_y,int i_endy,int i_maxframe,Easing::TYPE i_type,Easing::FUNCTION i_function,double i_degree)
		:x(i_x,i_endx,i_maxframe,i_type,i_function,i_degree),y(i_y,i_endy,i_maxframe,i_type,i_function,i_degree){}
	PositionControl(Vector2D start,Vector2D end,int i_maxframeX,Easing::TYPE i_typeX,Easing::FUNCTION i_functionX,double i_degreeX,int i_maxframeY,Easing::TYPE i_typeY,Easing::FUNCTION i_functionY,double i_degreeY)
		:x((int)start.x,(int)end.x,i_maxframeX,i_typeX,i_functionX,i_degreeX),y((int)start.y,(int)end.y,i_maxframeY,i_typeY,i_functionY,i_degreeY){}
	PositionControl(Vector2D start,Vector2D end,int i_maxframe,Easing::TYPE i_type,Easing::FUNCTION i_function,double i_degree)
		:PositionControl(start,end,i_maxframe,i_type,i_function,i_degree,i_maxframe,i_type,i_function,i_degree){}
	virtual ~PositionControl(){}//デストラクタ
	virtual void Update();//位置更新
	void SetTarget(int i_endx,int i_endy,bool initframe);//目標位置を決める
	void EnforceEnd();//強制的に動作後にする
	void Retry();//動作をリセットしてやり直す
	void Retry(int i_startx,int i_starty);//動作をリセットしてやり直す。スタート位置も変える
	Easing GetEasingX()const{
		return x;
	}
	Easing GetEasingY()const{
		return y;
	}
	int GetX()const{
		return x.GetX();
	}
	int GetstartX()const{
		return x.GetstartX();
	}
	int GetendX()const{
		return x.GetendX();
	}
	int GetY()const{
		return y.GetX();
	}
	int GetstartY()const{
		return y.GetstartX();
	}
	int GetendY()const{
		return y.GetendX();
	}
	int GetFrame()const{
		return x.GetFrame();
	}
	virtual int GetMaxFrame()const{
		return x.GetMaxFrame();
	}
	Easing::FUNCTION GetFunction()const{
		return x.GetFunction();
	}
	Easing::TYPE GetType()const{
		return x.GetType();
	}
	double GetDegree()const{
		return x.GetDegree();
	}
	double GetRate()const{
		return x.GetRate();
	}
	void SetMaxFrame(int frame,bool targetinitflag);
	virtual bool GetEndFlag()const;//動作が終了しているかを判定する
};

//位置を複数の式で管理するクラス
class PositionComplexControl{
	//列挙体
public:
	//変数
protected:
	size_t indexX,indexY;
	std::vector<Easing> x,y;

	//関数
public:
	explicit PositionComplexControl(int i_x=0,int i_endx=0,int i_y=0,int i_endy=0,int i_maxframe=0,Easing::TYPE i_type=Easing::TYPE_IN,Easing::FUNCTION i_function=Easing::FUNCTION_LINER,double i_degree=0.0)
		:indexX(0),indexY(0),x{Easing(i_x,i_endx,i_maxframe,i_type,i_function,i_degree)},y{Easing(i_y,i_endy,i_maxframe,i_type,i_function,i_degree)}{}//位置の初期化（最初のみ）
	PositionComplexControl(Vector2D start,Vector2D end,int i_maxframe,Easing::TYPE i_type,Easing::FUNCTION i_function,double i_degree)
		:PositionComplexControl((int)start.x,(int)end.x,(int)start.y,(int)end.y,i_maxframe,i_type,i_function,i_degree){}
	PositionComplexControl(Vector2D start,Vector2D end,int i_maxframe,Easing::TYPE i_typeX,Easing::FUNCTION i_functionX,double i_degreeX,Easing::TYPE i_typeY,Easing::FUNCTION i_functionY,double i_degreeY)
		:indexX(0),indexY(0),x{Easing((int)start.x,(int)end.x,i_maxframe,i_typeX,i_functionX,i_degreeX)},y{Easing((int)start.y,(int)end.y,i_maxframe,i_typeY,i_functionY,i_degreeY)}{}
	PositionComplexControl(const std::vector<PositionControl> &controlgroup);//多用すると重い
	PositionComplexControl(const std::vector<Easing> &i_x,const std::vector<Easing> &i_y)
		:indexX(0),indexY(0),x(i_x),y(i_y){}
	virtual ~PositionComplexControl(){}//デストラクタ
	virtual void Update();//位置更新
	void EnforceEnd();//強制的に動作後にする
	void Retry();//動作をリセットしてやり直す
	void Retry(int i_startx,int i_starty);//動作をリセットしてやり直す。スタート位置も変える
	std::vector<Easing> GetEasingX()const{
		return x;
	}
	std::vector<Easing> GetEasingY()const{
		return y;
	}
	int GetX()const{
		return x.at(indexX).GetX();
	}
	int GetstartX()const{
		return x.front().GetstartX();
	}
	int GetendX()const{
		return x.back().GetendX();
	}
	int GetY()const{
		return y.at(indexY).GetX();
	}
	int GetstartY()const{
		return y.front().GetstartX();
	}
	int GetendY()const{
		return y.back().GetendX();
	}
	int GetFrame()const{
		int frame=0;
		for(const Easing &easing:x){
			frame+=easing.GetFrame();
		}
		return frame;
	}
	virtual int GetMaxFrame()const;
	virtual bool GetEndFlag()const;//動作が終了しているかを判定する
};

//大きさ調整しつつ並べて表示する位置を計算するクラス
class LiningupScalingMechanism{
	//型・列挙体
public:
	enum DIRECTION{
		UP,
		LEFT,
		RIGHT,
		UNDER
	};
	//定数

	//変数
protected:
	DIRECTION fixedside;//どの辺を合わせるか
	int startx,starty;//開始位置
	PositionControl size;//拡大している物の調整

	//関数
public:
	LiningupScalingMechanism(int x,int y,DIRECTION side,PositionControl initsize);
	~LiningupScalingMechanism();
	void SetScaling(int startdx,int startdy,int enddx,int enddy);//大きさを決定する
	void Update();
	void Retry();
	void EnforceEnd();
	int GetX(int n,int expandingn,int reducingn)const;
	int GetY(int n,int expandingn,int reducingn)const;
	int GetNormalSizeX()const;
	int GetNormalSizeY()const;
	int GetExpandingSizeX()const;
	int GetExpandingSizeY()const;
	int GetReducingSizeX()const;
	int GetReducingSizeY()const;
};

//フレームを数えるためのクラス
class Timer{
	//定数
protected:
	const int fps;//Frame per Second。1以上の値が入る。

	//変数
protected:
	int counter;//フレーム数を数える。Updateのたびに1増えるだけ。
	int startTimer,endTimer;//比較対象。timerまで数える、という場合が殆ど。

	//関数
public:
	Timer(int i_fps);
	~Timer();
	int GetProcessCounter(bool secondFlag)const;//startTimerから数えた経過時間を返す。frame単位か秒単位で返すか選べる。
	int GetLeftCounter(bool secondFlag)const;//endTimerまで残りどのくらいあるかを返す。frame単位か秒単位で返すか選べる。
	bool JudgeEnd()const;//counterがendTimerを超えたかどうかを判定する
	bool SetTimer(int timeLength,bool secondFlag);//タイマーの設定をする。frame単位か秒単位で設定するか選べる。
	void Update();
	void EnforceEnd();
};

#endif // !DEF_TOOLSLIB_H
#pragma once
