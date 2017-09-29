// LoginDlg.cpp : 实现文件
//

#include "stdafx.h"

#include "LoginDlg.h"
#include "afxdialogex.h"


// CLoginDlg 对话框

BOOL CLoginDlg::OnInitDialog()
{
	//初始化对话框
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
	MoveWindow((int)cx / 3, (int)cy / 3, (int)cx / 3, (int)cy / 3);

	m_workmode.SetCurSel(0);
	//设置字体，图标颜色等
	l_font.CreatePointFont(150, _T("宋体"));
	log_bar.SetBackColor(RGB(200,0,0));
	main_bar.SetBackColor(RGB(255, 255, 255));//
	main_bar.SetFont(&l_font);
	combo_user.SetCurSel(0);
	e_password.SetPasswordChar('*');
	e_password.SetFocus();
	return true;

}

IMPLEMENT_DYNAMIC(CLoginDlg, CDialogEx)

CLoginDlg::CLoginDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_LOGIN, pParent)
{

}

CLoginDlg::~CLoginDlg()
{
}


void CLoginDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_WorkMode, m_workmode);
	DDX_Control(pDX, IDC_STATIC_Bar, log_bar);

	DDX_Control(pDX, IDC_COMBO_User, combo_user);
	DDX_Control(pDX, IDC_PassWord, e_password);
	DDX_Control(pDX, IDC_STATIC_Main, main_bar);
}


BEGIN_MESSAGE_MAP(CLoginDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CLoginDlg::OnBnClickedOk)
	ON_CBN_SELCHANGE(IDC_WorkMode, &CLoginDlg::OnCbnSelchangeWorkmode)
	ON_WM_SIZE()
END_MESSAGE_MAP()


void CLoginDlg::OnBnClickedOk()
{
	//登陆
	CString pwd;
	int index;
	index = combo_user.GetCurSel();
	GetDlgItem(IDC_PassWord)->GetWindowTextW(pwd);
	if (index == 0 && pwd != _T("1"))
	{
		MessageBox(_T("密码错误！"), _T("警告"));
	}

	else
	{
		wm = m_workmode.GetCurSel();
		combo_user.GetLBText(combo_user.GetCurSel(),user);
		OnOK();
	}
}


void CLoginDlg::OnCbnSelchangeWorkmode()
{

	// TODO: 在此添加控件通知处理程序代码
}

void CLoginDlg::ReSize(void)
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

void CLoginDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	CDialogEx::OnSize(nType, cx, cy);
	ReSize();
}
