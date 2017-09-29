#pragma once
#include "afxwin.h"
#include "CExStatic\ExStatic.h"
#include "Rotate Scanning System.h"
//#include "C:\Users\jiamin\Documents\project\人体安检仪\Rotate Scanning System\Rotate Scanning System\Rotate Scanning System\CExStatic\ExStatic.h"


// CLoginDlg 对话框

class CLoginDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CLoginDlg)

public:
	CLoginDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CLoginDlg();
	int wm;
	CString user;
	CFont l_font;

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_LOGIN };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	void ReSize(void);
	POINT old;

	afx_msg void OnBnClickedOk();
	afx_msg void OnCbnSelchangeWorkmode();
	CComboBox m_workmode;
	CExStatic log_bar;
	CComboBox combo_user;
	CEdit e_password;
	CExStatic main_bar;
	afx_msg void OnSize(UINT nType, int cx, int cy);
};
