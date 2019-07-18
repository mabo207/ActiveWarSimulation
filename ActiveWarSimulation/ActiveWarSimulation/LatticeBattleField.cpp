#include"LatticeBattleField.h"
#include"BattleSceneData.h"
#include"CommonConstParameter.h"

//---------------LatticeBattleField::LatticeDistanceInfo------------------
bool LatticeBattleField::LatticeDistanceInfo::operator<(const LatticeDistanceInfo &otherobj)const{
	//�]���̗D�揇�ʂ́u�����v���u�C���f�b�N�X�v�̏��B�߂��Ƃ��납�珈�����Ă��������̂�
	if(this->dist<otherobj.dist){
		return true;
	} else if(this->dist>otherobj.dist){
		return false;
	}
	return (this->index<otherobj.index);
}

bool LatticeBattleField::LatticeDistanceInfo::operator==(const LatticeDistanceInfo &otherobj)const{
	return (this->index==otherobj.index) && (this->dist==otherobj.dist);
}

//-----------------------LatticeBattleField-------------------------
const size_t LatticeBattleField::latticeIntervalSize=CommonConstParameter::unitCircleSize;

std::shared_ptr<LatticeBattleField> LatticeBattleField::Create(const BattleSceneData &battleData,const Unit * const punit,bool unitExist){
	return std::shared_ptr<LatticeBattleField>(new LatticeBattleField(battleData,punit,unitExist));
}

LatticeBattleField::~LatticeBattleField(){}

LatticeBattleField::LatticePass LatticeBattleField::GetLatticeInShapeAt(size_t index)const{
	if(index<m_xLatticeNum*m_yLatticeNum){
		return m_latticeInShape[index];
	} else{
		//�z��O�Q�Ƃ������́A���݂��Ȃ��_���ʂ낤�Ƃ��Ă���̂�unpassable��Ԃ�
		return LatticePass::e_unpassable;
	}
}

void LatticeBattleField::BecomeImpassibleLattice(size_t index){
	if(m_xLatticeNum>0){
		BecomeImpassibleLattice(index%m_xLatticeNum,index/m_xLatticeNum);
	}
}

void LatticeBattleField::BecomeImpassibleLattice(size_t x,size_t y){
	const size_t index=x+y*m_xLatticeNum;
	if(x<m_xLatticeNum && y<m_yLatticeNum){
		m_latticeInShape[index]=LatticePass::e_unpassable;
	}
	//�S�Ă�unpassable�ł���i�q�_�ɗאڂ���i�q�_��neighborUnpassable�ɂ���B
	const auto BecomeNeighborUnpassable=[](LatticePass &pass){
		if(pass!=LatticePass::e_unpassable){
			//����unpassable�ł���ꏊ��neighborUnpassable�ɂ��Ă͂����Ȃ�
			pass=LatticePass::e_neighborUnpassable;
		}
	};
	if(y>0){
		//��̗אړ_
		BecomeNeighborUnpassable(m_latticeInShape[index-m_xLatticeNum]);
	}
	if(x>0){
		//���̗אړ_
		BecomeNeighborUnpassable(m_latticeInShape[index-1]);
	}
	if(x+1<m_xLatticeNum){
		//��̗אړ_
		BecomeNeighborUnpassable(m_latticeInShape[index+1]);
	}
	if(y+1<m_yLatticeNum){
		//��̗אړ_
		BecomeNeighborUnpassable(m_latticeInShape[index+m_xLatticeNum]);
	}
}

void LatticeBattleField::BecomeImpassibleLattice(const Unit *punit,const Unit::Team::Kind operateTeam){
	const size_t vecSize=m_xLatticeNum*m_yLatticeNum;
	std::vector<int> info(vecSize,0);//�ʂ�邩�̏�����U�i�[����B�ʂ�Ȃ��Ƃ���݂̂�1�Ƃ���B
	//Team��p����punit�̓����蔻��}�`���č\�z
	const Shape *shape=punit->GetHitJudgeShape(Unit::Team::JudgeFriend(punit->GetBattleStatus().team,operateTeam));
	//�����蔻��}�`����unpassable�Ȋi�q�_��ǉ��A������unpasssable�i�q�_�ǉ��֐���p���Ċy������
	shape->RecordLatticePointInShape(info,m_xLatticeNum,m_yLatticeNum,latticeIntervalSize,latticeIntervalSize,1);
	for(size_t i=0,size=vecSize;i<size;i++){
		if(info[i]==1){
			//�ʂ�Ȃ��ƕ��������ꏊ��ʂ�Ȃ�������
			BecomeImpassibleLattice(i);
		}
	}
}

void LatticeBattleField::CalculateLatticeDistanceInfo(std::vector<LatticeDistanceInfo> &retPal,const Vector2D startPos)const{
	//���O����
	const size_t latticeNum=m_latticeInShape.size();//���x���g���̂ŗ\�ߌv�Z���Ă���
	retPal=std::vector<LatticeDistanceInfo>(latticeNum,LatticeDistanceInfo(latticeNum,latticeNum,-0.1f));//������
	std::set<LatticeDistanceInfo> latticeSet;//�T�����Ă����i�q�_�̃��X�g�B�߂�������T�����߂�set��p����B
	const auto SearchUpdate=[&latticeSet,&retPal](const size_t from,const float dist,const size_t x,const size_t y,const size_t width,const size_t height,const std::vector<LatticePass> &latticeInShape){
		//retPal���X�V���AlatticeSet�Ɋi�[����B
		if(x>=0 && x<width && y>=0 && y<height){
			size_t index=x+y*width;
			if(latticeInShape[index]!=LatticePass::e_unpassable && (retPal[index].dist<0.0f || dist<retPal[index].dist)){
				//�i���s�ł͂Ȃ��A�Ȃ��������������[�g�����Z�����[�g����������Ă��Ȃ����
				//�����}�b�v�̍X�V���s��
				LatticeDistanceInfo info(index,from,dist);
				retPal[index]=info;
				if(latticeInShape[index]==LatticePass::e_passable){
					//�T������i�q�_�̒ǉ��́Am_latticeInShape[index]���ʂ��ꍇ�̂ݍs��
					latticeSet.insert(info);
				}
			}
		}
	};
	//�����ʒu�ɋ߂�4�_��T��
	const size_t initX=(size_t)(startPos.x/latticeIntervalSize),initY=(size_t)(startPos.y/latticeIntervalSize);
	SearchUpdate(latticeNum,(startPos-Vector2D((float)(initX*latticeIntervalSize),(float)(initY*latticeIntervalSize))).size(),initX,initY,m_xLatticeNum,m_yLatticeNum,m_latticeInShape);
	SearchUpdate(latticeNum,(startPos-Vector2D((float)((initX+1)*latticeIntervalSize),(float)(initY*latticeIntervalSize))).size(),initX+1,initY,m_xLatticeNum,m_yLatticeNum,m_latticeInShape);
	SearchUpdate(latticeNum,(startPos-Vector2D((float)(initX*latticeIntervalSize),(float)((initY+1)*latticeIntervalSize))).size(),initX,initY+1,m_xLatticeNum,m_yLatticeNum,m_latticeInShape);
	SearchUpdate(latticeNum,(startPos-Vector2D((float)((initX+1)*latticeIntervalSize),(float)((initY+1)*latticeIntervalSize))).size(),initX+1,initY+1,m_xLatticeNum,m_yLatticeNum,m_latticeInShape);
	//�ʂ��\���̂��镔����S�ĒT��
	const int dx[8]={-1,0,1,-1,1,-1,0,1};
	const int dy[8]={-1,-1,-1,0,0,1,1,1};
	const float slantingdist=Vector2D((float)(latticeIntervalSize),(float)(latticeIntervalSize)).size();
	const float horizondist=(float)latticeIntervalSize;
	const float dis[8]={slantingdist,horizondist,slantingdist,horizondist,horizondist,slantingdist,horizondist,slantingdist};
	while(!latticeSet.empty()){
		std::set<LatticeDistanceInfo>::iterator beginIt=latticeSet.begin();
		LatticeDistanceInfo beginInfo(*beginIt);
		latticeSet.erase(beginIt);//�T��������s��Ȃ��̂�set�����菜���Ă���
		//����8�̊i�q�_�ɂ��čX�V���s��
		const size_t x=beginInfo.index%m_xLatticeNum,y=beginInfo.index/m_xLatticeNum;
		for(size_t i=0;i<8;i++){
			SearchUpdate(beginInfo.index,beginInfo.dist+dis[i],x+dx[i],y+dy[i],m_xLatticeNum,m_yLatticeNum,m_latticeInShape);
		}
	}
}

Vector2D LatticeBattleField::CalculateLatticePointPos(size_t x,size_t y)const{
	return Vector2D((float)(x*latticeIntervalSize),(float)(y*latticeIntervalSize));
}

Vector2D LatticeBattleField::CalculateLatticePointPos(size_t index)const{
	return CalculateLatticePointPos(index%m_xLatticeNum,index/m_xLatticeNum);
}

std::vector<float> LatticeBattleField::CalculateRouteDistance(const Vector2D start,const std::vector<Vector2D> &endVec)const{
	//�����}�b�v���쐬
	std::vector<LatticeDistanceInfo> distanceInfoVec;
	CalculateLatticeDistanceInfo(distanceInfoVec,start);
	//�e�i�q�_�ɑ΂��ċ��������߂�
	std::vector<float> retPal;
	const auto CalcMinDistance=[&distanceInfoVec,this](size_t x,size_t y,float minDistance){
		if(x>=0 && x<m_xLatticeNum && y>=0 && y<m_yLatticeNum){
			const float dist=distanceInfoVec[x+y*m_xLatticeNum].dist;
			if(minDistance<0.0f || dist<minDistance){
				return dist;
			}
		}
		return minDistance;
	};
	for(const Vector2D &end:endVec){
		//end�̎��ӂ̊i�q�_�܂ł̋���������,�ŏ������̂��̂�end�܂ł̃��[�g�����Ƃ���
		const size_t endLeftUpX=(size_t)(end.x/latticeIntervalSize),endLeftUpY=(size_t)(end.y/latticeIntervalSize);
		float minDistance=-0.1f;
		minDistance=CalcMinDistance(endLeftUpX,endLeftUpY,minDistance);
		minDistance=CalcMinDistance(endLeftUpX+1,endLeftUpY,minDistance);
		minDistance=CalcMinDistance(endLeftUpX,endLeftUpY+1,minDistance);
		minDistance=CalcMinDistance(endLeftUpX+1,endLeftUpY+1,minDistance);
		retPal.push_back(minDistance);
	}
	return retPal;
}

LatticeBattleField::LatticeBattleField(const BattleSceneData &battleData,const Unit * const punit,bool unitExist)
	:m_xLatticeNum((size_t)(battleData.m_stageSize.x)/latticeIntervalSize+1)
	,m_yLatticeNum((size_t)(battleData.m_stageSize.y)/latticeIntervalSize+1)
{
	CalculateLatticeInShape(battleData,punit,unitExist);
}

void LatticeBattleField::CalculateLatticeInShape(const BattleSceneData &battleData,const Unit * const punit,bool unitExist){
	const size_t vecSize=m_xLatticeNum*m_yLatticeNum;
	//�ŏ��ɑS�Ă̊i�q�_���ʂ��Ƃ��ď���������
	m_latticeInShape=std::vector<LatticePass>(vecSize,LatticePass::e_passable);
	//���삷�郆�j�b�g���ʂ�Ȃ��ꏊ��e_unpassable���i�[���Ă���
	//�u��Q���̒������ɂ��Ȃ��ƃ��j�b�g�͒ʂ�Ȃ��v�Ƃ����l������p����
	for(const BattleObject *object:battleData.m_field){
		if(object==punit){
			//���샆�j�b�g�͒ʂ�邩�ۂ��ɉe����^���Ȃ�

		} else if(object->GetFix()==Shape::Fix::e_ignore){
			//�����蔻��̂Ȃ��I�u�W�F�N�g�͒ʂ�邩�ۂ��ɉe����^���Ȃ�

		} else if(!unitExist && object->GetType()==BattleObject::Type::e_unit){
			//unitExist��false�̎��A���j�b�g�͒ʂ�邩�ۂ��ɉe����^���Ȃ�

		} else if(punit!=nullptr && object->JudgePointInsideShape(punit->getPos())){
			//object�����Ƀ��j�b�g������Ȃ�A���̊O����ʂ�Ȃ��悤�ɂ���
			std::vector<int> info(vecSize,1);//�ʂ�邩�̏�����U�i�[����B�ʂ��Ƃ���݂̂�0�Ƃ���B
			object->GetHitJudgeShape()->RecordLatticePointInShape(info,m_xLatticeNum,m_yLatticeNum,latticeIntervalSize,latticeIntervalSize,0);
			for(size_t i=0,size=vecSize;i<size;i++){
				if(info[i]==1){
					//�ʂ�Ȃ��ƕ��������ꏊ��ʂ�Ȃ�������
					m_latticeInShape[i]=LatticePass::e_unpassable;
				}
			}
		} else{
			//object�O���Ƀ��j�b�g������Ȃ�A���̓����͒ʂ�Ȃ��悤�ɂ���
			std::vector<int> info(vecSize,0);//�ʂ�邩�̏�����U�i�[����B�ʂ�Ȃ��Ƃ���݂̂�1�Ƃ���B
			object->GetHitJudgeShape()->RecordLatticePointInShape(info,m_xLatticeNum,m_yLatticeNum,latticeIntervalSize,latticeIntervalSize,1);
			for(size_t i=0,size=vecSize;i<size;i++){
				if(info[i]==1){
					//�ʂ�Ȃ��ƕ��������ꏊ��ʂ�Ȃ�������
					m_latticeInShape[i]=LatticePass::e_unpassable;
				}
			}
		}
	}
	//unpassable�ׂ̗�neighborUnpassable��u��
	for(size_t y=0;y<m_yLatticeNum;y++){
		for(size_t x=0;x<m_xLatticeNum;x++){
			const size_t index=x+y*m_xLatticeNum;
			if(m_latticeInShape[index]==LatticePass::e_unpassable){
				//�S�Ă�unpassable�ł���i�q�_�ɗאڂ���i�q�_��neighborUnpassable�ɂ���B
				BecomeImpassibleLattice(x,y);//m_latticeInShape[index]��unpassable�Ƃ��鏈�����܂ނ��A���Xunpassable�Ȃ̂Ŗ��Ȃ�
			}
		}
	}
}