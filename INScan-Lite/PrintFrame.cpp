// PrintFrame.cpp : implementation file
//

#include "stdafx.h"

#include "PrintFrame.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPrintFrame

IMPLEMENT_DYNCREATE(CPrintFrame, CFrameWnd)

CPrintFrame::CPrintFrame()
{
	m_pCallerDlg = NULL;
	m_pWnd		= AfxGetApp()->m_pMainWnd;
	AfxGetApp()->m_pMainWnd = this;
}

CPrintFrame::~CPrintFrame()
{
}


BEGIN_MESSAGE_MAP(CPrintFrame, CFrameWnd)
	ON_WM_DESTROY()
	ON_COMMAND(ID_FILE_PRINT,Print)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPrintFrame message handlers

BOOL CPrintFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext) 
{

	m_pView = new CPrintView();
	//创建大的窗口
	m_pView->Create(NULL,NULL,WS_CHILD|WS_VISIBLE,CRect(200,500,1000,1300),this,AFX_IDW_PANE_FIRST,pContext);

	return TRUE;
}


void CPrintFrame::Print() 
{
	if(m_pView!=NULL)
	{
		m_pView->m_bPrint=TRUE;
		m_pView->OnMyPrint();		
	}
}
void CPrintFrame::OnDestroy() 
{
	if(m_pView!=NULL)
	{
		m_pView->DestroyWindow();
		m_pView=NULL;
	}
	CFrameWnd::OnDestroy();
	AfxGetApp()->m_pMainWnd = m_pWnd;
}
