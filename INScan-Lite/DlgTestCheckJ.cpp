// DlgTestCheckJ.cpp : 实现文件
//

#include "stdafx.h"
#include "INScan-Lite.h"
#include "DlgTestCheckJ.h"
#include "afxdialogex.h"
#include "INScan-LiteDlg.h"

#include "..\file\TeeChart2011\series.h"


// CDlgTestCheckJ 对话框
DWORD WINAPI  ThreadGetDataD(LPVOID lpParam);
DWORD WINAPI  ThreadGetDataL(LPVOID lpParam);

DWORD WINAPI  ThreadGetDataD1064(LPVOID lpParam);//  lcz new 23/05/12
DWORD WINAPI  ThreadGetDataL1064(LPVOID lpParam);//  lcz new 23/05/12

DWORD WINAPI  ThreadGetDataDA1(LPVOID lpParam);
DWORD WINAPI  ThreadGetDataLA1(LPVOID lpParam);

DWORD WINAPI  ThreadGetData_D(LPVOID lpParam);
DWORD WINAPI  ThreadGetData_L(LPVOID lpParam);



IMPLEMENT_DYNAMIC(CDlgTestCheckJ, CDialog)

CDlgTestCheckJ::CDlgTestCheckJ(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgTestCheckJ::IDD, pParent)
	, m_EditK(0)
{
	m_bStop = false;
}

CDlgTestCheckJ::~CDlgTestCheckJ()
{
	if (pMiniCamera!=nullptr&&::IsWindow(pMiniCamera->m_hWnd))
	{
		pMiniCamera->OnClose();
	}
	delete pMiniCamera;
}

void CDlgTestCheckJ::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_L1, m_BtnL1);
	DDX_Control(pDX, IDC_BUTTON_L5, m_BtnL5);
	DDX_Control(pDX, IDC_BUTTON_R1, m_BtnR1);
	DDX_Control(pDX, IDC_BUTTON_R5, m_BtnR5);
	DDX_Control(pDX, IDC_BUTTON_D, m_BtnSingle);
	DDX_Control(pDX, IDC_BUTTON_L, m_BtnFollow);
	DDX_Control(pDX, IDC_BUTTON_STOP, m_BtnStop);
	DDX_Control(pDX, IDC_BUTTON_SET, m_BtnSet);
	DDX_Control(pDX, IDC_BUTTON_KSET, m_BtnKSet);
	DDX_Control(pDX, IDC_BUTTON_LED, m_BtnLED);

	DDX_Text(pDX, IDC_EDIT_STEP, m_EditLaser);
	DDX_Text(pDX, IDC_EDIT_TIME, m_EditTime);
	DDX_Text(pDX, IDC_EDIT_ACC, m_EditAcc);
	DDX_Text(pDX, IDC_EDIT_K, m_EditK);
}


BEGIN_MESSAGE_MAP(CDlgTestCheckJ, CDialog)
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BUTTON_L5, &CDlgTestCheckJ::OnBnClickedButtonL5)
	ON_BN_CLICKED(IDC_BUTTON_L1, &CDlgTestCheckJ::OnBnClickedButtonL1)
	ON_BN_CLICKED(IDC_BUTTON_R1, &CDlgTestCheckJ::OnBnClickedButtonR1)
	ON_BN_CLICKED(IDC_BUTTON_R5, &CDlgTestCheckJ::OnBnClickedButtonR5)
	ON_EN_CHANGE(IDC_EDIT_TIME, &CDlgTestCheckJ::OnEnChangeEditTime)
	ON_BN_CLICKED(IDC_BUTTON_SET, &CDlgTestCheckJ::OnBnClickedButtonSet)
	ON_EN_CHANGE(IDC_EDIT_ACC, &CDlgTestCheckJ::OnEnChangeEditAcc)
	ON_BN_CLICKED(IDC_CHECK_BK, &CDlgTestCheckJ::OnBnClickedCheckBk)
	ON_EN_CHANGE(IDC_EDIT_STEP, &CDlgTestCheckJ::OnEnChangeEditStep)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_P, &CDlgTestCheckJ::OnNMReleasedcaptureSliderP)

	ON_EN_CHANGE(IDC_EDIT_K, &CDlgTestCheckJ::OnEnChangeEditK)
	ON_BN_CLICKED(IDC_BUTTON_KSET, &CDlgTestCheckJ::OnBnClickedButtonKset)
	ON_BN_CLICKED(IDC_BUTTON_D, &CDlgTestCheckJ::OnBnClickedButtonD)
	ON_BN_CLICKED(IDC_BUTTON_STOP, &CDlgTestCheckJ::OnBnClickedButtonStop)
	ON_BN_CLICKED(IDC_BUTTON_L, &CDlgTestCheckJ::OnBnClickedButtonL)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_LED, &CDlgTestCheckJ::OnBnClickedButtonLed)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_P, &CDlgTestCheckJ::OnNMCustomdrawSliderP)
	ON_BN_CLICKED(IDC_BTN_AUTOTEST, &CDlgTestCheckJ::OnBnClickedBtnAutotest)
	ON_BN_CLICKED(IDC_BTN_STOPAUTO, &CDlgTestCheckJ::OnBnClickedBtnStopauto)
	ON_BN_CLICKED(IDC_BTN_TEST_CAMERA, &CDlgTestCheckJ::OnBnClickedBtnTestCamera)
	ON_BN_CLICKED(IDC_BTN_STOP_CAMERA, &CDlgTestCheckJ::OnBnClickedBtnStopCamera)
	
END_MESSAGE_MAP()


// CDlgTestCheckJ 消息处理程序


BOOL CDlgTestCheckJ::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_FontChinese.CreatePointFont(nFontSize,strFontChinese);
	m_FontEnglish.CreatePointFont(nFontSizeEH,strFontEngish);

	SetData();
	SetControlFont();
	SetCheckBk(sTInfo.JBack);
	SetLedButton(sSInfo.CheckLed);
	((CButton*)GetDlgItem(IDC_CHECK_BK))->SetCheck(sTInfo.JBack);	

	// lcz new 22/10/21
	if(sTInfo.AutoTest)
	{
		GetDlgItem(IDC_BTN_STOPAUTO)->ShowWindow(true);
		GetDlgItem(IDC_BTN_AUTOTEST)->ShowWindow(true);
	}
	else
	{
		GetDlgItem(IDC_BTN_STOPAUTO)->ShowWindow(false);
		GetDlgItem(IDC_BTN_AUTOTEST)->ShowWindow(false);
	}


	m_Handle[0]=NULL;
	m_Handle[1]=NULL;
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
void CDlgTestCheckJ::SetData()
{
	
	((CSliderCtrl *)GetDlgItem(IDC_SLIDER_P))->SetRange(0,100);
	((CSliderCtrl *)GetDlgItem(IDC_SLIDER_P))->SetPageSize(1);
	((CSliderCtrl *)GetDlgItem(IDC_SLIDER_P))->SetPos(sTInfo.JPower);
	m_EditAcc=sTInfo.JAccNum;
	m_EditLaser=sTInfo.JPower;
	m_EditTime=sTInfo.JTime;
	m_EditK=sTInfo.JBkK;
	//  lcz new
	CString str;
	str.Format(_T("%d"), (int)sSInfo.XLeft);
	GetDlgItem(IDC_EDIT_XJ_LEFT)->SetWindowText(str);
	str.Format(_T("%d"), (int)sSInfo.XRight);
	GetDlgItem(IDC_EDIT_XJ_RIGHT)->SetWindowText(str);

	
	UpdateData(FALSE);

	SetButtonState(&m_BtnL5);
	SetButtonState(&m_BtnL1);
	SetButtonState(&m_BtnR5);
	SetButtonState(&m_BtnR1);
	SetButtonState(&m_BtnSingle);
	SetButtonState(&m_BtnFollow);
	SetButtonState(&m_BtnStop);
	SetButtonState(&m_BtnSet);
	SetButtonState(&m_BtnKSet);
	SetButtonState(&m_BtnLED);

	pMiniCamera = new CMiniCamera();
}
void CDlgTestCheckJ::SetControlFont()
{
	CFont *m_Font;

	if (sInfo.nLanguage==1)
		m_Font=&m_FontEnglish;
	else
		m_Font=&m_FontChinese;

	GetDlgItem(IDC_BUTTON_L1)->SetFont(m_Font);
	GetDlgItem(IDC_BUTTON_L5)->SetFont(m_Font);
	GetDlgItem(IDC_BUTTON_R1)->SetFont(m_Font);
	GetDlgItem(IDC_BUTTON_R5)->SetFont(m_Font);
	GetDlgItem(IDC_BUTTON_L)->SetFont(m_Font);
	GetDlgItem(IDC_BUTTON_D)->SetFont(m_Font);
	GetDlgItem(IDC_BUTTON_STOP)->SetFont(m_Font);
	GetDlgItem(IDC_BUTTON_SET)->SetFont(m_Font);
	GetDlgItem(IDC_BUTTON_KSET)->SetFont(m_Font);
	GetDlgItem(IDC_BUTTON_LED)->SetFont(m_Font);
	
	GetDlgItem(IDC_STATIC_LA)->SetFont(m_Font);
	GetDlgItem(IDC_STATIC_S)->SetFont(m_Font);
	GetDlgItem(IDC_STATIC_TIME)->SetFont(m_Font);
	GetDlgItem(IDC_STATIC_ACC)->SetFont(m_Font);
	GetDlgItem(IDC_STATIC_0)->SetFont(m_Font);
	GetDlgItem(IDC_STATIC_100)->SetFont(m_Font);
	GetDlgItem(IDC_STATIC_K)->SetFont(m_Font);
	GetDlgItem(IDC_CHECK_BK)->SetFont(m_Font);
	GetDlgItem(IDC_EDIT_STEP)->SetFont(m_Font);
	GetDlgItem(IDC_EDIT_TIME)->SetFont(m_Font);
	GetDlgItem(IDC_EDIT_ACC)->SetFont(m_Font);
	//  lcz new
	GetDlgItem(IDC_STATIC_XJRange)->SetFont(m_Font);
	GetDlgItem(IDC_EDIT_XJ_LEFT)->SetFont(m_Font);
	GetDlgItem(IDC_EDIT_XJ_RIGHT)->SetFont(m_Font);
	GetDlgItem(IDC_STATIC_J_SYMBOL)->SetFont(m_Font);
}


void CDlgTestCheckJ::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	CRect rect;
	GetClientRect(rect);
	dc.FillSolidRect(rect,clr);
	CDialog::OnPaint();
}


HBRUSH CDlgTestCheckJ::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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
void CDlgTestCheckJ::UpdateLanguage()
{
	CINScanLiteDlg * pDlg=(CINScanLiteDlg*)AfxGetApp()->GetMainWnd();
	pDlg->m_ZLanguage.SetCreateLanguageState(FALSE);
	pDlg->m_ZLanguage.SetDialogStrings((long)(CDialog*)this,IDD);

	SetControlFont();
}

// 功率百分比减5
void CDlgTestCheckJ::OnBnClickedButtonL5()
{
	SetCButtonState(&m_BtnSet);
	m_EditLaser-=sTInfo.JowerB;
	if (m_EditLaser<0)
		m_EditLaser=0;
	UpdateData(FALSE);
}


void CDlgTestCheckJ::OnBnClickedButtonL1()
{
	SetCButtonState(&m_BtnSet);
	m_EditLaser-=sTInfo.JPowerL;
	if (m_EditLaser<0)
		m_EditLaser=0;
	UpdateData(FALSE);
}


void CDlgTestCheckJ::OnBnClickedButtonR1()
{
	SetCButtonState(&m_BtnSet);
	m_EditLaser+=sTInfo.JPowerL;
	if (m_EditLaser>100)
		m_EditLaser=100;
	UpdateData(FALSE);
}


void CDlgTestCheckJ::OnBnClickedButtonR5()
{
	SetCButtonState(&m_BtnSet);
	m_EditLaser+=sTInfo.JowerB;
	if (m_EditLaser>100)
		m_EditLaser=100;
	UpdateData(FALSE);
}

// 积分时间改变时触发事件，改变“确定”按钮的颜色和状态
void CDlgTestCheckJ::OnEnChangeEditTime()
{
	SetCButtonState(&m_BtnSet);
}

//  积分时间和累计次数的确定按钮触发事件
void CDlgTestCheckJ::OnBnClickedButtonSet()
{
	

	SetButtonState(&m_BtnSet);
	UpdateData(TRUE);

	CINScanLiteDlg * pDlg=(CINScanLiteDlg*)AfxGetApp()->GetMainWnd();

//	if (m_EditTime>sTInfo.KOutTime)
//	{
//		m_EditTime=sTInfo.KOutTime;
//	}
//	if (m_EditTime<sTInfo.KOutTimeLow)
//	{
//		m_EditTime=sTInfo.KOutTimeLow;
//	}
	sTInfo.JTime=m_EditTime;//  精检积分时间

	
	if (m_EditAcc >= 1){
		sTInfo.JAccNum = m_EditAcc;//  累计次数
	}
	else {
		
		sTInfo.JAccNum = 0;
		AfxMessageBox("累计次数需大于等于1");
		return;
	}
	
	sTInfo.JPower=m_EditLaser;//  占空比

	CString str1, str2;
	GetDlgItem(IDC_EDIT_XJ_LEFT)->GetWindowText(str1);
	double temp1 = atof(str1);
	GetDlgItem(IDC_EDIT_XJ_RIGHT)->GetWindowText(str2);
	double temp2 = atof(str2);
	if(temp1 >= temp2)
	{
		MessageBox(pDlg->LoadStringMeau(_T("ID_RANGE_ERROR")));
		return;
	}

	sSInfo.XLeft=atof(str1);
	sSInfo.XRight=atof(str2);

	if (m_bAutoRunning == TRUE)
	{
		pDlg->m_DlgShow.SetLawerPower(sTInfo.JPower, TRUE);
	}
	if (sInfo.LConnect == 1 && sInfo.CConnect == 0)
	{  //only激光器连接时,开激光
		WriteToLog("Laser01--(set)单连激光器必须走的分支Lconnect = 1,cconnect = 0");
		pDlg->m_DlgSet.m_Serial.SetVolt(sSInfo.nCOM, _T("LDON")); //先打开开关
		pDlg->m_DlgShow.SetLawerPower(sTInfo.JPower, TRUE);       //再设置功率
	}


	((CSliderCtrl *)GetDlgItem(IDC_SLIDER_P))->SetPos(m_EditLaser);
	UpdateData(FALSE);
}
//void CDlgTestCheckJ::SetLaserVolt(CINScanLiteDlg* pDlg) {
//
//	if (sInfo.LConnect == 1 && sInfo.CConnect == 1 && m_bAutoRunning == false)//2025
//	{
//		pDlg->m_DlgShow.SetLawerPower(sTInfo.JPower, TRUE);
//	}
//
//}
// 累计次数改变时触发事件，改变“确定”按钮的颜色和状态
void CDlgTestCheckJ::OnEnChangeEditAcc()
{
	SetCButtonState(&m_BtnSet);
}

//  点击“启用背景扣除”触发事件
void CDlgTestCheckJ::OnBnClickedCheckBk()
{
	sTInfo.JBack=((CButton*)GetDlgItem(IDC_CHECK_BK))->GetCheck();	
	SetCheckBk(sTInfo.JBack);
}

//  根据是否选择背景扣除来显示或隐藏控件
void CDlgTestCheckJ::SetCheckBk(int nBk)
{

	GetDlgItem(IDC_EDIT_K)->ShowWindow(nBk);
	GetDlgItem(IDC_STATIC_K)->ShowWindow(nBk);
	GetDlgItem(IDC_STATIC_KK)->ShowWindow(nBk);
	GetDlgItem(IDC_BUTTON_KSET)->ShowWindow(nBk);

}

//  百分比功率编辑框内容改变触发事件
void CDlgTestCheckJ::OnEnChangeEditStep()
{
	SetCButtonState(&m_BtnSet);
	if (m_EditLaser<0)
		m_EditLaser=0;
	if (m_EditLaser>100)
		m_EditLaser=100;
}

//   拖动滑片时触发事件，读取百分比
void CDlgTestCheckJ::OnNMReleasedcaptureSliderP(NMHDR *pNMHDR, LRESULT *pResult)
{
	CINScanLiteDlg * pDlg=(CINScanLiteDlg*)AfxGetApp()->GetMainWnd();

	m_EditLaser = ((CSliderCtrl *)GetDlgItem(IDC_SLIDER_P))->GetPos();
	sTInfo.JPower=m_EditLaser;
	//  lcz new 07/21 原先是测试状态或者打开相机，就可以调节激光
	if (m_bAutoRunning==TRUE)
	// if (m_bAutoRunning==TRUE||::IsWindow(pMiniCamera->m_hWnd))
	{
		pDlg->m_DlgShow.SetLawerPower(sTInfo.JPower,TRUE);
	}
	UpdateData(FALSE);
	*pResult = 0;
}

//  背景扣除的 k 值修改时触发事件，改变确定按钮状态
void CDlgTestCheckJ::OnEnChangeEditK()
{
	SetCButtonState(&m_BtnKSet);
}

//  启用背景扣除 里的确定按钮触发事件
void CDlgTestCheckJ::OnBnClickedButtonKset()
{
	SetButtonState(&m_BtnKSet);
	UpdateData(TRUE);
	sTInfo.JBkK=m_EditK;
}


void CDlgTestCheckJ::SetAutoScanBtnStatus(bool bStatus)
{
	CINScanLiteDlg * pDlg=(CINScanLiteDlg*)AfxGetApp()->GetMainWnd(); 
	pDlg->SetBtnEnable(bStatus);
	BtnEnable(bStatus);
}

// 96孔板自动扫描检测，单谱测量
void CDlgTestCheckJ::AutoScan_96()
{
	CINScanLiteDlg * pDlg=(CINScanLiteDlg*)AfxGetApp()->GetMainWnd();
	if (::IsWindow(pMiniCamera->m_hWnd))
	{
		pMiniCamera->OnClose();
	}
	
	sTInfo.JDLMode=0;//  单谱检测
	m_bAutoRunning=TRUE;

	m_bAutoRunningStatue=TRUE;
	pDlg->SetBtnEnable(FALSE);
	//pDlg->m_DlgFile.m_DlgDataPro.ClearList();
	BtnEnable(FALSE);	
	DWORD dwThreadID;
	SetTimer(2,1000,NULL);
	m_Handle[sTInfo.JDLMode]=CreateThread(NULL,0,ThreadGetDataD,NULL,0,&dwThreadID);
}


//  精检里  单谱测量
void CDlgTestCheckJ::OnBnClickedButtonD()
{

	CINScanLiteDlg * pDlg=(CINScanLiteDlg*)AfxGetApp()->GetMainWnd();
	
	CString str1, str2;
	GetDlgItem(IDC_EDIT_XJ_LEFT)->GetWindowText(str1);
	double temp1 = atof(str1);
	GetDlgItem(IDC_EDIT_XJ_RIGHT)->GetWindowText(str2);
	double temp2 = atof(str2);
	if(temp1 >= temp2)
	{
		MessageBox(pDlg->LoadStringMeau(_T("ID_RANGE_ERROR")));
		return;
	}
	//2.点击设置和累计次数判定
	OnBnClickedButtonSet();
	if (sTInfo.JAccNum == 0) {
		return;
	}

	//3.发送LDOFF 或LDON
	WriteToLog("\n");
	pDlg->m_DlgSet.m_Serial.OpenLaserPower(sSInfo.nCOM);
	
	//4.判断现有的条数是否为maxLineNum,，如果已经存在maxLineNum条曲线，提示先删除部分曲线
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

	//5.温度判断
	int Temp=0;
	g_bGetTemp = true;
	//EnterCriticalSection( & g_cs_temp);
	
	if( sInfo.OminiType != 20 && sInfo.OminiType != 50)// 如果不是OTO的，才进行温度检测
	{
		//m_ZolixCCD.GetTemper(&Temp);// lcz new 01/31
		Sleep(100);
		//LeaveCriticalSection( & g_cs_temp);
		if(g_iTemp > -5)
		{
			g_bGetTemp = false;
			if(MessageBox(pDlg->LoadStringMeau(_T("ID_START_FAIL")),pDlg->LoadStringMeau(_T("ID_WARNING")),MB_OKCANCEL|MB_ICONWARNING) == IDCANCEL)
				return;
		}

	//6.重启ccd
		CDlgTest *test = (CDlgTest*)GetParent();
		if (!test->HasRestarted())
		{
			test->restartCCD();
		}
	}

	if (::IsWindow(pMiniCamera->m_hWnd))
	{
		pMiniCamera->OnClose();
	}
	
	sTInfo.JDLMode=0;
	m_bAutoRunning=TRUE;

	m_bAutoRunningStatue=TRUE;
	pDlg->SetBtnEnable(FALSE);
	////pDlg->m_DlgFile.m_DlgDataPro.ClearList();
	BtnEnable(FALSE);	

	//// lcz new 22/10/20  每次单谱采集前先设置记录变量为false，采集完后为true
	g_bSingleTestFinish = false;
	//
	DWORD dwThreadID;
	SetTimer(1,1000,NULL);
	 // lcz new 23/05/12  重新写的采集函数
	if (pDlg->m_b1064) {
		m_Handle[sTInfo.JDLMode] = CreateThread(NULL, 0, ThreadGetDataD1064, NULL, 0, &dwThreadID);
	}
	//if (sInfo.OminiType == 50) {
	//	m_Handle[sTInfo.JDLMode] = CreateThread(NULL, 0, ThreadGetDataDA1, NULL, 0, &dwThreadID);
	//}
	else {
		m_Handle[sTInfo.JDLMode] = CreateThread(NULL, 0, ThreadGetDataD, NULL, 0, &dwThreadID);
	}
}

 
void CDlgTestCheckJ::BtnEnable(BOOL bShow)
{

	GetDlgItem(IDC_BUTTON_L)->EnableWindow(bShow);
	GetDlgItem(IDC_BUTTON_D)->EnableWindow(bShow);
	GetDlgItem(IDC_CHECK_BK)->EnableWindow(bShow);
	//GetDlgItem(IDC_EDIT_K)->EnableWindow(bShow);
	//GetDlgItem(IDC_BUTTON_KSET)->EnableWindow(bShow);
	if (sTInfo.JDLMode==0)
	{
		GetDlgItem(IDC_BUTTON_SET)->EnableWindow(bShow);
		GetDlgItem(IDC_EDIT_XJ_LEFT)->EnableWindow(bShow);
		GetDlgItem(IDC_EDIT_XJ_RIGHT)->EnableWindow(bShow);
		GetDlgItem(IDC_EDIT_ACC)->EnableWindow(bShow);
		GetDlgItem(IDC_EDIT_TIME)->EnableWindow(bShow);

		if (sInfo.LConnect == 1 && sInfo.CConnect == 0 && m_bAutoRunning == false)
		{
			GetDlgItem(IDC_BUTTON_SET)->EnableWindow(TRUE);
		}
	}
	else
	{
		GetDlgItem(IDC_EDIT_XJ_LEFT)->EnableWindow(bShow);
		GetDlgItem(IDC_EDIT_XJ_RIGHT)->EnableWindow(bShow);
		GetDlgItem(IDC_EDIT_ACC)->EnableWindow(bShow);
	}
	if (/*sInfo.LConnect==1&&*/sInfo.CConnect==1)
		GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(!bShow);
	else
		GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(bShow);
	if(g_bUSBDiscon)
	{
		GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
	}


}

//  精检里，点击停止采样触发事件
void CDlgTestCheckJ::OnBnClickedButtonStop()
{
	m_bAutoRunning=FALSE;
	//GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
	//GetDlgItem(IDC_BUTTON_L)->EnableWindow(FALSE);
	//GetDlgItem(IDC_BUTTON_D)->EnableWindow(FALSE);
	Sleep(1500);
	if (m_Handle[sTInfo.JDLMode]!=NULL)
	{
		// lcz new 22/10/26
		//TerminateThread(m_Handle[sTInfo.JDLMode], 1);

		CloseHandle(m_Handle[sTInfo.JDLMode]);
	}
	m_Handle[sTInfo.JDLMode]=NULL;
	m_bStop = true;

	//BtnEnable(TRUE);	
}

//  精检里，点击“连续”采样触发事件
//void CDlgTestCheckJ::OnBnClickedButtonL()
//{
//	CINScanLiteDlg * pDlg=(CINScanLiteDlg*)AfxGetApp()->GetMainWnd();
//
//	CString str1, str2;
//	GetDlgItem(IDC_EDIT_XJ_LEFT)->GetWindowText(str1);
//	double temp1 = atof(str1);
//	GetDlgItem(IDC_EDIT_XJ_RIGHT)->GetWindowText(str2);
//	double temp2 = atof(str2);
//	if(temp1 >= temp2)
//	{
//		MessageBox(pDlg->LoadStringMeau(_T("ID_RANGE_ERROR")));
//		return;
//	}
//	// 每次测试，都自动点击“确定”按钮
//	OnBnClickedButtonSet();
//
//	pDlg->m_DlgSet.m_Serial.OpenLaserPower(sSInfo.nCOM);
//
//	if (sTInfo.JAccNum == 0) {
//		return;
//	}
//
//	//  判断现有的条数是否为maxLineNum,，如果已经存在maxLineNum条曲线，提示先删除部分曲线
//	int numOfLineShowed = 0;
//	for(int i = 0; i < pDlg->m_DlgShow.m_Tchart.GetSeriesCount(); i++)
//	{
//		if(pDlg->m_DlgShow.m_Tchart.Series(i).GetShowInLegend() == true)
//			numOfLineShowed++;
//	}
//	if(numOfLineShowed >= maxLineNum)
//	{
//		MessageBox(pDlg->LoadStringMeau(_T("ID_OUTOF_LINE_RANGE")));
//		return;
//	}//  lcz new 22/10/20
//
//	//pDlg->m_DlgShow.SetLawerPower(0, FALSE);
//	//static bool bReconnect = false;
//	//int Temp=0;
//	g_bGetTemp = true;
//	//EnterCriticalSection( & g_cs_temp);
//	// lcz new 05/31
//	if( sInfo.OminiType != 20) // 如果不是oto的谱仪，才进行读取和显示温度
//	{
//		//m_ZolixCCD.GetTemper(&Temp);// lcz new 01/31
//		Sleep(100);
//		//LeaveCriticalSection( & g_cs_temp);
//		//if(Temp > -5)
//		// lcz new 温度达到-10，却仍提示高于-5
//		if(g_iTemp > -5)
//		{
//			g_bGetTemp = false;
//			if(MessageBox(pDlg->LoadStringMeau(_T("ID_START_FAIL")),pDlg->LoadStringMeau(_T("ID_WARNING")),MB_OKCANCEL|MB_ICONWARNING) == IDCANCEL)
//				return;
//		}
//
//		CDlgTest *test = (CDlgTest*)GetParent();
//		if (!test->HasRestarted())
//		{
//			test->restartCCD();
//		}
//	}
//	// 窗口句柄标识了一个已存在的窗口，返回值为非零；如果窗口句柄未标识一个已存在窗口，返回值为零。
//	if (::IsWindow(pMiniCamera->m_hWnd))
//	{
//		pMiniCamera->OnClose();
//	}
//	sTInfo.JDLMode=1;
//	m_bAutoRunning=TRUE;
//	m_bAutoRunningStatue=TRUE;
//	pDlg->SetBtnEnable(FALSE);
//
//	//pDlg->m_DlgFile.m_DlgDataPro.ClearList();
//	BtnEnable(FALSE);	
//
//	// lcz new 22/10/20  每次单谱采集前先设置记录变量为false，采集完后为true
//	g_bSingleTestFinish = false;
//
//	DWORD dwThreadID;
//	SetTimer(1,1000,NULL);
//
//	//  lcz new 23/05/12  重新写的采集函数
//	if (pDlg->m_b1064)
//		m_Handle[sTInfo.JDLMode] = CreateThread(NULL, 0, ThreadGetDataL1064, NULL, 0, &dwThreadID);
//	/*if (sInfo.OminiType == 50) {
//		m_Handle[sTInfo.JDLMode] = CreateThread(NULL, 0, ThreadGetDataLA1, NULL, 0, &dwThreadID);
//	}*/
//	else
//		m_Handle[sTInfo.JDLMode]=CreateThread(NULL,0,ThreadGetDataL,NULL,0,&dwThreadID);
//	
//}
void CDlgTestCheckJ::OnBnClickedButtonL()
{
	CINScanLiteDlg * pDlg = (CINScanLiteDlg*)AfxGetApp()->GetMainWnd();

	CString str1, str2;
	GetDlgItem(IDC_EDIT_XJ_LEFT)->GetWindowText(str1);
	double temp1 = atof(str1);
	GetDlgItem(IDC_EDIT_XJ_RIGHT)->GetWindowText(str2);
	double temp2 = atof(str2);
	if (temp1 >= temp2)
	{
		MessageBox(pDlg->LoadStringMeau(_T("ID_RANGE_ERROR")));
		return;
	}
	// 每次测试，都自动点击“确定”按钮
	OnBnClickedButtonSet();
	WriteToLog("\n");
	// 开启
	pDlg->m_DlgSet.m_Serial.OpenLaserPower(sSInfo.nCOM);

	//  判断现有的条数是否为maxLineNum,，如果已经存在maxLineNum条曲线，提示先删除部分曲线
	int numOfLineShowed = 0;
	for (int i = 0; i < pDlg->m_DlgShow.m_Tchart.GetSeriesCount(); i++)
	{
		if (pDlg->m_DlgShow.m_Tchart.Series(i).GetShowInLegend() == true)
			numOfLineShowed++;
	}
	if (numOfLineShowed >= maxLineNum)
	{
		MessageBox(pDlg->LoadStringMeau(_T("ID_OUTOF_LINE_RANGE")));
		return;
	}//  lcz new 22/10/20

	 //pDlg->m_DlgShow.SetLawerPower(0, FALSE);
	 //static bool bReconnect = false;
	 //int Temp=0;
	g_bGetTemp = true;
	//EnterCriticalSection( & g_cs_temp);
	// lcz new 05/31
	if (sInfo.OminiType != 20 && sInfo.OminiType!=50) // 如果不是oto的谱仪，才进行读取和显示温度
	{
		//m_ZolixCCD.GetTemper(&Temp);// lcz new 01/31
		Sleep(100);
		//LeaveCriticalSection( & g_cs_temp);
		//if(Temp > -5)
		// lcz new 温度达到-10，却仍提示高于-5
		if (g_iTemp > -5)
		{
			g_bGetTemp = false;
			if (MessageBox(pDlg->LoadStringMeau(_T("ID_START_FAIL")), pDlg->LoadStringMeau(_T("ID_WARNING")), MB_OKCANCEL | MB_ICONWARNING) == IDCANCEL)
				return;
		}

		CDlgTest *test = (CDlgTest*)GetParent();
		if (!test->HasRestarted())
		{
			test->restartCCD();
		}
	}
	// 窗口句柄标识了一个已存在的窗口，返回值为非零；如果窗口句柄未标识一个已存在窗口，返回值为零。
	if (::IsWindow(pMiniCamera->m_hWnd))
	{
		pMiniCamera->OnClose();
	}
	sTInfo.JDLMode = 1;
	m_bAutoRunning = TRUE;
	m_bAutoRunningStatue = TRUE;
	pDlg->SetBtnEnable(FALSE);

	//pDlg->m_DlgFile.m_DlgDataPro.ClearList();
	BtnEnable(FALSE);

	// lcz new 22/10/20  每次单谱采集前先设置记录变量为false，采集完后为true
	g_bSingleTestFinish = false;

	DWORD dwThreadID;
	SetTimer(1, 1000, NULL);

	//  lcz new 23/05/12  重新写的采集函数
	if (pDlg->m_b1064)
		m_Handle[sTInfo.JDLMode] = CreateThread(NULL, 0, ThreadGetDataL1064, NULL, 0, &dwThreadID);
	else
		m_Handle[sTInfo.JDLMode] = CreateThread(NULL, 0, ThreadGetDataL, NULL, 0, &dwThreadID);

}

void CDlgTestCheckJ::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent==1)
	{
		CINScanLiteDlg * pDlg=(CINScanLiteDlg*)AfxGetApp()->GetMainWnd();
		if(g_bUSBDiscon)
		{
			m_bAutoRunning==FALSE;
			m_bAutoRunningStatue==FALSE;
		}
		if (m_bAutoRunning==FALSE&&m_bAutoRunningStatue==FALSE)
		{
			//Sleep(500);
			KillTimer(1);
			//  如果是异常断开的，则不将测试按钮恢复
			//if(!g_bUSBDiscon)
			//{
				pDlg->SetBtnEnable(TRUE);

				if(g_bUSBDiscon)
					BtnEnable(FALSE);
				else
					BtnEnable(TRUE);
			//}
			pDlg->m_DlgShow.OnAutomatic();
			pDlg->m_DlgShow.OnSetAutoRepaint(TRUE);
			pDlg->WriteDeviceInfo();
			//OnBnClickedButtonStop();  //  停止采样时，会重复触发“停止测试”事件
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

	else if(nIDEvent==120)
	{
		if(g_bSingleTestFinish)
		{
			CINScanLiteDlg * pDlg=(CINScanLiteDlg*)AfxGetApp()->GetMainWnd();
			int numOfLineShowed = 0;
			for(int i = 0; i < pDlg->m_DlgShow.m_Tchart.GetSeriesCount(); i++)
			{
				if(pDlg->m_DlgShow.m_Tchart.Series(i).GetShowInLegend() == true)
					numOfLineShowed++;
			}
			if(numOfLineShowed >= maxLineNum)
			{
				pDlg->OnDelAll();
			}
			OnBnClickedButtonD();
		}
	}
	//  lcz new 03/16 测试相机
	else if (nIDEvent == 180)
	{
		CINScanLiteDlg * pDlg = (CINScanLiteDlg*)AfxGetApp()->GetMainWnd();
		int numOfLineShowed = 0;
		for (int i = 0; i < pDlg->m_DlgShow.m_Tchart.GetSeriesCount(); i++)
		{
			if (pDlg->m_DlgShow.m_Tchart.Series(i).GetShowInLegend() == true)
				numOfLineShowed++;
		}
		if (numOfLineShowed >= maxLineNum)
		{
			pDlg->OnDelAll();
		}
		OnBnClickedButtonL();
		Sleep(2000);
		OnBnClickedButtonLed();
		Sleep(3000);
		pMiniCamera->OnClose();
		OnBnClickedButtonStop();
		
	}


	CDialog::OnTimer(nIDEvent);
}

//  键盘触发事件（具体为空，可修改）
BOOL CDlgTestCheckJ::PreTranslateMessage(MSG* pMsg)
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

//  打开相机触发事件
void CDlgTestCheckJ::OnBnClickedButtonLed()
{
	//  lcz new
	//  在点击“打开相机”前若已开激光测试，则直接打开相机，不改变激光开关
	//  若打开相机前已停止测试，则点击“打开相机”时先将激光关闭
	CINScanLiteDlg * pDlg=(CINScanLiteDlg*)AfxGetApp()->GetMainWnd();
	//if(m_bAutoRunning == FALSE)
	//{
	//	//pDlg->m_DlgShow.SetLawerPower(0, FALSE);
	//}
	//else
	//{
	//	pDlg->m_DlgShow.SetLawerPower(0, TRUE);
	//}
	//int i=60;
	double x=1.0;
	for (int i=1;i<=60;i++)
	{
		x=x*(16.0/12.0*0.01+1);
	}
	x=x*1500;

	// 改变标志位，表示相机打开
	g_bOpenCamera = true;
	g_bCameraOpen2Close = false;// 由false 到 true 表示相机由开到关
	//Sleep(500);
	//  如果相机界面窗口已打开，则返回
	if (::IsWindow(pMiniCamera->m_hWnd))
	{
		return;
	}
	else
	{
		// 点击打开相机，采谱按钮变为不可用
		GetDlgItem(IDC_BUTTON_D)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_L)->EnableWindow(FALSE);
		pDlg->m_DlgTest.m_DlgTestCheckK.GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);

		pMiniCamera = new CMiniCamera(this);
		pMiniCamera->Create(IDD_MINI_CAMERA,this);
		pMiniCamera->ShowWindow(SW_SHOW);
		pMiniCamera->CenterWindow();
	}
}


void CDlgTestCheckJ::OnNMCustomdrawSliderP(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
}

void CDlgTestCheckJ::SetLedButton(bool bS)
{
	GetDlgItem(IDC_BUTTON_LED)->ShowWindow(bS);
}

void CDlgTestCheckJ::OnBnClickedBtnAutotest()
{
	// TODO: 在此添加控件通知处理程序代码
	//  循环单谱采集，如果超过35条，删除所有重新采集
	SetTimer(120, 5000, NULL);

}


void CDlgTestCheckJ::OnBnClickedBtnStopauto()
{
	// TODO: 在此添加控件通知处理程序代码
	KillTimer(120);
}


void CDlgTestCheckJ::OnBnClickedBtnTestCamera()
{
	// TODO: 在此添加控件通知处理程序代码
	SetTimer(180, 35000, NULL);
	//testCamera = true;
	//while (testCamera)
	//{
	//	CINScanLiteDlg * pDlg = (CINScanLiteDlg*)AfxGetApp()->GetMainWnd();
	//	int numOfLineShowed = 0;
	//	for (int i = 0; i < pDlg->m_DlgShow.m_Tchart.GetSeriesCount(); i++)
	//	{
	//		if (pDlg->m_DlgShow.m_Tchart.Series(i).GetShowInLegend() == true)
	//			numOfLineShowed++;
	//	}
	//	if (numOfLineShowed >= maxLineNum)
	//	{
	//		pDlg->OnDelAll();
	//	}
	//	OnBnClickedButtonL();
	//	Sleep(2000);
	//	OnBnClickedButtonLed();
	//	Sleep(5000);
	//	pMiniCamera->OnClose();
	//	Sleep(2000);
	//	OnBnClickedButtonStop();
	//}
}


void CDlgTestCheckJ::OnBnClickedBtnStopCamera()
{
	// TODO: 在此添加控件通知处理程序代码
	KillTimer(180);
	//testCamera = false;
}

int CDlgTestCheckJ::WriteToLog(char* str)
{

	char exePath[MAX_PATH] = { 0 };

	GetModuleFileNameA(NULL, exePath, MAX_PATH);


	char* pSlash = strrchr(exePath, '\\');
	if (pSlash)
	{
		*pSlash = '\0';
	}

	char logFilePath[MAX_PATH] = { 0 };
	sprintf_s(logFilePath, "%s\\FI-PC-Log.txt", exePath);


	FILE* pfile = nullptr;
	errno_t err = fopen_s(&pfile, logFilePath, "a+");
	if (err != 0 || pfile == nullptr)
	{
		return -1;
	}


	SYSTEMTIME tm;
	GetLocalTime(&tm);

	char szDateTime[64] = { 0 };
	sprintf_s(szDateTime, "%02d:%02d:%02d.%03d",
		tm.wHour, tm.wMinute, tm.wSecond, tm.wMilliseconds);

	fprintf_s(pfile, "%s:%s\n", szDateTime, str);

	fclose(pfile);

	return 0;
}




