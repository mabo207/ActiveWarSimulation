#ifndef DEF_STAGELEVEL_H
#define DEF_STAGELEVEL_H

#include<array>
#include<unordered_map>
#include<string>

//�X�e�[�W���x���ɂ��Ă̏��
struct StageLevel{
	//StageLevel��v�f��StageLevel::Kind��enum class���ۂ��g����悤�ɍH�v����
public:
	enum Kind{
		//�v�f�ǉ����AlevelCount��levelArray�ϐ����ς��Ă�������
		e_easy
		,e_normal
		,e_hard
		,e_lunatic
	};

	StageLevel()noexcept;
	StageLevel(Kind kind)noexcept;
	bool operator==(const StageLevel &otherobj)const;
	bool operator<(const StageLevel &otherobj)const;
	std::string GetString()const;
	size_t GetIndex()const;
	StageLevel Shift(int shift)const;//levelArray�ɉ����ėv�f������ł��鎞�Aindex�������炵�����̗v�f���~�����I
	static StageLevel CreateFromString(const std::string &str);

	static const size_t levelCount=e_lunatic+1;//���x���̐�
	static const std::array<StageLevel,levelCount> levelArray;//���x���ꗗ

private:
	Kind m_kind;
	//��ނƕ�����̑Ή��\
	static const std::unordered_map<std::string,Kind> kindStringMap;
};

#endif // !DEF_STAGELEVEL_H