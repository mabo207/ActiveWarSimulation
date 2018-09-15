#ifndef DEFINE_COMMONCONSTPARAMETER_H
#define DEFINE_COMMONCONSTPARAMETER_H

//include禁止！どのプロジェクトにも簡単に取り入れられるという利点が消滅してしまう！

//複数のプログラムにまたがって使用する定数値を定義しておくクラス
struct CommonConstParameter{
	//数値は宣言とともに確定する。宣言と初期化が別になってしまうと、他のクラスのstatic変数の処理より後に行われるなどで処理順序がおかしくなる可能性がある。
	//上記の理由により、整数でない値はここで宣言しないこと。
	static const int unitCircleSize;
	static const int gameResolutionX;
	static const int gameResolutionY;
};

#endif // !DEFINE_COMMONCONSTPARAMETER_H
