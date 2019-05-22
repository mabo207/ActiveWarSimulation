#ifndef DEF_STRINGBUILDER_H
#define DEF_STRINGBUILDER_H

#include<string>
#include<vector>

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

#endif // !DEF_STRINGBUILDER_H