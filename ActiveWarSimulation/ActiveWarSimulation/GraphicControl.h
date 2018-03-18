#ifndef DEF_GRAPHICCONTROL_H
#define DEF_GRAPHICCONTROL_H

#include<vector>
#include<string>
#include<map>

//�d�������O���t�B�b�N�ǂݍ��݂�h�~���邽�߂̃O���t�B�b�N�ǂݍ��݊Ǘ��N���X
//�����ǂݍ��݂������̂̈ꕔ�������݂����Ȃ��Ƃ�����f�[�^�͂����ł͊Ǘ����Ȃ�
class GraphicControlClass{
	//�^�錾
protected:
	//(�����ǂݍ��݂��Ȃ�)�O���t�B�b�N�^(�����ǂݍ��ݎ��͕������ꂽ���̂�����)
	struct GraphicData{
		int m_handle;//�f�[�^�n���h��
		std::string m_name;//���[�h���̖��O
		int m_number;//���̃O���t�B�b�N�f�[�^�����ӏ��ŗp�����Ă��邩
		GraphicData(int handle,std::string name,int number);
		~GraphicData();
	};
	//�����ǂݍ��݂���O���t�B�b�N�^(����Ȃ�)
	struct DivGraphicData{
		int *m_handle;//�f�[�^�n���h��(���I�m��)
		std::string m_name;//���[�h���̖��O
		int m_number;//���̃O���t�B�b�N�f�[�^�����ӏ��ŗp�����Ă��邩
		int m_divNUM;//������(�f�[�^�n���h����)
		DivGraphicData(int *handle,std::string name,int number,int divNUM)
			:m_handle(handle),m_name(name),m_number(number),m_divNUM(divNUM)
		{
			int i=0;
		}
		~DivGraphicData(){
			if(m_handle!=NULL){
				delete [] m_handle;
				m_handle=NULL;
			}
		}
		DivGraphicData(const DivGraphicData &otherobj)
			:m_handle(NULL),m_name(otherobj.m_name),m_number(otherobj.m_number),m_divNUM(otherobj.m_divNUM)
		{
			//m_handle�͓��I�m�ۂ����z��Ȃ̂ŁA�R�s�[�̍ۂ͐V�������������m�ۂ��Ă�����K�v������
			if(otherobj.m_handle!=NULL){//NULL�łȂ���΃R�s�[������̂�����
				m_handle=new int [m_divNUM];
				for(int i=0;i<m_divNUM;i++){
					m_handle[i]=otherobj.m_handle[i];
				}
			}
		}
		DivGraphicData operator=(const DivGraphicData &otherobj){//������Z�q=�̃I�[�o�[���[�h
			return DivGraphicData(otherobj);
		}
	};
	
	//�ϐ�
protected:
	std::vector<GraphicData> m_Graphic;
	std::vector<DivGraphicData> m_DivGraphic;//����Ȃ�

	//�֐�
protected:
	std::vector<GraphicData>::iterator GraphicSearch(int GRHandle);
	std::vector<GraphicData>::iterator GraphicSearch(std::string GRName);


public:
	GraphicControlClass();
	~GraphicControlClass();
	int LoadGraphEX(std::string filename);//(�߂�l)-1:�G���[���� -1�ȊO:�O���t�B�b�N�n���h��
	int LoadDivGraphEX(std::string filename,int Allnum,int XNum,int YNum,int XSize,int YSize,int *HandleBuf);//(�߂�l)-1:�G���[���� 0:����
	int CopyGraph(int GRHandle);//(�߂�l)-1:�G���[���� ����ȊO:�O���t�B�b�N�n���h��//�O���t�B�b�N�f�[�^�𕡐�����(�����I�ɂ͎g���Ă���ӏ���1���₷����)
	int DeleteGraphEX(int GRHandle);//(�߂�l)-1:�G���[���� 0:����
	int InitGraphEX();//�S�ẴO���t�B�b�N�f�[�^���폜����
};

void GraphicControler_Init();

void GraphicControler_End();

int LoadGraphEX(std::string filename);

int LoadDivGraphEX(std::string filename,int Allnum,int XNum,int YNum,int XSize,int YSize,int *HandleBuf);

int CopyGraph(int GRHandle);

int DeleteGraphEX(int GRHandle);

//�d�������t�H���g�ǂݍ��݂�h�~���邽�߂̃t�H���g�ǂݍ��݊Ǘ��N���X
class FontControlClass{
	//�^�E�񋓑�
private:
	struct FontData{
		std::string fontname;
		int size;
		int thick;
		int fonttype;
		FontData(std::string i_fontname,int i_size,int i_thick,int i_fonttype);
		bool operator<(const FontData &otherobj)const;
		bool operator==(const FontData &otherobj)const;
	};
	struct MapValue{
		int handle;
		int count;
		MapValue(int i_handle,int i_count):handle(i_handle),count(i_count){}
	};
	
	//�ϐ�
private:
	std::map<FontData,MapValue> m_font;//�t�H���g�f�[�^�ƌ��ƃn���h��

	//�֐�
private:

public:
	FontControlClass();
	~FontControlClass();
	int CreateFontToHandleEX(std::string fontname,int size,int thick,int fonttype=-1);
	int DeleteFontToHandleEX(int handle);
};

void FontControler_Init();

void FontControler_End();

int CreateFontToHandleEX(std::string fontname,int size,int thick,int fonttype=-1);

int DeleteFontToHandleEX(int handle);

#endif
