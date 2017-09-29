#include <WinSock2.h>  
#include <stdio.h>
#include <stdlib.h>
#include <iostream> 
#include <string> 
#include <sstream> 
#include <afxsock.h>
#include "resource.h"
using namespace std;

#pragma once
class UDPSocket
{
public:
	UDPSocket();
	~UDPSocket();
	SOCKET sockClient;
	bool TCP_Connect();
	byte* UDPSendRecv(char[],int);
	void TCP_Close();
	double GetCurrentPos();
	int bytesToInt(byte* bytes, int size);
	bool EnableTrigger();
	bool DisableTrigger();
	bool SetTriggerAngle(double angle);
	void intToByte(int i, byte *bytes, int size);
	bool GetAngleArr();
	bool GetTimeArr();
	bool StartCalibration();
	bool SetCalPulse(int cnt);
	bool UDPSocket::SetCalDelay(int time);
	void WriteCSV(CString str);
	void ReadCSV(CListBox* pListBox);
	bool UDPConnect();

	CSocket fpgaudp;
};
extern CString ip;
extern int port;
extern 	UDPSocket fpga;
