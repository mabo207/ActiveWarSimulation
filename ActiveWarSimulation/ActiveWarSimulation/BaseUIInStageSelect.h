#ifndef DEF_BASEUIINSTAGESELECT_H
#define DEF_BASEUIINSTAGESELECT_H

#include<memory>
#include"StageLevel.h"

//バトルマップ選択場面のUIの基底クラス
class BaseUIInStageSelect{
public:
	//UIが管理するデータ
	struct ControledData{
		size_t stageIndex;
		StageLevel selectLevel;
		ControledData(size_t i_stageIndex,StageLevel i_selectLevel)
			:stageIndex(i_stageIndex),selectLevel(i_selectLevel){}
		~ControledData(){}
	};

	//更新処理の結果
	enum class UpdateResult{
		e_notTransition		//遷移しない
		,e_gotoTitle		//タイトル場面に遷移
		,e_gotoBattle		//バトル場面に遷移
		,e_gotoStageSelect	//ステージセレクトUIに遷移
		,e_gotoLevelSelect	//レベル選択UIに遷移
	};

	virtual ~BaseUIInStageSelect();
	virtual UpdateResult Update()=0;
	virtual void Draw()const=0;//UIの表示

protected:
	BaseUIInStageSelect(const std::weak_ptr<ControledData> &controledData);

	const std::weak_ptr<ControledData> m_controledData;
};

#endif // !DEF_BASEUIINSTAGESELECT_H