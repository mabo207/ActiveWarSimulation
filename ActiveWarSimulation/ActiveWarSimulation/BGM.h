#ifndef DEF_RESOURCE_BGM_H
#define DEF_RESOURCE_BGM_H

#include<string>
#include<map>
#include<vector>

namespace Resource{
	//BGM�f�[�^���Ǘ�����N���X
	//�`�揈���͏����Ȃ�
	//�X���b�h�Z�[�t�ł͂Ȃ�
	class BGM{
		//�ÓI�����o(BGM���Ǘ�����N���X)
	public:
		static BGM Load(const std::string &dirName);
		static void Clear();

		//�G���[�l
		static const BGM errorObject;

	private:
		//�摜�����[�h����ۂɕK�v�ȏ��
		struct LoadInfo{
			const std::string infoFileName;
			explicit LoadInfo(const std::string &i_infoFileName):infoFileName(i_infoFileName){}
			~LoadInfo()=default;
			BGM Load()const;
			bool operator<(const LoadInfo &other)const{
				return this->infoFileName<other.infoFileName;
			}
			bool operator==(const LoadInfo &other)const{
				return this->infoFileName==other.infoFileName;
			}
		};

		//LoadInfo��p���ēǂݍ���
		static BGM Load(const LoadInfo &info);

		//�ǂݍ��܂�Ă���BGM�ꗗ
		static std::map<LoadInfo,std::pair<BGM,size_t>> s_bgmMap;

		//�����o
	public:
		int GetHandle()const;//�Đ��ȊO�̏������������Ȃ�n���h�����K�v�ɂȂ������ɗp����
		void SetAndPlay(int playType,int topPositionFlag)const;//SetOption()������ɍĐ�
		void Stop()const;
		bool IsError()const;
		BGM Copy()const;
		void Delete()const;
		~BGM()=default;
		bool operator==(const BGM &otherobj)const;

	private:
		BGM(int i_handle,int i_volume,int i_loopTop,int i_loopBottom);//�Đ����O��volume����ݒ肷�鎖�ɒ���
		void SetOption()const;//volume,loopTop,loopButtom���n���h���ɐݒ肷��

		int handle;//�T�E���h�n���h��
		int volume;
		int loopTop;
		int loopBottom;
	};
}


#endif // !DEF_RESOURCE_BGM_H
