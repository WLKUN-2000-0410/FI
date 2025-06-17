// DlgLogin.cpp : 实现文件
//

#include "stdafx.h"
#include "INScan-Lite.h"
#include "DlgWait.h"
#include "afxdialogex.h"


// CDlgLogin 对话框

IMPLEMENT_DYNAMIC(CDlgWait, CDialog)

CDlgWait::CDlgWait(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgWait::IDD, pParent)
{

}

CDlgWait::~CDlgWait()
{
}

void CDlgWait::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_PIC, m_StaticP);
}


BEGIN_MESSAGE_MAP(CDlgWait, CDialog)
	ON_WM_PAINT()
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CDlgLogin 消息处理程序


BOOL CDlgWait::OnInitDialog()
{
	CDialog::OnInitDialog();


	CRect rect(0,0,1280,750);
	::SetWindowPos(this->m_hWnd,HWND_TOP,rect.left,rect.top,rect.Width(),rect.Height(),SWP_NOZORDER|SWP_NOMOVE);

	::SetWindowLong(GetSafeHwnd(),GWL_EXSTYLE,::GetWindowLongPtr(GetSafeHwnd(),GWL_EXSTYLE)|WS_EX_LAYERED);
	this->SetLayeredWindowAttributes(clr,150,LWA_ALPHA|LWA_COLORKEY);

	//CRect rect;
 
	CString m_strAviPath;
	m_strAviPath = sInfo.Path + _T("\\res\\wait.gif");
	if (m_StaticP.Load(m_strAviPath))
	{
		m_StaticP.Draw();
		m_StaticP.SetBkColor(RGB(0,0,0));
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDlgWait::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	CRect rect;
	GetClientRect(rect);
	dc.FillSolidRect(rect,RGB(0,0,0));
	/*CBitmap bmp;  
	if (bmp.LoadBitmap(IDB_BITMAP9))  
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
	} */
}


void CDlgWait::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

}


BOOL CDlgWait::DestroyWindow()
{
	m_StaticP.DestroyWindow();

	return CDialog::DestroyWindow();
}
