#pragma once
#include "afxwin.h"
#include "CExStatic\ExStatic.h"


// MotionSetDlg dialog

class MotionSetDlg : public CDialogEx
{
	DECLARE_DYNAMIC(MotionSetDlg)

public:
	MotionSetDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~MotionSetDlg();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

// Dialog Data
	enum { IDD = IDD_Motion };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	void ReSize(void);
	POINT old;

	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedRun();	
	afx_msg void OnBnClickedZerof();
	afx_msg void OnBnClickedZerob();
	afx_msg void OnBnClickedLimitb();
	CExStatic s_title;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedRunf();
	afx_msg void OnBnClickedLimitf();
	CComboBox runmode;
	CFont l_font;
	afx_msg void OnBnClickedRunb();
	int MotionSetDlg::CstringToInt(CString);
	afx_msg void OnSize(UINT nType, int cx, int cy);
};

