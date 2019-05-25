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
			const StringBuilder newBuilder(testStr,topSpliter,topBeginer,topEnder);
		}
	};
}