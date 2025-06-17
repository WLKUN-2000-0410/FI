// DlgSet.cpp : 实现文件
//

#include "stdafx.h"
#include "INScan-Lite.h"
#include "DlgSet.h"
#include "afxdialogex.h"
#include "INScan-LiteDlg.h"
#include "BxsOffset.h"
#include ".\Prior\PriorCtrl.h"
#include "DlgDccdSet.h"

//lcz
#include "resource.h"

// CDlgSet 对话框

IMPLEMENT_DYNAMIC(CDlgSet, CDialog)

CDlgSet::CDlgSet(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgSet::IDD, pParent)
	, iRadio(0)
	, m_bCloseLaser(true)
	, iUSBCameraID(0)
	, m_bnew_board(false)
	//, m_iReconCCD(0)
{

}

CDlgSet::~CDlgSet()
{
	// CCD断开连接
	m_ZolixCCD.DisConnect();
	// 激光器短开连接
	m_Serial.CloseLaser(sSInfo.nCOM);
	sInfo.LConnect = 0;
}

void CDlgSet::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_RESET, m_BtnPriorReset);
	DDX_Control(pDX, IDC_BUTTON_LDIS3, m_BtnPriorDis);
	DDX_Control(pDX, IDC_BUTTON_LCON3, m_BtnPriorCon);
	DDX_Control(pDX, IDC_BUTTON_LDIS, m_BtnLDis);
	DDX_Control(pDX, IDC_BUTTON_LCON, m_BtnLCon);
	DDX_Control(pDX, IDC_BUTTON_CDIS, m_BtnCDis);
	DDX_Control(pDX, IDC_BUTTON_CCON, m_BtnCCon);
	DDX_Control(pDX, IDC_BUTTON_PSET, m_BtnPSet);
	DDX_Control(pDX, IDC_BUTTON_SET, m_BtnSet);
	DDX_Control(pDX, IDC_BUTTON_RSET, m_BtnRSet);
	DDX_Control(pDX, IDC_BUTTON_IN, m_BtnIN);
	DDX_Control(pDX, IDC_BUTTON_CORR, m_BtnCorr);
	DDX_Control(pDX, IDC_BUTTON_CCD, m_BtnCCD);
	DDX_Control(pDX, IDC_BUTTON_UPDATA, m_BtnUpData);
	DDX_Radio(pDX, IDC_RADIO_H, iRadio);
	DDX_Control(pDX, IDC_BUTTON_OFFSET, m_BtnOffset);
	DDX_Control(pDX, IDC_BUTTON_SETLED, m_BtnSetLed);
	DDX_Control(pDX, IDC_BUTTON_LCON2, m_BtnLConLed);
	DDX_Control(pDX, IDC_BUTTON_LDIS2, m_BtnLDisLed);

	DDX_Control(pDX, IDC_BUTTON_OpenLaser, m_BtnOpenLaser);
	DDX_Control(pDX, IDC_BTN_CAM_SET1, m_cameraConfigBtn);


	// 相机
	DDX_Control(pDX, IDC_COMBO_LED, m_ComboLed);
}


BEGIN_MESSAGE_MAP(CDlgSet, CDialog)
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_RADIO_H, &CDlgSet::OnBnClickedRadioH)
	ON_BN_CLICKED(IDC_RADIO_L, &CDlgSet::OnBnClickedRadioL)
	ON_CBN_SELCHANGE(IDC_COMBO_COM, &CDlgSet::OnCbnSelchangeComboCom, TRUE)
	ON_BN_CLICKED(IDC_CHECK_BG, &CDlgSet::OnBnClickedCheckBg)
	ON_BN_CLICKED(IDC_BUTTON_RSET, &CDlgSet::OnBnClickedButtonRset)
	ON_BN_CLICKED(IDC_BUTTON_LCON, &CDlgSet::OnBnClickedButtonLcon)
	ON_BN_CLICKED(IDC_BUTTON_LDIS, &CDlgSet::OnBnClickedButtonLdis)
	ON_BN_CLICKED(IDC_BUTTON_CCON, &CDlgSet::OnBnClickedButtonCcon)
	ON_BN_CLICKED(IDC_BUTTON_CDIS, &CDlgSet::OnBnClickedButtonCdis)
	ON_BN_CLICKED(IDC_BUTTON_SET, &CDlgSet::OnBnClickedButtonSet)
	ON_BN_CLICKED(IDC_BUTTON_CCD, &CDlgSet::OnBnClickedButtonCcd)
	ON_BN_CLICKED(IDC_BUTTON_UPDATA, &CDlgSet::OnBnClickedButtonUpdata)

	ON_EN_CHANGE(IDC_EDIT_RIGHT, &CDlgSet::OnEnChangeEditRight)
	ON_EN_CHANGE(IDC_EDIT_LEFT, &CDlgSet::OnEnChangeEditLeft)
	ON_EN_CHANGE(IDC_EDIT_DELAY, &CDlgSet::OnEnChangeEditDelay)
	ON_BN_CLICKED(IDC_BUTTON_CORR, &CDlgSet::OnBnClickedButtonCorr)
	ON_BN_CLICKED(IDC_BUTTON_IN, &CDlgSet::OnBnClickedButtonIn)
	ON_BN_CLICKED(IDC_BUTTON_PSET, &CDlgSet::OnBnClickedButtonPset)
	ON_EN_CHANGE(IDC_EDIT_M, &CDlgSet::OnEnChangeEditM)
	ON_EN_CHANGE(IDC_EDIT_N, &CDlgSet::OnEnChangeEditN)
	ON_BN_CLICKED(IDC_BUTTON_OFFSET, &CDlgSet::OnBnClickedButtonOffset)
	ON_BN_CLICKED(IDC_CHECK_OFFSET, &CDlgSet::OnBnClickedCheckOffset)
	ON_BN_CLICKED(IDC_BUTTON_SETLED, &CDlgSet::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON_LCON2, &CDlgSet::OnBnClickedButtonLcon2)
	ON_BN_CLICKED(IDC_BUTTON_LDIS2, &CDlgSet::OnBnClickedButtonLdis2)
	ON_CBN_SELCHANGE(IDC_COMBO_COM2, &CDlgSet::OnCbnSelchangeComboCom2, TRUE)
	ON_BN_CLICKED(IDC_CHECK3, &CDlgSet::OnBnClickedCheck3)
	ON_CBN_SELCHANGE(IDC_COMBO4, &CDlgSet::OnCbnSelchangeCombo4)
	ON_BN_CLICKED(IDC_BUTTON_LCON3, &CDlgSet::OnBnClickedButtonLcon3)
	ON_CBN_SELCHANGE(IDC_COMBO_COM3, &CDlgSet::OnCbnSelchangeComboCom3, TRUE)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_LDIS3, &CDlgSet::OnBnClickedButtonLdis3)
	ON_BN_CLICKED(IDC_BUTTON_RESET, &CDlgSet::OnBnClickedButtonReset)
	ON_BN_CLICKED(IDC_BUTTON_OpenLaser, &CDlgSet::OnBnClickedButtonOpenlaser)
	ON_BN_CLICKED(IDC_RADIO_WAVELEN_out, &CDlgSet::OnBnClickedRadioWavelenout)
	ON_BN_CLICKED(IDC_RADIO_WAVENUM_out, &CDlgSet::OnBnClickedRadioWavenumout)
	ON_BN_CLICKED(IDC_BTN_CAM_SET1, &CDlgSet::OnBnClickedBtnCamSet1)


	// 相机
	ON_BN_CLICKED(IDC_CHECK_LED, &CDlgSet::OnBnClickedCheckLed)
	ON_CBN_SELCHANGE(IDC_COMBO_LED, &CDlgSet::OnCbnSelchangeComboLed)
	ON_BN_CLICKED(IDC_CHECK1, &CDlgSet::OnBnClickedCheck1)
	ON_STN_CLICKED(IDC_STATIC_COM2, &CDlgSet::OnStnClickedStaticCom2)
END_MESSAGE_MAP()


// CDlgSet 消息处理程序


BOOL CDlgSet::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_FontChinese.CreatePointFont(120,strFontChinese);
	m_FontEnglish.CreatePointFont(120,strFontEngish);
	
	InitCamera();

	CComboBox * pSelbox = (CComboBox *)GetDlgItem(IDC_COMBO4);
	pSelbox->SetCurSel(sSInfo.nSqlMode);

    SetData();
	OnBnState();	
	SetControlFont();
	UpdateData(FALSE);
	//SetTimer(100, 10, NULL);

	//  lcz new 23/05/17
	GetClientRect(&m_rect); //获取对话框的大小
	ScreenToClient(m_rect);
	i_dlg_width = m_rect.Width();
	i_dlg_height = m_rect.Height();

	((CButton*)GetDlgItem(IDC_CHECK1))->SetCheck(sSInfo.isCheckLed);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDlgSet::InitCamera()
{
	// 如果是自用版本 不要隐藏相机配置按钮
	int selfUse = GetPrivateProfileIntA("CAMERA", "self_use", 0, sInfo.strPath);
	if (selfUse != 1)
	{
		GetDlgItem(IDC_BTN_CAM_SET1)->ShowWindow(SW_HIDE);
	}
	CComboBox * pSelbox = (CComboBox *)GetDlgItem(IDC_COMBO_LED);
	m_vCameraInfo.clear();
	CDlgTestCheckJ * pDlg=(CDlgTestCheckJ*)GetParent();
	int NumId=0;
	NumId=pDlg->pMiniCamera->m_cap.EnumDevices (m_ComboLed);
	CString cameraName;
	m_ComboLed.ResetContent();
	map<int, CString>::iterator it = m_vCameraInfo.begin();
	bool bAlreadyFindCamera = false;
	//  lcz new 04/24  显示所有相机
	for (int i = 0; i < NumId; i++)
	{
		cameraName = it->second;
		m_ComboLed.InsertString(i, it->second);
		it++;
	}
	it = m_vCameraInfo.begin();
	for(int i = 0; i < NumId; i++)
	{
		cameraName = it->second;
		if(cameraName.Find(_T("2MP USB2.0 CAMERA")) != -1 
			|| cameraName.Find(_T("USB Video Device")) != -1 
			|| cameraName.Find(_T("500E")) != -1 
			|| (cameraName.Find(_T("usb camera")) != -1 
				|| cameraName.Find(_T("USB CAMERA")) != -1) && !bAlreadyFindCamera)// 表示找到了外部相机  //  lcz new 04/23  如果未找到相机：先判断名字里是否有500E，如果有，则认为已找到；如果没有，再判断名字里是否有usb camera，如果有，则认为已找到；
		{
			iUSBCameraID = i;
		
			if (cameraName.Find(_T("2MP")) >= 0&& selfUse!=1)
			{
				WritePrivateProfileStringA("CAMERA", "method", "xt", sInfo.strPath);
			}
			else
			{
				WritePrivateProfileStringA("CAMERA", "method", "opencv", sInfo.strPath);
			}
			char method[1024];
			GetPrivateProfileStringA("CAMERA", "method", "opencv", method, 1024, sInfo.strPath);

			


			// 不显示直接返回，便于调校
			int show = GetPrivateProfileInt("CAMERA", "show",  1, sInfo.strPath);
			if (show == 0)
			{
				break;
			}
			pSelbox->SetCurSel(sSInfo.ComboLed);
			// 500E直接返回
			if (strcmp(method, "500E") == 0)
			{
				::OutputDebugString("使用500E动态库方式进行加载相机！");
				break;
			}
			else if (strcmp(method, "xt") == 0)
			{
				::OutputDebugString("xt相机！");
				break;
			}
			// 否则就使用默认方式了
			else
			{
				CINScanLiteDlg * pDlgT = (CINScanLiteDlg*)AfxGetApp()->GetMainWnd();
				// 注意第一个参数必须对应着摄像头的id号，直接使用循环当中的i不可以
				bool b = pDlgT->cap.open(i, cv::CAP_DSHOW);//  去视频上边的黑边 lcz new 02/06
														   //pDlgT->cap.set(CV_CAP_PROP_FPS, 60);//lcz new 02/17 帧率 帧/秒
				if (!b)//  lcz new 04/23  如果打不开该com口，则认为没找到
				{
					g_bFindCamera = false;
					bAlreadyFindCamera = false;
				}
				else
				{
					sSInfo.ComboLed = i;
					g_bFindCamera = true;
					bAlreadyFindCamera = true;
					break;
				}

			}
		}
		it++;
	}
}

void CDlgSet::SetData()
{
		
	CComboBox * pSelbox;
	CString str;

	pSelbox = (CComboBox *)GetDlgItem(IDC_COMBO_OMNI);
	pSelbox->ResetContent();
	pSelbox->InsertString(0,sInfo.sDInfo.CCDName);
	pSelbox->SetCurSel(0);

	pSelbox = (CComboBox *)GetDlgItem(IDC_COMBO_COM);
	pSelbox->ResetContent();
	for(int x=1;x<80;x++)
	{
		str.Format("COM%d",x);
		pSelbox->InsertString(x-1,str);
	}
	pSelbox->SetCurSel(sSInfo.nCOM-1);

	pSelbox = (CComboBox *)GetDlgItem(IDC_COMBO_COM2);
	pSelbox->ResetContent();
	for(int x=1;x<80;x++)
	{
		str.Format("COM%d",x);
		pSelbox->InsertString(x-1,str);
	}
	pSelbox->SetCurSel(sSInfo.nCOMLED-1);

	pSelbox = (CComboBox *)GetDlgItem(IDC_COMBO_COM3);
	pSelbox->ResetContent();
	for(int x=1;x<80;x++)
	{
		str.Format("COM%d",x);
		pSelbox->InsertString(x-1,str);
	}
	pSelbox->SetCurSel(sSInfo.nComPrior-1);
	//////////////////////////////////////////////////

	pSelbox = (CComboBox *)GetDlgItem(IDC_COMBO4);
	pSelbox->SetCurSel(sSInfo.nSqlMode);
	//////////////////////////////////////////////////
	iRadio=sSInfo.HLMODE;
	//////////////////////////////////////////////////

	str.Format(_T("%d"),sSInfo.Delay);
	GetDlgItem(IDC_EDIT_DELAY)->SetWindowText(str);
	str.Format(_T("%.3f"),sSInfo.XLeft);
	GetDlgItem(IDC_EDIT_LEFT)->SetWindowText(str);
	str.Format(_T("%.3f"),sSInfo.XRight);
	GetDlgItem(IDC_EDIT_RIGHT)->SetWindowText(str);

	GetDlgItem(IDC_EDIT_M)->SetWindowText(sPInfo.Model);
	GetDlgItem(IDC_EDIT_N)->SetWindowText(sPInfo.Number);
	str.Format(_T("%d"),sSInfo.LockSet);
	GetDlgItem(IDC_EDIT1)->SetWindowText(str);

	((CButton*)GetDlgItem(IDC_CHECK_BG))->SetCheck(sSInfo.Back);
	((CButton*)GetDlgItem(IDC_CHECK_OFFSET))->SetCheck(sSInfo.CheckOffset);
	((CButton*)GetDlgItem(IDC_CHECK3))->SetCheck(sSInfo.isOEM);
	((CButton*)GetDlgItem(IDC_CHECK_LED))->SetCheck(sSInfo.CheckLed);
	SetButtonState(&m_BtnLCon);
	SetButtonState(&m_BtnLDis);
	SetButtonState(&m_BtnCDis);
	SetButtonState(&m_BtnCCon);
	SetButtonState(&m_BtnSet);
	SetButtonState(&m_BtnRSet);
	SetButtonState(&m_BtnIN);
	SetButtonState(&m_BtnCorr);
	SetButtonState(&m_BtnCCD);
	SetButtonState(&m_BtnUpData);
	SetButtonState(&m_BtnPSet);
	SetButtonState(&m_BtnOffset);
	SetButtonState(&m_BtnSetLed);
	SetButtonState(&m_BtnLConLed);
	SetButtonState(&m_BtnLDisLed);
	SetButtonState(&m_BtnPriorCon);
	SetButtonState(&m_BtnPriorDis);
	SetButtonState(&m_BtnPriorReset);

	SetButtonState(&m_BtnOpenLaser);
	SetButtonState(&m_cameraConfigBtn);
	SetButtonState(&m_cameraConfigBtn);
}
void CDlgSet::SetControlFont()
{
	CFont *m_Font;

	if (sInfo.nLanguage==1)
		m_Font=&m_FontEnglish;
	else
		m_Font=&m_FontChinese;


	GetDlgItem(IDC_STATIC_HW)->SetFont(m_Font);
	//GetDlgItem(IDC_STATIC_COM)->SetFont(m_Font);
	GetDlgItem(IDC_CHECK1)->SetFont(m_Font);
	GetDlgItem(IDC_CHECK3)->SetFont(m_Font);
	GetDlgItem(IDC_STATIC_LASER)->SetFont(m_Font);
	GetDlgItem(IDC_STATIC_CCD)->SetFont(m_Font);
	GetDlgItem(IDC_STATIC_OMNI)->SetFont(m_Font);
	GetDlgItem(IDC_STATIC_DELAY)->SetFont(m_Font);
	GetDlgItem(IDC_STATIC_RANGE)->SetFont(m_Font);
	GetDlgItem(IDC_STATIC_LEFT)->SetFont(m_Font);
	GetDlgItem(IDC_STATIC_RIGHT)->SetFont(m_Font);
	GetDlgItem(IDC_STATIC_PARA)->SetFont(m_Font);
	GetDlgItem(IDC_STATIC_M)->SetFont(m_Font);
	GetDlgItem(IDC_STATIC_N)->SetFont(m_Font);
	GetDlgItem(IDC_STATIC_HLMODE)->SetFont(m_Font);
	GetDlgItem(IDC_CHECK_OFFSET)->SetFont(m_Font);
	GetDlgItem(IDC_COMBO_LED)->SetFont(m_Font);
	GetDlgItem(IDC_CHECK_LED)->SetFont(m_Font);
	GetDlgItem(IDC_BUTTON_LCON)->SetFont(m_Font);
	GetDlgItem(IDC_BUTTON_LDIS)->SetFont(m_Font);
	GetDlgItem(IDC_BUTTON_CCON)->SetFont(m_Font);
	GetDlgItem(IDC_BUTTON_CDIS)->SetFont(m_Font);
	GetDlgItem(IDC_BUTTON_RSET)->SetFont(m_Font);
	GetDlgItem(IDC_BUTTON_SET)->SetFont(m_Font);
	GetDlgItem(IDC_BUTTON_IN)->SetFont(m_Font);
    GetDlgItem(IDC_BUTTON_CORR)->SetFont(m_Font);
	GetDlgItem(IDC_BUTTON_CCD)->SetFont(m_Font);
	GetDlgItem(IDC_BUTTON_UPDATA)->SetFont(m_Font);
	GetDlgItem(IDC_BUTTON_PSET)->SetFont(m_Font);
	GetDlgItem(IDC_BUTTON_OFFSET)->SetFont(m_Font);

	GetDlgItem(IDC_RADIO_H)->SetFont(m_Font);
	GetDlgItem(IDC_RADIO_L)->SetFont(m_Font);
	GetDlgItem(IDC_CHECK_BG)->SetFont(m_Font);

	GetDlgItem(IDC_STATIC_MS)->SetFont(m_Font);
	GetDlgItem(IDC_STATIC_CM1)->SetFont(m_Font);
	GetDlgItem(IDC_STATIC_CM2)->SetFont(m_Font);

	GetDlgItem(IDC_COMBO_COM)->SetFont(m_Font);
	GetDlgItem(IDC_COMBO_OMNI)->SetFont(m_Font);
	GetDlgItem(IDC_EDIT_LEFT)->SetFont(m_Font);
	GetDlgItem(IDC_EDIT_RIGHT)->SetFont(m_Font);
	GetDlgItem(IDC_EDIT_DELAY)->SetFont(m_Font);
	GetDlgItem(IDC_EDIT1)->SetFont(m_Font);
	GetDlgItem(IDC_BUTTON_SETLED)->SetFont(m_Font);
	GetDlgItem(IDC_STATIC_SETLED)->SetFont(m_Font);
	GetDlgItem(IDC_COMBO_COM2)->SetFont(m_Font);
	GetDlgItem(IDC_STATIC_COM2)->SetFont(m_Font);
	GetDlgItem(IDC_BUTTON_LCON2)->SetFont(m_Font);
	GetDlgItem(IDC_BUTTON_LDIS2)->SetFont(m_Font);
	GetDlgItem(IDC_STATIC_SQL)->SetFont(m_Font);
	GetDlgItem(IDC_COMBO4)->SetFont(m_Font);

	GetDlgItem(IDC_STATIC_COM3)->SetFont(m_Font);
	GetDlgItem(IDC_COMBO_COM3)->SetFont(m_Font);
	GetDlgItem(IDC_BUTTON_LCON3)->SetFont(m_Font);
	GetDlgItem(IDC_BUTTON_LDIS3)->SetFont(m_Font);
	GetDlgItem(IDC_BUTTON_RESET)->SetFont(m_Font);
	GetDlgItem(IDC_STATIC_COM4)->SetFont(m_Font);
	GetDlgItem(IDC_STATIC_LASER2)->SetFont(m_Font);
	GetDlgItem(IDC_STATIC_LASER3)->SetFont(m_Font);

	GetDlgItem(IDC_BUTTON_OpenLaser)->SetFont(m_Font);
	GetDlgItem(IDC_RADIO_WAVELEN_out)->SetFont(m_Font);
	GetDlgItem(IDC_RADIO_WAVENUM_out)->SetFont(m_Font);
	GetDlgItem(IDC_BTN_CAM_SET1)->SetFont(m_Font);
}


void CDlgSet::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	CRect rect;
	GetClientRect(rect);
	dc.FillSolidRect(rect,clr);
	CDialog::OnPaint();
}


HBRUSH CDlgSet::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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

void CDlgSet::UpdateLanguage()
{
	CINScanLiteDlg * pDlg=(CINScanLiteDlg*)GetParent();
	pDlg->m_ZLanguage.SetCreateLanguageState(FALSE);
	pDlg->m_ZLanguage.SetDialogStrings((long)(CDialog*)this,IDD);

	SetControlFont();
	m_ZolixCCD.SetLanguage(sInfo.nLanguage);
}



void CDlgSet::OnBnClickedRadioH()
{
	UpdateData(TRUE);
	sSInfo.HLMODE=iRadio;
	CString str;
	str.Format(TEXT("%d"), sSInfo.HLMODE);
	WritePrivateProfileString("SET", "HLMODE", str, sInfo.strPath);
}


void CDlgSet::OnBnClickedRadioL()
{
	UpdateData(TRUE);
	sSInfo.HLMODE=iRadio;
	CString str;
	str.Format(TEXT("%d"), sSInfo.HLMODE);
	WritePrivateProfileString("SET", "HLMODE", str, sInfo.strPath);
}


void CDlgSet::OnCbnSelchangeComboCom()
{
	sSInfo.nCOM=((CComboBox *)GetDlgItem(IDC_COMBO_COM))->GetCurSel()+1;
}


void CDlgSet::OnBnClickedCheckBg()
{
	sSInfo.Back=((CButton*)GetDlgItem(IDC_CHECK_BG))->GetCheck();
}


void CDlgSet::OnBnClickedButtonRset()
{
	CString str;
	GetDlgItem(IDC_EDIT_LEFT)->GetWindowText(str);
	sSInfo.XLeft=atof(str);
	GetDlgItem(IDC_EDIT_RIGHT)->GetWindowText(str);
	sSInfo.XRight=atof(str);
	SetButtonState(&m_BtnRSet);

}

int CDlgSet::WriteToLog_F(char* str)
{
	// 1) 先获取当前可执行文件的完整路径
	char exePath[MAX_PATH] = { 0 };
	// GetModuleFileNameA参数:
	//   第1个NULL表示获取当前进程可执行文件路径
	//   exePath数组用来接收
	//   MAX_PATH是缓冲区大小
	// 返回值是长度,但这里一般不再额外检查是否溢出
	GetModuleFileNameA(NULL, exePath, MAX_PATH);

	// 2) 截断掉最后的 '\\文件名.exe' 部分, 保留下目录路径
	char* pSlash = strrchr(exePath, '\\');
	if (pSlash)
	{
		*pSlash = '\0';  // 将最后一个反斜杠替换为字符串结束符
	}

	// 3) 在exe所在目录拼出日志文件名 "FILog.txt"
	char logFilePath[MAX_PATH] = { 0 };
	sprintf_s(logFilePath, "%s\\FI-PC-Log.txt", exePath);

	// 4) 以"w"模式打开,每次都覆盖之前的内容
	FILE* pfile = nullptr;
	errno_t err = fopen_s(&pfile, logFilePath, "w");
	if (err != 0 || pfile == nullptr)
	{
		return -1; // 打开失败
	}

	// 5) 获取当前时间,写到日志里
	SYSTEMTIME tm;
	GetLocalTime(&tm);

	char szDateTime[64] = { 0 };
	sprintf_s(szDateTime, "%02d:%02d:%02d.%03d",
		tm.wHour, tm.wMinute, tm.wSecond, tm.wMilliseconds);

	fprintf_s(pfile, "%s:%s\n", szDateTime, str);

	fclose(pfile);

	return 0;
}
int CDlgSet::WriteToLog(char* str)
{
	// 1) 先获取当前可执行文件的完整路径
	char exePath[MAX_PATH] = { 0 };
	// GetModuleFileNameA参数:
	//   第1个NULL表示获取当前进程可执行文件路径
	//   exePath数组用来接收
	//   MAX_PATH是缓冲区大小
	// 返回值是长度,但这里一般不再额外检查是否溢出
	GetModuleFileNameA(NULL, exePath, MAX_PATH);

	// 2) 截断掉最后的 '\\文件名.exe' 部分, 保留下目录路径
	char* pSlash = strrchr(exePath, '\\');
	if (pSlash)
	{
		*pSlash = '\0';  // 将最后一个反斜杠替换为字符串结束符
	}

	// 3) 在exe所在目录拼出日志文件名 "FILog.txt"
	char logFilePath[MAX_PATH] = { 0 };
	sprintf_s(logFilePath, "%s\\FI-PC-Log.txt", exePath);

	// 4) 以"w"模式打开,每次都覆盖之前的内容
	FILE* pfile = nullptr;
	errno_t err = fopen_s(&pfile, logFilePath, "a+");
	if (err != 0 || pfile == nullptr)
	{
		return -1; // 打开失败
	}

	// 5) 获取当前时间,写到日志里
	SYSTEMTIME tm;
	GetLocalTime(&tm);

	char szDateTime[64] = { 0 };
	sprintf_s(szDateTime, "%02d:%02d:%02d.%03d",
		tm.wHour, tm.wMinute, tm.wSecond, tm.wMilliseconds);

	fprintf_s(pfile, "%s:%s\n", szDateTime, str);

	fclose(pfile);

	return 0;
}


void CDlgSet::ConnectAll()
{

	//  lcz new 23/05/11  1064的探测器在注册表里名字为VCP0/VCP1，会被误识别为电机
	Sleep(4000);
	ConnectCCD();
	
	GetComList_Reg();
	sInfo.LockConnect = 0;
	sInfo.LConnect = 0;
	//2023
	if (sSInfo.isCheckLed==0)
	{
		sInfo.LockConnect = 1;
	}
	

	int mc = m_vComInfo.size();
	for(map<int, CString>::iterator it = m_vComInfo.begin(); it != m_vComInfo.end(); it++)
	{
		//连接激光器
		//else if((it->second == "Serial0" || it->second == "Serial1" || it->second == "Serial2" || it->second == "Serial3") && sInfo.LConnect == 0)//  如果找到了激光器的COM口，则将sSInfo.nCOM设置为该COM口，并连接激光器
		if ((it->second.Find(TEXT("Serial")) != -1) && sInfo.LConnect == 0)//  如果找到了激光器的COM口，则将sSInfo.nCOM设置为该COM口，并连接激光器
		{
			if (it->first == 1)// 如果是com1，则跳过 (可能com1被保留或不使用)
				continue;
			sSInfo.nCOM = it->first;
			
			//if(sSInfo.nCOM == it->first)//  如果激光器串口号正确
			//{
		
			sInfo.LConnect = m_Serial.OpenLaser(sSInfo.nCOM);//  1 表示成功连接， 0  表示连接失败
															 //}
															 //else
															 //	sInfo.LConnect = 0;
													 
			//如果是新板子（激光、电机公用一个com口）
			//连上激光后，发送 000，如果有回复，就是新板子，不需要再连电机
			



			if (1 == sInfo.LConnect)
			{
				bool bret = m_SerialLed.SetLed_is_new(sSInfo.nCOM, _T("000"));
				if (bret)
				{
					m_bnew_board = true;
					sSInfo.nCOMLED = sSInfo.nCOM;
					m_SerialLed.set_board_flag(true);
					m_SerialLed.SetLed(sSInfo.nCOMLED, _T("HX"));
					break;
				}	
			}
			
		}

		//if((it->second == "VCP0" || it->second == "VCP1" || it->second == "VCP2" || it->second == "VCP3") && sInfo.LockConnect == 0)//  如果找到了相机的COM口，则将sSInfo.nCOMLED设置为该COM口，并连接相机
		else if((it->second.Find(TEXT("VCP")) != -1) && sInfo.LockConnect == 0)//  如果找到了相机电机的COM口，则将sSInfo.nCOMLED设置为该COM口，并连接相机电机
		{
			sSInfo.nCOMLED =  it->first;
			//if(sSInfo.nCOMLED == it->first)//  如果电机串口号正确，那么就正常连接
			//{
				sInfo.LockConnect = m_SerialLed.OpenLed(sSInfo.nCOMLED);
				if (sInfo.LockConnect==1)//  如果相机连接成功，则对它进行初始化
				{
					//WriteToLog("<<<<<<<<<<相机连接成功>>>>>>>");// lcz new 01/31
					bool a = m_SerialLed.SetLed(sSInfo.nCOMLED,_T("PX,1"));
					if(a == false)// 如果电脑接了其他的串口线，而且是第一个接，名为vcp0
						sInfo.LockConnect = 0;
					bool b =m_SerialLed.SetLed(sSInfo.nCOMLED,_T("NX,1"));
					if(b == false)
					{
						m_SerialLed.CloseLaser(sSInfo.nCOMLED);
						sInfo.LockConnect = 0;
					}
					bool c =m_SerialLed.SetLed(sSInfo.nCOMLED,_T("HX"));// lcz new 05/16 c的值不能作为能否连接的标志
					//if(c == false)
					//{
					//	m_SerialLed.CloseLaser(sSInfo.nCOMLED);
					//	sInfo.LockConnect = 0;
					//}
					bool d = true;
				}

		}
	}


	//  更新相机和激光器串口号
	CString strResult;
	strResult.Format(TEXT("%d"), sSInfo.nCOMLED);
	WritePrivateProfileString("SET","COMLED",strResult,sInfo.strPath);
	strResult.Format(TEXT("%d"), sSInfo.nCOM);
	WritePrivateProfileString("SET","COM",strResult,sInfo.strPath);


	OnBnState();//  三个都进行了连接后，更新按钮状态----------还需要根据COM口来调整设置界面里的COM口
	SetData(); //  如果激光器、相机的串口被更新，则在设置界面的combo组合框
	//CINScanLiteDlg * pDlg = (CINScanLiteDlg *)AfxGetApp()->GetMainWnd();
	//sInfo.LConnect=m_Serial.OpenLaser(sSInfo.nCOM);//  1 表示成功连接， 0  表示连接失败
	//sInfo.LockConnect=m_SerialLed.OpenLed(sSInfo.nCOMLED);
	//if (sInfo.LockConnect==1)//  如果相机连接成功，则对它进行初始化
	//{
	//	m_SerialLed.SetLed(sSInfo.nCOMLED,_T("PX,1"));
	//	m_SerialLed.SetLed(sSInfo.nCOMLED,_T("NX,1"));
	// 上来就直接开启激光
	// m_SerialLed.OpenLaserPower(sSInfo.nCOMLED);
	//	//m_SerialLed.SetVolt(sSInfo.nCOMLED,_T("LDOFF"));
	//	m_SerialLed.SetLed(sSInfo.nCOMLED,_T("HX"));
	//}
	////OnBnClickedButtonLcon3();
	//OnBnState();
}

 void CDlgSet::GetComList_Reg()   
{   
    HKEY hkey;     
    int result;   
    int i = 0;   
    CString strComName;//串口名称   
    CString strDrName;//串口详细名称   
	m_vComInfo.clear();

    result = RegOpenKeyEx( HKEY_LOCAL_MACHINE,  _T( "Hardware\\DeviceMap\\SerialComm" ),  NULL,  KEY_READ,  &hkey );   

    if( ERROR_SUCCESS == result )   //   打开串口注册表      
    {      
		TCHAR portName[ 0x100 ], commName[ 0x100 ];   
		 DWORD dwLong, dwSize;   
		// CCombox->ResetContent();   
        do   
        {      
			dwSize = sizeof( portName ) / sizeof( TCHAR );   
			dwLong = dwSize;   
			result = RegEnumValue( hkey, i, portName, &dwLong, NULL, NULL, ( LPBYTE )commName, &dwSize );   
			if( ERROR_NO_MORE_ITEMS == result )   
			{   
               //   枚举串口 
               break;   //   commName就是串口名字"COM2"   
			}   
			strComName=commName;   
			strDrName = portName;   
		// 从右往左边开始查找第一个'\\'，获取左边字符串的长度   
			int len = strDrName.ReverseFind( '\\');   
		// 获取'\\'左边的字符串   
			CString strFilePath = strDrName.Left( len+1 );   
		// 获取'\\'右边的字符串   
			CString fileName = strDrName.Right(strDrName.GetLength() - len-1);   
			//fileName = strComName + _T(":")+fileName;   
			CString tempCom = strComName.Mid(3);
			m_vComInfo.insert(make_pair(atoi(tempCom), fileName));
			//CCombox->AddString(fileName);   
			i++;      
		} while ( 1 );   
			RegCloseKey( hkey );      
   }   

    //CCombox->SetCurSel(0);   
}   

//  连接激光器
void CDlgSet::OnBnClickedButtonLcon()
{
	CString str;
	CINScanLiteDlg * pDlg = (CINScanLiteDlg *)AfxGetApp()->GetMainWnd();
	
	GetComList_Reg();// 重新查看注册表

	for(map<int, CString>::iterator it = m_vComInfo.begin(); it != m_vComInfo.end(); it++)
	{
		// lcz new 05/27  修复自动连接激光器的bug，插拔usb后注册表里的名称可能会改变较大
		//if((it->second == "Serial0" || it->second == "Serial1" || it->second == "Serial2" || it->second == "Serial3") && sInfo.LConnect == 0)
		if((it->second.Find(TEXT("Serial")) != -1 ) && sInfo.LConnect == 0)
		{
			if(it->first == 1)
				continue;
			if(sSInfo.nCOM == it->first)//  如果激光器串口号正确
			{
				sInfo.LConnect = m_Serial.OpenLaser(sSInfo.nCOM);//  1 表示成功连接， 0  表示连接失败
			}
		}
	}
	if( sInfo.LConnect == 0)
	{
		str=pDlg->LoadStringMeau(_T("ID_LASER_CONNECT"));
		MessageBox(str);
	}

	//if (m_Serial.OpenLaser(sSInfo.nCOM)==FALSE)
	//{
	//	sInfo.LConnect=0;
	//	str=pDlg->LoadStringMeau(_T("ID_LASER_CONNECT"));
	//	MessageBox(str);
	//}

	OnBnState();
	pDlg->WriteDeviceInfo();
}

//  断开激光器连接
void CDlgSet::OnBnClickedButtonLdis()
{
	m_Serial.CloseLaser(sSInfo.nCOM);
	sInfo.LConnect=0;
	OnBnState();

}


void CDlgSet::OnBnClickedButtonCcon()
{
	ConnectCCD();
	CString str;
	CINScanLiteDlg * pDlg = (CINScanLiteDlg *)AfxGetApp()->GetMainWnd();
	if (sInfo.CConnect==0)
	{
		str=pDlg->LoadStringMeau(_T("ID_OMINI_CONNECT"));

		MessageBox(str);
	}
	else
	{
		if(g_bUSBDiscon) // 如果异常断开过，然后又重新连上了，重开28号定时器，检测CCD是否断开
			pDlg->SetTimer(28, 8000, NULL);
		g_bUSBDiscon = false;
		
	}
	OnBnState();
   
	g_bGetTemp = false;
}
 
//  lcz new 23/05/16  这里 左下方的CCD连接按钮的颜色（状态）将会更新  pDlg->SetBtnS();
void CDlgSet::OnBnState()
{
	BOOL bS;
	bS=(sInfo.CConnect==1)?TRUE:FALSE ;
	GetDlgItem(IDC_BUTTON_CCON)->EnableWindow(!bS);
	GetDlgItem(IDC_BUTTON_CDIS)->EnableWindow(bS);
	GetDlgItem(IDC_BUTTON_CCD)->EnableWindow(bS);
	GetDlgItem(IDC_COMBO_OMNI)->EnableWindow(!bS);
	bS=(sInfo.LConnect==1)?TRUE:FALSE ;
	GetDlgItem(IDC_BUTTON_LCON)->EnableWindow(!bS);
	GetDlgItem(IDC_BUTTON_LDIS)->EnableWindow(bS);
	GetDlgItem(IDC_COMBO_COM)->EnableWindow(!bS);

	GetDlgItem(IDC_BUTTON_OpenLaser)->EnableWindow(bS);

	bS=(sInfo.LockConnect==1)?TRUE:FALSE ;
	GetDlgItem(IDC_BUTTON_LCON2)->EnableWindow(!bS);
	GetDlgItem(IDC_BUTTON_LDIS2)->EnableWindow(bS);
	GetDlgItem(IDC_COMBO_COM2)->EnableWindow(!bS);

	bS=(sInfo.PriorConnect==1)?TRUE:FALSE ;
	GetDlgItem(IDC_BUTTON_LCON3)->EnableWindow(!bS);
	GetDlgItem(IDC_BUTTON_LDIS3)->EnableWindow(bS);
	GetDlgItem(IDC_COMBO_COM3)->EnableWindow(!bS);


	if(sSInfo.isOEM==0)
	{
		GetDlgItem(IDC_COMBO_COM)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_LCON)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_LDIS)->EnableWindow(FALSE);
	}


	
	CINScanLiteDlg * pDlg = (CINScanLiteDlg *)AfxGetApp()->GetMainWnd();
	pDlg->SetBtnS();
	

	// lcz new 07/05
	if(g_bWaveLenCorr)
		((CButton *)GetDlgItem(IDC_RADIO_WAVELEN_out))->SetCheck(TRUE);
	else
		((CButton *)GetDlgItem(IDC_RADIO_WAVENUM_out))->SetCheck(TRUE);

	if (m_bnew_board)    //如果是新板子，监视电机对应的按钮和下拉列表置灰
	{
		GetDlgItem(IDC_COMBO_COM2)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_LCON2)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_LDIS2)->EnableWindow(FALSE);
	}

}

void CDlgSet::OnBnClickedButtonCdis()
{
	g_bGetTemp = true;
	// lcz new 23/05/12
	CINScanLiteDlg * pDlg = (CINScanLiteDlg *)AfxGetApp()->GetMainWnd();
	if (pDlg->m_b1064)
	{
		//  停止制冷
		// dfield_stop_cooling(pDlg->_handle);
		pDlg->_cooling = FALSE;
		dfield_close(pDlg->_handle);
	}
	else
		m_ZolixCCD.DisConnect();
	sInfo.CConnect=0;
	OnBnState();
}



void CDlgSet::OnBnClickedButtonSet()
{
	CString str;
	GetDlgItem(IDC_EDIT_DELAY)->GetWindowText(str);
	sSInfo.Delay=atoi(str);
	SetButtonState(&m_BtnSet);
}


void CDlgSet::OnBnClickedButtonCcd()
{
	
	CINScanLiteDlg * pDlg = (CINScanLiteDlg *)AfxGetApp()->GetMainWnd();
	if (m_bAutoRunning==FALSE&&m_bAutoRunningStatue==FALSE)
	{
		// lcz new 06/09 添加的记录是否打开CCD设置的标识符
		/*g_bOpenCCDSet = true;
		Sleep(800);*/
		
		if (pDlg->m_b1064 == true)
		{
			g_bOpenCCDSet = true;
			CDlgDccdSet dlg;
			dlg.DoModal();
			g_bOpenCCDSet = false;
		}
		else
		{
			g_tmpMutex.Lock();
			g_bOpenCCDSet = true;  
			
			
			m_ZolixCCD.ShowCameraSetDlg();

			
			g_bOpenCCDSet = false;  
			g_tmpMutex.Unlock();
		}
	}
	
}

void CDlgSet::ConnectCCD()
{
	unsigned short count = dfield_enum_dev_count();
	CINScanLiteDlg * pDlg = (CINScanLiteDlg *)AfxGetApp()->GetMainWnd();
	//  lcz new 23/05/12   先要连1064，再连电机。因为1064的com口也能通过电机的连接，先连电机的话，后续就连不上1064了；先连1064，将该com口占用，连电机时就不会误连1064了，而且电机的com口还能正常连上
	sInfo.CConnect = 0;
	pDlg->m_b1064 = FALSE;// lcz new 23/05/22  设置其默认为false
	int nlen = 64;
	for (int i = 0; i < count; i++) {
		//pDlg->_1064c_sn = new char[32]{ 0 };
		char buff[32] = { 0 };
		bool b = dfield_enum_dev_sn(i, buff, nlen);
		if (buff[0] == 'D' && buff[1] == 'F'&& buff[2] == 'N')
		{
			pDlg->_handle = dfield_open(buff);
			if (pDlg->_handle <= 0)
				sInfo.CConnect = 0;
			else
			{
				pDlg->_1064sn = buff;
				pDlg->m_b1064 = TRUE;
				sInfo.CConnect = 1;
				CString tempSN;
				tempSN.Format("%s", pDlg->_1064sn.c_str());
				sInfo.sDInfo.CCDName = tempSN;
				//  获取像素点个数
				short width = 0, height = 0;
				dfield_get_detector_size(pDlg->_handle, &width, &height);
				short bin_h = 1, bin_v = 1;
				dfield_get_bin(pDlg->_handle, &bin_h, &bin_v);
				int len = width / bin_h;//  像素个数 = 宽度/bin_h
				sInfo.sDInfo.nGXPix = len;
				// 开启制冷
				dfield_set_temperature(pDlg->_handle, sSInfo.CoolTemp);
				dfield_start_cooling(pDlg->_handle);
				pDlg->_cooling = TRUE;

				//  如果是1064探测器，则波长校正不可用
				OnBnClickedRadioWavenumout();
				GetDlgItem(IDC_RADIO_WAVELEN_out)->EnableWindow(FALSE);
			}
		}
	}

	int bState=0;
	
	//  如果AndorCCD断开连接，并且激光器已连接，则连接CCD
	if(g_bAndorTestingDiscon)
	{
		g_bAndorTestingDiscon = false;
	}

	//  lcz new 23/05/12  先判断1064的有没有成功连上，如果连上了的话就不用试其他四种CCD了
	if(sInfo.CConnect && pDlg->m_b1064)
	{
		bState = 0;
	}
	else if (pDlg->m_b1064 && !sInfo.CConnect)//  如果是1064，未连接，则进行连接测试
	{
		pDlg->_handle = dfield_open(pDlg->_1064sn.c_str());
		if (pDlg->_handle <= 0)
			sInfo.CConnect = 0;
		else
		{
			sInfo.CConnect = 1;
			CString tempSN;
			tempSN.Format("%s", pDlg->_1064sn.c_str());
			sInfo.sDInfo.CCDName = tempSN;
			// 开启制冷
			dfield_set_temperature(pDlg->_handle, sSInfo.CoolTemp);
			dfield_start_cooling(pDlg->_handle);
			pDlg->_cooling = TRUE;
		}
	}
	else
	{
		WriteToLog_F("------start--from1064 no connect else -----");
	
		m_ZolixCCD.InitType(sSInfo.firstOminiType);   //sSInfo.firstOminiType 
		bState = m_ZolixCCD.Connect();
		
		if (bState)  
		{
			m_ZolixCCD.InitType(15);
			bState = m_ZolixCCD.Connect();
			if (bState) // 如果15连接失败
			{
				m_ZolixCCD.InitType(10);
				bState = m_ZolixCCD.Connect();
				if (bState) // 如果10连接失败
				{
					m_ZolixCCD.InitType(20);
					bState = m_ZolixCCD.Connect();
					if (bState) // 如果20连接失败
					{
						m_ZolixCCD.InitType(30);
						bState = m_ZolixCCD.Connect();
						if (bState) // 如果30连接失败，再尝试50
						{
							m_ZolixCCD.InitType(50);
							bState = m_ZolixCCD.Connect();
							if (!bState)
								sInfo.OminiType = 50;
							else
								sInfo.OminiType = sSInfo.firstOminiType; // 全部都连不上
						}
						else
							sInfo.OminiType = 30;
					}
					else
						sInfo.OminiType = 20;
				}
				else
					sInfo.OminiType = 10;
			}
			else
				sInfo.OminiType = 15;
		}
		else
		{
			sInfo.OminiType = sSInfo.firstOminiType;
		}


		//  如果CCD连接成功，则sInfo.CConnect为1.  lcz new 23/05/12 挪到此处
		if (bState == 0)
		{
		
			sInfo.sDInfo.CCDName = m_ZolixCCD.GetDevName();
 			m_ZolixCCD.GetDevSize(&sInfo.sDInfo.nGXPix, &sInfo.sDInfo.nGYPix);
			m_ZolixCCD.GetPixSize(&sInfo.sDInfo.fXPixSize);;
			m_ZolixCCD.SetCooler(sSInfo.Cool);
			sInfo.CConnect = 1;
			//  开启外部定时器
			//g_iConnectCCDOkNum++;// 如果在软件打开时，成功连接CCD，g_iConnectCCDOkNum = 1；
			//pDlg->SetTimer(8, 1200, NULL);
			//pDlg->SetTimer(18, 5000, NULL);
		}
		else
		{//ccd连接失败
			
			sInfo.CConnect = 0;
			sInfo.sDInfo.CCDName = _T("");
			m_ZolixCCD.DisConnect();
		}

	}
	
	pDlg->ClockTreadProc(FALSE, 0);

	sInfo.statusBar=100;
	//  如果CCD连接成功，则sInfo.CConnect为1.  lcz new 23/05/12 挪到上方

	CComboBox * pSelbox;
	pSelbox = (CComboBox *)GetDlgItem(IDC_COMBO_OMNI);
	pSelbox->ResetContent();
	pSelbox->InsertString(0,sInfo.sDInfo.CCDName);
	pSelbox->SetCurSel(0);
}

void CDlgSet::OnBnClickedButtonUpdata()
{
	CINScanLiteDlg * pDlg = (CINScanLiteDlg *)AfxGetApp()->GetMainWnd();

	//  如果是低使能，则点击低使能单选按钮
	if(iRadio)
	{
		OnBnClickedRadioL();
		((CButton*)GetDlgItem(IDC_RADIO_L))->SetCheck(true);
		((CButton*)GetDlgItem(IDC_RADIO_H))->SetCheck(false);
	}
	else
	{
		OnBnClickedRadioH();
		((CButton*)GetDlgItem(IDC_RADIO_H))->SetCheck(true);
		((CButton*)GetDlgItem(IDC_RADIO_L))->SetCheck(false);
	}
	//  更新电机步数值
	CString temp;
	temp.Format(TEXT("%d"), sSInfo.LockSet);
	GetDlgItem(IDC_EDIT1)->SetWindowTextA(temp);

	pDlg->InitDeviceInfo();
	SetData();
	pDlg->m_DlgTest.m_DlgTestCheckJ.SetData();
	pDlg->m_DlgTest.m_DlgTestCheckK.SetData();

}


void CDlgSet::OnEnChangeEditRight()
{
	SetCButtonState(&m_BtnRSet);
}

void CDlgSet::OnEnChangeEditLeft()
{
	SetCButtonState(&m_BtnRSet);

}



void CDlgSet::OnEnChangeEditDelay()
{
	SetCButtonState(&m_BtnSet);
}

void CDlgSet::OnBnClickedButtonCorr()
{
	CINScanLiteDlg * pDlg=(CINScanLiteDlg*)AfxGetApp()->GetMainWnd();

	if (pDlg->m_DlgShow.GetTeeCount()>0)
	{
		CBxsOffset dlg;
		dlg.DoModal();
	}
	
}

//  导入校正文件
void CDlgSet::OnBnClickedButtonIn()
{
	// TODO: 在此添加控件通知处理程序代码
	CINScanLiteDlg * pThreadApp=(CINScanLiteDlg*)AfxGetApp()->GetMainWnd();
	CFileDialog dlg(TRUE,_T("ini"),_T("value.ini"),OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,"value(*.ini)|*.ini||");
	if (dlg.DoModal()!=IDOK)
	{
		return;
	}
	CString strFathName =dlg.GetPathName();
	pThreadApp->WriteDeviceInfo();

	CString strResult;
	DWORD	dwSize=1024;

	//pThreadApp->WriteDeviceInfo();
	GetPrivateProfileString("CCD","Num","0",strResult.GetBuffer(dwSize),dwSize,strFathName);
	WritePrivateProfileString("A","CCDNum",strResult,sInfo.strPathCCD);
	WritePrivateProfileString("M","CCDNum",strResult,sInfo.strPathCCD);
	WritePrivateProfileString("T","CCDNum",strResult,sInfo.strPathCCD);

	GetPrivateProfileString("CCD","Pixel","0",strResult.GetBuffer(dwSize),dwSize,strFathName);
	WritePrivateProfileString("A","CCD",strResult,sInfo.strPathCCD);
	WritePrivateProfileString("M","CCD",strResult,sInfo.strPathCCD);
	WritePrivateProfileString("T","CCD",strResult,sInfo.strPathCCD);

	GetPrivateProfileString("CCD","Model","FI",strResult.GetBuffer(dwSize),dwSize,strFathName);
	WritePrivateProfileString("PRINT","Model",strResult,sInfo.strPath);

	GetPrivateProfileString("CCD","Number","10009",strResult.GetBuffer(dwSize),dwSize,strFathName);
	WritePrivateProfileString("PRINT","Number",strResult,sInfo.strPath);

	GetPrivateProfileString("CCD","OminiType","15",strResult.GetBuffer(dwSize),dwSize,strFathName);
	WritePrivateProfileString("System","OminiType",strResult,sInfo.strPath);

	GetPrivateProfileString("sz","Laser","785",strResult.GetBuffer(dwSize),dwSize,strFathName);
	WritePrivateProfileString("CORRECT","Laser",strResult,sInfo.strPath);



	GetPrivateProfileString("sz", "LaserPowerMin_XingLin", "0", strResult.GetBuffer(dwSize), dwSize, strFathName);
	WritePrivateProfileString("SET", "LaserPowerMin_XingLin", strResult, sInfo.strPath);

	GetPrivateProfileString("sz", "LaserPowerMax_XingLin", "250", strResult.GetBuffer(dwSize), dwSize, strFathName);
	WritePrivateProfileString("SET", "LaserPowerMax_XingLin", strResult, sInfo.strPath);

	GetPrivateProfileString("sz", "LaserPowerMin_XinChanYe", "2200", strResult.GetBuffer(dwSize), dwSize, strFathName);
	WritePrivateProfileString("SET", "LaserPowerMin_XinChanYe", strResult, sInfo.strPath);

	GetPrivateProfileString("sz", "LaserPowerMax_XinChanYe", "2500", strResult.GetBuffer(dwSize), dwSize, strFathName);
	WritePrivateProfileString("SET", "LaserPowerMax_XinChanYe", strResult, sInfo.strPath);

	GetPrivateProfileString("sz","LockSet","5000",strResult.GetBuffer(dwSize),dwSize,strFathName);
	WritePrivateProfileString("SET","LockSet",strResult,sInfo.strPath);
	
	GetPrivateProfileString("sz","SqlMode","0",strResult.GetBuffer(dwSize),dwSize,strFathName);
	WritePrivateProfileString("SET","SqlMode",strResult,sInfo.strPath);

	// lcz new 08/30  导入高低使能、电机步数、谱峰值
	GetPrivateProfileString("SET","HLMODE","0",strResult.GetBuffer(dwSize),dwSize,strFathName);
	WritePrivateProfileString("SET","HLMODE",strResult,sInfo.strPath);
	if (strResult == TEXT("0"))
	{
		iRadio = 0;
		sSInfo.HLMODE = 0;
	}
	else
	{
		sSInfo.HLMODE = 1;
		iRadio = 1;
	}
		
	//GetPrivateProfileString("SET","LockSet","5000",strResult.GetBuffer(dwSize),dwSize,strFathName);
	//WritePrivateProfileString("SET","LockSet",strResult,sInfo.strPath);
	GetPrivateProfileString("SET","FI532rdWaveNum","2839.1",strResult.GetBuffer(dwSize),dwSize,strFathName);
	WritePrivateProfileString("SET","FI532rdWaveNum",strResult,sInfo.strPath);
	GetPrivateProfileString("SET","FI785rdWaveNum","2384.5",strResult.GetBuffer(dwSize),dwSize,strFathName);
	WritePrivateProfileString("SET","FI785rdWaveNum",strResult,sInfo.strPath);


	GetPrivateProfileString("xs","a0","1",strResult.GetBuffer(dwSize),dwSize,strFathName);
	WritePrivateProfileString("xs","a0",strResult,sInfo.strPath);

	GetPrivateProfileString("xs","a1","1",strResult.GetBuffer(dwSize),dwSize,strFathName);
	WritePrivateProfileString("xs","a1",strResult,sInfo.strPath);

	GetPrivateProfileString("xs","a2","1",strResult.GetBuffer(dwSize),dwSize,strFathName);
	WritePrivateProfileString("xs","a2",strResult,sInfo.strPath);

	GetPrivateProfileString("xs","a3","0",strResult.GetBuffer(dwSize),dwSize,strFathName);
	WritePrivateProfileString("xs","a3",strResult,sInfo.strPath);

	///////////////////////////////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////////////////////////////////////
	GetPrivateProfileString("bxjzxs","a0","1",strResult.GetBuffer(dwSize),dwSize,strFathName);
	WritePrivateProfileString("bxjzxs","a0",strResult,sInfo.strPath);

	GetPrivateProfileString("bxjzxs","a1","2",strResult.GetBuffer(dwSize),dwSize,strFathName);
	WritePrivateProfileString("bxjzxs","a1",strResult,sInfo.strPath);

	GetPrivateProfileString("bxjzxs","a2","3",strResult.GetBuffer(dwSize),dwSize,strFathName);
	WritePrivateProfileString("bxjzxs","a2",strResult,sInfo.strPath);

	GetPrivateProfileString("bxjzxs","a3","4",strResult.GetBuffer(dwSize),dwSize,strFathName);
	WritePrivateProfileString("bxjzxs","a3",strResult,sInfo.strPath);

	GetPrivateProfileString("bxjzxs","a4","5",strResult.GetBuffer(dwSize),dwSize,strFathName);
	WritePrivateProfileString("bxjzxs","a4",strResult,sInfo.strPath);

	GetPrivateProfileString("bxjzxs","a5","6",strResult.GetBuffer(dwSize),dwSize,strFathName);
	WritePrivateProfileString("bxjzxs","a5",strResult,sInfo.strPath);

	GetPrivateProfileString("bxjzxs","a6","7",strResult.GetBuffer(dwSize),dwSize,strFathName);
	WritePrivateProfileString("bxjzxs","a6",strResult,sInfo.strPath);

	GetPrivateProfileString("bxjzxs","a7","8",strResult.GetBuffer(dwSize),dwSize,strFathName);
	WritePrivateProfileString("bxjzxs","a7",strResult,sInfo.strPath);

	GetPrivateProfileString("bxjzxs","a8","9",strResult.GetBuffer(dwSize),dwSize,strFathName);
	WritePrivateProfileString("bxjzxs","a8",strResult,sInfo.strPath);

	GetPrivateProfileString("bxjzxs","a9","10",strResult.GetBuffer(dwSize),dwSize,strFathName);
	WritePrivateProfileString("bxjzxs","a9",strResult,sInfo.strPath);

	GetPrivateProfileString("bxjzxs","a10","11",strResult.GetBuffer(dwSize),dwSize,strFathName);
	WritePrivateProfileString("bxjzxs","a10",strResult,sInfo.strPath);

	GetPrivateProfileString("bxjzxs","a11","12",strResult.GetBuffer(dwSize),dwSize,strFathName);
	WritePrivateProfileString("bxjzxs","a11",strResult,sInfo.strPath);

	GetPrivateProfileString("bxjzxs","a12","13",strResult.GetBuffer(dwSize),dwSize,strFathName);
	WritePrivateProfileString("bxjzxs","a12",strResult,sInfo.strPath);

	GetPrivateProfileString("bxjzxs","a13","14",strResult.GetBuffer(dwSize),dwSize,strFathName);
	WritePrivateProfileString("bxjzxs","a13",strResult,sInfo.strPath);

	GetPrivateProfileString("bxjzxs","a14","15",strResult.GetBuffer(dwSize),dwSize,strFathName);
	WritePrivateProfileString("bxjzxs","a14",strResult,sInfo.strPath);
	//////////////////////////////////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////////////////////////////////////////
	GetPrivateProfileString("CS","a0","1",strResult.GetBuffer(dwSize),dwSize,strFathName);
	WritePrivateProfileString("CS","a0",strResult,sInfo.strPath);

	GetPrivateProfileString("CS","a1","2",strResult.GetBuffer(dwSize),dwSize,strFathName);
	WritePrivateProfileString("CS","a1",strResult,sInfo.strPath);

	GetPrivateProfileString("CS","a2","3",strResult.GetBuffer(dwSize),dwSize,strFathName);
	WritePrivateProfileString("CS","a2",strResult,sInfo.strPath);

	GetPrivateProfileString("CS","a3","4",strResult.GetBuffer(dwSize),dwSize,strFathName);
	WritePrivateProfileString("CS","a3",strResult,sInfo.strPath);

	GetPrivateProfileString("CS","a4","5",strResult.GetBuffer(dwSize),dwSize,strFathName);
	WritePrivateProfileString("CS","a4",strResult,sInfo.strPath);

	GetPrivateProfileString("CS","a5","6",strResult.GetBuffer(dwSize),dwSize,strFathName);
	WritePrivateProfileString("CS","a5",strResult,sInfo.strPath);

	GetPrivateProfileString("CS","a6","7",strResult.GetBuffer(dwSize),dwSize,strFathName);
	WritePrivateProfileString("CS","a6",strResult,sInfo.strPath);

	GetPrivateProfileString("CS","a7","8",strResult.GetBuffer(dwSize),dwSize,strFathName);
	WritePrivateProfileString("CS","a7",strResult,sInfo.strPath);

	GetPrivateProfileString("CS","a8","9",strResult.GetBuffer(dwSize),dwSize,strFathName);
	WritePrivateProfileString("CS","a8",strResult,sInfo.strPath);

	GetPrivateProfileString("CS","a9","10",strResult.GetBuffer(dwSize),dwSize,strFathName);
	WritePrivateProfileString("CS","a9",strResult,sInfo.strPath);

	GetPrivateProfileString("CS","a10","11",strResult.GetBuffer(dwSize),dwSize,strFathName);
	WritePrivateProfileString("CS","a10",strResult,sInfo.strPath);

	GetPrivateProfileString("CS","a11","12",strResult.GetBuffer(dwSize),dwSize,strFathName);
	WritePrivateProfileString("CS","a11",strResult,sInfo.strPath);

	GetPrivateProfileString("CS","a12","13",strResult.GetBuffer(dwSize),dwSize,strFathName);
	WritePrivateProfileString("CS","a12",strResult,sInfo.strPath);

	GetPrivateProfileString("CS","a13","14",strResult.GetBuffer(dwSize),dwSize,strFathName);
	WritePrivateProfileString("CS","a13",strResult,sInfo.strPath);

	GetPrivateProfileString("CS","a14","15",strResult.GetBuffer(dwSize),dwSize,strFathName);
	WritePrivateProfileString("CS","a14",strResult,sInfo.strPath);

	//  导入波数校正的系数
	GetPrivateProfileString("xsWaveNum","b0","1",strResult.GetBuffer(dwSize),dwSize,strFathName);
	WritePrivateProfileString("xsWaveNum","b0",strResult,sInfo.strPath);

	GetPrivateProfileString("xsWaveNum","b1","1",strResult.GetBuffer(dwSize),dwSize,strFathName);
	WritePrivateProfileString("xsWaveNum","b1",strResult,sInfo.strPath);

	GetPrivateProfileString("xsWaveNum","b2","1",strResult.GetBuffer(dwSize),dwSize,strFathName);
	WritePrivateProfileString("xsWaveNum","b2",strResult,sInfo.strPath);

	GetPrivateProfileString("xsWaveNum","b3","0",strResult.GetBuffer(dwSize),dwSize,strFathName);
	WritePrivateProfileString("xsWaveNum","b3",strResult,sInfo.strPath);

	///////////////////////////////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////////////////////////////////////
	GetPrivateProfileString("waveNumber","a0","1",strResult.GetBuffer(dwSize),dwSize,strFathName);
	WritePrivateProfileString("waveNumber","a0",strResult,sInfo.strPath);

	GetPrivateProfileString("waveNumber","a1","2",strResult.GetBuffer(dwSize),dwSize,strFathName);
	WritePrivateProfileString("waveNumber","a1",strResult,sInfo.strPath);

	GetPrivateProfileString("waveNumber","a2","3",strResult.GetBuffer(dwSize),dwSize,strFathName);
	WritePrivateProfileString("waveNumber","a2",strResult,sInfo.strPath);

	GetPrivateProfileString("waveNumber","a3","4",strResult.GetBuffer(dwSize),dwSize,strFathName);
	WritePrivateProfileString("waveNumber","a3",strResult,sInfo.strPath);

	GetPrivateProfileString("waveNumber","a4","5",strResult.GetBuffer(dwSize),dwSize,strFathName);
	WritePrivateProfileString("waveNumber","a4",strResult,sInfo.strPath);

	GetPrivateProfileString("waveNumber","a5","6",strResult.GetBuffer(dwSize),dwSize,strFathName);
	WritePrivateProfileString("waveNumber","a5",strResult,sInfo.strPath);

	GetPrivateProfileString("waveNumber","a6","7",strResult.GetBuffer(dwSize),dwSize,strFathName);
	WritePrivateProfileString("waveNumber","a6",strResult,sInfo.strPath);

	GetPrivateProfileString("waveNumber","a7","8",strResult.GetBuffer(dwSize),dwSize,strFathName);
	WritePrivateProfileString("waveNumber","a7",strResult,sInfo.strPath);

	GetPrivateProfileString("waveNumber","a8","9",strResult.GetBuffer(dwSize),dwSize,strFathName);
	WritePrivateProfileString("waveNumber","a8",strResult,sInfo.strPath);

	GetPrivateProfileString("waveNumber","a9","10",strResult.GetBuffer(dwSize),dwSize,strFathName);
	WritePrivateProfileString("waveNumber","a9",strResult,sInfo.strPath);

	GetPrivateProfileString("waveNumber","a10","11",strResult.GetBuffer(dwSize),dwSize,strFathName);
	WritePrivateProfileString("waveNumber","a10",strResult,sInfo.strPath);

	GetPrivateProfileString("waveNumber","a11","12",strResult.GetBuffer(dwSize),dwSize,strFathName);
	WritePrivateProfileString("waveNumber","a11",strResult,sInfo.strPath);

	GetPrivateProfileString("waveNumber","a12","13",strResult.GetBuffer(dwSize),dwSize,strFathName);
	WritePrivateProfileString("waveNumber","a12",strResult,sInfo.strPath);

	GetPrivateProfileString("waveNumber","a13","14",strResult.GetBuffer(dwSize),dwSize,strFathName);
	WritePrivateProfileString("waveNumber","a13",strResult,sInfo.strPath);

	GetPrivateProfileString("waveNumber","a14","15",strResult.GetBuffer(dwSize),dwSize,strFathName);
	WritePrivateProfileString("waveNumber","a14",strResult,sInfo.strPath);
	//////////////////////////////////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////////////////////////////////////////
	GetPrivateProfileString("CSWaveNum","a0","1",strResult.GetBuffer(dwSize),dwSize,strFathName);
	WritePrivateProfileString("CSWaveNum","a0",strResult,sInfo.strPath);

	GetPrivateProfileString("CSWaveNum","a1","2",strResult.GetBuffer(dwSize),dwSize,strFathName);
	WritePrivateProfileString("CSWaveNum","a1",strResult,sInfo.strPath);

	GetPrivateProfileString("CSWaveNum","a2","3",strResult.GetBuffer(dwSize),dwSize,strFathName);
	WritePrivateProfileString("CSWaveNum","a2",strResult,sInfo.strPath);

	GetPrivateProfileString("CSWaveNum","a3","4",strResult.GetBuffer(dwSize),dwSize,strFathName);
	WritePrivateProfileString("CSWaveNum","a3",strResult,sInfo.strPath);

	GetPrivateProfileString("CSWaveNum","a4","5",strResult.GetBuffer(dwSize),dwSize,strFathName);
	WritePrivateProfileString("CSWaveNum","a4",strResult,sInfo.strPath);

	GetPrivateProfileString("CSWaveNum","a5","6",strResult.GetBuffer(dwSize),dwSize,strFathName);
	WritePrivateProfileString("CSWaveNum","a5",strResult,sInfo.strPath);

	GetPrivateProfileString("CSWaveNum","a6","7",strResult.GetBuffer(dwSize),dwSize,strFathName);
	WritePrivateProfileString("CSWaveNum","a6",strResult,sInfo.strPath);

	GetPrivateProfileString("CSWaveNum","a7","8",strResult.GetBuffer(dwSize),dwSize,strFathName);
	WritePrivateProfileString("CSWaveNum","a7",strResult,sInfo.strPath);

	GetPrivateProfileString("CSWaveNum","a8","9",strResult.GetBuffer(dwSize),dwSize,strFathName);
	WritePrivateProfileString("CSWaveNum","a8",strResult,sInfo.strPath);

	GetPrivateProfileString("CSWaveNum","a9","10",strResult.GetBuffer(dwSize),dwSize,strFathName);
	WritePrivateProfileString("CSWaveNum","a9",strResult,sInfo.strPath);

	GetPrivateProfileString("CSWaveNum","a10","11",strResult.GetBuffer(dwSize),dwSize,strFathName);
	WritePrivateProfileString("CSWaveNum","a10",strResult,sInfo.strPath);

	GetPrivateProfileString("CSWaveNum","a11","12",strResult.GetBuffer(dwSize),dwSize,strFathName);
	WritePrivateProfileString("CSWaveNum","a11",strResult,sInfo.strPath);

	GetPrivateProfileString("CSWaveNum","a12","13",strResult.GetBuffer(dwSize),dwSize,strFathName);
	WritePrivateProfileString("CSWaveNum","a12",strResult,sInfo.strPath);

	GetPrivateProfileString("CSWaveNum","a13","14",strResult.GetBuffer(dwSize),dwSize,strFathName);
	WritePrivateProfileString("CSWaveNum","a13",strResult,sInfo.strPath);

	GetPrivateProfileString("CSWaveNum","a14","15",strResult.GetBuffer(dwSize),dwSize,strFathName);
	WritePrivateProfileString("CSWaveNum","a14",strResult,sInfo.strPath);
	//
	GetPrivateProfileString("CAMERA", "left", "0", strResult.GetBuffer(dwSize), dwSize, strFathName);
	WritePrivateProfileString("CAMERA", "left", strResult, sInfo.strPath);

	GetPrivateProfileString("CAMERA", "top", "0", strResult.GetBuffer(dwSize), dwSize, strFathName);
	WritePrivateProfileString("CAMERA", "top", strResult, sInfo.strPath);

	GetPrivateProfileString("CAMERA", "right", "0", strResult.GetBuffer(dwSize), dwSize, strFathName);
	WritePrivateProfileString("CAMERA", "right", strResult, sInfo.strPath);

	GetPrivateProfileString("CAMERA", "bottom", "0", strResult.GetBuffer(dwSize), dwSize, strFathName);
	WritePrivateProfileString("CAMERA", "bottom", strResult, sInfo.strPath);

	GetPrivateProfileString("CAMERA", "hoffset", "0", strResult.GetBuffer(dwSize), dwSize, strFathName);
	WritePrivateProfileString("CAMERA", "hoffset", strResult, sInfo.strPath);

	GetPrivateProfileString("CAMERA", "voffset", "0", strResult.GetBuffer(dwSize), dwSize, strFathName);
	WritePrivateProfileString("CAMERA", "voffset", strResult, sInfo.strPath);

	pThreadApp->InitDeviceInfo();
	OnBnClickedButtonUpdata();
	MessageBox(pThreadApp->LoadStringMeau(_T("ID_LOAD_FINISH")));	
	
}

BOOL CDlgSet::PreTranslateMessage(MSG* pMsg)
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


void CDlgSet::OnBnClickedButtonPset()
{
	GetDlgItem(IDC_EDIT_N)->GetWindowText(sPInfo.Number);
	GetDlgItem(IDC_EDIT_M)->GetWindowText(sPInfo.Model);
	SetButtonState(&m_BtnPSet);
}


void CDlgSet::OnEnChangeEditM()
{
    SetCButtonState(&m_BtnPSet);
}


void CDlgSet::OnEnChangeEditN()
{
	SetCButtonState(&m_BtnPSet);
}

void CDlgSet::OnBnClickedCheckOffset()
{
	sSInfo.CheckOffset=((CButton*)GetDlgItem(IDC_CHECK_OFFSET))->GetCheck();
}

void CDlgSet::OnBnClickedButtonOffset()
{
	CINScanLiteDlg * pThreadApp=(CINScanLiteDlg*)AfxGetApp()->GetMainWnd();

	OPENFILENAME ofn;
	TCHAR szFile[MAX_PATH] = _T("");
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = m_hWnd;
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = _T("TxtFile(*.txt)\0*.txt");
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_EXPLORER | OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	CString str;


	if (!GetOpenFileName(&ofn))
	{
		return ;
	}

	CString strFathName ;
	strFathName=sInfo.Path+_T("\\ZolixOffSetFile.bm");



	CopyFile(szFile,strFathName,FALSE);
	MessageBox(pThreadApp->LoadStringMeau(_T("ID_LOAD_FINISH")));	
}





void CDlgSet::OnBnClickedButton1()
{
	int nTemp=0;
	CString str;
	GetDlgItem(IDC_EDIT1)->GetWindowText(str);
	SetButtonState(&m_BtnSetLed);
	nTemp=atoi(str)-m_SerialLed.ReadLed(sSInfo.nCOMLED);//  读取当前步数
	sSInfo.LockSet=atoi(str);//  设置要走的步数
	//  lcz new 23/05/23  设置步数  将逗号改为空格
	//if (nTemp<0)
	//{
	//	str.Format(_T("-X %d"),abs(nTemp));
	//}
	//else
	//{
	//	str.Format(_T("+X %d"),abs(nTemp));
	//}

	if (nTemp<0)
	{
		str.Format(_T("-X,%d"),abs(nTemp));
	}
	else
	{
		str.Format(_T("+X,%d"),abs(nTemp));
	}


	m_SerialLed.SetLed(sSInfo.nCOMLED,str);//  电机移动
	CINScanLiteDlg * pDlg=(CINScanLiteDlg*)AfxGetApp()->GetMainWnd();
	pDlg->WriteDeviceInfo();
}

//  连接电机
void CDlgSet::OnBnClickedButtonLcon2()
{
	CString str;
	CINScanLiteDlg * pDlg = (CINScanLiteDlg *)AfxGetApp()->GetMainWnd();

	GetComList_Reg();// 重新查看注册表

	//for(map<int, CString>::iterator it = m_vComInfo.begin(); it != m_vComInfo.end(); it++)
	//{
	//	if((it->second.Find(TEXT("VCP")) != -1) && sInfo.LockConnect == 0)
	//	{
	//		if(sSInfo.nCOMLED == it->first)//  如果电机串口号正确，那么就正常连接
	//		{
				sInfo.LockConnect = m_SerialLed.OpenLed(sSInfo.nCOMLED);
				if (sInfo.LockConnect==1)//  如果相机连接成功
				{
					bool a = m_SerialLed.SetLed(sSInfo.nCOMLED,_T("PX,1"));
					if(a == false)// 如果电脑接了其他的串口线，而且是第一个接，名为vcp0，进行测试
						sInfo.LockConnect = 0;
					bool b =m_SerialLed.SetLed(sSInfo.nCOMLED,_T("NX,1"));
					if(b == false)
					{
						m_SerialLed.CloseLaser(sSInfo.nCOMLED);
						sInfo.LockConnect = 0;
					}
					bool c =m_SerialLed.SetLed(sSInfo.nCOMLED,_T("HX"));
					//if(c == false)
					//	sInfo.LockConnect = 0;
				}
	//		}
	//	}
	//}


	if(sInfo.LockConnect == 0)
	{
		str=pDlg->LoadStringMeau(_T("ID_CAMERA_CONNECT"));
		MessageBox(str);
	}

			//}
			//else
	//		{
	//			sInfo.LockConnect = 0;
	//			str=pDlg->LoadStringMeau(_T("ID_CAMERA_CONNECT"));
	//			MessageBox(str);
	//		}
	//	}
	//}

	
	//// 如果激光器不能连相机的窗口，相机连该串口也连接失败，则给出提示信息
	//if (m_SerialLed.OpenLed(sSInfo.nCOMLED)==FALSE)
	//{
	//	sInfo.LockConnect=0;
	//	str=pDlg->LoadStringMeau(_T("ID_CAMERA_CONNECT"));
	//	MessageBox(str);
	//}

	//m_SerialLed.SetLed(sSInfo.nCOMLED,_T("PX,1"));
	//m_SerialLed.SetLed(sSInfo.nCOMLED,_T("NX,1"));
	//m_SerialLed.SetLed(sSInfo.nCOMLED,_T("HX"));

	OnBnState();
	pDlg->WriteDeviceInfo();
}


void CDlgSet::OnBnClickedButtonLdis2()
{
	m_SerialLed.CloseLaser(sSInfo.nCOMLED);
	sInfo.LockConnect=0;
	OnBnState();
}


void CDlgSet::OnCbnSelchangeComboCom2()
{
	sSInfo.nCOMLED=((CComboBox*)GetDlgItem(IDC_COMBO_COM2))->GetCurSel()+1;
}


void CDlgSet::OnBnClickedCheck3()
{
	sSInfo.isOEM=((CButton*)GetDlgItem(IDC_CHECK3))->GetCheck();
	if (sSInfo.isOEM==0)
	{
		GetDlgItem(IDC_COMBO_COM)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_LCON)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_LDIS)->EnableWindow(FALSE);
	}
	else
	{
		OnBnClickedButtonLdis();
		GetDlgItem(IDC_COMBO_COM)->EnableWindow(TRUE);
		OnBnClickedButtonLcon();
	}

	CINScanLiteDlg * pDlg = (CINScanLiteDlg *)AfxGetApp()->GetMainWnd();
	pDlg->WriteDeviceInfo();
	pDlg->InitDeviceInfo();
}


void CDlgSet::OnCbnSelchangeCombo4()
{
	sSInfo.nSqlMode=((CComboBox*)GetDlgItem(IDC_COMBO4))->GetCurSel();
	CINScanLiteDlg * pDlg = (CINScanLiteDlg *)AfxGetApp()->GetMainWnd();
	pDlg->WriteDeviceInfo();
	pDlg->InitDeviceInfo();
}

void CDlgSet::OnBnClickedButtonLcon3()
{
	// TODO: 在此添加控件通知处理程序代码
	CINScanLiteDlg * pDlg = (CINScanLiteDlg *)AfxGetApp()->GetMainWnd();
	CString str;
	sInfo.PriorConnect=1;
	if(!connect_prior(sSInfo.nComPrior))
	{
		sInfo.PriorConnect=0;
		str=pDlg->LoadStringMeau(_T("ID_PRIOR_CONNECT"));
		MessageBox(str);
	}
	else
	{
		//move_position(0,0);
	}
	OnBnState();
// 	CString str;
// 	sInfo.LConnect=1;
// 	CINScanLiteDlg * pDlg = (CINScanLiteDlg *)AfxGetApp()->GetMainWnd();
// 	if (m_Serial.OpenLaser(sSInfo.nCOM)==FALSE)
// 	{
// 		sInfo.LConnect=0;
// 		str=pDlg->LoadStringMeau(_T("ID_LASER_CONNECT"));
// 		MessageBox(str);
// 	}
// 	OnBnState();
// 	pDlg->WriteDeviceInfo();
}


void CDlgSet::OnCbnSelchangeComboCom3()
{
	// TODO: 在此添加控件通知处理程序代码
	sSInfo.nComPrior = ((CComboBox *)GetDlgItem(IDC_COMBO_COM3))->GetCurSel()+1;
}





void CDlgSet::OnBnClickedButtonLdis3()
{
	// TODO: 在此添加控件通知处理程序代码
	sInfo.PriorConnect=0;
	if(!disconnect_prior())
	{
		sInfo.PriorConnect=1;
		//MessageBox("断开连接电动位移台失败");
	}
	else
	{
		//move_position(0,0);
	}
	OnBnState();
}


void CDlgSet::OnBnClickedButtonReset()
{
	// TODO: 在此添加控件通知处理程序代码
	reset_zero_position_prior();
}


void CDlgSet::OnBnClickedButtonOpenlaser()
{
	
	 //TODO: 在此添加控件通知处理程序代码
	CINScanLiteDlg *pDlgF = (CINScanLiteDlg*)AfxGetApp()->GetMainWnd();

	if(m_bCloseLaser) //  如果激光未开，则开激光，按钮显示关激光
	{
		m_bCloseLaser = false;
		g_bCloseLaser = false;
		Sleep(100);
		pDlgF->m_DlgShow.SetLawerPower(sSInfo.iInitLaserPower, FALSE);
		CString tempTEXT = pDlgF->LoadStringMeau(_T("IDD148_12501"));
		SetDlgItemText(IDC_BUTTON_OpenLaser, tempTEXT);
		SetTimer(88, 100, NULL);//  设置88号定时器来关激光
		Sleep(600);
	}
	else//  如果已开激光，点击关激光，按钮显示开激光
	{
	
		m_bCloseLaser = true;
		KillTimer(88);
		Sleep(100);
		pDlgF->m_DlgShow.SetLawerPower(CloseLaserPow,FALSE);
		CString tempTEXT = pDlgF->LoadStringMeau(_T("IDD148_12502"));
		SetDlgItemText(IDC_BUTTON_OpenLaser, tempTEXT);
	}

}

void CDlgSet::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	switch(nIDEvent)
	{
	case 100:
		OnBnState();
		break;
	}

	if (nIDEvent == 88)
	{
		if(g_bCloseLaser == true)//  外界操作导致须关激光
		{
			CINScanLiteDlg *pDlgF = (CINScanLiteDlg*)AfxGetApp()->GetMainWnd();
			KillTimer(88);
			m_bCloseLaser = true;
			CINScanLiteDlg * pDlg=(CINScanLiteDlg*)AfxGetApp()->GetMainWnd();
			pDlg->m_DlgShow.SetLawerPower(CloseLaserPow,FALSE);
			CString tempTEXT = pDlgF->LoadStringMeau(_T("IDD148_12502"));
			SetDlgItemText(IDC_BUTTON_OpenLaser, tempTEXT);
			g_bCloseLaser = false;
		}
	}

	//if (nIDEvent==8)
	//{
	//	if(g_bUSBDiscon && m_iReconCCD > 1)
	//	{
	//		KillTimer(8);
	//		CINScanLiteDlg * pDlg = (CINScanLiteDlg *)AfxGetApp()->GetMainWnd();
	//		CString str;
	//		str=pDlg->LoadStringMeau(_T("ID_USBDiscon"));
	//		//  更改谱仪连接按钮
	//		pDlg->m_DlgSet.OnBnClickedButtonCdis();
	//		pDlg->m_DlgSet.OnBnState();
	//		m_bAutoRunning=FALSE;//  停止采样
	//		MessageBox(str);
	//		g_bUSBDiscon = false;	
	//		m_iReconCCD = 0;
	//	}
	//}

	CDialog::OnTimer(nIDEvent);
}


void CDlgSet::OnBnClickedRadioWavelenout()
{
	// TODO: 在此添加控件通知处理程序代码
	g_bWaveLenCorr = true;
	g_bCorrNumLen = false;
}


void CDlgSet::OnBnClickedRadioWavenumout()
{
	// TODO: 在此添加控件通知处理程序代码
	g_bWaveLenCorr = false;
	g_bCorrNumLen = true;
}

//  lcz new 23/05/17   根据当前框大小和界面实际框大小对控件进行缩放，修改其top/left/bottom/right四个属性
void CDlgSet::ChangeSize(UINT nID, int x, int y, BOOL isCom) //nID为控件ID，x,y分别为对话框的当前长和宽
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
		rec.bottom = isCom ? 3 * rec.bottom*y / i_dlg_height : rec.bottom*y / i_dlg_height;
		rec.right = rec.right*x / i_dlg_width;
		pWnd->MoveWindow(rec); //伸缩控件
	}
}

//  lcz new 23/05/17  调整SET界面各按钮的大小和位置
void CDlgSet::resize()
{
	CINScanLiteDlg * pDlg = (CINScanLiteDlg *)AfxGetApp()->GetMainWnd();
	ChangeSize(IDC_STATIC_HW, pDlg->i_Set_About_Page_Width, pDlg->i_Set_About_Page_Height);
	ChangeSize(IDC_CHECK1, pDlg->i_Set_About_Page_Width, pDlg->i_Set_About_Page_Height);
	ChangeSize(IDC_STATIC_LASER, pDlg->i_Set_About_Page_Width, pDlg->i_Set_About_Page_Height);
	ChangeSize(IDC_CHECK3, pDlg->i_Set_About_Page_Width, pDlg->i_Set_About_Page_Height);
	ChangeSize(IDC_COMBO_COM, pDlg->i_Set_About_Page_Width, pDlg->i_Set_About_Page_Height);
	ChangeSize(IDC_BUTTON_LCON, pDlg->i_Set_About_Page_Width, pDlg->i_Set_About_Page_Height);
	ChangeSize(IDC_BUTTON_LDIS, pDlg->i_Set_About_Page_Width, pDlg->i_Set_About_Page_Height);

	ChangeSize(IDC_STATIC_CCD, pDlg->i_Set_About_Page_Width, pDlg->i_Set_About_Page_Height);
	ChangeSize(IDC_STATIC_OMNI, pDlg->i_Set_About_Page_Width, pDlg->i_Set_About_Page_Height);
	ChangeSize(IDC_COMBO_OMNI, pDlg->i_Set_About_Page_Width, pDlg->i_Set_About_Page_Height, TRUE);
	ChangeSize(IDC_BUTTON_CCON, pDlg->i_Set_About_Page_Width, pDlg->i_Set_About_Page_Height);
	ChangeSize(IDC_BUTTON_CDIS, pDlg->i_Set_About_Page_Width, pDlg->i_Set_About_Page_Height);

	ChangeSize(IDC_STATIC_LASER2, pDlg->i_Set_About_Page_Width, pDlg->i_Set_About_Page_Height);
	ChangeSize(IDC_STATIC_COM2, pDlg->i_Set_About_Page_Width, pDlg->i_Set_About_Page_Height);
	ChangeSize(IDC_COMBO_COM2, pDlg->i_Set_About_Page_Width, pDlg->i_Set_About_Page_Height);
	ChangeSize(IDC_BUTTON_LCON2, pDlg->i_Set_About_Page_Width, pDlg->i_Set_About_Page_Height);
	ChangeSize(IDC_BUTTON_LDIS2, pDlg->i_Set_About_Page_Width, pDlg->i_Set_About_Page_Height);

	ChangeSize(IDC_STATIC_LASER3, pDlg->i_Set_About_Page_Width, pDlg->i_Set_About_Page_Height);
	ChangeSize(IDC_STATIC_COM3, pDlg->i_Set_About_Page_Width, pDlg->i_Set_About_Page_Height);
	ChangeSize(IDC_COMBO_COM3, pDlg->i_Set_About_Page_Width, pDlg->i_Set_About_Page_Height);
	ChangeSize(IDC_BUTTON_LCON3, pDlg->i_Set_About_Page_Width, pDlg->i_Set_About_Page_Height);
	ChangeSize(IDC_BUTTON_LDIS3, pDlg->i_Set_About_Page_Width, pDlg->i_Set_About_Page_Height);
	ChangeSize(IDC_STATIC_COM4, pDlg->i_Set_About_Page_Width, pDlg->i_Set_About_Page_Height);
	ChangeSize(IDC_BUTTON_RESET, pDlg->i_Set_About_Page_Width, pDlg->i_Set_About_Page_Height);
	       
	ChangeSize(IDC_STATIC_HLMODE, pDlg->i_Set_About_Page_Width, pDlg->i_Set_About_Page_Height);
	ChangeSize(IDC_RADIO_H, pDlg->i_Set_About_Page_Width, pDlg->i_Set_About_Page_Height);
	ChangeSize(IDC_RADIO_L, pDlg->i_Set_About_Page_Width, pDlg->i_Set_About_Page_Height);

	       
	ChangeSize(IDC_STATIC_LASERDEALY, pDlg->i_Set_About_Page_Width, pDlg->i_Set_About_Page_Height);
	ChangeSize(IDC_STATIC_DELAY, pDlg->i_Set_About_Page_Width, pDlg->i_Set_About_Page_Height);
	ChangeSize(IDC_EDIT_DELAY, pDlg->i_Set_About_Page_Width, pDlg->i_Set_About_Page_Height);
	ChangeSize(IDC_STATIC_MS, pDlg->i_Set_About_Page_Width, pDlg->i_Set_About_Page_Height);
	ChangeSize(IDC_BUTTON_SET, pDlg->i_Set_About_Page_Width, pDlg->i_Set_About_Page_Height);
	ChangeSize(IDC_BUTTON_OpenLaser, pDlg->i_Set_About_Page_Width, pDlg->i_Set_About_Page_Height);

	ChangeSize(IDC_STATIC_PARA, pDlg->i_Set_About_Page_Width, pDlg->i_Set_About_Page_Height);
	
	ChangeSize(IDC_STATIC_RANGE3, pDlg->i_Set_About_Page_Width, pDlg->i_Set_About_Page_Height);
	ChangeSize(IDC_STATIC_SETLED, pDlg->i_Set_About_Page_Width, pDlg->i_Set_About_Page_Height);
	ChangeSize(IDC_EDIT1, pDlg->i_Set_About_Page_Width, pDlg->i_Set_About_Page_Height);
	ChangeSize(IDC_BUTTON_SETLED, pDlg->i_Set_About_Page_Width, pDlg->i_Set_About_Page_Height);

	ChangeSize(IDC_STATIC_RANGE2, pDlg->i_Set_About_Page_Width, pDlg->i_Set_About_Page_Height);
	ChangeSize(IDC_STATIC_M, pDlg->i_Set_About_Page_Width, pDlg->i_Set_About_Page_Height);
	ChangeSize(IDC_EDIT_M, pDlg->i_Set_About_Page_Width, pDlg->i_Set_About_Page_Height);
	ChangeSize(IDC_STATIC_N, pDlg->i_Set_About_Page_Width, pDlg->i_Set_About_Page_Height);
	ChangeSize(IDC_EDIT_N, pDlg->i_Set_About_Page_Width, pDlg->i_Set_About_Page_Height);
	ChangeSize(IDC_BUTTON_PSET, pDlg->i_Set_About_Page_Width, pDlg->i_Set_About_Page_Height);

	ChangeSize(IDC_CHECK_BG, pDlg->i_Set_About_Page_Width, pDlg->i_Set_About_Page_Height);
	ChangeSize(IDC_CHECK_OFFSET, pDlg->i_Set_About_Page_Width, pDlg->i_Set_About_Page_Height);
	ChangeSize(IDC_STATIC_SQL, pDlg->i_Set_About_Page_Width, pDlg->i_Set_About_Page_Height);
	ChangeSize(IDC_COMBO4, pDlg->i_Set_About_Page_Width, pDlg->i_Set_About_Page_Height, TRUE);

	ChangeSize(IDC_STATIC_CORRECT, pDlg->i_Set_About_Page_Width, pDlg->i_Set_About_Page_Height);
	ChangeSize(IDC_RADIO_WAVELEN_out, pDlg->i_Set_About_Page_Width, pDlg->i_Set_About_Page_Height);
	ChangeSize(IDC_RADIO_WAVENUM_out, pDlg->i_Set_About_Page_Width, pDlg->i_Set_About_Page_Height);
	ChangeSize(IDC_BUTTON_CORR, pDlg->i_Set_About_Page_Width, pDlg->i_Set_About_Page_Height);

	ChangeSize(IDC_BUTTON_IN, pDlg->i_Set_About_Page_Width, pDlg->i_Set_About_Page_Height);
	ChangeSize(IDC_BUTTON_OFFSET, pDlg->i_Set_About_Page_Width, pDlg->i_Set_About_Page_Height);
	ChangeSize(IDC_BUTTON_CCD, pDlg->i_Set_About_Page_Width, pDlg->i_Set_About_Page_Height);

	ChangeSize(IDC_BTN_CAM_SET1, pDlg->i_Set_About_Page_Width, pDlg->i_Set_About_Page_Height);

	ChangeSize(IDC_CHECK_LED, pDlg->i_Set_About_Page_Width, pDlg->i_Set_About_Page_Height);
	ChangeSize(IDC_COMBO_LED, pDlg->i_Set_About_Page_Width, pDlg->i_Set_About_Page_Height, TRUE);
}


void CDlgSet::OnBnClickedBtnCamSet()
{
	

}


void CDlgSet::OnBnClickedBtnCamSet1()
{
}


void CDlgSet::OnBnClickedCheckLed()
{
	CINScanLiteDlg * pDlg = (CINScanLiteDlg*)AfxGetApp()->GetMainWnd();
	sSInfo.CheckLed = ((CButton*)GetDlgItem(IDC_CHECK_LED))->GetCheck();
	pDlg->m_DlgTest.m_DlgTestCheckJ.SetLedButton(sSInfo.CheckLed);
}

//  选择了相机之后，把它的索引存进配置文件
void CDlgSet::OnCbnSelchangeComboLed()
{
	CINScanLiteDlg * pDlgT = (CINScanLiteDlg*)AfxGetApp()->GetMainWnd();
	// 获取当前相机编号
	sSInfo.ComboLed = ((CComboBox*)GetDlgItem(IDC_COMBO_LED))->GetCurSel();

	// 首先停止播放
	g_bFindCamera = false;
	pDlgT->cap.release();

	// 判断打开方式，如果是opencv方式，则直接打开，如果是其它，如500E方法，则不进行处理！
	char method[1024];
	GetPrivateProfileStringA("CAMERA", "method", "opencv", method, 1024, sInfo.strPath);
	// 不显示直接返回，便于调校
	int show = GetPrivateProfileInt("CAMERA", "show", 1, sInfo.strPath);
	if (show == 0)
	{
		AfxMessageBox("请先修改配置文件开启相机播放权限");
		return;
	}
	// 500E直接返回
	if (strcmp(method, "opencv") != 0)
	{
		::OutputDebugString("使用500E动态库方式进行加载相机！");
		return;
	}
	else
	{
		int b = pDlgT->cap.open(sSInfo.ComboLed, cv::CAP_DSHOW);
		if (!b)//  lcz new 04/23  如果打不开该com口，则认为没找到
		{
			g_bFindCamera = false;
		}
		else
		{
			g_bFindCamera = true;
		}
	}
}


void CDlgSet::OnBnClickedCheck1()
{
	sSInfo.isCheckLed = ((CButton*)GetDlgItem(IDC_CHECK1))->GetCheck();
	if (sSInfo.isCheckLed == 0)
	{
		GetDlgItem(IDC_COMBO_COM2)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_LCON2)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_LDIS2)->EnableWindow(FALSE);
	}
	else
	{
		OnBnClickedButtonLdis2();
		GetDlgItem(IDC_COMBO_COM2)->EnableWindow(TRUE);
		OnBnClickedButtonLcon2();
	}
	CINScanLiteDlg * pDlg = (CINScanLiteDlg *)AfxGetApp()->GetMainWnd();
	pDlg->WriteDeviceInfo();
	pDlg->InitDeviceInfo();
}


void CDlgSet::OnStnClickedStaticCom2()
{
	// TODO: 在此添加控件通知处理程序代码
}
