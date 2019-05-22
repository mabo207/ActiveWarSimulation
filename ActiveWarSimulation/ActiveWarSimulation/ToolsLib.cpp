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
	:m_frame(0),m_recordTime(0){}

FpsMeasuring::~FpsMeasuring(){}

void FpsMeasuring::Update(){
	m_frame++;
	m_time[m_frame%fpsSize]=timeGetTime();
}

double FpsMeasuring::GetFps()const{
	double fps;
	double timeUpdateFpsSize=(double)(m_time[m_frame%fpsSize]-m_time[(m_frame+1)%fpsSize]);//fpsSize��̍X�V�ɂǂ̂��炢�̎��Ԃ������������B�P�ʂ̓~���b
	if(timeUpdateFpsSize!=0.0){
		fps=(fpsSize-1)/timeUpdateFpsSize*1000;//�z��̊Ԋu�̌����X�V�񐔂Ȃ̂ŁAfpsSize����-1����B
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

int DrawStringNewLineToHandle(const int strX,const int strY,const int maxDX,const int maxDY,const char *str,const int Color,const int Font,int spaceBetweenLines){
	//�t�H���g�̑傫�����v�Z
	const int fontsize=GetFontSizeToHandle(Font);
	//�`��
	return DrawStringNewLineToHandle(strX,strY,strX,strY,maxDX,maxDY,Color,Font,fontsize+spaceBetweenLines,str);
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

//�����Ŏg�p���Ă���`��𑜓x���擾����(�E�C���h�E�̊g��k���ō��E����Ȃ��T�C�Y����ɓ���)
std::pair<int,int> GetWindowResolution(){
	//���ꂾ�ƃt���X�N���[�����[�h�ł͑Ή��ł��Ȃ��̂ŁA��ʉ𑜓x�̏���ێ����邵���Ȃ�
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

//�}�E�X���E�C���h�E���ɓ����Ă��邩�̔���
bool JudgeMouseInWindow(){
	std::pair<int,int> resolution=GetWindowResolution();
	int x,y;
	GetMousePoint(&x,&y);
	//�E�C���h�E�̑傫����������΁A�����`�Ɠ_�̓�������
	return (x>=0 && x<resolution.first && y>=0 && y<resolution.second);
}

//bgm��ǂݍ��ފ֐�
int LoadBGMMem(const std::string &dirname,int BufferNum,int UnionHandle){
	//����ǂݍ��ށB
	int handle=LoadSoundMem((dirname+"bgm.ogg").c_str(),BufferNum,UnionHandle);
	//���[�v�ʒu�̐ݒ�Binfo.csv�Ɂu���[�v�J�n�ʒu�v�u���[�v�I���ʒu�v�u����%�v���i�[����Ă���
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

//�O���f�[�V�����L��̎l�p�`�`��(x1<x2,y1<y2�łȂ��Ɛ��������삵�Ȃ�)
void DrawBoxGradation(int x1,int y1,int x2,int y2,unsigned int leftUpColor,unsigned int rightDownColor,bool horizontal){
	//���������܂��͐��������݂̂̃O���f�[�V����
	if(horizontal){
		//��������
		DrawBoxGradation(x1,y1,x2,y2,leftUpColor,leftUpColor,rightDownColor,rightDownColor);
	} else{
		//��������
		DrawBoxGradation(x1,y1,x2,y2,leftUpColor,rightDownColor,leftUpColor,rightDownColor);
	}
}

void DrawBoxGradation(int x1,int y1,int x2,int y2,unsigned int leftUpColor,unsigned int leftDownColor,unsigned int rightUpColor,unsigned int rightDownColor){
	//���_�F�̎擾
	int vertexColor[4][3];//0:(x1,y1) 1:(x1,y2) 2:(x2,y1) 3:(x2,y2)
	GetColor2(leftUpColor,&vertexColor[0][0],&vertexColor[0][1],&vertexColor[0][2]);
	GetColor2(leftDownColor,&vertexColor[1][0],&vertexColor[1][1],&vertexColor[1][2]);
	GetColor2(rightUpColor,&vertexColor[2][0],&vertexColor[2][1],&vertexColor[2][2]);
	GetColor2(rightDownColor,&vertexColor[3][0],&vertexColor[3][1],&vertexColor[3][2]);
	//�l�p�`�̕`��
	if(x1<x2 && y1<y2){
		for(int y=y1;y<y2;y++){
			for(int x=x1;x<x2;x++){
				//(x,y)�ɕ`�悷��F��
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
		//0���Z������Ń~�X���Ă���ꍇ�͕��ʂ�DrawBox������
		DrawBox(x1,y1,x2,y2,leftUpColor,TRUE);
	}
}

//���l�ω���l�X�Ȏ��ŊǗ�����N���X
//---Easing---
Easing::Easing(int i_x,int i_endx,int i_maxframe,TYPE i_type,FUNCTION i_function,double i_degree)
	:frame(0),maxframe(i_maxframe),startx(i_x),endx(i_endx),type(i_type),function(i_function),degree(i_degree){}

int Easing::GetX()const{
	if(!GetEndFlag()){
		//���삪�I����Ă��Ȃ��ꍇ�́A�������v�Z����
		return startx+(int)((endx-startx)*GetRate());
	} else{
		//���삪�I����Ă���ꍇ�́Aendx�ƕ����肫���Ă���
		return endx;
	}
}

double Easing::GetRate()const{
	double ft;//�Ԃ��l
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
					//0.5�ɂȂ��0���Z���N����̂ŕʏ���������
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
					//liner�ɂ���
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
		//initframe��true�̎��̂�frame��0��
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
	frame=maxframe;//frame��maxframe�Ŏ~�܂�悤�ɂ���B
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

//�ʒu��F�X�Ȏ��ŊǗ�����N���X
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

//�傫�����������ׂĕ\������ʒu���v�Z����N���X
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
		//frame�P�ʂŕԂ�
		return counter-startTimer;
	}
}

int Timer::GetLeftCounter(bool secondFlag)const{
	if(secondFlag){
		//�b�P�ʂŕԂ�
		return (endTimer-startTimer)/fps-GetProcessCounter(true);
	}else{
		//frame�P�ʂŕԂ�
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
		//frame�P�ʂŐݒ�
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
