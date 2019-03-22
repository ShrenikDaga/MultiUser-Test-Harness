/////////////////////////////////////////////////////////////////////
// TestRequest.cpp - Creates and parses Test Requests              //
// ver 1.0                                                         //
// Jim Fawcett, CSE687 - Object Oriented Design, Fall 2018         //
/////////////////////////////////////////////////////////////////////

#include "TestRequest.h"
#include <iostream>

namespace Testing
{
	//----< add DLL name to request >----------------------------------

	inline void TestRequest::addDll(const Dll& dll)
	{
		request.valueRef().push_back(dll);
	}
	//----< serialize TestRequest to std::string >---------------------
	/*
	* - builds string holding comma separated list of TestRequest parts
	*/
	inline std::string TestRequest::toString()
	{
		std::string temp(name());
		temp += ", " + author() + ", " + date();
		for (auto dll : request())
		{
			temp += ", " + dll;
		}
		return temp;
	}
	//----< create TestRequest from serialized std::string >-----------
	/*
	* - Constructs TestRequest from a string previously created
	*   by a TestRequest instance.
	*/
	inline TestRequest TestRequest::fromString(const std::string& trStr)
	{
		TestRequest tr;
		std::vector<std::string> splits = Utilities::split(trStr);
		if (splits.size() < 3)
			return tr;
		tr.name(splits[0]);
		tr.author(splits[1]);
		tr.date(splits[2]);
		for (size_t i = 3; i < splits.size(); ++i)
		{
			tr.request.valueRef().push_back(splits[i]);
		}
		return tr;
	}
	//----< construction test for TestRequest >------------------------

	void demonstrate_TestRequest()
	{
		Utilities::title("Create TestRequest and Serialize:");
		TestRequest tr1;
		DateTime dt;
		tr1.name("tr1");
		tr1.author("Fawcett");
		tr1.date(dt.now());
		tr1.addDll("dll1");
		tr1.addDll("dll2");
		std::string tr1Str = tr1.toString();
		std::cout << "\n  " << tr1Str;
		std::cout << "\n";

		Utilities::title("Create new TestRequest from Serialized String");
		TestRequest tr2 = TestRequest::fromString(tr1Str);
		std::cout << "\n  tr2.name() = " << tr2.name();
		std::cout << "\n  tr2.author() = " << tr2.author();
		std::cout << "\n  tr2.date() = " << tr2.date();
		for (auto item : tr2.request())
		{
			std::cout << "\n  dll = " << item;
		}
		std::cout << "\n";
	}
}

#ifdef TEST_TESTREQUEST

using namespace Testing;

int main()
{
	Utilities::Title("Demonstrating TestRequest Class");
	std::cout << "\n";

	demonstrate_TestRequest();

	std::cout << "\n";
	return 0;
}

#endif
