#include"SelectLogScene.h"
#include"input.h"
#include"DxLib.h"
#include"GeneralPurposeResource.h"
#include"CommonConstParameter.h"
#include"WatchLogScene.h"
#include"TitleScene.h"
#include<Windows.h>

//-----------------SelectLogScene-----------------
void SelectLogScene::InitCompletely(){}

void SelectLogScene::Activate(){}

int SelectLogScene::Calculate(){
	//�I���t�@�C���ύX
	const auto recognize=[](int frame){return (frame==1 || (frame>30 && frame%5==0));};
	if(m_selectFileIt!=m_fileNameList.begin() && (recognize(keyboard_get(KEY_INPUT_LEFT)) || recognize(keyboard_get(KEY_INPUT_UP)))){
		m_selectFileIt--;
	} else if(m_selectFileIt!=m_fileNameList.end() && (recognize(keyboard_get(KEY_INPUT_RIGHT)) || recognize(keyboard_get(KEY_INPUT_DOWN)))){
		m_selectFileIt++;
	}
	//��ʑJ��
	if(keyboard_get(KEY_INPUT_Z)==1 || mouse_get(MOUSE_INPUT_LEFT)==1){
		//�I�������t�@�C���̃��O�����ɍs��
		return 1;
	} else if(keyboard_get(KEY_INPUT_X)==1 || mouse_get(MOUSE_INPUT_RIGHT)==1){
		//�^�C�g����ʂɖ߂�
		m_selectFileIt=m_fileNameList.end();//�����w���Ă��Ȃ���Ԃɂ��邱�ƂŁA�^�C�g���ɖ߂鎖���w������
		return 1;
	}
	return 0;
}

void SelectLogScene::Draw()const{
	//�`�������t�@�C�����̏ꏊ�����߂�
	const int maxGap=6;
	std::vector<std::string>::const_iterator it=m_selectFileIt;
	int gap=0;
	for(size_t i=0;i<maxGap;i++){
		if(it!=m_fileNameList.begin()){
			it--;
			gap--;
		} else{
			//m_fileNameList�̐擪���O�ɖ߂�Ȃ��悤�ɂ���
			break;
		}
	}
	//�I�����Ă�����̂̑O��̃t�@�C�������܂Ƃ߂ĕ`��
	for(;gap<=maxGap;gap++){
		const int centerX=CommonConstParameter::gameResolutionX/2,centerY=CommonConstParameter::gameResolutionY/2+gap*60;
		DrawStringCenterBaseToHandle(centerX,centerY,(it!=m_fileNameList.end()?it->c_str():"�^�C�g����ʂɖ߂�"),GetColor(255,255,255),gap==0?GeneralPurposeResource::popLargeFont:GeneralPurposeResource::gothicMiddleFont,true);
		//�\������t�@�C�����̍X�V
		if(it==m_fileNameList.end()){
			break;
		} else{
			it++;
		}
	}
}

SelectLogScene::SelectLogScene(){
	//m_fileNameList�̏������i���s�p�X�ɂ��郍�O�t�@�C�������ׂĊi�[����j
	//�t�H���_������
	char cdir[1024];
	GetCurrentDirectory(1024,cdir);
	const std::string cdir_str(cdir);
	WIN32_FIND_DATA find_dir_data;
	HANDLE hFind=FindFirstFile((cdir_str+"/*").c_str(),&find_dir_data);
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
	do{
		if(hFind!=INVALID_HANDLE_VALUE){
			const std::string filename=GetFileName(find_dir_data);
			if(filename!="." && filename!=".."){
				if(find_dir_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY){
					//�t�H���_�ł���
					//���ɉ������Ȃ�
				} else{
					//�t�@�C���ł���
					bool firstMatch=true;//�擪����v���Ă��邩�ǂ���
					const std::string firstStr="GameLog_";
					if(filename.size()>=firstStr.size()){
						for(size_t i=0,siz=firstStr.size();i<siz;i++){
							if(firstStr[i]!=filename[i]){
								firstMatch=false;
								break;
							}
						}
					} else{
						firstMatch=false;
					}
					if(firstMatch){
						//filename�̐擪��"GameLog_"�Ɉ�v���Ă�����̂����i�[����
						m_fileNameList.push_back(filename);
					}
				}
			}
		}
	} while(FindNextFile(hFind,&find_dir_data));
	//m_selectFileIt�̏�����
	m_selectFileIt=m_fileNameList.begin();
}

SelectLogScene::~SelectLogScene(){}

std::shared_ptr<GameScene> SelectLogScene::VGetNextScene(const std::shared_ptr<GameScene> &thisSharedPtr)const{
	if(m_selectFileIt==m_fileNameList.end()){
		return CreateFadeOutInSceneCompletely(thisSharedPtr,std::shared_ptr<GameScene::SceneFactory>(new TitleScene::TitleSceneFactory()),15,15);
	} else{
		return CreateFadeOutInSceneCompletely(thisSharedPtr,std::shared_ptr<GameScene::SceneFactory>(new WatchLogScene::WatchLogSceneFactory(*m_selectFileIt)),15,15);
	}
}
