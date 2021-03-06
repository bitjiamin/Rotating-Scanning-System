#include "stdafx.h"
#include "TCPClient.h"



#pragma comment(lib, "ws2_32.lib")

TCPClient::TCPClient()
{
	
	//获取当前路径
	//TCHAR buf[100];
	//GetCurrentDirectory(sizeof(buf), buf);
	
}
CString ip = _T("192.168.1.250");
int port = 10001;
TCPClient tcp;

TCPClient::~TCPClient()
{
}

bool TCPClient::TCP_Connect()
{
	//¼ÓÔØÌ×½Ó×Ö  
	WSADATA wsaData;
	const char* ip_addr;
	//ip_addr = (const char *)IP.GetBuffer(sizeof(IP));
	//IP.ReleaseBuffer();

	char szStr[256] = { 0 };

	wcstombs(szStr, ip, ip.GetLength());//Unicode转换为ASCII

	ip_addr = szStr;


	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		printf("Failed to load Winsock");
		return false;
	}
	SOCKADDR_IN addrSrv;
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_port = htons(port);
	addrSrv.sin_addr.S_un.S_addr = inet_addr(ip_addr);

	//´´½¨Ì×½Ó×Ö  
	sockClient = socket(AF_INET, SOCK_STREAM, 0);
	
	

	if (SOCKET_ERROR == sockClient)
	{
		printf("Socket() error:%d", WSAGetLastError());
		return false;
	}
	//closesocket(sockClient);

	//Ïò·þÎñÆ÷·¢³öÁ¬½ÓÇëÇó 
	if (connect(sockClient, (struct  sockaddr*)&addrSrv, sizeof(addrSrv)) == INVALID_SOCKET)
	{
		printf("Connect failed:%d", WSAGetLastError());
		return false;

	}

	return true;
}



void TCPClient::TCP_Close()
{
	closesocket(sockClient);
}


int TCPClient::TCP_Send(char s_buff[])
{	
	int timeout = 2000;
	setsockopt(sockClient, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(timeout));
	setsockopt(sockClient, SOL_SOCKET, SO_SNDTIMEO, (char*)&timeout, sizeof(timeout));
	char buff[1024];
	//memset(s_buf, 0, sizeof(s_buf));	
	send(sockClient, s_buff, 100, 0);
	int i = 0;
	Sleep(0);
	recv(sockClient, buff, sizeof(buff), 0);
	CString result;
	result = buff;

	//将数据高位，低位分别转成十进制再相加
	byte high, low;
	high = buff[3];
	low = buff[2];
	int highbit;
	int lowbit;
	//需转成byte类型再进行强制转换
	highbit = (int)high;
	lowbit = (int)low;
	highbit = highbit << 8;
	int d_result = highbit + lowbit;

	return d_result;
}



void TCPClient::WriteM(short m_number, bool state)
{
	char s_buf[128];
	s_buf[0] = 0x02;
	s_buf[1] = 0xFF;
	s_buf[2] = 0x0A;
	s_buf[3] = 0x00;
	//s_buf[4] = 0x6E;
	//s_buf[5] = 0x00;
	//s_buf[6] = 0x00;
	//s_buf[7] = 0x00;
	s_buf[8] = 0x20;
	s_buf[9] = 0x4D;
	s_buf[10] = 0x01;
	s_buf[11] = 0x00;

	//将十进制数转成十六进制并按要求排列高低位
	CStringA s;
	s.Format("%2x", m_number);
	while (s.GetLength() < 8)
	{
		s = "0" + s;
	}
	CStringA str;
	byte data[4] = { 0 };
	for (int i = 0; i < 4; i++)
	{
		str = s.Left(8 - 2 * i).Right(2);
		int j;
		data[i] = strtol(str, NULL, 16);
	}

	s_buf[4] = data[0];
	s_buf[5] = data[1];
	s_buf[6] = data[2];
	s_buf[7] = data[3];

	//写入数据0或者1
	if (state == true)
	{
		s_buf[12] = 0x11;
	}
	else
	{
		s_buf[12] = 0x00;
	}

	TCP_Send(s_buf);
}

int TCPClient::ReadD(short m_number)
{
	char s_buf[128];
	s_buf[0] = 0x01;
	s_buf[1] = 0xFF;
	s_buf[2] = 0x0A;
	s_buf[3] = 0x00;
	//s_buf[4] = 0x6E;
	s_buf[5] = 0x00;
	s_buf[6] = 0x00;
	s_buf[7] = 0x00;
	s_buf[8] = 0x20;
	s_buf[9] = 0x44;
	s_buf[10] = 0x01;
	s_buf[11] = 0x00;

	CStringA s;
	s.Format("%2x", m_number);
	while (s.GetLength() < 8)
	{
		s = "0" + s;
	}
	CStringA str;
	byte data[4] = { 0 };
	for (int i = 0; i < 4; i++)
	{
		str = s.Left(8 - 2 * i).Right(2);
		int j;
		data[i] = strtol(str, NULL, 16);
	}

	//itoa(n, buf, 16);
	//char char_m;
	//char_m = (char)m_number;
	s_buf[4] = data[0];
	s_buf[5] = data[1];
	s_buf[6] = data[2];
	s_buf[7] = data[3];

	int result;
	result = TCP_Send(s_buf);
}


void TCPClient::WriteD(short m_number, int d_data)
{
	char s_buf[128];
	s_buf[0] = 0x03;
	s_buf[1] = 0xFF;
	s_buf[2] = 0x0A;
	s_buf[3] = 0x00;
	//s_buf[4] = 0x6E;
	//s_buf[5] = 0x00;
	//s_buf[6] = 0x00;
	//s_buf[7] = 0x00;
	s_buf[8] = 0x20;
	s_buf[9] = 0x44;
	s_buf[10] = 0x02;
	s_buf[11] = 0x00;
	//s_buf[12] = 0x23;
	//s_buf[13] = 0x45;


	CStringA s;
	s.Format("%2x", m_number);
	while (s.GetLength() < 8)
	{
		s = "0" + s;
	}
	CStringA str;
	byte data[4] = { 0 };
	for (int i = 0; i < 4; i++)
	{
		str = s.Left(8 - 2 * i).Right(2);
		int j;
		data[i] = strtol(str, NULL, 16);
	}

	s_buf[4] = data[0];
	s_buf[5] = data[1];
	s_buf[6] = data[2];
	s_buf[7] = data[3];


	s.Format("%2x", d_data);
	while (s.GetLength() < 8)
	{
		s = "0" + s;
	}
	CStringA str1;
	byte data1[4] = { 0 };
	for (int i = 0; i < 4; i++)
	{
		str1 = s.Left(8 - 2 * i).Right(2);
		int j;
		data1[i] = strtol(str1, NULL, 16);
	}

	s_buf[12] = data1[0];
	s_buf[13] = data1[1];
	s_buf[14] = data1[2];
	s_buf[15] = data1[3];

	TCP_Send(s_buf);
}








