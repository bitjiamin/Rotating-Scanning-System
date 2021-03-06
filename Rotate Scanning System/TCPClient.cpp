#include "stdafx.h"
#include "TCPClient.h"



#pragma comment(lib, "ws2_32.lib")

TCPClient::TCPClient()
{
	
}
CString ip = _T("192.168.1.250");       //PLC IP地址
int port = 10001;                       //PLC端口
TCPClient tcp;
bool reading = false;
bool writing = false;

TCPClient::~TCPClient()
{
}

bool TCPClient::TCP_Connect()
{
	//连接PLC
	WSADATA wsaData;
	const char* ip_addr;
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

	sockClient = socket(AF_INET, SOCK_STREAM, 0);
	if (SOCKET_ERROR == sockClient)
	{
		printf("Socket() error:%d", WSAGetLastError());
		return false;
	}
	if (connect(sockClient, (struct  sockaddr*)&addrSrv, sizeof(addrSrv)) == INVALID_SOCKET)
	{
		printf("Connect failed:%d", WSAGetLastError());
		return false;
	}
	return true;
}

void TCPClient::TCP_Close()
{
	//关闭TCP连接
	closesocket(sockClient);
}


byte* TCPClient::TCP_Send(char s_buff[])
{	
	//TCP发送数据，并接收返回信息
	CCriticalSection global_CriticalSection;
	//global_CriticalSection.Lock();
	int timeout = 1000;
	setsockopt(sockClient, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(timeout));   //设置接收超时
	setsockopt(sockClient, SOL_SOCKET, SO_SNDTIMEO, (char*)&timeout, sizeof(timeout));   //设置发送超时
	char buff[1024];
	send(sockClient, s_buff, 100, 0);
	Sleep(20);
	
	recv(sockClient, buff, sizeof(buff), 0);
	byte result[1024];
	for (int i = 0; i < 1024; i++)
	{
		result[i] = buff[i];
	}
	//global_CriticalSection.Unlock();
	return result;
}

int TCPClient::ReadM(short m_number)  //每次读取两位，即两个寄存器值
{
	//读取PLC M寄存器，输入参数为寄存器编号	
	while (writing)
	{
		Sleep(10);
	}
	reading = true;
	char s_buf[128];
	s_buf[0] = 0x00;
	s_buf[1] = 0xFF;
	s_buf[2] = 0x0A;
	s_buf[3] = 0x00;
	s_buf[8] = 0x20;
	s_buf[9] = 0x4D;
	s_buf[10] = 0x02;
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
	s_buf[4] = data[0];
	s_buf[5] = data[1];
	s_buf[6] = data[2];
	s_buf[7] = data[3];

	byte* result;
	result = TCP_Send(s_buf);

	//将数据高位，低位分别转成十进制再相加
	int d_result[15];
	for (int i = 0; i < 10; i++)
	{
		d_result[i] = (int)result[i];
	}
	reading = false;
	return d_result[2];
}

bool TCPClient::WriteM(short m_number, bool state)
{
	//写入PLC M寄存器，输入为寄存器编号和需要写入的寄存器状态
	while (reading)
	{
		Sleep(10);
	}
	writing = true;
	char s_buf[128];
	s_buf[0] = 0x02;
	s_buf[1] = 0xFF;
	s_buf[2] = 0x0A;
	s_buf[3] = 0x00;
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
	byte* result;
	result = TCP_Send(s_buf);
	writing = false;
	if (result[0] == 0x82 && result[1] == 0x00)
		return true;
	else
		return false;
}

int TCPClient::ReadD(short m_number)
{
	//读取PLC D寄存器状态
	while (writing)
	{
		Sleep(10);
	}
	reading = true;
	char s_buf[128];
	s_buf[0] = 0x01;
	s_buf[1] = 0xFF;
	s_buf[2] = 0x0A;
	s_buf[3] = 0x00;
	s_buf[5] = 0x00;
	s_buf[6] = 0x00;
	s_buf[7] = 0x00;
	s_buf[8] = 0x20;
	s_buf[9] = 0x44;
	s_buf[10] = 0x02;
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
	s_buf[4] = data[0];
	s_buf[5] = data[1];
	s_buf[6] = data[2];
	s_buf[7] = data[3];

	byte* result;
	result = TCP_Send(s_buf);

	//将数据高位，低位分别转成十进制再相加
	int d_byte;
	int d_result = 0;
	for (int i = 0; i < 4; i++)
	{
		d_byte = (int)result[i + 2];
		d_byte = d_byte << (8 * i);
		d_result = d_result + d_byte;
	}
	reading = false;
	return d_result;
}


bool TCPClient::WriteD(short m_number, int d_data)
{
	//写入PLC D寄存器状态
	while (reading)
	{
		Sleep(10);
	}
	writing = true;
	char s_buf[128];
	s_buf[0] = 0x03;
	s_buf[1] = 0xFF;
	s_buf[2] = 0x0A;
	s_buf[3] = 0x00;
	s_buf[8] = 0x20;
	s_buf[9] = 0x44;
	s_buf[10] = 0x02;
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
	byte* result;
	result = TCP_Send(s_buf);
	writing = false;
	if (result[0] == 0x83 && result[1] == 0x00)
		return true;
	else
		return false;
}

int* TCPClient::ReadAllPara(short m_number)
{
	//从D200开始读取50个D寄存器
	while (writing)
	{
		Sleep(10);
	}
	reading = true;
	char s_buf[128];
	s_buf[0] = 0x01;
	s_buf[1] = 0xFF;
	s_buf[2] = 0x0A;
	s_buf[3] = 0x00;
	s_buf[5] = 0x00;
	s_buf[6] = 0x00;
	s_buf[7] = 0x00;
	s_buf[8] = 0x20;
	s_buf[9] = 0x44;
	s_buf[10] = 0x32;
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
	s_buf[4] = data[0];
	s_buf[5] = data[1];
	s_buf[6] = data[2];
	s_buf[7] = data[3];
	byte* result;
	result = TCP_Send(s_buf);
	//读取50个D寄存器，共25个参数
	static int d_results[25];
	for (int j = 0; j < 25; j++)
	{
		int d_byte;
		int d_result = 0;
		for (int i = 0; i < 4; i++)
		{
			d_byte = (int)result[4*j + i + 2];
			d_byte = d_byte << (8 * i);
			d_result = d_result + d_byte;
		}
		d_results[j] = d_result;
	}
	reading = false;
	return d_results;
}

CString TCPClient::AddPoint(CString strin)
{
	CString strout;
	if (strin.GetLength() > 1)
	{
		strout = strin.Left(strin.GetLength() - 2) + _T(".") + strin.Right(2);
	}
	else
	{
		strout = strin;
	}
	return strout;
}








