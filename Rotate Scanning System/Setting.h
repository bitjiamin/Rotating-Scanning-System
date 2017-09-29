#pragma once
#include "afxwin.h"
#include "CExStatic\ExStatic.h"
#include "Rotate Scanning System.h"
#include "TCPClient.h"
#include "Serial.h"
//#include "UDPSocket.h"
#include "Rotate Scanning SystemDlg.h"

// CSetting dialog

class CSetting : public CDialogEx
{
	DECLARE_DYNAMIC(CSetting)

public:
	CSetting(CWnd* pParent = NULL);   // standard constructor
	virtual BOOL OnInitDialog();
	virtual ~CSetting();
	

// Dialog Data
	enum { IDD = IDD_Setting };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	HANDLE hThread;
	DWORD ThreadID;

	DECLARE_MESSAGE_MAP()
public:
	void ReSize(void);
	POINT old;

	CExStatic s_title;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedZerov();
	static void ReadAllParas();
	int CstringToInt(CString);
	CEdit r_edit1;
	CEdit r_edit2;
	CEdit r_edit3;
	CEdit r_edit4;
	CEdit r_edit5;
	CEdit r_edit6;
	CEdit r_edit7;
	CEdit r_edit8;
	CEdit r_edit9;
	CEdit r_edit10;
	CEdit r_edit11;
	CEdit r_edit12;
	CEdit w_edit1;
	CEdit w_edit2;
	CEdit w_edit3;
	CEdit w_edit4;
	CEdit w_edit5;
	CEdit w_edit6;
	CEdit w_edit7;
	CEdit w_edit8;
	CEdit w_edit9;
	CEdit w_edit10;
	CEdit w_edit11;
	CEdit w_edit12;
	afx_msg void OnBnClickedCrawlv();
	afx_msg void OnBnClickedAt();
	afx_msg void OnBnClickedDt();
	afx_msg void OnBnClickedZerop();
	afx_msg void OnBnClickedMax();
	afx_msg void OnBnClickedMannual();
	afx_msg void OnBnClickedIntert();
	afx_msg void OnBnClickedInterv();
	afx_msg void OnBnClickedIntera();
	afx_msg void OnBnClickedInterd();
	afx_msg void OnBnClickedContiv();
	CFont l_font;
	double test;
	CEdit w_edit14;
	afx_msg void OnBnClickedCont();
	CEdit r_edit14;
	CEdit w_edit13;
	CEdit r_edit13;
	afx_msg void OnBnClickedTimes();
	CEdit r_edit0;
	afx_msg void OnBnClickedTriger();
	CEdit w_edit0;
	afx_msg void OnSize(UINT nType, int cx, int cy);
};
