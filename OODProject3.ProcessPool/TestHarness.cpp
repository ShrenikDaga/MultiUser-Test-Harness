/////////////////////////////////////////////////////////////////////
// TestHarness.cpp - ProcessPool for OOD Project3, Fall 2018       //
// ver 2.0                                                         //
//Author:   Shrenik Daga, CSE687 - Object Oriented Design, Fall 18 //
//Source:	Dr Jim Fawcett, Syracuse University                    //
/////////////////////////////////////////////////////////////////////
/*
*  Package Operations:
*  -------------------
*  TestHarness.cpp is the funcatioanlity of the ProcessPool of Project 3
*	Supports reaceving and sending of TestReqaut messages and Ready messages using
*	the Comm channel provided by Dr Fawcett.
*	Supports enqueing and dequeing of the messages.
*	Supports dynamic spawing of processes using the Process project provided by Dr Fawcett.
*
*  Required Files:
*  ---------------
*  TestHarness.h
*  Process.h
*
*  Maintenance History:
*  --------------------
*  ver 2.0 : 14 Nov 2018
*  ver 1.0 : 6 Nov 2018
*  - first release
*/

#include "TestHarness.h"
#include "../Process/Process.h"


TestHarness::TestHarness(EndPoint from, EndPoint to,const int count)
	: comm_(from, "recvr"), to_(to), from_(from)
{
	comm_.start();
	comm_.setSaveFilePath("../TestDLLs");
	numberOfChild = count;
}

TestHarness::~TestHarness()
{
	std::cout << "\n    TestHarness Comm shutting down";
	std::cout << "\n    requestQ size = " + Utilities::Converter<size_t>::toString(requestQ_.size());
	std::cout << "\n    readyQ size   = " + Utilities::Converter<size_t>::toString(readyQ_.size());
	comm_.stop();
}

void TestHarness::start()
{
	std::cout << "\n    starting TestHarness receive thread listening on " << from_.toString();
	recvr = std::thread([&]() { recvMessages(); });
	recvr.detach();

	std::cout << "\n    starting TestHarness dispatch thread";
	dspat = std::thread([&]() { dispatchMessages(); });
	dspat.detach();

}

void TestHarness::stop()
{
	Message stop;
	stop.command("TestHarnessStop");
	stop.to(from_);  // send to self
	stop.from(from_);
	comm_.postMessage(stop);
}

void TestHarness::wait()
{
	if (recvr.joinable())
		recvr.join();
	if (dspat.joinable())
		dspat.join();
}

void TestHarness::recvMessages()
{
	while (true)
	{
		Message msg = comm_.getMessage();
		std::cout << "\n    TestHnRcv received msg: " + msg.command();
		if (msg.command() == "ready")
		{
			std::cout << "\n    TestHnRcv posting msg: " + msg.command();
			readyQ_.enQ(msg);
		}
		else if (msg.command() == "testReq")
		{
			std::cout << "\n    TestHnRcv posting msg: " + msg.command();
			requestQ_.enQ(msg);
		}
		else if (msg.command() == "sendingFile")
		{
			std::cout << "\nSendingFile\n";
		}
		else if (msg.command() == "STOP")
		{
			std::cout << "\nStopping\n";
			for(int i=0;i<numberOfChild;i++)
				requestQ_.enQ(msg);
			while (true)
			{
				if (readyQ_.size() == 0)
					exit(0);
			}
		}
		else
		{
			std::cout << "\n Invalid command for ." << msg.name();
		}
	}
}

void TestHarness::dispatchMessages()
{
	int count = 0;
	while (true)
	{
		count++;
		Message trMsg = requestQ_.deQ();
		std::cout << "\n    TestHnDsp deQ msg: " + trMsg.command();

		Message rdyMsg = readyQ_.deQ();
		std::cout << "\n    TestHnDsp deQ msg: " + rdyMsg.command();

		trMsg.to(rdyMsg.from());  // send request to ready child
		trMsg.attribute("req_id", std::to_string(count));
		std::cout << "\n    TestHnDsp sending msg: " + trMsg.command();
		comm_.postMessage(trMsg);
	}
}

int main(int argc, char* argv[])
{
	MsgPassingCommunication::EndPoint ep1("localhost", 8081);  // client
	MsgPassingCommunication::EndPoint ep2("localhost", 8082);

	const int noOfChildProcesses = 4;
	TestHarness harness(ep2,ep1,noOfChildProcesses);
	harness.start();

	std::string app = "../Debug/OODProject3.TestHarnessCore.exe";
	
	
	//Creation of child processess,

	Process child[noOfChildProcesses];
	for (int i = 0; i < noOfChildProcesses; i++)
	{
		child[i].title("ChildProcess"+std::to_string(i));
		child[i].application(app);
		child[i].commandLine(std::to_string(8085+i)+" 8082");
		child[i].create();
		CBP callback = [&]() { child[i].create(); };
		child[i].setCallBackProcessing(callback);
		child[i].registerCallback();
	}
	harness.wait();
	getchar();

}