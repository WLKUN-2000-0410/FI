// DlgNihe.cpp : 实现文件
//

#include "stdafx.h"
#include "DlgNihe.h"
#include "afxdialogex.h"
#include "..\file\TeeChart2011\series.h"


// CDlgNihe 对话框

IMPLEMENT_DYNAMIC(CDlgNihe, CDialogEx)

CDlgNihe::CDlgNihe(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgNihe::IDD, pParent)
{
	m_app = (CINScanLiteDlg*)AfxGetApp()->GetMainWnd();
}

CDlgNihe::~CDlgNihe()
{
	CINScanLiteDlg * pThreadApp=(CINScanLiteDlg*)AfxGetApp();
	//pThreadApp->isnh =FALSE;
	if(m_hWnd != NULL) 
		DestroyWindow();
}

void CDlgNihe::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SUB, m_BtnSub);
	DDX_Control(pDX, IDC_AUTOFIT, m_BtnAu);
	DDX_Control(pDX, IDC_RECOVER, m_BtnRe);
	DDX_Control(pDX, IDC_CLEAR, m_BtnCl);
}


BEGIN_MESSAGE_MAP(CDlgNihe, CDialogEx)
	ON_BN_CLICKED(IDC_CLEAR, &CDlgNihe::OnBnClickedClear)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_SUB, &CDlgNihe::OnBnClickedSub)
	ON_BN_CLICKED(IDC_AUTOFIT, &CDlgNihe::OnBnClickedAutofit)
	ON_BN_CLICKED(IDC_RECOVER, &CDlgNihe::OnBnClickedRecover)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_MICISHU, &CDlgNihe::OnNMCustomdrawMicishu)
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CDlgNihe 消息处理程序


BOOL CDlgNihe::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	SetButtonState(&m_BtnSub);
	SetButtonState(&m_BtnAu);
	SetButtonState(&m_BtnCl);
	SetButtonState(&m_BtnRe);
	UpdataEdit();

	((CSliderCtrl *)GetDlgItem(IDC_MICISHU))->SetRange(1,8);
	((CSliderCtrl *)GetDlgItem(IDC_MICISHU))->SetPos(6);
	((CSliderCtrl *)GetDlgItem(IDC_MICISHU))->SetPageSize(1);
	GetDlgItem(IDC_MIVALUESHOW)->SetWindowText("6");
	oldvalue = 6;
	int newvalue = ((CSliderCtrl *)GetDlgItem(IDC_MICISHU))->GetPos();

	m_app->m_DlgShow.Nihe(newvalue);
	GetDlgItem(IDC_SUB)->EnableWindow(TRUE);
	UpdateLanguage();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDlgNihe::UpdateLanguage()
{

	m_app->m_ZLanguage.SetCreateLanguageState(FALSE);
	m_app->m_ZLanguage.SetDialogStrings((long)(CDialog*)this,IDD);

}


void CDlgNihe::UpdataEdit()
{
	
}


void CDlgNihe::OnBnClickedClear()
{
	// TODO: 在此添加控件通知处理程序代码
	m_app->m_DlgShow.clearline();
	GetDlgItem(IDC_SUB)->EnableWindow(FALSE);
	GetDlgItem(IDC_AUTOFIT)->EnableWindow(FALSE);
	GetDlgItem(IDC_RECOVER)->EnableWindow(FALSE);
}


void CDlgNihe::OnClose()
{

	m_app->m_DlgShow.ReMoveSeries(1);
	CDialogEx::OnClose();
}



void CDlgNihe::OnBnClickedSub()
{
	// TODO: 在此添加控件通知处理程序代码
	m_app->m_DlgShow.subline();
	GetDlgItem(IDC_SUB)->EnableWindow(FALSE);
	GetDlgItem(IDC_AUTOFIT)->EnableWindow(FALSE);
	GetDlgItem(IDC_RECOVER)->EnableWindow(TRUE);
}


void CDlgNihe::OnBnClickedAutofit()
{
	// TODO: 在此添加控件通知处理程序代码
	int newvalue = ((CSliderCtrl *)GetDlgItem(IDC_MICISHU))->GetPos();
	m_app->m_DlgShow.PolynomSubChart(1,newvalue);
	OnBnClickedSub();
}



void CDlgNihe::OnBnClickedRecover()
{
	// TODO: 在此添加控件通知处理程序代码
	
    m_app->m_DlgShow.Recover();
	GetDlgItem(IDC_AUTOFIT)->EnableWindow(TRUE);
	GetDlgItem(IDC_RECOVER)->EnableWindow(FALSE);

}


void CDlgNihe::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类

	CDialogEx::OnOK();
}



void CDlgNihe::OnNMCustomdrawMicishu(NMHDR *pNMHDR, LRESULT *pResult)
{
	//LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	//*pResult = 0;
	
	int newvalue = ((CSliderCtrl *)GetDlgItem(IDC_MICISHU))->GetPos();
	CString strvalue;
	strvalue.Format("%d",newvalue);
	GetDlgItem(IDC_MIVALUESHOW)->SetWindowText(strvalue);
	int cont = m_app->m_DlgShow.m_Tchart.Series(0).GetCount();
	if (newvalue!=oldvalue&&cont>20)
	{
		oldvalue = newvalue;
		m_app->m_DlgShow.PolynomSubChart(1,newvalue);
		GetDlgItem(IDC_SUB)->EnableWindow(TRUE);
	}	
}


BOOL CDlgNihe::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (!CDialogEx::PreCreateWindow(cs))
	{
		return FALSE;
	}
	
	cs.style &= ~(LONG) FWS_ADDTOTITLE;

	return TRUE;
}


void CDlgNihe::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	CRect rect;
	GetClientRect(rect);
	dc.FillSolidRect(rect,clr);
	CDialog::OnPaint();
}


HBRUSH CDlgNihe::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
	if (nCtlColor==CTLCOLOR_STATIC)
	{
		pDC->SetBkColor(clr);
		HBRUSH b =CreateSolidBrush(clr);
		return b;
	}
	return hbr;
}
