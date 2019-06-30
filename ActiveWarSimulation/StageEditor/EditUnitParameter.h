#ifndef DEF_EDITUNITPARAMETER_H
#define DEF_EDITUNITPARAMETER_H

#include"EditAction.h"

//���j�b�g��I�����āA�p�����[�^��ҏW�ł���悤�ɂ���ҏW����
class EditUnitParameter:public EditAction{
	//�^�E�񋓑�
public:
	struct EditUnitParameterButton:public EditActionButton{
		EditUnitParameterButton(Vector2D point,Vector2D vec);
		void PushedProcess(EditActionSettings &settings)const;//�{�^���������ꂽ���̏���
	};

	//�萔

	//�ϐ�
protected:

	//�֐�
public:
	EditUnitParameter(int buttonX,int buttonY,int buttonDX,int buttonDY,unsigned int pushedColor);
	~EditUnitParameter();

	void VNonPressEditing(Vector2D point,EditActionSettings &settings)const;//�}�E�X�������Ă��Ȃ������ҏW���s���ҏW�s�ׂɂ��āA���̂悤�ȕҏW�s�ׂ��s��
	void VProcessAction(Vector2D point,EditActionSettings &settings);//�ҏW�s�ׂ����s����
	PosSetKind VGetPosSetKind(const EditActionSettings &settings)const;//�ҏW�󋵂Ɉ˂��ĕԂ��l���قȂ邽��
};

#endif // !DEF_EDITUNITPARAMETER_H