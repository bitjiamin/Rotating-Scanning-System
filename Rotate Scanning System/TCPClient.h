#include <WinSock2.h>  
#include <stdio.h>
#include <stdlib.h>
#include <iostream> 
#include <string> 
#include <sstream> 
using namespace std;

#pragma once
class TCPClient
{
public:
	TCPClient();
	~TCPClient();
	SOCKET sockClient;
	bool TCP_Connect();
	byte* TCP_Send(char[]);
	void TCP_Close();
	bool WriteM(short, bool);
	int ReadD(short);
	bool WriteD(short, int);
	int* ReadAllPara(short);
	int ReadM(short);
	CString AddPoint(CString);

};
extern CString ip;
extern int port;
extern TCPClient tcp;
