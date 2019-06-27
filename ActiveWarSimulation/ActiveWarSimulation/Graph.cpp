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
	//排他的に行われるようにlockを取る
	std::lock_guard<std::mutex> lock(s_loadMutex);
	//以下ロード処理
	const auto it=s_graphMap.find(info);
	if(it==s_graphMap.end()){
		//まだロードされていない画像であるなら、新たに読み込む
		if(s_loadThreadId==std::this_thread::get_id()){
			//ロードができるthreadでLoad()が呼び出されたなら、LoadGraph()する
			const Graph graph(info.Load());
			std::pair<Graph,size_t> inp(graph,1);
			s_graphMap.insert(std::pair<LoadInfo,std::pair<Graph,size_t>>(info,inp));
			return graph.GetHandle();
		} else{
			//ロードができないthreadでLoad()が呼び出されたなら、taskVecに貯める
			s_taskVec.push_back(info);
			//------------------何返せばええんや……？？？-------------------
		}
	} else{
		//既に読み込みされているなら、カウントを増やす
		it->second.second++;
		return it->second.first.GetHandle();
	}
}

int Resource::Graph::Copy(int handle){
	//排他的に行われるようにlockを取る
	std::lock_guard<std::mutex> lock(s_loadMutex);
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
	//排他的に行われるようにlockを取る
	std::lock_guard<std::mutex> lock(s_loadMutex);
	const Graph info(handle);
	//infoと同じデータを探して、削除処理をする
	bool findFlag=false;//削除処理をしたかどうか
	for(auto it=s_graphMap.begin(),ite=s_graphMap.end();it!=ite;it++){
		if(it->second.first==info){
			if(it->second.second<=1){
				//参照箇所が1箇所だけだった場合は、DeleteGraph()をしてs_graphMapからも削除
				if(s_loadThreadId==std::this_thread::get_id()){
					//メインスレッドでないとDeleteできない
					DeleteGraph(info.GetHandle());
				} else{
					//-----------deleteタスクを貯める-------------
				}
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
		if(s_loadThreadId==std::this_thread::get_id()){
			//メインスレッドでないとDeleteできない
			DeleteGraph(handle);
		} else{
			//------------deleteタスクを貯める----------------
		}
	}
}

void Resource::Graph::Clear(){
	//mainスレッド以外からこの処理が呼ばれたら何もせずに返す
	if(s_loadThreadId!=std::this_thread::get_id()){
		return;
	}
	//排他的に行われるようにlockを取る
	std::lock_guard<std::mutex> lock(s_loadMutex);
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