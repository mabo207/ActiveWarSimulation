#ifndef DEF_BROWSERTWEET_H
#define DEF_BROWSERTWEET_H

#include<string>
#include<vector>

//ツイート機能を補助するためのライブラリ
class BrowserTweet{
public:
	static bool TweetText(const std::string &text,const std::vector<std::string> &hashtags);
private:
	static std::wstring ShiftjisToWstring(const std::string &sjisStr);
	static std::string WstringToUtf8(const std::wstring &wstr);
	static std::string ShiftjisToUtf8(const std::string &sjisStr);
	static std::string HttpEncode(const std::string &str);

	static const std::string site;
};

#endif // !DEF_BROWSERTWEET_H