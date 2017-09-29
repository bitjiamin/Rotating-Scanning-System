#pragma once
#include "TCPClient.h"
#include "afxwin.h"
#include "Rotate Scanning SystemDlg.h"
#include "CExStatic\ExStatic.h"
#include "Serial.h"

// CommSetDlg dialog

class CommSetDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CommSetDlg)

public:
	CommSetDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CommSetDlg();

// Dialog Data
	enum { IDD = IDD_COMM };

protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CExStatic s_commbar;
	CExStatic s_commtitle;
	afx_msg void OnBnClickedOpen();
	afx_msg void OnBnClickedCancel();
};
