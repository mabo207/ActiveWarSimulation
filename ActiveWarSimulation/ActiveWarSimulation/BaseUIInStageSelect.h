#ifndef DEF_BASEUIINSTAGESELECT_H
#define DEF_BASEUIINSTAGESELECT_H

#include<memory>
#include"StageLevel.h"

//�o�g���}�b�v�I����ʂ�UI�̊��N���X
class BaseUIInStageSelect{
public:
	//UI���Ǘ�����f�[�^
	struct ControledData{
		size_t stageIndex;
		StageLevel selectLevel;
		ControledData(size_t i_stageIndex,StageLevel i_selectLevel)
			:stageIndex(i_stageIndex),selectLevel(i_selectLevel){}
		~ControledData(){}
	};

	//�X�V�����̌���
	enum class UpdateResult{
		e_notTransition	//�J�ڂ��Ȃ�
		,e_nextUI		//����UI�ɑJ��
		,e_gotoTitle	//�^�C�g����ʂɑJ��
		,e_gotoBattle	//�o�g����ʂɑJ��
	};

	virtual ~BaseUIInStageSelect();
	virtual UpdateResult Update()=0;
	virtual void Draw()const=0;//UI�̕\��
	virtual std::shared_ptr<BaseUIInStageSelect> GetNextUI(const std::weak_ptr<ControledData> &controledData)const=0;

protected:
	BaseUIInStageSelect(const std::weak_ptr<ControledData> &controledData);

	const std::weak_ptr<ControledData> m_controledData;
};

#endif // !DEF_BASEUIINSTAGESELECT_H