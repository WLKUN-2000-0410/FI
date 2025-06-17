// DlgAdv.cpp : 实现文件
//

#include "stdafx.h"
#include "INScan-Lite.h"
#include "INScan-LiteDlg.h"
#include "DlgAdv.h"
#include "afxdialogex.h"
#include "DlgPassWord.h"

#include "DlgLaCorr.h"
#include "BxsOffset.h"
// CDlgAdv 对话框

IMPLEMENT_DYNAMIC(CDlgAdv, CDialog)

CDlgAdv::CDlgAdv(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgAdv::IDD, pParent)
{

}

CDlgAdv::~CDlgAdv()
{
}

void CDlgAdv::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_ABOUT, m_BtnAbout);
	DDX_Control(pDX, IDC_BUTTON_LAN, m_BtnLan);
	DDX_Control(pDX, IDC_BUTTON_SET, m_BtnSet);
	DDX_Control(pDX, IDC_BUTTON_OFF, m_BtnOff);
}


BEGIN_MESSAGE_MAP(CDlgAdv, CDialog)
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BUTTON_ABOUT, &CDlgAdv::OnBnClickedButtonAbout)
	ON_BN_CLICKED(IDC_BUTTON_LAN, &CDlgAdv::OnBnClickedButtonLan)
	ON_BN_CLICKED(IDC_BUTTON_SET, &CDlgAdv::OnBnClickedButtonSet)
	ON_BN_CLICKED(IDC_BUTTON_OFF, &CDlgAdv::OnBnClickedButtonOff)
END_MESSAGE_MAP()


// CDlgAdv 消息处理程序


BOOL CDlgAdv::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetButtonState(&m_BtnAbout);
	SetButtonState(&m_BtnSet);
	SetButtonState(&m_BtnLan);
	SetButtonState(&m_BtnOff);

	m_FontChinese.CreatePointFont(nFontSize,strFontChinese);
	m_FontEnglish.CreatePointFont(nFontSize,strFontEngish);
	SetControlFont();

	//InitDialog();

	//InitDialogShow(SW_HIDE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

//void CDlgAdv::InitDialog()
//{
//	m_DlgLaCorr.Create(IDD_DLG_LACORR,this);
//
//	CRect rect;
//	GetDlgItem(IDC_STATIC_OFF)->GetWindowRect(rect);
//	GetDlgItem(IDC_STATIC_OFF)->ShowWindow(SW_HIDE);
//	ScreenToClient(&rect);
//	::SetWindowPos(m_DlgLaCorr.m_hWnd,HWND_TOP,rect.left,rect.top,rect.Width(),rect.Height()+50,SWP_SHOWWINDOW|SWP_FRAMECHANGED);
//
//	m_DlgLaCorr.ModifyStyle(0,WS_CLIPSIBLINGS);
//	m_DlgLaCorr.SetParent(this);
//
//}
//void CDlgAdv::InitDialogShow(BOOL bShow)
//{
//	m_DlgLaCorr.ShowWindow(bShow);
//}

void CDlgAdv::SetControlFont()
{
	CFont *m_Font;

	if (sInfo.nLanguage==1)
		m_Font=&m_FontEnglish;
	else
		m_Font=&m_FontChinese;

	GetDlgItem(IDC_BUTTON_ABOUT)->SetFont(m_Font);
	GetDlgItem(IDC_BUTTON_LAN)->SetFont(m_Font);
	GetDlgItem(IDC_BUTTON_SET)->SetFont(m_Font);
	GetDlgItem(IDC_BUTTON_OFF)->SetFont(m_Font);
}

void CDlgAdv::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	CRect rect;
	GetClientRect(rect);
	dc.FillSolidRect(rect,clr);
	CDialog::OnPaint();
}


HBRUSH CDlgAdv::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}


void CDlgAdv::OnBnClickedButtonAbout()
{
	CINScanLiteDlg *pDlg =(CINScanLiteDlg *)GetParent();
	sInfo.AdvShowID=0;
	pDlg->SetAdvShow(sInfo.AdvShowID);

	g_bCloseLaser = true;//  关闭设置里的激光
	Sleep(100);
}


void CDlgAdv::OnBnClickedButtonLan()
{
	CINScanLiteDlg *pDlg =(CINScanLiteDlg *)GetParent();
	sInfo.AdvShowID=1;
	pDlg->SetAdvShow(sInfo.AdvShowID);

	g_bCloseLaser = true;//  关闭设置里的激光
	Sleep(100);
}


void CDlgAdv::OnBnClickedButtonSet()
{
	CDlgPassWord dlg;
	if (dlg.DoModal()==IDOK)
	{
		CINScanLiteDlg *pDlg =(CINScanLiteDlg *)GetParent();
		sInfo.AdvShowID=2;
		pDlg->SetAdvShow(sInfo.AdvShowID);
	}
	
}
void CDlgAdv::SetBtnEnable(BOOL bShow)
{
	GetDlgItem(IDC_BUTTON_ABOUT)->EnableWindow(bShow);
	GetDlgItem(IDC_BUTTON_SET)->EnableWindow(bShow);
	GetDlgItem(IDC_BUTTON_LAN)->EnableWindow(bShow);
	GetDlgItem(IDC_BUTTON_OFF)->EnableWindow(bShow);
}

void CDlgAdv::OnBnClickedButtonOff()
{
	CINScanLiteDlg *pDlg =(CINScanLiteDlg *)GetParent();
	sInfo.AdvShowID=3;
	pDlg->SetAdvShow(sInfo.AdvShowID);
	g_bCloseLaser = true;//  关闭设置里的激光
	Sleep(100);
	// lcz new 07/05 根据是波长校正还是波数校正，选择不同的界面
	if(g_bWaveLenCorr)
	{
		CDlgLaCorr dlg;
		dlg.DoModal();
	}
	else
	{
		if (pDlg->m_DlgShow.GetTeeCount()>0)
		{
			CBxsOffset dlg;
			dlg.DoModal();
		}
	}
	
	
}
void CDlgAdv::UpdateLanguage()
{
	CINScanLiteDlg * pDlg=(CINScanLiteDlg*)GetParent();
	pDlg->m_ZLanguage.SetCreateLanguageState(FALSE);
	pDlg->m_ZLanguage.SetDialogStrings((long)(CDialog*)this,IDD);

	//m_DlgLaCorr.UpdateLanguage();
	SetControlFont();
}
BOOL CDlgAdv::PreTranslateMessage(MSG* pMsg)
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
