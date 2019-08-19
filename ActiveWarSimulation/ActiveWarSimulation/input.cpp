#define _USE_MATH_DEFINES
#include<iostream>
#include"DxLib.h"
#include"input.h"
#include"GraphicControl.h"

//���͊֘A
static InputControler *inputControler;

int input_update(){
	return inputControler->Update();
}

//�L�[�{�[�h�֘A
int keyboard_get(int KeyCode){//KeyCode�̓L�[�{�[�h�̖��O
	return inputControler->Get(KeyCode);
}

//�}�E�X�֘A
int mouse_get(int MouseCode){
	return inputControler->MouseGet(MouseCode);
}

int mouse_wheel_get(){
	return inputControler->MouseWheelGet();
}

//�A�i���O�W���C�p�b�h�֘A
Vector2D analogjoypad_get(int InputType){
	int x,y;
	GetJoypadAnalogInput(&x,&y,InputType);
	return Vector2D((float)x,(float)y);
}

//���͏���S�ď���(�ǂ̃{�^�������͂���ĂȂ����Ƃɂ���)
void input_erase(){
	inputControler->InitInput();
}

//�{�^���������ꂽ���Ƃɂ���
void keyboard_COMinput(int KeyCode){
	inputControler->COMinput(KeyCode);
}

//���͊֘A
void InitInputControler(){
	inputControler=new InputControler();
}

void DeleteInputControler(){
	if(inputControler!=NULL){
		delete inputControler;
		inputControler=NULL;
	}
}

const std::string InputControler::InitFileName="Database/savedata/InputEdit.csv";

bool InputControler::GamepadKeyboardMap::operator<(const GamepadKeyboardMap &otherobj)const{
	if(keyboard < otherobj.keyboard){
		return true;
	}else if(keyboard > otherobj.keyboard){
		return false;
	}
	return (padbutton < otherobj.padbutton);
}

bool InputControler::GamepadKeyboardMap::operator==(const GamepadKeyboardMap &otherobj)const{
	return (this->keyboard == otherobj.keyboard);
}

bool InputControler::AnalogJoypadKeyboardMap::operator<(const AnalogJoypadKeyboardMap &otherobj)const{
	//keyboard�̑傫���ŏ��������߂�B
	if(keyboard < otherobj.keyboard){
		return true;
	} else if(keyboard > otherobj.keyboard){
		return false;
	}
	//�ȉ��e�L�g�[�B�������������Ȃ��悤�ɂ��Ă���B
	if(center < otherobj.center){
		return true;
	} else if(center > otherobj.center){
		return false;
	}
	if(okerror < otherobj.okerror){
		return true;
	} else if(okerror > otherobj.okerror){
		return false;
	}
	return(sizemin < otherobj.sizemin);
}

bool InputControler::AnalogJoypadKeyboardMap::operator==(const AnalogJoypadKeyboardMap &otherobj)const{
	return (keyboard==otherobj.keyboard);
}

bool InputControler::AnalogJoypadKeyboardMap::JudgeInput()const{
	Vector2D v=analogjoypad_get(DX_INPUT_PAD1);
	//�\���L�[�ɂ��v�̕␳
	if(GetJoypadNum()>0){
		if(keyboard_get(KEY_INPUT_UP)>0){
			v.y-=1000;
		}
		if(keyboard_get(KEY_INPUT_LEFT)>0){
			v.x-=1000;
		}
		if(keyboard_get(KEY_INPUT_RIGHT)>0){
			v.x+=1000;
		}
		if(keyboard_get(KEY_INPUT_DOWN)>0){
			v.y+=1000;
		}
	}
	//�p�x�̔F��
	double radian=v.GetRadian();
	double size=(double)v.size();
if(keyboard_get(KEY_INPUT_NUMPADENTER)==1){
		int poi=0;
	}


	return (std::fmin(std::abs(radian-center),M_PI*2-std::abs(radian-center))<okerror && sizemin<=size);
}

InputControler::InputControler(){
	for(int i=0;i<KeyNum;i++){
		m_keyboardFrame[i]=0;
	}
	for(int i=0;i<MouseButtonNum;i++){
		m_mouseFrame[i]=0;
	}
	m_mouseWheelRotVol=0;
	
	//�W���C�p�b�h�{�^���ƃL�[�{�[�h�̑Ή��\
	m_connectmap.insert(GamepadKeyboardMap(KEY_INPUT_Z,PAD_INPUT_4));
	m_connectmap.insert(GamepadKeyboardMap(KEY_INPUT_X,PAD_INPUT_3));
	m_connectmap.insert(GamepadKeyboardMap(KEY_INPUT_C,PAD_INPUT_1));
	m_connectmap.insert(GamepadKeyboardMap(KEY_INPUT_V,PAD_INPUT_2));
	m_connectmap.insert(GamepadKeyboardMap(KEY_INPUT_A,PAD_INPUT_7));
	m_connectmap.insert(GamepadKeyboardMap(KEY_INPUT_S,PAD_INPUT_8));
	m_connectmap.insert(GamepadKeyboardMap(KEY_INPUT_D,PAD_INPUT_5));
	m_connectmap.insert(GamepadKeyboardMap(KEY_INPUT_F,PAD_INPUT_6));
	m_connectmap.insert(GamepadKeyboardMap(KEY_INPUT_UP,PAD_INPUT_UP));
	m_connectmap.insert(GamepadKeyboardMap(KEY_INPUT_LEFT,PAD_INPUT_LEFT));
	m_connectmap.insert(GamepadKeyboardMap(KEY_INPUT_RIGHT,PAD_INPUT_RIGHT));
	m_connectmap.insert(GamepadKeyboardMap(KEY_INPUT_DOWN,PAD_INPUT_DOWN));

	
	//�W���C�p�b�h�̃A�i���O�X�e�B�b�N�ƃL�[�{�[�h�̑Ή��\
	//(center�Ɍ덷��^���Ă������Ƃŏ㉺�L�[���͂�double�̌v�Z�덷�ɂ���č��㍶���ɑΉ����ĕs�t���͂ɂȂ�Ƃ����c�Ȏ��Ԃ������)
	SetJoypadDeadZone(DX_INPUT_PAD1,0.2);
	//m_stickmap.insert(AnalogJoypadKeyboardMap(KEY_INPUT_W,-M_PI*2/3+0.0001,M_PI/6,1000));
	//m_stickmap.insert(AnalogJoypadKeyboardMap(KEY_INPUT_E,-M_PI/3+0.0001,M_PI/6,1000));
	//m_stickmap.insert(AnalogJoypadKeyboardMap(KEY_INPUT_D,0.0001,M_PI/6,1000));
	//m_stickmap.insert(AnalogJoypadKeyboardMap(KEY_INPUT_X,M_PI/3+0.0001,M_PI/6,1000));
	//m_stickmap.insert(AnalogJoypadKeyboardMap(KEY_INPUT_Z,M_PI*2/3+0.0001,M_PI/6,1000));
	//m_stickmap.insert(AnalogJoypadKeyboardMap(KEY_INPUT_A,M_PI+0.0001,M_PI/6,1000));
}

InputControler::~InputControler(){}

int InputControler::Update(){
	//�L�[�{�[�h�̍X�V
	char tmpKey[256];
	GetHitKeyStateAll(tmpKey);//tmpKey����tmpKey�̐擪�̃A�h���X������
	int tmpPad=0;//�Q�[���p�b�h�̃{�^���̓���
	if(GetJoypadNum()>0){
		tmpPad=GetJoypadInputState(DX_INPUT_PAD1);//�W���C�p�b�h���P�ł����͂���Ă���΁A�P�ڂ̃W���C�p�b�h�̃{�^�����͂��󂯕t����
	}
	std::set<GamepadKeyboardMap>::iterator cit=m_connectmap.begin(),cite=m_connectmap.end();
	std::set<AnalogJoypadKeyboardMap>::iterator sit=m_stickmap.begin(),site=m_stickmap.end();
	bool cflag,sflag;
	for(int i=0;i<KeyNum;i++){
		cflag=((cit!=cite) && (cit->keyboard)==i);//�L�[�{�[�h���͂ɑΉ�����p�b�h�{�^�������݂����true�ƂȂ�B
		sflag=((sit!=site) && (sit->keyboard)==i);//�L�[�{�[�h���͂ɑΉ�����W���C�p�b�h�A�i���O�X�e�B�b�N�̓��͕��@�����݂����true�ƂȂ�B
		if(tmpKey[i]!=0 || (cflag && (tmpPad & cit->padbutton)!=0) || (sflag && sit->JudgeInput())){
			m_keyboardFrame[i]++;
		}else{
			m_keyboardFrame[i]=0;
		}
		if(cflag){
			cit++;
		}
		if(sflag){
			sit++;
		}
	}
	//�}�E�X�̍X�V
	int mouseinput=GetMouseInput();
	for(int i=0;i<MouseButtonNum;i++){
		if((mouseinput>>i) & 0x01){
			m_mouseFrame[i]++;
		}else{
			m_mouseFrame[i]=0;
		}
	}
	m_mouseWheelRotVol=GetMouseWheelRotVol();

	return 0;
}

int InputControler::Get(int KeyCode){
	if(KeyCode>=0 && KeyCode<KeyNum){
		return m_keyboardFrame[KeyCode];
	}
	return 0;
}

int InputControler::MouseGet(int MouseCode){
	//MouseCode�̉�����x�ڂ�bit��1�Ȃ��m_mouseFrame[x-1]�ɓ��̓t���[�������i�[����Ă���
	//�܂�n��E�V�t�g�����Ƃ���1���������Ȃ��m_mouseFrame[n]��Ԃ��Ă�����Ηǂ�
	//�����ł́A�ŉ���bit��1�̂��̂�1bit�����V�t�g���Ă����A����Ƃ�AND���Z�ɂ���ĉ�bit�ڂ�1�����邩�����o����
	int bit=0x01;
	for(int i=0;i<MouseButtonNum;i++){
		if(MouseCode & (bit<<i)){
			return m_mouseFrame[i];
		}
	}
	return 0;
}

int InputControler::MouseWheelGet(){
	return m_mouseWheelRotVol;
}

void InputControler::InitInput(){
	for(int i=0;i<KeyNum;i++){
		m_keyboardFrame[i]=0;
	}
	for(int i=0;i<MouseButtonNum;i++){
		m_mouseFrame[i]=0;
	}
}

void InputControler::COMinput(int KeyCode){
	if(KeyCode>=0 && KeyCode<KeyNum){
		m_keyboardFrame[KeyCode]++;
	}
}

void InputControler::AddConnectMap(int KeyCode,int PadButton){
	m_connectmap.insert(GamepadKeyboardMap(KeyCode,PadButton));
}

void InputControler::MapSaving(){

}

//���A���^�C�����p��������͂̃T�|�[�g
const int InputSingleCharStringControler::inputBreakFrame=30;

InputSingleCharStringControler::InputSingleCharStringControler(const std::string &banString,size_t maxLen)
	:m_string(""),m_banString(banString),m_maxLen(maxLen),m_inputFlag(true)
{
	//�v�Z�ʍ팸�̂��߂ɁA���������������܂��Ă���ꍇ�͗\�߃������m�ۂ�����B
	if(maxLen>0){
		m_string.reserve(maxLen);
	}
}

InputSingleCharStringControler::~InputSingleCharStringControler(){}

char InputSingleCharStringControler::InputCharString()const{
	//�X�[�p�[�x�^�����BShift����̓��͂��m�F����B
	bool shiftFlag=(keyboard_get(KEY_INPUT_LSHIFT)>0 || keyboard_get(KEY_INPUT_RSHIFT)>0);
	auto f=[](int key)->bool{int t=keyboard_get(key);return (t==1 || t>=inputBreakFrame);};
	if(f(KEY_INPUT_TAB)){
		if(!shiftFlag){
			return '\t';
		} else{
			return '\t';
		}
	} else if(f(KEY_INPUT_SPACE)){
		if(!shiftFlag){
			return ' ';
		} else{
			return ' ';
		}
	} else if(f(KEY_INPUT_MINUS)){
		if(!shiftFlag){
			return '-';
		} else{
			return '=';
		}
	} else if(f(KEY_INPUT_YEN)){
		if(!shiftFlag){
			return '\\';
		} else{
			return '|';
		}
	} else if(f(KEY_INPUT_PREVTRACK)){
		if(!shiftFlag){
			return '^';
		} else{
			return '~';
		}
	} else if(f(KEY_INPUT_PERIOD)){
		if(!shiftFlag){
			return '.';
		} else{
			return '>';
		}
	} else if(f(KEY_INPUT_SLASH)){
		if(!shiftFlag){
			return '/';
		} else{
			return '?';
		}
	} else if(f(KEY_INPUT_SEMICOLON)){
		if(!shiftFlag){
			return ';';
		} else{
			return '+';
		}
	} else if(f(KEY_INPUT_COLON)){
		if(!shiftFlag){
			return ':';
		} else{
			return '*';
		}
	} else if(f(KEY_INPUT_LBRACKET)){
		if(!shiftFlag){
			return '[';
		} else{
			return '{';
		}
	} else if(f(KEY_INPUT_RBRACKET)){
		if(!shiftFlag){
			return ']';
		} else{
			return '}';
		}
	} else if(f(KEY_INPUT_AT)){
		if(!shiftFlag){
			return '@';
		} else{
			return '`';
		}
	} else if(f(KEY_INPUT_BACKSLASH)){
		if(!shiftFlag){
			return '\\';
		} else{
			return '_';
		}
	} else if(f(KEY_INPUT_COMMA)){
		if(!shiftFlag){
			return ',';
		} else{
			return '<';
		}
	} else if(f(KEY_INPUT_0)){
		if(!shiftFlag){
			return '0';
		} else{
			return '\0';
		}
	} else if(f(KEY_INPUT_1)){
		if(!shiftFlag){
			return '1';
		} else{
			return '!';
		}
	} else if(f(KEY_INPUT_2)){
		if(!shiftFlag){
			return '2';
		} else{
			return '"';
		}
	} else if(f(KEY_INPUT_3)){
		if(!shiftFlag){
			return '3';
		} else{
			return '#';
		}
	} else if(f(KEY_INPUT_4)){
		if(!shiftFlag){
			return '4';
		} else{
			return '$';
		}
	} else if(f(KEY_INPUT_5)){
		if(!shiftFlag){
			return '5';
		} else{
			return '%';
		}
	} else if(f(KEY_INPUT_6)){
		if(!shiftFlag){
			return '6';
		} else{
			return '&';
		}
	} else if(f(KEY_INPUT_7)){
		if(!shiftFlag){
			return '7';
		} else{
			return '\'';
		}
	} else if(f(KEY_INPUT_8)){
		if(!shiftFlag){
			return '8';
		} else{
			return '(';
		}
	} else if(f(KEY_INPUT_9)){
		if(!shiftFlag){
			return '9';
		} else{
			return ')';
		}
	} else if(f(KEY_INPUT_A)){
		if(!shiftFlag){
			return 'a';
		} else{
			return 'A';
		}
	} else if(f(KEY_INPUT_B)){
		if(!shiftFlag){
			return 'b';
		} else{
			return 'B';
		}
	} else if(f(KEY_INPUT_C)){
		if(!shiftFlag){
			return 'c';
		} else{
			return 'C';
		}
	} else if(f(KEY_INPUT_D)){
		if(!shiftFlag){
			return 'd';
		} else{
			return 'D';
		}
	} else if(f(KEY_INPUT_E)){
		if(!shiftFlag){
			return 'e';
		} else{
			return 'E';
		}
	} else if(f(KEY_INPUT_F)){
		if(!shiftFlag){
			return 'f';
		} else{
			return 'F';
		}
	} else if(f(KEY_INPUT_G)){
		if(!shiftFlag){
			return 'g';
		} else{
			return 'G';
		}
	} else if(f(KEY_INPUT_H)){
		if(!shiftFlag){
			return 'h';
		} else{
			return 'H';
		}
	} else if(f(KEY_INPUT_I)){
		if(!shiftFlag){
			return 'i';
		} else{
			return 'I';
		}
	} else if(f(KEY_INPUT_J)){
		if(!shiftFlag){
			return 'j';
		} else{
			return 'J';
		}
	} else if(f(KEY_INPUT_K)){
		if(!shiftFlag){
			return 'k';
		} else{
			return 'K';
		}
	} else if(f(KEY_INPUT_L)){
		if(!shiftFlag){
			return 'l';
		} else{
			return 'L';
		}
	} else if(f(KEY_INPUT_M)){
		if(!shiftFlag){
			return 'm';
		} else{
			return 'M';
		}
	} else if(f(KEY_INPUT_N)){
		if(!shiftFlag){
			return 'n';
		} else{
			return 'N';
		}
	} else if(f(KEY_INPUT_O)){
		if(!shiftFlag){
			return 'o';
		} else{
			return 'O';
		}
	} else if(f(KEY_INPUT_P)){
		if(!shiftFlag){
			return 'p';
		} else{
			return 'P';
		}
	} else if(f(KEY_INPUT_Q)){
		if(!shiftFlag){
			return 'q';
		} else{
			return 'Q';
		}
	} else if(f(KEY_INPUT_R)){
		if(!shiftFlag){
			return 'r';
		} else{
			return 'R';
		}
	} else if(f(KEY_INPUT_S)){
		if(!shiftFlag){
			return 's';
		} else{
			return 'S';
		}
	} else if(f(KEY_INPUT_T)){
		if(!shiftFlag){
			return 't';
		} else{
			return 'T';
		}
	} else if(f(KEY_INPUT_U)){
		if(!shiftFlag){
			return 'u';
		} else{
			return 'U';
		}
	} else if(f(KEY_INPUT_V)){
		if(!shiftFlag){
			return 'v';
		} else{
			return 'V';
		}
	} else if(f(KEY_INPUT_W)){
		if(!shiftFlag){
			return 'w';
		} else{
			return 'W';
		}
	} else if(f(KEY_INPUT_X)){
		if(!shiftFlag){
			return 'x';
		} else{
			return 'X';
		}
	} else if(f(KEY_INPUT_Y)){
		if(!shiftFlag){
			return 'y';
		} else{
			return 'Y';
		}
	} else if(f(KEY_INPUT_Z)){
		if(!shiftFlag){
			return 'z';
		} else{
			return 'Z';
		}
	}
	/*
	if(f(KEY_INPUT_)){
		if(!shiftFlag){
			return '';
		} else{
			return '';
		}
	} else 
	//*/
	return '\0';
}

void InputSingleCharStringControler::Update(){
	if(m_inputFlag){
		//���͒��Ȃ�
		char c=InputCharString();//������擾
		if(keyboard_get(KEY_INPUT_NUMPADENTER)==1){
			//Enter�L�[�������ꂽ�ꍇ
			m_inputFlag=false;//���͏I��
		} else if((keyboard_get(KEY_INPUT_BACK)==1 || keyboard_get(KEY_INPUT_BACK)>inputBreakFrame) && !m_string.empty()){
			//back�L�[�������ꂽ�ꍇ(30�t���[���ȍ~�͘A������)
			m_string.pop_back();//1�����폜
		} else{
			//���̑��̓���
			if(c!='\0'){
				//�����͂ł͂Ȃ�
				if(m_maxLen==0 || m_string.size()+1<m_maxLen){
					//�������������N���A���Ă���
					if(m_banString.find(c)==std::string::npos){
						//�֎~������̓��͂ł��Ȃ�
						m_string.push_back(c);
					}
				}
			}
		}
	}
}

//�}�E�X��^�b�`�Ȃǂ̉�ʏ�ɒu���{�^��
MouseButtonUI::MouseButtonUI(int x,int y,int dx,int dy,int graphic)
	:m_x(x),m_y(y),m_dx(dx),m_dy(dy),m_graphic(graphic){}

MouseButtonUI::MouseButtonUI(int x,int y,int graphic)
	:m_x(x),m_y(y),m_graphic(graphic)
{
	GetGraphSize(m_graphic,&m_dx,&m_dy);
}

MouseButtonUI::~MouseButtonUI(){
	DeleteGraphEX(m_graphic);
}

bool MouseButtonUI::JudgeIn()const{
	int mouseX,mouseY;
	GetMousePoint(&mouseX,&mouseY);
	return (mouseX>=m_x && mouseX<m_x+m_dx && mouseY>=m_y && mouseY<m_y+m_dy);
}

bool MouseButtonUI::JudgePressMoment()const{
	return (mouse_get(MOUSE_INPUT_LEFT)==1 && JudgeIn());
}

bool MouseButtonUI::JudgePushed()const{
	const bool push=(mouse_get(MOUSE_INPUT_LEFT)>0);
	return (push && JudgeIn());
}

void MouseButtonUI::DrawButton()const{
	//DrawBox(m_x,m_y,m_x+m_dx,m_y+m_dy,GetColor(255,255,0),TRUE);//�f�o�b�O�p
	DrawGraph(m_x,m_y,m_graphic,TRUE);
}

void MouseButtonUI::DrawButtonRect(unsigned int color,int fillFlag,int lineThickness)const{
	DrawBox(m_x,m_y,m_x+m_dx,m_y+m_dy,color,fillFlag);
	//�g���̕`��(DrawBox()�͘g���̑����̎w�肪�ł��Ȃ�)
	if(lineThickness>1){
		DrawLine(m_x,m_y,m_x+m_dx,m_y,color,lineThickness);
		DrawLine(m_x+m_dx,m_y,m_x+m_dx,m_y+m_dy,color,lineThickness);
		DrawLine(m_x+m_dx,m_y+m_dy,m_x,m_y+m_dy,color,lineThickness);
		DrawLine(m_x,m_y+m_dy,m_x,m_y,color,lineThickness);
	}
}

void MouseButtonUI::GetButtonInfo(int *x,int *y,int *dx,int *dy)const{
	if(x!=nullptr){
		*x=m_x;
	}
	if(y!=nullptr){
		*y=m_y;
	}
	if(dx!=nullptr){
		*dx=m_dx;
	}
	if(dy!=nullptr){
		*dy=m_dy;
	}
}

void MouseButtonUI::WarpTo(int x,int y){
	m_x=x;
	m_y=y;
}

MouseButtonUI MouseButtonUI::CreateWithCenter(int x,int y,int dx,int dy,int graphic){
	return MouseButtonUI(x-dx/2,y-dy/2,dx,dy,graphic);
}
