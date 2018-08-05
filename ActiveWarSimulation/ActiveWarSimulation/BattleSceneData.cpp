#include"DxLib.h"
#include"BattleSceneData.h"
#include"input.h"
#include"Edge.h"
#include<algorithm>
#include"GraphicControl.h"
#include"ToolsLib.h"
#include"FileRead.h"

//----------------------BattleSceneData----------------------
BattleSceneData::BattleSceneData(const char *stagename)
	:m_Window(new Terrain(std::shared_ptr<Shape>(new Edge(Vector2D(0.0f,0.0f),Vector2D(1920.0f,1080.0f),Shape::Fix::e_ignore)),-1,0,true))
	,m_fpsMesuring(),m_operateUnit(nullptr),m_orderFont(CreateFontToHandle("Bell MT",32,2,DX_FONTTYPE_EDGE))
	,m_mapPic(LoadGraphEX(("Stage/"+std::string(stagename)+"/map.png").c_str())),m_drawObjectShapeFlag(false)
{
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
	m_stageSize=Vector2D(1920.0f,1080.0f);//�{���̓X�e�[�W�̑傫���̓O���t�B�b�N�f�[�^�̏c���̑傫���Ō��߂�

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
	//�t�H���g�J��
	DeleteFontToHandle(m_orderFont);
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
		for(const BattleObject *obj:m_field){
			if(m_Window->JudgeInShapeRect(obj)
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
		if(m_Window->JudgeInShapeRect(obj)
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
		if(m_Window->JudgeInShapeRect(unit) && unit->GetFix()!=Shape::Fix::e_ignore){
			//�E�C���h�E�ɓ����Ă��Ȃ����͕`�悵�Ȃ�
			//�ދp�������j�b�g(type��e_unit����fix��e_ignore)�͕`�悵�Ȃ�
			unit->DrawHPGage();
		}
	}
}

void BattleSceneData::DrawOrder()const{
	std::pair<int,int> windowSize=GetWindowResolution();
	auto calDrawPoint=[windowSize](size_t i){return Vector2D((i+1)*Unit::unitCircleSize*2.4f,(float)windowSize.second-Unit::unitCircleSize*1.1f);};//�`��ʒu�����̂܂܌v�Z����֐�
	
	//�I�[�_�[��ʂ̔w�i��`��
	DrawBox(0,windowSize.second-(int)(Unit::unitCircleSize*1.5f),windowSize.first,windowSize.second,GetColor(128,128,128),TRUE);//�w�i�̕`��
	
	//�s���I�����̃��j�b�g�̃I�[�_�[�ʒu�\���̖��̕`��
	const size_t arrowNum=2;
	Vector2D arrowPos[arrowNum]={calDrawPoint(0),calDrawPoint(0)};//���̐�[�ʒu(�擪:�s�����Ȃ��� ���:�s�����鎞)
	float op[arrowNum]={CalculateOperateUnitFinishOP(),CalculateOperateUnitFinishOP(m_operateUnit->ConsumeOPVirtualByCost(m_operateUnit->GetBattleStatus().weapon->GetCost()))};//���̈ʒu�ōs���I����������OP(�擪:�s�����Ȃ��� ���:�s�����鎞)
	const size_t listsize=m_unitList.size();
	for(size_t j=0;j<arrowNum;j++){
		bool flag=false;//�ʒu�ݒ肪�I�������
		for(size_t i=1;i+1<listsize;i++){
			if(!flag && op[j]<=m_unitList[i]->GetBattleStatus().OP && op[j]>m_unitList[i+1]->GetBattleStatus().OP){
				//���j�b�g������ł��낤��Ԃ����߂�(���ɋ��܂��Ă���ꍇ:flag[j]==true�͌v�Z�̕K�v�͂Ȃ�)
				//�����ʒu�ɒ��ӁI���݂̃��j�b�g�Ɠ���OP�̃��j�b�g���A���݂̃��j�b�g����ɓ�������悤�ɂ���悤�ɂ��Ă��邩��A�����j�b�g���OP���������ꍇ�͂��̋�Ԃ͊Ԉ���Ă���悤�ɂ��Ȃ��Ƃ����Ȃ��B
				//�܂��A����OP�̃��j�b�g�����݂���ꍇ�́A�����������ɓ���Ȃ��Ɠ���ׂ���Ԃ��������\��������̂œ����͓����B
				const float pal=(m_unitList[i]->GetBattleStatus().OP-op[j])/(m_unitList[i]->GetBattleStatus().OP-m_unitList[i+1]->GetBattleStatus().OP);//���������0���Z�ɂȂ�Ȃ����Ƃ��ۏ؂���Ă���
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
		}
	}

	//���j�b�g�̃I�[�_�[�������Ԃɕ`��
	for(size_t i=0,size=m_unitList.size();i<size;i++){
		const Vector2D centerPoint=calDrawPoint(i);
		//�}�E�X���d�Ȃ��Ă���΁A�Ή��L�����܂Ő���L�΂�
		if((GetMousePointVector2D()-centerPoint).sqSize()<Unit::unitCircleSize*Unit::unitCircleSize){
			const Vector2D unitDrawPos=m_unitList[i]->getPos()-Vector2D();
			DrawLineAA(centerPoint.x,centerPoint.y,unitDrawPos.x,unitDrawPos.y,GetColor(196,196,196),3.0f);
			DrawLineAA(centerPoint.x,centerPoint.y,unitDrawPos.x,unitDrawPos.y,GetColor(255,255,255));
		}
		//���j�b�g�A�C�R��(�`���_�͐^��)
		m_unitList[i]->DrawFacePic(centerPoint);
		//�c��OP
		const int x=(int)centerPoint.x,y=((int)centerPoint.y)-(int)(Unit::unitCircleSize)-20;
		DrawStringCenterBaseToHandle(x,y,std::to_string((int)m_unitList[i]->GetBattleStatus().OP).c_str(),GetColor(255,255,255),m_orderFont,true,GetColor(0,0,0));
	}
}
