#ifndef DEF_EDITUNITPARAMETER_H
#define DEF_EDITUNITPARAMETER_H

#include"EditAction.h"
#include"Unit.h"

//���j�b�g��I�����āA�p�����[�^��ҏW�ł���悤�ɂ���ҏW����
class EditUnitParameter:public EditAction{
	//�^�E�񋓑�
public:
	struct EditUnitParameterButton:public EditActionButton{
		EditUnitParameterButton(Vector2D point,Vector2D vec);
		void PushedProcess(EditActionSettings &settings)const;//�{�^���������ꂽ���̏���
	};

	//�萔
	const static size_t editItemNum;//�ҏW���ڐ�(���₵����StageEditor�̕`����ς��Ă�����)

	//�ϐ�
protected:
	size_t m_editIndex;//�ҏW����
	std::shared_ptr<Unit> m_editResult;//�ҏW����

	//�֐�
public:
	EditUnitParameter(int buttonX,int buttonY,int buttonDX,int buttonDY,unsigned int pushedColor);
	~EditUnitParameter();

	void VNonPressEditing(Vector2D point,EditActionSettings &settings)const;//�}�E�X�������Ă��Ȃ������ҏW���s���ҏW�s�ׂɂ��āA���̂悤�ȕҏW�s�ׂ��s��
	void VProcessAction(Vector2D point,EditActionSettings &settings);//�ҏW�s�ׂ����s����
	PosSetKind VGetPosSetKind(const EditActionSettings &settings)const;//�ҏW�󋵂Ɉ˂��ĕԂ��l���قȂ邽��
	size_t GetEditIndex()const{
		return m_editIndex;
	}
	std::shared_ptr<Unit> GetEditResult()const{
		return m_editResult;
	}
	void EditParameter(bool up,bool down,bool left,bool right);
};

#endif // !DEF_EDITUNITPARAMETER_H