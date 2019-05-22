#ifndef DEF_STRINGBUILDER_H
#define DEF_STRINGBUILDER_H

#include<string>
#include<vector>
#include<memory>

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
private:
	void Split(const std::string &str,char spliter,char beginer,char ender,bool deepen);

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
	//Split()は前後でm_strが変化する事に注意
	void Split(bool deepen){
		Split(m_spliter,m_beginer,m_ender,deepen);
	}
	void Split(char spliter,char beginer,char ender,bool deepen){
		Split(m_str,spliter,beginer,ender,deepen);
	}
};

class NewSB{
public:
	NewSB(const std::shared_ptr<const std::string> &originStr
		,const size_t originStrSize
		,const char spliter
		,const char beginer
		,const char ender
		,const char parentBeginer
		,const char parentEnder
		,const size_t topIndex
		,const bool deepen
		,const bool splitFlag);
	~NewSB();
	std::string GetString()const;
	
	const char m_parentBeginer,m_parentEnder;//この要素の親のm_beginerとm_ender
	const char m_spliter,m_beginer,m_ender;//文字列の区切り方
	std::vector<NewSB> m_vec;

private:
	NewSB(const std::shared_ptr<const std::string> &originStr
		,const char parentBeginer
		,const char parentEnder
		,const size_t topIndex
		,const size_t length);//末尾作成用コンストラクタ
	size_t GetButtomIndex()const;

	const std::shared_ptr<const std::string> m_originStr;//分割の元となる文字列
	//m_originStrのどこからどこまでを参照しているか
	const size_t m_topIndex;//コンストラクタ作成時に初期化可能
	size_t m_length;//部分文字列の長さ
};

#endif // !DEF_STRINGBUILDER_H