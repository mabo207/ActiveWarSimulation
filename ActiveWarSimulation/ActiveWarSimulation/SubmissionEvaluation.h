#ifndef DEF_SUBMISSIONEVALUATION_H
#define DEF_SUBMISSIONEVALUATION_H

#include<string>

//サブミッションの評価を記録する
struct SubmissionEvaluation{
	//SubmissionEvaluationを要素がSubmissionEvaluation::Kindなenum classっぽく使えるように工夫する
public:
	//要素一覧
	enum Kind{
		e_noevaluation=0x00
		,e_bad=0x01
		,e_ok=0x02
		,e_good=0x04
		,e_great=0x08
		,e_excellent=0x10
	};

	//必要な関数
	SubmissionEvaluation()noexcept
		:SubmissionEvaluation(e_noevaluation){}
	SubmissionEvaluation(Kind kind)noexcept
		:m_kind(kind){}
	~SubmissionEvaluation()=default;
	bool operator==(const SubmissionEvaluation &otherobj)const;
	bool operator!=(const SubmissionEvaluation &otherobj)const{
		return !(*this==otherobj);
	}
	bool operator<(const SubmissionEvaluation &otherobj)const;
	bool operator>(const SubmissionEvaluation &otherobj)const{
		return otherobj<(*this);
	}
	bool operator<=(const SubmissionEvaluation &otherobj)const;
	bool operator>=(const SubmissionEvaluation &otherobj)const{
		return otherobj<=(*this);
	}
	//色と文字列を返す
	unsigned int Color()const;
	std::string GetString()const;

private:
	Kind m_kind;
};

#endif // !DEF_SUBMISSIONEVALUATION_H
