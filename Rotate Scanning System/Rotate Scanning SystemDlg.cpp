
// Rotate Scanning SystemDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Rotate Scanning System.h"
#include "Rotate Scanning SystemDlg.h"
#include "afxdialogex.h"
#include "TCPClient.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框
volatile bool save_ok;

CString s_results[25];
int a, b;
int d_results[25];
int motor_stop;


class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CRotateScanningSystemDlg 对话框



CRotateScanningSystemDlg::CRotateScanningSystemDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_ROTATESCANNINGSYSTEM_DIALOG, pParent)
	, s_encoder_pos(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	memset(m_nzValues, 0, sizeof(int) * POINT_COUNT);
}

void CRotateScanningSystemDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//DDX_Control(pDX, IDC_WAVE_DRAW, m_picDraw);
	DDX_Control(pDX, IDC_STATIC1, stc_title);
	DDX_Control(pDX, IDC_STATIC_Mainbar, main_bar);
	DDX_Control(pDX, IDC_EDIT_User, e_user);
	DDX_Control(pDX, IDC_STATIC_State, plc_state);
	DDX_Control(pDX, IDC_COMBO_Wm, c_workmode);
	DDX_Control(pDX, IDC_EDIT_Recv1, e_errorcode);
	DDX_Control(pDX, IDC_EDIT_MPos, motor_pos);
	DDX_Control(pDX, IDC_EDIT_EPos, encoder_pos);
	DDX_Text(pDX, IDC_EDIT_EPos, s_encoder_pos);
	DDX_Control(pDX, IDC_STATIC_State2, d_state);
	DDX_Control(pDX, IDC_STATIC_State3, fpga_state);
	DDX_Control(pDX, IDC_Loop, loop_check);
	DDX_Control(pDX, IDC_Trig, trig_out);
	DDX_Control(pDX, IDC_EDIT_Speed, scan_speed);
	DDX_Control(pDX, IDC_Save, save_check);
	DDX_Control(pDX, IDC_Info, system_info);
}

BEGIN_MESSAGE_MAP(CRotateScanningSystemDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CRotateScanningSystemDlg::OnBnClickedOk)
	ON_WM_TIMER()
	ON_COMMAND(ID_32772, &CRotateScanningSystemDlg::OnCommSet)
	ON_COMMAND(ID_32774, &CRotateScanningSystemDlg::OnMotionSet)
	ON_COMMAND(ID_32773, &CRotateScanningSystemDlg::OnParaSet)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_Reset, &CRotateScanningSystemDlg::OnBnClickedReset)
	ON_BN_CLICKED(IDC_Stop, &CRotateScanningSystemDlg::OnBnClickedStop)

	ON_MESSAGE(WM_DISPLAY_CHANGE, OnDisplayChange)

	ON_BN_CLICKED(IDC_Start, &CRotateScanningSystemDlg::OnBnClickedStart)
	ON_BN_CLICKED(IDC_Exit, &CRotateScanningSystemDlg::OnBnClickedExit)
	ON_BN_CLICKED(IDC_Loop, &CRotateScanningSystemDlg::OnBnClickedLoop)
	ON_BN_CLICKED(IDC_Trig, &CRotateScanningSystemDlg::OnBnClickedTrig)
	ON_BN_CLICKED(IDC_BUTTON3, &CRotateScanningSystemDlg::OnBnClickedButton3)
	//ON_STN_CLICKED(IDC_WAVE_DRAW, &CRotateScanningSystemDlg::OnStnClickedWaveDraw)
	ON_BN_CLICKED(IDC_Save, &CRotateScanningSystemDlg::OnBnClickedSave)
	ON_WM_SIZE()
END_MESSAGE_MAP()

// CRotateScanningSystemDlg 消息处理程序

BOOL CRotateScanningSystemDlg::OnInitDialog()
{
	//初始化
	CDialogEx::OnInitDialog();

	CRect rect;
	GetClientRect(&rect);     //取客户区大小    
	old.x = rect.right - rect.left;
	old.y = rect.bottom - rect.top;
	int cx = GetSystemMetrics(SM_CXFULLSCREEN);
	int cy = GetSystemMetrics(SM_CYFULLSCREEN);
	CRect rt;
	SystemParametersInfo(SPI_GETWORKAREA, 0, &rt, 0);
	cy = rt.bottom;
	MoveWindow((int)cx / 8, (int)cy / 8, (int)cx * 3 / 4, (int)cy * 3 / 4);

	if (m_loginDlg.DoModal() != IDOK)
	{
		OnOK();
	}
	//工作模式和用户名从登陆界面传到主界面
	CString workmode,username;
	c_workmode.SetCurSel(m_loginDlg.wm);
	username = m_loginDlg.user;
	e_user.SetWindowTextW(username);
	// 将“关于...”菜单项添加到系统菜单中。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}
	

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动执行此操作

	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	//设置菜单
	pMenu = new CMenu();
	pMenu->LoadMenu(IDR_MENU1);
	SetMenu(pMenu);
	//设置字体
	m_font.CreatePointFont(250, _T("黑体"));
	stc_title.SetFont(&m_font);
	stc_title.SetBackColor(RGB(255, 255, 255));
	main_bar.SetBackColor(RGB(200,0,0));
	plc_state.SetBackColor(RGB(0, 220, 0));
	plc_state.SetWindowTextW(_T("PLC OK"));
	d_state.SetBackColor(RGB(0, 220, 0));
	d_state.SetWindowTextW(_T("Motor OK"));

	GetDlgItem(IDC_STC_Ver)->SetWindowTextW(_T("2.1.0"));

	//获取当前路径
	TCHAR buf[1024];
	GetCurrentDirectory(sizeof(buf), buf);
	path = buf;
	path = path + _T("\\Config");

	//定义变量
	int pos = 0;            //当前位置
	int pos_before = 0;     //上次读取的位置
	long secstop = 0;       //停止时间
	long secstart = 0;      //开始时间
	bool running = false;   //运行状态
	save_ok = false;

	//判断FPGA模块通信是否正常
	if (fpga.UDPConnect())
	{
		fpga_state.SetBackColor(RGB(0, 200, 0));
		fpga_state.SetWindowTextW(_T("FPGA OK"));
	}
	else
	{
		fpga_state.SetBackColor(RGB(200, 0, 0));
		fpga_state.SetWindowTextW(_T("FPGA Error"));
		MessageBox(_T("FPGA模块连接失败，请检查后重新运行程序！"), _T("警告"));
		PostQuitMessage(0);
		return false;
	}

	//判断PLC通信是否正常
	if (tcp.TCP_Connect())
	{
		plc_state.SetBackColor(RGB(0, 200, 0));
		plc_state.SetWindowTextW(_T("PLC OK"));
	}
	else
	{
		MessageBox(_T("PLC连接失败，请检查网络后重新运行程序！"), _T("警告"));
		plc_state.SetBackColor(RGB(200, 0, 0));
		plc_state.SetWindowTextW(_T("PLC Error"));
		PostQuitMessage(0);
		return false;
	}

	// 启动定时器，ID为1，定时时间为200ms，以该间隔循环读取PLC参数  
	SetTimer(1, 50, NULL);

	hThread = CreateThread(NULL,
	0,
	(LPTHREAD_START_ROUTINE)ReadAllParas,
	NULL,
	0,
	&ThreadID);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CRotateScanningSystemDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

HCURSOR CRotateScanningSystemDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CRotateScanningSystemDlg::OnBnClickedOk()

{
		
}

void CRotateScanningSystemDlg::OnTimer(UINT_PTR nIDEvent)
{
	//读取编码器位置
	double epos = fpga.GetCurrentPos();
	//if (epos > 300)
		//epos = epos - 360;
	epos = 203.515 - epos;

	CString s_epos;
	s_epos.Format(_T("%3f"), epos);
	encoder_pos.SetWindowTextW(s_epos);
	//解析出循环测试当前次数
	CString looptime = s_results[23];
	if (loop_check.GetCheck() == true)
		system_info.SetWindowTextW(looptime);
	//解析连续运动速度
	scan_speed.SetWindowTextW(tcp.AddPoint(s_results[15]));
	////读取运动方向
	//int a = tcp.ReadM(121) / 16;
	//int b = tcp.ReadM(123) / 16;
	/*if (a == 1)
	{
		GetDlgItem(IDC_EDIT_Dir)->SetWindowTextW(_T("正向运动"));
	}
	else if (b == 1)
	{
		GetDlgItem(IDC_EDIT_Dir)->SetWindowTextW(_T("反向运动"));
	}
	else
	{
		GetDlgItem(IDC_EDIT_Dir)->SetWindowTextW(_T("电机停止"));
	}*/



	if (motor_stop == 16)
	{
		GetDlgItem(IDC_Start)->EnableWindow(true);
		GetDlgItem(IDC_Reset)->EnableWindow(true);
		GetDlgItem(IDC_EDIT_Dir)->SetWindowTextW(_T("电机停止"));
	}
	else
	{
		//GetDlgItem(IDC_Start)->EnableWindow(false);
		//GetDlgItem(IDC_Reset)->EnableWindow(false);
		GetDlgItem(IDC_EDIT_Dir)->SetWindowTextW(_T("电机运行"));
		//显示测试时间
		CString testtime;
		testtime.Format(_T("%d"), d_results[24]);
		GetDlgItem(IDC_EDIT_Ttime)->SetWindowTextW(testtime+_T("ms"));
	}

	//显示速度
	CString testv;
	testv.Format(_T("%d"), d_results[16] / 100);
	GetDlgItem(IDC_EDIT_Speed)->SetWindowTextW(testv);

	CRect rectPicture;

	//将数组中的所有元素前移一个单位，第一个元素丢弃   
	for (int i = 0; i < POINT_COUNT - 1; i++)
	{
		m_nzValues[i] = m_nzValues[i + 1];
	}
	//解析当前位置
	CString s_pos;
	pos = d_results[10];
	if (pos < 0)
	{
		pos = 0;
	}

	CTime time = CTime::GetCurrentTime();	
	CString m_strTime = time.Format("%Y-%m-%d %H:%M:%S");
	GetDlgItem(IDC_STATIC_Time)->SetWindowTextW(m_strTime);

	//计算每次扫描的时间并显示
	secstop = GetTickCount();

	if (pos == pos_before && running == true && motor_stop == 16)
	{
		//本次扫描结束，计算扫描时间，读取触发角度,触发时间等信息
		//CString scan_time;
		//scan_time.Format(_T("%d"),secstop - secstart);
		//GetDlgItem(IDC_EDIT_Ttime)->SetWindowTextW(s_results[23]+_T("ms"));
		if (loop_check.GetCheck() == false)
			//running = false;

		//读取FPGA触发角度及时间信息并保存到csv文件中
		if (loop_check.GetCheck() == false && save_check.GetCheck() == true)
		{
			Sleep(100);
			SaveWave();
			running = false;
		}
	}

	pos_before = pos;
	s_pos.Format(_T("%d"), pos);
	s_pos = tcp.AddPoint(s_pos); 
	float f_pos = (float)pos / 100.0;

	motor_pos.SetWindowTextW(s_pos);

	//解析错误代码
	int err = d_results[20];
	CString s_err;
	while (err != 0)
	{
		if (err % 2 == 0)
			s_err = _T("0") + s_err;
		else
			s_err = _T("1") + s_err;
		err = err / 2;
	}
	CString e_value, e_code, e_info, e_infoall;
	for (int i = 0; i < s_err.GetLength(); i++)
	{
		e_value = s_err.GetAt(s_err.GetLength() - i - 1);
		e_code.Format(_T("%d"), (i + 1));
		if (e_value == _T("1"))
		{
			//从配置文件中读取错误代码对应的错误信息
			GetPrivateProfileString(_T("errorcode"), e_code, _T("0"), e_info.GetBuffer(50), 50, path + _T("\\config.ini"));
			e_info.ReleaseBuffer();
			e_infoall = e_infoall + e_info + _T("\r\n");
		}
	}
	
	//判断是否显示剩余次数
	/*if (loop_check.GetCheck() == true)
	{
		e_infoall = _T("当前循环次数：") + looptime + _T("\r\n") + e_infoall;
		if (looptime == _T("9"))
		{
			running = false;
		}
	}*/
	//显示错误信息
	e_errorcode.SetWindowTextW(e_infoall);
	CDialogEx::OnTimer(nIDEvent);
}

UINT CRotateScanningSystemDlg::SaveWave()
{
	//读取FPGA触发角度及时间信息并保存到csv文件中
	bool ret1, ret2;
	ret1 = fpga.GetAngleArr();
	Sleep(100);
	ret2 = fpga.GetTimeArr();
	ret1 = fpga.GetAngleArr();
	ret2 = fpga.GetTimeArr();
	save_ok = true;
	return 0;
}

//刷新界面
LRESULT CRotateScanningSystemDlg::OnDisplayChange(WPARAM wParam, LPARAM lParam)
{
	//更新Edit  
	CString str;
	str = (LPCTSTR)lParam;
	encoder_pos.SetWindowTextW(str.Left(25).Right(7));
	return 0;
}


void CRotateScanningSystemDlg::ReadAllParas()
{
	while (true)
	{
		//读取PLC参数	
		static CString str;

		//实时读取PLC信息显示（包括错误代码）
		int* d_result;
		
		d_result = tcp.ReadAllPara(200);     //读取PLC参数

		for (int i = 0; i < 25; i++)
		{
			d_results[i] = d_result[i];
			s_results[i].Format(_T("%d"), d_results[i]);
		}
		//读取运动方向
		int a = tcp.ReadM(121)/16;
		int b = tcp.ReadM(123)/16;
		motor_stop = tcp.ReadM(122);

		Sleep(50);
	}
}

void CRotateScanningSystemDlg::OnCommSet()
{
	//打开触发信息显示界面
	CommSetDlg comm;
	comm.DoModal();
}


void CRotateScanningSystemDlg::OnMotionSet()
{
	//打开运动控制界面
	MotionSetDlg motion;
	motion.DoModal();
}

void CRotateScanningSystemDlg::OnParaSet()
{
	// 打开参数设置界面
	CSetting para;
	para.DoModal();
}


void CRotateScanningSystemDlg::OnClose()
{
	// 关闭程序时运行的代码
	//com.write_serial(_T("11StopTrigger"));   //停止触发
	fpga.DisableTrigger();
	//tcp.WriteM(124, 0);                      //关闭老化测试
	tcp.TCP_Close();                         //关闭TCP连接
	//com.close_serial();                      //关闭串口连接
	CDialogEx::OnClose();                    //关闭窗口

}

void CRotateScanningSystemDlg::OnBnClickedReset()
{
	// 复位转臂
	tcp.WriteM(100, 1);
}


void CRotateScanningSystemDlg::OnBnClickedStop()
{
	//停止旋转
	tcp.WriteM(104, 1);
}


void CRotateScanningSystemDlg::OnBnClickedStart()
{
	//开始扫描
	//com.write_serial(_T("14ClearAngleWave"));      //清除FPGA模块触发角度信息
	//Sleep(200);
	//com.write_serial(_T("13ClearTimeWave"));       //清除FPGA模块触发时间信息
	//Sleep(200);
	bool ret1, ret2;
	ret1 = fpga.DisableTrigger();
	Sleep(100);
	ret2 = fpga.EnableTrigger();
	if (ret1&&ret2)
		system_info.SetWindowTextW(_T("开启触发成功！"));
	else
		system_info.SetWindowTextW(_T("开启触发失败！"));
	secstart = GetTickCount();                     //获取开始时间
	
	if (c_workmode.GetCurSel() == 0)               //连续模式
	{
		tcp.WriteM(116, 1);
	}
	else                                           //走停模式
	{
		tcp.WriteM(115, 1);
	}
	Sleep(100);
	running = true;
}


void CRotateScanningSystemDlg::OnBnClickedExit()
{
	//退出程序
	OnClose();
}


void CRotateScanningSystemDlg::OnBnClickedLoop()
{
	//使能老化测试选项，此时再按开始按钮，将开始老化测试
	if (loop_check.GetCheck() == true)
	{
		tcp.WriteM(124, 1);
	}
	else
	{
		tcp.WriteM(124, 0);
	}
}


void CRotateScanningSystemDlg::OnBnClickedTrig()
{
	bool ret;
	if (trig_out.GetCheck() == true)                       //打开触发信号
	{		
		ret = fpga.EnableTrigger();
		if (ret)
			system_info.SetWindowTextW(_T("开启触发成功"));
		else
			system_info.SetWindowTextW(_T("开启触发失败"));
	}
	else                                                   //关闭触发信号
	{		
		ret = fpga.DisableTrigger();
		if (ret)
			system_info.SetWindowTextW(_T("关闭触发成功"));
		else
			system_info.SetWindowTextW(_T("关闭触发失败"));
	}
}


void CRotateScanningSystemDlg::OnBnClickedButton3()
{
	// TODO: Add your control notification handler code here

	double encoder_pos;
	encoder_pos = fpga.GetCurrentPos();
}


void CRotateScanningSystemDlg::OnStnClickedWaveDraw()
{
	// TODO: Add your control notification handler code here
}


void CRotateScanningSystemDlg::OnBnClickedSave()
{
	// TODO: Add your control notification handler code here
}


void CRotateScanningSystemDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	CDialogEx::OnSize(nType, cx, cy);
	//ReSize();
}

void CRotateScanningSystemDlg::ReSize(void)
{
	float fsp[2];
	POINT Newp; //获取现在对话框的大小  
	CRect recta;
	GetClientRect(&recta);     //取客户区大小    
	Newp.x = recta.right - recta.left;
	Newp.y = recta.bottom - recta.top;
	fsp[0] = (float)Newp.x / old.x;
	fsp[1] = (float)Newp.y / old.y;
	CRect Rect;
	int woc, mainid;
	CPoint OldTLPoint, TLPoint; //左上角  
	CPoint OldBRPoint, BRPoint; //右下角  
	HWND  hwndChild = ::GetWindow(m_hWnd, GW_CHILD);  //列出所有控件 
	int id;
	id = GetDlgItem(IDC_EDIT_User)->GetDlgCtrlID();

	while (hwndChild)
	{
		woc = ::GetDlgCtrlID(hwndChild);//取得ID
		GetDlgItem(woc)->GetWindowRect(Rect);
		ScreenToClient(Rect);
		OldTLPoint = Rect.TopLeft();
		TLPoint.x = long(OldTLPoint.x*fsp[0]);
		TLPoint.y = long(OldTLPoint.y*fsp[1]);
		OldBRPoint = Rect.BottomRight();
		BRPoint.x = long(OldBRPoint.x *fsp[0]);
		BRPoint.y = long(OldBRPoint.y *fsp[1]);
		//用户名y方向不扩张
		//if (woc == id)
			//BRPoint.y = long(TLPoint.y + (OldBRPoint.y - OldTLPoint.y));
		Rect.SetRect(TLPoint, BRPoint);
		GetDlgItem(woc)->MoveWindow(Rect, TRUE);
		hwndChild = ::GetWindow(hwndChild, GW_HWNDNEXT);
	}
	old = Newp;
}