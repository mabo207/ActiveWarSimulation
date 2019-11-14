#ifndef DEF_SUBMISSIONREFLECTIONSCENE_H
#define DEF_SUBMISSIONREFLECTIONSCENE_H

#include"BattleSceneElement.h"
#include"BattleSceneData.h"
#include<optional>
#include<functional>
#include"ReflectionWorkBase.h"

//�T�u�~�b�V�����ݒ莞�A���U���g��ʂ̑O�ɐU��Ԃ���s�����
class SubmissionReflectionScene:public BattleSceneElement{
public:
	SubmissionReflectionScene(const std::shared_ptr<BattleSceneData> &battleSceneData,const std::shared_ptr<BattleSceneElement> &clearScene);
	~SubmissionReflectionScene();

private:
	//�^
	struct MinimapDrawInfo{
		//�~�j�}�b�v�`��ɕK�v�ȏ��Q
		MinimapDrawInfo(const std::shared_ptr<const LogElement> &log,Unit::Team::Kind phase);
		~MinimapDrawInfo();
		const std::vector<Unit>& GetUnitList()const{ return unitList; }//unitList��getter
		const std::vector<RouteInfo>& GetRoute()const{ return route; }
		Unit *GetOperateUnit()const{ return pOperateUnit; }
		Unit *GetAttackedUnit()const{ return pAttackedUnit; }
	private:
		//���Q(�����̈ʒu���́A��ʒu������g�嗦�̉e�����󂯂Ă��Ȃ��A���O���̂܂܂̏��ł���)
		std::vector<Unit> unitList;//pOperateUnit��pAttackedUnit�����̗v�f���w���Ă���s����A�O������̕ύX��h�������̂�private
		std::vector<RouteInfo> route;//pOperateUnit�̈ړ��o�H
		//unitList�̒��g�̂ǂꂩ���w���|�C���^
		Unit *pOperateUnit;
		Unit *pAttackedUnit;//nullptr�Ȃ�A�N�V�������̕`����s��Ȃ�
	};
	//�~�j�}�b�v�̕`��̎d�����L�^���邽�߂̍\����
	class MinimapLayoutBase{
	public:
		MinimapLayoutBase(){}
		virtual ~MinimapLayoutBase()=default;
		virtual void DrawMinimap()const=0;
		virtual void Update(){}//�������X�V���K�v����΍s�����A�ʏ�͉����X�V����K�v���Ȃ�
	};
	class NormalDraw:public MinimapLayoutBase{
	public:
		//���ʂɕ`�悷��
		NormalDraw(const std::function<void()> &drawFunc):m_drawFunc(drawFunc){}
		~NormalDraw()=default;
		void DrawMinimap()const{
			m_drawFunc();
		}
	private:
		const std::function<void()> m_drawFunc;
	};
	class ExtendDraw:public MinimapLayoutBase{
	public:
		//�g�債�Ȃ���`�悷��
		ExtendDraw(const std::function<void(int,int,float)> &drawFunc,const PositionControl &startPos,const Easing &width)
			:m_drawFunc(drawFunc)
			,m_startPos(startPos)
			,m_width(width)
		{}
		~ExtendDraw()=default;
		void DrawMinimap()const;
		void Update(){
			m_startPos.Update();
			m_width.Update();
		}
	private:
		const std::function<void(int,int,float)> m_drawFunc;
		PositionControl m_startPos;
		Easing m_width;
	};

	//�֐�
	void DrawResizedMap(int x,int y,const MinimapDrawInfo &minimapInfo,const float rate)const;//�ȈՊg��k���}�b�v��`�悷��
	void InitReflectionWork();//���t���N�V�������[�N���쐬���鏈��(�`��ʒu�̒����̂��߂̏�񂪂��̃N���X���ɑ��݂���̂ŁAWholeReflectionInfo�N���X�łȂ����̃N���X���ɏ������L�q����)
	void DrawTwoMinimap()const;//2�Ȃ�ׂĕ��ʂɃ~�j�}�b�v��`�悷��
	//���z�֐��̃I�[�o�[���C�h
	int thisCalculate();//0:���O��ʂɖ߂� 1~END-1:���̏�ʂɐi�� END:�o�g���p�[�g�I��
	void thisDraw()const;
	int UpdateNextScene(int index);//����ʂ̐ݒ�Bindex�ɖ߂肽�����A���ۂɖ߂�邩�͕s���Ȃ̂Ŏ��ۂ̖߂�ꏊ������int�l��Ԃ������B��{�I�ɕԂ�l��index�ɓ������Ȃ�B
	void ReturnProcess();//���̏�ʂɖ߂��Ă������̏���
	//���[�N�쐬�֐�
	void SetDrawLineWork();
	void SetClickWork(const std::function<std::shared_ptr<const Shape>(Vector2D,Vector2D)> &conditionShapeFunc);
	void SetLineClickWork();
	void SetAreaClickWork();
	void SetSelectOneWork();
	void SetMoveSimulationWork();

	//�ϐ�
	const std::shared_ptr<BattleSceneData> m_battleSceneData;
	std::shared_ptr<BattleSceneElement> m_clearScene;//�O�N���X�ō쐬����ClearScene���ꎞ�I�ɕۑ����邽�߂̕ϐ�
	std::optional<MinimapDrawInfo> m_goodLogInfo,m_badLogInfo;//���������R���X�g���N�^��{}���ōs�������̂Œx���̕K�v������
	std::shared_ptr<ReflectionWork::Base> m_reflectionWork;//���ݍs���Ă��郊�t���N�V�������[�N
	std::shared_ptr<MinimapLayoutBase> m_layoutInfo;//�~�j�}�b�v���ǂ��\�����邩

	const int m_operateCursor;//���샆�j�b�g���w���������߂̃}�[�J�[
	const int m_predictNumberFont;
	const int m_predictExplainFont;
};

#endif // !DEF_SUBMISSIONREFLECTIONSCENE_H
