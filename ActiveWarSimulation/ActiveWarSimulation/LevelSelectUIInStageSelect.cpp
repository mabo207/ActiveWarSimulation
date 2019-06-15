#include"LevelSelectUIInStageSelect.h"
#include"DxLib.h"
#include"input.h"
#include"GeneralPurposeResourceManager.h"
#include"CommonConstParameter.h"

namespace {
	const Vector2D levelBoxSize=Vector2D(400.0f,180.0f);
	const std::array<MyPolygon,StageLevel::levelCount> levelBox={
		MyPolygon::CreateRectangle(Vector2D(1440.0f,100.0f),levelBoxSize,Shape::Fix::e_ignore)
		,MyPolygon::CreateRectangle(Vector2D(1440.0f,300.0f),levelBoxSize,Shape::Fix::e_ignore)
		,MyPolygon::CreateRectangle(Vector2D(1440.0f,500.0f),levelBoxSize,Shape::Fix::e_ignore)
		,MyPolygon::CreateRectangle(Vector2D(1440.0f,700.0f),levelBoxSize,Shape::Fix::e_ignore)
	};
}

//-------------------LevelSelectUIInStageSelect----------------------
LevelSelectUIInStageSelect::LevelSelectUIInStageSelect(const std::weak_ptr<ControledData> &controledData
	,const StageInfoInStageSelect &stageInfo
	,int stageNameFont
	,int explainFont
)
	:BaseUIInStageSelect(controledData)
	,m_stageInfo(stageInfo)
	,m_stageNameFont(stageNameFont)
	,m_explainFont(explainFont)
{}

LevelSelectUIInStageSelect::~LevelSelectUIInStageSelect(){}

BaseUIInStageSelect::UpdateResult LevelSelectUIInStageSelect::Update(){
	//���x���I���X�V����
	bool levelUpdate=false;
	bool inLevelBox=false;
	if(!m_controledData.expired()){
		const std::shared_ptr<ControledData> lock=m_controledData.lock();
		const StageLevel beforeLevel=lock->selectLevel;
		if(keyboard_get(KEY_INPUT_UP)==1){
			lock->selectLevel=beforeLevel.Shift(-1);
		} else if(keyboard_get(KEY_INPUT_DOWN)==1){
			lock->selectLevel=beforeLevel.Shift(1);
		} else{
			//�}�E�X�ɂ��X�V����
			const Vector2D mousePos=GetMousePointVector2D();
			for(size_t i=0;i<StageLevel::levelCount;i++){
				if(levelBox[i].VJudgePointInsideShape(mousePos)){
					lock->selectLevel=StageLevel::levelArray[i];
					inLevelBox=true;
					break;
				}
			}
		}
		levelUpdate=!(lock->selectLevel==beforeLevel);//StageLevel�ɂ�!=�����݂��Ȃ�
	}
	if(levelUpdate){
		//���x���X�V���s��ꂽ�ꍇ�͌��ʉ���炷
		PlaySoundMem(GeneralPurposeResourceManager::selectSound,DX_PLAYTYPE_BACK,TRUE);
	} else{
		//���̑���UI����(���x���X�V���s��ꂽ���͂����̏����͂��Ȃ��悤�ɂ���)
		if(keyboard_get(KEY_INPUT_Z)==1
			|| (inLevelBox && mouse_get(MOUSE_INPUT_LEFT)==1))
		{
			//���̑I����
			PlaySoundMem(GeneralPurposeResourceManager::decideSound,DX_PLAYTYPE_BACK,TRUE);
			return UpdateResult::e_gotoBattle;
		} else if(keyboard_get(KEY_INPUT_X)==1 || mouse_get(MOUSE_INPUT_RIGHT)==1){
			//�߂�
			PlaySoundMem(GeneralPurposeResourceManager::cancelSound,DX_PLAYTYPE_BACK,TRUE);
			return UpdateResult::e_gotoStageSelect;
		}
	}

	return UpdateResult::e_notTransition;
}

void LevelSelectUIInStageSelect::Draw()const{
	const int explainFontSize=GetFontSizeToHandle(m_explainFont);
	//�����ɃX�e�[�W��\��
	m_stageInfo.DrawInfo(200,300,m_stageNameFont,m_explainFont);
	//�E���Ƀ��x���I����\��
	for(size_t i=0;i<StageLevel::levelCount;i++){
		const int x=(int)levelBox[i].GetPosition().x,y=(int)levelBox[i].GetPosition().y;
		const StageLevel level=StageLevel::levelArray[i];
		//���x�����̕`��
		DrawStringToHandle(x+5,y+5,level.GetString().c_str(),GetColor(255,255,255),m_explainFont);
		//�����L���O�f�[�^�ꗗ
		const std::map<StageLevel,ScoreRankingData::LevelData>::const_iterator itLevel=m_stageInfo.m_rankingVec.levelMap.find(level);
		size_t counter=0;
		const size_t rankingSize=5;
		const int nameX=10,scoreX=350;
		int rankingY=40;
		if(itLevel!=m_stageInfo.m_rankingVec.levelMap.end()){
			//�����L���O�f�[�^�����݂���ꍇ�̓f�[�^��`��
			std::set<ScoreRankingData::PlayerData>::const_iterator itPlayer;
			for(const ScoreRankingData::PlayerData &data:itLevel->second.playerDataSet){
				//���O
				DrawStringToHandle(x+nameX,y+rankingY,data.name.c_str(),GetColor(255,255,255),m_explainFont);
				//�_��
				DrawStringRightJustifiedToHandle(x+scoreX,y+rankingY,to_string_0d(data.score,7),GetColor(255,255,255),m_explainFont);
				//�ʒu���炵
				rankingY+=explainFontSize;
				//���𑝂₵�āArankingSize�ȏ�̕`��ɂȂ�����`���ł��؂�
				counter++;
				if(counter>=rankingSize){
					break;
				}
			}
		}
		for(;counter<rankingSize;counter++){
			//����Ȃ�����-----��`��
			//���O
			DrawStringToHandle(x+nameX,y+rankingY,"----------",GetColor(255,255,255),m_explainFont);
			//�_��
			DrawStringRightJustifiedToHandle(x+scoreX,y+rankingY,"-------",GetColor(255,255,255),m_explainFont);
			//�ʒu���炵
			rankingY+=explainFontSize;
		}
	}
	//�I�����Ă��郌�x��������
	if(!m_controledData.expired()){
		const std::shared_ptr<ControledData> lock=m_controledData.lock();
		const size_t index=lock->selectLevel.GetIndex();
		if(index<StageLevel::levelCount){
			levelBox[index].Draw(levelBox[index].GetPosition(),Vector2D(),GetColor(255,255,0),FALSE,3.0f);
		}
	}
}