#ifndef DEF_SUBMISSIONREFLECTIONSCENE_H
#define DEF_SUBMISSIONREFLECTIONSCENE_H

#include"BattleSceneElement.h"
#include"BattleSceneData.h"
#include<optional>
#include"ReflectionWorkBase.h"

//�T�u�~�b�V�����ݒ莞�A���U���g��ʂ̑O�ɐU��Ԃ���s�����
class SubmissionReflectionScene:public BattleSceneElement{
public:
	SubmissionReflectionScene(const std::shared_ptr<BattleSceneData> &battleSceneData,const std::shared_ptr<BattleSceneElement> &clearScene);
	~SubmissionReflectionScene();

private:
	//�^
	struct MinimapDrawInfo{
		//�~�j�}�b�v�`��ɕK�v�ȏ��Q
		MinimapDrawInfo(const std::shared_ptr<const LogElement> &log,Unit::Team::Kind phase);
		~MinimapDrawInfo();
		const std::vector<Unit>& GetUnitList()const{ return unitList; };//unitList��getter
		//unitList�̒��g�̂ǂꂩ���w���|�C���^
		const Unit *pOperateUnit;
		const Unit *pAttackedUnit;//nullptr�Ȃ�A�N�V�������̕`����s��Ȃ�
		//�`�悷�郆�j�b�g�̈ꗗ
	private:
		std::vector<Unit> unitList;//pOperateUnit��pAttackedUnit�����̗v�f���w���Ă���s����A�O������̕ύX��h�������̂�private
	};

	//�֐�
	void DrawResizedMap(int x,int y,const MinimapDrawInfo &minimapInfo)const;//�ȈՊg��k���}�b�v��`�悷��
	void InitReflectionWork();//���t���N�V�������[�N���쐬���鏈��(�`��ʒu�̒����̂��߂̏�񂪂��̃N���X���ɑ��݂���̂ŁAWholeReflectionInfo�N���X�łȂ����̃N���X���ɏ������L�q����)
	//���z�֐��̃I�[�o�[���C�h
	int thisCalculate();//0:���O��ʂɖ߂� 1~END-1:���̏�ʂɐi�� END:�o�g���p�[�g�I��
	void thisDraw()const;
	int UpdateNextScene(int index);//����ʂ̐ݒ�Bindex�ɖ߂肽�����A���ۂɖ߂�邩�͕s���Ȃ̂Ŏ��ۂ̖߂�ꏊ������int�l��Ԃ������B��{�I�ɕԂ�l��index�ɓ������Ȃ�B
	void ReturnProcess();//���̏�ʂɖ߂��Ă������̏���
	//���[�N�쐬�֐�
	std::shared_ptr<ReflectionWork::Base> CreateDrawLineWork()const;
	std::shared_ptr<ReflectionWork::Base> CreateClickWork(const std::function<std::shared_ptr<const Shape>(Vector2D,Vector2D)> &conditionShapeFunc)const;
	std::shared_ptr<ReflectionWork::Base> CreateLineClickWork()const;
	std::shared_ptr<ReflectionWork::Base> CreateAreaClickWork()const;
	std::shared_ptr<ReflectionWork::Base> CreateSelectOneWork()const;

	//�ϐ�
	const std::shared_ptr<BattleSceneData> m_battleSceneData;
	std::shared_ptr<BattleSceneElement> m_clearScene;//�O�N���X�ō쐬����ClearScene���ꎞ�I�ɕۑ����邽�߂̕ϐ�
	std::optional<MinimapDrawInfo> m_goodLogInfo,m_badLogInfo;//���������R���X�g���N�^��{}���ōs�������̂Œx���̕K�v������
	std::shared_ptr<ReflectionWork::Base> m_reflectionWork;//���ݍs���Ă��郊�t���N�V�������[�N

	const int m_operateCursor;//���샆�j�b�g���w���������߂̃}�[�J�[
	const int m_predictNumberFont;
	const int m_predictExplainFont;
};

#endif // !DEF_SUBMISSIONREFLECTIONSCENE_H
