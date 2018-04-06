#include<vector>
#include"ToolsLib.h"
#include"DxLib.h"
#include<fstream>
#include<iostream>
#include<algorithm>
#include<time.h>
#include<Windows.h>
#pragma comment(lib, "winmm.lib")

#define INTMAXINDEX 12//int�^�̍ő包��+2('-'��'\0'�̂��߂�+2)(int�^�̕�����)


//Vector2D�ɂ��Ă̊֐�
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

//FpeMeasuring�ɂ��Ă̊֐�
FpsMeasuring::FpsMeasuring()
	:m_flame(0),m_recordTime(0){}

FpsMeasuring::~FpsMeasuring(){}

void FpsMeasuring::Update(){
	m_flame++;
	m_time[m_flame%fpsSize]=timeGetTime();
}

double FpsMeasuring::GetFps()const{
	double fps;
	double timeUpdateFpsSize=(double)(m_time[m_flame%fpsSize]-m_time[(m_flame+1)%fpsSize]);//fpsSize��̍X�V�ɂǂ̂��炢�̎��Ԃ������������B�P�ʂ̓~���b
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

//�`��֘A
//��ʑS�̂�`��͈͂ɂ���
int SetDrawAllArea(){
	int dx,dy;
	GetScreenState(&dx,&dy,0);

	return SetDrawArea(0,0,dx,dy);
}

//������`��B\n�ŉ��s������B�܂��E�[�܂ōs��������s����B
//�Ōオ\0�ŏI���Ȃ������񂾂ƃt���[�Y�܂��̓I�[�o�[�t���[���N����
int DrawStringNewLineToHandle(const int strX,const int strY,const int printableX,const int printableY,const int maxDX,const int maxDY,const int Color,const int Font,const int FontSize,const char *str){
	//�`��\�͈͂�ݒ�
	SetDrawArea(printableX-1,printableY-1,printableX+maxDX+1,printableY+maxDY+1);//�ׂ��������ŕ������؂�邱�Ƃ�����̂ŗ]�T����������
																				 //������̒������i�[
	int strlength=strlen(str)+1;
	//���ꂼ��̕������ǂꂾ���`�敝���g�p���邩���v�Z����
	int *indexlength=new int[strlength];
	for(int i=0;i<strlength;){
		//i�͌��ݒ��ׂĂ��镶���̈ʒu�������Ă���
		char *partstr;
		if(str[i]>=0 || i+1>=strlength){//i�����ڂ�1�o�C�g�����Ȃ�
			partstr=new char[2];
			partstr[0]=str[i];
			partstr[1]='\0';//���ꂪ�Ȃ��ƕ�����������������̒��������߂Ă��܂�
			indexlength[i]=GetDrawStringWidthToHandle(partstr,2,Font);
			delete[] partstr;
			i+=1;
		} else{//2�o�C�g�����Ȃ�
			partstr=new char[3];
			partstr[0]=str[i];
			partstr[1]=str[i+1];
			partstr[2]='\0';
			indexlength[i]=0;//2�o�C�g�����̏ꍇ��2�o�C�g�ڂɂ̂ݒ������i�[����B
			indexlength[i+1]=GetDrawStringWidthToHandle(partstr,2,Font);
			delete[] partstr;
			i+=2;
		}
	}
	int linenum=0;//�s��(0�s�ځA1�s�ځE�E�E)
	int totalstrlen=0;//i�����ڂ܂ł̕`���̒����̍��v
	int totalstrdrawlen=0;//���܂łɕ`�悵���̕�����̕`���̒����̍��v
	int nowstartindex=0;//���̍s�̕`��̍ŏ��̕����̔z��ԍ�
	for(int i=0;i<strlength;i++){
		totalstrlen+=indexlength[i];
		//i�܂ł̕����������
		if(totalstrlen>maxDX+totalstrdrawlen || str[i]=='\n'){
			//������(i�܂�)�����ɃI�[�o�[����܂��͂����ŉ��s����Ȃ�
			//���̍s�̕�����(endnum�܂�)�𐶐�
			totalstrdrawlen=totalstrlen-indexlength[i];//�`��͂���̂ł��炩���ߍX�V
			int endnum=i;//���̍s�̕�����̍Ō�̕����̒���̔z��ԍ�
			if(endnum>0 && str[endnum-1]<0 && indexlength[endnum-1]==0){//���O�̔z��ԍ��̒��g�����āA2�o�C�g�����Ɣ��f������
				endnum--;//�Ō�̕������ЂƂO�Ɏ����Ă���
				totalstrlen-=indexlength[endnum];//�ς��������̒����������Ă���
			}
			char *STR=new char[endnum-nowstartindex+1];//�����ɏI�[���������邽�߂ɗv�f��1��������
			for(int s=0;s<endnum-nowstartindex;s++){
				STR[s]=str[s+nowstartindex];
			}
			STR[endnum-nowstartindex]='\0';
			//������̕`��
			if(strY+FontSize*(linenum+1)>printableY && strY+FontSize*linenum<printableY+maxDY){//�͈͊O�Ȃ�`�悵�Ȃ�
				DrawStringToHandle(strX,strY+FontSize*linenum,STR,Color,Font);
			}
			//��������̍X�V
			linenum++;
			nowstartindex=endnum;
			if(str[endnum]=='\n'){
				//���s�������݂Đ�ɐi�񂾏ꍇ
				nowstartindex++;//���̍s�͉��s��������X�^�[�g���Ȃ�
				totalstrdrawlen+=indexlength[endnum];//���s�������̒����͒ǉ�����ĂȂ��̂Œǉ�����
			}
			//if���I������
			delete[] STR;
			STR=NULL;
			continue;
		}
		if(i>=strlength-1){//�z�񂪍Ō�܂ŒB������
						   //�c��̕�����̕`��
			const char *pstr=&str[nowstartindex];
			if(strY+FontSize*(linenum+1)>printableY && strY+FontSize*linenum<printableY+maxDY){//�͈͊O�Ȃ�`�悵�Ȃ�
				DrawStringToHandle(strX,strY+FontSize*linenum,pstr,Color,Font);
			}
			//���[�v�𔲂���x��

		}
	}
	SetDrawAllArea();//�`��͈͐ݒ�̉���

					 //�I������
	delete[] indexlength;
	//return 0;
	return (linenum+1)*FontSize;
}

int DrawStringNewLineToHandle(const int X,const int Y,const int printableX,const int printableY,const int maxDX,const int maxDY,const int Color,const int Font,const int FontSize,const std::string &str){
	//�`��
	int returnnum=DrawStringNewLineToHandle(X,Y,printableX,printableY,maxDX,maxDY,Color,Font,FontSize,str.c_str());
	
	return returnnum;
}

//��̕�����`��̕����ŁA�`��͂����ɕK�v��Y���W�̕��̂݋��߂�
int GetStringHeightNewLineToHandle(const int maxDX,const int font,const char *str){
	//�ϐ�������
	int strlength=strlen(str)+1;//str�̕����񐔂��i�[
	int *indexlength=new int[strlength];//str�̊e������X�������߂�
	for(int i=0;i<strlength;){
		//�estr�𒲂ׂāA������̒������i�[����
		if(str[i]<0 && i+1<strlength){//str[i]��2�o�C�g������str[i+1]���z��O�Q�ƂłȂ����
			char *ch=new char[3];
			ch[0]=str[i];
			ch[1]=str[i+1];
			ch[2]='\0';
			indexlength[i]=GetDrawStringWidthToHandle(ch,2,font);
			indexlength[i+1]=0;
			delete[] ch;
			i+=2;
		} else{//1�o�C�g�����̏ꍇ
			char *ch=new char[2];
			ch[0]=str[i];
			ch[1]='\0';
			indexlength[i]=GetDrawStringWidthToHandle(ch,1,font);
			delete[] ch;
			i++;
		}
	}
	int fontsize=GetFontSizeToHandle(font);//�t�H���g�̑傫�����i�[
	int nowlinenum=1;//���ݑ��݂��Ă���s��
	int nowabovelineprintwidth=0;//���݂̍s����ɕ`�悵�Ă��镶�����X��
	int nowprintwidth=0;//���ݕ`�悵�Ă��镶�����X��
						//�s���v�Z
	for(int i=0;i<strlength;i++){
		if(nowprintwidth-nowabovelineprintwidth+indexlength[i]<=maxDX && str[i]!='\n'){
			//���̕������`�悵�Ă��O�ɂ͂ݏo���Ȃ��ꍇ�A�X�ɉ��s�����łȂ��ꍇ
			nowprintwidth+=indexlength[i];//����������ǉ�
		} else{
			//�͂ݏo���ꍇ�A�܂��͉��s�����𔭌������ꍇ
			nowabovelineprintwidth=nowprintwidth;//���݂܂ł̕�����̕����L�^
			nowlinenum++;//�s���̃J�E���g���P���₷
			nowprintwidth+=indexlength[i];//����������ǉ�
		}
	}
	//�I������
	delete[] indexlength;
	return nowlinenum*fontsize;
}

int GetStringHeightNewLineToHandle(const int maxDX,const int font,const std::string str){
	//string�^�����H
	char *c_str=new char[str.size()];
	//strcpy(c_str,str.c_str());
	for(unsigned int i=0;i<str.size();i++){
		c_str[i]=str[i];
	}
	//�v�Z
	int returnnum=GetStringHeightNewLineToHandle(maxDX,font,c_str);
	//�I������
	delete[] c_str;
	c_str=NULL;

	return returnnum;
}

//�g��`��B�ʒu�w��ł͂Ȃ��傫���w��Ŋg�嗦���w��B
int DrawExtendGraphSizeAssign(int x,int y,int dx,int dy,int GrHandle,int TransFlag){
	return DrawExtendGraph(x,y,x+dx,y+dy,GrHandle,TransFlag);
}

//���������̕�����`��
int DrawStringCenterBaseToHandle(const int centerx,const int centery,const char *str,unsigned int color,int fonthandle,bool yposcenterbaseflag,unsigned int EdgeColor,int VerticalFlag){
	if(yposcenterbaseflag){
		return DrawStringToHandle(centerx-GetDrawStringWidthToHandle(str,strlen(str),fonthandle)/2,centery-GetFontSizeToHandle(fonthandle)/2
			,str,color,fonthandle,EdgeColor,VerticalFlag);
	} else{
		return DrawStringToHandle(centerx-GetDrawStringWidthToHandle(str,strlen(str),fonthandle)/2,centery,str,color,fonthandle,EdgeColor,VerticalFlag);
	}
}

//�E�����̕�����`��
int DrawStringRightJustifiedToHandle(int x,int y,const std::string &str,int color,int handle,unsigned int edgeColor,int verticalFlag){
	const char *const pc=str.c_str();
	int dx=GetDrawStringWidthToHandle(pc,str.size(),handle,verticalFlag);
	return DrawStringToHandle(x-dx,y,pc,color,handle,edgeColor,verticalFlag);
}

//int��string�ϊ��̍ۂɁA0�l�߂��s���悤�ɂ���
std::string to_string_0d(int pal,unsigned int length){
	std::string str;
	str+=std::to_string(pal);
	str.reserve(length);
	for(unsigned int i=str.size();i<length;i++){
		str="0"+str;
	}
	return str;
}

//�t�@�C�������݂��邩�ǂ����𒲂ׂ�B���s�t�@�C���̑��݂���f�B���N�g����ɑ��݂��邩�𒲂ׂ�B
bool JudgeFileExist(const std::string &str){
	//�u�t�@�C�����J����v���u�t�@�C�������݂���v�Ƃ����l�����B�Z�L�����e�B�ʂ̖�肪����B
	std::ifstream ifs(str);
	bool flag=ifs.is_open();
	ifs.close();
	return flag;
}

//���݂̔w�i�F�̃J���[�R�[�h���擾����
unsigned int GetBackgroundColor(){
	int r,g,b;
	GetBackgroundColor(&r,&g,&b);
	return GetColor(r,g,b);
}

//���]�F�̃J���[�R�[�h���擾����
unsigned int GetInvertedColor(unsigned int color){
	int r,g,b;
	GetColor2(color,&r,&g,&b);
	return GetColor(255-r,255-g,255-b);
}

//point��p1,p2,p3�ɂ��O�p�`�̓����ɂ��邩�𔻒�
bool JudgeInTriangle(Vector2D point,Vector2D p1,Vector2D p2,Vector2D p3){
	//point��p1,p2,p3�ɂ��O�p�`�̓����ɂ��邩�𔻒�
	Vector2D p[3]={p1,p2,p3};//�������₷���悤�ɔz��
	//3�ӂ̃x�N�g����3�_����point�ւ̃x�N�g�����v�Z
	Vector2D v[3];//3�ӂ̃x�N�g��
	Vector2D toPos[3];//3�_����point�ւ̃x�N�g��
	for(int i=0;i<3;i++){
		v[i]=p[(i+1)%3]-p[i];
		toPos[i]=point-p[i];
	}
	//i=0,1,2�ɂ��āAv[i]*toPos[i]�̊O�ς̐������S�ē����ł���ΎO�p�`�����ɂ���Ɣ���ł���B
	bool direction[3];
	for(int i=0;i<3;i++){
		direction[i]=(v[i].cross(toPos[i])>=0.0f);
	}
	return (direction[0] & direction[1] & direction[2])|(!direction[0] & !direction[1] & !direction[2]);
}

//���l�ω���l�X�Ȏ��ŊǗ�����N���X
//---Easing---
Easing::Easing(int i_x,int i_endx,int i_maxflame,TYPE i_type,FUNCTION i_function,double i_degree)
	:flame(0),maxflame(i_maxflame),x(i_x),startx(i_x),endx(i_endx),type(i_type),function(i_function),degree(i_degree){}

void Easing::SetTarget(int i_endx,bool initflame){
	startx=x;
	if(initflame){
		//initflame��true�̎��̂�flame��0��
		flame=0;
	}
	endx=i_endx;
}

void Easing::Update(){
	double ft;//��������
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
	flame=maxflame;//flame��maxflame�Ŏ~�܂�悤�ɂ���B
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

//�ʒu��F�X�Ȏ��ŊǗ�����N���X
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
	//x��y�������t���[���Ǘ��Ȃ̂ŁAy��GetEndFlag��x��GetEndFlag������
	return x.GetEndFlag();
}

//�ʒu�𕡐��̎��ŊǗ�����N���X
//---PositionComplexControl---
PositionComplexControl::PositionComplexControl(const std::vector<PositionControl> &controlgroup)
	:indexX(0),indexY(0)
{
	//�T�C�Y�m��
	const size_t size=controlgroup.size();
	x.reserve(size);
	y.reserve(size);
	//�`��
	for(const PositionControl &control:controlgroup){
		x.push_back(control.GetEasingX());
		y.push_back(control.GetEasingY());
	}

}

void PositionComplexControl::Update(){
	x.at(indexX).Update();
	y.at(indexY).Update();
	//index�̍X�V
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
	//x��y�������t���[���Ǘ��Ȃ̂ŁAy��GetEndFlag��x��GetEndFlag������
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

//�傫�����������ׂĕ\������ʒu���v�Z����N���X
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
		//��ӂ𑵂��ĉ����тȂ̂ŁAn�Ԗڂ܂őS�đ������킹��
	case(UNDER):
		//���ӂ𑵂������тȂ̂ŁAn�Ԗڂ܂ő������킹��
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
		//���ӂ𑵂��ďc���тȂ̂ŁA���̂܂ܕԂ�
		break;
	case(RIGHT):
		//�E�ӂ𑵂��ďc���тȂ̂ŁAn�Ԗڂ̉����������ĕԂ�
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
		//���ӂ𑵂��ĉ����тȂ̂ŁAn�Ԗڂ܂őS�đ������킹��
	case(RIGHT):
		//�E�ӂ𑵂������тȂ̂ŁAn�Ԗڂ܂ő������킹��
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
		//��ӂ𑵂��ďc���тȂ̂ŁA���̂܂ܕԂ�
		break;
	case(UNDER):
		//���ӂ𑵂��ďc���тȂ̂ŁAn�Ԗڂ̉����������ĕԂ�
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

//�t���[�����𐔂��邽�߂̃N���XTimer
Timer::Timer(int i_fps)
	:fps(max(1,i_fps)),counter(0),startTimer(0),endTimer(0){}

Timer::~Timer(){}

int Timer::GetProcessCounter(bool secondFlag)const{
	if(secondFlag){
		//�b�P�ʂŕԂ�
		return (counter-startTimer)/fps;
	}else{
		//flame�P�ʂŕԂ�
		return counter-startTimer;
	}
}

int Timer::GetLeftCounter(bool secondFlag)const{
	if(secondFlag){
		//�b�P�ʂŕԂ�
		return (endTimer-startTimer)/fps-GetProcessCounter(true);
	}else{
		//flame�P�ʂŕԂ�
		return endTimer-counter;
	}
}

bool Timer::JudgeEnd()const{
	return counter>=endTimer;
}

bool Timer::SetTimer(int timeLength,bool secondFlag){
	startTimer=counter;
	if(secondFlag){
		//�b�P�ʂŐݒ�
		endTimer=startTimer+timeLength*fps;
	} else{
		//flame�P�ʂŐݒ�
		endTimer=startTimer+timeLength;
	}
	return true;//���s���Ȃ��̂�true��Ԃ��B
}

void Timer::Update(){
	counter++;
}

void Timer::EnforceEnd(){
	counter=endTimer;
}

//������̕����E�������s���N���X
StringBuilder::StringBuilder(const std::string &str,char spliter,char beginer,char ender,bool deepen,bool setSplit)
	:m_spliter(spliter),m_beginer(beginer),m_ender(ender)
{
	//������Ƃ��ď���������ƌ��܂��Ă���̂Ȃ炻�̂悤�ɂ��ďI������
	if(!setSplit){
		m_str=str;
		m_splitFlag=false;
	} else{
		//�����ǂݍ��ݏ������s���ꍇ
		m_splitFlag=true;//���̏����œǂݍ��܂��ꍇ�͕K����؂�ꂽ����������Ƃ��ēǂݍ���
		//������
		std::string splitStr;//�������ꂽ������
		int tokenCount=0;//m_beginer��ǂݍ��񂾌�-m_ender��ǂݍ��񂾌��B���ɂȂ錸���͖�������B
		bool setExist=false;//���ɏW����ǂ񂾂��ǂ����B��؂蕶����ǂݎ�邽�т�false�ɂ���B�uaa(cc,(dd,ee))bb(ff)�v��aa,bb,ff�̂悤�ȃo�O�f�[�^�𖳎����邽�߂ɗp����B
		//�V����StringSpliter�𐶐�����ۂ̏���
		const auto pushFunc=[&]()->void{
			//deepen��false�ł��邩�AsplitStr�ɏW�����܂܂�Ă��Ȃ��Ȃ炱��ȏ�[���Ȃ�Ȃ�
			//�ŏI������true�ł�����肱�̏����͍ċA�I�ɌĂяo����邪�A������(next!=ite)��false�ɂȂ�B�i�����̂��тɏW���O��m_spliter�ƍŊO��m_beginer,m_ender��str������ł��邽�߁j
			m_vec.push_back(StringBuilder(splitStr,m_spliter,m_beginer,m_ender,true,deepen && setExist));
			splitStr.clear();
			setExist=false;
		};
		//str�̐擪���疖���܂œǂݍ���
		for(std::string::const_iterator it=str.begin(),ite=str.end();it!=ite;it++){
			if(*it==m_spliter && tokenCount<=0 && !splitStr.empty()){
				//��؂蕶���ɂ��ǂ蒅������splitStr��p���ĐV����StringSpliter���쐬����
				pushFunc();
			} else if(*it==m_beginer){
				//�W���̐擪�������������ꍇ�́AtokenCount��1���₷
				if(!setExist){
					//splitStr��M��̂͂܂��W����ǂݍ���ł��Ȃ����̂݁B
					if(tokenCount>0){
						//���ɏW�����ł���ꍇ�́AsplitStr�ɕ�����ǉ�
						splitStr.push_back(m_beginer);
					} else{
						//�W���O�ł���Ȃ炱������W�����n�܂�̂ŁAsplitStr�ɕ�����ǉ����Ȃ��B�܂��A�uaa(cc,(dd,ee))bb(ff)�v��aa,bb,ff�̂悤�ȃo�O�f�[�^�𖳎����邽�߂�aa�����͍폜����B
						splitStr.clear();
					}
				}
				tokenCount++;
			} else if(*it==m_ender){
				//�W���̏I�[�������������ꍇ�́AtokenCount��1���炷�B
				tokenCount--;
				if(tokenCount>0){
					//�W���̓ǂݍ��݂��I�����Ă��炸�A�����̊��������W���ǂݍ��݂����݂��Ȃ��ꍇ�͏I�[������splitStr�ɒǉ�����
					splitStr.push_back(m_ender);
				}else if(tokenCount==0){
					//�W���̓ǂݍ��݂��I�������ꍇ�́A�W���̓ǂݍ��݃t���O�𑀍삷��B
					setExist=true;
				} else{
					//tokenCount�����ɂȂ�悤�ȏI�[�����̓ǂݍ��݂͖�������B
					tokenCount=0;
				}
			} else{
				//����ȊO�̕����́AsplitStr�Ɋi�[����
				splitStr.push_back(*it);
			}
		}
		//�����܂œǂ񂾌�ɁAsplitStr�ɕ����񂪎c���Ă���ꍇ�͂����p����StringSpliter�ɒǉ�����
		pushFunc();
	}
}

StringBuilder::~StringBuilder(){}

std::string StringBuilder::GetString()const{
	if(m_splitFlag){
		//��؂肪����̂ł���΁A��؂蕶����ǉ����Ȃ���string������Ă����B
		if(!m_vec.empty()){
			std::string str="";
			for(const StringBuilder &sb:m_vec){
				if(sb.GetSplitFlag()){
					//sb���������ꂽ���̂Ȃ�W������������
					str+=m_beginer+sb.GetString()+m_ender+m_spliter;
				} else{
					//sb���������ꂽ���̂łȂ��Ȃ�W�������͓���Ȃ�
					str+=sb.GetString()+m_spliter;
				}
			}
			//�Ō�̋�؂蕶���͔����Bstr��empty�ł��鎖�͂Ȃ��̂ŗ�O�����͏����Ȃ��ėǂ��B
			str.pop_back();
			return str;
		} else{
			return "";
		}
	} else{
		//��؂肪�����̂ł����m_str�����̂܂ܕԂ��B
		return m_str;
	}
}

std::vector<StringBuilder> StringBuilder::GetVector()const{
	if(m_splitFlag){
		//��؂肪����̂ł���΁Am_vec�����̂܂ܕԂ�
		return m_vec;
	} else{
		//��؂肪�����̂ł����this�݂̂̔z��Ƃ��ĕԂ��B
		return std::vector<StringBuilder>{*this};
	}
}

std::vector<std::string> StringBuilder::GetStringVector()const{
	if(m_splitFlag){
		//��؂肪����̂ł���΁Am_vec�̊e�v�f��GetString()�̔z���Ԃ�
		std::vector<std::string> v;
		for(const StringBuilder &sb:m_vec){
			v.push_back(sb.GetString());
		}
		return v;
	} else{
		//��؂肪�����̂ł����this�݂̂�string�̔z��Ƃ��ĕԂ��B
		return std::vector<std::string>{m_str};
	}
}
