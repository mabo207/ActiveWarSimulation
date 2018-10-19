#include"DxLib.h"
#include"BattleSceneData.h"
#include"input.h"
#include"Edge.h"
#include<algorithm>
#include"GraphicControl.h"
#include"ToolsLib.h"
#include"FileRead.h"
#include"GameScene.h"
#include"CommonConstParameter.h"
#include<math.h>

//----------------------BattleSceneData----------------------
const Vector2D BattleSceneData::mapDrawSize=Vector2D((float)CommonConstParameter::gameResolutionX,900.0f);
const Vector2D BattleSceneData::uiDrawSize=Vector2D(mapDrawSize.x,(float)CommonConstParameter::gameResolutionX-BattleSceneData::mapDrawSize.y);

BattleSceneData::BattleSceneData(const std::string &stagename)
	:m_mapRange(new Terrain(std::shared_ptr<Shape>(new Edge(Vector2D(0.0f,0.0f),mapDrawSize,Shape::Fix::e_ignore)),-1,0,true))
	,m_fpsMesuring(),m_operateUnit(nullptr)
	,m_totalOP(0.0f)
	,m_stageName(stagename)
//	,m_orderFont(CreateFontToHandle("04���񂶂キ�S�V�b�N",24,4,DX_FONTTYPE_EDGE,-1,2))
	,m_orderFont(LoadFontDataToHandleEX("Font/OrderPalFont.dft",2))
	,m_mapPic(LoadGraphEX(("Stage/"+std::string(stagename)+"/nonfree/map.png").c_str())),m_drawObjectShapeFlag(false)
	,m_mapBGM(LoadBGMMem("Sound/bgm/nonfree/stage1/"))
	,m_aimchangeSound(LoadSoundMem("Sound/effect/nonfree/aimchange.ogg"))
	,m_attackSound(LoadSoundMem("Sound/effect/nonfree/damage.ogg"))
	,m_healSound(LoadSoundMem("Sound/effect/nonfree/recover.ogg"))
	,m_footSound(LoadSoundMem("Sound/effect/nonfree/foot.ogg"))
{
	//�O���t�B�b�N�f�[�^�̓ǂݍ���
	LoadDivGraphEX("Graphic/drawOrderHelp.png",drawOrderHelpNum,1,drawOrderHelpNum,90,15,m_drawOrderHelp);

	//�t�@�C������X�e�[�W��ǂݍ���
	const std::string stagedir("Stage/"+std::string(stagename)+"/");
	//�t�@�C�����J�����ׂĂ̕�����������o��
	std::ifstream ifs((stagedir+"stage.txt").c_str());
	if(!ifs){

	} else{
		std::string str;//�����o����
		while(true){
			char ch;
			ch=ifs.get();
			//�t�@�C���̏I�[��while����E�o
			if(ch==EOF){
				break;
			}
			str.push_back(ch);
		}
		//�I�u�W�F�N�g�Q��{}�ň͂܂�\n�ŋ�؂��Ă���̂ŁA�P�K�w���������ǂݍ��݂��āA�I�u�W�F�N�g�𐶐�����
		StringBuilder sb(str,'\n','{','}',false,true);
		for(const StringBuilder &ssb:sb.m_vec){
			BattleObject *pb=BattleObject::CreateRawObject(ssb.GetString());
			if(pb!=nullptr){
				m_field.push_back(pb);
			}
		}
	}
	//�t�@�C������X�e�[�W�̃O���t�B�b�N�f�[�^�̓ǂݍ���
	m_stageSize=mapDrawSize;//�{���̓X�e�[�W�̑傫���̓O���t�B�b�N�f�[�^�̏c���̑傫���Ō��߂�

	//�t�@�C�����烆�j�b�g��ǂݍ���
	StringBuilder unitlist(FileStrRead((stagedir+"unitlist.txt").c_str()),'\n','{','}',false,true);
	for(const StringBuilder &unitdata:unitlist.m_vec){
		const StringBuilder sb(unitdata.GetString(),',','(',')',true,true);
		//�܂����u�p�̐ݒ�����邩�Œ胆�j�b�g�p�̐ݒ�����邩�𔻒肷��
		bool uniqueFlag=false;
		for(const StringBuilder &ssb:sb.m_vec){
			if(ssb.m_vec.size()>=2 && ssb.m_vec[0].GetString()=="definition"){
				//�ݒ���@��definition�ɋL�ڂ���Ă���
				uniqueFlag=(ssb.m_vec[1].GetString()=="unique");
				break;
			}
		}
		//���u���Œ肩�ŕ���
		if(uniqueFlag){
			//�Œ胆�j�b�g(������)

		} else{
			//���u
			//�e�l��錾�B�ݒ肵�����ǂ�����pair��second�Ɋi�[
			std::pair<std::string,bool> name;
			name.second=false;
			std::pair<Vector2D,bool> pos;
			pos.second=false;
			std::pair<int,bool> lv;
			lv.second=false;
			std::pair<Unit::Profession::Kind,bool> prof;
			prof.second=false;
			std::pair<Unit::Team::Kind,bool> team;
			team.second=false;
			//�e�l�̓ǂݎ��
			for(const StringBuilder &ssb:sb.m_vec){
				if(!ssb.m_vec.empty()){
					//�擪�����񂪂��邱�Ƃ�ۏ�
					if(ssb.m_vec[0].GetString()=="name" && ssb.m_vec.size()>=2){
						name.first=ssb.m_vec[1].GetString();
						name.second=true;
					} else if(ssb.m_vec[0].GetString()=="profession" && ssb.m_vec.size()>=2){
						prof.first=Unit::Profession::link(std::atoi(ssb.m_vec[1].GetString().c_str()));
						prof.second=true;
					} else if(ssb.m_vec[0].GetString()=="lv" && ssb.m_vec.size()>=2){
						lv.first=std::atoi(ssb.m_vec[1].GetString().c_str());
						lv.second=true;
					} else if(ssb.m_vec[0].GetString()=="pos" && ssb.m_vec.size()>=3){
						pos.first=Vector2D((float)std::atoi(ssb.m_vec[1].GetString().c_str()),(float)std::atoi(ssb.m_vec[2].GetString().c_str()));
						pos.second=true;
					} else if(ssb.m_vec[0].GetString()=="team" && ssb.m_vec.size()>=2){
						team.first=Unit::Team::link(std::atoi(ssb.m_vec[1].GetString().c_str()));
						team.second=true;
					}
				}
			}
			//�e�l���烆�j�b�g���i�[
			if(name.second && prof.second && lv.second && pos.second && team.second){
				m_field.push_back(Unit::CreateMobUnit(name.first,prof.first,lv.first,pos.first,team.first));
			}
		}
	}
	//m_unitList��m_operateUnit�̏�����
	for(BattleObject *obj:m_field){
		if(obj->GetType()==BattleObject::Type::e_unit){
			m_unitList.push_back(dynamic_cast<Unit *>(obj));
		}
	}
	//m_unitList�\�[�g��������
	SortUnitList();
	//�^�C�}�[�Z�b�g
	m_fpsMesuring.RecordTime();

}

BattleSceneData::~BattleSceneData(){
	//�O���t�B�b�N�J��
	DeleteGraphEX(m_mapPic);
	for(size_t i=0;i<drawOrderHelpNum;i++){
		DeleteGraphEX(m_drawOrderHelp[i]);
	}
	//�T�E���h�J��
	StopSoundMem(m_mapBGM);
	DeleteSoundMem(m_mapBGM);
	DeleteSoundMem(m_aimchangeSound);
	DeleteSoundMem(m_attackSound);
	DeleteSoundMem(m_healSound);
	DeleteSoundMem(m_footSound);
	//�t�H���g�J��
	DeleteFontToHandleEX(m_orderFont);
	//�I�u�W�F�N�g�ꗗ���J��
	for(BattleObject *obj:m_field){
		delete obj;
	}
}

float BattleSceneData::CalculateOperateUnitFinishOP()const{
	return CalculateOperateUnitFinishOP(m_operateUnit->GetBattleStatus().OP);
}

float BattleSceneData::CalculateOperateUnitFinishOP(float op)const{
	//2�Ԗڂ̐擪���j�b�g�Ɣ�r����
	if(m_unitList.size()<2 || m_unitList[1]->GetBattleStatus().OP>op){
		//1�̂������j�b�g�����Ȃ���Ε␳�͍s��Ȃ�
		//���̑��샆�j�b�g��OP���op���������Ȃ�␳�͍s��Ȃ�
		return op;
	} else{
		//�����łȂ��Ȃ�␳���s��
		return m_unitList[1]->GetBattleStatus().OP-1.0f;
	}
}

void BattleSceneData::UpdateFix(){
	//m_field�̊e�I�u�W�F�N�g��m_fix���X�V����
	//e_ignore�͂��̂܂܁A����ȊO�͑S��e_static�ɂ��Am_operateUnit������e_dynamic�ɂ���
	for(BattleObject *obj:m_field){
		if(obj->GetFix()!=Shape::Fix::e_ignore){
			obj->SetFix(Shape::Fix::e_static);
			if(obj->GetType()==BattleObject::Type::e_unit){
				//unit�ɑ΂��Ă�Penetratable���X�V����
				dynamic_cast<Unit *>(obj)->SetPenetratable(m_operateUnit->GetBattleStatus().team);
			}
		}
	}
	m_operateUnit->SetFix(Shape::Fix::e_dynamic);
}

bool BattleSceneData::PositionUpdate(const Vector2D inputVec){
	//�e�l�̒�`
	const float speed=(float)(m_operateUnit->GetBaseStatus().move);//�I�u�W�F�N�g�̈ړ����x
	const size_t moveCount=5;//�ړ��񐔂̕�����
	const size_t judgeCount=3;//1�ړ����̓����蔻����s��
	const Vector2D beforePos=m_operateUnit->getPos();//�ړ��O�̈ʒu���擾
	Vector2D moveVec;
	//�ړ��x�N�g���̌v�Z
	bool inputFlag=false;//�ړ��̓��͂����������ǂ���
	if(m_operateUnit->GetBattleStatus().OP>0.0f){
		//OP������Ȃ��Ɠ����Ȃ�
		if(inputVec.sqSize()==0.0f){
			inputFlag=false;
		} else{
			inputFlag=true;
			moveVec=inputVec.norm()*std::fminf((float)(speed/moveCount),inputVec.size());
		}
	}
	//�ʒu�X�V���
	for(size_t i=0;i<moveCount;i++){
		//1�t���[������5��ړ�
		//�ړ��������������ɂ��鎖�ŁA�ړ����x�����コ���邱�Ƃ��ł���
		//1��̈ړ����������������Ȃ��ƃM���M���ʂ�Ȃ��ꏊ���ʂ��悤�ɂȂ��Ă��܂�
		if(inputFlag){
			m_operateUnit->Move(moveVec);
		}
		//1�t���[�����ɕ����񓖂��蔻�菈�����s���ƁA�������d���Ȃ����ɃI�u�W�F�N�g�̈ړ����x���グ�邱�Ƃ��ł���
		for(BattleObject *pObject:m_field){
			//�����蔻��n�̏���
			pObject->UpdatePosition(pointer_array_cast<ShapeHaving>(m_field.data()),m_field.size(),judgeCount);
		}
	}
	//�ړ������̌v����OP����
	const float moveCost=(m_operateUnit->getPos()-beforePos).size()/speed;
	//m_operateUnit->AddOP(-moveCost);//�����Ȃ̂�cost���}�C�i�X�������̂����Z����
	m_operateUnit->ConsumeOPByCost(moveCost);

	return inputFlag;
}

void BattleSceneData::SortUnitList(){
	std::vector<Unit *> list=m_unitList;//���̔z����R�s�[
	//���݂̃��j�b�g����Ԍ��܂Ŏ����Ă���
	//���݂̃��j�b�g�Ɠ���OP�̃��j�b�g���A���݂̃��j�b�g����ɓ�������悤�ɂ��邽��
	for(std::vector<Unit *>::const_iterator it=list.begin(),ite=list.end();it!=ite;it++){
		//�������̎��Ȃ�m_operateUnit��nullptr�̎��͌��Ɏ����Ă����̂͋N����Ȃ�
		if((*it)==m_operateUnit){
			list.erase(it);
			list.push_back(m_operateUnit);
			break;
		}
	}
	//m_unitList��clear���āAlist���珇�Ԃɂǂ�ǂ�i�[���Ă���
	m_unitList.clear();
	//m_unitList�Ƀ\�[�g���ʂ��i�[�BO(n^2)�̎����Ȃ̂Œ�����̂Ȃ璼���������A����OP�̃I�u�W�F�N�g�͑O��œ������Ԃɂ������B
	while(!list.empty()){
		std::vector<Unit *>::const_iterator maxit=list.begin();
		//it��OP�ő�̃I�u�W�F�N�g�̓��A�擪�ɋ߂����̂��i�[
		for(std::vector<Unit *>::const_iterator it=list.begin(),ite=list.end();it!=ite;it++){
			if((*maxit)->GetBattleStatus().OP<(*it)->GetBattleStatus().OP){
				//it�̕����傫���ꍇ
				maxit=it;
			}
		}
		//m_unitList�Ɋi�[���Alist����폜
		m_unitList.push_back(*maxit);
		list.erase(maxit);
	}
}

void BattleSceneData::FinishUnitOperation(){
	//���̑��샆�j�b�g��m_operateUnit�̂܂܂ɂȂ�Ȃ��悤��OP��␳����
	//�������̍�(m_operateUnit��nullptr�ł���)�ɂ��p������̂ŁAnullptr�̎��͕␳�͕K�v�Ȃ��̂ł��Ȃ��B
	if(m_operateUnit!=nullptr){
		m_operateUnit->SetOP(CalculateOperateUnitFinishOP());
	}
	//m_unitList�\�[�g��������
	SortUnitList();
	//�擪��m_operateUnit�Ɋi�[
	m_operateUnit=m_unitList.front();
	//m_operateUnit��OP���ő�ɂȂ�悤��m_unitList�S����OP�l��ω�
	const float plusOP=Unit::BattleStatus::maxOP-m_operateUnit->GetBattleStatus().OP;
	for(Unit *u:m_unitList){
		//u->AddOP(plusOP);
		u->SetOP(u->GetBattleStatus().OP+plusOP);
	}
	m_totalOP+=plusOP;//�����OP��plusOP�����Z�B
	//m_operateUnit��OP�����炷(�R�X�g�Ƃ��ď����̂ŏ���OP�������̉e�����󂯂�d�l�ƂȂ�)
	//m_operateUnit->AddOP(-Unit::reduceStartActionCost);
	m_operateUnit->ConsumeOPByCost(Unit::reduceStartActionCost);
	//�����蔻��}�`�̕ω�
	UpdateFix();
	//�^�C�}�[�Z�b�g
	m_fpsMesuring.RecordTime();

}

Unit *BattleSceneData::GetUnitPointer(Vector2D pos)const{
	for(Unit *pu:m_unitList){
		if(pu->JudgePointInsideShape(pos)){
			return pu;
		}
	}
	return nullptr;
}

void BattleSceneData::DrawField(const std::set<const BattleObject *> &notDraw)const{
	//�w�i�f�[�^�����̂܂ܕ`��
	//�w�i���A�j���[�V��������Ƃ��Ȃ����炱���ŏ������L�q
	DrawGraph(0,0,m_mapPic,TRUE);
	//�t�B�[���h�I�u�W�F�N�g�̓����蔻��}�`�̕`��i�f�o�b�O�@�\�Ƃ��ăf�t�H���g��OFF�A�R�}���h���͂�ON�ɂȂ�j
	if(m_drawObjectShapeFlag){
		//���l����printfDx()
		{
			Vector2D v=GetMousePointVector2D();
			int wx,wy;
			GetWindowSize(&wx,&wy);
			printfDx("mouse:(%f,%f)\nwindowsize:(%d,%d)\nresolution:(%d,%d)\n",v.x,v.y,wx,wy,CommonConstParameter::gameResolutionX,CommonConstParameter::gameResolutionY);
		}
		//�����蔻��}�`�̕`��
		for(const BattleObject *obj:m_field){
			if(m_mapRange->JudgeInShapeRect(obj)
				&& obj->GetType()!=BattleObject::Type::e_unit
				&& notDraw.find(obj)==notDraw.end())
			{
				//�E�C���h�E�ɓ����Ă��Ȃ����͕`�悵�Ȃ�
				//���j�b�g(type��e_unit)�͕`�悵�Ȃ�
				//�`�悵�Ȃ�����(notDraw�Ɋi�[����Ă������)�͕`�悵�Ȃ�
				obj->VDraw();
			}
		}
	}
}

void BattleSceneData::DrawUnit(bool infoDrawFlag,const std::set<const Unit *> &notDraw)const{
	for(const Unit *obj:m_unitList){
		if(m_mapRange->JudgeInShapeRect(obj)
			&& obj->GetFix()!=Shape::Fix::e_ignore
			&& notDraw.find(obj)==notDraw.end())
		{
			//�E�C���h�E�ɓ����Ă��Ȃ����͕`�悵�Ȃ�
			//�ދp�������j�b�g(m_fix��e_ignore)�͕`�悵�Ȃ�
			//�`�悵�Ȃ�����(notDraw�Ɋi�[����Ă������)�͕`�悵�Ȃ�
			obj->DrawUnit(obj->getPos(),Vector2D(),infoDrawFlag);
		}
	}
}

void BattleSceneData::DrawHPGage()const{
	for(const Unit *unit:m_unitList){
		if(m_mapRange->JudgeInShapeRect(unit) && unit->GetFix()!=Shape::Fix::e_ignore){
			//�E�C���h�E�ɓ����Ă��Ȃ����͕`�悵�Ȃ�
			//�ދp�������j�b�g(type��e_unit����fix��e_ignore)�͕`�悵�Ȃ�
			unit->DrawHPGage();
		}
	}
}

void BattleSceneData::DrawOrder(const std::set<const BattleObject *> &lineDraw)const{
	const float opStrDy=20.0f;//OP�̐����̕`��ɃE�C���h�E���[���牽px�g�p���邩
	auto calDrawPoint=[opStrDy](size_t i){
		//�`��ʒu�����̂܂܌v�Z����֐�
		float topOpenWidth;//�擪�L�����i�����샆�j�b�g�j���������ĕ`�悳���悤�ɂ��邽�߂̒l
		if(i==0){
			topOpenWidth=0.0f;
		} else{
			topOpenWidth=Unit::unitCircleSize;
		}
		return Vector2D((i+1)*Unit::unitCircleSize*2.4f+topOpenWidth,(float)CommonConstParameter::gameResolutionY-Unit::unitCircleSize*1.1f-opStrDy);
	};
	
	//�I�[�_�[��ʂ̔w�i��`��
	//DrawBox(0,windowSize.second-(int)(Unit::unitCircleSize*1.5f),windowSize.first,windowSize.second,GetColor(128,128,128),TRUE);//�w�i�̕`��
	

	//���j�b�g�̃I�[�_�[�������Ԃɕ`��i�^�C�}�[���`��j
	size_t unitListIndex=0;//���j�b�g���X�g�̎Q�Ɣԍ�
	bool turnTimerDrawFlag=false;//����turnTimer��`�悵����
	std::vector<float> opVec(m_unitList.size()+1);//OP�̒l�ꗗ
	for(size_t i=0,size=m_unitList.size();i<size+1;i++){
		const Vector2D centerPoint=calDrawPoint(i);//�I�[�_�[��ʂɏ�������Ă��郆�j�b�g�A�C�R���̒��S�ʒu
		const float turnTimerOP=fmodf(m_totalOP,Unit::BattleStatus::maxOP);//�^�[���v���^�C�}�[�̕\��OP
		float opPal;//�A�C�R���̉��ɕ`�悷��OP�̒l
		//���j�b�g��`�悷�邩�A�^�C�}�[��`�悷�邩�ł��̐�͕ω�����
		if(turnTimerDrawFlag
			|| unitListIndex==0
			|| (unitListIndex<size && m_unitList[unitListIndex]->GetBattleStatus().OP>=turnTimerOP)
			)
		{
			//���Ƀ^�C�}�[��`�悵�Ă���ꍇ�A�擪���j�b�g��`�悵�Ă��Ȃ��ꍇ�͕K�����j�b�g�`��B�܂��AturnTimerOP���傫��OP�̃��j�b�g�̓^�C�}�[����ɕ`��
			//�}�E�X���d�Ȃ��Ă���΁A�Ή��L�����܂Ő���L�΂�
			//����L�΂��L�����̃��X�g�ł���lineDraw��m_unitList[unitListIndex]�������Ă��Ă�����L�΂�
			if((GetMousePointVector2D()-centerPoint).sqSize()<Unit::unitCircleSize*Unit::unitCircleSize || lineDraw.find(m_unitList[unitListIndex])!=lineDraw.end()){
				const Vector2D unitDrawPos=m_unitList[unitListIndex]->getPos()-Vector2D();
				//���̕`��
				DrawLineAA(centerPoint.x,centerPoint.y,unitDrawPos.x,unitDrawPos.y,GetColor(196,196,196),3.0f);
				//���̒���ʂ�����̃A�j���[�V����
				const int animeDuration=30;//�A�j���[�V�����̒���
				const int startDuration=45;//�A�j���[�V�������N����Ԋu
				const int t=m_fpsMesuring.GetFlame()%startDuration;
				const int lineLength=5;//�����̒����͂��̃t���[����
				if(t<animeDuration+lineLength){
					//�����̏I�[���o�����Ă���n�_����������܂Ő����̕`����s���̂ŁAAnimeDuration+lineLength�̒������������͕`�悷��
					if(t<lineLength){
						//�n�_���o�����Ă��Ȃ��ꍇ
						const Vector2D v=(centerPoint*(float)(animeDuration-t)+unitDrawPos*(float)t)/(float)animeDuration;//�I�_�ʒu
						DrawLineAA(centerPoint.x,centerPoint.y,v.x,v.y,GetColor(255,255,255));//�n�_�̈ʒu�̓I�[�_�[�`��̈ʒu�Ƃ���
					} else if(t<animeDuration){
						//�n�_�I�_�Ƃ��ɏo�����Ă���ꍇ
						const Vector2D v1=(centerPoint*(float)(animeDuration+lineLength-t)+unitDrawPos*(float)(t-lineLength))/(float)animeDuration;//�n�_�ʒu
						const Vector2D v2=(centerPoint*(float)(animeDuration-t)+unitDrawPos*(float)t)/(float)animeDuration;//�I�_�ʒu
						DrawLineAA(v1.x,v1.y,v2.x,v2.y,GetColor(255,255,255));//�n�_�ƏI�_������
					} else{
						//�I�_���������Ă���ꍇ
						const Vector2D v=(centerPoint*(float)(animeDuration+lineLength-t)+unitDrawPos*(float)(t-lineLength))/(float)animeDuration;//�n�_�ʒu
						DrawLineAA(v.x,v.y,unitDrawPos.x,unitDrawPos.y,GetColor(255,255,255));//�I�_�̈ʒu�̓��j�b�g�̃}�b�v�`��ʒu�Ƃ���
					}
				}
			}
			//���j�b�g�A�C�R��(�`���_�͐^��)
			m_unitList[unitListIndex]->DrawFacePic(centerPoint);
			//opPal�̏�����
			opPal=m_unitList[unitListIndex]->GetBattleStatus().OP;
			//unitListIndex�̃C���N�������g
			unitListIndex++;
		} else{
			//�^�C�}�[�A�C�R���̕`��
			DrawCircleAA(centerPoint.x,centerPoint.y,30.0f,9,GetColor(128,128,0),TRUE);
			//opPal�̏�����
			opPal=turnTimerOP;
			//�����^�C�}�[��`�悵�Ȃ��悤�ɂ���
			turnTimerDrawFlag=true;
		}
		//�c��OP
		const int x=(int)centerPoint.x,y=(int)(CommonConstParameter::gameResolutionY-opStrDy);
		DrawStringCenterBaseToHandle(x,y,std::to_string((int)opPal).c_str(),GetColor(255,255,255),m_orderFont,true,GetColor(0,0,0));
		opVec[i]=opPal;
	}

	//�s���I�����̃��j�b�g�̃I�[�_�[�ʒu�\���̖��̕`��ʒu�v�Z
	const size_t arrowNum=drawOrderHelpNum;
	Vector2D arrowPos[arrowNum]={calDrawPoint(0),calDrawPoint(0)};//���̐�[�ʒu(�擪:�s�����Ȃ��� ���:�s�����鎞)
	float op[arrowNum]={CalculateOperateUnitFinishOP(),CalculateOperateUnitFinishOP(m_operateUnit->ConsumeOPVirtualByCost(m_operateUnit->GetBattleStatus().weapon->GetCost()))};//���̈ʒu�ōs���I����������OP(�擪:�s�����Ȃ��� ���:�s�����鎞)
	const size_t listsize=opVec.size();
	for(size_t j=0;j<arrowNum;j++){
		bool flag=false;//�ʒu�ݒ肪�I�������
		for(size_t i=1;i+1<listsize;i++){
			if(!flag && op[j]<=opVec[i] && op[j]>opVec[i+1]){
				//���j�b�g������ł��낤��Ԃ����߂�(���ɋ��܂��Ă���ꍇ:flag[j]==true�͌v�Z�̕K�v�͂Ȃ�)
				//�����ʒu�ɒ��ӁI���݂̃��j�b�g�Ɠ���OP�̃��j�b�g���A���݂̃��j�b�g����ɓ�������悤�ɂ���悤�ɂ��Ă��邩��A�����j�b�g���OP���������ꍇ�͂��̋�Ԃ͊Ԉ���Ă���悤�ɂ��Ȃ��Ƃ����Ȃ��B
				//�܂��A����OP�̃��j�b�g�����݂���ꍇ�́A�����������ɓ���Ȃ��Ɠ���ׂ���Ԃ��������\��������̂œ����͓����B
				const float pal=(opVec[i]-op[j])/(opVec[i]-opVec[i+1]);//���������0���Z�ɂȂ�Ȃ����Ƃ��ۏ؂���Ă���
				arrowPos[j]=calDrawPoint(i)*(1.0f-pal)+calDrawPoint(i+1)*pal;//�����_�̎Z�o��:p=ap0+bp1(a+b=1,a>=0,b>=0)
				//�v�Z���I���������flag��true�ɂ��Čv�Z�I��
				flag=true;
				break;
			}
		}
		//��Ԃ�������Ȃ��������Ԍ��Ƃ�������
		if(!flag){
			arrowPos[j]=calDrawPoint(listsize);
			arrowPos[j].x+=(float)(j*8);//�Y�������Ă����邱�Ƃ�2��󂪊��S�ɔ�鎖��h��
		}
	}
	//����`��
/*
	for(size_t j=0;j<2;j++){
		const float width[2]={8.0f,6.0f};
		const unsigned int color[2]={GetColor(0,0,0),GetColor(255,255,255)};//���n�͍��A��͔�
		for(size_t i=0;i<arrowNum;i++){
			const float height=Unit::unitCircleSize*0.6f;
			const Vector2D v=calDrawPoint(0)-Vector2D(0.0f,width[0]*2.0f+Unit::unitCircleSize*0.9f);//�擪���j�b�g�̕`���ʒu(���n�E�����������Ƃ��ɓ����ʒu�ɂ������̂�width[0]��p����)
			DrawBoxAA(v.x-width[j],v.y-height-width[j],v.x+width[j],v.y,color[j],TRUE);
			DrawBoxAA(v.x-width[j],v.y-height-width[j],arrowPos[i].x+width[j],v.y-height+width[j],color[j],TRUE);
			DrawBoxAA(arrowPos[i].x-width[j],v.y-height-width[j],arrowPos[i].x+width[j],v.y,color[j],TRUE);
			DrawTriangleAA(arrowPos[i].x,v.y+width[j]*2.0f,arrowPos[i].x-width[j]*2.0f,v.y,arrowPos[i].x+width[j]*2.0f,v.y,color[j],TRUE);
			if(j==0){
				//���̕���̏�Ƀw���v�`��
				int dx,dy;
				GetGraphSize(m_drawOrderHelp[i],&dx,&dy);
				DrawGraph((int)arrowPos[i].x-dx/2,(int)(v.y-height-width[j])-dy*(drawOrderHelpNum-i),m_drawOrderHelp[i],TRUE);
			}
		}
	}
//*/
	const unsigned int color[2][2]={{GetColor(100,162,234),GetColor(231,121,24)},{GetColor(255,255,255),GetColor(255,255,255)}};//���n�͍��A��͔�
	for(size_t j=0;j<2;j++){
		//���Ǝ�O���ɖ���`��(j=0�����̐F�t���Aj=1����O�̔�)
		const float width[2]={8.0f,4.0f};
		for(size_t i=0;i<arrowNum;i++){
			const float height=15.0f;
			const Vector2D v=calDrawPoint(0)-Vector2D(0.0f,width[0]*2.0f+Unit::unitCircleSize*0.9f);//�擪���j�b�g�̕`���ʒu(���n�E�����������Ƃ��ɓ����ʒu�ɂ������̂�width[0]��p����)
			DrawBoxAA(arrowPos[i].x-width[j],v.y-height-width[j],arrowPos[i].x+width[j],v.y,color[j][i],TRUE);
			DrawTriangleAA(arrowPos[i].x,v.y+width[j]*2.0f,arrowPos[i].x-width[j]*2.0f,v.y,arrowPos[i].x+width[j]*2.0f,v.y,color[j][i],TRUE);
			if(j==0){
				//���̕���̏�Ƀw���v�`��
				int dx,dy;
				GetGraphSize(m_drawOrderHelp[i],&dx,&dy);
				DrawGraph((int)arrowPos[i].x-dx/2,(int)(v.y-height-width[j])-dy*(drawOrderHelpNum-i),m_drawOrderHelp[i],TRUE);
			}
		}
	}
}

bool BattleSceneData::JudgeMousePushInsideMapDrawZone(int mouseCode,bool continuousFlag){
	const Vector2D mouse=GetMousePointVector2D();
	if(mouse.x>=0.0f && mouse.x<mapDrawSize.x && mouse.y>=0.0f && mouse.y<mapDrawSize.y){
		//�}�b�v�`�敔���Ƀ}�E�X�����邩
		const int f=mouse_get(mouseCode);
		if(continuousFlag){
			//true�Ȃ牟���Ă��邩�ǂ�����
			return f>0;
		} else{
			//false�Ȃ牟�����u�Ԃ��ǂ�����
			return f==1;
		}
	}
	return false;
}
