#include"StringBuilder.h"

//---------------StringBuilder--------------------
StringBuilder::StringBuilder(const std::shared_ptr<const std::string> &originStr,
	const size_t originStrSize,
	const char spliter,
	const char beginer,
	const char ender,
	const char parentEnder,
	const size_t topIndex)
	:m_spliter(spliter)
	,m_beginer(beginer)
	,m_ender(ender)
	,m_originStr(originStr)
	,m_topIndex(topIndex)
	,m_length(originStrSize-topIndex)//最大の文字列数をひとまず格納
{
	Split(originStrSize,spliter,beginer,ender,parentEnder);
}

StringBuilder::StringBuilder(const std::shared_ptr<const std::string> &originStr,
	const size_t topIndex,
	const size_t length)
	:m_originStr(originStr)
	,m_spliter('\0')
	,m_beginer('\0')
	,m_ender('\0')
	,m_topIndex(topIndex)
	,m_length(length)
{}

StringBuilder::StringBuilder(const std::string &originStr,
	const char spliter,
	const char beginer,
	const char ender)
	:StringBuilder(std::make_shared<const std::string>(originStr),originStr.size(),spliter,beginer,ender,'\0',0)
{}

StringBuilder::~StringBuilder(){}

std::string StringBuilder::GetString()const{
	return m_originStr->substr(m_topIndex,m_length);
}

void StringBuilder::Split(const char spliter,const char beginer,const char ender){
	if(spliter==m_spliter && beginer==m_beginer && ender==m_ender){
		//既にしている分割処理と同じ分割処理をしようとしている時は、分割せずに関数を終了する
		return;
	}
	//文字列の長さを計算
	const size_t originSize=m_originStr->size();
	//親の終端文字を確認
	const size_t parentEnderIndex=m_topIndex+m_length;
	char parentEnderChar;
	if(parentEnderIndex<originSize){
		parentEnderChar=(*m_originStr)[parentEnderIndex];
	} else{
		//一番rootのStringBuilderは、親の終端文字にアクセスできないので、'\0'を渡しておく
		parentEnderChar='\0';
	}
	//分割文字を変更
	m_spliter=spliter;
	m_beginer=beginer;
	m_ender=ender;
	//分割
	Split(originSize,m_spliter,m_beginer,m_ender,parentEnderChar);
}

void StringBuilder::Split(const size_t originStrSize,
	const char spliter,
	const char beginer,
	const char ender,
	const char parentEnder)
{
	//分割処理の前に初期化
	m_vec.clear();
	//m_topIndexから文字を調べていく
	bool subStringBuilderExist=false;
	size_t subStringBuilderTopIndex=m_topIndex;//子要素となる
	for(size_t i=m_topIndex;i<originStrSize;i++){
		const char c=(*m_originStr)[i];
		if(c==m_beginer){
			//ここから先はm_enderが出現するまでしばらく区切り文字を無視するよ
			const StringBuilder subStringBuilder=StringBuilder(m_originStr,originStrSize,m_spliter,m_beginer,m_ender,m_ender,i+1);
			//読み込んだ要素を追加
			m_vec.push_back(subStringBuilder);
			subStringBuilderExist=true;
			//読み込み位置をズラす
			i=subStringBuilder.GetButtomIndex()+1;//subStringBuilderのm_enderの読み込みは無視して良いため、+1する
		} else if(c==m_spliter){
			//ここで区切る
			if(!subStringBuilderExist){
				//まだ子要素のStringBuilderを作成していない場合は、ここまでで要素を作成する
				m_vec.push_back(StringBuilder(m_originStr,subStringBuilderTopIndex,i-subStringBuilderTopIndex));//長さはi番目の要素(m_spliter)を除くので、+1しなくて良い
			}
			//次の要素の読み取りの準備
			subStringBuilderExist=false;
			subStringBuilderTopIndex=i+1;
		} else if(c==parentEnder){
			//親要素の終端地点が来たら、このStringBuilderの読み取りは終了
			if(!subStringBuilderExist){
				//まだ子要素のStringBuilderを作成していない場合は、ここまでで要素を作成する
				m_vec.push_back(StringBuilder(m_originStr,subStringBuilderTopIndex,i-subStringBuilderTopIndex));//長さはi番目の要素(m_spliter)を除くので、+1しなくて良い
			}
			//このStringBuilderの長さを再計算
			m_length=i-m_topIndex;//m_parentEnderは除くので、+1はしなくて良い
			//ループ脱出
			break;
		}
	}
}

size_t StringBuilder::GetButtomIndex()const{
	return m_topIndex+m_length-1;
}