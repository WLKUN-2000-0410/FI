// DlgLogin.cpp : 实现文件
//

#include "stdafx.h"
#include "INScan-Lite.h"
#include "DlgLogin.h"
#include "afxdialogex.h"


// CDlgLogin 对话框

IMPLEMENT_DYNAMIC(CDlgLogin, CDialog)

CDlgLogin::CDlgLogin(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgLogin::IDD, pParent)
{

}

CDlgLogin::~CDlgLogin()
{
}

void CDlgLogin::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROGRESS, m_ProgressCtrl);
	DDX_Control(pDX, IDC_STATIC_PIC, m_StaticP);
}


BEGIN_MESSAGE_MAP(CDlgLogin, CDialog)
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CDlgLogin 消息处理程序


BOOL CDlgLogin::OnInitDialog()
{
	CDialog::OnInitDialog();

	/*CRect rect(0,0,300,400);
	::SetWindowPos(this->m_hWnd,HWND_TOP,0,0,rect.Width(),rect.Height(),SWP_NOZORDER|SWP_NOMOVE);*/

	m_ProgressCtrl.SetRange(0,100);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDlgLogin::SetPro()
{
	//临时
	m_ProgressCtrl.SetPos(sInfo.statusBar);
	if(sInfo.statusBar==100)
	{
		CDialog::OnClose();
		CDialog::DestroyWindow();
	}
}
void CDlgLogin::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	CBitmap bmp;  
#ifdef Release_LO
	if (bmp.LoadBitmap(IDB_BITMAP_LD))
#else
	if (bmp.LoadBitmap(IDB_BITMAP7))  
#endif
	{  
		BITMAP bmpInfo;  
		bmp.GetBitmap(&bmpInfo);  
		CDC dcMemory;  
		CDC* pDC=GetDlgItem(IDC_STATIC_PIC)->GetDC();  
		dcMemory.CreateCompatibleDC(pDC);  
		CBitmap* pOldBitmap = dcMemory.SelectObject(&bmp);  
		CRect rect;  
		GetDlgItem(IDC_STATIC_PIC)->GetClientRect(&rect);  
	
		pDC->SetStretchBltMode(COLORONCOLOR);  
		pDC->StretchBlt(0, 0,rect.Width(),rect.Height(),&dcMemory,0, 0, bmpInfo.bmWidth, bmpInfo.bmHeight,SRCCOPY);  
		dcMemory.SelectObject(pOldBitmap);  
		ReleaseDC(pDC);  
	} 
}
