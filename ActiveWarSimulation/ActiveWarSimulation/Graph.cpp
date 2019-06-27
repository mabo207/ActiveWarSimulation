#include"DxLib.h"
#include"Graph.h"

//--------------Graph--------------
std::map<Resource::Graph::LoadInfo,std::pair<Resource::Graph,size_t>> Resource::Graph::s_graphMap;
std::vector<Resource::Graph::LoadInfo> Resource::Graph::s_taskVec;
std::mutex Resource::Graph::s_loadMutex;
std::thread::id Resource::Graph::s_loadThreadId;

void Resource::Graph::Init(){
	s_loadThreadId=std::this_thread::get_id();
}

Resource::Graph Resource::Graph::LoadInfo::Load()const{
	return Graph(LoadGraph(fileName.c_str()));
}

int Resource::Graph::Load(const std::string &fileName){
	const LoadInfo info(fileName);
	return Load(info);
}

int Resource::Graph::Load(const LoadInfo &info){
	//�r���I�ɍs����悤��lock�����
	std::lock_guard<std::mutex> lock(s_loadMutex);
	//�ȉ����[�h����
	const auto it=s_graphMap.find(info);
	if(it==s_graphMap.end()){
		//�܂����[�h����Ă��Ȃ��摜�ł���Ȃ�A�V���ɓǂݍ���
		if(s_loadThreadId==std::this_thread::get_id()){
			//���[�h���ł���thread��Load()���Ăяo���ꂽ�Ȃ�ALoadGraph()����
			const Graph graph(info.Load());
			std::pair<Graph,size_t> inp(graph,1);
			s_graphMap.insert(std::pair<LoadInfo,std::pair<Graph,size_t>>(info,inp));
			return graph.GetHandle();
		} else{
			//���[�h���ł��Ȃ�thread��Load()���Ăяo���ꂽ�Ȃ�AtaskVec�ɒ��߂�
			s_taskVec.push_back(info);
			//------------------���Ԃ��΂������c�c�H�H�H-------------------
		}
	} else{
		//���ɓǂݍ��݂���Ă���Ȃ�A�J�E���g�𑝂₷
		it->second.second++;
		return it->second.first.GetHandle();
	}
}

int Resource::Graph::Copy(int handle){
	//�r���I�ɍs����悤��lock�����
	std::lock_guard<std::mutex> lock(s_loadMutex);
	const Graph info(handle);
	//info�Ɠ����f�[�^��T���āA�Q�ƌ���1���₷
	for(auto &data:s_graphMap){
		if(data.second.first==info){
			data.second.second++;
			break;
		}
	}
	return handle;
}

void Resource::Graph::Delete(int handle){
	//�r���I�ɍs����悤��lock�����
	std::lock_guard<std::mutex> lock(s_loadMutex);
	const Graph info(handle);
	//info�Ɠ����f�[�^��T���āA�폜����������
	bool findFlag=false;//�폜�������������ǂ���
	for(auto it=s_graphMap.begin(),ite=s_graphMap.end();it!=ite;it++){
		if(it->second.first==info){
			if(it->second.second<=1){
				//�Q�Ɖӏ���1�ӏ������������ꍇ�́ADeleteGraph()������s_graphMap������폜
				if(s_loadThreadId==std::this_thread::get_id()){
					//���C���X���b�h�łȂ���Delete�ł��Ȃ�
					DeleteGraph(info.GetHandle());
				} else{
					//-----------delete�^�X�N�𒙂߂�-------------
				}
				s_graphMap.erase(it);
			} else{
				//�Q�Ɖӏ���2�ӏ��ȏゾ�����ꍇ�́A�J�E���g��1���炷
				it->second.second--;
			}
			findFlag=true;
			break;
		}
	}
	//�f�[�^��������Ȃ�������ADeleteGraph()�����Ă���
	if(!findFlag){
		if(s_loadThreadId==std::this_thread::get_id()){
			//���C���X���b�h�łȂ���Delete�ł��Ȃ�
			DeleteGraph(handle);
		} else{
			//------------delete�^�X�N�𒙂߂�----------------
		}
	}
}

void Resource::Graph::Clear(){
	//main�X���b�h�ȊO���炱�̏������Ă΂ꂽ�牽�������ɕԂ�
	if(s_loadThreadId!=std::this_thread::get_id()){
		return;
	}
	//�r���I�ɍs����悤��lock�����
	std::lock_guard<std::mutex> lock(s_loadMutex);
	//s_graphMap�S�Ăɑ΂��č폜����
	for(const auto &data:s_graphMap){
		DeleteGraph(data.second.first.GetHandle());
	}
	s_graphMap.clear();
}

int Resource::Graph::GetHandle()const{
	return handle;
}

Resource::Graph::Graph(int i_handle)
	:handle(i_handle)
{}

Resource::Graph::~Graph(){}