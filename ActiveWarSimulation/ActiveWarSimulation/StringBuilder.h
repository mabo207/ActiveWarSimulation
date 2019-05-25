#ifndef DEF_STRINGBUILDER_H
#define DEF_STRINGBUILDER_H

#include<string>
#include<vector>
#include<memory>

//������̕����E�������s���N���X
//���o�[�W�����ɔ�ׂāAdeepen�ݒ肪�ł��Ȃ����A�v�Z���x��std::vector<>::push_back()�̃R�X�g�������΋��o�[�W������deepen���Ȃ��̂Ɠ������炢�̑��x���o��̂Ŗ��Ȃ��B
//��Split()���A�����郁�����������܂ő傫���Ȃ��̂Œɂ��Ȃ��B
class StringBuilder{
public:
	StringBuilder(const std::string &originStr,
		const char spliter,
		const char beginer,
		const char ender);//�擪�v�f�쐬�p�R���X�g���N�^
	~StringBuilder();
	std::string GetString()const;
	void Split(const char spliter,const char beginer,const char ender);
	
	std::vector<StringBuilder> m_vec;

private:
	StringBuilder(const std::shared_ptr<const std::string> &originStr,
		const size_t topIndex,
		const size_t length);//�����쐬�p�R���X�g���N�^
	StringBuilder(const std::shared_ptr<const std::string> &originStr,
		const size_t originStrSize,
		const char spliter,
		const char beginer,
		const char ender,
		const char parentEnder,
		const size_t topIndex);//�����v�f�쐬�p�R���X�g���N�^
	size_t GetButtomIndex()const;
	void Split(const size_t originStrSize,
		const char spliter,
		const char beginer,
		const char ender,
		const char parentEnder);//�R���X�g���N�^��p

	const std::shared_ptr<const std::string> m_originStr;//�����̌��ƂȂ镶����
	char m_spliter,m_beginer,m_ender;//������̋�؂��(Split()�ŕύX�����\��������̂Ŕ�const)
	//m_originStr�̂ǂ�����ǂ��܂ł��Q�Ƃ��Ă��邩
	const size_t m_topIndex;//�R���X�g���N�^�쐬���ɏ������\
	size_t m_length;//����������̒���
};

#endif // !DEF_STRINGBUILDER_H