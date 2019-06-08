#ifndef DEF_STAGELEVEL_H
#define DEF_STAGELEVEL_H

#include<unordered_map>
#include<string>

//�X�e�[�W���x���ɂ��Ă̏��
struct StageLevel{
	//StageLevel��v�f��StageLevel::Kind��enum class���ۂ��g����悤�ɍH�v����
public:
	enum Kind{
		//�v�f�ǉ����AlevelCount�ϐ����ς��Ă�������
		e_easy
		,e_normal
		,e_hard
		,e_lunatic
	};

	StageLevel():m_kind(e_easy){}
	StageLevel(Kind kind):m_kind(kind){}
	explicit StageLevel(const std::string &str):StageLevel(kindStringMap.at(str)){}
	bool operator==(const Kind kind)const{
		return m_kind==kind;
	}
	std::string GetString()const;
	size_t GetIndex()const;

	static const size_t levelCount=e_lunatic+1;//���x���̐�

private:
	Kind m_kind;
	//��ނƕ�����̑Ή��\
	static const std::unordered_map<std::string,Kind> kindStringMap;
};



#endif // !DEF_STAGELEVEL_H