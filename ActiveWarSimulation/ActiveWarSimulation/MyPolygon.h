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
	std::vector<std::array<size_t,3>> m_triangleSet;//�O�p�`�����̌��ʂ��i�[����Ă���B��]�E�g��k�����Ă��ύX����K�v�͂Ȃ��B

	//�֐�
protected:
	void DivideTriangle(std::vector<std::array<size_t,3>> *triangleSet)const;
	
public:
	MyPolygon(Vector2D begin,std::vector<Vector2D> points,Fix::Kind fix);
	~MyPolygon();
	std::vector<Vector2D> GetAllEdgeVecs()const;//m_edgeVecs�ɏI�_���n�_�ւ̃x�N�g���������A�S�Ă̕ӂ̃x�N�g����Ԃ��B
	void CalculateAllPointPosition(std::vector<Vector2D> *vertexPos)const;//�S�Ă̒��_�̍��W��vertexPos�Ɋi�[����

	//�������z�֐�
	std::shared_ptr<Shape> VCopy()const;//���e�������Ń|�C���^�̈ʒu�݂̂��قȂ�I�u�W�F�N�g�̃|�C���^��Ԃ�
	void Draw(Vector2D point,Vector2D adjust,unsigned int color,int fillFlag,float lineThickness=1.0f)const;
	//Vector2D CalculatePushVec(const Shape *pShape)const;//pShape��this���d�Ȃ��Ă��邩���肵�A�����o���x�N�g����Ԃ��B�d�Ȃ��Ă��Ȃ��ꍇ��Vector2D(0,0)���Ԃ����B
	bool PushParentObj(const Shape *pShape,ShapeHaving *parentObj,float pushRate)const;//this��pShape���d�Ȃ��Ă��邩���肵�A�d�Ȃ��Ă���ꍇ��parentObj���ړ�����true��Ԃ��B
	bool JudgeInShape(const Shape *pShape)const;//this����pShape�����邩�ǂ����̔���
	Vector2D GetLeftTop()const;//����̍��W�����߂�
	Vector2D GetRightBottom()const;//�E���̍��W�����߂�
	void RecordLatticePointInShape(std::vector<int> &latticeInShape,const size_t xNum,const size_t yNum,const size_t squareWidth,const size_t squareHeight,int index)const;//���̐}�`�����ɂ���i�q�_�̔z���S��index�ɂ���
	//�G�f�B�^�p�̏������z�֐�
	bool VJudgePointInsideShape(Vector2D point)const;//�}�`���ɓ_�����邩�ǂ����̔���ACalculatePushVec��p�����荂���Ɏ����ł���̂Ŋ֐��𕪂���
	Vector2D VGetNearEndpoint(Vector2D point,float capacity)const;//point���[�_�ɋ߂�(������capacity�ȓ�)�ꍇ�A���̒[�_��Ԃ�
	void Resize(Vector2D v);//�}�`��\������x�N�g����p���Đ}�`�̑傫����ύX����
	Vector2D GetRetResize()const;//Resize�̋t�֐��B����r��Resize����ƌ��݂̐}�`�ɂȂ�悤��r��Ԃ��B
	//�ǂݍ��݁E�����o���p���z�֐�
	void WriteOutShape(std::ofstream &ofs)const;//�����o���p�֐�

};


#endif // !DEF_MYPOLYGON_H

