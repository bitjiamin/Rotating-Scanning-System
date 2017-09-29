#include "stdafx.h"
#include "Serial.h"
#include<string>


Serial::Serial()
{
	
}


Serial::~Serial()
{
}

Serial com;
bool Serial::open_serial()
{
	//打开串口
	hCom = CreateFile(_T("COM5"),//端口号  
	
	GENERIC_READ | GENERIC_WRITE, //允许读和写  
	0, //独占方式  
	NULL,
	OPEN_EXISTING, //打开而不是创建  
	0, //同步方式  
	NULL);
	if (hCom == (HANDLE)-1)
	{
		//MessageBox(NULL,_T("打开串口失败!"),_T("Warning"),MB_OK);
		return FALSE;
	}
	return TRUE;
}

void Serial::setup_serial()
{

	//设置串口
	SetupComm(hCom, 254, 254); //输入缓冲区和输出缓冲区的大小都是254

	COMMTIMEOUTS TimeOuts; 
	//设定读超时 
	TimeOuts.ReadIntervalTimeout = 0;
	TimeOuts.ReadTotalTimeoutMultiplier = 0;
	TimeOuts.ReadTotalTimeoutConstant = 50;
	//设定写超时 
	TimeOuts.WriteTotalTimeoutMultiplier = 0;
	TimeOuts.WriteTotalTimeoutConstant = 50;
	SetCommTimeouts(hCom, &TimeOuts);

	DCB dcb;
	GetCommState(hCom, &dcb);
	dcb.BaudRate = 115200; //波特率为115200
	dcb.ByteSize = 8; //每个字节有8位 
	dcb.Parity = NOPARITY; //无奇偶校验位 
	dcb.StopBits = ONESTOPBIT; //1个停止位 
	SetCommState(hCom, &dcb);
	PurgeComm(hCom, PURGE_TXCLEAR | PURGE_RXCLEAR); //在读写串口之前，还要用PurgeComm()函数清空缓冲区
}

CString Serial::read_serial()
{
	//同步读串口 
	PurgeComm(hCom, PURGE_TXCLEAR | PURGE_RXCLEAR);
	char str[80] = {0};
	DWORD wCount;//读取的字节数 
	BOOL bReadStat; 
	bReadStat = ReadFile(hCom, str, 80, &wCount, NULL);
	CString s_data;
	s_data = str;
	if(!bReadStat) 
	{ 
		return _T(""); 
	} 
	return s_data;
}

bool Serial::write_serial(CString data)
{
	//同步写串口 
	CString add(_T("\r\n"));
	CString result;
	result =data+add;
	CStringA a_data;
	a_data = result;
	char InData[50];
	DWORD dwBytesWrite = a_data.GetLength();

	COMSTAT ComStat; 
	DWORD dwErrorFlags; 
	BOOL bWriteStat; 
	ClearCommError(hCom,&dwErrorFlags,&ComStat); 

	memset(InData, 0x00, 50);
	memcpy(InData, a_data, a_data.GetLength());

	bWriteStat=WriteFile(hCom, InData,dwBytesWrite,& dwBytesWrite,NULL);
	if(!bWriteStat) 
	{ 
		return false;
	}
	return true;
}

void Serial::close_serial()
{
	//关闭串口
	CloseHandle(hCom);
}

CString Serial::ReadAngleWave()
{
	//读取触发角度信息

	write_serial(_T("12GetAngleWave"));                        //发送命令给FPGA模块，请求触发角度信息

	//同步读串口 
	PurgeComm(hCom, PURGE_TXCLEAR | PURGE_RXCLEAR);
	char str[4096] = { 0 };                                    //读取4096个字符
	DWORD wCount;                                              //读取的字节数 
	BOOL bReadStat;
	Sleep(500);
	bReadStat = ReadFile(hCom, str, 4096, &wCount, NULL);
	CString s_data1;
	s_data1 = str;
	if (!bReadStat)
	{
		return _T("");
	}
	//读完后清除触发角度信息
	write_serial(_T("14ClearAngleWave"));
	Sleep(200);
	CString s_data2;
	s_data2 = read_serial();
	CString s_data;
	s_data = s_data1 + s_data2;
	return s_data;
}

CString Serial::ReadTimeWave()
{
	//读取触发时间信息
	write_serial(_T("11GetTimeWave"));                       //发送命令给FPGA模块，请求触发时间信息

	//同步读串口 
	PurgeComm(hCom, PURGE_TXCLEAR | PURGE_RXCLEAR);
	char str[4096] = { 0 };                                  //读取4096个字符
	DWORD wCount;                                            //读取的字节数 
	BOOL bReadStat;
	Sleep(500);
	bReadStat = ReadFile(hCom, str, 4096, &wCount, NULL);
	CString s_data1;
	s_data1 = str;
	if (!bReadStat)
	{
		return _T("");
	}
	//读完后清除触发时间信息
	write_serial(_T("13ClearTimeWave"));
	Sleep(200);
	CString s_data2;
	s_data2 = read_serial();
	CString s_data;
	s_data = s_data1 + s_data2;
	return s_data;
}

void Serial::WriteCSV(CString str)
{
	//写csv文件
	//按时间命名文件
	COleDateTime time;
	time = COleDateTime::GetCurrentTime();
	CString curTime = time.Format(_T("%Y-%m-%d:%H:%M:%S"));
	CStdioFile file;

	TCHAR buf[200];
	GetCurrentDirectory(sizeof(buf), buf);
	CString path = buf;
	path = path;

	CString filePath = path + _T("\\TriggerInfo.csv");
	str.Replace('*', ',');
	str = curTime + _T(",") + str;

	CFileException fileException;
	if (CFileFind().FindFile(filePath))
	{
		file.Open(filePath, CFile::typeText | CFile::modeReadWrite), &fileException;
	}
	else
	{
		file.Open(filePath, CFile::typeText | CFile::modeCreate | CFile::modeReadWrite), &fileException;
	}

	file.SeekToEnd();
	file.WriteString(str);
	file.WriteString(_T("\n"));
}

void Serial::ReadCSV(CListBox* pListBox)
{
	//读csv文件
	pListBox->ResetContent();
	CStdioFile file;
	TCHAR buf[200];
	GetCurrentDirectory(sizeof(buf), buf);
	CString path = buf;
	path = path;

	CString filePath = path + _T("\\TriggerInfo.csv");
	CString str = _T("0");
	if (file.Open(filePath, CFile::modeRead))
	{

		while (str != _T(""))
		{
			file.ReadString(str);
			str.Replace(_T(","), _T("   "));
			pListBox->AddString(str);
		}
		file.Close();
	}
}




 


