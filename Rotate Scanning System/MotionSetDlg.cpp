// MotionSetDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Rotate Scanning System.h"
#include "MotionSetDlg.h"
#include "afxdialogex.h"
#include "TCPClient.h"


// MotionSetDlg dialog

IMPLEMENT_DYNAMIC(MotionSetDlg, CDialogEx)

MotionSetDlg::MotionSetDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(MotionSetDlg::IDD, pParent)
{
	
}

MotionSetDlg::~MotionSetDlg()
{
	
}

BOOL MotionSetDlg::OnInitDialog()
{
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
	MoveWindow((int)cx / 6, (int)cy / 6, (int)cx * 2 / 3, (int)cy * 2 / 3);

	// TODO:  Add extra initialization here
	l_font.CreatePointFont(150, _T("宋体"));
	//s_title.SetBackColor(RGB(200, 0, 0));
	s_title.SetBackColor(RGB(255, 255, 255));//
	s_title.SetFont(&l_font);
	runmode.SetCurSel(0);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void MotionSetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_Title, s_title);
	DDX_Control(pDX, IDC_RunMode, runmode);
}


BEGIN_MESSAGE_MAP(MotionSetDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &MotionSetDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_Run, &MotionSetDlg::OnBnClickedRun)
	ON_BN_CLICKED(IDC_Zerof, &MotionSetDlg::OnBnClickedZerof)
	ON_BN_CLICKED(IDC_Zerob, &MotionSetDlg::OnBnClickedZerob)
	ON_BN_CLICKED(IDC_Limitb, &MotionSetDlg::OnBnClickedLimitb)
	ON_BN_CLICKED(IDC_RunF, &MotionSetDlg::OnBnClickedRunf)
	ON_BN_CLICKED(IDC_Limitf, &MotionSetDlg::OnBnClickedLimitf)
	ON_BN_CLICKED(IDC_RunB, &MotionSetDlg::OnBnClickedRunb)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// MotionSetDlg message handlers


void MotionSetDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CDialogEx::OnOK();
}


void MotionSetDlg::OnBnClickedRun()
{
	//绝对运动和相对运动
	int data;
	CString s_data;
	GetDlgItem(IDC_MoveP)->GetWindowTextW(s_data);
	data = CstringToInt(s_data);

	if (runmode.GetCurSel() == 0)
	{
		tcp.WriteD(238,data);
		tcp.WriteM(109, true);
	}
	else
	{
		tcp.WriteD(236, data);
		tcp.WriteM(108, true);
	}
	
}

BOOL MotionSetDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_LBUTTONDOWN)
	{
		if (pMsg->hwnd == GetDlgItem(IDC_RunF)->m_hWnd)
		{
			//按钮按下
			tcp.WriteM(112, true);
		}
		if (pMsg->hwnd == GetDlgItem(IDC_RunB)->m_hWnd)
		{
			//按钮按下
			tcp.WriteM(113, true);
		}

	}

	else if (pMsg->message == WM_LBUTTONUP)
	{
		if (pMsg->hwnd == GetDlgItem(IDC_RunF)->m_hWnd)
		{
			tcp.WriteM(112, false);
			//tcp.TCP_Close();
		}
		if (pMsg->hwnd == GetDlgItem(IDC_RunB)->m_hWnd)
		{
			tcp.WriteM(113, false);
			//tcp.TCP_Close();
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}


void MotionSetDlg::OnBnClickedZerof()
{
	//正向回原点
	tcp.WriteM(117, true);
}


void MotionSetDlg::OnBnClickedZerob()
{
	//负向回原点
	tcp.WriteM(118, true);

}


void MotionSetDlg::OnBnClickedLimitb()
{
	//到负向极限位
	tcp.WriteM(120, true);
}





void MotionSetDlg::OnBnClickedRunf()
{
	// TODO: Add your control notification handler code here
}


void MotionSetDlg::OnBnClickedLimitf()
{
	//到正向极限位
	tcp.WriteM(119, true);
}


void MotionSetDlg::OnBnClickedRunb()
{
	// TODO: Add your control notification handler code here
}

int MotionSetDlg::CstringToInt(CString s_data)
{
	//CString转int
	int data;
	int a = s_data.Find('.');
	if (s_data.Find('.') == -1)
	{
		s_data = s_data + _T(".00");
	}
	else
	{
		//只有一位小数
		if (s_data.GetLength() - s_data.Find('.') < 3)
		{
			s_data = s_data + _T("0");
		}
		//超过两位小数取前两位
		if (s_data.GetLength() - s_data.Find('.') > 3)
		{
			s_data = s_data.Left(s_data.Find('.') + 3);
		}
	}
	s_data.Remove('.');
	data = _ttoi(s_data);
	return data;
}



void MotionSetDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	CDialogEx::OnSize(nType, cx, cy);
	ReSize();
}

void MotionSetDlg::ReSize(void)
{
	float fsp[2];
	POINT Newp; //获取现在对话框的大小  
	CRect recta;
	GetClientRect(&recta);  //取客户区大小    
	Newp.x = recta.right - recta.left;
	Newp.y = recta.bottom - recta.top;
	fsp[0] = (float)Newp.x / old.x;
	fsp[1] = (float)Newp.y / old.y;
	CRect Rect;
	int woc, mainid;
	CPoint OldTLPoint, TLPoint; //左上角  
	CPoint OldBRPoint, BRPoint; //右下角  
	HWND  hwndChild = ::GetWindow(m_hWnd, GW_CHILD);  //列出所有控件

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
		Rect.SetRect(TLPoint, BRPoint);
		GetDlgItem(woc)->MoveWindow(Rect, TRUE);
		hwndChild = ::GetWindow(hwndChild, GW_HWNDNEXT);
	}
	old = Newp;
}