#ifndef DEF_TERRAIN_H
#define DEF_TERRAIN_H

//�n�`�E��Q���ɂ��Ă̏��̐錾
#include"BattleObject.h"

class Terrain :public BattleObject {
	//�^�E�񋓑�

	//�萔

	//�ϐ�
protected:
	unsigned int m_color;//�n�`�̓h��Ԃ��̐F(���̂����e�N�X�`�����ɕς��Ǝv����)
	bool m_breakable;//�j��ł��邩�ǂ���

	//�֐�
protected:
	void WriteOutObjectPeculiarInfo(std::ofstream &ofs)const;//�I�u�W�F�N�g���L�̏���Ԃ�

public:
	Terrain(const std::shared_ptr<Shape> hitJudgeShape,int gHandle,unsigned int color,bool breakable);
	~Terrain();
	void VDraw(Vector2D point,Vector2D adjust)const;//�`�揈��
	void VHitProcess(const BattleObject *potherobj);
	std::shared_ptr<BattleObject> VCopy()const;
};


#endif // !DEF_TERRAIN_H
