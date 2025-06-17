// DlgTestCheckIN.cpp : 实现文件
//

#include "stdafx.h"
#include "INScan-Lite.h"
#include "DlgTestCheckIN.h"
#include "afxdialogex.h"
#include "INScan-LiteDlg.h"
#include "FileInfo.h"

#include "..\file\TeeChart2011\series.h"
// DlgTestCheckIN 对话框
DWORD WINAPI  ThreadGetDataIN(LPVOID lpParam);
// 1064间隔采集
DWORD WINAPI  ThreadGetDataD1064IN(LPVOID lpParam);

IMPLEMENT_DYNAMIC(CDlgTestCheckIN, CDialog)

CDlgTestCheckIN::CDlgTestCheckIN(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgTestCheckIN::IDD, pParent)
{
	m_bStop = false;
}

CDlgTestCheckIN::~CDlgTestCheckIN()
{
}

void CDlgTestCheckIN::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_START, m_BtnStart);
	DDX_Control(pDX, IDC_BUTTON_STOP, m_BtnStop);
	DDX_Control(pDX, IDC_BUTTON_PARA, m_BtnPara);
	DDX_Control(pDX, IDC_BUTTON_SET, m_BtnSet);
	DDX_Control(pDX, IDC_BUTTON_SET2, m_BtnSet2);
	DDX_Text(pDX, IDC_EDIT_TIME, m_EditTime);
	DDX_Text(pDX, IDC_EDIT_ACC, m_EditAcc);
}


BEGIN_MESSAGE_MAP(CDlgTestCheckIN, CDialog)
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BUTTON_START, &CDlgTestCheckIN::OnBnClickedButtonStart)
	ON_BN_CLICKED(IDC_BUTTON_STOP, &CDlgTestCheckIN::OnBnClickedButtonStop)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_SET2, &CDlgTestCheckIN::OnBnClickedButtonSet2)
	ON_BN_CLICKED(IDC_BUTTON_PARA, &CDlgTestCheckIN::OnBnClickedButtonPara)
	ON_BN_CLICKED(IDC_CHECK_ACC, &CDlgTestCheckIN::OnBnClickedCheckAcc)
	ON_BN_CLICKED(IDC_BUTTON_SET, &CDlgTestCheckIN::OnBnClickedButtonSet)
	ON_EN_CHANGE(IDC_EDIT_TIME, &CDlgTestCheckIN::OnEnChangeEditTime)
	ON_EN_CHANGE(IDC_EDIT_ACC, &CDlgTestCheckIN::OnEnChangeEditAcc)
END_MESSAGE_MAP()


// DlgTestCheckIN 消息处理程序
BOOL CDlgTestCheckIN::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_FontChinese.CreatePointFont(nFontSize,strFontChinese);
	m_FontEnglish.CreatePointFont(nFontSizeEH,strFontEngish);
	SetData();
	SetCheckBk(sINInfo.bCheak);
	SetControlFont();

	((CButton*)GetDlgItem(IDC_CHECK_ACC))->SetCheck(sINInfo.bCheak);	
	m_Handle=NULL;

	m_EditAcc= sINInfo.AccNum;
	m_EditTime= sINInfo.Time;

	// lcz new 
	CString str;
	str.Format(_T("%d"), (int)sSInfo.XLeft);
	GetDlgItem(IDC_EDIT_X_LEFT)->SetWindowText(str);
	str.Format(_T("%d"), (int)sSInfo.XRight);
	GetDlgItem(IDC_EDIT_X_RIGHT)->SetWindowText(str);

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
void CDlgTestCheckIN::SetData()
{
	SetButtonState(&m_BtnStart);
	SetButtonState(&m_BtnStop);
	SetButtonState(&m_BtnPara);
	SetButtonState(&m_BtnSet);
	SetButtonState(&m_BtnSet2);

	UpdateData(FALSE);
}
void CDlgTestCheckIN::SetControlFont()
{
	CFont *m_Font;

	if (sInfo.nLanguage==1)
		m_Font=&m_FontEnglish;
	else
		m_Font=&m_FontChinese;

	GetDlgItem(IDC_BUTTON_START)->SetFont(m_Font);
	GetDlgItem(IDC_BUTTON_STOP)->SetFont(m_Font);
	GetDlgItem(IDC_STATIC_TIME)->SetFont(m_Font);
	GetDlgItem(IDC_CHECK_ACC)->SetFont(m_Font);
	GetDlgItem(IDC_STATIC_ACC)->SetFont(m_Font);
	GetDlgItem(IDC_BUTTON_SET2)->SetFont(m_Font);
	GetDlgItem(IDC_BUTTON_SET)->SetFont(m_Font);
	GetDlgItem(IDC_BUTTON_PARA)->SetFont(m_Font);
	GetDlgItem(IDC_STATIC_S)->SetFont(m_Font);
	GetDlgItem(IDC_EDIT_TIME)->SetFont(m_Font);
	GetDlgItem(IDC_EDIT_ACC)->SetFont(m_Font);

	GetDlgItem(IDC_STATIC_XRange)->SetFont(m_Font);
	GetDlgItem(IDC_STATIC_SYMBOL)->SetFont(m_Font);
	GetDlgItem(IDC_EDIT_X_LEFT)->SetFont(m_Font);
	GetDlgItem(IDC_EDIT_X_RIGHT)->SetFont(m_Font);
}

void CDlgTestCheckIN::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	CRect rect;
	GetClientRect(rect);
	dc.FillSolidRect(rect,clr);
	CDialog::OnPaint();
}


HBRUSH CDlgTestCheckIN::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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
void CDlgTestCheckIN::UpdateLanguage()
{
	CINScanLiteDlg * pDlg=(CINScanLiteDlg*)AfxGetApp()->GetMainWnd();
	pDlg->m_ZLanguage.SetCreateLanguageState(FALSE);
	pDlg->m_ZLanguage.SetDialogStrings((long)(CDialog*)this,IDD);

	SetControlFont();
}

void CDlgTestCheckIN::SetAutoScanBtnStatus(bool bStatus)
{
	CINScanLiteDlg * pDlg=(CINScanLiteDlg*)AfxGetApp()->GetMainWnd();
	pDlg->SetBtnEnable(bStatus);
	BtnEnable(bStatus);
}

void CDlgTestCheckIN::AutoScan_96()
{
	CINScanLiteDlg * pDlg=(CINScanLiteDlg*)AfxGetApp()->GetMainWnd();
	m_bAutoRunning=TRUE;
	m_bAutoRunningStatue=TRUE;
	pDlg->SetBtnEnable(FALSE);
	//pDlg->m_DlgFile.m_DlgDataPro.ClearList();
	BtnEnable(FALSE);

	DWORD dwThreadID;
	SetTimer(2,1000,NULL);
	m_Handle=CreateThread(NULL,0,ThreadGetDataIN,NULL,0,&dwThreadID);
}



//  间隔里，开始采样
void CDlgTestCheckIN::OnBnClickedButtonStart()
{
	//AfxBeginThread(IntervalTest, (LPVOID)this);


	CINScanLiteDlg * pDlg=(CINScanLiteDlg*)AfxGetApp()->GetMainWnd();
	//  判断范围是否有问题
	CString str1, str2;
	GetDlgItem(IDC_EDIT_X_LEFT)->GetWindowText(str1);
	double temp1 = atof(str1);
	GetDlgItem(IDC_EDIT_X_RIGHT)->GetWindowText(str2);
	double temp2 = atof(str2);
	if(temp1 >= temp2)
	{
		MessageBox(pDlg->LoadStringMeau(_T("ID_RANGE_ERROR")));
		return;
	}
	OnBnClickedButtonSet2();
	OnBnClickedButtonSet();

	//  判断现有的条数是否为maxLineNum,，如果已经存在maxLineNum条曲线，提示先删除部分曲线
	int numOfLineShowed = 0;
	for(int i = 0; i < pDlg->m_DlgShow.m_Tchart.GetSeriesCount(); i++)
	{
		if(pDlg->m_DlgShow.m_Tchart.Series(i).GetShowInLegend() == true)
			numOfLineShowed++;
	}
	if(numOfLineShowed >= maxLineNum)
	{
		MessageBox(pDlg->LoadStringMeau(_T("ID_OUTOF_LINE_RANGE")));
		return;
	}

	//static bool bReconnect = false;
	int Temp=0;
	g_bGetTemp = true;

	if( sInfo.OminiType != 20)
	{
		Sleep(100);

		if(g_iTemp > -5)
		{
			g_bGetTemp = false;
			if(MessageBox(pDlg->LoadStringMeau(_T("ID_START_FAIL")),pDlg->LoadStringMeau(_T("ID_WARNING")),MB_OKCANCEL|MB_ICONWARNING) == IDCANCEL)
				return;
		}

		CDlgTest *test = (CDlgTest*)GetParent();
		if (!test->HasRestarted())
		{
			test->restartCCD();
		}
	}

	
	m_bAutoRunning=TRUE;
	m_bAutoRunningStatue=TRUE;
	pDlg->SetBtnEnable(FALSE);
	//pDlg->m_DlgFile.m_DlgDataPro.ClearList();
	BtnEnable(FALSE);

	DWORD dwThreadID;
	SetTimer(1,1000,NULL);// 如果检测完成，标志位改变，则关激光

	// 判断是1064 CCD还是其它CCD
	if(pDlg->m_b1064)
	{
		m_Handle = CreateThread(NULL, 0, ThreadGetDataD1064IN, NULL, 0, &dwThreadID);
	}
	else
	{
		m_Handle = CreateThread(NULL, 0, ThreadGetDataIN, NULL, 0, &dwThreadID);
	}
}

void CDlgTestCheckIN::SetCheckBk(int nBk)
{

	GetDlgItem(IDC_BUTTON_SET)->ShowWindow(nBk);
	GetDlgItem(IDC_EDIT_ACC)->ShowWindow(nBk);
	GetDlgItem(IDC_STATIC_ACC)->ShowWindow(nBk);
	GetDlgItem(IDC_STATIC_KK)->ShowWindow(nBk);
	
}

void CDlgTestCheckIN::BtnEnable(BOOL bShow)
{
	GetDlgItem(IDC_BUTTON_START)->EnableWindow(bShow);
	if (/*sInfo.LConnect==1&&*/sInfo.CConnect==1)
		GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(!bShow);
	else
		GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(bShow);


	if(m_bAutoRunning==true&&bShow==FALSE)
	{
		GetDlgItem(IDC_EDIT_TIME)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK_ACC)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_SET2)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_SET)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_ACC)->EnableWindow(FALSE);
		GetDlgItem(IDC_STATIC_ACC)->EnableWindow(FALSE);
		GetDlgItem(IDC_STATIC_TIME)->EnableWindow(FALSE);
		GetDlgItem(IDC_STATIC_S)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_PARA)->EnableWindow(FALSE);

		GetDlgItem(IDC_EDIT_X_LEFT)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_X_RIGHT)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_EDIT_TIME)->EnableWindow(TRUE);
		GetDlgItem(IDC_CHECK_ACC)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_SET2)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_SET)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_ACC)->EnableWindow(TRUE);
		GetDlgItem(IDC_STATIC_ACC)->EnableWindow(TRUE);
		GetDlgItem(IDC_STATIC_TIME)->EnableWindow(TRUE);
		GetDlgItem(IDC_STATIC_S)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_PARA)->EnableWindow(TRUE);

		GetDlgItem(IDC_EDIT_X_LEFT)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_X_RIGHT)->EnableWindow(TRUE);
	}

	
}

//  间隔采样时，点击 停止采样触发事件
void CDlgTestCheckIN::OnBnClickedButtonStop()
{

	m_bAutoRunning=FALSE;
	Sleep(1000);
	if (m_Handle!=NULL)
	{
		CloseHandle(m_Handle);
	}
	m_Handle=NULL;
	m_bStop = true;
}


void CDlgTestCheckIN::OnTimer(UINT_PTR nIDEvent)
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
			OnBnClickedButtonStop();
		}	
	}
	else if(nIDEvent==2)
	{
		CINScanLiteDlg * pDlg=(CINScanLiteDlg*)AfxGetApp()->GetMainWnd();

		if (m_bAutoRunning==FALSE&&m_bAutoRunningStatue==FALSE)
		{
			KillTimer(2);
			/*pDlg->SetBtnEnable(TRUE);
			BtnEnable(TRUE);*/
			pDlg->m_DlgShow.OnAutomatic();
			pDlg->m_DlgShow.OnSetAutoRepaint(TRUE);
		}	
	}

	CDialog::OnTimer(nIDEvent);
}
BOOL CDlgTestCheckIN::PreTranslateMessage(MSG* pMsg)
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

//  文件信息预设  按钮触发事件
void CDlgTestCheckIN::OnBnClickedButtonPara()
{
	CFileInfo dlg;
	dlg.DoModal();
}

//   点击累计停止触发事件
void CDlgTestCheckIN::OnBnClickedCheckAcc()
{
	sINInfo.bCheak=((CButton*)GetDlgItem(IDC_CHECK_ACC))->GetCheck();	
	SetCheckBk(sINInfo.bCheak);
}

//   次数的确定按钮触发事件
void CDlgTestCheckIN::OnBnClickedButtonSet()
{
	SetButtonState(&m_BtnSet);
	UpdateData(TRUE);
	sINInfo.AccNum=m_EditAcc;// 间隔里的累计次数
	if (sINInfo.AccNum<=0)
	{
		sINInfo.AccNum=1;
		m_EditAcc=1;
		UpdateData(FALSE);
	}
}

//  间隔时间的确定按钮触发事件
void CDlgTestCheckIN::OnBnClickedButtonSet2()
{
	SetButtonState(&m_BtnSet2);
	UpdateData(TRUE);
	sINInfo.Time=m_EditTime;//  间隔里的积分时间
	if (sINInfo.Time<=0)
	{
		sINInfo.Time=1;
		sINInfo.Time=1;
		UpdateData(FALSE);
	}

	CString str1, str2;
	GetDlgItem(IDC_EDIT_X_LEFT)->GetWindowText(str1);
	double temp1 = atof(str1);
	GetDlgItem(IDC_EDIT_X_RIGHT)->GetWindowText(str2);
	double temp2 = atof(str2);
	if(temp1 >= temp2)
	{
		CINScanLiteDlg * pDlg=(CINScanLiteDlg*)AfxGetApp()->GetMainWnd();
		MessageBox(pDlg->LoadStringMeau(_T("ID_RANGE_ERROR")));
		return;
	}
	sSInfo.XLeft=atof(str1);
	sSInfo.XRight=atof(str2);

}

void CDlgTestCheckIN::OnEnChangeEditTime()
{
	SetCButtonState(&m_BtnSet2);
}


void CDlgTestCheckIN::OnEnChangeEditAcc()
{
    SetCButtonState(&m_BtnSet);
}

