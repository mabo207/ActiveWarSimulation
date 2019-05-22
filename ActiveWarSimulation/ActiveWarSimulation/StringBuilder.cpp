#include"StringBuilder.h"

//文字列の分割・結合を行うクラス
StringBuilder::StringBuilder(const std::string &str,char spliter,char beginer,char ender,bool deepen,bool setSplit)
	:m_spliter(spliter),m_beginer(beginer),m_ender(ender),m_splitFlag(false)
{
	if(!setSplit){
		//文字列として初期化すると決まっているのならそのようにする
		m_str=str;
	} else{
		//分割読み込み処理を行う場合、分割処理をする
		Split(str,spliter,beginer,ender,deepen);
	}
}

StringBuilder::~StringBuilder(){}

std::string StringBuilder::GetString()const{
	if(m_splitFlag){
		//区切りがあるのであれば、区切り文字を追加しながらstringを作っていく。
		if(!m_vec.empty()){
			std::string str="";
			for(const StringBuilder &sb:m_vec){
				if(sb.GetSplitFlag()){
					//sbが分割されたものなら集合文字を入れる
					str+=m_beginer+sb.GetString()+m_ender+m_spliter;
				} else{
					//sbが分割されたものでないなら集合文字は入れない
					str+=sb.GetString()+m_spliter;
				}
			}
			//最後の区切り文字は抜く。strがemptyである事はないので例外処理は書かなくて良い。
			str.pop_back();
			return str;
		} else{
			return "";
		}
	} else{
		//区切りが無いのであればm_strをそのまま返す。
		return m_str;
	}
}

std::vector<StringBuilder> StringBuilder::GetVector()const{
	if(m_splitFlag){
		//区切りがあるのであれば、m_vecをそのまま返す
		return m_vec;
	} else{
		//区切りが無いのであればthisのみの配列として返す。
		return std::vector<StringBuilder>{*this};
	}
}

std::vector<std::string> StringBuilder::GetStringVector()const{
	if(m_splitFlag){
		//区切りがあるのであれば、m_vecの各要素のGetString()の配列を返す
		std::vector<std::string> v;
		for(const StringBuilder &sb:m_vec){
			v.push_back(sb.GetString());
		}
		return v;
	} else{
		//区切りが無いのであればthisのみのstringの配列として返す。
		return std::vector<std::string>{m_str};
	}
}

void StringBuilder::Split(const std::string &str,char spliter,char beginer,char ender,bool deepen){
	if(!m_splitFlag){
		//分割が行われていない時のみ行う
		//下準備
		std::string splitStr;//分割された文字列
		int tokenCount=0;//beginerを読み込んだ個数-enderを読み込んだ個数。負になる減少は無視する。
		bool setExist=false;//既に集合を読み終わったかどうか。区切り文字を読み取るたびにfalseにする。「aa(cc,(dd,ee))bb(ff)」のaa,bb,ffのようなバグデータを無視するために用いる。
		//新たなStringSpliterを生成する際の処理
		const auto pushFunc=[&]()->void{
			//deepenがfalseであるか、splitStrに集合が含まれていないならこれ以上深くならない
			//最終引数がtrueである限りこの処理は再帰的に呼び出されるが、いずれ(next!=ite)がfalseになる。（処理のたびに集合外のspliterと最外のbeginer,enderがstrから消滅するため）
			m_vec.push_back(StringBuilder(splitStr,spliter,beginer,ender,true,deepen && setExist));
			splitStr.clear();
			setExist=false;
		};
		//strの先頭から末尾まで読み込み
		for(std::string::const_iterator it=str.begin(),ite=str.end();it!=ite;it++){
			if(*it==spliter && tokenCount<=0 && !splitStr.empty()){
				//区切り文字にたどり着いたらsplitStrを用いて新たなStringSpliterを作成する
				pushFunc();
			} else if(*it==beginer){
				//集合の先頭文字を見つけた場合は、tokenCountを1増やす
				if(!setExist){
					//splitStrを弄るのはまだ集合を読み込んでいない時のみ。
					if(tokenCount>0){
						//既に集合内である場合は、splitStrに文字を追加
						splitStr.push_back(beginer);
					} else{
						//集合外であるならここから集合が始まるので、splitStrに文字を追加しない。また、「aa(cc,(dd,ee))bb(ff)」のaa,bb,ffのようなバグデータを無視するためにaa部分は削除する。
						splitStr.clear();
					}
				}
				tokenCount++;
			} else if(*it==ender){
				//集合の終端文字を見つけた場合は、tokenCountを1減らす。
				tokenCount--;
				if(!setExist){
					if(tokenCount>0){
						//集合の読み込みが終了しておらず、既存の完了した集合読み込みも存在しない場合は終端文字もsplitStrに追加する
						splitStr.push_back(ender);
					} else if(tokenCount==0){
						//集合の読み込みが終了した場合は、集合の読み込みフラグを操作する。
						setExist=true;
					} else{
						//tokenCountが負になるような終端文字の読み込みは無視する。
						tokenCount=0;
					}
				}
			} else{
				//それ以外の文字は、splitStrに格納する
				if(!setExist){
					splitStr.push_back(*it);
				}
			}
		}
		//末尾まで読んだ後に、splitStrに文字列が残っている場合はそれを用いてStringSpliterに追加する
		pushFunc();
		//分割が行われたため、それが分かるように処理をする
		m_splitFlag=true;
		m_str.clear();
	}
}

//---------------NewSB--------------------
NewSB::NewSB(const std::shared_ptr<const std::string> &originStr
	,const size_t originStrSize
	,const char spliter
	,const char beginer
	,const char ender
	,const char parentBeginer
	,const char parentEnder
	,const size_t topIndex
	,const bool deepen
	,const bool splitFlag)
	:m_spliter(spliter)
	,m_beginer(beginer)
	,m_ender(ender)
	,m_parentBeginer(parentBeginer)
	,m_parentEnder(parentEnder)
	,m_originStr(originStr)
	,m_topIndex(topIndex)
	,m_length(originStrSize-topIndex)//最大の文字列数をひとまず格納
{
	//m_topIndexから文字を調べていく
	bool subNewSBExist=false;
	size_t subNewSBTopIndex=m_topIndex;//子要素となる
	for(size_t i=m_topIndex;i<originStrSize;i++){
		const char c=(*originStr)[i];
		if(c==m_beginer){
			//ここから先はm_enderが出現するまでしばらく区切り文字を無視するよ
			const NewSB subNewSB=NewSB(m_originStr,originStrSize,m_spliter,m_beginer,m_ender,m_beginer,m_ender,i+1,deepen,deepen);
			//読み込んだ要素を追加
			m_vec.push_back(subNewSB);
			subNewSBExist=true;
			//読み込み位置をズラす
			i=subNewSB.GetButtomIndex()+1;//subNewSBのm_enderの読み込みは無視して良いため、+1する
		} else if(c==m_spliter){
			//ここで区切る
			if(subNewSBExist){
				//まだ子要素のNewSBを作成していない場合は、ここまでで要素を作成する
				m_vec.push_back(NewSB(m_originStr,m_beginer,m_ender,subNewSBTopIndex,i-subNewSBTopIndex));//長さはi番目の要素(m_spliter)を除くので、+1しなくて良い
			}
			//次の要素の読み取りの準備
			subNewSBExist=false;
			subNewSBTopIndex=i+1;
		} else if(c==m_parentEnder){
			//親要素の終端地点が来たら、このNewSBの読み取りは終了
			if(subNewSBExist){
				//まだ子要素のNewSBを作成していない場合は、ここまでで要素を作成する
				m_vec.push_back(NewSB(m_originStr,m_beginer,m_ender,subNewSBTopIndex,i-subNewSBTopIndex));//長さはi番目の要素(m_spliter)を除くので、+1しなくて良い
			}
			//このNewSBの長さを再計算
			m_length=i-m_topIndex;//m_parentEnderは除くので、+1はしなくて良い
			//ループ脱出
			break;
		}
	}
}

NewSB::NewSB(const std::shared_ptr<const std::string> &originStr
	,const char parentBeginer
	,const char parentEnder
	,const size_t topIndex
	,const size_t length)
	:m_originStr(originStr)
	,m_spliter('\0')
	,m_beginer('\0')
	,m_ender('\0')
	,m_parentBeginer(parentBeginer)
	,m_parentEnder(parentEnder)
	,m_topIndex(topIndex)
	,m_length(length)
{}

NewSB::~NewSB(){}

std::string NewSB::GetString()const{
	return m_originStr->substr(m_topIndex,m_length);
}

size_t NewSB::GetButtomIndex()const{
	return m_topIndex+m_length-1;
}