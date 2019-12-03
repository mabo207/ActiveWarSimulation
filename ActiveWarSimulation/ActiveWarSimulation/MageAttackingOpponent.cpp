#include"DxLib.h"
#include"MageAttackingOpponent.h"
#include"BattleSceneData.h"
#include"AttackLog.h"

SubmissionEvaluation MageAttackingOpponent::RubricEvaluate(const std::vector<BattleObject *> &field,const Vector2D stageSize,const std::shared_ptr<const LogElement> &evaluateLog)const{
	//- —áŠOˆ—
	//	- UŒ‚‚µ‚È‚¢(-1)
	//- •]‰¿
	//	0. “G‚ª–‚“¹mE‰q¶•º‚Å‚ ‚é
	//	1. “G‚ª•ºmEËè‚Å‚ ‚é
	//	2. “G‚ªd‘••º‚Å‚ ‚é
	const std::shared_ptr<const AttackLog> attackLog=std::dynamic_pointer_cast<const AttackLog>(evaluateLog);
	SubmissionEvaluation evaluate;
	if(!attackLog){
		//ƒƒO‚ªAttackLog‚Å‚È‚¢ê‡‚ÍuUŒ‚‚ğ‚µ‚È‚©‚Á‚½v‚Æ”»’f‚Å‚«‚é
		evaluate=SubmissionEvaluation::e_noevaluation;
	} else{
		//UŒ‚‘Šè‚Ì•ºí‚ğŒ©‚ÄA•]‰¿‚ğs‚¤
		switch(attackLog->GetAimedUnit()->GetBaseStatus().profession){
		case(Unit::Profession::e_healer):
		case(Unit::Profession::e_mage):
			evaluate=SubmissionEvaluation::e_bad;
			break;
		case(Unit::Profession::e_archer):
		case(Unit::Profession::e_soldier):
			evaluate=SubmissionEvaluation::e_ok;
			break;
		case(Unit::Profession::e_armer):
			evaluate=SubmissionEvaluation::e_excellent;
			break;
		}
	}

	return evaluate;
}

std::string MageAttackingOpponent::GetWholeLookBack(SubmissionEvaluation mostFrequentEvaluate)const{
	std::string comment;
	if(mostFrequentEvaluate==SubmissionEvaluation::e_noevaluation){
		//UŒ‚‚µ‚Ä‚¢‚È‚¢
		comment="‚à‚Á‚Æ–‚“¹m‚ÅUŒ‚‚µ‚Ä‚İ‚æ‚¤I";
	} else if(mostFrequentEvaluate==SubmissionEvaluation::e_bad){
		//–‚–@Œnƒ†ƒjƒbƒg‚Ö‚ÌUŒ‚
		comment="–‚–@UŒ‚‚ÍA–‚–@‚ğg‚¤“G‚æ‚è•Ší‚Åí‚¤“G‚É‚µ‚½•û‚ªŒø‰Ê“I‚¾‚æI";
	} else if(mostFrequentEvaluate==SubmissionEvaluation::e_ok){
		//ËèE•ºm‚Ö‚ÌUŒ‚
		comment="‚Å‚«‚ê‚ÎA•Ší‚ğg‚¤“G‚Ì’†‚Å‚à“Á‚É–‚–@‚Éã‚¢d‘••º‚ğ‚à‚Á‚Æ‘_‚Á‚Ä‚İ‚æ‚¤I";
	} else if(mostFrequentEvaluate==SubmissionEvaluation::e_excellent){
		//d‘••º‚Ö‚ÌUŒ‚
		comment="Œ¾‚¤‚±‚Æ‚È‚µ‚Å‚·I–‚–@‚Éã‚¢“G‚ğ‘_‚Á‚ÄUŒ‚‚Å‚«‚Ä‚¢‚Ü‚·I";
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

std::string MageAttackingOpponent::GetReason(SubmissionEvaluation rubric)const{
	if(rubric==SubmissionEvaluation::e_noevaluation){
		//•`‰æ‚ğs‚í‚È‚¢
		return "";
	} else if(rubric==SubmissionEvaluation::e_bad){
		return "–‚“¹m‚â‰q¶•º‚É‚Í–‚–@UŒ‚‚ÍŒø‰Ê‚ª”–‚¢‚İ‚½‚¢‚¾B";
	} else if(rubric==SubmissionEvaluation::e_ok){
		return "•ºm‚âËè‚ğ–‚–@‚ÅUŒ‚‚·‚é‚Ì‚ÍA‚Ü‚ ‚Ü‚ Œø‰Ê“I‚¾B";
	} else if(rubric==SubmissionEvaluation::e_excellent){
		return "–‚–@‚ªã“_‚Ìd‘••º‚ğUŒ‚‚Å‚«‚Ä‚¢‚Ü‚·I";
	}
	return "";
}
