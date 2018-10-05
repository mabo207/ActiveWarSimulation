#ifndef DEF_GENERALPURPOSERESOURCEMANAGER_H
#define DEF_GENERALPURPOSERESOURCEMANAGER_H

#include<vector>

//ゲームのいかなる場面でも頻繁に用いられる素材を管理するクラス
//インクルードすればどのソースコードでも呼び出せる
struct GeneralPurposeResourceManager{
private:

public:
	//画像

	//フォント

	//音
	static int decideSound;
	static int cancelSound;
	static int selectSound;

	//ローダー関数
	static void LoadResource();//main関数以外で呼び出されてはいけない
	//解放関数
	static void ReleaseResource();//main関数以外で呼び出されてはいけない
};


#endif // !DEF_GENERALPURPOSEEFFECTSOUNDMANAGER_H
