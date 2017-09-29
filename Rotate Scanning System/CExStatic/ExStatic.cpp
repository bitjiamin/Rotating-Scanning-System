// ExStatic.cpp : implementation file
//

#include "stdafx.h"
#include "ExStatic.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CExStatic

CExStatic::CExStatic()
{
	m_crText = RGB(0,0,0);
	m_crBackGnd = RGB(255,255,255);
}

CExStatic::~CExStatic()
{
	//delete brush
	if (m_brBackGnd.GetSafeHandle())
       m_brBackGnd.DeleteObject();
}


BEGIN_MESSAGE_MAP(CExStatic, CStatic)
	//{{AFX_MSG_MAP(CExStatic)
	ON_WM_CTLCOLOR_REFLECT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CExStatic message handlers

HBRUSH CExStatic::CtlColor(CDC* pDC, UINT nCtlColor) 
{
	// TODO: Return a non-NULL brush if the parent's handler should not be called
	
	pDC->SetBkMode(TRANSPARENT);
	//set text color
	pDC->SetTextColor(m_crText);
	//set the text's background color
	pDC->SetBkColor(m_crBackGnd);

	//return the brush used for background this sets control background
	return m_brBackGnd;
}

void CExStatic::SetBackColor(COLORREF rgb)
{
	//set background color ref (used for text's background)
	m_crBackGnd = rgb;
	
	//free brush
	if (m_brBackGnd.GetSafeHandle())
       m_brBackGnd.DeleteObject();
	//set brush to new color
	m_brBackGnd.CreateSolidBrush(rgb);
	
	//redraw
	Invalidate(TRUE);
}


void CExStatic::SetTextColor(COLORREF rgb)
{
	//set text color ref
	m_crText = rgb;

	//redraw
	Invalidate(TRUE);
}