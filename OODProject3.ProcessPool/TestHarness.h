#pragma once
/////////////////////////////////////////////////////////////////////
// Source.cpp - Automated testsuite made as a DLL for OOD Project3 //
// ver 1.0                                                         //
//Author:   Shrenik Daga, CSE687 - Object Oriented Design, Fall 18 //
//Source:	Dr Jim Fawcett, Syracuse University                    //
/////////////////////////////////////////////////////////////////////

#include "../CppCommWithFileXfer\MsgPassingComm/Comm.h"
#include "../CppCommWithFileXfer/Cpp11-BlockingQueue/Cpp11-BlockingQueue.h"
#include "../CppCommWithFileXfer/Message/Message.h"

using namespace MsgPassingCommunication;
class TestHarness
{
public:
	TestHarness(EndPoint from, EndPoint to,const int count);
	~TestHarness();
	void start();
	void wait();
	void stop();
	void recvMessages();
	void dispatchMessages();
private:
	BlockingQueue<Message> readyQ_;
	BlockingQueue<Message> requestQ_;
	std::thread recvr;
	std::thread dspat;
	Comm comm_;
	EndPoint to_;
	EndPoint from_;
	int numberOfChild;
};