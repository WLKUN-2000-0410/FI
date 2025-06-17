// PrintView.cpp : implementation file
//

#include "stdafx.h"
#include "PrintView.h"
#include "PrintFrame.h"



/////////////////////////////////////////////////////////////////////////////
// CPrintView

IMPLEMENT_DYNCREATE(CPrintView, CView)

CPrintView::CPrintView()
{
	m_bPrint = TRUE;
}

CPrintView::~CPrintView()
{
}


BEGIN_MESSAGE_MAP(CPrintView, CView)
		ON_COMMAND(ID_FILE_PRINT,OnMyPrint)
		ON_WM_TIMER()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPrintView drawing

void CPrintView::OnDraw(CDC* pDC)
{

}

void CPrintView::OnMyPrint()
{
	CView::OnFilePrint();//显示打印文件窗口，AFXWIN.H中
	KillTimer(1);	
	CPrintFrame *pFrame =(CPrintFrame *)GetParent();
	pFrame->m_pCallerDlg->SendMessage(WM_MY_PRINT_S,0, 0);
	GetParent()->DestroyWindow();

}
//显示对话框
void CPrintView::OnMyPrintPreview()
{
	GetParent()->ShowWindow(SW_SHOWMAXIMIZED);
	CView::OnFilePrintPreview();
}

//准备打印
BOOL CPrintView::OnPreparePrinting(CPrintInfo* pInfo) 
{
	pInfo->SetMaxPage(1);
	pInfo->m_pPD->m_pd.Flags |= PD_HIDEPRINTTOFILE;

	return DoPreparePrinting(pInfo);//打印和大意预览开始的时返回非0;
}
//开始打印，发送开始打印消息
void CPrintView::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo) 
{
	CView::OnBeginPrinting(pDC, pInfo);
	CPrintFrame *pFrame =(CPrintFrame *)GetParent();
	pFrame->m_pCallerDlg->SendMessage(WM_BEGIN_PRINTING,(WPARAM) pDC, (LPARAM) pInfo);

}
void CPrintView::OnEndPrinting(CDC* pDC, CPrintInfo* pInfo) 
{
	CView::OnEndPrinting(pDC, pInfo);
	CPrintFrame *pFrame =(CPrintFrame *)GetParent();
	pFrame->m_pCallerDlg->SendMessage(WM_END_PRINTING,(WPARAM) pDC, (LPARAM) pInfo);
	GetParent()->ShowWindow(SW_MINIMIZE);
	m_bPrint=FALSE;
	SetTimer(1,500,NULL);
}
//打印，发送打印消息
void CPrintView::OnPrint(CDC* pDC, CPrintInfo* pInfo) 
{
	CPrintFrame *pFrame =(CPrintFrame *)GetParent();
	pFrame->SendMessageToDescendants(WM_IDLEUPDATECMDUI,(WPARAM)TRUE,0,0,TRUE);
	pFrame->m_pCallerDlg->SendMessage(WM_MY_PRINT,(WPARAM) pDC, (LPARAM) pInfo) ;

}
void CPrintView::OnEndPrintPreview(CDC* pDC, CPrintInfo* pInfo, POINT point, CPreviewView* pView) 
{
	CView::OnEndPrintPreview(pDC, pInfo, point, pView);
}

BOOL CPrintView::PreTranslateMessage(MSG* pMsg)
{
	
	return CView::PreTranslateMessage(pMsg);
}





void CPrintView::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent==1)
	{
		if(m_bPrint==FALSE)
		{
			KillTimer(1);
			m_bPrint=TRUE;
			CPrintFrame *pFrame =(CPrintFrame *)GetParent();
			pFrame->m_pCallerDlg->SendMessage(WM_MY_PRINT_S,0, 0);
			GetParent()->DestroyWindow();
			return;
		}
	}

	CView::OnTimer(nIDEvent);
}
