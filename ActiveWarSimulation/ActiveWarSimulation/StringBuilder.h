#ifndef DEF_STRINGBUILDER_H
#define DEF_STRINGBUILDER_H

#include<string>
#include<vector>

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

#endif // !DEF_STRINGBUILDER_H