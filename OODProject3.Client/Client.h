/////////////////////////////////////////////////////////////////////
// Client.h - Header file for Client class						   //
// ver 1.0                                                         //
//Author:   Shrenik Daga, CSE687 - Object Oriented Design, Fall 18 //
//Source:	Dr Jim Fawcett, Syracuse University                    //
/////////////////////////////////////////////////////////////////////


#include "../CppCommWithFileXfer/MsgPassingComm/Comm.h"
#include "../CppCommWithFileXfer/Message/Message.h"
#include "../Process/Process.h"


using namespace MsgPassingCommunication;
class Client
{
	
public:
	Client(EndPoint from, EndPoint to);
	~Client();
	void start();
	void wait();
	void sendMessages();
	void getLogs();
private:
	std::thread recv, logs;
	int testCounter=0;
	Comm comm_;
	EndPoint to_;
	EndPoint from_;
};