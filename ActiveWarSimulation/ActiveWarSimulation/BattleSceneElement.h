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
		static const int start=1;
		enum Kind{
			e_move=start,
			e_attackNormal,
			END
		};
		static const int kindNum=END-start;
		static Kind link(int num){
			if(num>=start && num<=END){
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
	virtual int thisCalculate()=0;//0:���O��ʂɖ߂� 1~END-1:���̏�ʂɐi�� END:�o�g���p�[�g�I��
	virtual void thisDraw()const=0;
	virtual int UpdateNextScene(int index)=0;//����ʂ̐ݒ�Bindex�ɖ߂肽�����A���ۂɖ߂�邩�͕s���Ȃ̂Ŏ��ۂ̖߂�ꏊ������int�l��Ԃ������B��{�I�ɕԂ�l��index�ɓ������Ȃ�B
	virtual void ReturnProcess()=0;//���̏�ʂɖ߂��Ă������̏���

public:
	virtual ~BattleSceneElement();
	int Calculate();//0:���O��ʂɖ߂� 1~END-1:���̏�ʂɐi�� END:�o�g���p�[�g�I��
	void Draw()const;
};


#endif // !DEF_BATTLESCENEELEMENT_H


#pragma once
