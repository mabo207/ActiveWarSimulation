#ifndef DEF_GRAPH_H
#define DEF_GRAPH_H

#include<string>
#include<map>
#include<vector>
#include<mutex>
#include<thread>

namespace Resource{
	//�摜�f�[�^���Ǘ�����N���X
	//�`�揈���͏����Ȃ�
	//�X���b�h�Z�[�t�ł͂Ȃ�
	class Graph{
		//�ÓI�����o(Graph���Ǘ�����N���X)
	public:
		static int Load(const std::string &fileName);
		static int Copy(int handle);
		static void Delete(int handle);
		static void Clear();

	private:
		//�摜�����[�h����ۂɕK�v�ȏ��
		struct LoadInfo{
			const std::string fileName;
			explicit LoadInfo(const std::string &i_fileName):fileName(i_fileName){}
			~LoadInfo(){}
			Graph Load()const;
			bool operator<(const LoadInfo &other)const{
				return this->fileName<other.fileName;
			}
			bool operator==(const LoadInfo &other)const{
				return this->fileName==other.fileName;
			}
		};

		static int Load(const LoadInfo &info);

		static std::map<LoadInfo,std::pair<Graph,size_t>> s_graphMap;//�ǂݍ��܂�Ă���O���t�B�b�N�f�[�^�ꗗ

		//�����o
	public:
		int GetHandle()const;
		~Graph();

	private:
		explicit Graph(int i_handle);
		bool operator==(const Graph &otherobj)const{
			return (this->handle==otherobj.handle);
		}

		const int handle;//�O���t�B�b�N�n���h��
	};
}

#endif // !DEF_GRAPH_H
