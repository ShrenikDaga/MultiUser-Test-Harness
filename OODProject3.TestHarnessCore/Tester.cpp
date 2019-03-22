/////////////////////////////////////////////////////////////////////
// Tester.cpp - Prototype for Test Harness child Tester            //
// ver 1.0														   //
//Author:	Shrenik Daga, Syracuse Unicersity					   //
//Source:	Jim Fawcett, CSE687 - Object Oriented Design, Fall 2018//
/////////////////////////////////////////////////////////////////////

#include "Tester.h"
#include "../OODProject3.DllLoader/ITests.h"
#include "../Utilities/TestUtilities/TestUtilities.h"
#include "../Utilities/FileSystem/FileSystem.h"
#include "../Utilities/FileUtilities/FileUtilities.h"
#include "../Utilities/StringUtilities/StringUtilities.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <memory>
#include <vector>

namespace Testing
{

	//----< initialize paths and logger >------------------------------

	Tester::Tester(EndPoint from, EndPoint to)
		:comm_(from,"Tester"),from_(from),to_(to)
	{
		comm_.start();
		comm_.setSendFilePath("../TestLogs");
		requestPath("../TestRequests");
		libPath("../TestDLLs");
		pLog(Utilities::Logger<0, Utilities::Lock>::getInstance());
		pLog()->setTerminator("");
	}
	//----< deallocate logger >----------------------------------------

	Tester::~Tester()
	{
		delete pLog();
	}

	void Tester::loadTestRequests()
	{
		std::vector<std::string> files = FileSystem::Directory::getFiles(requestPath());
		std::ifstream in;
		for (auto file : files)
		{
			if (!Utilities::openReadFile(requestPath() + "\\" + file, in))
				continue;
			std::ostringstream sstr;
			sstr << in.rdbuf();
			TestRequest tr = TestRequest::fromString(sstr.str());
			requests.valueRef().push_back(tr);
			pLog()->write("\n  loaded TestRequest \"" + tr.name() + "\"");
			pLog()->write("\n  " + tr.toString() + "\n");
		}
	}

	void Tester::loadLibraries(TestRequest& tr)
	{
		std::string path = FileSystem::Path::getFullFileSpec(libPath());
		pLog()->write("\n  loading from \"" + path + "\"");

		std::vector<std::string> files;
		
		for (Dll dll : tr.request.valueRef())
		{
			std::cout << "$dll: " << dll;
			files.push_back(dll);
		}

		for (auto file : files)
		{
			std::cout << "$dll: " << file;
			pLog()->write("\n    " + file);
			std::string fileSpec = path + "\\" + file;
			dllLoader_.loadAndExtract(fileSpec);
		}
		pLog()->write("\n");
	}
	
	bool Tester::doTests()
	{
		using sPtr = std::shared_ptr<ITest>;
		Utilities::TestExecutive te;
		for (ITest* test : dllLoader_.tests()) {
			// Host passes logger resource pointer to test
			// - test will use logger via this pointer
			test->acceptHostedResource(pLog.valueRef());
			sPtr pTest(test);
			te.registerTest(pTest, pTest->name());
		}
		bool result = te.doTests();
		return result;
	}

	void Tester::runTests(TestRequest tr, MsgPassingCommunication::Message ready)
	{
		
		while (true)
		{
			comm_.postMessage(ready);
			MsgPassingCommunication::Message msg = comm_.getMessage();
			if (msg.command() == "testReq")
			{
				std::string fileSpec = "..\\TestLogs\\"+msg.value("FileName")+"LogFile.txt";
				std::ofstream outfileStrm(fileSpec, std::ios::out);
				if (outfileStrm.good())
					pLog()->addStream(&outfileStrm);

				tr = TestRequest::fromString(msg.value("Content"));
				std::cout << "\n\n*HTTP style message:\n" << msg.toString()<<"*\n";
				loadLibraries(tr);
				doTests();
				clear();
				pLog()->removeStream(&outfileStrm);
				outfileStrm.close();

				MsgPassingCommunication::Message logMessage;
				logMessage.to(msg.from());
				logMessage.from(msg.to());
				logMessage.value("sendingFile");
				logMessage.attribute("FileName",msg.value("FileName"));
				logMessage.command("sendingFile");
				logMessage.name("ResultFile");
				logMessage.file(msg.value("FileName") + "LogFile.txt");
				comm_.postMessage(logMessage);
			}
			if (msg.command() == "STOP")
			{
				exit(0);
			}
			
		}
		
	}
}


using namespace Testing;

int main(int argc, char* argv[])
{
	std::cout << Utilities::sTitle("Testing Tester using Dll Loader and Logger");

	int  port1 = std::stoi(argv[0]), port2=std::stoi(argv[1]);

	EndPoint ep3("localhost", port1);
	EndPoint ep4("localhost", port2);
	std::cout << "\nInitialing tester";

	Tester tester(ep3,ep4);
	TestRequest tr;
	//Initialize its comm and send a Ready message
	MsgPassingCommunication::Message ready;
	ready.from(tester.from_);
	ready.to(tester.to_);
	ready.command("ready");
	std::cout << "\n      Tester sending msg: " + ready.command();
	tester.recv = std::thread([&]() { tester.runTests(tr,ready); });
	tester.recv.join();

	Utilities::putline(2);
	return 0;

}
