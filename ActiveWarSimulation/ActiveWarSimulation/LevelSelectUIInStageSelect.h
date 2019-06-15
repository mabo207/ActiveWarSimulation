#ifndef DEF_LEVELSELECTUIINSTAGESELECT_H
#define DEF_LEVELSELECTUIINSTAGESELECT_H

#include"BaseUIInStageSelect.h"
#include"StageInfoInStageSelect.h"
#include"MyPolygon.h"

//���x���I��������UI
class LevelSelectUIInStageSelect:public BaseUIInStageSelect{
public:
	LevelSelectUIInStageSelect(const std::weak_ptr<ControledData> &controledData,const StageInfoInStageSelect &stageInfo,int explainFont);
	~LevelSelectUIInStageSelect();
	UpdateResult Update();
	void Draw()const;
	std::shared_ptr<BaseUIInStageSelect> GetNextUI(const std::weak_ptr<ControledData> &controledData)const;

private:
	const StageInfoInStageSelect m_stageInfo;

	//�Q�Ƃ��Ă���t�H���g�i�����ō폜�͂��Ȃ��j
	const int m_explainFont;
};

#endif // !DEF_LEVELSELECTUIINSTAGESELECT_H