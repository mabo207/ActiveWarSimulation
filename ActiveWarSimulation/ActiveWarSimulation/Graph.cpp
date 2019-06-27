#include"DxLib.h"
#include"Graph.h"

//--------------Graph--------------
void Resource::Graph::Init(){
}

Resource::Graph Resource::Graph::LoadInfo::Load()const{
	return Graph(LoadGraph(fileName.c_str()));
}

int Resource::Graph::Load(const std::string &fileName){
	const LoadInfo info(fileName);
	return Load(info);
}

int Resource::Graph::Load(const LoadInfo &info){
	//以下ロード処理
	const auto it=s_graphMap.find(info);
	if(it==s_graphMap.end()){
		//まだロードされていない画像であるなら、新たに読み込む
		const Graph graph(info.Load());
		std::pair<Graph,size_t> inp(graph,1);
		s_graphMap.insert(std::pair<LoadInfo,std::pair<Graph,size_t>>(info,inp));
		return graph.GetHandle();
	} else{
		//既に読み込みされているなら、カウントを増やす
		it->second.second++;
		return it->second.first.GetHandle();
	}
}

int Resource::Graph::Copy(int handle){
	const Graph info(handle);
	//infoと同じデータを探して、参照個数を1増やす
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
	//infoと同じデータを探して、削除処理をする
	bool findFlag=false;//削除処理をしたかどうか
	for(auto it=s_graphMap.begin(),ite=s_graphMap.end();it!=ite;it++){
		if(it->second.first==info){
			if(it->second.second<=1){
				DeleteGraph(info.GetHandle());
				s_graphMap.erase(it);
			} else{
				//参照箇所が2箇所以上だった場合は、カウントを1減らす
				it->second.second--;
			}
			findFlag=true;
			break;
		}
	}
	//データが見つからなかったら、DeleteGraph()をしておく
	if(!findFlag){
		DeleteGraph(handle);
	}
}

void Resource::Graph::Clear(){
	//s_graphMap全てに対して削除処理
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