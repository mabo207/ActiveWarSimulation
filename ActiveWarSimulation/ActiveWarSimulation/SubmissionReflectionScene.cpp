#include"DxLib.h"
#include"input.h"
#include"SubmissionReflectionScene.h"
#include"StageClearScene.h"
#include"ToolsLib.h"
#include"CommonConstParameter.h"
#include"LogElement.h"
#include"GeneralPurposeResource.h"
#include"GraphicControl.h"
#include"FilePath.h"
#include"WaitLog.h"
#include"AttackLog.h"

namespace {
	const int lineWidth=5;
	const unsigned int merginColor=GetColor(128,128,255);
	const float minimapRate=0.5f;
	const int minimapWidth=(int)(CommonConstParameter::mapSizeX*minimapRate);
	const int minimapHeight=(int)(CommonConstParameter::mapSizeY*minimapRate);
}
//---------------SubmissionReflectionScene::MinimapDrawInfo-----------------
SubmissionReflectionScene::MinimapDrawInfo::MinimapDrawInfo(const std::shared_ptr<const LogElement> &log)
	:pOperateUnit(nullptr),pAttackedUnit(nullptr)
{
	if(log){
		//log�̗L��������
		//�֐��̍쐬
		const std::shared_ptr<const WaitLog> waitLog=std::dynamic_pointer_cast<const WaitLog>(log);
		const std::shared_ptr<const AttackLog> attackLog=std::dynamic_pointer_cast<const AttackLog>(log);
		std::function<const Unit *()> getOperateUnit,getAttackedUnit;
		if(log->GetLogKind()==LogElement::LogKind::e_wait && waitLog){
			//�ҋ@���O�̏ꍇ
			getOperateUnit=[&waitLog](){return waitLog->GetOperateUnitData().punit;};
		} else if(log->GetLogKind()==LogElement::LogKind::e_attack && attackLog){
			//�U�����O�̏ꍇ
			getOperateUnit=[&attackLog](){return attackLog->GetOperateUnitData().punit;};
			getAttackedUnit=[&attackLog](){return attackLog->GetAimedUnit();};
		}
		//�Ֆʂ̃��j�b�g�ꗗ�̍쐬
		const size_t maxSize=log->m_unitDataList.size();
		size_t operateIndex=maxSize,attackedIndex=maxSize,index=0;//pOperateUnit��pAttackedUnit�̒T���ɗp����
		for(const LogElement::UnitLogData &logData:log->m_unitDataList){
			Unit u=*logData.punit;
			u.Warp(logData.pos);
			u.AddHP(logData.hp-u.GetBattleStatus().HP);
			u.SetOP(logData.op);
			u.SetPenetratable(Unit::Team::e_player);
			//�i�[
			unitList.push_back(u);
			//operateIndex,attackedIndex�̒T���A�֐������݂��邩�ǂ����C������
			if(getOperateUnit && logData.punit==getOperateUnit()){
				operateIndex=index;
			}
			if(getAttackedUnit && logData.punit==getAttackedUnit()){
				attackedIndex=index;
			}
			//�T���pindex�̍X�V
			index++;
		}
		//pOperateUnit��pAttackedUnit�̒T��(����ȍ~unitList�̑���͍s���Ȃ�)
		if(operateIndex<maxSize){
			pOperateUnit=&unitList[operateIndex];
		}
		if(attackedIndex<maxSize){
			pAttackedUnit=&unitList[attackedIndex];
		}
	}
}

SubmissionReflectionScene::MinimapDrawInfo::~MinimapDrawInfo(){}

//---------------SubmissionReflectionScene-----------------
SubmissionReflectionScene::SubmissionReflectionScene(const std::shared_ptr<BattleSceneData> &battleSceneData,const std::shared_ptr<BattleSceneElement> &clearScene)
	:BattleSceneElement(SceneKind::e_submissionReflection)
	,m_battleSceneData(battleSceneData)
	,m_clearScene(clearScene)
	,m_operateCursor(LoadGraphEX(FilePath::graphicDir+"operatedCursor.png"))
	,m_predictNumberFont(CreateFontToHandleEX("���C���I",28,8,DX_FONTTYPE_EDGE))
	,m_predictExplainFont(CreateFontToHandleEX("���C���I",10,2,DX_FONTTYPE_EDGE))
{
	//m_goodLogInfo,m_badLogInfo�̏�����
	const WholeReflectionInfo reflectionInfo=m_battleSceneData->m_scoreObserver->GetSubmission().GetReflectionInfo();
	m_goodLogInfo.emplace(reflectionInfo.m_goodLog.second);
	m_badLogInfo.emplace(reflectionInfo.m_badLog.second);
}

SubmissionReflectionScene::~SubmissionReflectionScene(){
	DeleteGraphEX(m_operateCursor);
	DeleteFontToHandleEX(m_predictNumberFont);
	DeleteFontToHandleEX(m_predictExplainFont);
}

void SubmissionReflectionScene::DrawResizedMap(int x,int y,const MinimapDrawInfo &minimapInfo)const{
	//�}�[�W���̕`��
	DrawBox(x-lineWidth,y-lineWidth,x+minimapWidth+lineWidth,y+minimapHeight+lineWidth,merginColor,TRUE);
	const Vector2D startPos((float)x,(float)y);
	//BattleSceneData�̕`��֐��͊g��k���`��ɑΉ����Ă��Ȃ��̂ŁA�Ǝ��Ɏ�������
	//�w�i�`��
	DrawExtendGraphExRateAssign(startPos.x,startPos.y,minimapRate,m_battleSceneData->m_mapPic,TRUE);
	//���j�b�g�̕`��
	for(size_t i=1,siz=minimapInfo.GetUnitList().size();i<siz+1;i++){
		const size_t index=i%siz;
		//�E�C���h�E�ɓ����Ă��Ȃ����͕`�悵�Ȃ�
		//�����蔻��}�`��m_fix����͍s��Ȃ��A������������ł��邩�ǂ����̔���Ȃ̂ŁA�u���ۂ̃}�b�v�ł͎���ł��邪���z�}�b�v�ł͐����Ă���悤�ɂ������v�Ƃ����v�]�ɂ͑Ή��ł��Ȃ�����
		//�擪���j�b�g�͍Ō�ɕ`�悷��
		if(m_battleSceneData->m_mapRange->JudgeInShapeRect(&minimapInfo.GetUnitList()[index])){
			//�A�N�V�����͈͂̕`��t���O
			bool actionRangeDraw,animationFlag;
			if(index==0){
				//�擪���j�b�g�̂����`��t���O�𗧂Ă�
				actionRangeDraw=true;
				animationFlag=true;
			} else{
				actionRangeDraw=false;
				animationFlag=false;
			}
			//�ދp�������j�b�g(m_fix��e_ignore)�͕`�悵�Ȃ�
			minimapInfo.GetUnitList()[index].DrawUnit(minimapInfo.GetUnitList()[index].getPos(),startPos,minimapRate,0,animationFlag,true,actionRangeDraw);
		}
	}
	//���j�b�g��HP�Q�[�W�̕`��
	for(size_t i=1,siz=minimapInfo.GetUnitList().size();i<siz+1;i++){
		const size_t index=i%siz;
		if(m_battleSceneData->m_mapRange->JudgeInShapeRect(&minimapInfo.GetUnitList()[index])){
			//�E�C���h�E�ɓ����Ă��Ȃ����͕`�悵�Ȃ�
			minimapInfo.GetUnitList()[index].DrawHPGage(minimapInfo.GetUnitList()[index].getPos(),startPos,minimapRate);
		}
	}
	//���샆�j�b�g�̃}�[�J�[��\��
	if(minimapInfo.pOperateUnit!=nullptr){
		float dx,dy;
		GetGraphSizeF(m_operateCursor,&dx,&dy);
		const Vector2D pos=(minimapInfo.pOperateUnit->getPos()+Vector2D(0.0f,-Unit::unitCircleSize+10.0f))*minimapRate+startPos-Vector2D(dx/2.0f,dy);
		DrawGraph((int)pos.x,(int)pos.y,m_operateCursor,TRUE);
	}
	//�_���[�W�̕\��
	if(minimapInfo.pOperateUnit!=nullptr && minimapInfo.pAttackedUnit!=nullptr){
		//�U�����́A�U������̏�Ƀ_���[�W��\��
		const Vector2D pos=minimapInfo.pAttackedUnit->getPos()*minimapRate+startPos;
		minimapInfo.pOperateUnit->GetBattleStatus().weapon->DrawPredict((int)pos.x,(int)pos.y,m_predictExplainFont,m_predictNumberFont,minimapInfo.pOperateUnit,minimapInfo.pAttackedUnit);
	}
}

int SubmissionReflectionScene::thisCalculate(){
	if(keyboard_get(KEY_INPUT_Z)==1 || mouse_get(MOUSE_INPUT_LEFT)==1){
		return SceneKind::e_clear;
	}

	return SceneKind::e_submissionReflection;
}

void SubmissionReflectionScene::thisDraw()const{
	//���͂̕`��
	DrawStringToHandle(30,30,"�ǂ���̕����ǂ��Ƃ����s���ł��傤���H",GetColor(255,255,255),GeneralPurposeResource::popLargeFont);
	//��r�}�b�v�̕`��
	if(m_goodLogInfo.has_value()){
		DrawResizedMap(20,300,m_goodLogInfo.value());
	}
	if(m_badLogInfo.has_value()){
		DrawResizedMap(980,300,m_badLogInfo.value());
	}
}

int SubmissionReflectionScene::UpdateNextScene(int index){
	switch(index){
	case(SceneKind::e_clear):
		m_nextScene=m_clearScene;
		m_clearScene.reset();//m_nextScene�ɃR�s�[�����̂ŁA���L�������
		break;
	}
	return index;
}

void SubmissionReflectionScene::ReturnProcess(){
	//���ɉ������Ȃ�
}
