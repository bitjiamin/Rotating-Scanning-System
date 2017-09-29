// CommSetDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Rotate Scanning System.h"
#include "CommSetDlg.h"
#include "afxdialogex.h"


// CommSetDlg dialog

IMPLEMENT_DYNAMIC(CommSetDlg, CDialogEx)

CommSetDlg::CommSetDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CommSetDlg::IDD, pParent)
{
	
}

CommSetDlg::~CommSetDlg()
{
}



void CommSetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//DDX_Control(pDX, IDC_EDIT_Port, e_port);
	DDX_Control(pDX, IDC_CommBar, s_commbar);
	DDX_Control(pDX, IDC_Title, s_commtitle);
}


BEGIN_MESSAGE_MAP(CommSetDlg, CDialogEx)
	ON_BN_CLICKED(IDC_Open, &CommSetDlg::OnBnClickedOpen)
	ON_BN_CLICKED(IDCANCEL, &CommSetDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


BOOL CommSetDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	CFont m_font;
	s_commbar.SetBackColor(RGB(200, 0, 0));
	s_commtitle.SetBackColor(RGB(255,255,255));
	m_font.CreatePointFont(200, _T("ºÚÌå"));
	s_commtitle.SetFont(&m_font);

	CListBox* pListBox = (CListBox*)GetDlgItem(IDC_Result);
	com.ReadCSV(pListBox);

	return TRUE;
}

// CommSetDlg message handlers


void CommSetDlg::OnBnClickedOpen()
{
	// TODO: Add your control notification handler code here
	CListBox* pListBox = (CListBox*)GetDlgItem(IDC_Result);
	com.ReadCSV(pListBox);
}


void CommSetDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	CDialogEx::OnCancel();
}
