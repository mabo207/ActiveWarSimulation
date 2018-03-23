#include<iostream>
#include"DxLib.h"
#include"GraphicProgramming.h"

//--------------------------VerticalExtendCommandBox--------------------------
const int VerticalExtendCommandBox::merginDX=4;
const int VerticalExtendCommandBox::merginDY=4;
const int VerticalExtendCommandBox::strUpMergin=1;
const int VerticalExtendCommandBox::strDownMergin=1;

VerticalExtendCommandBox::VerticalExtendCommandBox(const std::vector<std::string> &commandString,int usingFont)
	:m_commandString(commandString),m_commandNum(commandString.size()),m_fontCreateFlag(usingFont==-1),m_strDX(0)
{
	//フォントについての処理
	if(!m_fontCreateFlag){
		//フォントがしっかり生成されている場合
		m_usingFont=usingFont;
		m_usingFontSize=GetFontSizeToHandle(m_usingFont);
	}else{
		//フォントの再生成が必要な場合は生成する
		m_usingFontSize=12;
		m_usingFont=CreateFontToHandle("メイリオ",m_usingFontSize,1);
	}
	//コマンド表示に用いる値を求めておく
	for(unsigned int i=0;i<m_commandNum;i++){
		int dx=GetDrawStringWidthToHandle(m_commandString[i].c_str(),m_commandString[i].size(),m_usingFont);
		if(m_strDX<dx){
			m_strDX=dx;
		}
	}
	m_strDY=(static_cast<int>(m_commandNum))*(strUpMergin+m_usingFontSize+strDownMergin);
}

VerticalExtendCommandBox::~VerticalExtendCommandBox(){
	if(m_fontCreateFlag){
		DeleteFontToHandle(m_usingFont);
	}
}

int VerticalExtendCommandBox::Draw(int x,int y){
	//色定義
	int black=GetColor(85,85,85);
	int lightblue=GetColor(184,204,235);
	int darkblue=GetColor(53,107,162);
	int white=GetColor(255,255,255);
	//左上の枠の描画
	for(int yy=0;yy<merginDY;yy++){
		for(int xx=0;xx<merginDX;xx++){
			if(yy>=-xx+2){
				if(yy>=-xx+4){
					if(yy>=-xx+5){
						DrawPixel(x+xx,y+yy,darkblue);
					}else{
						DrawPixel(x+xx,y+yy,lightblue);
					}
				}else{
					DrawPixel(x+xx,y+yy,black);
				}
			}
		}
	}
	//上枠の描画
	DrawLine(x+merginDX,y,x+merginDX+m_strDX,y,black);
	DrawLine(x+merginDX,y+1,x+merginDX+m_strDX,y+1,lightblue);
	if(merginDY>2){
		DrawBox(x+merginDX,y+2,x+merginDX+m_strDX,y+merginDY,darkblue,TRUE);
	}
	//右上の描画
	DrawLine(x+merginDX+m_strDX,y,x+2*merginDX+m_strDX,y,black);
	DrawLine(x+2*merginDX+m_strDX-1,y,x+2*merginDX+m_strDX-1,y+merginDY,black);
	for(int yy=1;yy<merginDY;yy++){
		for(int xx=0;xx<merginDX-1;xx++){
			if(yy>=xx+1){
				if(yy>=xx+2){
					if(yy>=xx+3){
						DrawPixel(x+merginDX+m_strDX+xx,y+yy,darkblue);
					}else{
						DrawPixel(x+merginDX+m_strDX+xx,y+yy,lightblue);
					}
				}else{
					DrawPixel(x+merginDX+m_strDX+xx,y+yy,black);
				}
			}else{
				DrawPixel(x+merginDX+m_strDX+xx,y+yy,lightblue);
			}
		}
	}
	//左枠の描画
	DrawLine(x,y+merginDY,x,y+merginDY+m_strDY,black);
	DrawLine(x+1,y+merginDY,x+1,y+merginDY+m_strDY,lightblue);
	if(merginDX>3){
		DrawBox(x+2,y+merginDY,x+merginDX,y+merginDY+m_strDY,darkblue,TRUE);
	}
	//真ん中の描画
	DrawBox(x+merginDX,y+merginDY,x+merginDX+m_strDX,y+merginDY+m_strDY,darkblue,TRUE);
	//右枠の描画
	DrawLine(x+2*merginDX+m_strDX-1,y+merginDY,x+2*merginDX+m_strDX-1,y+merginDY+m_strDY,black);
	DrawLine(x+2*merginDX+m_strDX-2,y+merginDY,x+2*merginDX+m_strDX-2,y+merginDY+m_strDY,lightblue);
	if(merginDX>3){
		DrawBox(x+merginDX+m_strDX,y+merginDY,x+2*merginDX+m_strDX-2,y+merginDY+m_strDY,darkblue,TRUE);
	}
	//左下の描画
	DrawLine(x,y+merginDY+m_strDY,x,y+2*merginDY+m_strDY,black);
	DrawLine(x,y+2*merginDY+m_strDY-1,x+merginDX,y+2*merginDY+m_strDY-1,black);
	for(int yy=0;yy<merginDY-1;yy++){
		for(int xx=1;xx<merginDX;xx++){
			if(yy<=xx-1){
				if(yy<=xx-2){
					if(yy<=xx-3){
						DrawPixel(x+xx,y+merginDY+m_strDY+yy,darkblue);
					}else{
						DrawPixel(x+xx,y+merginDY+m_strDY+yy,lightblue);
					}
				}else{
					DrawPixel(x+xx,y+merginDY+m_strDY+yy,black);
				}
			}else{
				DrawPixel(x+xx,y+merginDY+m_strDY+yy,lightblue);
			}
		}
	}
	//下枠の描画
	DrawLine(x+merginDX,y+2*merginDY+m_strDY-1,x+merginDX+m_strDX,y+2*merginDY+m_strDY-1,black);
	DrawLine(x+merginDX,y+2*merginDY+m_strDY-2,x+merginDX+m_strDX,y+2*merginDY+m_strDY-2,lightblue);
	if(merginDY>2){
		DrawBox(x+merginDX,y+merginDY+m_strDY,x+merginDX+m_strDX,y+2*merginDY+m_strDY-2,darkblue,TRUE);
	}
	//右下の描画
	for(int yy=0;yy<merginDY;yy++){
		for(int xx=0;xx<merginDX;xx++){
			if(yy<=-xx+4){
				if(yy<=-xx+2){
					if(yy<=-xx+1){
						DrawPixel(x+merginDX+m_strDX+xx,y+merginDY+m_strDY+yy,darkblue);
					}else{
						DrawPixel(x+merginDX+m_strDX+xx,y+merginDY+m_strDY+yy,lightblue);
					}
				}else{
					DrawPixel(x+merginDX+m_strDX+xx,y+merginDY+m_strDY+yy,black);
				}
			}
		}
	}
	//文字描画
	for(unsigned int i=0;i<m_commandNum;i++){
		DrawStringToHandle(x+GetCommandStrX(),y+GetCommandStrY(i),m_commandString[i].c_str(),white,m_usingFont);
	}

	return 0;
}

int VerticalExtendCommandBox::GetCommandStrX(){
	return merginDX;
}

int VerticalExtendCommandBox::GetCommandStrY(unsigned int y){
	if(y>=0 && y<m_commandString.size()){
		int yy=static_cast<int>(y);
		return merginDY+strUpMergin*(yy+1)+(m_usingFontSize+strDownMergin)*yy;
	}
	return -30000;
}

int VerticalExtendCommandBox::GetCommandBoxDX(){
	return 2*merginDX+m_strDX;
}

int VerticalExtendCommandBox::GetCommandBoxDY(){
	return 2*merginDY+m_strDY;
}
