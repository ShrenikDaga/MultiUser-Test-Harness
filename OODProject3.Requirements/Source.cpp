/////////////////////////////////////////////////////////////////////
// Source.cpp - Automated testsuite made as a DLL for OOD Project3 //
// ver 2.0                                                         //
//Author:   Shrenik Daga, CSE687 - Object Oriented Design, Fall 18 //
//Source:	Dr Jim Fawcett, Syracuse University                    //
/////////////////////////////////////////////////////////////////////
/*
*  Package Operations:
*  -------------------
*  Source.cpp is a part of the Dynamic Link Library
*	which runs the automated testsuite
*
*  Required Files:
*  ---------------
*  ITests.h
*  FileUtilities.h,
*  StringUtilities.h
*
*  Maintenance History:
*  --------------------
*  Ver 2.0 : 04 Dec 2018
*  - changed and added methods according to Project4 requirements
*  ver 1.0 : 14 Nov 2018
*  - first release
*/

#define IN_DLL
#define ARG_SIZE 256  // for the manufactured cmd args

#include "ITests.h"
#include <iostream>
#include <string>
#include <string.h>
#include "../Utilities/FileUtilities/FileUtilities.h"
#include "../Utilities/StringUtilities/StringUtilities.h"

using namespace std;

class Requirements : public ITest
{

public:
	DLL_DECL bool test();
	DLL_DECL std::string name();
	DLL_DECL std::string author();

	DLL_DECL void requirement1();
	DLL_DECL void requirement2();
	DLL_DECL void requirement3();
	DLL_DECL void requirement4();
	DLL_DECL void requirement5();
	DLL_DECL void requirement6();
	DLL_DECL void requirement7();
	DLL_DECL void requirement8();
	DLL_DECL void requirement9();
	DLL_DECL void requirement10();

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
	std::string path = "../OODProject3.ProcessPool", msg = "Requirement1: Showing the lsit of c++ files used in this project.";
	Utilities::Title("OOD Project 4 Requirements 1 through 10:");
	Utilities::showDirContents(path, msg);
	Utilities::title("***********************************************");
}

DLL_DECL void Requirements::requirement2()
{
	std::string path = "../OODProject4.Client", msg = "Requirement2: Showing use of WPF for user display.";
	Utilities::showDirContents(path, msg);
	Utilities::title("***********************************************");
}

DLL_DECL void Requirements::requirement3()
{
	std::string path = "../OODProject3.ProcessPool/TestHarness.cpp", msg = "Requirement3a: Shows the working parts of Projects 1,2 and 3. Showing TestHarness, also shown the ChildProcess creation.";
	Utilities::showFileLines(path, 134, 170, msg);
	path = "../OODProject3.TestHarnessCore/Tester.cpp";
	msg = "Requirement3b: Showing the ChildProcess having functionality of doTests.";
	Utilities::showFileLines(path, 98, 109, msg);
	path = "../OODProject3.ProcessPool/TestHarness.cpp";
	msg = "Requirement3c: Showing lines of code which allows receiving of messages from client irrepective of the messages sent to the client. You can verify this while using the GUI. Line 114, 117";
	Utilities::showFileLines(path, 77, 96, msg);
	Utilities::title("***********************************************");
}

DLL_DECL void Requirements::requirement4()
{
	std::cout << "\nRequirement4: As you can see the GUI on the screen, there are different ListBoxes used for showing the directory contents, doubleclicks enable selection of Test Libraries.\nExecute button creates a testRequest message and send it to the TestHarness.";
	Utilities::title("***********************************************");
}

DLL_DECL void Requirements::requirement5()
{
	std::string path = "../OODProject3.TestHarnessCore/Tester.cpp", msg = "Requirement5a: Showing lines of code which use the Message class to create messages tp be sent to the client. ";
	Utilities::showFileLines(path, 132, 141, msg);

	path = "../OODProject4.Client/bin/MainWindow.xaml.cs";
	msg = "Requirement5b: Showing lines of code which use the Message class to create messages to be sent to the TestHarness. ";
	Utilities::showFileLines(path, 103, 114, msg);
	Utilities::title("***********************************************");
}

DLL_DECL void Requirements::requirement6()
{
	std::string path = "../OODProject4.Client/bin/MainWindow.xaml.cs", msg = "Requirement6: Showing lines of code which use the Message class to create TestRequest messages to be sent to the TestHarness.";
	Utilities::showFileLines(path, 238, 247, msg);
	Utilities::title("***********************************************");
}

DLL_DECL void Requirements::requirement7()
{
	std::string path = "../OODProject3.ProcessPool/TestHarness.cpp", msg = "Requirement7: Showing the code where TestHarness creates multiple ChildProcesses and each TestRequest from the client is executed by either one of these ChildProcesses.";
	Utilities::showFileLines(path, 152, 170, msg);
	Utilities::title("***********************************************");
}

DLL_DECL void Requirements::requirement8()
{
	std::string path = "../OODProject3.TestHarnessCore/Tester.cpp", msg = "Requirement8: Showing the code which shows ChildProcess sends a ResultLog file back to the client. The ResultLog file is saved in the TestLogs directory and then sent over to the Clients TestLogsClient directory using Comm. ";
	Utilities::showFileLines(path, 133, 141, msg);
	Utilities::title("***********************************************");
}

DLL_DECL void Requirements::requirement9()
{
	std::cout << "\nAs you can see, there are two Client GUI windows running and sending testReqs from both of them can be executed successfully.";
	Utilities::title("***********************************************");
}

DLL_DECL void Requirements::requirement10()
{
	std::string path = "../OODProject3.Requirements/Source.cpp", msg = "Requirement10: Showing the code which runs the automated testsuite as a dll, which was sent over as a testRequest for this demonstration.";
	Utilities::showFileLines(path, 66, 74, msg);
	Utilities::title("***********************************************");
}

DLL_DECL bool Requirements::test()
{
	Requirements::requirement1();
	Requirements::requirement2();
	Requirements::requirement3();
	Requirements::requirement4();
	Requirements::requirement5();
	Requirements::requirement6();
	Requirements::requirement7();
	Requirements::requirement8();
	Requirements::requirement9();
	Requirements::requirement10();

	return true;
}
DLL_DECL std::string Requirements::name()
{
	return string("Project4 Requirements");
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

DLL_DECL ITests* get_ITests()
{
	return new TestCollection;
}
