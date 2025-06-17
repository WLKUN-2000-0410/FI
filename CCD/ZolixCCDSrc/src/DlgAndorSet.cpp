// DlgAndorSet.cpp : 实现文件
//

#include "stdafx.h"
#include "ZolixCCD.h"
#include "DlgAndorSet.h"
#include "afxdialogex.h"
#include "AndorCamera.h"
#include "Language.h"
#include "DlgPassword.h"
#include "DlgAdvAndor.h"
NS_MFC_Res_BEGIN
// CDlgAndorSet 对话框
CDlgAndorSet *g_pDlg=NULL;
UINT CCDImageAndor(LPVOID lParam);

IMPLEMENT_DYNAMIC(CDlgAndorSet, CDialog)

CDlgAndorSet::CDlgAndorSet(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgAndorSet::IDD, pParent)
	, m_StaticX(_T(""))
	, m_StaticY(_T(""))
{
	m_CameraState=FALSE;
	fTime=0.1;
}

CDlgAndorSet::~CDlgAndorSet()
{
}

void CDlgAndorSet::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_BINMIN, iBinMin);	//中心
	DDX_Text(pDX, IDC_EDIT_BINMAX, iBinMax);
	DDX_Text(pDX, IDC_EDIT_TIME, fTime);
	DDX_Text(pDX, IDC_STATIC_X, m_StaticX);
	DDX_Text(pDX, IDC_STATIC_Y, m_StaticY);
	
}


BEGIN_MESSAGE_MAP(CDlgAndorSet, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_TEMP, &CDlgAndorSet::OnBnClickedButtonTemp)
	ON_BN_CLICKED(IDC_BUTTON_BIN, &CDlgAndorSet::OnBnClickedButtonBin)
	ON_CBN_SELCHANGE(IDC_COMBO_MODE, &CDlgAndorSet::OnCbnSelchangeComboMode)
	ON_BN_CLICKED(IDC_BUTTON_ADV, &CDlgAndorSet::OnBnClickedButtonAdv)
	ON_BN_CLICKED(IDC_BUTTON_IMAGE, &CDlgAndorSet::OnBnClickedButtonImage)
	ON_EN_CHANGE(IDC_EDIT_TIME, &CDlgAndorSet::OnEnChangeEditTime)
	ON_BN_CLICKED(IDC_BUTTON_STOP, &CDlgAndorSet::OnBnClickedButtonStop)
	ON_WM_CLOSE()
	ON_CBN_SELCHANGE(IDC_COMBO_GAIN, &CDlgAndorSet::OnCbnSelchangeComboGain)
	ON_CBN_SELCHANGE(IDC_COMBO_SPEED, &CDlgAndorSet::OnCbnSelchangeComboSpeed)
	ON_CBN_SELCHANGE(IDC_COMBO_CCDSPEED, &CDlgAndorSet::OnCbnSelchangeComboCcdspeed)
	ON_BN_CLICKED(IDC_CHECK_CURSOR, &CDlgAndorSet::OnBnClickedCheckCursor)
	ON_BN_CLICKED(IDC_BUTTON_BMP, &CDlgAndorSet::OnBnClickedButtonBmp)
	ON_WM_CTLCOLOR()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_H, &CDlgAndorSet::OnBnClickedButtonH)
	ON_BN_CLICKED(IDC_CHECK_AUTO, &CDlgAndorSet::OnBnClickedCheckShutter)
	ON_BN_CLICKED(IDC_BUTTON_CLOSE, &CDlgAndorSet::OnBnClickeButtonClose)
END_MESSAGE_MAP()
int WriteToLog(char* str)
{
	FILE* pfile;
	fopen_s(&pfile, "D:\\01wlog.txt", "a+");   //FILE_PATH 是个宏, 是文件的位置,如#define FILE_PATH "D:\\FILog.txt"

	if (pfile == NULL)
	{
		return -1;
	}

	char szDateTime[20];
	SYSTEMTIME tm;
	GetLocalTime(&tm);
	sprintf(szDateTime, "%02d:%02d:%02d.%03d", tm.wHour, tm.wMinute, tm.wSecond, tm.wMilliseconds);
	fprintf_s(pfile, "%s:%s\n", szDateTime, str);

	fclose(pfile);

	return 0;
}
BOOL CDlgAndorSet::OnInitDialog()
{
	CDialog::OnInitDialog();
	pAndorCamera->ReadAllIni();
	InitComboGain();
	InitComboSpeed();
	InitComboPixSpeed();
	InitTemp();
	InitFVB();
	GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_H)->EnableWindow(FALSE);
	
	CLanguage mLag;
	CDialog *pDlg;
	pDlg=(CDialog *)this;
	mLag.UpdateLanguage(pDlg,ID_DLG_A,mLag.szDig[nLanguage],g_strPath);
	
	g_pDlg = this;
	
	CRect rect;
	GetDlgItem(IDC_STATIC_BMP)->GetWindowRect(&rect);
	GetDlgItem(IDC_STATIC_BMP)->ModifyStyle(0,WS_CLIPSIBLINGS|WS_CLIPCHILDREN);
	ScreenToClient(&rect);
	dlgImage.Create(ID_DLG_IMAGE,this);

	::SetWindowPos(dlgImage.m_hWnd,HWND_TOP,rect.left,rect.top,rect.Width(),rect.Height(),SWP_SHOWWINDOW|SWP_FRAMECHANGED);
	dlgImage.ModifyStyle(0,WS_CLIPSIBLINGS);
	dlgImage.SetParent(this);
	
	((CButton *)GetDlgItem(IDC_CHECK_AUTO))->SetCheck(stuAndorCCD.ShutterMode);
	pAndorCamera->SetShutterAuto(stuAndorCCD.ShutterMode);

	SetString(0,0);
	SetTimer(1,100,NULL);
	SetDlgItemText(IDC_EDIT_ADV_GHOST, stuAndorCCD.ghostImageStrRelative);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}
void CDlgAndorSet::OnBnClickedButtonTemp()
{
	CString str;
	GetDlgItem(IDC_EDIT_TEMP)->GetWindowText(str);
	pAndorCamera->SetTemper(atoi(str));
	stuAndorCCD.nTemp=atoi(str);
}
void CDlgAndorSet::OnBnClickedButtonBin()
{
	UpdateData(TRUE);
	if (iBinMax<0||iBinMax>=stuAndorCCD.nGYPix||iBinMin<0||iBinMin>stuAndorCCD.nGYPix)
	{
		return;
	}
	if (iBinMax<iBinMin)
	{
		int temp=iBinMin;
		iBinMin=iBinMax;
		iBinMax=temp;
	}
	stuAndorCCD.BinMax=iBinMax;
	stuAndorCCD.BinMin=iBinMin;
	UpdateData(FALSE);
}
void CDlgAndorSet::InitComboGain()
{
	CString str;
	float fGain;
	((CComboBox *)GetDlgItem(IDC_COMBO_GAIN))->ResetContent();
	for (int i=0;i<pAndorCamera->GetGainNum();i++)
	{
		pAndorCamera->GetGain(i,&fGain);
		str.Format(_T("%.1fX"),fGain);
		((CComboBox *)GetDlgItem(IDC_COMBO_GAIN))->AddString(str);
	}

	// lcz new 22/10/24
	g_iCurrentGain = stuAndorCCD.iGain;// 记录从配置文件里读取到的增益值，但不知道当前的增益值

	((CComboBox *)GetDlgItem(IDC_COMBO_GAIN))->SetCurSel(stuAndorCCD.iGain);
}
void CDlgAndorSet::InitComboSpeed()
{
	CString str;
	float fSpeed;
	((CComboBox *)GetDlgItem(IDC_COMBO_SPEED))->ResetContent();
	for (int i=0;i<pAndorCamera->GetSpeedNum();i++)
	{
		pAndorCamera->GetSpeed(0,i,&fSpeed);
		str.Format(_T("%.1f kHz"),fSpeed*1000);
		((CComboBox *)GetDlgItem(IDC_COMBO_SPEED))->AddString(str);
	}
	((CComboBox *)GetDlgItem(IDC_COMBO_SPEED))->SetCurSel(stuAndorCCD.iSInedx);
}
void CDlgAndorSet::InitComboPixSpeed()
{
	CString str;
	float fSpeed;
	((CComboBox *)GetDlgItem(IDC_COMBO_CCDSPEED))->ResetContent();
	for (int i=0;i<pAndorCamera->GetPixSpeedNum();i++)
	{
		pAndorCamera->GetPixSpeed(i,&fSpeed);
		str.Format(_T("%.2f"),fSpeed);
		((CComboBox *)GetDlgItem(IDC_COMBO_CCDSPEED))->AddString(str);
	}
	((CComboBox *)GetDlgItem(IDC_COMBO_CCDSPEED))->SetCurSel(stuAndorCCD.iPixSInedx);
}
void CDlgAndorSet::InitTemp()
{
	CString str;
	str.Format(_T("%d"),stuAndorCCD.nTemp);
	GetDlgItem(IDC_EDIT_TEMP)->SetWindowText(str);
}
void CDlgAndorSet::InitFVB()
{
	((CComboBox *)GetDlgItem(IDC_COMBO_MODE))->ResetContent();
	((CComboBox *)GetDlgItem(IDC_COMBO_MODE))->AddString(_T("FVB"));
	((CComboBox *)GetDlgItem(IDC_COMBO_MODE))->AddString(_T("Multi"));
	((CComboBox *)GetDlgItem(IDC_COMBO_MODE))->SetCurSel(stuAndorCCD.nReadMode);
	ShowBin(stuAndorCCD.nReadMode);
}
void CDlgAndorSet::ShowBin(int nMode)
{
	GetDlgItem(IDC_STATIC_BIN)->ShowWindow(nMode);
	GetDlgItem(IDC_STATIC_TO)->ShowWindow(nMode);
	GetDlgItem(IDC_BUTTON_BIN)->ShowWindow(nMode);
	GetDlgItem(IDC_EDIT_BINMIN)->ShowWindow(nMode);
	GetDlgItem(IDC_EDIT_BINMAX)->ShowWindow(nMode);
	CString str;
	str.Format(_T("%d"),stuAndorCCD.BinMin);
	GetDlgItem(IDC_EDIT_BINMIN)->SetWindowText(str);
	str.Format(_T("%d"),stuAndorCCD.BinMax);
	GetDlgItem(IDC_EDIT_BINMAX)->SetWindowText(str);
}
void CDlgAndorSet::OnCbnSelchangeComboMode()
{
	int nMode;
	nMode=((CComboBox *)GetDlgItem(IDC_COMBO_MODE))->GetCurSel();
	stuAndorCCD.nReadMode=nMode;
	ShowBin(stuAndorCCD.nReadMode);
}
void CDlgAndorSet::OnCbnSelchangeComboGain()
{
	stuAndorCCD.iGain=((CComboBox *)GetDlgItem(IDC_COMBO_GAIN))->GetCurSel();
	pAndorCamera->SetGain(stuAndorCCD.iGain);
}
void CDlgAndorSet::OnCbnSelchangeComboSpeed()
{
	stuAndorCCD.iSInedx=((CComboBox *)GetDlgItem(IDC_COMBO_SPEED))->GetCurSel();
	pAndorCamera->SetSpeed(0,stuAndorCCD.iSInedx);
}


void CDlgAndorSet::OnCbnSelchangeComboCcdspeed()
{
	stuAndorCCD.iPixSInedx=((CComboBox *)GetDlgItem(IDC_COMBO_CCDSPEED))->GetCurSel();
	pAndorCamera->SetPixSpeed(stuAndorCCD.iPixSInedx);
}
void CDlgAndorSet::OnBnClickedButtonAdv()
{
	CDlgPassword dlg;
	if (dlg.DoModal()==IDOK)
	{
		CDlgAdvAndor dlg;
		dlg.DoModal();
	}
}
void CDlgAndorSet::OnEnChangeEditTime()
{
	UpdateData(TRUE);
}
void CDlgAndorSet::OnBnClickedButtonImage()
{
	//  lcz new 22/10/24 如果开始采集
	g_bStartDetect = true;

	m_CameraState=TRUE;
	SetButtomState(m_CameraState);
	AfxBeginThread(CCDImageAndor,(LPVOID)this);
}
void CDlgAndorSet::OnBnClickedButtonStop()
{
	//  lcz new 22/10/24
	g_bStartDetect = false;

	m_CameraState=FALSE;
	pAndorCamera->SetImageAccState(m_CameraState);
	SetButtomState(m_CameraState);
}


UINT CCDImageAndor(LPVOID lParam)
{
	pAndorCamera->SetImageMode(g_pDlg->fTime);
	int x,y;
	pAndorCamera->GetDevSize(&x,&y);
	pAndorCamera->SetImageAccState(g_pDlg->m_CameraState);

	long *lpData1=new long[x*y];
	BYTE *pData1=new BYTE[x*y*3];
	memset(lpData1,0,x*y*sizeof(long));
	memset(pData1,0,x*y*3);

	int m_acqStatus;

	while (g_pDlg->m_CameraState==TRUE)
	{
        g_pDlg->dlgImage.SetPicSize(x,y);
		pAndorCamera->ImageDataAcquisition(lpData1,pData1,x,y);	
		memcpy(g_pDlg->dlgImage.pBuffer,pData1,x*y*3);
	    g_pDlg->dlgImage.OnDisplay();
	}
	
	delete []lpData1;
	delete []pData1;
	return 0;

}

void CDlgAndorSet::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	// lcz new 22/10/24
	if(g_bStartDetect)
		OnBnClickedButtonStop();
	g_bOpenDlg = false;
	//OnBnClickedButtonStop();
	//Sleep(500);
	
	pAndorCamera->WriteAllIni();

	// lcz new 22/10/24
	Sleep(1000);

	KillTimer(1);
	CDialog::OnClose();
}

void CDlgAndorSet::OnBnClickedButtonBmp()
{
	if (m_CameraState==FALSE)
	{
		dlgImage.SavePicBmp();
	}

}
void CDlgAndorSet::SetString(int x,int y)
{
	CString str;
	str.Format(_T("X:%4d"),x);
	m_StaticX=str;
	str.Format(_T("Y:%4d"),y);
	m_StaticY=str;

}
void CDlgAndorSet::OnBnClickedCheckCursor()
{
	int state =((CButton *)GetDlgItem(IDC_CHECK_CURSOR))->GetCheck();
	dlgImage.SetMark(state);
}
BOOL CDlgAndorSet::PreTranslateMessage(MSG* pMsg)
{
		CRect rect;
		dlgImage.GetWindowRect(&rect);
		if (rect.PtInRect(pMsg->pt))
		{
			int x,y;
			dlgImage.GetPoint(&x,&y);
			SetString(x,y);
		}
	if(pMsg->wParam==VK_RETURN || pMsg->wParam==VK_ESCAPE ) 
		return TRUE;
	else    
		return CDialog::PreTranslateMessage(pMsg);
}
void CDlgAndorSet::SetButtomState(BOOL bSta)
{
	GetDlgItem(IDC_BUTTON_BIN)->EnableWindow(!bSta);
	GetDlgItem(IDC_COMBO_MODE)->EnableWindow(!bSta);
	GetDlgItem(IDC_BUTTON_TEMP)->EnableWindow(!bSta);
	GetDlgItem(IDC_BUTTON_BMP)->EnableWindow(!bSta);
	GetDlgItem(IDC_BUTTON_IMAGE)->EnableWindow(!bSta);
	GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(bSta);
	GetDlgItem(IDC_BUTTON_H)->EnableWindow(bSta);
	GetDlgItem(IDC_BUTTON_ADV)->EnableWindow(!bSta);	
	GetDlgItem(IDC_EDIT_BINMAX)->EnableWindow(!bSta);
	GetDlgItem(IDC_EDIT_BINMIN)->EnableWindow(!bSta);
}
HBRUSH CDlgAndorSet::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	switch (pWnd->GetDlgCtrlID())
	{
		case IDC_STATIC_X:
			pDC->SetBkMode(TRANSPARENT);
			break;	
		case IDC_STATIC_Y:
			pDC->SetBkMode(TRANSPARENT);
			break;	
		default:
			break;
	}
	
	//return (HBRUSH)GetStockObject(NULL_BRUSH);
	return hbr;
}
void CDlgAndorSet::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent==1)
	{
		GetDlgItem(IDC_STATIC_X)->SetWindowText(m_StaticX);
		GetDlgItem(IDC_STATIC_Y)->SetWindowText(m_StaticY);
	}

	CDialog::OnTimer(nIDEvent);
}
void CDlgAndorSet::OnBnClickedButtonH()
{
	stuAndorCCD.HMirror=!stuAndorCCD.HMirror;
}
void CDlgAndorSet::OnBnClickedCheckShutter()
{
	int nCheck =((CButton *)GetDlgItem(IDC_CHECK_AUTO))->GetCheck();
	CString str;
	int nopen,nclose;
	GetDlgItem(IDC_EDIT1)->GetWindowText(str);
	nopen=atoi(str);
	GetDlgItem(IDC_EDIT2)->GetWindowText(str);
	nclose=atoi(str);
	pAndorCamera->SetShutterAuto(nCheck,0,0);
	stuAndorCCD.ShutterMode=nCheck;
}
void CDlgAndorSet::OnBnClickeButtonClose()
{
	pAndorCamera->SetShutterAuto(2);
}

// CDlgAndorSet 消息处理程序
NS_MFC_Res_END