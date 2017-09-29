
// Rotate Scanning SystemDlg.h : 头文件
//

#pragma once
#include "LoginDlg.h"
#include "afxwin.h"
#include "CExStatic\ExStatic.h"
#include "CommSetDlg.h"
#include "TCPClient.h"
#include "MotionSetDlg.h"
#include "Setting.h"
#include "UDPSocket.h"
#include <Winsock2.h>

#define POINT_COUNT 100
#define WM_DISPLAY_CHANGE (WM_USER + 100)

// CRotateScanningSystemDlg 对话框
class CRotateScanningSystemDlg : public CDialogEx
{
// 构造
public:
	CRotateScanningSystemDlg(CWnd* pParent = NULL);	// 标准构造函数
	CLoginDlg m_loginDlg;
	CComboBox m_workmode;
	float m_nzValues[POINT_COUNT];
	void DrawWave(CDC *pDC, CRect &rectPicture);
	CMenu *pMenu;
	CFont m_font;


// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ROTATESCANNINGSYSTEM_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持
	HANDLE hThread;
	DWORD ThreadID;


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	afx_msg  LRESULT  OnDisplayChange(WPARAM wParam, LPARAM lParam);

public:
	void ReSize(void);
	POINT old;

	afx_msg void OnBnClickedOk();
	//CStatic m_picDraw;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	CExStatic stc_title;
	CExStatic main_bar;
	CEdit e_user;
	afx_msg void OnCommSet();
	afx_msg void OnMotionSet();
	afx_msg void OnParaSet();
	afx_msg void OnClose();
	
    static void ReadAllParas();
	CComboBox c_workmode;
	afx_msg void OnBnClickedReset();
	afx_msg void OnBnClickedStop();
	CEdit e_errorcode;
	CString path;
	CEdit motor_pos;
	CEdit encoder_pos;
	CString s_encoder_pos;

	afx_msg void OnBnClickedStart();
	afx_msg void OnBnClickedExit();
	CExStatic plc_state;
	CExStatic d_state;
	CExStatic fpga_state;
	afx_msg void OnBnClickedLoop();
	CButton loop_check;
	afx_msg void OnBnClickedTrig();
	CButton trig_out;
	//static UINT SaveWave(LPVOID pParam);
	static UINT SaveWave();

	int pos;
	int pos_before;
	long secstop;
	long secstart;
	bool running;
	//volatile bool save_ok;
	CEdit scan_speed;
	
	afx_msg void OnBnClickedButton3();

	afx_msg void OnStnClickedWaveDraw();
	afx_msg void OnBnClickedSave();
	CButton save_check;
	CEdit system_info;
	afx_msg void OnSize(UINT nType, int cx, int cy);
};
extern int d_results[25];
extern CString s_results[25];
