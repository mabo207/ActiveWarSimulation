#define _USE_MATH_DEFINES
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
//���t���N�V���������ꗗ
#include"LineDraw.h"
#include"ObjectClick.h"
#include"SelectOne.h"
#include"MoveSimulation.h"
//���z�}�`�\�z�̂��߂ɕK�v�Ȃ���
#include"Edge.h"
#include"MyPolygon.h"
#include<cmath>

namespace {
	const int lineWidth=5;
	const unsigned int merginColor=GetColor(128,128,255);
	const float twoMinimapRate=0.5f;
	const float oneMinimapRate=0.8f;
	const Vector2D minimapPos[2]={Vector2D(20.0f,300.0f),Vector2D(980.0f,300.0f)};
	const int minimapX[2]={(int)minimapPos[0].x,(int)minimapPos[1].x};
	const int minimapY[2]={(int)minimapPos[0].y,(int)minimapPos[1].y};
}
//---------------SubmissionReflectionScene::MinimapDrawInfo-----------------
SubmissionReflectionScene::MinimapDrawInfo::MinimapDrawInfo(const std::shared_ptr<const LogElement> &log,Unit::Team::Kind phase)
	:pOperateUnit(nullptr),pAttackedUnit(nullptr)
{
	if(log){
		//log�̗L��������
		//�֐��̍쐬
		const std::shared_ptr<const WaitLog> waitLog=std::dynamic_pointer_cast<const WaitLog>(log);
		const std::shared_ptr<const AttackLog> attackLog=std::dynamic_pointer_cast<const AttackLog>(log);
		std::function<const Unit *()> getOperateUnit,getAttackedUnit;
		std::function<const std::vector<RouteInfo>()> getRoute;
		if(log->GetLogKind()==LogElement::LogKind::e_wait && waitLog){
			//�ҋ@���O�̏ꍇ
			getOperateUnit=[&waitLog](){return waitLog->GetOperateUnitData().punit;};
			getRoute=[&waitLog](){return waitLog->GetRoute();};
		} else if(log->GetLogKind()==LogElement::LogKind::e_attack && attackLog){
			//�U�����O�̏ꍇ
			getOperateUnit=[&attackLog](){return attackLog->GetOperateUnitData().punit;};
			getAttackedUnit=[&attackLog](){return attackLog->GetAimedUnit();};
			getRoute=[&attackLog](){return attackLog->GetRoute();};
		}
		//�Ֆʂ̃��j�b�g�ꗗ�̍쐬
		const size_t maxSize=log->m_unitDataList.size();
		size_t operateIndex=maxSize,attackedIndex=maxSize,index=0;//pOperateUnit��pAttackedUnit�̒T���ɗp����
		for(const LogElement::UnitLogData &logData:log->m_unitDataList){
			//���j�b�g�f�[�^�̃R�s�[�i���H�����邽�߁j
			Unit u=*logData.punit;
			//���j�b�g�f�[�^�̉��H
			u.Warp(logData.pos);
			u.AddHP(logData.hp-u.GetBattleStatus().HP);
			u.SetOP(logData.op);
			u.SetPenetratable(phase);
			//operateIndex,attackedIndex�̒T���A�֐������݂��邩�ǂ����C������
			if(getOperateUnit && logData.punit==getOperateUnit()){
				operateIndex=index;
				u.SetPenetratable(u.GetBattleStatus().team);//�s�����j�b�g�ɂ��ẮA�N���s�͈͂���ɍL���Ȃ���Ԃɂ���i���Â炢���߁j
			}
			if(getAttackedUnit && logData.punit==getAttackedUnit()){
				attackedIndex=index;
			}
			//�i�[
			unitList.push_back(u);
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
		//�ړ��o�H�̍쐬
		if(getRoute){
			for(const RouteInfo &r:getRoute()){
				route.push_back(r);
			}
		}
	}
}

SubmissionReflectionScene::MinimapDrawInfo::~MinimapDrawInfo(){}

//---------------SubmissionReflectionScene::ExtendDraw------------------
void SubmissionReflectionScene::ExtendDraw::DrawMinimap()const{
	m_drawFunc(m_startPos.GetX(),m_startPos.GetY(),(float)(1.0*m_width.GetX()/CommonConstParameter::mapSizeX));
}

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
	//�ˎ�̈��S�n�тɂ��Ẵ��[�N�Ȃ̂ŁA�G�t�F�[�Y�̐N���s�͈͐ݒ�ɂ���
	const Unit::Team::Kind phaseSetting=Unit::Team::e_enemy;//�T�u�~�b�V�����ɂ���Ă����̃t�F�[�Y�ݒ肪���߂���
	m_goodLogInfo.emplace(reflectionInfo.m_goodLog.second,phaseSetting);
	m_badLogInfo.emplace(reflectionInfo.m_badLog.second,phaseSetting);
	//m_reflectionWork�̏�����
	InitReflectionWork();
}

SubmissionReflectionScene::~SubmissionReflectionScene(){
	DeleteGraphEX(m_operateCursor);
	DeleteFontToHandleEX(m_predictNumberFont);
	DeleteFontToHandleEX(m_predictExplainFont);
}

void SubmissionReflectionScene::DrawResizedMap(int x,int y,const MinimapDrawInfo &minimapInfo,const float rate)const{
	//�}�[�W���̕`��
	const int minimapWidth=(int)(CommonConstParameter::mapSizeX*rate);
	const int minimapHeight=(int)(CommonConstParameter::mapSizeY*rate);
	DrawBox(x-lineWidth,y-lineWidth,x+minimapWidth+lineWidth,y+minimapHeight+lineWidth,merginColor,TRUE);
	const Vector2D startPos((float)x,(float)y);
	//BattleSceneData�̕`��֐��͊g��k���`��ɑΉ����Ă��Ȃ��̂ŁA�Ǝ��Ɏ�������
	//�w�i�`��
	DrawExtendGraphExRateAssign(startPos.x,startPos.y,rate,m_battleSceneData->m_mapPic,TRUE);
	//���[�g�̕`��
	for(size_t i=0,siz=minimapInfo.GetRoute().size();i+1<siz;i++){
		//�o�H�̐����̍��W���擾
		Vector2D pos[2]={minimapInfo.GetRoute()[i].pos,minimapInfo.GetRoute()[i+1].pos};
		//�~�j�}�b�v�p�Ɉʒu�����H
		for(Vector2D &p:pos){
			p=startPos+p*rate;
		}
		//������`��
		DrawLineAA(pos[0].x,pos[0].y,pos[1].x,pos[1].y,GetColor(255,255,0),2.0f);
	}
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
			minimapInfo.GetUnitList()[index].DrawUnit(minimapInfo.GetUnitList()[index].getPos(),startPos,rate,0,animationFlag,true,actionRangeDraw);
		}
	}
	//���j�b�g��HP�Q�[�W�̕`��
	for(size_t i=1,siz=minimapInfo.GetUnitList().size();i<siz+1;i++){
		const size_t index=i%siz;
		if(m_battleSceneData->m_mapRange->JudgeInShapeRect(&minimapInfo.GetUnitList()[index])){
			//�E�C���h�E�ɓ����Ă��Ȃ����͕`�悵�Ȃ�
			minimapInfo.GetUnitList()[index].DrawHPGage(minimapInfo.GetUnitList()[index].getPos(),startPos,rate);
		}
	}
	//���샆�j�b�g�̃}�[�J�[��\��
	if(minimapInfo.GetOperateUnit()!=nullptr){
		float dx,dy;
		GetGraphSizeF(m_operateCursor,&dx,&dy);
		const Vector2D pos=(minimapInfo.GetOperateUnit()->getPos()+Vector2D(0.0f,-Unit::unitCircleSize+10.0f))*rate+startPos-Vector2D(dx/2.0f,dy);
		DrawGraph((int)pos.x,(int)pos.y,m_operateCursor,TRUE);
	}
	//�_���[�W�̕\��
	if(minimapInfo.GetOperateUnit()!=nullptr && minimapInfo.GetAttackedUnit()!=nullptr){
		//�U�����́A�U������̏�Ƀ_���[�W��\��
		const Vector2D pos=minimapInfo.GetAttackedUnit()->getPos()*rate+startPos;
		minimapInfo.GetOperateUnit()->GetBattleStatus().weapon->DrawPredict((int)pos.x,(int)pos.y,m_predictExplainFont,m_predictNumberFont,minimapInfo.GetOperateUnit(),minimapInfo.GetAttackedUnit());
	}
}

void SubmissionReflectionScene::DrawTwoMinimap()const{
	if(m_goodLogInfo.has_value()){
		DrawResizedMap(minimapX[0],minimapY[0],m_goodLogInfo.value(),twoMinimapRate);
	}
	if(m_badLogInfo.has_value()){
		DrawResizedMap(minimapX[1],minimapY[1],m_badLogInfo.value(),twoMinimapRate);
	}
}

int SubmissionReflectionScene::thisCalculate(){
	if(m_reflectionWork){
		//���[�N���͏���
		m_reflectionWork->Update();
		//���[�N�؂�ւ�����
		if(m_reflectionWork->WorkClear()){
			//���[�N���N���A������
			m_reflectionWork.reset();
		}
	} else{
		//���[�N���ݒ肳��Ă��Ȃ����ɁA�J�ڏ������s��
		if(keyboard_get(KEY_INPUT_Z)==1 || mouse_get(MOUSE_INPUT_LEFT)==1){
			return SceneKind::e_clear;
		}
	}
	if(m_layoutInfo){
		//�}�b�v�`�揈���̍X�V
		m_layoutInfo->Update();
	}

	return SceneKind::e_submissionReflection;
}

void SubmissionReflectionScene::thisDraw()const{
	//���͂̕`��
	if(m_reflectionWork){
		DrawStringToHandle(30,30,m_reflectionWork->GetQuestion().c_str(),GetColor(255,255,255),GeneralPurposeResource::popLargeFont);
	} else{
		DrawStringToHandle(30,30,"�ǂ���̕����ǂ��Ƃ����s���ł��傤���H",GetColor(255,255,255),GeneralPurposeResource::popLargeFont);
	}
	//��r�}�b�v�̕`��
	if(m_layoutInfo){
		m_layoutInfo->DrawMinimap();
	}
	//���[�N�ɂ��Ă̕`��
	if(m_reflectionWork){
		m_reflectionWork->WorkDraw();
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

void SubmissionReflectionScene::InitReflectionWork(){
	SetMoveSimulationWork();
}

//���[�N�쐬�֐�
void SubmissionReflectionScene::SetDrawLineWork(){
	//�U�����j�b�g�Ɣ�U�����j�b�g�����Ԑ����������[�N
	const Vector2D goodLogStart=minimapPos[0]+m_goodLogInfo->GetOperateUnit()->getPos()*twoMinimapRate;
	const Vector2D goodLogEnd=minimapPos[0]+m_goodLogInfo->GetAttackedUnit()->getPos()*twoMinimapRate;
	const Vector2D badLogStart=minimapPos[1]+m_badLogInfo->GetOperateUnit()->getPos()*twoMinimapRate;
	const Vector2D badLogEnd=minimapPos[1]+m_badLogInfo->GetAttackedUnit()->getPos()*twoMinimapRate;
	m_reflectionWork=std::shared_ptr<ReflectionWork::Base>(new ReflectionWork::LineDraw(
		{Edge(goodLogStart,goodLogEnd-goodLogStart,Shape::Fix::e_ignore)
		,Edge(badLogStart,badLogEnd-badLogStart,Shape::Fix::e_ignore)}
		,"�U�������L�����ƓG�Ƃ̋�������������Ċm�F���Ă݂悤�I"
	));
	//�}�b�v�̕`��̎d����ݒ�
	const auto drawFunc=[this](){
		DrawTwoMinimap();
	};
	m_layoutInfo=std::shared_ptr<MinimapLayoutBase>(new NormalDraw(drawFunc));
}

void SubmissionReflectionScene::SetClickWork(const std::function<std::shared_ptr<const Shape>(Vector2D,Vector2D)> &conditionShapeFunc){
	std::vector<std::shared_ptr<const Shape>> shapeList;
	const auto addFunc=[&shapeList,this](std::shared_ptr<Shape> &addShape,const Vector2D &minimapPosition,const std::shared_ptr<const Shape> &conditionShape,const std::shared_ptr<const Shape> &attackedUnitShape){
		//�n�}�ɍ����悤�ɉ��H
		const Vector2D pos=addShape->GetPosition();//���݈ʒu�A�k���}�b�v��̈ʒu���w�肷�邽�߂ɂ����p���Ĉړ������Ȃ��Ƃ����Ȃ�
		addShape->Move(minimapPosition+pos*twoMinimapRate-pos);
		addShape->Resize(addShape->GetRetResize()*twoMinimapRate);
		//�����t���ǉ�
		if(conditionShape->JudgeCross(addShape.get()) || conditionShape->JudgeInShape(addShape.get())){
			//�uaddShape��conditionShape���Ɋ��S�ɓ����Ă���v�������́u��_�����v�ꍇ�̂݃N���b�N�}�`���X�g�ɒǉ�
			if(!addShape->JudgeInShape(attackedUnitShape.get())){
				//�������A���j�b�g���܂���ƒ��ɓ���Ă���}�`�i�O�ǁj�͏��O�i�O�ǂ̓N���b�NUI�����̂������Q���Ƃ��ĔF������Â炢����j
				shapeList.push_back(addShape);
			}
		}
	};
	const auto addMinimapObject0=[&conditionShapeFunc,&addFunc,this](std::shared_ptr<Shape> &shape){
		//���j�b�g���m�����Ԑ����̒[�_
		const Vector2D p0=minimapPos[0]+m_goodLogInfo->GetAttackedUnit()->getPos()*twoMinimapRate
			,p1=minimapPos[0]+m_goodLogInfo->GetOperateUnit()->getPos()*twoMinimapRate;
		//�U�����ꂽ���j�b�g�̓����蔻��}�`�̍쐬
		std::shared_ptr<Shape> attackedUnitShape=m_goodLogInfo->GetOperateUnit()->GetHitJudgeShape()->VCopy();
		const Vector2D pos=attackedUnitShape->GetPosition();
		attackedUnitShape->Move(minimapPos[0]+pos*twoMinimapRate-pos);
		attackedUnitShape->Resize(attackedUnitShape->GetRetResize()*twoMinimapRate);
		//�}�`���쐬���ď��������shape�����X�g�ɒǉ�
		addFunc(shape,minimapPos[0],conditionShapeFunc(p0,p1),attackedUnitShape);
	};
	const auto addMinimapObject1=[&conditionShapeFunc,&addFunc,this](std::shared_ptr<Shape> &shape){
		//���j�b�g���m�����Ԑ����̒[�_
		const Vector2D p0=minimapPos[1]+m_badLogInfo->GetAttackedUnit()->getPos()*twoMinimapRate
			,p1=minimapPos[1]+m_badLogInfo->GetOperateUnit()->getPos()*twoMinimapRate;
		//�U�����ꂽ���j�b�g�̓����蔻��}�`�̍쐬
		std::shared_ptr<Shape> attackedUnitShape=m_badLogInfo->GetOperateUnit()->GetHitJudgeShape()->VCopy();
		const Vector2D pos=attackedUnitShape->GetPosition();
		attackedUnitShape->Move(minimapPos[1]+pos*twoMinimapRate-pos);
		attackedUnitShape->Resize(attackedUnitShape->GetRetResize()*twoMinimapRate);
		//�}�`���쐬���ď��������shape�����X�g�ɒǉ�
		addFunc(shape,minimapPos[1],conditionShapeFunc(p0,p1),attackedUnitShape);
	};
	//���j�b�g�f�[�^�ȊO�̏�Q���̊i�[
	for(const BattleObject *object:m_battleSceneData->m_field){
		if(object->GetType()!=BattleObject::Type::e_unit){
			//�����蔻��}�`�������o���Ēǉ�
			addMinimapObject0(object->GetHitJudgeShape()->VCopy());
			addMinimapObject1(object->GetHitJudgeShape()->VCopy());
		}
	}
	//���j�b�g�f�[�^�̊i�[
	for(size_t i=0,siz=m_goodLogInfo->GetUnitList().size();i<siz;i++){
		//�����蔻��}�`�������o���Ēǉ�
		if(&m_goodLogInfo->GetUnitList()[i]!=m_goodLogInfo->GetOperateUnit() && &m_goodLogInfo->GetUnitList()[i]!=m_goodLogInfo->GetAttackedUnit()){
			addMinimapObject0(m_goodLogInfo->GetUnitList()[i].GetHitJudgeShape()->VCopy());
		}
	}
	for(size_t i=0,siz=m_badLogInfo->GetUnitList().size();i<siz;i++){
		//�����蔻��}�`�������o���Ēǉ�
		if(&m_badLogInfo->GetUnitList()[i]!=m_badLogInfo->GetOperateUnit() && &m_badLogInfo->GetUnitList()[i]!=m_badLogInfo->GetAttackedUnit()){
			addMinimapObject1(m_badLogInfo->GetUnitList()[i].GetHitJudgeShape()->VCopy());
		}
	}
	//���[�N�̍쐬
	m_reflectionWork=std::shared_ptr<ReflectionWork::Base>(new ReflectionWork::ObjectClick(shapeList,"�U�������G�̈ړ����ז����Ă����Q�����N���b�N���Ă݂悤�I"));
	//�}�b�v�̕`��̎d����ݒ�
	const auto drawFunc=[this](){
		DrawTwoMinimap();
	};
	m_layoutInfo=std::shared_ptr<MinimapLayoutBase>(new NormalDraw(drawFunc));
}

void SubmissionReflectionScene::SetLineClickWork(){
	//�U�����j�b�g�Ɣ�U�����j�b�g�����Ԑ���̏�Q�����N���b�N���郏�[�N
	//�}�`�쐬�֐��̍쐬
	const auto createFunc=[](Vector2D p0,Vector2D p1){
		return std::shared_ptr<Shape>(new Edge(p0,p1-p0,Shape::Fix::e_dynamic));
	};
	//���[�N�쐬
	SetClickWork(createFunc);
}

void SubmissionReflectionScene::SetAreaClickWork(){
	//�U�����j�b�g�Ɣ�U�����j�b�g�����Ԑ�����Ίp���Ƃ����H�`�̈�̏�Q�����N���b�N���郏�[�N
	//�}�`�쐬�֐��̍쐬
	const auto createFunc=[this](Vector2D p0,Vector2D p1){
		//p0p1�ɐ����ȃx�N�g���ŁAp0p1�̒��_����h�����i�񂾏��ɂ���_��p2�Ƃ����|p0p2|+|p1p2|��pAttackedUnit�̈ړ������ɂȂ�悤�ȃx�N�g��
		Vector2D h=(p1-p0).turn((float)M_PI_4);
		const float unitMoveDistance=m_goodLogInfo->GetAttackedUnit()->GetMaxMoveDistance();
		h=h.norm()*std::powf(unitMoveDistance*unitMoveDistance-h.sqSize(),0.5f)*0.5f;
		return std::shared_ptr<Shape>(new MyPolygon(p0-h,{p0+h,p1+h,p1-h},Shape::Fix::e_dynamic));
	};
	//���[�N�쐬
	SetClickWork(createFunc);
}

void SubmissionReflectionScene::SetSelectOneWork(){
	//���[�N�̍쐬
	const float minimapWidth=(float)CommonConstParameter::mapSizeX*twoMinimapRate;
	const float minimapHeight=(float)CommonConstParameter::mapSizeY*twoMinimapRate;
	const std::shared_ptr<Shape> correct(new MyPolygon(MyPolygon::CreateRectangle(minimapPos[0],Vector2D(minimapWidth,minimapHeight),Shape::Fix::e_static)));
	const std::shared_ptr<Shape> incorrect(new MyPolygon(MyPolygon::CreateRectangle(minimapPos[1],Vector2D(minimapWidth,minimapHeight),Shape::Fix::e_static)));
	m_reflectionWork=std::shared_ptr<ReflectionWork::Base>(new ReflectionWork::SelectOne(correct,{incorrect},"�ǂ���̕����K�����s���ł��傤���H"));
	//�}�b�v�̕`��̎d����ݒ�
	const auto drawFunc=[this](){
		DrawTwoMinimap();
	};
	m_layoutInfo=std::shared_ptr<MinimapLayoutBase>(new NormalDraw(drawFunc));
}

void SubmissionReflectionScene::SetMoveSimulationWork(){
	//�U���L�����̈ʒu��ς��Ă݂ĕ]�����ǂ��Ȃ邩���V�~�����[�V�����w�K���Ă݂郏�[�N
	if(m_badLogInfo.has_value()){
		//�t�B�[���h�̍쐬
		std::vector<BattleObject *> field;
		for(BattleObject *obj:m_battleSceneData->m_field){
			//�S�Ă̏�Q����ǉ�
			if(obj->GetType()==BattleObject::Type::e_terrain){
				field.push_back(obj);
			}
		}
		for(size_t i=0;i<m_badLogInfo->GetUnitList().size();i++){
			field.push_back(m_badLogInfo->GetUnitListPtr(i));
		}
		//���[�N�̐ݒ�
		m_reflectionWork=std::shared_ptr<ReflectionWork::Base>(new
			ReflectionWork::MoveSimulation(field
				,m_badLogInfo->GetOperateUnit()
				,m_battleSceneData->m_stageSize
				,m_badLogInfo->GetAttackedUnit()
				,minimapPos[0]
				,oneMinimapRate
				,m_battleSceneData->m_scoreObserver->GetSubmission().GetRule()
				,"�V�~�����[�V�����w�K"));
		//�}�b�v�̕`��̎d����ݒ�
		const Easing::TYPE type=Easing::TYPE_IN;
		const Easing::FUNCTION function=Easing::FUNCTION_QUAD;
		const double degree=4.0;
		const int maxFrame=30;
		const auto drawFunc=[this](int x,int y,float rate){
			if(m_badLogInfo.has_value()){
				DrawResizedMap(x,y,m_badLogInfo.value(),rate);
			}
		};
		m_layoutInfo=std::shared_ptr<MinimapLayoutBase>(new ExtendDraw(drawFunc
			,PositionControl(minimapPos[1],minimapPos[0],maxFrame,type,function,degree)
			,Easing((int)(CommonConstParameter::mapSizeX*twoMinimapRate),(int)(CommonConstParameter::mapSizeX*oneMinimapRate),maxFrame,type,function,degree)));
	}
}
