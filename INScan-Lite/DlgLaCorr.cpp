// DlgLaCorr.cpp : 实现文件
//

#include "stdafx.h"
#include "INScan-Lite.h"
#include "DlgLaCorr.h"
#include "afxdialogex.h"
#include "INScan-LiteDlg.h"

// CDlgLaCorr 对话框

IMPLEMENT_DYNAMIC(CDlgLaCorr, CDialog)

CDlgLaCorr::CDlgLaCorr(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgLaCorr::IDD, pParent)
{

}

CDlgLaCorr::~CDlgLaCorr()
{
}

void CDlgLaCorr::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_SET, m_BtnSet);
}


BEGIN_MESSAGE_MAP(CDlgLaCorr, CDialog)
	ON_WM_CTLCOLOR()
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BUTTON_SET, &CDlgLaCorr::OnBnClickedButtonSet)
END_MESSAGE_MAP()


// CDlgLaCorr 消息处理程序


BOOL CDlgLaCorr::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化

	m_FontChinese.CreatePointFont(120,strFontChinese);
	m_FontEnglish.CreatePointFont(120,strFontEngish);


    SetData();
	//SetControlFont();
	UpdateLanguage();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
void CDlgLaCorr::SetData()
{
	CString str;
	str.Format(_T("%.3f"),sCInfo.TargetNum);
	GetDlgItem(IDC_EDIT_OBJ)->SetWindowText(str);
	str.Format(_T("%.3f"),sCInfo.CorrectNum);
	GetDlgItem(IDC_EDIT_CORR)->SetWindowText(str);
	str.Format(_T("%.3f"),sCInfo.Laser);
	GetDlgItem(IDC_STATIC_NUM)->SetWindowText(str);

	SetButtonState(&m_BtnSet);
}
void CDlgLaCorr::SetControlFont()
{
	CFont *m_Font;

	if (sInfo.nLanguage==1)
		m_Font=&m_FontEnglish;
	else
		m_Font=&m_FontChinese;


	GetDlgItem(IDC_STATIC_OBJ)->SetFont(m_Font);
	GetDlgItem(IDC_STATIC_CORR)->SetFont(m_Font);
	GetDlgItem(IDC_STATIC_LA)->SetFont(m_Font);
	GetDlgItem(IDC_STATIC_NUM)->SetFont(m_Font);
	GetDlgItem(IDC_STATIC_NM)->SetFont(m_Font);
	GetDlgItem(IDC_STATIC_CM1)->SetFont(m_Font);
	GetDlgItem(IDC_STATIC_CM2)->SetFont(m_Font);

	GetDlgItem(IDC_BUTTON_SET)->SetFont(m_Font);
	GetDlgItem(IDC_EDIT_OBJ)->SetFont(m_Font);
	GetDlgItem(IDC_EDIT_CORR)->SetFont(m_Font);
	

}


HBRUSH CDlgLaCorr::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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


void CDlgLaCorr::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	CRect rect;
	GetClientRect(rect);
	dc.FillSolidRect(rect,clr);
	CDialog::OnPaint();
}
void CDlgLaCorr::UpdateLanguage()
{
	CINScanLiteDlg * pDlg=(CINScanLiteDlg*)AfxGetApp()->GetMainWnd();
	pDlg->m_ZLanguage.SetCreateLanguageState(FALSE);
	pDlg->m_ZLanguage.SetDialogStrings((long)(CDialog*)this,IDD);

	SetControlFont();
}

void CDlgLaCorr::OnBnClickedButtonSet()
{
	CINScanLiteDlg * pDlg=(CINScanLiteDlg*)AfxGetApp()->GetMainWnd();
	if (sInfo.CConnect==1&&/*sInfo.LConnect==1&&*/pDlg->m_DlgShow.GetCount()>0)
	{
		CString str;
		GetDlgItem(IDC_EDIT_CORR)->GetWindowText(str);
		sCInfo.CorrectNum=atof(str);
		GetDlgItem(IDC_EDIT_OBJ)->GetWindowText(str);
		sCInfo.TargetNum=atof(str);

		double newlaser = CM/((CM/sCInfo.Laser)-sCInfo.CorrectNum+sCInfo.TargetNum);
		//double newlaser = CM / ( ( CM / standardLaser ) - sCInfo.CorrectNum + sCInfo.TargetNum );
		sCInfo.Laser=newlaser;

		str.Format(_T("%.3f"),newlaser);
		GetDlgItem(IDC_STATIC_NUM)->SetWindowText(str);
		//sSInfo.CorrMode=1;
		//SetButtonS(FALSE);
		//pDlg->m_DlgTest.m_DlgTestCheckJ.OnBnClickedButtonL();
		pDlg->WriteDeviceInfo();
		CString tempTEXT = pDlg->LoadStringMeau(_T("ID_CW_CORR"));
		AfxMessageBox(tempTEXT, MB_ICONINFORMATION);
	}
	
}
void CDlgLaCorr::SetBtnLaEnable(BOOL bRet)
{
	GetDlgItem(IDC_BUTTON_SET)->EnableWindow(bRet);
}
void CDlgLaCorr::SetButtonS(BOOL bRet)
{
	CINScanLiteDlg * pDlg=(CINScanLiteDlg*)AfxGetApp()->GetMainWnd();
	SetBtnLaEnable(bRet);
	pDlg->SetBtnEnable(bRet);
	pDlg->m_DlgAdv.SetBtnEnable(bRet);
}
BOOL CDlgLaCorr::PreTranslateMessage(MSG* pMsg)
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
