// DlgDataCom.cpp : 实现文件
//

#include "stdafx.h"
#include "INScan-Lite.h"
#include "DlgDataCom.h"
#include "afxdialogex.h"
#include "INScan-LiteDlg.h"
#include "DlgNihe.h"
#include "DlgAutoMarkValue.h"
#include "DenoiseDlg.h"
#include "FitSmoothDlg.h"
#include "SoomthDlg.h"
#include "CutOutLineDlg.h"
#include "DlgDataPro.h"
#include "DialogQualityAnalyse.h"

// CDlgDataCom 对话框

IMPLEMENT_DYNAMIC(CDlgDataCom, CDialog)

CDlgDataCom::CDlgDataCom(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgDataCom::IDD, pParent)
{

}

CDlgDataCom::~CDlgDataCom()
{
}

void CDlgDataCom::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_FIT, m_BtnFit);
	DDX_Control(pDX, IDC_BUTTON_PEAK, m_BtnPeak);
	DDX_Control(pDX, IDC_BUTTON_SMOOTH, m_BtnSmooth);
	DDX_Control(pDX, IDC_BUTTON_MATH, m_CtrlButtonMath);
	DDX_Control(pDX, IDC_BUTTON_CUT_OUT_LINE, m_ctrlButtonCutOutLine);
	DDX_Control(pDX, IDC_BUTTON_SQL, m_BtnCom);
	DDX_Control(pDX, IDC_BUTTON_CONSTRUCT, m_constructBtn);
	DDX_Control(pDX,IDC_BUTTON_QUALITY_ANALYSE, m_qualityAnalyse);
}


BEGIN_MESSAGE_MAP(CDlgDataCom, CDialog)
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BUTTON_FIT, &CDlgDataCom::OnBnClickedButtonFit)
	ON_BN_CLICKED(IDC_BUTTON_SMOOTH, &CDlgDataCom::OnBnClickedButtonSmooth)
	ON_BN_CLICKED(IDC_BUTTON_PEAK, &CDlgDataCom::OnBnClickedButtonPeak)
	ON_BN_CLICKED(IDC_BUTTON_MATH, &CDlgDataCom::OnBnClickedButtonMath)
	ON_BN_CLICKED(IDC_BUTTON_CUT_OUT_LINE, &CDlgDataCom::OnBnClickedButtonCutOutLine)
	ON_BN_CLICKED(IDC_BUTTON_SQL, &CDlgDataCom::OnBnClickedButtonSql)

	//ON_WM_TIMER()// lcz 05/12
	ON_BN_CLICKED(IDC_BUTTON_CONSTRUCT, &CDlgDataCom::OnBnClickedButtonConstruct)
	
	ON_BN_CLICKED(IDC_BUTTON_QUALITY_ANALYSE, &CDlgDataCom::OnBnClickedButtonQualityAnalyse)
END_MESSAGE_MAP()


// CDlgDataCom 消息处理程序


BOOL CDlgDataCom::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_FontChinese.CreatePointFont(nFontSize,strFontChinese);
	m_FontEnglish.CreatePointFont(nFontSize,strFontEngish);

	SetButtonState(&m_BtnFit);
	SetButtonState(&m_BtnPeak);
	SetButtonState(&m_BtnSmooth);
	SetButtonState(&m_CtrlButtonMath);
	SetButtonState(&m_ctrlButtonCutOutLine);
	SetButtonState(&m_BtnCom);
	SetButtonState(&m_constructBtn);
	SetButtonState(&m_qualityAnalyse);

	SetControlFont();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDlgDataCom::SetControlFont()
{
	CFont *m_Font;

	if (sInfo.nLanguage==1)
		m_Font=&m_FontEnglish;
	else
		m_Font=&m_FontChinese;

	GetDlgItem(IDC_BUTTON_FIT)->SetFont(m_Font);
	GetDlgItem(IDC_BUTTON_PEAK)->SetFont(m_Font);
	GetDlgItem(IDC_BUTTON_SMOOTH)->SetFont(m_Font);
	GetDlgItem(IDC_BUTTON_MATH)->SetFont(m_Font);
	GetDlgItem(IDC_BUTTON_CUT_OUT_LINE)->SetFont(m_Font);
	GetDlgItem(IDC_BUTTON_SQL)->SetFont(m_Font);
	GetDlgItem(IDC_BUTTON_CONSTRUCT)->SetFont(m_Font);
	GetDlgItem(IDC_BUTTON_QUALITY_ANALYSE)->SetFont(m_Font);
}


void CDlgDataCom::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	CRect rect;
	GetClientRect(rect);
	dc.FillSolidRect(rect,clr);
	CDialog::OnPaint();
}


HBRUSH CDlgDataCom::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	
	return hbr;
}

void CDlgDataCom::UpdateLanguage()
{
	CINScanLiteDlg * pDlg=(CINScanLiteDlg*)AfxGetApp()->GetMainWnd();
	pDlg->m_ZLanguage.SetCreateLanguageState(FALSE);
	pDlg->m_ZLanguage.SetDialogStrings((long)(CDialog*)this,IDD);
	SetControlFont();
}

BOOL CDlgDataCom::PreTranslateMessage(MSG* pMsg)
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

//  曲线拟合
void CDlgDataCom::OnBnClickedButtonFit()
{
	CINScanLiteDlg * pDlg=(CINScanLiteDlg*)AfxGetApp()->GetMainWnd();

	if (pDlg->m_DlgShow.GetCount()<1)
	{
		return;
	}
	// lcz new 05/12
	//else
	//{
	//	//closeDlgs();
	//	pDlg->pFitSmoothDlg.DoModal();
	//}
	closeDlgs();
	if (pDlg->pFitSmoothDlg->GetSafeHwnd()==NULL)
	{
		pDlg->pFitSmoothDlg->Create(IDD_DIALOG_FIT_SMOOTH,pDlg);
		pDlg->pFitSmoothDlg->ShowWindow(SW_SHOW);
		pDlg->pFitSmoothDlg->CenterWindow();
	}
	//SetTimer(12, 100, NULL);
}

//  曲线平滑
void CDlgDataCom::OnBnClickedButtonSmooth()
{
	CINScanLiteDlg * pDlg=(CINScanLiteDlg*)AfxGetApp()->GetMainWnd();

	if (pDlg->m_DlgShow.GetCount()<1)
	{
		return;
	}
		// lcz new 05/12
	//else
	//{
	//	//closeDlgs();
	//	pDlg->pSmoothDlg.DoModal();
	//}

	closeDlgs();
	if (pDlg->pSmoothDlg->GetSafeHwnd()==NULL)
	{
		pDlg->pSmoothDlg->Create(IDD_DLG_SOOMTH,pDlg);
		pDlg->pSmoothDlg->ShowWindow(SW_SHOW);
		pDlg->pSmoothDlg->CenterWindow();
	}
	//SetTimer(13, 100, NULL);
	
}

//  曲线标峰
void CDlgDataCom::OnBnClickedButtonPeak()
{
	CINScanLiteDlg * pDlg=(CINScanLiteDlg*)AfxGetApp()->GetMainWnd();

	if (pDlg->m_DlgShow.GetCount()<1)
	{
		return;
	}

	closeDlgs();
	//pDlg->pAotumarkvalue.DoModal();// 问题所在
	CDlgAutoMarkValue dlg;// lcz new 05/14  模态对话框的方式
	dlg.DoModal();

	//pDlg->findPeaksZ.DoModal();
	/*	= new CDlgAutoMarkValue(pDlg);
	m_aotumarkvalue->Create(IDD_AUTOMAKRVALUE,pDlg);
	m_aotumarkvalue->ShowWindow(SW_SHOW);
	m_aotumarkvalue->CenterWindow();*/
}

//  计算峰面积、半高宽
void CDlgDataCom::OnBnClickedButtonMath()
{
	// TODO: 在此添加控件通知处理程序代码
	CINScanLiteDlg * pDlg=(CINScanLiteDlg*)AfxGetApp()->GetMainWnd();

	if (pDlg->m_DlgShow.GetCount()<1)
	{
		return;
	}
			// lcz new 05/12
	//else
	//{
	//	//closeDlgs();
	//	pDlg->pDenoiseDlg.DoModal();
	//}

	closeDlgs();
	if (pDlg->pDenoiseDlg->GetSafeHwnd()==NULL)
	{
		pDlg->pDenoiseDlg->Create(IDD_DLG_DENOISE,pDlg);
		pDlg->pDenoiseDlg->ShowWindow(SW_SHOW);
		pDlg->pDenoiseDlg->CenterWindow();
	}
	//SetTimer(10, 100, NULL);
}

//  曲线截取
void CDlgDataCom::OnBnClickedButtonCutOutLine()
{
	// TODO: 在此添加控件通知处理程序代码
	CINScanLiteDlg * pDlg=(CINScanLiteDlg*)AfxGetApp()->GetMainWnd();

	if (pDlg->m_DlgShow.GetCount()<1)
	{
		return;
	}
				// lcz new 05/12
	//else
	//{
	//	//closeDlgs();
	//	pDlg->pCutLineDlg.DoModal();
	//}

	closeDlgs();
	if (pDlg->pCutLineDlg->GetSafeHwnd() == NULL){
		//pDlg->pCutLineDlg = new CCutOutLineDlg(pDlg);
		pDlg->pCutLineDlg->Create(IDD_DLG_CUTOUT_LINE,pDlg);
		pDlg->pCutLineDlg->ShowWindow(SW_SHOW);
		pDlg->pCutLineDlg->CenterWindow();
	}
	//SetTimer(11, 100, NULL);
}

void CDlgDataCom::closeDlgs()
{
	CINScanLiteDlg * pDlg=(CINScanLiteDlg*)AfxGetApp()->GetMainWnd();
	pDlg->pCutLineDlg->OnClose();
	pDlg->pFitSmoothDlg->OnClose();
	pDlg->pSmoothDlg->OnClose();
	pDlg->pDenoiseDlg->OnClose();
	//pDlg->pAotumarkvalue.OnClose();
	// lcz new 05/12
	//pDlg->pCutLineDlg.OnClose();
	//pDlg->pFitSmoothDlg.OnClose();
	//pDlg->pSmoothDlg.OnClose();
	//pDlg->pDenoiseDlg.OnClose();

}

//  数据库比对
void CDlgDataCom::OnBnClickedButtonSql()
{
	CINScanLiteDlg * pDlg=(CINScanLiteDlg*)AfxGetApp()->GetMainWnd();

	if (pDlg->m_DlgShow.GetCount()<1)
	{
		return;
	}
	closeDlgs();
	CDlgDataPro dlg;
	dlg.DoModal();
}

// lcz new 05/12
//void CDlgDataCom::OnTimer(UINT_PTR nIDEvent)
//{
//	// TODO: 在此添加消息处理程序代码和/或调用默认值
//	CINScanLiteDlg * pDlg=(CINScanLiteDlg*)AfxGetApp()->GetMainWnd();
//	switch(nIDEvent)
//	{
//	case 10:
//	{
//		if (!pDlg->pDenoiseDlg->IsWindowVisible())
//		{
//			pDlg->pDenoiseDlg->RemoveLine();
//			KillTimer(10);
//		}
//	}
//	break;
//	case 11:
//	{
//		if (!pDlg->pCutLineDlg->IsWindowVisible())
//		{
//			pDlg->pCutLineDlg->RemoveLine();
//			KillTimer(11);
//		}
//	}
//	break;
//	case 12:
//	{
//		if (!pDlg->pFitSmoothDlg->IsWindowVisible())
//		{
//			pDlg->pFitSmoothDlg->RemoveLine();
//			KillTimer(12);
//		}
//	}
//	break;
//	case 13:
//	{
//		if (!pDlg->pSmoothDlg->IsWindowVisible())
//		{
//			pDlg->pSmoothDlg->RemoveLine();
//			KillTimer(13);
//		}
//	}
//	break;
//	}
//	CDialog::OnTimer(nIDEvent);
//}


void CDlgDataCom::OnBnClickedButtonConstruct()
{

	char filename[150] = { 0 };
	// 获取当前程序目录
	::GetModuleFileName(AfxGetApp()->m_hInstance, filename, 150);
	// 转化为配置文件目录
	CString path = filename;
	path.Replace("INScan-Lite.exe", "FI-DB.exe");

	// 打开自建库窗口
	HANDLE hProcess = NULL;
	PROCESS_INFORMATION processInfo;
	STARTUPINFO startupInfo;
	::ZeroMemory(&startupInfo, sizeof(startupInfo));
	startupInfo.cb = sizeof(startupInfo);
	BOOL res = ::CreateProcess(path, (LPTSTR)0,
		NULL,  // process security
		NULL,  // thread security
		FALSE, // no inheritance
		0,     // no startup flags
		NULL,  // no special environment
		NULL,  // default startup directory
		&startupInfo,
		&processInfo);
		
}

void CDlgDataCom::OnBnClickedButtonQualityAnalyse()
{

    //2024.12.31
	CDialogQualityAnalyse dlg;
	dlg.DoModal();        //弹出模态对话框

}


