#ifndef DEF_EDITPUT_H
#define DEF_EDITPUT_H

#include"EditAction.h"

//��ʂɃI�u�W�F�N�g��u���s�ׂ��L�q����N���X
class EditPut :public EditAction {
	//�^�E�񋓑�
public:
	struct EditPutButton:public EditActionButton{
		EditPutButton(Vector2D point,Vector2D vec);
		void PushedProcess(EditActionSettings &settings)const;//�{�^���������ꂽ���̏���
	};

	//�萔

	//�ϐ�
protected:
	PosSetKind m_setKind;//���ݐ}�`�̈ʒu�����߂Ă��邩�傫�������߂Ă��邩

	//�֐�
public:
	EditPut(int buttonX,int buttonY,int buttonDX,int buttonDY,unsigned int pushedColor);
	~EditPut() {}

	void VNonPressEditing(Vector2D point,EditActionSettings &settings)const;//�}�E�X�������Ă��Ȃ������ҏW���s���ҏW�s�ׂɂ��āA���̂悤�ȕҏW�s�ׂ��s��
	void VProcessAction(Vector2D point,EditActionSettings &settings);//�ҏW�s�ׂ����s����
	PosSetKind VGetPosSetKind(const EditActionSettings &settings)const;//�ҏW�󋵂Ɉ˂��ĕԂ��l���قȂ邽��
	void ActionDraw(const Vector2D adjust,const EditActionSettings &settings)const;//�ҏW�����₷���Ȃ�悤�ɐ}�`�`�������B
};

#endif // !DEF_EDITPUT_H
#pragma once
