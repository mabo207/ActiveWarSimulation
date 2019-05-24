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