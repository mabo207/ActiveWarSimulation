#include"SelectSubmissionScene.h"
#include"DxLib.h"
#include"BattleSceneData.h"
#include"CommonConstParameter.h"
#include"GeneralPurposeResource.h"
#include"input.h"

#include"SwitchUnitScene.h"

//�T�u�~�b�V�����ꗗ
#include"ArcherAttackDistance.h"
#include"MageAttackingOpponent.h"
#include"ArmerPosition.h"
#include"HealerPosition.h"
#include"ProtectFriend.h"
#include"IntensiveAttack.h"

//--------------------SelectSubmissionScene-----------------
const std::array<MyPolygon,SelectSubmissionScene::submissionCount> SelectSubmissionScene::submissionHitJudgeShape={
	MyPolygon::CreateRectangle(Vector2D(80.0f,40.0f),Vector2D(800.0f,460.0f),Shape::Fix::e_ignore)
	,MyPolygon::CreateRectangle(Vector2D(1040.0f,40.0f),Vector2D(800.0f,460.0f),Shape::Fix::e_ignore)
	,MyPolygon::CreateRectangle(Vector2D(80.0f,580.0f),Vector2D(800.0f,460.0f),Shape::Fix::e_ignore)
	,MyPolygon::CreateRectangle(Vector2D(1040.0f,580.0f),Vector2D(800.0f,460.0f),Shape::Fix::e_ignore)
};

SelectSubmissionScene::SelectSubmissionScene(const std::shared_ptr<BattleSceneData> &battleSceneData)
	:BattleSceneElement(SceneKind::e_selectSubmission)
	,m_battleSceneData(battleSceneData)
	,m_selectIndex(0)
{
	m_submissionList[0]=std::shared_ptr<SubmissionRuleBase>(new ArcherAttackDistance());
	m_submissionList[1]=std::shared_ptr<SubmissionRuleBase>(new ArmerPosition());
	m_submissionList[2]=std::shared_ptr<SubmissionRuleBase>(new HealerPosition());
	m_submissionList[3]=std::shared_ptr<SubmissionRuleBase>(new MageAttackingOpponent());
}

SelectSubmissionScene::~SelectSubmissionScene(){}

int SelectSubmissionScene::thisCalculate(){
	//�T�u�~�b�V�����̑I��
	const Vector2D mousePos=GetMousePointVector2D();
	for(size_t i=0;i<submissionCount;i++){
		if(submissionHitJudgeShape[i].VJudgePointInsideShape(mousePos)){
			m_selectIndex=i;
			break;
		}
	}
	//�T�u�~�b�V�������蔻��
	if(keyboard_get(KEY_INPUT_Z)==1 || mouse_get(MOUSE_INPUT_LEFT)==1){
		//����{�^����������
		if(submissionHitJudgeShape[m_selectIndex].VJudgePointInsideShape(mousePos)){
			//�}�E�X�������ƍ��ڂɓ����Ă���
			PlaySoundMem(GeneralPurposeResource::decideSound,DX_PLAYTYPE_BACK,TRUE);//���艹��炷
			return SceneKind::e_switch;//���̃V�[����
		}
	}
	return SceneKind::e_selectSubmission;
}

void SelectSubmissionScene::thisDraw()const{
	//�o�g����ʂ̕`��
	m_battleSceneData->DrawField();//�t�B�[���h�̕`��
	m_battleSceneData->DrawUnit(true,std::set<const Unit *>{});//���j�b�g�̕`��
	m_battleSceneData->DrawHPGage();//�S���j�b�g��HP�Q�[�W�̕`��
	//m_battleSceneData->DrawOrder(std::set<const BattleObject*>{});//���j�b�g�̃I�[�_�[���Ԃ�`��(����m_operatedUnit�����������ĂȂ��e���Ńo�O��̂ł��Ȃ�)
	//���������`��
	{
		int mode,pal;
		GetDrawBlendMode(&mode,&pal);
		SetDrawBlendMode(DX_BLENDMODE_ALPHA,128);
		DrawBox(0,0,CommonConstParameter::gameResolutionX,CommonConstParameter::gameResolutionY,GetColor(0,0,0),TRUE);
		SetDrawBlendMode(mode,pal);
	}
	//�T�u�~�b�V�����̕`��
	for(size_t i=0;i<submissionCount;i++){
		//���n�̕`��
		const Vector2D pos=submissionHitJudgeShape[i].GetPosition();
		submissionHitJudgeShape[i].Draw(pos,Vector2D(),GetColor(64,192,64),TRUE,0.0f);
		if(i==m_selectIndex){
			//�I�����Ă��鍀�ڂɂ͐Ԙg��
			submissionHitJudgeShape[i].Draw(pos,Vector2D(),GetColor(196,128,128),FALSE,10.0f);
		} else{
			//�I�����Ă��Ȃ����ڂɂ͊D���F�g��
			submissionHitJudgeShape[i].Draw(pos,Vector2D(),GetColor(128,128,64),FALSE,10.0f);
		}
		//�T�u�~�b�V�����̓��e�̕`��
		DrawStringNewLineToHandle((int)pos.x,(int)pos.y,800,440,m_submissionList[i]->GetSubmissionExplanation().c_str(),GetColor(255,255,255),GeneralPurposeResource::popLargeFont,4);
	}
}

int SelectSubmissionScene::UpdateNextScene(int index){
	switch(index){
	case(SceneKind::e_switch):
		//�Q�[���J�n��
		//�T�u�~�b�V�����ݒ�
		m_battleSceneData->m_scoreObserver->SetSubmissionRule(m_submissionList[m_selectIndex]);
		//���V�[���쐬
		m_nextScene=std::shared_ptr<BattleSceneElement>(new SwitchUnitScene(m_battleSceneData));
		break;
	}
	return index;
}

void SelectSubmissionScene::ReturnProcess(){
	//���ɉ������Ȃ�
}
