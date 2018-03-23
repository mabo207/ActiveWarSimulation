#include"DxLib.h"
#include"input.h"
#include"ScrollBar.h"
#include"EditActionSettings.h"

ScrollBar::ScrollButton::ScrollButton(Vector2D p1,Vector2D p2,Vector2D p3,Vector2D p4,Vector2D v)
	:m_point{p1,p2,p3,p4},m_move(v){}

//描画
void ScrollBar::ScrollButton::ButtonDraw(int font,int fillFlag)const{
	//枠あり、塗りつぶし四角形の描画なので
	//1.三角形２つの描画
	//2.４直線の描画
	//で行う
	unsigned int color=GetColor(192,192,192);
	unsigned int edge=GetBackgroundColor();
	if(fillFlag==TRUE){
		//三角形２つ
		DrawTriangle((int)m_point[0].x,(int)m_point[0].y,(int)m_point[1].x,(int)m_point[1].y,(int)m_point[2].x,(int)m_point[2].y,color,TRUE);
		DrawTriangle((int)m_point[2].x,(int)m_point[2].y,(int)m_point[3].x,(int)m_point[3].y,(int)m_point[0].x,(int)m_point[0].y,color,TRUE);
	}
	//枠
	for(size_t i=0;i<vNum;i++){
		DrawLine((int)m_point[i%vNum].x,(int)m_point[i%vNum].y,(int)m_point[(i+1)%vNum].x,(int)m_point[(i+1)%vNum].y,edge,1);
	}
}

//判定
bool ScrollBar::ScrollButton::JudgeInButton(Vector2D point)const{
	//Crossing Number Algorithmを用いる
	//pointからある方向の半直線について、交差する点の個数が奇数なら内部と判定する
	//自己交差している図形は内部に点があっても外部と判定されるが、そんな図形はスクロールバーに用いないので問題ない
	//辺上判定は曖昧
	//計算のため、半直線の方向ベクトルは(1,0)とする
	int crossNum=0;//交差点数
	for(size_t i=0;i<vNum;i++){
		Vector2D v=m_point[(i+1)%vNum]-m_point[i%vNum];//直線の方向ベクトルの長さ倍
		if(v.y==0){
			//直線がx軸並行の時
			//辺上判定にしかならないので何もしなくて良い
/*
			//pointのy座標が2点と等しく、x座標が2点の間にあれば交わっていると判定できる
			float vx1=point.x-m_point[i%vNum].x,vx2=point.x-m_point[(i+1)%vNum].x;
			if(((vx1>0 && vx2<=0) || (vx1<0 && vx2>=0)) && m_point[i%vNum].y==point.y){
				crossNum++;
			}
//*/
		}else{
			//直線がx軸平行でない時
			float crossx=m_point[i%vNum].x+(point.y-m_point[i%vNum].y)/v.y*v.x;
			//pointのy座標が2点の間にあり、交点がpointよりx方向に大きい位置にあれば交わっていると判定できる
			float vy1=point.y-m_point[i%vNum].y,vy2=point.y-m_point[(i+1)%vNum].y;
			if(((vy1>=0 && vy2<0) || (vy1<0 && vy2>=0)) && crossx>point.x){
				//y成分が増加する方向の辺は始点を含み終点を含まない(vy1>=0 && vy2<0)
				//y成分が減少する方向の辺は始点を含まず終点を含む(vy1<0 && vy2>=0)
				crossNum++;
			}
		}
	}
	//交差点数の遇奇の判定
	return (crossNum%2==1);//奇数ならtrueを返す
}

//ボタン押し判定
bool ScrollBar::ScrollButton::JudgeButtonPushed()const{
	return (mouse_get(MOUSE_INPUT_LEFT)==1 || mouse_get(MOUSE_INPUT_LEFT)>30);
}

//押された時の処理
void ScrollBar::ScrollButton::PushedProcess(EditActionSettings &settings)const{
	float rate;//動かす速さ・1フレーム目の何倍か
	if(mouse_get(MOUSE_INPUT_LEFT)==1){
		rate=1.0;
	}else{
		rate=20.0;
	}
	settings.PushScrollBar(m_move*rate);
}
