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

//文字列の分割・結合を行うクラス
//旧バージョンに比べて、deepen設定ができないが、計算速度はstd::vector<>::push_back()のコストを除けば旧バージョンでdeepenしないのと同等くらいの速度が出るので問題ない。
//再Split()も、かかるメモリがそこまで大きくないので痛くない。
class NewSB{
public:
	NewSB(const std::shared_ptr<const std::string> &originStr,
		const size_t originStrSize,
		const char spliter,
		const char beginer,
		const char ender,
		const char parentEnder,
		const size_t topIndex);
	~NewSB();
	std::string GetString()const;
	void Split();
	void Split(const char spliter,const char beginer,const char ender);
	
	std::vector<NewSB> m_vec;

private:
	NewSB(const std::shared_ptr<const std::string> &originStr,
		const size_t topIndex,
		const size_t length);//末尾作成用コンストラクタ
	size_t GetButtomIndex()const;
	void Split(const size_t originStrSize,
		const char spliter,
		const char beginer,
		const char ender,
		const char parentEnder);//コンストラクタ専用

	const std::shared_ptr<const std::string> m_originStr;//分割の元となる文字列
	char m_spliter,m_beginer,m_ender;//文字列の区切り方(Split()で変更される可能性があるので非const)
	//m_originStrのどこからどこまでを参照しているか
	const size_t m_topIndex;//コンストラクタ作成時に初期化可能
	size_t m_length;//部分文字列の長さ
};

#endif // !DEF_STRINGBUILDER_H