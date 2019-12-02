#include"DxLib.h"
#include"SubmissionEvaluation.h"

//------------SubmissionEvaluation------------
bool SubmissionEvaluation::operator==(const SubmissionEvaluation &otherobj)const{
	return this->m_kind==otherobj.m_kind;
}

bool SubmissionEvaluation::operator<(const SubmissionEvaluation &otherobj)const{
	return this->m_kind<otherobj.m_kind;
}

unsigned int SubmissionEvaluation::Color()const{
	switch(m_kind){
	case(e_bad):
		return GetColor(96,96,196);
	case(e_ok):
		return GetColor(128,128,196);
	case(e_good):
		return GetColor(196,196,64);
	case(e_great):
		return GetColor(196,128,96);
	case(e_excellent):
		return GetColor(196,64,128);
	}
	return GetColor(0,0,0);
}

std::string SubmissionEvaluation::GetString()const{
	switch(m_kind){
	case(e_bad):
		return "Bad...";
	case(e_ok):
		return "OK";
	case(e_good):
		return "Good!";
	case(e_great):
		return "Great!!";
	case(e_excellent):
		return "Excellent!!!";
	}
	return "";
}
