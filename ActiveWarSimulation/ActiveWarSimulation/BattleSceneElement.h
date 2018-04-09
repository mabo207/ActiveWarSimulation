#ifndef DEF_BATTLESCENEELEMENT_H
#define DEF_BATTLESCENEELEMENT_H

#include"GameScene.h"
#include<memory>

//�o�g���p�[�g�̊e��ʂɗp���鉼�z���N���X
//��ʏ��ƕ��ՓI�ȑJ�ڃA���S���Y���������Ă���
class BattleSceneElement:public GameScene{
	//�^�E�񋓑�
public:
	struct SceneKind{
		enum Kind{
			e_move,
			e_attackNormal,
			END
		};
		static Kind link(int num){
			if(num>=0 && num<=END){
				return static_cast<Kind>(num);
			}
			return END;
		}
	};

	//�萔

	//�ϐ�
private:
	const SceneKind::Kind m_sceneKind;
protected:
	std::shared_ptr<BattleSceneElement> m_nextScene;

	//�֐�
protected:
	BattleSceneElement(SceneKind::Kind sceneKind);
	SceneKind::Kind GetSceneKind()const{
		return m_sceneKind;
	}
	//virtual int thisCalculate()=0;
	//virtual void thisDraw()const=0;

public:
	virtual ~BattleSceneElement();
	//int Calculate();
	//void Draw();
};


#endif // !DEF_BATTLESCENEELEMENT_H


#pragma once
