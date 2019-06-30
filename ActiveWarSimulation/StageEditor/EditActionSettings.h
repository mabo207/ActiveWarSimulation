#ifndef DEF_EDITACTIONSETTINGS_H
#define DEF_EDITACTIONSETTINGS_H

#include<memory>
#include<vector>
#include"ToolsLib.h"

//�v���g�^�C�v�錾
class EditAction;
class BattleObject;
class PosSetting;
class ShapeFactory;

//�X�e�[�W�ҏW�������Ȃ��ۂ́u�����s�����v�̐ݒ���W�񂵂��N���X
class EditActionSettings {
	//�񋓑́E�^

	//�萔

	//�ϐ�
protected:
	//�ҏW�f�[�^
	std::vector<std::shared_ptr<BattleObject>> m_objects;//�}�b�v��ɐݒu���Ă���I�u�W�F�N�g�ꗗ
	Vector2D m_adjust;//�`��̍��E�␳�l(���ۂ̃X�e�[�W��m_adjust�̍��W�_������ɍ��킹�ăG�f�B�^�ɕ\��)
	std::pair<size_t,Vector2D> m_mouseObjectDepth;//�ҏW�s�ׂ��s���ہA�}�E�X�_������_�Ƃ��Ď��悤�Ȑ}�`�̉��ڂ�ҏW���邩�B�E�N���b�N��depth�𑝂₵�A�E�N���b�N���������ʒu����10px������0�ɂȂ�B

	//�ҏW��蒼���p�f�[�^(�ҏW�f�[�^�ɂ��̕ϐ��g���ĕҏW���Ă��Ύ����)
	std::shared_ptr<BattleObject> m_pOriginObject;//�ҏW�O�̃I�u�W�F�N�g�̃R�s�[

public:
	//�s�ׂɂ��Ẵf�[�^(StageEditor���Ŏ��R�ɕύX�ł���)
	std::shared_ptr<EditAction> m_pEditAction;//�ҏW�s��
	std::shared_ptr<BattleObject> m_pBattleObject;//�ҏW�Ώ�
	std::shared_ptr<ShapeFactory> m_pShapeFactory;//EditPut���ɒu���}�`
	std::shared_ptr<PosSetting> m_pPosSetting;//�ʒu���킹�̎�@

	//�֐�
protected:
	std::vector<std::shared_ptr<BattleObject>>::const_iterator GetMousePointedObject(Vector2D point)const;//point���܂ސ}�`��Ԃ�

public:
	//�R���X�g���N�^�ƃf�X�g���N�^
	EditActionSettings(std::shared_ptr<EditAction> pEditAction,std::shared_ptr<BattleObject> pBattleObject,std::shared_ptr<ShapeFactory> pShapeFactory,std::shared_ptr<PosSetting> pPosSetting);
	~EditActionSettings();
	//�����o�ϐ��̃R�s�[��Ԃ��֐�
	Vector2D GetMAdjust()const{
		return m_adjust;
	}
	const std::vector<std::shared_ptr<BattleObject>> *GetPMObject()const{
		return &m_objects;
	}
	const BattleObject *GetMPOriginObject()const{
		return m_pOriginObject.get();
	}
	//���ݍs�����Ƃ��Ă���ҏW�����s����
	void PracticeEdit(Vector2D point);
	//��ɍs���Ă���ҏW�����s����
	void PracticeNonPressEdit(Vector2D point);
	//���ʂ̊֐�
	void PushScrollBar(float scrollpx,float maxX,float maxY,int mouseX,int mouseY,int leftUpPosX,int leftUpPosY,int mapSizeX,int mapSizeY);//�}�b�v�̃X�N���[���o�[�����������̏���
	void PushScrollBar(Vector2D move);
	void DrawEditButtonPushed()const;//���ݑI������Ă���ҏW�{�^���ɑ΂��ĉ�����Ă���l�q��`�悷��
	void DrawShapeFactoryButtonPushed()const;//���ݑI������Ă���}�`�ݒ�{�^���ɑ΂��ĉ�����Ă���l�q��`�悷��
	void DrawPosSettingButtonPushed()const;//���ݑI������Ă���ʒu�ݒ�{�^���ɑ΂��ĉ�����Ă���l�q��`�悷��
	void DrawPosSettingGuide(int leftUpPosX,int leftUpPosY,int mapSizeX,int mapSizeY)const;//�ʒu�ݒ�K�C�h�̕`��
	void PutObject(Vector2D point);//���݃}�E�X���w���Ă���ʒu�ɃI�u�W�F�N�g��ݒu����
	void RemoveObject(Vector2D point);//���݃}�E�X���w���Ă���ʒu�ɂ���I�u�W�F�N�g����菜��(vector�̓��̕������菜�����)
	void SetEditObject(Vector2D point);//���݃}�E�X���w���Ă���ʒu�ɂ���I�u�W�F�N�g��ҏW�Ώۂɐݒ肷��
	void CancelEditing();//�ҏW���L�����Z������B�ҏW�Ώۂ�ҏW�̕ύX�O�̏�Ԃɖ߂�
	void InitEditObject();//�ҏW�s�ׂ��s���ۂ̏������Bm_pBattleObject��m_pOriginObject��nullptr�ɁB
	const BattleObject *GetMousePointedObjectPointer(Vector2D point)const;//�C�e���[�^��Ԃ�GetMousePointedObject()���N���X�O������g����悤�ɂ���
	void UpdateMouseObjectDepth(const int keyinputright);//m_mouseObjectDepth���X�V����
	void RemoveOriginObject();//m_pOriginObject��m_objects����폜����
	//m_objects�̏�����
	void InitObjects();
	//����f�[�^�̏����o��
	void WriteOutStage(const char *filename)const;
	//���j�b�g�f�[�^�̏����o��
	void WriteOutUnit(const char *filename)const;
	//�X�e�[�W�̓ǂݍ���
	void ReadStage(const char *filename);
	//���j�b�g�̓ǂݍ���
	void ReadUnit(const char *filename);
};

#endif // !DEF_EDITACTIONSETTINGS_H
#pragma once
