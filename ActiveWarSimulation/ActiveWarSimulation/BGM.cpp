#include"DxLib.h"
#include"BGM.h"
#include"FileRead.h"
#include<optional>
#include"StringBuilder.h"

//--------------Resource::BGM::LoadInfo-----------------
std::map<Resource::BGM::LoadInfo,std::pair<Resource::BGM,size_t>> Resource::BGM::s_bgmMap;
const Resource::BGM Resource::BGM::errorObject(-1,-1,-1,-1);

Resource::BGM Resource::BGM::LoadInfo::Load()const{
	int loopTop=-1;
	int loopBottom=-1;
	int volume=255;
	std::optional<std::string> fileName;
	//���[�v�ʒu�̐ݒ�Binfo.txt�Ɂu���[�v�J�n�ʒu�v�u���[�v�I���ʒu�v�u����%�v�u�t�@�C�����v���i�[����Ă���
	StringBuilder builder(FileStrRead((dirName+"info.txt").c_str()),',','(',')');
	for(StringBuilder &sb:builder.m_vec){
		try{
			//std::stoi��p����̂ŗ�O������p��
			if(sb.m_vec.size()>2 && sb.m_vec[0].GetString()=="top"){
				loopTop=std::stoi(sb.m_vec[1].GetString());
			} else if(sb.m_vec.size()>2 && sb.m_vec[0].GetString()=="bottom"){
				loopBottom=std::stoi(sb.m_vec[1].GetString());
			} else if(sb.m_vec.size()>2 && sb.m_vec[0].GetString()=="volume"){
				volume=std::stoi(sb.m_vec[1].GetString());
			} else if(sb.m_vec.size()>2 && sb.m_vec[0].GetString()=="fileName"){
				fileName=dirName+sb.m_vec[1].GetString();
			}
		} catch(const std::invalid_argument &){
			//���l����Ȃ����̂����o�����ꍇ
		} catch(const std::out_of_range &){
			//�\���͈̔͊O�̐��l�����o�����ꍇ
		}
	}
	//�t�@�C�������������ꂽ�ꍇ�Ƃ����łȂ��ꍇ������
	if(fileName){
		return BGM(LoadSoundMem(fileName.value().c_str()),volume,loopTop,loopBottom);
	} else{
		return errorObject;
	}
}

//--------------Resource::BGM----------------------
Resource::BGM Resource::BGM::Load(const std::string &fileName){
	const LoadInfo info(fileName);
	return info.Load();
}

Resource::BGM Resource::BGM::Load(const LoadInfo &info){
	const auto it=s_bgmMap.find(info);
	if(it==s_bgmMap.end()){
		//�܂����[�h����Ă��Ȃ��摜�ł���Ȃ�A�V���ɓǂݍ���
		const BGM bgm=info.Load();
		if(!bgm.IsError()){
			//�G���[�l�łȂ���Βǉ�
			std::pair<BGM,size_t> inp(bgm,1);
			s_bgmMap.insert(std::pair<LoadInfo,std::pair<BGM,size_t>>(info,inp));
		}
		return bgm;
	} else{
		//���ɓǂݍ��݂���Ă���Ȃ�A�J�E���g�𑝂₷
		it->second.second++;
		return it->second.first;
	}
}

void Resource::BGM::Clear(){
	//main�X���b�h�ȊO���炱�̏������Ă΂ꂽ�牽�������ɕԂ�
	//s_bgmMap�S�Ăɑ΂��č폜����
	for(const auto &data:s_bgmMap){
		DeleteSoundMem(data.second.first.GetHandle());
	}
	s_bgmMap.clear();
}

int Resource::BGM::GetHandle()const{
	return handle;
}

void Resource::BGM::SetOption()const{
	if(loopBottom>=0){
		SetLoopStartSamplePosSoundMem(loopBottom,handle);//���[�v���n�܂�ʒu�̐ݒ�A���[�v��Ԃ̌��[�ɂȂ�B
	}
	if(loopTop>=0){
		SetLoopPosSoundMem(loopTop,handle);//���[�v�ʒu�̐ݒ�A���[�v����Ƃǂ��ɔ�Ԃ��Ȃ̂Ń��[�v��Ԃ̑O�[�ɂȂ�B
	}
	ChangeVolumeSoundMem(volume,handle);//�{�����[����0~255�Őݒ�
}

void Resource::BGM::SetAndPlay(int playType,int topPositionFlag)const{
	//�ݒ�
	SetOption();
	//�Đ�
	PlaySoundMem(handle,playType,topPositionFlag);
}

bool Resource::BGM::IsError()const{
	return (handle<0);
}

Resource::BGM Resource::BGM::Copy()const{
	//instance�Ɠ����f�[�^��T���āA�Q�ƌ���1���₷
	for(auto &data:s_bgmMap){
		if(data.second.first==*this){
			data.second.second++;
			break;
		}
	}
	return *this;
}

void Resource::BGM::Delete()const{
	//instance�Ɠ����f�[�^��T���āA�폜����������
	bool findFlag=false;//�폜�������������ǂ���
	for(auto it=s_bgmMap.begin(),ite=s_bgmMap.end();it!=ite;it++){
		if(it->second.first==*this){
			if(it->second.second<=1){
				//�Q�Ɖӏ���1�ӏ������������ꍇ�́ADeleteSoundMem()������s_bgmMap������폜
				DeleteSoundMem(handle);
				s_bgmMap.erase(it);
			} else{
				//�Q�Ɖӏ���2�ӏ��ȏゾ�����ꍇ�́A�J�E���g��1���炷
				it->second.second--;
			}
			findFlag=true;
			break;
		}
	}
	//�f�[�^��������Ȃ�������ADeleteSoundMem()�������Ă���
	if(!findFlag){
		DeleteSoundMem(handle);
	}
}

Resource::BGM::BGM(int i_handle,int i_volume,int i_loopTop,int i_loopBottom)
	:handle(i_handle)
	,volume(i_volume)
	,loopTop(i_loopTop)
	,loopBottom(i_loopBottom)
{}

bool Resource::BGM::operator==(const BGM &otherobj)const{
	return (this->handle==otherobj.handle
		&& this->volume==otherobj.volume
		&& this->loopTop==otherobj.loopTop
		&& this->loopBottom==otherobj.loopBottom);
}
