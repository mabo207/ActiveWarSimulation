#ifndef DEF_STRINGBUILDER_H
#define DEF_STRINGBUILDER_H

#include<string>
#include<vector>
#include<memory>

//������̕����E�������s���N���X
class StringBuilder{
	//�^�E�񋓑�

	//�萔

	//�ϐ�
protected:
	char m_spliter,m_beginer,m_ender;//���ꂼ��A��؂蕶���E�W���̐擪�����E�W���̏I�[����
	//�ȉ��͂ǂ��炩�ɂ����p���Ȃ�
	std::string m_str;//��؂�̂Ȃ��P�̕�����
public:
	std::vector<StringBuilder> m_vec;//��؂�ꂽ����������A�g�[�N����r���ŕς�����悤��StringBuilder�̔z��ɂ��Apublic�ɂ���B
protected:
	//�ǂ���ɒl�������Ă��邩
	bool m_splitFlag;

	//�֐�
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
	//Split()�͑O���m_str���ω����鎖�ɒ���
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
	
	const char m_parentBeginer,m_parentEnder;//���̗v�f�̐e��m_beginer��m_ender
	const char m_spliter,m_beginer,m_ender;//������̋�؂��
	std::vector<NewSB> m_vec;

private:
	NewSB(const std::shared_ptr<const std::string> &originStr
		,const char parentBeginer
		,const char parentEnder
		,const size_t topIndex
		,const size_t length);//�����쐬�p�R���X�g���N�^
	size_t GetButtomIndex()const;

	const std::shared_ptr<const std::string> m_originStr;//�����̌��ƂȂ镶����
	//m_originStr�̂ǂ�����ǂ��܂ł��Q�Ƃ��Ă��邩
	const size_t m_topIndex;//�R���X�g���N�^�쐬���ɏ������\
	size_t m_length;//����������̒���
};

#endif // !DEF_STRINGBUILDER_H