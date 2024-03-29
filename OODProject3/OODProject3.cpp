// OODProject3.cpp : Defines the entry point for the console application.
//





#define IN_DLL

#define ARG_SIZE 256  // for the manufactured cmd args

#include "ITests.h"
#include <iostream>
#include <string>
#include "stdafx.h"
#include <string.h>
#include "../Utilities/FileUtilities/FileUtilities.h"
#include "../Utilities/StringUtilities/StringUtilities.h"

using namespace Utilities;
using namespace std;


// Concrete Test class that implements ITest.
// It performs tests on ProcessCmdArgs and Convereter classes in a single test class.

class Requirements : public ITest 
{

public:
	DLL_DECL bool test();
	DLL_DECL std::string name();
	DLL_DECL std::string author();
	//DLL_DECL std::string path = "..";

	DLL_DECL void requirement1();

	// get access to Hosted Resource
	DLL_DECL void acceptHostedResource(ILog* pRes)
	{
		pRes_ = pRes;
	}

private:
	ILog * pRes_ = nullptr;
};

DLL_DECL void Requirements::requirement1()
{
	Utilities::Title("OOD Project 3 Requirements 1 through 7, followed by actual run of code.");
	Utilities::showDirContents("../OODProject3.Client", "Showing the lsit of c++ files used in this project.");
	Utilities::title("***********************************************");
}

DLL_DECL bool Requirements::test()
{
	requirement1();

}
DLL_DECL std::string Requirements::name()
{
	return string("Project3 Requirements");
}

DLL_DECL std::string Requirements::author()
{
	return string("Shrenik");
}

class TestCollection : public ITests {
	DLL_DECL std::vector<ITest*> tests();
};

DLL_DECL std::vector<ITest*> TestCollection::tests()
{
	std::vector<ITest*> tests_vec;
	tests_vec.push_back(new Requirements);

	return tests_vec;
}
