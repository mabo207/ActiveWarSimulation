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

	StageLevel()noexcept;
	StageLevel(Kind kind)noexcept;
	bool operator==(const Kind kind)const;
	bool operator<(const StageLevel &otherobj)const;
	std::string GetString()const;
	size_t GetIndex()const;
	static StageLevel CreateFromString(const std::string &str);

	static const size_t levelCount=e_lunatic+1;//���x���̐�

private:
	Kind m_kind;
	//��ނƕ�����̑Ή��\
	static const std::unordered_map<std::string,Kind> kindStringMap;
};

#endif // !DEF_STAGELEVEL_H