#include "stdafx.h"
#include "UDPSocket.h"


#pragma comment(lib, "ws2_32.lib")

UDPSocket::UDPSocket()
{
	UINT m_uLocalPort = 8081;
	fpgaudp.Create(m_uLocalPort, SOCK_DGRAM);
}
UDPSocket fpga;
char head_b[8] = {0x4D,0x65,0x67,0x61,0x53,0x69,0x67,0x2E};
CString otherIP = _T("192.168.1.10");
UINT otherPORT = 8080;
CString localIP = _T("192.168.1.100");
UINT localPORT = 8081;
CString s_anglearr,s_timearr;

UDPSocket::~UDPSocket()
{

}

struct fd_set fds;

struct timeval timeout = { 0.5, 0 }; //select等待3秒，3秒轮询，要非阻塞就置0

bool UDPSocket::UDPConnect()
{
	UINT m_uLocalPort = 8081;
	bool ret = fpgaudp.Create(m_uLocalPort, SOCK_DGRAM);
	return ret;
}

byte* UDPSocket::UDPSendRecv(char s_buff[],int cnt)
{	
	FD_ZERO(&fds); //每次循环都要清空集合，否则不能检测描述符变化 

	FD_SET(fpgaudp, &fds); //添加描述符 	

	fpgaudp.SendTo(s_buff, 20, otherPORT, otherIP);
	Sleep(50);
	int ret = select(fpgaudp + 1, &fds, NULL, NULL, &timeout);

	byte szBuf[1024] = { 0 };		//定义接收数据的缓存区

	//fpgaudp.ReceiveFrom(szBuf, cnt, otherIP, otherPORT);
	if (ret!=0)
		fpgaudp.ReceiveFrom(szBuf, cnt, otherIP, otherPORT);
	
	byte result[1024];
	CString str,tem;
	for (int i = 0; i < cnt; i++)
	{
		result[i] = szBuf[i];
		tem.Format(_T("%02x"), szBuf[i]);
		str += tem + _T(" ");
	}
	//CFPGACommDlg *pDlg = (CFPGACommDlg *)AfxGetMainWnd();
	//pDlg->udp_result.SetWindowTextW(str);
	return result;
}

int UDPSocket::bytesToInt(byte* bytes, int size = 4)
{
	int addr = bytes[0] & 0xFF;
	addr |= ((bytes[1] << 8) & 0xFF00);
	addr |= ((bytes[2] << 16) & 0xFF0000);
	addr |= ((bytes[3] << 24) & 0xFF000000);
	return addr;
}

void UDPSocket::intToByte(int i, byte *bytes, int size = 4)
{
	memset(bytes, 0, sizeof(byte) *  size);
	bytes[0] = (byte)(0xff & i);
	bytes[1] = (byte)((0xff00 & i) >> 8);
	bytes[2] = (byte)((0xff0000 & i) >> 16);
	bytes[3] = (byte)((0xff000000 & i) >> 24);
	return;
}

//获取编码器当前角度值
double UDPSocket::GetCurrentPos()
{
	//4D65 6761 5369 672E 0102 0001 0000 0000 0000 0000
	char s_buf[20];	
	char data_b[12] = { 0x01, 0x02, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

	for (int i = 0; i < 20; i++)
	{
		if (i<8)
			s_buf[i] = head_b[i];
		else
			s_buf[i] = data_b[i-8];
	}
	byte *result;
	result = UDPSendRecv(s_buf,20);
	byte step[4];
	step[0] = result[19];
	step[1] = result[18];
	step[2] = result[17];
	step[3] = result[16];
	int s_angle;
	double angle = bytesToInt(step, 4)/pow(2,25)*360; 
	return angle;
}

//开启触发功能
bool UDPSocket::EnableTrigger()
{
	//4D65 6761 5369 672E 0101 0035 0001 0000 0000 0000
	char s_buf[20];
	char data_b[12] = { 0x01, 0x01, 0x00, 0x35, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

	for (int i = 0; i < 20; i++)
	{
		if (i<8)
			s_buf[i] = head_b[i];
		else
			s_buf[i] = data_b[i - 8];
	}
	byte *result;
	result = UDPSendRecv(s_buf,20);
	bool ret = false;
	if (result[10] == 0x00 && result[11] == 0x35 && result[12] == 0x00 && result[13] == 0x01)
		ret = true;
	return ret;
}

//关闭触发功能
bool UDPSocket::DisableTrigger()
{
	//4D65 6761 5369 672E 0101 0035 0000 0000 0000 0000
	char s_buf[20];
	char data_b[12] = { 0x01, 0x01, 0x00, 0x35, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

	for (int i = 0; i < 20; i++)
	{
		if (i<8)
			s_buf[i] = head_b[i];
		else
			s_buf[i] = data_b[i - 8];
	}
	byte *result;
	result = UDPSendRecv(s_buf,20);
	bool ret = false;
	if (result[10] == 0x00 && result[11] == 0x35 && result[12] == 0x00 && result[13] == 0x00)
		ret = true;
	return ret;
}

//开始标定模式
bool UDPSocket::StartCalibration()
{
	//4D65 6761 5369 672E 0101 0035 0000 0000 0000 0000
	char s_buf1[20];
	char s_buf2[20];
	char data_b1[12] = { 0x01, 0x01, 0x00, 0x3C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
	char data_b2[12] = { 0x01, 0x01, 0x00, 0x3C, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

	for (int i = 0; i < 20; i++)
	{
		if (i<8)
			s_buf1[i] = head_b[i];
		else
			s_buf1[i] = data_b1[i - 8];
	}

	for (int i = 0; i < 20; i++)
	{
		if (i<8)
			s_buf2[i] = head_b[i];
		else
			s_buf2[i] = data_b2[i - 8];
	}
	byte *result;
	result = UDPSendRecv(s_buf1,20);
	bool ret = false;
	if (result[10] == 0x00 && result[11] == 0x3C && result[12] == 0x00 && result[13] == 0x00)
		ret = true;
	else
		ret = false;
	result = UDPSendRecv(s_buf2,20);
	if (result[10] == 0x00 && result[11] == 0x3C && result[12] == 0x00 && result[13] == 0x01)
		ret = true;
	else
		ret = false;
	return ret;
}

//设置标定时输出的脉冲数
bool UDPSocket::SetCalPulse(int cnt)
{
	//4D65 6761 5369 672E 0101 003D 0000 0000 0000 0000
	char s_buf[20];
	byte data[4];
	if (cnt < 1||cnt>100)
		cnt = 10;
	intToByte(cnt-1, data, 4);
	char data_b[12] = { 0x01, 0x01, 0x00, 0x3D, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
	data_b[4] = data[1];
	data_b[5] = data[0];
	for (int i = 0; i < 20; i++)
	{
		if (i<8)
			s_buf[i] = head_b[i];
		else
			s_buf[i] = data_b[i - 8];
	}
	byte *result;
	result = UDPSendRecv(s_buf, 20);

	bool ret = false;
	if (result[8] == 0x01 && result[9] == 0x11 && result[10] == 0x00 && result[11] == 0x3D)
		ret = true;
	return ret;
}

//设置校准时的延时，即到校准位置后多久后发出脉冲，time为延时值（单位ms）
bool UDPSocket::SetCalDelay(int time)
{	
	//4D65 6761 5369 672E 0101 003E 0000 0000 0000 0000
	char s_buf[20];
	byte data[4];
	if (time < 1 || time>1000)
		time = 100;
	intToByte(time - 1, data, 4);
	char data_b[12] = { 0x01, 0x01, 0x00, 0x3E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
	data_b[4] = data[1];
	data_b[5] = data[0];
	for (int i = 0; i < 20; i++)
	{
		if (i<8)
			s_buf[i] = head_b[i];
		else
			s_buf[i] = data_b[i - 8];
	}
	byte *result;
	result = UDPSendRecv(s_buf, 20);

	bool ret = false;
	if (result[8] == 0x01 && result[9] == 0x11 && result[10] == 0x00 && result[11] == 0x3E)
		ret = true;
	return ret;
}

//设置触发角度间隔
bool UDPSocket::SetTriggerAngle(double angle)
{
	//4D65 6761 5369 672E 0101 0030 7482 0000 0000 0000
	char s_buf[20];
	int step = (int)(pow(2, 25) / 360 * angle);
	byte data[4];
	intToByte(step, data, 4);
	char data_b[12] = { 0x01, 0x01, 0x00, 0x30, 0x74, 0x82, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
	data_b[4] = data[1];
	data_b[5] = data[0];
	for (int i = 0; i < 20; i++)
	{
		if (i<8)
			s_buf[i] = head_b[i];
		else
			s_buf[i] = data_b[i - 8];
	}
	byte *result;
	result = UDPSendRecv(s_buf,20);

	bool ret = false;
	if (result[8] == 0x01 && result[9] == 0x11 && result[10] == 0x00 && result[11] == 0x30)
		ret = true;
	return ret;
}

//获取触发角度值信息并保存到csv文件中
bool UDPSocket::GetAngleArr()
{
	//4D65 6761 5369 672E 0103 0001 0000 00C8 0000 0000
	s_anglearr = _T("");
	char s_buf[20];
	char data_b[12] = { 0x01, 0x03, 0x00, 0x01, 0x00, 0x00, 0x00, 0xC8, 0x00, 0x00, 0x00, 0x00 };

	for (int i = 0; i < 20; i++)
	{
		if (i<8)
			s_buf[i] = head_b[i];
		else
			s_buf[i] = data_b[i - 8];
	}
	byte *result;
	byte x[1024];
	result = UDPSendRecv(s_buf,820);
	int i_step[200];
	byte b_step[4];
	double angle[200];
	CString s_angle;

	for (int i = 0; i < 200; i++)
	{
		b_step[0] = result[4 * i + 23];
		b_step[1] = result[4 * i + 22];
		b_step[2] = result[4 * i + 21];
		b_step[3] = result[4 * i + 20];
		i_step[i] = bytesToInt(b_step, 4);
		angle[i] = i_step[i] / pow(2, 25) * 360;
		x[i] = result[i];
	}
	bool ret = false;
	if (x[8] == 0x01 && x[9] == 0x13 && x[10] == 0x00 && x[11] == 0x01)
		ret = true;

	for (int i = 0; i < 200; i++)
	{
		s_angle.Format(_T("%f"), angle[i]);
		if (angle[i]>0)
			s_anglearr = s_anglearr + s_angle + _T(",");
	}
	WriteCSV(s_anglearr);
	return ret;
}

//获取触发时间值信息并保存到csv文件中
bool UDPSocket::GetTimeArr()
{
	//4D65 6761 5369 672E 0103 0000 0000 00C8 0000 0000
	s_timearr = _T("");
	char s_buf[20];
	char data_b[12] = { 0x01, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC8, 0x00, 0x00, 0x00, 0x00 };

	for (int i = 0; i < 20; i++)
	{
		if (i<8)
			s_buf[i] = head_b[i];
		else
			s_buf[i] = data_b[i - 8];
	}
	byte *result;
	result = UDPSendRecv(s_buf,820);
	int i_time[200];
	byte b_time[4];
	double time[200];
	byte x[1024];
	CString s_time;
	for (int i = 0; i < 200; i++)
	{
		b_time[0] = result[4 * i + 23];
		b_time[1] = result[4 * i + 22];
		b_time[2] = result[4 * i + 21];
		b_time[3] = result[4 * i + 20];
		i_time[i] = bytesToInt(b_time, 4);
		time[i] = (double)i_time[i]/1000000;
		x[i] = result[i];
	}
	bool ret = false;
	if (x[8] == 0x01 && x[9] == 0x13 && x[10] == 0x00 && x[11] == 0x00)
		ret = true;

	for (int i = 0; i < 200; i++)
	{
		s_time.Format(_T("%f"), time[i]);
		if (time[i]>0)
			s_timearr = s_timearr + s_time + _T(",");
	}
	WriteCSV(s_timearr);
	return ret;
}


void UDPSocket::WriteCSV(CString str)
{
	//写csv文件
	//按时间命名文件
	COleDateTime time;
	time = COleDateTime::GetCurrentTime();
	CString curTime = time.Format(_T("%Y-%m-%d:%H:%M:%S"));
	CStdioFile file;

	TCHAR buf[200];
	//GetCurrentDirectory(sizeof(buf), buf);
	GetModuleFileName(NULL, buf, sizeof(buf));
	CString path = buf;
	path = path.Left(path.ReverseFind('\\'));
	CString filePath = path + _T("\\TriggerInfo.csv");
	//str.Replace('*', ',');
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

void UDPSocket::ReadCSV(CListBox* pListBox)
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








