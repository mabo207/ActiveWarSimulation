#ifndef DEF_POLYGONFACTORY_H
#define DEF_POLYGONFACTORY_H

#include"ShapeFactory.h"

class PolygonFactory:public ShapeFactory{
	//�^�E�񋓑�
public:
	struct PolygonFactoryButton:public ShapeFactoryButton{
		PolygonFactoryButton(Vector2D point,Vector2D vec);
		void ButtonDraw(int font,int fillFlag)const;//�{�^���̕`��
		void PushedProcess(EditActionSettings &settings)const;//�{�^���������ꂽ���ɍs������
	};
	//�萔
protected:
	static const float pointCapacity;

	//�ϐ�
protected:
	//���p�`�\���̂��߂̕ϐ�
	std::vector<Vector2D> m_pointVec;

	//�֐�
protected:

public:
	PolygonFactory(Vector2D buttonPos,Vector2D buttonSize,unsigned int lightcolor);
	~PolygonFactory();

	std::shared_ptr<BattleObject> CreateObject(Vector2D point)const;
	EditPut::PosSetKind VPutAction(EditPut::PosSetKind pskind,Vector2D point,EditActionSettings &settings);
	void VPutNotPressAction(EditPut::PosSetKind pskind,Vector2D point,EditActionSettings &settings)const;//EditPut�̔�N���b�N�̍ۂɍs������(�f�t�H���g��Resize()�����Ă�������)
	void FactoryDraw(const Vector2D adjust,const EditActionSettings &settings)const;//�}�`�z�u�����₷���悤�ȕ`�������B
};

#endif // !DEF_POLYGONFACTORY_H
