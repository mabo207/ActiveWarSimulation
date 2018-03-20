#ifndef DEF_GRAPHICPROGRAMMING_H
#define DEF_GRAPHICPROGRAMMING_H

#include<vector>
#include<string>

//縦連結式コマンド表示ボックスとコマンドを表示する
class VerticalExtendCommandBox{
	//列挙体・型

	//定数
protected:
	static const int merginDX,merginDY;//ボックスの枠の太さ
	static const int strUpMergin,strDownMergin;//文字間の余白

	//変数
protected:
	unsigned int m_commandNum;//縦に表示するコマンドの数
	std::vector<std::string> m_commandString;//表示するコマンド
	int m_strDX,m_strDY;//コマンド表示部分の縦横の長さ
	int m_usingFont;//コマンド表示に用いるフォント
	int m_usingFontSize;//フォントの大きさ
	const bool m_fontCreateFlag;//フォントをここで作ったかどうか

	//関数
protected:

public:
	VerticalExtendCommandBox(const std::vector<std::string> &commandString,int usingFont);
	virtual ~VerticalExtendCommandBox();
	int Draw(int x,int y);
	int GetCommandStrX();
	int GetCommandStrY(unsigned int y);
	int GetCommandBoxDX();
	int GetCommandBoxDY();
};

#endif