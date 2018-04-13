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
	static const size_t fpsSize=60;

	//変数
private:
	int m_flame;
	unsigned long m_time[fpsSize];
	unsigned long m_recordTime;//処理時間を計測するための変数

	//関数
private:

public:
	FpsMeasuring();
	~FpsMeasuring();
	void Update();
	int GetFlame()const{
		return m_flame;
	}
	double GetFps()const;
	void RecordTime();//現在の時刻をm_recordTimeに記録する
	double GetProcessedTime()const;//m_recordTimeから経過した時間を返す
};

//描画関連
//画面全体を描画範囲にする
int SetDrawAllArea();

//文字列描画。\nで改行させる。また右端まで行ったら改行する。
//最後が\0で終わらない文字列だとフリーズまたはオーバーフローが起こる
int DrawStringNewLineToHandle(const int strX,const int strY,const int printableX,const int printableY,const int maxDX,const int maxDY,const int Color,const int Font,const int FontSize,const char *str);

int DrawStringNewLineToHandle(const int strX,const int strY,const int printableX,const int printableY,const int maxDX,const int maxDY,const int Color,const int Font,const int FontSize,const std::string &str);

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
		FUNCTION_EXPO
	};
	//変数
protected:
	int flame,maxflame;//フレーム数の管理。flameはmaxflameを超えて増加する。
	int x,startx,endx;//数値xの管理
	TYPE type;//変化形式
	FUNCTION function;//使用する関数
	double degree;//変化度合い
	//関数
public:
	Easing(int i_x=0,int i_maxflame=0,TYPE i_type=TYPE_IN,FUNCTION i_function=FUNCTION_LINER,double i_degree=0.0)
		:Easing(i_x,i_x,i_maxflame,i_type,i_function,i_degree){}
	Easing(int i_x,int i_endx,int i_maxflame,TYPE i_type,FUNCTION i_function,double i_degree);
	virtual ~Easing(){}//デストラクタ
	virtual void Update();//位置更新
	void SetTarget(int i_endx,bool initflame);//目標位置を決める
	void EnforceEnd();//強制的に動作後にする
	void Retry();//動作をリセットしてやり直す
	void Retry(int i_startx);//動作をリセットしてやり直す。スタート位置も変える
	int GetX()const{
		return x;
	}
	int GetstartX()const{
		return startx;
	}
	int GetendX()const{
		return endx;
	}
	int GetFlame()const{
		return flame;
	}
	virtual int GetMaxFlame()const{
		return maxflame;
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
	void SetMaxFlame(int flame,bool targetinitflag);
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
	PositionControl(int i_x=0,int i_y=0,int i_maxflame=0,Easing::TYPE i_type=Easing::TYPE_IN,Easing::FUNCTION i_function=Easing::FUNCTION_LINER,double i_degree=0.0)
		:PositionControl(i_x,i_x,i_y,i_y,i_maxflame,i_type,i_function,i_degree){}//位置の初期化（最初のみ）
	PositionControl(int i_x,int i_endx,int i_y,int i_endy,int i_maxflame,Easing::TYPE i_type,Easing::FUNCTION i_function,double i_degree)
		:x(i_x,i_endx,i_maxflame,i_type,i_function,i_degree),y(i_y,i_endy,i_maxflame,i_type,i_function,i_degree){}
	PositionControl(Vector2D start,Vector2D end,int i_maxflameX,Easing::TYPE i_typeX,Easing::FUNCTION i_functionX,double i_degreeX,int i_maxflameY,Easing::TYPE i_typeY,Easing::FUNCTION i_functionY,double i_degreeY)
		:x((int)start.x,(int)end.x,i_maxflameX,i_typeX,i_functionX,i_degreeX),y((int)start.y,(int)end.y,i_maxflameY,i_typeY,i_functionY,i_degreeY){}
	PositionControl(Vector2D start,Vector2D end,int i_maxflame,Easing::TYPE i_type,Easing::FUNCTION i_function,double i_degree)
		:PositionControl(start,end,i_maxflame,i_type,i_function,i_degree,i_maxflame,i_type,i_function,i_degree){}
	virtual ~PositionControl(){}//デストラクタ
	virtual void Update();//位置更新
	void SetTarget(int i_endx,int i_endy,bool initflame);//目標位置を決める
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
	int GetFlame()const{
		return x.GetFlame();
	}
	virtual int GetMaxFlame()const{
		return x.GetMaxFlame();
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
	void SetMaxFlame(int flame,bool targetinitflag);
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
	PositionComplexControl(int i_x=0,int i_endx=0,int i_y=0,int i_endy=0,int i_maxflame=0,Easing::TYPE i_type=Easing::TYPE_IN,Easing::FUNCTION i_function=Easing::FUNCTION_LINER,double i_degree=0.0)
		:indexX(0),indexY(0),x{Easing(i_x,i_endx,i_maxflame,i_type,i_function,i_degree)},y{Easing(i_y,i_endy,i_maxflame,i_type,i_function,i_degree)}{}//位置の初期化（最初のみ）
	PositionComplexControl(Vector2D start,Vector2D end,int i_maxflame,Easing::TYPE i_type,Easing::FUNCTION i_function,double i_degree)
		:PositionComplexControl((int)start.x,(int)end.x,(int)start.y,(int)end.y,i_maxflame,i_type,i_function,i_degree){}
	PositionComplexControl(Vector2D start,Vector2D end,int i_maxflame,Easing::TYPE i_typeX,Easing::FUNCTION i_functionX,double i_degreeX,Easing::TYPE i_typeY,Easing::FUNCTION i_functionY,double i_degreeY)
		:indexX(0),indexY(0),x{Easing((int)start.x,(int)end.x,i_maxflame,i_typeX,i_functionX,i_degreeX)},y{Easing((int)start.y,(int)end.y,i_maxflame,i_typeY,i_functionY,i_degreeY)}{}
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
	int GetFlame()const{
		int flame=0;
		for(const Easing &easing:x){
			flame+=easing.GetFlame();
		}
		return flame;
	}
	virtual int GetMaxFlame()const;
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
	const int fps;//Flame per Second。1以上の値が入る。

	//変数
protected:
	int counter;//フレーム数を数える。Updateのたびに1増えるだけ。
	int startTimer,endTimer;//比較対象。timerまで数える、という場合が殆ど。

	//関数
public:
	Timer(int i_fps);
	~Timer();
	int GetProcessCounter(bool secondFlag)const;//startTimerから数えた経過時間を返す。flame単位か秒単位で返すか選べる。
	int GetLeftCounter(bool secondFlag)const;//endTimerまで残りどのくらいあるかを返す。flame単位か秒単位で返すか選べる。
	bool JudgeEnd()const;//counterがendTimerを超えたかどうかを判定する
	bool SetTimer(int timeLength,bool secondFlag);//タイマーの設定をする。flame単位か秒単位で設定するか選べる。
	void Update();
	void EnforceEnd();
};

//文字列の分割・結合を行うクラス
class StringBuilder{
	//型・列挙体

	//定数

	//変数
protected:
	char m_spliter,m_beginer,m_ender;//それぞれ、区切り文字・集合の先頭文字・集合の終端文字
	//以下はどちらかにしか用いない
	std::string m_str;//区切りのない１つの文字列
public:
	std::vector<StringBuilder> m_vec;//区切られた複数文字列、トークンを途中で変えられるようにStringBuilderの配列にし、publicにする。
protected:
	//どちらに値が入っているか
	bool m_splitFlag;

	//関数
public:
	StringBuilder(const std::string &str,char spliter,char beginer,char ender,bool deepen,bool setSplit);
	~StringBuilder();

	char GetSpliter()const{
		return m_spliter;
	}
	char GetBeginer()const{
		return m_beginer;
	}
	char GetEnder()const{
		return m_ender;
	}
	bool GetSplitFlag()const{
		return m_splitFlag;
	}
	std::string GetString()const;
	std::vector<StringBuilder> GetVector()const;
	std::vector<std::string> GetStringVector()const;
};


#endif // !DEF_TOOLSLIB_H
#pragma once
