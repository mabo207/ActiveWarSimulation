#include "stdafx.h"
#include "CppUnitTest.h"
#include"../ActiveWarSimulation/StringBuilder.h"
#include"../ActiveWarSimulation/FileRead.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest_StringBuilder
{		
	TEST_CLASS(UnitTest1)
	{
	public:
		
		TEST_METHOD(TestMethod1)
		{
			// TODO: テスト コードをここに挿入します
			const std::string testStr=FileStrRead("../ActiveWarSimulation/SaveData/scoreRanking.txt");
			const char topSpliter='\n'
				,topBeginer='{'
				,topEnder='}'
				,inSpliter=','
				,inBeginer='('
				,inEnder=')';
			const StringBuilderOld oldBuilder(testStr,topSpliter,topBeginer,topEnder,false,true);
			const StringBuilder newBuilder(std::shared_ptr<const std::string>(new std::string(testStr)),testStr.size(),topSpliter,topBeginer,topEnder,'\0',0);
			Assert::AreEqual(oldBuilder.m_vec.size(),newBuilder.m_vec.size());
		}
	};
}