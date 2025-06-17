// DlgTestCheckK.cpp : 实现文件
//

#include "stdafx.h"
#include "INScan-Lite.h"
#include "DlgTestCheckK.h"
#include "afxdialogex.h"
#include "INScan-LiteDlg.h"

DWORD WINAPI  ThreadGetDataK(LPVOID lpParam);
DWORD WINAPI  ThreadGetDataA1K(LPVOID lpParam);
// CDlgTestCheckK 对话框

IMPLEMENT_DYNAMIC(CDlgTestCheckK, CDialog)

CDlgTestCheckK::CDlgTestCheckK(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgTestCheckK::IDD, pParent)
{

}

CDlgTestCheckK::~CDlgTestCheckK()
{
	/*m_FontEnglish.DeleteObject();
	m_FontChinese.DeleteObject();*/
}

void CDlgTestCheckK::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_START, m_BtnStart);
	DDX_Control(pDX, IDC_BUTTON_STOP, m_BtnStop);
	DDX_Radio(pDX, IDC_RADIO1, m_RadioL);
}


BEGIN_MESSAGE_MAP(CDlgTestCheckK, CDialog)
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_RADIO1, &CDlgTestCheckK::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO2, &CDlgTestCheckK::OnBnClickedRadio2)
	ON_BN_CLICKED(IDC_RADIO3, &CDlgTestCheckK::OnBnClickedRadio3)
	ON_BN_CLICKED(IDC_BUTTON_START, &CDlgTestCheckK::OnBnClickedButtonStart)
	ON_BN_CLICKED(IDC_BUTTON_STOP, &CDlgTestCheckK::OnBnClickedButtonStop)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CDlgTestCheckK 消息处理程序


BOOL CDlgTestCheckK::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_FontChinese.CreatePointFont(nFontSize,strFontChinese);
	m_FontEnglish.CreatePointFont(nFontSizeEH,strFontEngish);
	SetData();
	SetControlFont();
	BtnEnable(TRUE);
	m_Handle=NULL;
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
void CDlgTestCheckK::SetData()
{
	SetButtonState(&m_BtnStart);
	SetButtonState(&m_BtnStop);
	m_RadioL=sTInfo.KPowerMode;

	UpdateData(FALSE);
}
void CDlgTestCheckK::SetControlFont()
{
	CFont *m_Font;

	if (sInfo.nLanguage==1)
		m_Font=&m_FontEnglish;
	else
		m_Font=&m_FontChinese;

	GetDlgItem(IDC_BUTTON_START)->SetFont(m_Font);
	GetDlgItem(IDC_BUTTON_STOP)->SetFont(m_Font);
	GetDlgItem(IDC_STATIC_LA)->SetFont(m_Font);
	GetDlgItem(IDC_STATIC_L)->SetFont(m_Font);
	GetDlgItem(IDC_STATIC_M)->SetFont(m_Font);
	GetDlgItem(IDC_STATIC_H)->SetFont(m_Font);
}

void CDlgTestCheckK::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	CRect rect;
	GetClientRect(rect);
	dc.FillSolidRect(rect,clr);
	CDialog::OnPaint();
}


HBRUSH CDlgTestCheckK::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	if (nCtlColor==CTLCOLOR_STATIC)
	{
		//pDC->SetBkMode(TRANSPARENT);
		pDC->SetBkColor(clr);
		HBRUSH b =CreateSolidBrush(clr);
		return b;
	}
	return hbr;
}
void CDlgTestCheckK::UpdateLanguage()
{
	CINScanLiteDlg * pDlg=(CINScanLiteDlg*)AfxGetApp()->GetMainWnd();
	pDlg->m_ZLanguage.SetCreateLanguageState(FALSE);
	pDlg->m_ZLanguage.SetDialogStrings((long)(CDialog*)this,IDD);

	SetControlFont();
}

void CDlgTestCheckK::OnBnClickedRadio1()
{
	UpdateData(TRUE);
	sTInfo.KPowerMode=m_RadioL;
}


void CDlgTestCheckK::OnBnClickedRadio2()
{
	UpdateData(TRUE);
	sTInfo.KPowerMode=m_RadioL;
}


void CDlgTestCheckK::OnBnClickedRadio3()
{
	UpdateData(TRUE);
	sTInfo.KPowerMode=m_RadioL;
}


void CDlgTestCheckK::OnBnClickedButtonStart()
{
	CINScanLiteDlg * pDlg=(CINScanLiteDlg*)AfxGetApp()->GetMainWnd();

	static bool bReconnect = false;
	int Temp=0;
	m_ZolixCCD.GetTemper(&Temp);
	if(Temp > -5)
	{
		if(MessageBox(pDlg->LoadStringMeau(_T("ID_START_FAIL")),pDlg->LoadStringMeau(_T("ID_WARNING")),MB_OKCANCEL|MB_ICONWARNING) == IDCANCEL)
			return;
	}
	else
	{
		if(!bReconnect)
		{
			m_ZolixCCD.DisConnect();
			pDlg->m_DlgSet.ConnectCCD();
			bReconnect = true;
		}

	}

	m_bAutoRunning=TRUE;
	m_bAutoRunningStatue=TRUE;
	pDlg->SetBtnEnable(FALSE);
	//pDlg->m_DlgFile.m_DlgDataPro.ClearList();
	BtnEnable(FALSE);

	DWORD dwThreadID;
	SetTimer(1,1000,NULL);
	/*if (sInfo.OminiType == 50) {
		m_Handle = CreateThread(NULL, 0, ThreadGetDataA1K, NULL, 0, &dwThreadID);

	}*/
	//else {
		m_Handle = CreateThread(NULL, 0, ThreadGetDataK, NULL, 0, &dwThreadID);

	//}
}
void CDlgTestCheckK::BtnEnable(BOOL bShow)
{
	

	GetDlgItem(IDC_BUTTON_START)->EnableWindow(bShow);
	if (/*sInfo.LConnect==1&&*/sInfo.CConnect==1)
		GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(!bShow);
	else
		GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(bShow);
	 

	if(m_bAutoRunning==true&&bShow==FALSE)
	{
		GetDlgItem(IDC_RADIO1)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO2)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO3)->EnableWindow(FALSE);
		GetDlgItem(IDC_STATIC_H)->EnableWindow(FALSE);
		GetDlgItem(IDC_STATIC_L)->EnableWindow(FALSE);
		GetDlgItem(IDC_STATIC_M)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_RADIO1)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO2)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO3)->EnableWindow(TRUE);
		GetDlgItem(IDC_STATIC_H)->EnableWindow(TRUE);
		GetDlgItem(IDC_STATIC_L)->EnableWindow(TRUE);
		GetDlgItem(IDC_STATIC_M)->EnableWindow(TRUE);
	}
}


void CDlgTestCheckK::OnBnClickedButtonStop()
{
	
	m_bAutoRunning=FALSE;
	Sleep(1000);
	if (m_Handle!=NULL)
	{
		CloseHandle(m_Handle);
	}
	m_Handle=NULL;

}


void CDlgTestCheckK::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent==1)
	{
		CINScanLiteDlg * pDlg=(CINScanLiteDlg*)AfxGetApp()->GetMainWnd();

		if (m_bAutoRunning==FALSE&&m_bAutoRunningStatue==FALSE)
		{
			KillTimer(1);
			pDlg->SetBtnEnable(TRUE);
			BtnEnable(TRUE);
			pDlg->m_DlgShow.OnAutomatic();
			pDlg->m_DlgShow.OnSetAutoRepaint(TRUE);
			if(pDlg->m_DlgShow.bReturn==FALSE)
			{
				MessageBox(pDlg->LoadStringMeau(_T("ID_CHECK_FAIL")));
			}
		}	
	}

	CDialog::OnTimer(nIDEvent);
}
BOOL CDlgTestCheckK::PreTranslateMessage(MSG* pMsg)
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
