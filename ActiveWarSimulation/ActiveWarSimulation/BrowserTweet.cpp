#include"BrowserTweet.h"
#include<Windows.h>
#include<shellapi.h>
#include<objbase.h>
#include<math.h>

namespace {
	const char encTable[16] ={
		'0','1','2','3','4','5','6','7',
		'8','9','a','b','c','d','e','f'
	};
	const bool safeTable[128] ={
		//      0     1     2     3     4     5     6     7     8     9     A     B     C     D     E     F
		/* 0 */ false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,
		/* 1 */ false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,
		/* 2 */ false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,
		/* 3 */ true,true,true,true,true,true,true,true,true,true,false,false,false,false,false,false,
		/* 4 */ false,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,
		/* 5 */ true,true,true,true,true,true,true,true,true,true,true,false,false,false,false,false,
		/* 6 */ false,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,
		/* 7 */ true,true,true,true,true,true,true,true,true,true,true,false,false,false,false,false
	};
}

//---------------BrowserTweet---------------
const std::string BrowserTweet::site="https://twitter.com/intent/tweet";

bool BrowserTweet::TweetText(const std::string &text,const std::vector<std::string> &hashtags){
	//URIの作成
	std::string uri=site;
	uri+="?text="+HttpEncode(ShiftjisToUtf8(text));
	if(!hashtags.empty()){
		uri+="&hashtags=";
		for(size_t i=0,siz=hashtags.size();i<siz;i++){
			if(i>0){
				uri+=',';
			}
			uri+=HttpEncode(ShiftjisToUtf8(hashtags[i]));
		}
	}
	//既定ブラウザで上記URIを開く
	CoInitializeEx(NULL,COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
	const HINSTANCE hInst = ShellExecute(GetDesktopWindow(),"open",uri.c_str(),"","",SW_SHOW);
	//開くのに成功したかどうかを返す
	return(32 < (int)hInst);
}

std::wstring BrowserTweet::ShiftjisToWstring(const std::string &sjisStr){
	auto const dest_wc_size = ::MultiByteToWideChar(CP_ACP,0U,sjisStr.data(),-1,nullptr,0U);
	std::vector<wchar_t> destWide(dest_wc_size,L'\0');
	if(::MultiByteToWideChar(CP_ACP,0U,sjisStr.data(),-1,destWide.data(),destWide.size()) == 0) {
		throw std::system_error{static_cast<int>(::GetLastError()),std::system_category()};
	}
	destWide.resize(std::char_traits<wchar_t>::length(destWide.data()));
	destWide.shrink_to_fit();
	return std::wstring(destWide.begin(),destWide.end());
}

std::string BrowserTweet::WstringToUtf8(const std::wstring &wstr){
	auto const dest_c_size = ::WideCharToMultiByte(CP_UTF8,0U,wstr.data(),-1,nullptr,0,nullptr,nullptr);
	std::vector<char> destChar(dest_c_size,'\0');
	if(::WideCharToMultiByte(CP_UTF8,0U,wstr.data(),-1,destChar.data(),destChar.size(),nullptr,nullptr) == 0) {
		throw std::system_error{static_cast<int>(::GetLastError()),std::system_category()};
	}
	destChar.resize(std::char_traits<char>::length(destChar.data()));
	destChar.shrink_to_fit();
	return std::string(destChar.begin(),destChar.end());
}

std::string BrowserTweet::ShiftjisToUtf8(const std::string &sjisStr){
	return WstringToUtf8(ShiftjisToWstring(sjisStr));
}

std::string BrowserTweet::HttpEncode(const std::string &str){
	std::string encStr="";
	for(const char &ch:str){
		//マルチバイト文字が入り、ch<0になる可能性も存在する事に注意
		if(ch<0 || !safeTable[ch]){
			//エンコードが必要な文字
			encStr+='%';
			char p=(ch>>0x04)&0x0f;//負数の右シフト演算は、左端を何で埋められるかは処理系依存で未定義動作。なので00001111とAND演算して0埋めした場合と同じにする。
			encStr+=encTable[(ch>>0x04)&0x0f];
			p=ch&0x0f;
			encStr+=encTable[ch & 0x0f];
		} else{
			//エンコードが不必要な文字([0-9a-zA-Z])
			encStr+=ch;
		}
	}
	return encStr;
}
