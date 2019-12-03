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
#include"ReadExplanation.h"
//���z�}�`�\�z�̂��߂ɕK�v�Ȃ���
#include"Edge.h"
#include"MyPolygon.h"
#include"Circle.h"
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
			u.SetFix(Shape::Fix::e_static);
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
	,m_predictNumberFont(CreateFontToHandleEX("���C���I",56,8,DX_FONTTYPE_EDGE,-1,4))
	,m_predictExplainFont(CreateFontToHandleEX("���C���I",20,2,DX_FONTTYPE_EDGE,-1,2))
	,m_nowWork(std::shared_ptr<ReflectionWork::Base>(),std::shared_ptr<MinimapLayoutBase>())
{
	//m_goodLogInfo,m_badLogInfo�̏�����
	const WholeReflectionInfo reflectionInfo=m_battleSceneData->m_scoreObserver->GetSubmission().GetReflectionInfo();
	//�ˎ�̈��S�n�тɂ��Ẵ��[�N�Ȃ̂ŁA�G�t�F�[�Y�̐N���s�͈͐ݒ�ɂ���
	const Unit::Team::Kind phaseSetting=Unit::Team::e_enemy;//�T�u�~�b�V�����ɂ���Ă����̃t�F�[�Y�ݒ肪���߂���
	m_goodLogInfo.emplace(reflectionInfo.m_goodLog.second,phaseSetting);
	m_badLogInfo.emplace(reflectionInfo.m_badLog.second,phaseSetting);
	//m_workMethodList�̏�����
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
		const int centerX=(int)(pos.x),centerY=(int)(pos.y);
		minimapInfo.GetOperateUnit()->GetBattleStatus().weapon->DrawExtendPredict((int)pos.x,(int)pos.y,rate,m_predictExplainFont,m_predictNumberFont,minimapInfo.GetOperateUnit(),minimapInfo.GetAttackedUnit());
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

void SubmissionReflectionScene::SetUnitPenetratable(Unit::Team::Kind phase){
	if(m_goodLogInfo.has_value()){
		for(size_t i=0,siz=m_goodLogInfo->GetUnitList().size();i<siz;i++){
			Unit *pu=m_goodLogInfo->GetUnitListPtr(i);
			if(pu!=m_goodLogInfo->GetOperateUnit() && pu!=nullptr){
				//���샆�j�b�g�͎��F������̂��߂ɕύX�ł��Ȃ��悤�ɂ���
				pu->SetPenetratable(phase);
			}
		}
	}
	if(m_badLogInfo.has_value()){
		for(size_t i=0,siz=m_badLogInfo->GetUnitList().size();i<siz;i++){
			Unit *pu=m_badLogInfo->GetUnitListPtr(i);
			if(pu!=m_badLogInfo->GetOperateUnit() && pu!=nullptr){
				//���샆�j�b�g�͎��F������̂��߂ɕύX�ł��Ȃ��悤�ɂ���
				pu->SetPenetratable(phase);
			}
		}
	}
}

int SubmissionReflectionScene::thisCalculate(){
	//�X�V����
	//���[�N���͏���
	if(m_nowWork.reflection){
		m_nowWork.reflection->Update();
	}
	if(m_nowWork.minimap){
		//�}�b�v�`�揈���̍X�V
		m_nowWork.minimap->Update();
	}
	if(m_nowWork.reflection->WorkClear()){
		//���[�N���N���A������
		if(m_nextWorkMethod==m_workMethodList.end()){
			//�Ō�̃��[�N��������
			return SceneKind::e_clear;
		} else{
			//�܂����[�N���c���Ă���Ȃ�
			m_nowWork=(*m_nextWorkMethod)();//���[�N�X�V
			m_nextWorkMethod++;//���̃��[�N�쐬���@���X�V
		}
	}

	return SceneKind::e_submissionReflection;
}

void SubmissionReflectionScene::thisDraw()const{
	//���͂̕`��
	if(m_nowWork.reflection){
		DrawStringToHandle(30,30,m_nowWork.reflection->GetQuestion().c_str(),GetColor(255,255,255),GeneralPurposeResource::popLargeFont);
	} else{
		DrawStringToHandle(30,30,"�ǂ���̕����ǂ��Ƃ����s���ł��傤���H",GetColor(255,255,255),GeneralPurposeResource::popLargeFont);
	}
	//��r�}�b�v�̕`��
	if(m_nowWork.minimap){
		m_nowWork.minimap->DrawMinimap();
	}
	//���[�N�ɂ��Ă̕`��
	if(m_nowWork.reflection){
		m_nowWork.reflection->WorkDraw();
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
	//���[�N�ꗗ�̍쐬
	const auto twoMinimapDrawFunc=[this](){
		DrawTwoMinimap();
	};
	const auto oneMinimapDrawFunc=[this](){
		if(m_badLogInfo.has_value()){
			DrawResizedMap(minimapX[0],minimapY[0],m_badLogInfo.value(),oneMinimapRate);
		}
	};
	AddAreaClickWork(std::vector<ShapeClickWorkInfo>{ShapeClickWorkInfo(&m_goodLogInfo,minimapPos[0],twoMinimapRate),ShapeClickWorkInfo(&m_badLogInfo,minimapPos[1],twoMinimapRate)}
		,std::shared_ptr<MinimapLayoutBase>(new NormalDraw(twoMinimapDrawFunc))
		,Unit::Team::e_enemy);
	AddMoveSimulationWork(Unit::Team::e_player);//�v���C���[���j�b�g�𓮂������߁Ae_player���w��
	AddAreaClickWork(std::vector<ShapeClickWorkInfo>{ShapeClickWorkInfo(&m_badLogInfo,minimapPos[0],oneMinimapRate)}
		,std::shared_ptr<MinimapLayoutBase>(new NormalDraw(oneMinimapDrawFunc))
		,Unit::Team::e_enemy);
	//���[�N�ݒ�
	m_nextWorkMethod=m_workMethodList.begin();
	m_nowWork=(*m_nextWorkMethod)();
	m_nextWorkMethod++;
}

//���[�N�쐬�֐�
void SubmissionReflectionScene::AddDrawLineWork(Unit::Team::Kind phase){
	//�U�����j�b�g�Ɣ�U�����j�b�g�����Ԑ����������[�N
	const auto lineWorkMethod=[this,phase](){
		//���j�b�g�̐N���ۂ�G�t�F�[�Y�p�ɐݒ肵�Ă���
		this->SetUnitPenetratable(phase);
		//�����̈ʒu�̐ݒ�
		const Vector2D goodLogStart=minimapPos[0]+m_goodLogInfo->GetOperateUnit()->getPos()*twoMinimapRate;
		const Vector2D goodLogEnd=minimapPos[0]+m_goodLogInfo->GetAttackedUnit()->getPos()*twoMinimapRate;
		const Vector2D badLogStart=minimapPos[1]+m_badLogInfo->GetOperateUnit()->getPos()*twoMinimapRate;
		const Vector2D badLogEnd=minimapPos[1]+m_badLogInfo->GetAttackedUnit()->getPos()*twoMinimapRate;
		const std::shared_ptr<ReflectionWork::Base> work=std::shared_ptr<ReflectionWork::Base>(new ReflectionWork::LineDraw(
			{Edge(goodLogStart,goodLogEnd-goodLogStart,Shape::Fix::e_ignore)
			,Edge(badLogStart,badLogEnd-badLogStart,Shape::Fix::e_ignore)}
			,"�U�������L�����ƓG�Ƃ̋�������������Ċm�F���Ă݂悤�I"
		));
		//�}�b�v�̕`��̎d����ݒ�
		const auto drawFunc=[this](){
			DrawTwoMinimap();
		};
		const std::shared_ptr<MinimapLayoutBase> minimap=std::shared_ptr<MinimapLayoutBase>(new NormalDraw(drawFunc));
		return WorkInfo(work,minimap);
	};
	m_workMethodList.push_back(lineWorkMethod);
}

void SubmissionReflectionScene::AddShapeClickWork(const std::function<std::shared_ptr<const Shape>(Vector2D,Vector2D)> conditionShapeFunc
	,std::vector<ShapeClickWorkInfo> minimapInfo
	,const std::shared_ptr<MinimapLayoutBase> minimapLayout
	,Unit::Team::Kind phase)
{
	const auto clickWorkMethod=[minimapInfo,conditionShapeFunc,minimapLayout,phase,this](){
		//����
		//���j�b�g�̐N���ۂ�G�t�F�[�Y�p�ɐݒ肵�Ă���
		this->SetUnitPenetratable(phase);
		std::vector<std::shared_ptr<const Shape>> shapeList;
		//�֐��̍쐬
		const auto addFunc=[&shapeList,this](std::shared_ptr<Shape> &addShape,const Vector2D &minimapPosition,const std::shared_ptr<const Shape> &conditionShape,const std::shared_ptr<const Shape> &attackedUnitShape,float rate){
			//�n�}�ɍ����悤�ɉ��H
			const Vector2D pos=addShape->GetPosition();//���݈ʒu�A�k���}�b�v��̈ʒu���w�肷�邽�߂ɂ����p���Ĉړ������Ȃ��Ƃ����Ȃ�
			addShape->Move(minimapPosition+pos*rate-pos);
			addShape->Resize(addShape->GetRetResize()*rate);
			//�����t���ǉ�
			if(conditionShape->JudgeCross(addShape.get()) || conditionShape->JudgeInShape(addShape.get())){
				//�uaddShape��conditionShape���Ɋ��S�ɓ����Ă���v�������́u��_�����v�ꍇ�̂݃N���b�N�}�`���X�g�ɒǉ�
				if(!addShape->JudgeInShape(attackedUnitShape.get())){
					//�������A���j�b�g���܂���ƒ��ɓ���Ă���}�`�i�O�ǁj�͏��O�i�O�ǂ̓N���b�NUI�����̂������Q���Ƃ��ĔF������Â炢����j
					shapeList.push_back(addShape);
				}
			}
		};
		const auto addMinimapObject=[&addFunc,&conditionShapeFunc](std::shared_ptr<Shape> &shape,const ShapeClickWorkInfo &minimapInfo){
			//���j�b�g���m�����Ԑ����̒[�_
			const Vector2D p0=minimapInfo.startPos+minimapInfo.drawInfo->value().GetAttackedUnit()->getPos()*minimapInfo.rate,
				p1=minimapInfo.startPos+minimapInfo.drawInfo->value().GetOperateUnit()->getPos()*minimapInfo.rate;
			//�����}�`�̍쐬
			const std::shared_ptr<const Shape> conditionShape=conditionShapeFunc(p0,p1);
			//�U�����ꂽ���j�b�g�̓����蔻��}�`�̍쐬
			std::shared_ptr<Shape> attackedUnitShape=minimapInfo.drawInfo->value().GetOperateUnit()->GetHitJudgeShape()->VCopy();
			const Vector2D pos=attackedUnitShape->GetPosition();
			attackedUnitShape->Move(minimapInfo.startPos+pos*minimapInfo.rate-pos);
			attackedUnitShape->Resize(attackedUnitShape->GetRetResize()*minimapInfo.rate);
			//�}�`���쐬���ď��������shape�����X�g�ɒǉ�
			addFunc(shape,minimapInfo.startPos,conditionShape,attackedUnitShape,minimapInfo.rate);
		};
		//���j�b�g�f�[�^�ȊO�̏�Q���̊i�[
		for(const BattleObject *object:m_battleSceneData->m_field){
			if(object->GetType()!=BattleObject::Type::e_unit){
				//�����蔻��}�`�������o���Ēǉ�
				for(const ShapeClickWorkInfo &info:minimapInfo){
					addMinimapObject(object->GetHitJudgeShape()->VCopy(),info);
				}
			}
		}
		//���j�b�g�f�[�^�̊i�[
		for(const ShapeClickWorkInfo &info:minimapInfo){
			for(size_t i=0,siz=info.drawInfo->value().GetUnitList().size();i<siz;i++){
				//�����蔻��}�`�������o���Ēǉ�
				if(&info.drawInfo->value().GetUnitList()[i]!=info.drawInfo->value().GetOperateUnit() && &info.drawInfo->value().GetUnitList()[i]!=info.drawInfo->value().GetAttackedUnit()){
					addMinimapObject(info.drawInfo->value().GetUnitList()[i].GetHitJudgeShape()->VCopy(),info);
				}
			}
		}
		//���[�N�̍쐬
		const std::shared_ptr<ReflectionWork::Base> clickWork=std::shared_ptr<ReflectionWork::Base>(new ReflectionWork::ObjectClick(shapeList,"�U�������G�̈ړ����ז����Ă����Q�����N���b�N���Ă݂悤�I"));
		return WorkInfo(clickWork,minimapLayout);
	};
	m_workMethodList.push_back(clickWorkMethod);
	//������[�N�̍쐬
	const auto explanationWorkMethod=[minimapInfo,minimapLayout,this](){
		std::vector<std::pair<std::shared_ptr<const Shape>,unsigned int>> assistList;
		//�G�̈ړ��͈͂ɓ����Ă���i�q�_��S�ĕ⏕�֐��ɉ�����
		minimapInfo[0].drawInfo->value().GetUnitListPtr(0);
		for(const ShapeClickWorkInfo &mapinfo:minimapInfo){
			std::vector<BattleObject *> field;
			std::shared_ptr<LatticeBattleField> lField;
			std::vector<LatticeBattleField::LatticeDistanceInfo> dField;
			float moveDistance;
			const unsigned int pointColor=GetColor(0,128,64);
			//�t�B�[���h�̍쐬
			for(BattleObject *obj:m_battleSceneData->m_field){
				//�S�Ă̏�Q����ǉ�
				if(obj->GetType()==BattleObject::Type::e_terrain){
					field.push_back(obj);
				}
			}
			for(size_t i=0;i<mapinfo.drawInfo->value().GetUnitList().size();i++){
				Unit *pu=mapinfo.drawInfo->value().GetUnitListPtr(i);
				if(pu!=mapinfo.drawInfo->value().GetAttackedUnit() && pu!=mapinfo.drawInfo->value().GetOperateUnit()){
					field.push_back(pu);
				}
			}
			//�i�q�_�}�b�v�̍쐬
			lField=LatticeBattleField::Create(field,m_battleSceneData->m_stageSize,mapinfo.drawInfo->value().GetOperateUnit(),true);
			//�����}�b�v�̍쐬
			lField->CalculateLatticeDistanceInfo(dField,mapinfo.drawInfo->value().GetAttackedUnit()->getPos());
			//�i�q�_�̒ǉ�
			moveDistance=mapinfo.drawInfo->value().GetOperateUnit()->GetMaxMoveDistance();
			for(const LatticeBattleField::LatticeDistanceInfo &info:dField){
				if(info.dist<moveDistance){
					const Vector2D pos=lField->CalculateLatticePointPos(info.index)*mapinfo.rate+mapinfo.startPos;
					assistList.push_back(std::make_pair(std::shared_ptr<const Shape>(new Circle(pos,2.0f,Shape::Fix::e_ignore)),pointColor));
				}
			}
		}
		//���[�N�쐬
		const std::shared_ptr<ReflectionWork::Base> explanationWork(new ReflectionWork::ReadExplanation(assistList,m_nowWork.reflection,"�_�ŕ\�����ꂽ�G�̓����͈͂����Ă݂�ƁA\n�傫���󂢂Ă��錄�Ԃ�D���Ďˎ�ɋ߂Â��Ă��鎖���킩��܂��B"));
		return WorkInfo(explanationWork,minimapLayout);
	};
	m_workMethodList.push_back(explanationWorkMethod);
}

void SubmissionReflectionScene::AddLineClickWork(std::vector<ShapeClickWorkInfo> &minimapInfo
	,const std::shared_ptr<MinimapLayoutBase> &minimapLayout
	,Unit::Team::Kind phase)
{
	//�U�����j�b�g�Ɣ�U�����j�b�g�����Ԑ���̏�Q�����N���b�N���郏�[�N
	//�}�`�쐬�֐��̍쐬
	const auto createFunc=[](Vector2D p0,Vector2D p1){
		return std::shared_ptr<Shape>(new Edge(p0,p1-p0,Shape::Fix::e_dynamic));
	};
	//���[�N�쐬
	AddShapeClickWork(createFunc,minimapInfo,minimapLayout,phase);
}

void SubmissionReflectionScene::AddAreaClickWork(std::vector<ShapeClickWorkInfo> &minimapInfo
	,const std::shared_ptr<MinimapLayoutBase> &minimapLayout
	,Unit::Team::Kind phase)
{
	//�U�����j�b�g�Ɣ�U�����j�b�g�����Ԑ�����Ίp���Ƃ����H�`�̈�̏�Q�����N���b�N���郏�[�N
	//�}�`�쐬�֐��̍쐬
	const auto createFunc=[this](Vector2D p0,Vector2D p1){
		//p0p1�ɐ����ȃx�N�g���ŁAp0p1�̒��_����h�����i�񂾏��ɂ���_��p2�Ƃ����|p0p2|+|p1p2|��pAttackedUnit�̈ړ������ɂȂ�悤�ȃx�N�g��
		Vector2D h=(p1-p0).turn((float)M_PI_2);
		const float unitMoveDistance=m_goodLogInfo->GetAttackedUnit()->GetMaxMoveDistance();
		h=h.norm()*std::powf(unitMoveDistance*unitMoveDistance-h.sqSize(),0.5f)*0.5f;
		return std::shared_ptr<Shape>(new MyPolygon(p0-h,{p0+h,p1+h,p1-h},Shape::Fix::e_dynamic));
	};
	//���[�N�쐬
	AddShapeClickWork(createFunc,minimapInfo,minimapLayout,phase);
}

void SubmissionReflectionScene::AddSelectOneWork(Unit::Team::Kind phase){
	const auto selectWorkMethod=[this,phase](){
		//���j�b�g�̐N���ۂ�G�t�F�[�Y�p�ɐݒ肵�Ă���
		this->SetUnitPenetratable(phase);
		//���[�N�̍쐬
		const float minimapWidth=(float)CommonConstParameter::mapSizeX*twoMinimapRate;
		const float minimapHeight=(float)CommonConstParameter::mapSizeY*twoMinimapRate;
		const std::shared_ptr<Shape> correct(new MyPolygon(MyPolygon::CreateRectangle(minimapPos[0],Vector2D(minimapWidth,minimapHeight),Shape::Fix::e_static)));
		const std::shared_ptr<Shape> incorrect(new MyPolygon(MyPolygon::CreateRectangle(minimapPos[1],Vector2D(minimapWidth,minimapHeight),Shape::Fix::e_static)));
		const std::shared_ptr<ReflectionWork::Base> work=std::shared_ptr<ReflectionWork::Base>(new ReflectionWork::SelectOne(correct,{incorrect},"�ǂ���̕����K�����s���ł��傤���H"));
		//�}�b�v�̕`��̎d����ݒ�
		const auto drawFunc=[this](){
			DrawTwoMinimap();
		};
		const std::shared_ptr<MinimapLayoutBase> minimap=std::shared_ptr<MinimapLayoutBase>(new NormalDraw(drawFunc));
		return WorkInfo(work,minimap);
	};
	m_workMethodList.push_back(selectWorkMethod);
}

void SubmissionReflectionScene::AddMoveSimulationWork(Unit::Team::Kind phase){
	//�U���L�����̈ʒu��ς��Ă݂ĕ]�����ǂ��Ȃ邩���V�~�����[�V�����w�K���Ă݂郏�[�N
	if(m_badLogInfo.has_value()){
		const auto simulationWorkMethod=[this,phase](){
			//���j�b�g�̐N���ۂ𖡕��t�F�[�Y�p�ɐݒ肵�Ă���
			this->SetUnitPenetratable(phase);
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
			//�N���A��ɍs���������L�q
			const auto clearProcess=[this](){
				//���[�N��ǉ�

			};
			//���[�N�̐ݒ�
			const std::shared_ptr<ReflectionWork::Base> work=std::shared_ptr<ReflectionWork::Base>(new
				ReflectionWork::MoveSimulation(field
					,m_badLogInfo->GetOperateUnit()
					,m_battleSceneData->m_stageSize
					,m_badLogInfo->GetAttackedUnit()
					,minimapPos[0]
					,oneMinimapRate
					,m_battleSceneData->m_scoreObserver->GetSubmission().GetRule()
					,"�V�~�����[�V�����w�K"));
			//�}�b�v�̕`��̎d����ݒ�
			const Easing::TYPE type=Easing::TYPE_OUT;
			const Easing::FUNCTION function=Easing::FUNCTION_QUAD;
			const double degree=4.0;
			const int maxFrame=10;
			const auto drawFunc=[this](int x,int y,float rate){
				if(m_badLogInfo.has_value()){
					DrawResizedMap(x,y,m_badLogInfo.value(),rate);
				}
			};
			const std::shared_ptr<MinimapLayoutBase> minimap=std::shared_ptr<MinimapLayoutBase>(new ExtendDraw(drawFunc
				,PositionControl(minimapPos[1],minimapPos[0],maxFrame,type,function,degree)
				,Easing((int)(CommonConstParameter::mapSizeX*twoMinimapRate),(int)(CommonConstParameter::mapSizeX*oneMinimapRate),maxFrame,type,function,degree)));
			return WorkInfo(work,minimap);
		};
		m_workMethodList.push_back(simulationWorkMethod);
	}
}
