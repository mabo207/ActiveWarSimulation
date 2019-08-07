#define NOMINMAX	//std::max()�̏Փ˂̉��
#include"StageSelectScene.h"
#include"DxLib.h"
#include"GraphicControl.h"
#include"FileRead.h"
#include<Windows.h>
#include"CommonConstParameter.h"
#include"GeneralPurposeResource.h"
#include"FilePath.h"
#include"BGMManager.h"
#include<algorithm>

#include"BattleScene.h"
#include"TitleScene.h"

#include"StageSelectUIInStageSelect.h"
#include"LevelSelectUIInStageSelect.h"

namespace {
	const int backButtonWidth=80;
	const int backButtonHeight=80;
	const int backButtonX=40;
	const int backButtonY=CommonConstParameter::gameResolutionY-backButtonX-backButtonHeight;
}

//----------------------StageSelectScene------------------
std::shared_ptr<GameScene> StageSelectScene::StageSelectSceneFactory::CreateIncompleteScene()const{
	return std::shared_ptr<GameScene>(new StageSelectScene());
}

StageSelectScene::StageSelectScene()
	:m_nextSceneName(NextSceneName::e_title)
	,m_backPic(LoadGraphEX(FilePath::graphicDir+"nonfree/stageSelectBack.png"))
	,m_backButton(backButtonX,backButtonY,backButtonWidth,backButtonHeight,LoadGraphEX(FilePath::graphicDir+"backButton.png"))
	,m_stageNameFont(CreateFontToHandleEX("���C���I",32,2,-1))
	,m_explainFont(CreateFontToHandleEX("���C���I",24,1,-1))
	,m_bgm(Resource::BGM::Load(FilePath::bgmDir+"nonfree/title/"))
	,m_uiControledData(new BaseUIInStageSelect::ControledData(0,StageLevel::e_easy))
{
	//�t�H���_������
	const std::vector<std::string> dirNameVec={"1_1","1_2","1_3","1_4","2_1","2_2","2_3","2_4","3_1","3_2","3_3","3_4","4_1","4_2","4_3","4_4","4_5"};
	//�e�t�H���_������Am_stageInfoFactoryMap���\�z���Ă���
	for(const std::string &dirName:dirNameVec){
		m_stageInfoFactoryMap.insert(std::make_pair(dirName,LoadGraphEX((FilePath::stageDir+dirName+"/nonfree/minimap.png").c_str())));
	}
}

StageSelectScene::~StageSelectScene(){
	//�O���t�B�b�N�̉��
	DeleteGraphEX(m_backPic);
	for(const auto pair:m_stageInfoFactoryMap){
		DeleteGraphEX(pair.second);
	}
	//�t�H���g�̉��
	DeleteFontToHandleEX(m_stageNameFont);
	DeleteFontToHandleEX(m_explainFont);
	//���̉��
	m_bgm.Delete();
}

void StageSelectScene::InitCompletely(){
	//�X�R�A�f�[�^�̓ǂݍ���
	const ScoreRankingData rankingData;
	//m_stageInfoFactoryMap����t�H���_�������s���Ȃ���AStageInfoInStageSelect���\�����Ă���
	for(const std::pair<std::string,int> &info:m_stageInfoFactoryMap){
		try{
			m_stageInfoVec.push_back(StageInfoInStageSelect(
				info.second
				,info.first
				,FileStrRead((FilePath::stageDir+info.first+"/explain.txt").c_str())
				,rankingData
			));
		} catch(const FileOpenFailedException &){
			//���̏�ʂł́A�t�@�C�����J���̂Ɏ��s���邱�Ƃ����e����B
			//�t�@�C�����J���̂Ɏ��s�����ꍇ�́Ainsert()���s��Ȃ������ł��̂܂܏����𑱂���
		}
	}
}

void StageSelectScene::Activate(){
	//UI�̍쐬
	m_ui=std::shared_ptr<StageSelectUIInStageSelect>(new StageSelectUIInStageSelect(m_uiControledData,m_backButton,m_stageInfoVec,m_stageNameFont,m_explainFont));
	//bgm�Đ�
	if(BGMManager::s_instance.has_value()){
		BGMManager::s_instance->PlayWithCopy(m_bgm);
	}
}

int StageSelectScene::Calculate(){
	//�\�����̈ʒu�X�V
	m_uiControledData->stageInfoPos.Update();
	m_uiControledData->levelInfoPos.Update();
	//�I���X�e�[�W�̍X�V
	const auto updateResult=m_ui->Update();
	if(updateResult==BaseUIInStageSelect::UpdateResult::e_gotoBattle){
		//�Q�[���v���C�֐i��
		m_nextSceneName=NextSceneName::e_battle;
		return -1;
	} else if(updateResult==BaseUIInStageSelect::UpdateResult::e_gotoTitle){
		//�^�C�g����ʂ֖߂�
		m_nextSceneName=NextSceneName::e_title;
		return -1;
	} else if(updateResult==BaseUIInStageSelect::UpdateResult::e_gotoStageSelect){
		//�X�e�[�W�Z���N�g��UI�J��
		m_ui=std::shared_ptr<BaseUIInStageSelect>(new StageSelectUIInStageSelect(m_uiControledData,m_backButton,m_stageInfoVec,m_stageNameFont,m_explainFont));
	} else if(updateResult==BaseUIInStageSelect::UpdateResult::e_gotoLevelSelect){
		//���x���Z���N�g��UI�J��
		m_ui=std::shared_ptr<BaseUIInStageSelect>(new LevelSelectUIInStageSelect(m_uiControledData,m_backButton,m_stageInfoVec[m_uiControledData->stageIndex],m_stageNameFont,m_explainFont));
	}

	return 0;
}

void StageSelectScene::Draw()const{
	//�w�i�̕`��
	DrawGraph(0,0,m_backPic,TRUE);
	//�X�e�[�W�ꗗ�ƌo�H�̕`��
	for(size_t i=0,siz=m_stageInfoVec.size();i<siz;i++){
		//�X�e�[�W�̌o�H�̕`��
		if(i+1<siz){
			//���̃X�e�[�W�Ƃ̋��������߂�
			const Vector2D dir=m_stageInfoVec[i+1].m_pos-m_stageInfoVec[i].m_pos;
			const float dist=dir.size();
			//���ۂ�`�悷�邩���߂�
			const float circleSize=10.0f;
			const int divideNum=std::max((int)(dist/(circleSize*6.0f)),2);//�Œ�1�͕`��AdivideNum�͌��Ԃ̌��Ȃ̂�(divideNum-1)�̊ۂ����Ƃ��ĕ`�悳���
			//�`��
			for(int j=1;j<divideNum;j++){
				//�����̒��g��divideNum>0�̎��̂ݏ��������̂ŁAdivideNum��0�`�F�b�N�͕K�v�Ȃ�
				const Vector2D pos=m_stageInfoVec[i].m_pos+dir*((float)j)/((float)divideNum);
				DrawCircleAA(pos.x,pos.y,circleSize,10,GetColor(64,64,128),TRUE);
				DrawCircleAA(pos.x,pos.y,circleSize*0.6f,10,GetColor(64,128,196),TRUE);
			}
		}
		//�X�e�[�W�̈ʒu�̕`��
		DrawCircleAA(m_stageInfoVec[i].m_pos.x,m_stageInfoVec[i].m_pos.y,30,10,GetColor(64,64,255),TRUE);
		DrawCircleAA(m_stageInfoVec[i].m_pos.x,m_stageInfoVec[i].m_pos.y,20,10,GetColor(128,196,255),TRUE);
	}
	//UI�̕`��
	m_ui->Draw();
	//�߂�{�^���̕`��
	m_backButton.DrawButton();
}

std::shared_ptr<GameScene> StageSelectScene::VGetNextScene(const std::shared_ptr<GameScene> &thisSharedPtr)const{
	if(m_nextSceneName==NextSceneName::e_title){
		const auto titleFactory=std::make_shared<TitleScene::TitleSceneFactory>();
		return CreateFadeOutInSceneCompletely(thisSharedPtr,titleFactory,15,15);
	} else if(m_nextSceneName==NextSceneName::e_battle){
		const std::shared_ptr<GameScene::SceneFactory> battleFactory=std::make_shared<BattleScene::BattleSceneFactory>(
			m_stageInfoVec[m_uiControledData->stageIndex].m_dirName
			,m_stageInfoVec[m_uiControledData->stageIndex].m_titleName
			,m_uiControledData->selectLevel);
		return CreateFadeOutInSceneCompletely(thisSharedPtr,battleFactory,15,15);
	}
	return std::shared_ptr<GameScene>();
}
