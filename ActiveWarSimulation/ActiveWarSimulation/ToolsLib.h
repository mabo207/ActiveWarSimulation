#ifndef DEF_TOOLSLIB_H
#define DEF_TOOLSLIB_H

//�C���N���[�h
#include<string>
#include<vector>
#include<math.h>
#include<type_traits>

//��ʓI�ɗp���邱�Ƃ��ł���֗��֐��E�\���̂������ɏ���
//�ʒu�ɂ��Ă̍\����
class Vector2D{
public:
	//�ϐ�
	float x, y;
	//�֐�
	Vector2D(): x(0),y(0){}//����̃R���X�g���N�^
	Vector2D(float i_x, float i_y) : x(i_x), y(i_y) {}
	//�����Z
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
	//�萔�{
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
	//���̑��̉��Z�q�̒�`
	const Vector2D operator-()const{
		return Vector2D(-x,-y);
	}
	bool operator==(const Vector2D &otherobj)const{
		return (this->x==otherobj.x) && (this->y==otherobj.y);
	}
	bool operator!=(const Vector2D &otherobj)const{
		return !((*this)==otherobj);
	}
	//����
	float dot(const Vector2D &otherobj)const {
		return x*otherobj.x + y*otherobj.y;
	}
	//�O��
	float cross(const Vector2D &otherobj)const {
		return x*otherobj.y - otherobj.x*y;
	}
	//����
	float size()const {
		return sqrt(x*x + y*y);
	}
	//�T�C�Y�̓���Ԃ�
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
	//�p�x��Ԃ�(�P�ʂ̓��W�A��)
	float GetRadian()const;
	//���v���ɉ�]���������̃x�N�g����Ԃ�(�p�x�̒P�ʂ̓��W�A��)
	Vector2D turn(float radian)const;
};

//�}�E�X�̈ʒu��Vector2D�^�ŕԂ��֐�
Vector2D GetMousePointVector2D();

//fps�֘A
class FpsMeasuring{
	//�萔
private:
	static const size_t fpsSize=60;

	//�ϐ�
private:
	int m_flame;
	unsigned long m_time[fpsSize];
	unsigned long m_recordTime;//�������Ԃ��v�����邽�߂̕ϐ�

	//�֐�
private:

public:
	FpsMeasuring();
	~FpsMeasuring();
	void Update();
	int GetFlame()const{
		return m_flame;
	}
	double GetFps()const;
	void RecordTime();//���݂̎�����m_recordTime�ɋL�^����
	double GetProcessedTime()const;//m_recordTime����o�߂������Ԃ�Ԃ�
};

//�`��֘A
//��ʑS�̂�`��͈͂ɂ���
int SetDrawAllArea();

//������`��B\n�ŉ��s������B�܂��E�[�܂ōs��������s����B
//�Ōオ\0�ŏI���Ȃ������񂾂ƃt���[�Y�܂��̓I�[�o�[�t���[���N����
int DrawStringNewLineToHandle(const int strX,const int strY,const int printableX,const int printableY,const int maxDX,const int maxDY,const int Color,const int Font,const int FontSize,const char *str);

int DrawStringNewLineToHandle(const int strX,const int strY,const int printableX,const int printableY,const int maxDX,const int maxDY,const int Color,const int Font,const int FontSize,const std::string &str);

//��̕�����`��̕����ŁA�`��͂����ɕK�v��Y���W�̕��̂݋��߂�
int GetStringHeightNewLineToHandle(const int maxDX,const int font,const char *str);

int GetStringHeightNewLineToHandle(const int maxDX,const int font,const std::string str);

//�g��`��B�ʒu�w��ł͂Ȃ��傫���w��Ŋg�嗦���w��B
int DrawExtendGraphSizeAssign(int x,int y,int dx,int dy,int GrHandle,int TransFlag);

//�����̕`��ʒu���w�肵��������`��
int DrawStringCenterBaseToHandle(const int centerx,const int centery,const char *str,unsigned int color,int fonthandle,bool yposcenterbaseflag,unsigned int EdgeColor=0U,int VerticalFlag=0);

//�E�񂹂̕�����`��
int DrawStringRightJustifiedToHandle(int x,int y,const std::string &str,int color,int handle,unsigned int edgeColor=0U,int verticalFlag=0);

//int��string�ϊ��̍ۂɁA0�l�߂��s���悤�ɂ���
std::string to_string_0d(int pal,unsigned int length);

//�t�@�C�������݂��邩�ǂ����𒲂ׂ�B���s�t�@�C���̑��݂���f�B���N�g����ɑ��݂��邩�𒲂ׂ�B
bool JudgeFileExist(const std::string &str);

//���݂̔w�i�F�̃J���[�R�[�h���擾����
unsigned int GetBackgroundColor();

//���]�F�̃J���[�R�[�h���擾����
unsigned int GetInvertedColor(unsigned int color);

//point��p1,p2,p3�ɂ��O�p�`�̓����ɂ��邩�𔻒�
bool JudgeInTriangle(Vector2D point,Vector2D p1,Vector2D p2,Vector2D p3);

//�����Ŏg�p���Ă���`��𑜓x���擾����(�E�C���h�E�̊g��k���ō��E����Ȃ��T�C�Y����ɓ���)
std::pair<int,int> GetWindowResolution();

//�p���N���X�̃|�C���^�̃|�C���^�����N���X�̃|�C���^�̃|�C���^�ɃL���X�g����֐��B�p����|�C���^�ō�����z������N���X�̃|�C���^�ō�����z��ɕς��������Ɏg��
template<typename T,typename FROM> std::enable_if_t<std::is_base_of_v<T,FROM>,T**> pointer_array_cast(FROM **arr){
	//T��FROM���p�����Ă��Ȃ��ƃR���p�C���G���[�ɂȂ�B
	//std::enable_if_t<bool Condition,class T> --- Condition��true�̎��̂݌^T��\��
	//std::is_base_of_v<class Base,class Derived> --- Derived��Base���p�����Ă��邩
	//std::is_convertible_v<class From,class To> --- From����To�ֈÖٓI�ɕϊ����\���ǂ���
	return reinterpret_cast<T**>(arr);
}

//�p���N���X�̃|�C���^�̃|�C���^�����N���X�̃|�C���^�̃|�C���^�ɃL���X�g����֐��B�p����|�C���^�ō�����z������N���X�̃|�C���^�ō�����z��ɕς��������Ɏg��
template<typename T,typename FROM> std::enable_if_t<std::is_convertible_v<FROM*,T*>,T**> pointer_array_cast2(FROM **arr){
	//T��FROM���p�����Ă��Ȃ��ƃR���p�C���G���[�ɂȂ�B
	//std::enable_if_t<bool Condition,class T> --- Condition��true�̎��̂݌^T��\��
	//std::is_base_of_v<class Base,class Derived> --- Derived��Base���p�����Ă��邩
	//std::is_convertible_v<class From,class To> --- From����To�ֈÖٓI�ɕϊ����\���ǂ���
	return reinterpret_cast<T**>(arr);
}

//���l�̕ω���l�X�Ȏ��ŊǗ�����N���X
class Easing{
	//�񋓑�
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
	//�ϐ�
protected:
	int flame,maxflame;//�t���[�����̊Ǘ��Bflame��maxflame�𒴂��đ�������B
	int x,startx,endx;//���lx�̊Ǘ�
	TYPE type;//�ω��`��
	FUNCTION function;//�g�p����֐�
	double degree;//�ω��x����
	//�֐�
public:
	Easing(int i_x=0,int i_maxflame=0,TYPE i_type=TYPE_IN,FUNCTION i_function=FUNCTION_LINER,double i_degree=0.0)
		:Easing(i_x,i_x,i_maxflame,i_type,i_function,i_degree){}
	Easing(int i_x,int i_endx,int i_maxflame,TYPE i_type,FUNCTION i_function,double i_degree);
	virtual ~Easing(){}//�f�X�g���N�^
	virtual void Update();//�ʒu�X�V
	void SetTarget(int i_endx,bool initflame);//�ڕW�ʒu�����߂�
	void EnforceEnd();//�����I�ɓ����ɂ���
	void Retry();//��������Z�b�g���Ă�蒼��
	void Retry(int i_startx);//��������Z�b�g���Ă�蒼���B�X�^�[�g�ʒu���ς���
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
	virtual bool GetEndFlag()const;//���삪�I�����Ă��邩�𔻒肷��
};

//�ʒu��F�X�Ȏ��ŊǗ�����N���X
class PositionControl{
	//�񋓑�
public:
	//�ϐ�
protected:
	Easing x,y;
	//�֐�
public:
	PositionControl(int i_x=0,int i_y=0,int i_maxflame=0,Easing::TYPE i_type=Easing::TYPE_IN,Easing::FUNCTION i_function=Easing::FUNCTION_LINER,double i_degree=0.0)
		:PositionControl(i_x,i_x,i_y,i_y,i_maxflame,i_type,i_function,i_degree){}//�ʒu�̏������i�ŏ��̂݁j
	PositionControl(int i_x,int i_endx,int i_y,int i_endy,int i_maxflame,Easing::TYPE i_type,Easing::FUNCTION i_function,double i_degree)
		:x(i_x,i_endx,i_maxflame,i_type,i_function,i_degree),y(i_y,i_endy,i_maxflame,i_type,i_function,i_degree){}
	PositionControl(Vector2D start,Vector2D end,int i_maxflameX,Easing::TYPE i_typeX,Easing::FUNCTION i_functionX,double i_degreeX,int i_maxflameY,Easing::TYPE i_typeY,Easing::FUNCTION i_functionY,double i_degreeY)
		:x((int)start.x,(int)end.x,i_maxflameX,i_typeX,i_functionX,i_degreeX),y((int)start.y,(int)end.y,i_maxflameY,i_typeY,i_functionY,i_degreeY){}
	PositionControl(Vector2D start,Vector2D end,int i_maxflame,Easing::TYPE i_type,Easing::FUNCTION i_function,double i_degree)
		:PositionControl(start,end,i_maxflame,i_type,i_function,i_degree,i_maxflame,i_type,i_function,i_degree){}
	virtual ~PositionControl(){}//�f�X�g���N�^
	virtual void Update();//�ʒu�X�V
	void SetTarget(int i_endx,int i_endy,bool initflame);//�ڕW�ʒu�����߂�
	void EnforceEnd();//�����I�ɓ����ɂ���
	void Retry();//��������Z�b�g���Ă�蒼��
	void Retry(int i_startx,int i_starty);//��������Z�b�g���Ă�蒼���B�X�^�[�g�ʒu���ς���
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
	virtual bool GetEndFlag()const;//���삪�I�����Ă��邩�𔻒肷��
};

//�ʒu�𕡐��̎��ŊǗ�����N���X
class PositionComplexControl{
	//�񋓑�
public:
	//�ϐ�
protected:
	size_t indexX,indexY;
	std::vector<Easing> x,y;

	//�֐�
public:
	PositionComplexControl(int i_x=0,int i_endx=0,int i_y=0,int i_endy=0,int i_maxflame=0,Easing::TYPE i_type=Easing::TYPE_IN,Easing::FUNCTION i_function=Easing::FUNCTION_LINER,double i_degree=0.0)
		:indexX(0),indexY(0),x{Easing(i_x,i_endx,i_maxflame,i_type,i_function,i_degree)},y{Easing(i_y,i_endy,i_maxflame,i_type,i_function,i_degree)}{}//�ʒu�̏������i�ŏ��̂݁j
	PositionComplexControl(Vector2D start,Vector2D end,int i_maxflame,Easing::TYPE i_type,Easing::FUNCTION i_function,double i_degree)
		:PositionComplexControl((int)start.x,(int)end.x,(int)start.y,(int)end.y,i_maxflame,i_type,i_function,i_degree){}
	PositionComplexControl(Vector2D start,Vector2D end,int i_maxflame,Easing::TYPE i_typeX,Easing::FUNCTION i_functionX,double i_degreeX,Easing::TYPE i_typeY,Easing::FUNCTION i_functionY,double i_degreeY)
		:indexX(0),indexY(0),x{Easing((int)start.x,(int)end.x,i_maxflame,i_typeX,i_functionX,i_degreeX)},y{Easing((int)start.y,(int)end.y,i_maxflame,i_typeY,i_functionY,i_degreeY)}{}
	PositionComplexControl(const std::vector<PositionControl> &controlgroup);//���p����Əd��
	PositionComplexControl(const std::vector<Easing> &i_x,const std::vector<Easing> &i_y)
		:indexX(0),indexY(0),x(i_x),y(i_y){}
	virtual ~PositionComplexControl(){}//�f�X�g���N�^
	virtual void Update();//�ʒu�X�V
	void EnforceEnd();//�����I�ɓ����ɂ���
	void Retry();//��������Z�b�g���Ă�蒼��
	void Retry(int i_startx,int i_starty);//��������Z�b�g���Ă�蒼���B�X�^�[�g�ʒu���ς���
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
	virtual bool GetEndFlag()const;//���삪�I�����Ă��邩�𔻒肷��
};

//�傫�����������ׂĕ\������ʒu���v�Z����N���X
class LiningupScalingMechanism{
	//�^�E�񋓑�
public:
	enum DIRECTION{
		UP,
		LEFT,
		RIGHT,
		UNDER
	};
	//�萔

	//�ϐ�
protected:
	DIRECTION fixedside;//�ǂ̕ӂ����킹�邩
	int startx,starty;//�J�n�ʒu
	PositionControl size;//�g�債�Ă��镨�̒���

	//�֐�
public:
	LiningupScalingMechanism(int x,int y,DIRECTION side,PositionControl initsize);
	~LiningupScalingMechanism();
	void SetScaling(int startdx,int startdy,int enddx,int enddy);//�傫�������肷��
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

//�t���[���𐔂��邽�߂̃N���X
class Timer{
	//�萔
protected:
	const int fps;//Flame per Second�B1�ȏ�̒l������B

	//�ϐ�
protected:
	int counter;//�t���[�����𐔂���BUpdate�̂��т�1�����邾���B
	int startTimer,endTimer;//��r�ΏہBtimer�܂Ő�����A�Ƃ����ꍇ���w�ǁB

	//�֐�
public:
	Timer(int i_fps);
	~Timer();
	int GetProcessCounter(bool secondFlag)const;//startTimer���琔�����o�ߎ��Ԃ�Ԃ��Bflame�P�ʂ��b�P�ʂŕԂ����I�ׂ�B
	int GetLeftCounter(bool secondFlag)const;//endTimer�܂Ŏc��ǂ̂��炢���邩��Ԃ��Bflame�P�ʂ��b�P�ʂŕԂ����I�ׂ�B
	bool JudgeEnd()const;//counter��endTimer�𒴂������ǂ����𔻒肷��
	bool SetTimer(int timeLength,bool secondFlag);//�^�C�}�[�̐ݒ������Bflame�P�ʂ��b�P�ʂŐݒ肷�邩�I�ׂ�B
	void Update();
	void EnforceEnd();
};

//������̕����E�������s���N���X
class StringBuilder{
	//�^�E�񋓑�

	//�萔

	//�ϐ�
protected:
	char m_spliter,m_beginer,m_ender;//���ꂼ��A��؂蕶���E�W���̐擪�����E�W���̏I�[����
	//�ȉ��͂ǂ��炩�ɂ����p���Ȃ�
	std::string m_str;//��؂�̂Ȃ��P�̕�����
public:
	std::vector<StringBuilder> m_vec;//��؂�ꂽ����������A�g�[�N����r���ŕς�����悤��StringBuilder�̔z��ɂ��Apublic�ɂ���B
protected:
	//�ǂ���ɒl�������Ă��邩
	bool m_splitFlag;

	//�֐�
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
