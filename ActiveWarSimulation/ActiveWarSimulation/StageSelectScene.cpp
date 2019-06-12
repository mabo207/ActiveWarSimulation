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
	//�C���f�b�N�X�̏�����
	if(m_stageInfoVec.empty()){
		m_selectStageIndex=-1;
	} else{
		m_selectStageIndex=0;
	}
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
	if(m_selectStageIndex!=-1){
		//-1�̎��͓ǂݍ���ł���X�e�[�W���Ȃ��̂ōX�V�͂ł��Ȃ�
		const size_t beforeIndex=m_selectStageIndex;//���ʉ��Đ��̉۔���ɗp����
		const size_t infoSize=m_stageInfoVec.size();
		if(keyboard_get(KEY_INPUT_LEFT)==1 || m_beforeStageButton.JudgePressMoment()){
			m_selectStageIndex=(m_selectStageIndex+infoSize-1)%infoSize;
		} else if(keyboard_get(KEY_INPUT_RIGHT)==1 || m_afterStageButton.JudgePressMoment()){
			m_selectStageIndex=(m_selectStageIndex+1)%infoSize;
		}
		if(m_selectStageIndex!=beforeIndex){
			//�ύX������Ό��ʉ��Đ�
			PlaySoundMem(GeneralPurposeResourceManager::selectSound,DX_PLAYTYPE_BACK,TRUE);
		}
	}

	//�J�ڌn�{�^���̏���
	if(m_selectStageIndex!=-1 &&
		(keyboard_get(KEY_INPUT_Z)==1 || m_playButton.JudgePressMoment())
		)
	{
		//�Q�[���v���C�֐i��
		m_nextSceneName=NextSceneName::e_battle;
		PlaySoundMem(GeneralPurposeResourceManager::decideSound,DX_PLAYTYPE_BACK,TRUE);//����̌��ʉ�
		return -1;
	} else if(keyboard_get(KEY_INPUT_X)==1 || m_backButton.JudgePressMoment()){
		//�^�C�g����ʂ֖߂�
		m_nextSceneName=NextSceneName::e_title;
		PlaySoundMem(GeneralPurposeResourceManager::cancelSound,DX_PLAYTYPE_BACK,TRUE);//�߂�̌��ʉ�(��Ɠ����Ƀf�[�^��������̂Ŗ�Ȃ�)
		return -2;
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
	if(m_selectStageIndex!=-1){
		int stageDx,stageDy;
		const int explainX=400;
		GetGraphSize(m_stageInfoVec[m_selectStageIndex].m_mapPic,&stageDx,&stageDy);
		DrawRotaGraph(CommonConstParameter::gameResolutionX/2,CommonConstParameter::gameResolutionY/3,((double)CommonConstParameter::gameResolutionY/2)/stageDy,0.0,m_stageInfoVec[m_selectStageIndex].m_mapPic,TRUE);
		DrawStringCenterBaseToHandle(CommonConstParameter::gameResolutionX/2,CommonConstParameter::gameResolutionY*3/5,m_stageInfoVec[m_selectStageIndex].m_titleName.c_str(),GetColor(255,255,255),m_stageNameFont,false);
		int explainY=CommonConstParameter::gameResolutionY*2/3;
		explainY+=DrawStringNewLineToHandle(explainX,explainY,CommonConstParameter::gameResolutionX-explainX*2,CommonConstParameter::gameResolutionY/4,m_stageInfoVec[m_selectStageIndex].m_explain.c_str(),GetColor(255,255,255),m_explainFont);
		//��Փx���ƂɃ����L���O����`��
		int rankingStrX=200;
		for(const auto &levelData:m_stageInfoVec[m_selectStageIndex].m_rankingVec.levelArray){
			int rankingStrY=explainY;
			//hogehoge(); ��Փx������̕`��
			std::set<ScoreRankingData::PlayerData>::const_iterator it=levelData.playerDataVec.begin();
			const std::set<ScoreRankingData::PlayerData>::const_iterator ite=levelData.playerDataVec.end();
			for(size_t i=0;i<5;i++){
				rankingStrY+=30;
				if(it!=ite){
					DrawStringToHandle(rankingStrX,rankingStrY,(it->name+"        "+std::to_string(it->score)).c_str(),GetColor(255,255,255),m_explainFont);
					it++;
				} else{
					//�f�[�^������Ȃ��ꍇ��---��\��
					DrawStringToHandle(rankingStrX,rankingStrY,"------        -----",GetColor(255,255,255),m_explainFont);
				}
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
			m_stageInfoVec[m_selectStageIndex].m_dirName
			,m_stageInfoVec[m_selectStageIndex].m_titleName
			,StageLevel::e_easy);//��Փx�͂ЂƂ܂��I���ł��Ȃ��悤��
		return CreateFadeOutInScene(thisSharedPtr,battleFactory,15,15);
	}
	return std::shared_ptr<GameScene>();
}
