#include<iostream>
#include<cassert>
#include"DxLib.h"
#include"GraphicControl.h"


/*GraphicControlClass��p���邽�߂̊֐�*/
static GraphicControlClass *GraphicControler=NULL;

void GraphicControler_Init(){
	GraphicControler=new GraphicControlClass;
	int i=0;
}

void GraphicControler_End(){
	if(GraphicControler!=NULL){
		delete GraphicControler;
		GraphicControler=NULL;
	}
}

int LoadGraphEX(std::string filename){
	if(GraphicControler==NULL){
		return -1;
	}
	return GraphicControler->LoadGraphEX(filename);
}

int LoadDivGraphEX(std::string filename,int Allnum,int XNum,int YNum,int XSize,int YSize,int *HandleBuf){
	if(GraphicControler==NULL){
		return -1;
	}
	return GraphicControler->LoadDivGraphEX(filename,Allnum,XNum,YNum,XSize,YSize,HandleBuf);
}

int CopyGraph(int GRHandle){
	return GraphicControler->CopyGraph(GRHandle);
}

int DeleteGraphEX(int GRHandle){
	if(GraphicControler==NULL){
		return -1;
	}
	return GraphicControler->DeleteGraphEX(GRHandle);
}

/*GraphicControlClass*///������`����GraphicData���̖߂�l�̏������ɒ��ӁI�I
//m_Graphic�̒�����~�����f�[�^��T��(�������O�̂��͈̂�Ԕz��ԍ��̏��������̂��Ԃ��Ă���)
std::vector<GraphicControlClass::GraphicData>::iterator GraphicControlClass::GraphicSearch(int GRHandle){
	//�C�e���[�^��p��
	std::vector<GraphicData>::iterator pGraphic=m_Graphic.begin();
	for(;pGraphic!=m_Graphic.end();pGraphic++){
		if(pGraphic->m_handle==GRHandle){
			return pGraphic;
		}
	}
	//���������Ȃ��C�e���[�^��End�̃C�e���[�^�B�G���[����m_Graphic.End()��Ԃ�
	return m_Graphic.end();
}

std::vector<GraphicControlClass::GraphicData>::iterator GraphicControlClass::GraphicSearch(std::string GRName){
	//�C�e���[�^��p��
	std::vector<GraphicData>::iterator pGraphic=m_Graphic.begin();
	for(;pGraphic!=m_Graphic.end();pGraphic++){
		if(pGraphic->m_name==GRName){
			return pGraphic;
		}
	}
	//���������Ȃ��C�e���[�^��End�̃C�e���[�^�B�G���[����m_Graphic.End()��Ԃ�
	return m_Graphic.end();
}

//�O���t�B�b�N�̓ǂݍ���
int GraphicControlClass::LoadGraphEX(std::string filename){
	//�܂������f�[�^���Ȃ������m�F����B
	std::vector<GraphicData>::iterator pData=GraphicSearch(filename);
	if( pData != m_Graphic.end() ){//�����f�[�^������Ȃ�
		pData->m_number++;//�O���t�B�b�N�f�[�^�̗p�����Ă���ӏ��𑝂₷�B
		return pData->m_handle;//�O���t�B�b�N�n���h����Ԃ�
	}
	//�����f�[�^���Ȃ���Ζ����Ƀf�[�^��ǉ�
	const int GRHandle=LoadGraph(filename.c_str());
	if(GRHandle!=-1){//�G���[���N���Ă��Ȃ����
		m_Graphic.push_back(GraphicData(GRHandle,filename,1));//m_Graphic�Ƀf�[�^��ǉ��B1�ӏ��ŗp�����Ă���
	}
	return GRHandle;
}

//�O���t�B�b�N�̕����ǂݍ���
int GraphicControlClass::LoadDivGraphEX(std::string filename,int Allnum,int XNum,int YNum,int XSize,int YSize,int *HandleBuf){
	//��������0�ȉ��Ȃ玸�s
	if(Allnum<=0){
		return -1;
	}
	int flag=0;//�������ǂ����̃t���O
	//���O�ƕ������Ń_�u���̑��݂�����
	std::vector<DivGraphicData>::iterator pData=m_DivGraphic.begin();
	for(;pData!=m_DivGraphic.end();pData++){
		if(pData->m_name==filename && pData->m_divNUM==Allnum){//����Ă�����
			break;//�C�e���[�^�����̂܂܂ɂ��ĕԂ�
		}
	}
	//�_�u�����ݎ�
	if(pData!=m_DivGraphic.end()){
		//m_DivGraphic�̕ҏW
		pData->m_number++;
		//m_Graphic�̕ҏW
		for(int i=0;i<Allnum;i++){
			std::vector<GraphicData>::iterator pGdata=GraphicSearch(pData->m_handle[i]);
			if(pGdata!=m_Graphic.end()){//������i�Ԗڂ̃f�[�^�����݂��Ă���Ȃ��
				HandleBuf[i]=pData->m_handle[i];//HandleBuf�Ƀn���h�������Ă�����
				pGdata->m_number++;//�g���Ă���񐔂𑝂₷
			}else{
				HandleBuf[i]=-1;//i�Ԗڂ����ǂݍ��ݎ��s
				flag=-1;//�S�̂Ƃ��Ă͎��s
				assert(1==0);
			}
		}
	}
	//�_�u���񑶍ݎ�
	else{
		flag=LoadDivGraph(filename.c_str(),Allnum,XNum,YNum,XSize,YSize,HandleBuf);//�����ǂݍ��݂��s��
		if(flag==0){//�ǂݍ��ݐ�����
			//m_DivGraphic�̕ҏW
			/*int *pointer=new int [Allnum];
			for(int i=0;i<Allnum;i++){
				pointer[i]=HandleBuf[i];
			}*/
			m_DivGraphic.push_back(DivGraphicData(NULL,filename,1,Allnum));
			m_DivGraphic.back().m_handle=new int [Allnum];
			for(int i=0;i<Allnum;i++){
				m_DivGraphic.back().m_handle[i]=HandleBuf[i];
			}
			//m_Graphic�̕ҏW
			for(int i=0;i<Allnum;i++){
				m_Graphic.push_back(GraphicData(HandleBuf[i],filename,1));
			}
		}
	}
	return flag;
}

//�O���t�B�b�N�̕���
int GraphicControlClass::CopyGraph(int GRHandle){
	//�Y������O���t�B�b�N�̌���
	std::vector<GraphicData>::iterator Data=GraphicSearch(GRHandle);
	//Data���Ӗ��̂Ȃ��C�e���[�^���w���Ă��鎞�̓G���[�Ƃ��ď����B
	if(Data==m_Graphic.end()){
		return -1;
	}else{
		//�����łȂ��ꍇ�A�g���ӏ��̃J�E���g��1���₹�Ηǂ�
		Data->m_number++;
	}
	return Data->m_handle;
}

//�O���t�B�b�N�̍폜
int GraphicControlClass::DeleteGraphEX(int GRHandle){
	int returnnum=0;//�߂�l
	//m_Graphic����T��
	std::vector<GraphicData>::iterator pData=GraphicSearch(GRHandle);
	if(pData==m_Graphic.end()){//�f�[�^��������Ȃ������ꍇ
		returnnum=DeleteGraph(GRHandle);//�Ƃ肠����DeleteGraph�Ɠ�������������悤�ɂ���
	}else{
		//�f�[�^�����������ꍇ
		std::string GRName=pData->m_name;//��X���O�͗p����̂ŕۑ�
		pData->m_number--;//�g���Ă���ӏ���1���炷
		if(pData->m_number <= 0){//�g���Ă���ꏊ���Ȃ����
			//���̏ꏊ��j������
			returnnum=DeleteGraph(pData->m_handle);//�O���t�B�b�N�f�[�^�̍폜
			m_Graphic.erase(pData);//std::vector�̗v�f�̍폜
			m_Graphic.shrink_to_fit();//���ёւ�
			//�폜��Ƃ��I������Am_DivGraphic�̕ҏW
			//�폜�����f�[�^�ɂ��āA���ꂪ�����ǂݍ��݂��Ă������A
			//����Ɠ������O�̃O���t�B�b�N�����݂��Ă��Ȃ����m_DivGraph�����菜��
			//for(std::vector<DivGraphicData>::iterator pd=m_DivGraphic.begin();pd!=m_DivGraphic.end();pd++){
			std::vector<DivGraphicData>::iterator pd=m_DivGraphic.begin();
			while(true){
				std::vector<DivGraphicData>::iterator pdtmp=m_DivGraphic.end();
				if(pd==m_DivGraphic.end()){
					break;
				}
				bool eraseflag=false;//�v�f���������ǂ���
				//�܂����O�̈�v������
				if(pd->m_name==GRName){
					//���Ƀf�[�^�̏������Ă��邩������
					eraseflag=true;//�Ƃ肠���������t���O�𗧂ĂĂ���
					for(int i=0;i<pd->m_divNUM;i++){
						if(GraphicSearch(pd->m_handle[i])!=m_Graphic.end()){//�O���t�B�b�N�����݂��Ă���Ȃ��
							eraseflag=false;//�����ɍ���Ȃ���Ώ����Ȃ����ɂ���
							break;
						}
					}
					if(eraseflag){//true�̂܂܂Ȃ��(�S�ď����Ă���Ȃ��)
						//m_DivGraphic����v�f������
						pd=m_DivGraphic.erase(pd);
						//m_DivGraphic.shrink_to_fit();
						int hhhh=0;
					}
				}
				if(!eraseflag){//m_DivGraphic.erase()�����ĂȂ����͎��̃C�e���[�^�������ɂ̓C�e���[�^�𓮂����Ȃ��Ƃ����Ȃ�
					pd++;//while���[�v�̏I���pd�����̃C�e���[�^�ɓ�����
				}
			}
		}
	}
	
	return returnnum;
}

//�S�ẴO���t�B�b�N�f�[�^�̍폜
int GraphicControlClass::InitGraphEX(){
	while(!m_Graphic.empty()){
		DeleteGraph(m_Graphic.begin()->m_handle);
		m_Graphic.erase(m_Graphic.begin());
	}
	m_Graphic.shrink_to_fit();
	return 0;
}

GraphicControlClass::GraphicData::GraphicData(int handle,std::string name,int number)
	:m_handle(handle),m_name(name),m_number(number){}

GraphicControlClass::GraphicData::~GraphicData(){}

GraphicControlClass::GraphicControlClass(){

}

GraphicControlClass::~GraphicControlClass(){
	//�S�ẴO���t�B�b�N�f�[�^���������
	GraphicControler->InitGraphEX();
}

//------------------FontControlClass�Ƃ�����������߂̊֐�------------------
static FontControlClass *pFontControler=nullptr;

//�N���X�̊֐�
FontControlClass::FontData::FontData(std::string i_fontname,int i_size,int i_thick,int i_fonttype)
	:fontname(i_fontname),size(i_size),thick(i_thick),fonttype(i_fonttype){}

bool FontControlClass::FontData::operator<(const FontData &otherobj)const{
	//size
	if(size<otherobj.size){
		return true;
	} else if(size>otherobj.size){
		return false;
	}
	//thick
	if(thick<otherobj.thick){
		return true;
	} else if(thick>otherobj.thick){
		return false;
	}
	//fonttype
	if(fonttype<otherobj.fonttype){
		return true;
	} else if(fonttype>otherobj.fonttype){
		return false;
	}
	//fontname
	if(fontname<otherobj.fontname){
		return true;
	} else if(fontname>otherobj.fontname){
		return false;
	}

	return false;
}

bool FontControlClass::FontData::operator==(const FontData &otherobj)const{
	return (fontname==otherobj.fontname && size==otherobj.size && thick==otherobj.thick && fonttype==otherobj.fonttype);
}

FontControlClass::FontControlClass(){}

FontControlClass::~FontControlClass(){
	//m_font��S�č폜����
	for(const std::pair<FontData,MapValue> &data:m_font){
		DeleteFontToHandle(data.second.handle);
	}
}

int FontControlClass::CreateFontToHandleEX(std::string fontname,int size,int thick,int fonttype){
	//����t�H���g�����݂��Ă��邩�̒���
	FontData fontdata(fontname,size,thick,fonttype);
	std::map<FontData,MapValue>::iterator it=m_font.find(fontdata);
	//�t�H���g�̍쐬����
	int handle;
	if(it!=m_font.end()){
		//���Ƀt�H���g������Ă���ꍇ
		it->second.count++;
		handle=it->second.handle;
	} else{
		//�܂��t�H���g������Ă��Ȃ��ꍇ
		handle=CreateFontToHandle(fontname.c_str(),size,thick,fonttype);
		if(handle!=-1){
			//�t�H���g���������������ꍇ��m_font�ɏ����i�[
			m_font.insert(std::pair<FontData,MapValue>(fontdata,MapValue(handle,1)));
		}
	}
	return handle;
}

int FontControlClass::DeleteFontToHandleEX(int handle){
	//handle�̌���
	bool flag=false;//handle��Delete�����ǂ���
	int ret=-1;
	for(std::map<FontData,MapValue>::iterator it=m_font.begin(),ite=m_font.end();it!=ite;it++){
		if(it->second.handle==handle){
			//�폜����
			it->second.count--;//�g�p�ӏ���1����
			if(it->second.count<=0){
				//�g�p�ӏ����Ȃ��Ȃ�����t�H���g���폜���Am_font����f�[�^����菜��
				ret=DeleteFontToHandle(handle);
				m_font.erase(it);
				flag=true;
				break;
			}
		}
	}
	//handle���f�[�^�ꗗ�ɑ��݂��Ă��Ȃ���΁A�폜�̂ݍs��
	if(!flag){
		ret=DeleteFontToHandle(handle);
	}
	return ret;

}

//�֐��Q
void FontControler_Init(){
	pFontControler=new FontControlClass();
}

void FontControler_End(){
	if(pFontControler!=nullptr){
		delete pFontControler;
		pFontControler=nullptr;
	}
}

int CreateFontToHandleEX(std::string fontname,int size,int thick,int fonttype){
	int handle;
	if(pFontControler!=nullptr){
		handle=pFontControler->CreateFontToHandleEX(fontname,size,thick,fonttype);
	} else{
		handle=CreateFontToHandle(fontname.c_str(),size,thick,fonttype);
	}
	return handle;
}

int DeleteFontToHandleEX(int handle){
	int ret;
	if(pFontControler!=nullptr){
		ret=pFontControler->DeleteFontToHandleEX(handle);
	} else{
		ret=DeleteFontToHandle(handle);
	}
	return ret;
}

