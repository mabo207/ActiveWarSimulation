#ifndef DEF_GRAPHICPROGRAMMING_H
#define DEF_GRAPHICPROGRAMMING_H

#include<vector>
#include<string>

//�c�A�����R�}���h�\���{�b�N�X�ƃR�}���h��\������
class VerticalExtendCommandBox{
	//�񋓑́E�^

	//�萔
protected:
	static const int merginDX,merginDY;//�{�b�N�X�̘g�̑���
	static const int strUpMergin,strDownMergin;//�����Ԃ̗]��

	//�ϐ�
protected:
	unsigned int m_commandNum;//�c�ɕ\������R�}���h�̐�
	std::vector<std::string> m_commandString;//�\������R�}���h
	int m_strDX,m_strDY;//�R�}���h�\�������̏c���̒���
	int m_usingFont;//�R�}���h�\���ɗp����t�H���g
	int m_usingFontSize;//�t�H���g�̑傫��
	const bool m_fontCreateFlag;//�t�H���g�������ō�������ǂ���

	//�֐�
protected:

public:
	VerticalExtendCommandBox(const std::vector<std::string> &commandString,int usingFont);
	virtual ~VerticalExtendCommandBox();
	int Draw(int x,int y);
	int GetCommandStrX();
	int GetCommandStrY(unsigned int y);
	int GetCommandBoxDX();
	int GetCommandBoxDY();
};

#endif