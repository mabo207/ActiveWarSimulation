#include"StageSelectScene.h"
#include"DxLib.h"
#include"GraphicControl.h"
#include"FileRead.h"
#include<Windows.h>
#include"CommonConstParameter.h"
#include"GeneralPurposeResourceManager.h"
#include"StageInfoReader.h"
#include"FilePath.h"

#include"BattleScene.h"
#include"TitleScene.h"

#include"StageSelectUIInStageSelect.h"

//----------------------StageSelectScene------------------
StageSelectScene::StageInfo::StageInfo(const int mapPic,const std::string &dirName,const std::string &explain,const ScoreRankingData &rankingData)
	:m_mapPic(mapPic)
	,m_dirName(dirName)
	,m_explain(explain)
	,m_rankingVec(rankingData.GetStageScoreData(dirName))
{
	//�X�e�[�W���̓ǂݎ��
	const StageInfoReader reader(dirName);
	m_titleName=reader.GetTitleName();
}

StageSelectScene::StageInfo::~StageInfo(){
	//DeleteGraphEX(m_mapPic);
}

std::shared_ptr<GameScene> StageSelectScene::StageSelectSceneFactory::CreateScene()const{
	return std::shared_ptr<GameScene>(new StageSelectScene());
}

StageSelectScene::StageSelectScene()
	:m_nextSceneName(NextSceneName::e_title)
	,m_beforeStageButton(100,300,LoadGraphEX(FilePath::graphicDir+"beforeItem.png"))
	,m_afterStageButton(1770,300,LoadGraphEX(FilePath::graphicDir+"afterItem.png"))
	,m_backButton(60,940,LoadGraphEX(FilePath::graphicDir+"backButton.png"))
	,m_playButton(900,940,LoadGraphEX(FilePath::graphicDir+"combatButton.png"))
	,m_backPic(LoadGraphEX(FilePath::graphicDir+"nonfree/titleScene.png"))
	,m_stageNameFont(CreateFontToHandleEX("���C���I",32,2,-1))
	,m_explainFont(CreateFontToHandleEX("���C���I",24,1,-1))
	,m_uiControledData(new BaseUIInStageSelect::ControledData(0,StageLevel::e_easy))
{
	//�X�R�A�f�[�^�̓ǂݍ���
	const ScoreRankingData rankingData;
	//�t�H���_������
	char cdir[1024];
	GetCurrentDirectory(1024,cdir);
	const std::string cdir_str(cdir);
	WIN32_FIND_DATA find_dir_data;
	HANDLE hFind=FindFirstFile((cdir_str+"/"+FilePath::stageDir+"/*").c_str(),&find_dir_data);
	auto GetFileName=[](WIN32_FIND_DATA data){
		//find_dir_data�̃t�@�C������std::string�^�ɕϊ�����֐�
		std::string s;
		s.reserve(260);//�z����������A�t�@�C�����̒�����260�����Ȃ��炵��
		for(int i=0;i<260;i++){
			if(data.cFileName[i]!='\0'){
				s.push_back(data.cFileName[i]);
			} else{
				//s.push_back('\0');//����͓���Ă͂����Ȃ��Bstring��r�̎��ɂ͎ז��ɂȂ�B
				//��Fchar[] c="."�Ƃ���ƁAc.size=2,c[0]='.',c[1]='\0'�Bstd::string c="."�Ƃ���ƁAc.size=1,c[0]='.'�Bchar[]���ƏI�[�����͔F������Astring���ƏI�[�����͖��������B
				break;
			}
		}
		return s;
	};
	std::vector<std::string> dirNameVec;
	do{
		if(hFind!=INVALID_HANDLE_VALUE){
			const std::string filename=GetFileName(find_dir_data);
			if(filename!="." && filename!=".."){
				if(find_dir_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY){
					//�t�H���_�ł���
					dirNameVec.push_back(filename);
				} else{
					//�t�@�C���ł���
					//���ɉ������Ȃ�
				}
			}
		}
	} while(FindNextFile(hFind,&find_dir_data));
	//�e�t�H���_�̒��g���������āAStageInfo���\�����Ă���
	for(const std::string &dirName:dirNameVec){
		if(dirName!="demo" && dirName!="tutorial" && dirName!="tutorial_2"){
			m_stageInfoVec.push_back(StageInfo(
				LoadGraphEX((FilePath::stageDir+dirName+"/nonfree/minimap.png").c_str())
				,dirName
				,FileStrRead((FilePath::stageDir+dirName+"/explain.txt").c_str())
				,rankingData
			));
		}
	}
	//UI�̍쐬
	m_ui=std::shared_ptr<StageSelectUIInStageSelect>(new StageSelectUIInStageSelect(m_uiControledData,m_stageInfoVec.size()));
}

StageSelectScene::~StageSelectScene(){
	//�O���t�B�b�N�̉��
	DeleteGraphEX(m_backPic);
	for(const StageInfo &info:m_stageInfoVec){
		DeleteGraphEX(info.m_mapPic);
	}
	//�t�H���g�̉��
	DeleteFontToHandleEX(m_stageNameFont);
	DeleteFontToHandleEX(m_explainFont);
	//���̉��

}

int StageSelectScene::Calculate(){
	//�I���X�e�[�W�̍X�V
	const auto updateResult=m_ui->Update();
	if(updateResult==BaseUIInStageSelect::UpdateResult::e_nextUI){
		m_ui=m_ui->GetNextUI(m_uiControledData);
	} else if(updateResult==BaseUIInStageSelect::UpdateResult::e_gotoBattle){
		//�Q�[���v���C�֐i��
		m_nextSceneName=NextSceneName::e_battle;
		return -1;
	} else if(updateResult==BaseUIInStageSelect::UpdateResult::e_gotoTitle){
		//�^�C�g����ʂ֖߂�
		m_nextSceneName=NextSceneName::e_title;
		return -1;
	}

	return 0;
}

void StageSelectScene::Draw()const{
	//�w�i�̕`��
	DrawGraph(0,0,m_backPic,TRUE);
	//�Â߂ɕ`�悷�邽�߂ɁA�ォ�獕�����`�𔼓����ŏd�˂�
	int mode,pal;
	GetDrawBlendMode(&mode,&pal);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA,128);
	DrawBox(0,0,CommonConstParameter::gameResolutionX,CommonConstParameter::gameResolutionY,GetColor(0,0,0),TRUE);
	SetDrawBlendMode(mode,pal);
	//�{�^���̕`��
	m_beforeStageButton.DrawButton();
	m_afterStageButton.DrawButton();
	m_backButton.DrawButton();
	m_playButton.DrawButton();
	//�X�e�[�W���̕`��
	if(m_uiControledData->stageIndex!=-1){
		int stageDx,stageDy;
		const int explainX=400;
		GetGraphSize(m_stageInfoVec[m_uiControledData->stageIndex].m_mapPic,&stageDx,&stageDy);
		DrawRotaGraph(CommonConstParameter::gameResolutionX/2,CommonConstParameter::gameResolutionY/3,((double)CommonConstParameter::gameResolutionY/2)/stageDy,0.0,m_stageInfoVec[m_uiControledData->stageIndex].m_mapPic,TRUE);
		DrawStringCenterBaseToHandle(CommonConstParameter::gameResolutionX/2,CommonConstParameter::gameResolutionY*3/5,m_stageInfoVec[m_uiControledData->stageIndex].m_titleName.c_str(),GetColor(255,255,255),m_stageNameFont,false);
		int explainY=CommonConstParameter::gameResolutionY*2/3;
		explainY+=DrawStringNewLineToHandle(explainX,explainY,CommonConstParameter::gameResolutionX-explainX*2,CommonConstParameter::gameResolutionY/4,m_stageInfoVec[m_uiControledData->stageIndex].m_explain.c_str(),GetColor(255,255,255),m_explainFont);
		//��Փx���ƂɃ����L���O����`��
		int rankingStrX=200;
		for(const StageLevel &level:StageLevel::levelArray){
			int rankingStrY=explainY;
			const int rankingStrDY=GetFontSizeToHandle(m_explainFont);
			//��Փx������̕`��
			DrawStringToHandle(rankingStrX,rankingStrY,level.GetString().c_str(),GetColor(255,255,255),m_explainFont);
			rankingStrY+=rankingStrDY;
			size_t drawCount=0;
			const size_t totalDrawCount=5;
			//�X�R�A�f�[�^�̕`��
			const std::map<StageLevel,ScoreRankingData::LevelData>::const_iterator itLevel=m_stageInfoVec[m_uiControledData->stageIndex].m_rankingVec.levelMap.find(level);
			if(itLevel!=m_stageInfoVec[m_uiControledData->stageIndex].m_rankingVec.levelMap.end()){
				std::set<ScoreRankingData::PlayerData>::const_iterator it=itLevel->second.playerDataSet.begin();
				const std::set<ScoreRankingData::PlayerData>::const_iterator ite=itLevel->second.playerDataSet.end();
				for(;drawCount<totalDrawCount;drawCount++){
					if(it!=ite){
						DrawStringToHandle(rankingStrX,rankingStrY,(it->name+"        "+std::to_string(it->score)).c_str(),GetColor(255,255,255),m_explainFont);
						it++;
					} else{
						break;
					}
					rankingStrY+=rankingStrDY;
				}
			}
			//�X�R�A�f�[�^������Ȃ������̕`��
			for(;drawCount<totalDrawCount;drawCount++){
				//�f�[�^������Ȃ��ꍇ��---��\��
				DrawStringToHandle(rankingStrX,rankingStrY,"------        -----",GetColor(255,255,255),m_explainFont);
				rankingStrY+=rankingStrDY;
			}
			rankingStrX+=400;
		}
	}
}

std::shared_ptr<GameScene> StageSelectScene::VGetNextScene(const std::shared_ptr<GameScene> &thisSharedPtr)const{
	if(m_nextSceneName==NextSceneName::e_title){
		const auto titleFactory=std::make_shared<TitleScene::TitleSceneFactory>();
		return CreateFadeOutInScene(thisSharedPtr,titleFactory,15,15);
	} else if(m_nextSceneName==NextSceneName::e_battle){
		const std::shared_ptr<GameScene::SceneFactory> battleFactory=std::make_shared<BattleScene::BattleSceneFactory>(
			m_stageInfoVec[m_uiControledData->stageIndex].m_dirName
			,m_stageInfoVec[m_uiControledData->stageIndex].m_titleName
			,StageLevel::e_easy);//��Փx�͂ЂƂ܂��I���ł��Ȃ��悤��
		return CreateFadeOutInScene(thisSharedPtr,battleFactory,15,15);
	}
	return std::shared_ptr<GameScene>();
}
