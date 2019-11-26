#include"DxLib.h"
#include"TutorialBattleSceneData.h"
#include"Circle.h"
#include"CommonConstParameter.h"
#include"GraphicControl.h"
#include"BattleSceneData.h"
#include"FileRead.h"
#include"StringBuilder.h"
#include"FilePath.h"

//----------------TutorialBattleSceneData::MoveTutorial----------------------
const float TutorialBattleSceneData::MoveTutorial::minDisplayPopOP=60.0f;

TutorialBattleSceneData::MoveTutorial::MoveTutorial(std::shared_ptr<Shape> moveTutorialArea)
	:TutorialBase(TutorialKind::e_move)
	,m_moveTutorialArea(moveTutorialArea)
	,m_displayPopFlag(false)
{}

TutorialBattleSceneData::MoveTutorial::~MoveTutorial(){}

void TutorialBattleSceneData::MoveTutorial::DrawSupplement(int font)const{
	const int width=650,height=290;
	const int x=CommonConstParameter::gameResolutionX-width,y=((int)BattleSceneData::mapDrawSize.y)-height;
	//���o�b�N�𔼓����ŕ`��
	int mode,pal;
	GetDrawBlendMode(&mode,&pal);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA,128);
	DrawBox(x,y,x+width,y+height,GetColor(0,0,0),TRUE);
	SetDrawBlendMode(mode,pal);
	//������̕`��
	DrawStringNewLineToHandle(x+5,y+5,width-10,height-10
		,"���ɓ_�ł��Ă���ꏊ�ɍs�����I\n\n"
		"�ړ�\n"
		"�@�}�E�X�@�@�@�F���N���b�N�ł��̏ꏊ�Ɍ������Ĉړ�\n"
		"�@�Q�[���p�b�h�F���W���C�X�e�B�b�N or �\���L�[\n"
		"�@�L�[�{�[�h�@�F�㉺���E�L�[\n\n"
		"�ړ��̂�蒼��\n"
		"�@�}�E�X�@�@�@�F�E�N���b�N\n"
		"�@�Q�[���p�b�h�F�{�^���R\n"
		"�@�L�[�{�[�h�@�FX�L�["
		,GetColor(255,255,255),font,2);
}

std::shared_ptr<TutorialBattleSceneData::TutorialBase> TutorialBattleSceneData::TutorialBase::Create(StringBuilder &info,const BattleSceneData &gameData){
	//�f�[�^�𕪊�
	info.Split(',','(',')');
	//�`���[�g���A���f�[�^���쐬
	try{
		if(info.m_vec.size()>=2){
			if(info.m_vec[0].GetString()=="move"){
				//MoveTutorial�́A���B�n�_��\���}�`���i�[����Ă���
				const std::shared_ptr<Shape> pShape=Shape::CreateShape(info.m_vec[1]);
				if(pShape.get()!=nullptr){
					return std::shared_ptr<TutorialBase>(new MoveTutorial(pShape));
				}
			} else if(info.m_vec[0].GetString()=="attack"){
				//AttackTutorial�́A���������ꂽ���̃��j�b�g�̔z��ԍ����i�[����Ă���
				const size_t index=std::stoi(info.m_vec[1].GetString().c_str());
				if(index<gameData.m_unitList.size()){
					return std::shared_ptr<TutorialBase>(new AttackTutorial(gameData.m_unitList[index]));
				}
			} else if(info.m_vec[0].GetString()=="wait"){
				//WaitTutorial�ɂ͓��ɐݒ肷�鍀�ڂ͂Ȃ�
				return std::shared_ptr<TutorialBase>(new WaitTutorial());
			} else if(info.m_vec[0].GetString()=="explain"){
				//ExplainTutorial�ɂ͉摜�t�@�C�����������Ă���
				return std::shared_ptr<TutorialBase>(new ExplainTutorial((FilePath::stageDir+gameData.m_stageDirName+info.m_vec[1].GetString()).c_str()));
			} else if(info.m_vec[0].GetString()=="blank"){
				//BlankTutorial�ɂ͉������Ȃ��s���̉񐔂������Ă���
				const int count=std::stoi(info.m_vec[1].GetString().c_str());
				if(count>0){
					return std::shared_ptr<TutorialBase>(new BlankTutorial(count));
				}
			}
		}
	} catch(const std::invalid_argument &){
		//���l����Ȃ��l�����o���ꂽ�ꍇ
	} catch(const std::out_of_range &){
		//�͈͊O�̒l�����o���ꂽ�ꍇ
	}
	//�쐬���s��
	return std::shared_ptr<TutorialBase>(nullptr);
}

//----------------TutorialBattleSceneData::AttackTutorial----------------------
TutorialBattleSceneData::AttackTutorial::AttackTutorial(const Unit *targetUnit)
	:TutorialBase(TutorialKind::e_normalAttack)
	,m_targetUnit(targetUnit)
{}

TutorialBattleSceneData::AttackTutorial::~AttackTutorial(){}

void TutorialBattleSceneData::AttackTutorial::DrawSupplement(int font)const{
	const int width=650,height=290;
	const int x=CommonConstParameter::gameResolutionX-width,y=((int)BattleSceneData::mapDrawSize.y)-height;
	//���o�b�N�𔼓����ŕ`��
	int mode,pal;
	GetDrawBlendMode(&mode,&pal);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA,128);
	DrawBox(x,y,x+width,y+height,GetColor(0,0,0),TRUE);
	SetDrawBlendMode(mode,pal);
	//������̕`��
	DrawStringNewLineToHandle(x+5,y+5,width-10,height-10
		,"���Ŏw�肳�ꂽ���j�b�g���U�����悤�I\n\n"
		"�U��\n"
		"�@�}�E�X�@�@�@�F�U�����������j�b�g�����N���b�N\n"
		"�@�Q�[���p�b�h�F�{�^���S\n"
		"�@�L�[�{�[�h�@�FZ�L�[\n\n"
		"�U���Ώۂ̕ύX\n"
		"�@�}�E�X�@�@�@�F�U�����������j�b�g�Ƀ}�E�X�����킹��\n"
		"�@�Q�[���p�b�h�F�{�^���V�E�{�^���W\n"
		"�@�L�[�{�[�h�@�FA�L�[�ES�L�["
		,GetColor(255,255,255),font,2);
}

//----------------TutorialBattleSceneData::WaitTutorial----------------------
TutorialBattleSceneData::WaitTutorial::WaitTutorial()
	:TutorialBase(TutorialKind::e_wait)
{}

TutorialBattleSceneData::WaitTutorial::~WaitTutorial(){}

void TutorialBattleSceneData::WaitTutorial::DrawSupplement(int font)const{
	const int width=650,height=195;
	const int x=CommonConstParameter::gameResolutionX-width,y=((int)BattleSceneData::mapDrawSize.y)-height;
	//���o�b�N�𔼓����ŕ`��
	int mode,pal;
	GetDrawBlendMode(&mode,&pal);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA,128);
	DrawBox(x,y,x+width,y+height,GetColor(0,0,0),TRUE);
	SetDrawBlendMode(mode,pal);
	//������̕`��
	DrawStringNewLineToHandle(x+5,y+5,width-10,height-10
		,"���̏�őҋ@���Ă݂悤�B\n\n"
		"�ҋ@\n"
		"�@�}�E�X�@�@�@�F�}�E�X�z�C�[��������\n"
		"�@�@�@�@�܂��́@�E����NEXT�{�^�������N���b�N\n"
		"�@�Q�[���p�b�h�F�{�^���Q\n"
		"�@�L�[�{�[�h�@�FV�L�["
		,GetColor(255,255,255),font,2);
}

//----------------TutorialBattleSceneData::ExplainTutorial----------------------
TutorialBattleSceneData::ExplainTutorial::ExplainTutorial(const char *gFileName)
	:TutorialBase(TutorialKind::e_explain)
	,m_gHandle(LoadGraphEX(gFileName))
	//,m_pos(CommonConstParameter::gameResolutionX*2,CommonConstParameter::gameResolutionX/2,CommonConstParameter::gameResolutionY/2,CommonConstParameter::gameResolutionY/2,30,Easing::TYPE_OUT,Easing::FUNCTION_BACK,0.2)
	,m_rate(0,1000,25,Easing::TYPE_OUT,Easing::FUNCTION_BACK,0.2)
	,m_cooltime(0)
	,m_secondMoveFlag(false)
{}

TutorialBattleSceneData::ExplainTutorial::~ExplainTutorial(){
	DeleteGraphEX(m_gHandle);
}

void TutorialBattleSceneData::ExplainTutorial::DrawSupplement(int font)const{
	int width,height;
	GetGraphSize(m_gHandle,&width,&height);
	const double rate=(m_secondMoveFlag?1.0-m_rate.GetRate():m_rate.GetRate());
	width=(int)(width*rate);
	height=(int)(height*rate);
	//DrawGraph(m_pos.GetX()-width/2,m_pos.GetY()-height/2,m_gHandle,TRUE);
	DrawExtendGraph((CommonConstParameter::gameResolutionX-width)/2,(CommonConstParameter::gameResolutionY-height)/2,(CommonConstParameter::gameResolutionX+width)/2,(CommonConstParameter::gameResolutionY+height)/2,m_gHandle,TRUE);
}

void TutorialBattleSceneData::ExplainTutorial::SetSecondMoveFlag(){
	//m_pos=PositionControl(m_pos.GetX(),m_pos.GetendX()*2-m_pos.GetstartX(),m_pos.GetY(),m_pos.GetendY()*2-m_pos.GetstartY(),m_pos.GetMaxFrame(),Easing::TYPE_IN,m_pos.GetFunction(),m_pos.GetDegree());
	m_rate=Easing(m_rate.GetX(),0,m_rate.GetMaxFrame(),Easing::TYPE_IN,m_rate.GetFunction(),m_rate.GetDegree());
	m_secondMoveFlag=true;
}

//----------------TutorialBattleSceneData::BlankTutorial----------------------
TutorialBattleSceneData::BlankTutorial::BlankTutorial(int count)
	:TutorialBase(TutorialKind::e_blank)
	,m_leftCount(count)
{}

void TutorialBattleSceneData::BlankTutorial::DrawSupplement(int font)const{}

//----------------TutorialBattleSceneData-----------------------
TutorialBattleSceneData::TutorialBattleSceneData(const std::string &stageDirName,const std::string &titleName,const StageLevel stageLevel)
	:BattleSceneData(stageDirName,titleName,stageLevel,PlayMode::e_tutorial)
{
	//�`���[�g���A���f�[�^�̓ǂݍ���
	//�I�u�W�F�N�g�Q��{}�ň͂܂�\n�ŋ�؂��Ă���̂ŁA�P�K�w���������ǂݍ��݂��āA�I�u�W�F�N�g�𐶐�����
	try{
		StringBuilder sb(FileStrRead((FilePath::stageDir+m_stageDirName+"/tutorialList.txt").c_str()),'\n','{','}');
		for(StringBuilder &ssb:sb.m_vec){
			std::shared_ptr<TutorialBase> pt=TutorialBase::Create(ssb,*this);//sb,ssb�͕ύX�����
			if(pt.get()!=nullptr){
				m_tutorialData.push_back(pt);
			}
		}
	} catch(const FileOpenFailedException &){
		//���ɉ������Ȃ�
	}
}

TutorialBattleSceneData::~TutorialBattleSceneData(){}
