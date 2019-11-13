#include"DxLib.h"
#include"MageAttackingOpponent.h"
#include"BattleSceneData.h"
#include"AttackLog.h"

int MageAttackingOpponent::RubricEvaluate(const std::vector<BattleObject *> &field,const Vector2D stageSize,const std::shared_ptr<const LogElement> &evaluateLog)const{
	//- —áŠOˆ—
	//	- UŒ‚‚µ‚È‚¢(-1)
	//- •]‰¿
	//	0. “G‚ª–‚“¹mE‰q¶•º‚Å‚ ‚é
	//	1. “G‚ª•ºmEËè‚Å‚ ‚é
	//	2. “G‚ªd‘••º‚Å‚ ‚é
	const std::shared_ptr<const AttackLog> attackLog=std::dynamic_pointer_cast<const AttackLog>(evaluateLog);
	int evaluate;
	if(!attackLog){
		//ƒƒO‚ªAttackLog‚Å‚È‚¢ê‡‚ÍuUŒ‚‚ğ‚µ‚È‚©‚Á‚½v‚Æ”»’f‚Å‚«‚é
		evaluate=-1;
	} else{
		//UŒ‚‘Šè‚Ì•ºí‚ğŒ©‚ÄA•]‰¿‚ğs‚¤
		switch(attackLog->GetAimedUnit()->GetBaseStatus().profession){
		case(Unit::Profession::e_healer):
		case(Unit::Profession::e_mage):
			evaluate=0;
			break;
		case(Unit::Profession::e_archer):
		case(Unit::Profession::e_soldier):
			evaluate=1;
			break;
		case(Unit::Profession::e_armer):
			evaluate=2;
			break;
		}
	}

	return evaluate;
}

std::pair<std::string,unsigned int> MageAttackingOpponent::GetRubricStringInfo(int rubric)const{
	std::string rubricStr;
	unsigned int edgeColor;
	switch(rubric){
	case(-1):
		rubricStr="";
		edgeColor=GetColor(0,0,0);
		break;
	case(0):
		//ˆ«‚¢
		rubricStr="Bad";
		edgeColor=GetColor(96,96,196);
		break;
	case(1):
		//‚Ü‚ ‚Ü‚ 
		rubricStr="OK";
		edgeColor=GetColor(128,128,196);
		break;
	case(2):
		//Š®àø
		rubricStr="Good!!";
		edgeColor=GetColor(196,196,64);
		break;
	}
	return std::make_pair(rubricStr,edgeColor);
}

std::string MageAttackingOpponent::GetWholeLookBack(int mostFrequentEvaluate)const{
	std::string comment;
	switch(mostFrequentEvaluate){
	case(-1):
		//UŒ‚‚µ‚Ä‚¢‚È‚¢
		comment="‚à‚Á‚Æ–‚“¹m‚ÅUŒ‚‚µ‚Ä‚İ‚æ‚¤I";
		break;
	case(0):
		//–‚–@Œnƒ†ƒjƒbƒg‚Ö‚ÌUŒ‚
		comment="–‚–@UŒ‚‚ÍA–‚–@‚ğg‚¤“G‚æ‚è•Ší‚Åí‚¤“G‚É‚µ‚½•û‚ªŒø‰Ê“I‚¾‚æI";
		break;
	case(1):
		//ËèE•ºm‚Ö‚ÌUŒ‚
		comment="‚Å‚«‚ê‚ÎA•Ší‚ğg‚¤“G‚Ì’†‚Å‚à“Á‚É–‚–@‚Éã‚¢d‘••º‚ğ‚à‚Á‚Æ‘_‚Á‚Ä‚İ‚æ‚¤I";
		break;
	case(2):
		//d‘••º‚Ö‚ÌUŒ‚
		comment="Œ¾‚¤‚±‚Æ‚È‚µ‚Å‚·I–‚–@‚Éã‚¢“G‚ğ‘_‚Á‚ÄUŒ‚‚Å‚«‚Ä‚¢‚Ü‚·I";
		break;
	}
	return comment;
}

std::string MageAttackingOpponent::GetWholeLookBackActionEmpty()const{
	return "‚à‚Á‚Æ–‚“¹m‚ÅUŒ‚‚µ‚Ä‚İ‚æ‚¤I";
}

std::string MageAttackingOpponent::GetSubmissionExplanation()const{
	return "–‚“¹m‚Å‚ÌUŒ‚‚ÌÛ‚ÍA\n–‚–@‚Éã‚¢“G‚ğ‘_‚¤–‚ğS‚ª‚¯‚æ‚¤I";
}

bool MageAttackingOpponent::JudgeEvaluateOrder(const BattleSceneData * const battleData)const{
	return (battleData->m_operateUnit->GetBattleStatus().team==Unit::Team::e_player
		&& battleData->m_operateUnit->GetBaseStatus().profession==Unit::Profession::e_mage);
}

std::string MageAttackingOpponent::GetReason(int rubric)const{
	switch(rubric){
	case(-1):
		//•`‰æ‚ğs‚í‚È‚¢
		return "";
	case(0):
		return "–‚“¹m‚â‰q¶•º‚É‚Í–‚–@UŒ‚‚ÍŒø‰Ê‚ª”–‚¢‚İ‚½‚¢‚¾B";
	case(1):
		return "•ºm‚âËè‚ğ–‚–@‚ÅUŒ‚‚·‚é‚Ì‚ÍA‚Ü‚ ‚Ü‚ Œø‰Ê“I‚¾B";
	case(2):
		return "–‚–@‚ªã“_‚Ìd‘••º‚ğUŒ‚‚Å‚«‚Ä‚¢‚Ü‚·I";
	}
	return "";
}
