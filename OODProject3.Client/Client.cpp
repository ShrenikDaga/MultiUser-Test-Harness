#pragma once
/////////////////////////////////////////////////////////////////////
// Client.cpp - creates TestRequest and stores in requestPath      //
// ver 2.0                                                         //
//Author:	Shrenik Daga, Syracuse University					   //
//Source:	Jim Fawcett, CSE687 - Object Oriented Design, Fall 2018 //
/////////////////////////////////////////////////////////////////////
/*
*  Package Operations:
*  -------------------
*   Client creates test request using the mock_Test() method
*	  and uses the Comm channel to send these messages to the ProcessPool
*	Client implents a getLogs() method to get results backfrom the testers in 
*	  TestHarness
*
*  Required Files:
*  ---------------
*  Client.h
*  TestRequest.h, TestRequest.cpp
*  FileUtilities.h, FileUtilities.cpp
*  DateTime.h, DateTime.cpp	
*
*  Maintenance History:
*  --------------------
*  ver 1.0 : 23 Oct 2018
*  - first release
*/

#include "Client.h"
#include "../OODProject3.TestRequest/TestRequest.h"
#include "../Utilities/FileUtilities/FileUtilities.h"
#include <iostream>


namespace Testing
{
	std::string mock_Test(int i)
	{
		TestRequest tr1;
		if (i == 0)
		{
			tr1.name("tr1");
			tr1.request.valueRef().push_back("OODProject3.TestDriver1.dll");
		}
		
		else if (i == 1)
		{
			tr1.name("tr2");
			tr1.request.valueRef().push_back("OODProject3.TestDriver2.dll");
		}
		else if (i == 2)
		{
			tr1.name("tr3");
			tr1.request.valueRef().push_back("OODProject3.TestDriver1.dll");
			tr1.request.valueRef().push_back("OODProject3.TestDriver2.dll");
		}
		else if (i == 3)
		{
			tr1.name("trR");
			tr1.request.valueRef().push_back("OODProject3.Requirements.dll");
		}

		tr1.author("Shrenik");
		tr1.date(DateTime().now());
	
		Utilities::title("\nCreated test request:");
		std::cout << "\n" << tr1.toString();


		return tr1.toString();
	}
}

using namespace MsgPassingCommunication;

Client::Client(EndPoint from, EndPoint to) : comm_(from, "sender"), to_(to), from_(from)
{
	comm_.start();
	comm_.setSendFilePath("../TestLibraries");
	comm_.setSaveFilePath("../TestLogsClient");
}
//----< stops comm and detaches thread if needed >-----------------

Client::~Client()
{
	std::cout << "\n  Client Comm shutting down";
	comm_.stop();
	if (recv.joinable())
		recv.detach();
	if (logs.joinable())
		logs.detach();
}
//----< creates thread to send test request messages >-------------

void Client::start()
{
	std::cout << "\n  starting Client's send thread listening on " + from_.toString();
	recv = std::thread([&]() { sendMessages(); });
	if (recv.joinable())
		recv.join();
	logs = std::thread([&]() { getLogs(); });
	if (logs.joinable())
		logs.join();
}
//----< waits for all messages to be sent >------------------------

void Client::wait()
{
	if (recv.joinable())
		recv.join();
	if (logs.joinable())
		logs.join();

	
}

void Client::getLogs()
{
	while(true)
	{
		MsgPassingCommunication::Message msg = comm_.getMessage();
		if (msg.command() == "sendingFile")
		{
			testCounter--;
			Utilities::title("\nResultsIncoming:");
			std::cout << "\nTest Result for TestReq:" << msg.value("FileName") << " received";
			std::cout << "\nYet to receive " << testCounter << " more results.\n";
		}
		if (testCounter == 0)
		{
			std::cout << "\nReceived all the results.Closing down after enter is pressed.\n";
			getchar();
			MsgPassingCommunication::Message stopMsg;
			stopMsg.to(to_); stopMsg.from(from_);
			stopMsg.name("STOP");
			stopMsg.command("STOP");
			comm_.postMessage(stopMsg);
			//exit(0);
		}
	}
}

void Client::sendMessages()
{
	MsgPassingCommunication::Message fileMsg;
	fileMsg.to(to_);fileMsg.from(from_);
	fileMsg.value("sendingFile");
	fileMsg.command("sendingFile");
	fileMsg.name("ReqDLL");
	fileMsg.file("OODProject3.Requirements.dll");
	comm_.postMessage(fileMsg);

	MsgPassingCommunication::Message testReq;
	testReq.name("Requirements");
	testReq.attribute("Content", Testing::mock_Test(3));
	testReq.attribute("FileName", "TestRequirements");
	testReq.to(to_);testReq.from(from_);
	testReq.command("testReq");
	comm_.postMessage(testReq);testCounter++;

	std::this_thread::sleep_for(std::chrono::milliseconds(10000));
	
	MsgPassingCommunication::Message fileMsg1;
	fileMsg1.to(to_);fileMsg1.from(from_);
	fileMsg1.value("sendingFile");
	fileMsg1.command("sendingFile");
	fileMsg1.name("DLL2");
	fileMsg1.file("OODProject3.TestDriver2.dll");
	comm_.postMessage(fileMsg1);

	fileMsg1.name("ReqDLL");
	fileMsg1.file("OODProject3.TestDriver1.dll");
	comm_.postMessage(fileMsg1);

	std::this_thread::sleep_for(std::chrono::milliseconds(3000));

	MsgPassingCommunication::Message testReq1;
	testReq1.name("TR1");
	testReq1.attribute("Content",Testing::mock_Test(0));
	testReq1.attribute("FileName","TestReq1");
	testReq1.to(to_);testReq1.from(from_);
	testReq1.command("testReq");
	comm_.postMessage(testReq1);testCounter++;

	testReq1.attribute("Content", Testing::mock_Test(1));
	testReq1.attribute("FileName", "TestReq2");
	comm_.postMessage(testReq1);testCounter++;

	testReq1.attribute("Content", Testing::mock_Test(2));
	testReq1.attribute("FileName", "TestReq3");
	comm_.postMessage(testReq1);testCounter++;
}


using namespace Testing;

int main()
{
	Utilities::Title("Demonstrating Client");

	MsgPassingCommunication::EndPoint ep1("localhost", 8081);  // client
	MsgPassingCommunication::EndPoint ep2("localhost", 8082);
	
	Client client(ep1,ep2);
	std::string app = "../Debug/OODProject3.ProcessPool.exe";
	Process ProcessPool;
	ProcessPool.title("ProcessPool");
	ProcessPool.application(app);
	ProcessPool.commandLine("1111 2222");
	ProcessPool.create();

	std::this_thread::sleep_for(std::chrono::milliseconds(3000));
	client.start();
	client.wait();
	return 0;
}

