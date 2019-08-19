#include"DxLib.h"
#include"Graph.h"

//--------------Resource::Graph--------------
std::map<Resource::Graph::LoadInfo,std::pair<Resource::Graph,size_t>> Resource::Graph::s_graphMap;

Resource::Graph Resource::Graph::LoadInfo::Load()const{
	return Graph(LoadGraph(fileName.c_str()));
}

int Resource::Graph::Load(const std::string &fileName){
	const LoadInfo info(fileName);
	return Load(info);
}

int Resource::Graph::Load(const LoadInfo &info){
	const auto it=s_graphMap.find(info);
	if(it==s_graphMap.end()){
		//�܂����[�h����Ă��Ȃ��摜�ł���Ȃ�A�V���ɓǂݍ���
		const Graph graph(info.Load());
		std::pair<Graph,size_t> inp(graph,1);
		s_graphMap.insert(std::pair<LoadInfo,std::pair<Graph,size_t>>(info,inp));
		return graph.GetHandle();
	} else{
		//���ɓǂݍ��݂���Ă���Ȃ�A�J�E���g�𑝂₷
		it->second.second++;
		return it->second.first.GetHandle();
	}
}

int Resource::Graph::Copy(int handle){
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
	const Graph info(handle);
	//info�Ɠ����f�[�^��T���āA�폜����������
	bool findFlag=false;//�폜�������������ǂ���
	for(auto it=s_graphMap.begin(),ite=s_graphMap.end();it!=ite;it++){
		if(it->second.first==info){
			if(it->second.second<=1){
				//�Q�Ɖӏ���1�ӏ������������ꍇ�́ADeleteGraph()������s_bgmMap������폜
				DeleteGraph(info.GetHandle());
				s_graphMap.erase(it);
			} else{
				//�Q�Ɖӏ���2�ӏ��ȏゾ�����ꍇ�́A�J�E���g��1���炷
				it->second.second--;
			}
			findFlag=true;
			break;
		}
	}
	//�f�[�^��������Ȃ�������ADeleteGraph()�������Ă���
	if(!findFlag){
		DeleteGraph(handle);
	}
}

void Resource::Graph::Clear(){
	//main�X���b�h�ȊO���炱�̏������Ă΂ꂽ�牽�������ɕԂ�
	//s_bgmMap�S�Ăɑ΂��č폜����
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