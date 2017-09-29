#if !defined(AFX_EXSTATIC_H__8CF68C75_9EAE_4836_A638_FC34C03C100B__INCLUDED_)
#define AFX_EXSTATIC_H__8CF68C75_9EAE_4836_A638_FC34C03C100B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ExStatic.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CExStatic window

class CExStatic : public CStatic
{
// Construction
public:
	CExStatic();

// Attributes
public:
	//text and text background colors
	COLORREF m_crText;
	COLORREF m_crBackGnd;
	//background brush
	CBrush m_brBackGnd;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CExStatic)
	//}}AFX_VIRTUAL

// Implementation
public:

	void SetTextColor(COLORREF rgb);
	void SetBackColor(COLORREF rgb);
	virtual ~CExStatic();

	// Generated message map functions
protected:
	//{{AFX_MSG(CExStatic)
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EXSTATIC_H__8CF68C75_9EAE_4836_A638_FC34C03C100B__INCLUDED_)
