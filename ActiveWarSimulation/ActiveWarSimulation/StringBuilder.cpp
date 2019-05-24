#include"StringBuilder.h"

//������̕����E�������s���N���X
StringBuilderOld::StringBuilderOld(const std::string &str,char spliter,char beginer,char ender,bool deepen,bool setSplit)
	:m_spliter(spliter),m_beginer(beginer),m_ender(ender),m_splitFlag(false)
{
	if(!setSplit){
		//������Ƃ��ď���������ƌ��܂��Ă���̂Ȃ炻�̂悤�ɂ���
		m_str=str;
	} else{
		//�����ǂݍ��ݏ������s���ꍇ�A��������������
		Split(str,spliter,beginer,ender,deepen);
	}
}

StringBuilderOld::~StringBuilderOld(){}

std::string StringBuilderOld::GetString()const{
	if(m_splitFlag){
		//��؂肪����̂ł���΁A��؂蕶����ǉ����Ȃ���string������Ă����B
		if(!m_vec.empty()){
			std::string str="";
			for(const StringBuilderOld &sb:m_vec){
				if(sb.GetSplitFlag()){
					//sb���������ꂽ���̂Ȃ�W������������
					str+=m_beginer+sb.GetString()+m_ender+m_spliter;
				} else{
					//sb���������ꂽ���̂łȂ��Ȃ�W�������͓���Ȃ�
					str+=sb.GetString()+m_spliter;
				}
			}
			//�Ō�̋�؂蕶���͔����Bstr��empty�ł��鎖�͂Ȃ��̂ŗ�O�����͏����Ȃ��ėǂ��B
			str.pop_back();
			return str;
		} else{
			return "";
		}
	} else{
		//��؂肪�����̂ł����m_str�����̂܂ܕԂ��B
		return m_str;
	}
}

std::vector<StringBuilderOld> StringBuilderOld::GetVector()const{
	if(m_splitFlag){
		//��؂肪����̂ł���΁Am_vec�����̂܂ܕԂ�
		return m_vec;
	} else{
		//��؂肪�����̂ł����this�݂̂̔z��Ƃ��ĕԂ��B
		return std::vector<StringBuilderOld>{*this};
	}
}

std::vector<std::string> StringBuilderOld::GetStringVector()const{
	if(m_splitFlag){
		//��؂肪����̂ł���΁Am_vec�̊e�v�f��GetString()�̔z���Ԃ�
		std::vector<std::string> v;
		for(const StringBuilderOld &sb:m_vec){
			v.push_back(sb.GetString());
		}
		return v;
	} else{
		//��؂肪�����̂ł����this�݂̂�string�̔z��Ƃ��ĕԂ��B
		return std::vector<std::string>{m_str};
	}
}

void StringBuilderOld::Split(const std::string &str,char spliter,char beginer,char ender,bool deepen){
	if(!m_splitFlag){
		//�������s���Ă��Ȃ����̂ݍs��
		//������
		std::string splitStr;//�������ꂽ������
		int tokenCount=0;//beginer��ǂݍ��񂾌�-ender��ǂݍ��񂾌��B���ɂȂ錸���͖�������B
		bool setExist=false;//���ɏW����ǂݏI��������ǂ����B��؂蕶����ǂݎ�邽�т�false�ɂ���B�uaa(cc,(dd,ee))bb(ff)�v��aa,bb,ff�̂悤�ȃo�O�f�[�^�𖳎����邽�߂ɗp����B
		//�V����StringSpliter�𐶐�����ۂ̏���
		const auto pushFunc=[&]()->void{
			//deepen��false�ł��邩�AsplitStr�ɏW�����܂܂�Ă��Ȃ��Ȃ炱��ȏ�[���Ȃ�Ȃ�
			//�ŏI������true�ł�����肱�̏����͍ċA�I�ɌĂяo����邪�A������(next!=ite)��false�ɂȂ�B�i�����̂��тɏW���O��spliter�ƍŊO��beginer,ender��str������ł��邽�߁j
			m_vec.push_back(StringBuilderOld(splitStr,spliter,beginer,ender,true,deepen && setExist));
			splitStr.clear();
			setExist=false;
		};
		//str�̐擪���疖���܂œǂݍ���
		for(std::string::const_iterator it=str.begin(),ite=str.end();it!=ite;it++){
			if(*it==spliter && tokenCount<=0 && !splitStr.empty()){
				//��؂蕶���ɂ��ǂ蒅������splitStr��p���ĐV����StringSpliter���쐬����
				pushFunc();
			} else if(*it==beginer){
				//�W���̐擪�������������ꍇ�́AtokenCount��1���₷
				if(!setExist){
					//splitStr��M��̂͂܂��W����ǂݍ���ł��Ȃ����̂݁B
					if(tokenCount>0){
						//���ɏW�����ł���ꍇ�́AsplitStr�ɕ�����ǉ�
						splitStr.push_back(beginer);
					} else{
						//�W���O�ł���Ȃ炱������W�����n�܂�̂ŁAsplitStr�ɕ�����ǉ����Ȃ��B�܂��A�uaa(cc,(dd,ee))bb(ff)�v��aa,bb,ff�̂悤�ȃo�O�f�[�^�𖳎����邽�߂�aa�����͍폜����B
						splitStr.clear();
					}
				}
				tokenCount++;
			} else if(*it==ender){
				//�W���̏I�[�������������ꍇ�́AtokenCount��1���炷�B
				tokenCount--;
				if(!setExist){
					if(tokenCount>0){
						//�W���̓ǂݍ��݂��I�����Ă��炸�A�����̊��������W���ǂݍ��݂����݂��Ȃ��ꍇ�͏I�[������splitStr�ɒǉ�����
						splitStr.push_back(ender);
					} else if(tokenCount==0){
						//�W���̓ǂݍ��݂��I�������ꍇ�́A�W���̓ǂݍ��݃t���O�𑀍삷��B
						setExist=true;
					} else{
						//tokenCount�����ɂȂ�悤�ȏI�[�����̓ǂݍ��݂͖�������B
						tokenCount=0;
					}
				}
			} else{
				//����ȊO�̕����́AsplitStr�Ɋi�[����
				if(!setExist){
					splitStr.push_back(*it);
				}
			}
		}
		//�����܂œǂ񂾌�ɁAsplitStr�ɕ����񂪎c���Ă���ꍇ�͂����p����StringSpliter�ɒǉ�����
		pushFunc();
		//�������s��ꂽ���߁A���ꂪ������悤�ɏ���������
		m_splitFlag=true;
		m_str.clear();
	}
}

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
	,m_length(originStrSize-topIndex)//�ő�̕����񐔂��ЂƂ܂��i�[
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
		//���ɂ��Ă��镪�������Ɠ����������������悤�Ƃ��Ă��鎞�́A���������Ɋ֐����I������
		return;
	}
	//������̒������v�Z
	const size_t originSize=m_originStr->size();
	//�e�̏I�[�������m�F
	const size_t parentEnderIndex=m_topIndex+m_length;
	char parentEnderChar;
	if(parentEnderIndex<originSize){
		parentEnderChar=(*m_originStr)[parentEnderIndex];
	} else{
		//���root��StringBuilder�́A�e�̏I�[�����ɃA�N�Z�X�ł��Ȃ��̂ŁA'\0'��n���Ă���
		parentEnderChar='\0';
	}
	//����������ύX
	m_spliter=spliter;
	m_beginer=beginer;
	m_ender=ender;
	//����
	Split(originSize,m_spliter,m_beginer,m_ender,parentEnderChar);
}

void StringBuilder::Split(const size_t originStrSize,
	const char spliter,
	const char beginer,
	const char ender,
	const char parentEnder)
{
	//���������̑O�ɏ�����
	m_vec.clear();
	//m_topIndex���當���𒲂ׂĂ���
	bool subStringBuilderExist=false;
	size_t subStringBuilderTopIndex=m_topIndex;//�q�v�f�ƂȂ�
	for(size_t i=m_topIndex;i<originStrSize;i++){
		const char c=(*m_originStr)[i];
		if(c==m_beginer){
			//����������m_ender���o������܂ł��΂炭��؂蕶���𖳎������
			const StringBuilder subStringBuilder=StringBuilder(m_originStr,originStrSize,m_spliter,m_beginer,m_ender,m_ender,i+1);
			//�ǂݍ��񂾗v�f��ǉ�
			m_vec.push_back(subStringBuilder);
			subStringBuilderExist=true;
			//�ǂݍ��݈ʒu���Y����
			i=subStringBuilder.GetButtomIndex()+1;//subStringBuilder��m_ender�̓ǂݍ��݂͖������ėǂ����߁A+1����
		} else if(c==m_spliter){
			//�����ŋ�؂�
			if(!subStringBuilderExist){
				//�܂��q�v�f��StringBuilder���쐬���Ă��Ȃ��ꍇ�́A�����܂łŗv�f���쐬����
				m_vec.push_back(StringBuilder(m_originStr,subStringBuilderTopIndex,i-subStringBuilderTopIndex));//������i�Ԗڂ̗v�f(m_spliter)�������̂ŁA+1���Ȃ��ėǂ�
			}
			//���̗v�f�̓ǂݎ��̏���
			subStringBuilderExist=false;
			subStringBuilderTopIndex=i+1;
		} else if(c==parentEnder){
			//�e�v�f�̏I�[�n�_��������A����StringBuilder�̓ǂݎ��͏I��
			if(!subStringBuilderExist){
				//�܂��q�v�f��StringBuilder���쐬���Ă��Ȃ��ꍇ�́A�����܂łŗv�f���쐬����
				m_vec.push_back(StringBuilder(m_originStr,subStringBuilderTopIndex,i-subStringBuilderTopIndex));//������i�Ԗڂ̗v�f(m_spliter)�������̂ŁA+1���Ȃ��ėǂ�
			}
			//����StringBuilder�̒������Čv�Z
			m_length=i-m_topIndex;//m_parentEnder�͏����̂ŁA+1�͂��Ȃ��ėǂ�
			//���[�v�E�o
			break;
		}
	}
}

size_t StringBuilder::GetButtomIndex()const{
	return m_topIndex+m_length-1;
}