#ifndef DEF_MYPOLYGON_H
#define DEF_MYPOLYGON_H

#include<vector>
#include"Edge.h"

//���p�`�������}�`
class MyPolygon:public Shape{
	//�񋓑́E�^
public:

	//�萔

	//�ϐ�
protected:
	std::vector<Vector2D> m_edgeVecs;//�ӂ�\���x�N�g���Bm_position��m_edgeVecs�̐擪������Z���Ă����ƁA�S�Ă̓_�̍��W�����߂���B�I�[�_->m_position�̃x�N�g���͎����Ȃ��߁A�_�̐����v�f����1���Ȃ��B

	//�֐�
protected:
	std::vector<Vector2D> GetAllEdgeVecs()const;//m_edgeVecs�ɏI�_���n�_�ւ̃x�N�g���������A�S�Ă̕ӂ̃x�N�g����Ԃ��B

public:
	MyPolygon(Vector2D begin,std::vector<Vector2D> points,Fix::Kind fix);
	~MyPolygon();

	//�������z�֐�
	std::shared_ptr<Shape> VCopy()const;//���e�������Ń|�C���^�̈ʒu�݂̂��قȂ�I�u�W�F�N�g�̃|�C���^��Ԃ�
	void Draw(Vector2D point,Vector2D adjust,unsigned int color,int fillFlag,float lineThickness=1.0f)const;
	Vector2D CalculatePushVec(const Shape *pShape)const;//pShape��this���d�Ȃ��Ă��邩���肵�A�����o���x�N�g����Ԃ��B�d�Ȃ��Ă��Ȃ��ꍇ��Vector2D(0,0)���Ԃ����B
	Vector2D GetLeftTop()const;//����̍��W�����߂�
	Vector2D GetRightBottom()const;//�E���̍��W�����߂�
	//�G�f�B�^�p�̏������z�֐�
	bool VJudgePointInsideShape(Vector2D point)const;//�}�`���ɓ_�����邩�ǂ����̔���ACalculatePushVec��p�����荂���Ɏ����ł���̂Ŋ֐��𕪂���
	Vector2D VGetNearEndpoint(Vector2D point,float capacity)const;//point���[�_�ɋ߂�(������capacity�ȓ�)�ꍇ�A���̒[�_��Ԃ�
	void Resize(Vector2D v);//�}�`��\������x�N�g����p���Đ}�`�̑傫����ύX����
	Vector2D GetRetResize()const;//Resize�̋t�֐��B����r��Resize����ƌ��݂̐}�`�ɂȂ�悤��r��Ԃ��B
	//�ǂݍ��݁E�����o���p���z�֐�
	void WriteOutShape(std::ofstream &ofs)const;//�����o���p�֐�

};


#endif // !DEF_MYPOLYGON_H

