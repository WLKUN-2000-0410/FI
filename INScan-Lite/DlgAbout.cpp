// DlgAbout.cpp : 实现文件
//

#include "stdafx.h"
#include "INScan-Lite.h"
#include "INScan-LiteDlg.h"
#include "DlgAbout.h"
#include "afxdialogex.h"


// CDlgAbout 对话框

IMPLEMENT_DYNAMIC(CDlgAbout, CDialog)

CDlgAbout::CDlgAbout(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgAbout::IDD, pParent)
{

}

CDlgAbout::~CDlgAbout()
{
}

void CDlgAbout::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgAbout, CDialog)
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CDlgAbout 消息处理程序


void CDlgAbout::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	CRect rect;
	GetClientRect(rect);
	#ifdef Release_LO
		//COLORREF clr=RGB(211,211,211); 
	#endif
	dc.FillSolidRect(rect,clr);
	CDialog::OnPaint();
}


HBRUSH CDlgAbout::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	if (nCtlColor==CTLCOLOR_STATIC)
	{
		pDC->SetBkColor(clr);
		HBRUSH b =CreateSolidBrush(clr);
		return b;
	}
	return hbr;
}


BOOL CDlgAbout::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_FontChinese.CreatePointFont(120,strFontChinese);
	m_FontEnglish.CreatePointFont(120,strFontEngish);


	SetControlFont();

#ifdef Release_LO
	//GetDlgItem(IDC_STATIC_ABOUT)->ShowWindow(FALSE);
	GetDlgItem(IDC_STATIC_NAME)->ShowWindow(FALSE);
	GetDlgItem(IDC_STATIC_ADDR)->ShowWindow(FALSE);
	GetDlgItem(IDC_STATIC_TEL)->ShowWindow(FALSE);
	GetDlgItem(IDC_STATIC_FOX)->ShowWindow(FALSE);
	GetDlgItem(IDC_STATIC_1)->ShowWindow(FALSE);
	GetDlgItem(IDC_STATIC_MAIL)->ShowWindow(FALSE);
	GetDlgItem(IDC_STATIC_WEB)->ShowWindow(FALSE);
#endif
	
	//  lcz new 23/05/17
	GetClientRect(&m_rect); //获取对话框的大小
	ScreenToClient(m_rect);
	i_dlg_width = m_rect.Width();
	i_dlg_height = m_rect.Height();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDlgAbout::SetControlFont()
{
	CFont *m_Font;

	if (sInfo.nLanguage==1)
		m_Font=&m_FontEnglish;
	else
		m_Font=&m_FontChinese;
	

	GetDlgItem(IDC_STATIC_ABOUT)->SetFont(m_Font);
	GetDlgItem(IDC_STATIC_NAME)->SetFont(m_Font);
	GetDlgItem(IDC_STATIC_ADDR)->SetFont(m_Font);
	GetDlgItem(IDC_STATIC_TEL)->SetFont(m_Font);
	GetDlgItem(IDC_STATIC_FOX)->SetFont(m_Font);
    GetDlgItem(IDC_STATIC_1)->SetFont(m_Font);
	GetDlgItem(IDC_STATIC_MAIL)->SetFont(m_Font);
	GetDlgItem(IDC_STATIC_WEB)->SetFont(m_Font);

}
void CDlgAbout::UpdateLanguage()
{
	CINScanLiteDlg * pDlg=(CINScanLiteDlg*)GetParent();
	pDlg->m_ZLanguage.SetCreateLanguageState(FALSE);
	pDlg->m_ZLanguage.SetDialogStrings((long)(CDialog*)this,IDD);

	SetControlFont();
}
BOOL CDlgAbout::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN)
	{
		switch(pMsg->wParam)
		{
		case VK_RETURN:
			return  TRUE;
		case VK_ESCAPE:
			return TRUE;
		default:
			break;
		}
	}

	return CDialog::PreTranslateMessage(pMsg);
}


// lcz new 23/05/17
void CDlgAbout::ChangeSize(UINT nID, int x, int y) //nID为控件ID，x,y分别为对话框的当前长和宽
{
	CWnd *pWnd;
	pWnd = GetDlgItem(nID);
	//判断是否为空，因为在窗口创建的时候也会调用OnSize函数，但是此时各个控件还没有创建，Pwnd为空
	if (pWnd != NULL)
	{
		CRect rec;
		pWnd->GetWindowRect(&rec); //获取控件变化前的大小
		ScreenToClient(&rec); //将控件大小装换位在对话框中的区域坐标
		rec.left = rec.left*x / i_dlg_width; //按照比例调整空间的新位置
		rec.top = rec.top*y / i_dlg_height;
		rec.bottom = rec.bottom*y / i_dlg_height;
		rec.right = rec.right*x / i_dlg_width;
		pWnd->MoveWindow(rec); //伸缩控件
	}
}

//  lcz new 23/05/17
void CDlgAbout::resize()
{
	CINScanLiteDlg * pDlg = (CINScanLiteDlg *)AfxGetApp()->GetMainWnd();
	ChangeSize(IDC_STATIC_NAME, pDlg->i_Set_About_Page_Width, pDlg->i_Set_About_Page_Height);
	ChangeSize(IDC_STATIC_ADDR, pDlg->i_Set_About_Page_Width, pDlg->i_Set_About_Page_Height);
	ChangeSize(IDC_STATIC_TEL, pDlg->i_Set_About_Page_Width, pDlg->i_Set_About_Page_Height);
	ChangeSize(IDC_STATIC_FOX, pDlg->i_Set_About_Page_Width, pDlg->i_Set_About_Page_Height);
	ChangeSize(IDC_STATIC_MAIL, pDlg->i_Set_About_Page_Width, pDlg->i_Set_About_Page_Height);
	ChangeSize(IDC_STATIC_1, pDlg->i_Set_About_Page_Width, pDlg->i_Set_About_Page_Height);
	ChangeSize(IDC_STATIC_WEB, pDlg->i_Set_About_Page_Width, pDlg->i_Set_About_Page_Height);

	ChangeSize(IDC_STATIC_ABOUT, pDlg->i_Set_About_Page_Width, pDlg->i_Set_About_Page_Height);

}
