// DlgAndorSet.cpp : 实现文件
//

#include "stdafx.h"
#include "ZolixCCD.h"
#include "DlgGreateyesSet.h"
#include "afxdialogex.h"
#include "GreateyesCamera.h"
#include "Language.h"
#include "DlgPassword.h"
#include "DlgAdvGreateyes.h"
NS_MFC_Res_BEGIN
// CDlgAndorSet 对话框
CDlgGreateyesSet *g_pDlg=NULL;
UINT CCDImageGreateyes(LPVOID lParam);

IMPLEMENT_DYNAMIC(CDlgGreateyesSet, CDialog)

CDlgGreateyesSet::CDlgGreateyesSet(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgGreateyesSet::IDD, pParent)
	, m_StaticX(_T(""))
	, m_StaticY(_T(""))
{
	m_CameraState=FALSE;
	fTime=0.1;
}

CDlgGreateyesSet::~CDlgGreateyesSet()
{
}

void CDlgGreateyesSet::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_BINMIN, iBinMin);	//中心
	DDX_Text(pDX, IDC_EDIT_BINMAX, iBinMax);
	DDX_Text(pDX, IDC_EDIT_TIME, fTime);
	DDX_Text(pDX, IDC_STATIC_X, m_StaticX);
	DDX_Text(pDX, IDC_STATIC_Y, m_StaticY);
}


BEGIN_MESSAGE_MAP(CDlgGreateyesSet, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_TEMP, &CDlgGreateyesSet::OnBnClickedButtonTemp)
	ON_BN_CLICKED(IDC_BUTTON_BIN, &CDlgGreateyesSet::OnBnClickedButtonBin)
	ON_CBN_SELCHANGE(IDC_COMBO_MODE, &CDlgGreateyesSet::OnCbnSelchangeComboMode)
	ON_BN_CLICKED(IDC_BUTTON_ADV, &CDlgGreateyesSet::OnBnClickedButtonAdv)
	ON_BN_CLICKED(IDC_BUTTON_IMAGE, &CDlgGreateyesSet::OnBnClickedButtonImage)
	ON_EN_CHANGE(IDC_EDIT_TIME, &CDlgGreateyesSet::OnEnChangeEditTime)
	ON_BN_CLICKED(IDC_BUTTON_STOP, &CDlgGreateyesSet::OnBnClickedButtonStop)
	ON_WM_CLOSE()
	ON_CBN_SELCHANGE(IDC_COMBO_GAIN, &CDlgGreateyesSet::OnCbnSelchangeComboGain)
	ON_CBN_SELCHANGE(IDC_COMBO_SPEED, &CDlgGreateyesSet::OnCbnSelchangeComboSpeed)
	ON_CBN_SELCHANGE(IDC_COMBO_CCDSPEED, &CDlgGreateyesSet::OnCbnSelchangeComboCcdspeed)
	ON_BN_CLICKED(IDC_CHECK_CURSOR, &CDlgGreateyesSet::OnBnClickedCheckCursor)
	ON_BN_CLICKED(IDC_BUTTON_BMP, &CDlgGreateyesSet::OnBnClickedButtonBmp)
	ON_WM_CTLCOLOR()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_H, &CDlgGreateyesSet::OnBnClickedButtonH)
END_MESSAGE_MAP()

BOOL CDlgGreateyesSet::OnInitDialog()
{
	CDialog::OnInitDialog();
	pGreateyesCamera->ReadAllIni();
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

	SetString(0,0);
	SetTimer(1,100,NULL);
		
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}
void CDlgGreateyesSet::OnBnClickedButtonTemp()
{
	CString str;
	GetDlgItem(IDC_EDIT_TEMP)->GetWindowText(str);
	pGreateyesCamera->SetTemper(atoi(str));
	stuGreateyesCCD.nTemp=atoi(str);
}
void CDlgGreateyesSet::OnBnClickedButtonBin()
{
	UpdateData(TRUE);
	if (iBinMax<0||iBinMax>=stuGreateyesCCD.nGYPix||iBinMin<0||iBinMin>stuGreateyesCCD.nGYPix)
	{
		return;
	}
	if (iBinMax<iBinMin)
	{
		int temp=iBinMin;
		iBinMin=iBinMax;
		iBinMax=temp;
	}
	stuGreateyesCCD.BinMax=iBinMax;
	stuGreateyesCCD.BinMin=iBinMin;
	UpdateData(FALSE);
}
void CDlgGreateyesSet::InitComboGain()
{
	CString str;
	float fGain;
	((CComboBox *)GetDlgItem(IDC_COMBO_GAIN))->ResetContent();
	for (int i=0;i<pGreateyesCamera->GetGainNum();i++)
	{
		pGreateyesCamera->GetGain(i,&fGain);
		str.Format(_T("%.1fX"),fGain);
		((CComboBox *)GetDlgItem(IDC_COMBO_GAIN))->AddString(str);
	}
	((CComboBox *)GetDlgItem(IDC_COMBO_GAIN))->SetCurSel(stuGreateyesCCD.iGain);
}
void CDlgGreateyesSet::InitComboSpeed()
{
	CString str;
	int fSpeed;
	((CComboBox *)GetDlgItem(IDC_COMBO_SPEED))->ResetContent();	
	pGreateyesCamera->GetSpeed(fSpeed);
   ((CComboBox *)GetDlgItem(IDC_COMBO_SPEED))->AddString("3 MHz");
   ((CComboBox *)GetDlgItem(IDC_COMBO_SPEED))->AddString("1 MHz");
   ((CComboBox *)GetDlgItem(IDC_COMBO_SPEED))->AddString("500 kHz");
   ((CComboBox *)GetDlgItem(IDC_COMBO_SPEED))->AddString("250 kHz");
   ((CComboBox *)GetDlgItem(IDC_COMBO_SPEED))->AddString("100 kHz");
   ((CComboBox *)GetDlgItem(IDC_COMBO_SPEED))->AddString("50 kHz");
   ((CComboBox *)GetDlgItem(IDC_COMBO_SPEED))->SetCurSel(fSpeed);
}
void CDlgGreateyesSet::InitComboPixSpeed()
{
	/*CString str;
	float fSpeed;
	((CComboBox *)GetDlgItem(IDC_COMBO_CCDSPEED))->ResetContent();
	for (int i=0;i<pGreateyesCamera->GetPixSpeedNum();i++)
	{
		pGreateyesCamera->GetPixSpeed(i,&fSpeed);
		str.Format(_T("%.2f"),fSpeed);
		((CComboBox *)GetDlgItem(IDC_COMBO_CCDSPEED))->AddString(str);
	}
	((CComboBox *)GetDlgItem(IDC_COMBO_CCDSPEED))->SetCurSel(stuGreateyesCCD.iPixSInedx);*/
}
void CDlgGreateyesSet::InitTemp()
{
	CString str;
	str.Format(_T("%d"),stuGreateyesCCD.nTemp);
	GetDlgItem(IDC_EDIT_TEMP)->SetWindowText(str);
}
void CDlgGreateyesSet::InitFVB()
{
	((CComboBox *)GetDlgItem(IDC_COMBO_MODE))->ResetContent();
	((CComboBox *)GetDlgItem(IDC_COMBO_MODE))->AddString(_T("FVB"));
	((CComboBox *)GetDlgItem(IDC_COMBO_MODE))->AddString(_T("Multi"));
	((CComboBox *)GetDlgItem(IDC_COMBO_MODE))->SetCurSel(stuGreateyesCCD.nReadMode);
	ShowBin(stuGreateyesCCD.nReadMode);
}
void CDlgGreateyesSet::ShowBin(int nMode)
{
	GetDlgItem(IDC_STATIC_BIN)->ShowWindow(nMode);
	GetDlgItem(IDC_STATIC_TO)->ShowWindow(nMode);
	GetDlgItem(IDC_BUTTON_BIN)->ShowWindow(nMode);
	GetDlgItem(IDC_EDIT_BINMIN)->ShowWindow(nMode);
	GetDlgItem(IDC_EDIT_BINMAX)->ShowWindow(nMode);
	CString str;
	str.Format(_T("%d"),stuGreateyesCCD.BinMin);
	GetDlgItem(IDC_EDIT_BINMIN)->SetWindowText(str);
	str.Format(_T("%d"),stuGreateyesCCD.BinMax);
	GetDlgItem(IDC_EDIT_BINMAX)->SetWindowText(str);
}
void CDlgGreateyesSet::OnCbnSelchangeComboMode()
{
	int nMode;
	nMode=((CComboBox *)GetDlgItem(IDC_COMBO_MODE))->GetCurSel();
	stuGreateyesCCD.nReadMode=nMode;
	ShowBin(stuGreateyesCCD.nReadMode);
}
void CDlgGreateyesSet::OnCbnSelchangeComboGain()
{
	stuGreateyesCCD.iGain=((CComboBox *)GetDlgItem(IDC_COMBO_GAIN))->GetCurSel();
	pGreateyesCamera->SetGain(stuGreateyesCCD.iGain);
}
void CDlgGreateyesSet::OnCbnSelchangeComboSpeed()
{
	stuGreateyesCCD.iSInedx=((CComboBox *)GetDlgItem(IDC_COMBO_SPEED))->GetCurSel();
	pGreateyesCamera->SetSpeed(stuGreateyesCCD.iSInedx);
}


void CDlgGreateyesSet::OnCbnSelchangeComboCcdspeed()
{
	/*stuGreateyesCCD.iPixSInedx=((CComboBox *)GetDlgItem(IDC_COMBO_CCDSPEED))->GetCurSel();
	pGreateyesCamera->SetPixSpeed(stuGreateyesCCD.iPixSInedx);*/
}
void CDlgGreateyesSet::OnBnClickedButtonAdv()
{
	CDlgPassword dlg;
	if (dlg.DoModal()==IDOK)
	{
		CDlgAdvGreateyes dlg;
		dlg.DoModal();
	}
}
void CDlgGreateyesSet::OnEnChangeEditTime()
{
	UpdateData(TRUE);
}
void CDlgGreateyesSet::OnBnClickedButtonImage()
{
	m_CameraState=TRUE;
	SetButtomState(m_CameraState);
	AfxBeginThread(CCDImageGreateyes,(LPVOID)this);
}
void CDlgGreateyesSet::OnBnClickedButtonStop()
{
	m_CameraState=FALSE;
	pGreateyesCamera->SetImageAccState(m_CameraState);
	SetButtomState(m_CameraState);
}


UINT CCDImageGreateyes(LPVOID lParam)
{
	pGreateyesCamera->SetImageMode(g_pDlg->fTime);
	int x,y;
	pGreateyesCamera->GetDevSize(&x,&y);
	pGreateyesCamera->SetImageAccState(g_pDlg->m_CameraState);
	int *lpData1=new int[x*y];
	BYTE *pData1=new BYTE[x*y*3];
	memset(lpData1,0,x*y*sizeof(int));
	memset(pData1,0,x*y*3);
	int m_acqStatus;

	while (g_pDlg->m_CameraState==TRUE)
	{
        g_pDlg->dlgImage.SetPicSize(x,y);
		pGreateyesCamera->ImageDataAcquisition(lpData1,pData1,x,y);	
		memcpy(g_pDlg->dlgImage.pBuffer,pData1,x*y*3);
	    g_pDlg->dlgImage.OnDisplay();
	}
	
	delete [] lpData1;
	delete [] pData1;
	return 0;

}
void CDlgGreateyesSet::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	OnBnClickedButtonStop();
	Sleep(500);
	pGreateyesCamera->WriteAllIni();
	KillTimer(1);
	CDialog::OnClose();
}

void CDlgGreateyesSet::OnBnClickedButtonBmp()
{
	if (m_CameraState==FALSE)
	{
		dlgImage.SavePicBmp();
	}

}
void CDlgGreateyesSet::SetString(int x,int y)
{
	CString str;
	str.Format(_T("X:%4d"),x);
	m_StaticX=str;
	str.Format(_T("Y:%4d"),y);
	m_StaticY=str;

}
void CDlgGreateyesSet::OnBnClickedCheckCursor()
{
	int state =((CButton *)GetDlgItem(IDC_CHECK_CURSOR))->GetCheck();
	dlgImage.SetMark(state);
}
BOOL CDlgGreateyesSet::PreTranslateMessage(MSG* pMsg)
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
void CDlgGreateyesSet::SetButtomState(BOOL bSta)
{
	/*GetDlgItem(IDC_BUTTON_BIN)->EnableWindow(!bSta);
	GetDlgItem(IDC_COMBO_MODE)->EnableWindow(!bSta);*/
	GetDlgItem(IDC_BUTTON_TEMP)->EnableWindow(!bSta);
	GetDlgItem(IDC_BUTTON_BMP)->EnableWindow(!bSta);
	GetDlgItem(IDC_BUTTON_IMAGE)->EnableWindow(!bSta);
	GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(bSta);
	GetDlgItem(IDC_BUTTON_H)->EnableWindow(bSta);
	GetDlgItem(IDC_BUTTON_ADV)->EnableWindow(!bSta);	
	//GetDlgItem(IDC_EDIT_BINMAX)->EnableWindow(!bSta);
	//GetDlgItem(IDC_EDIT_BINMIN)->EnableWindow(!bSta);
}
HBRUSH CDlgGreateyesSet::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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
void CDlgGreateyesSet::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent==1)
	{
		GetDlgItem(IDC_STATIC_X)->SetWindowText(m_StaticX);
		GetDlgItem(IDC_STATIC_Y)->SetWindowText(m_StaticY);
	}

	CDialog::OnTimer(nIDEvent);
}
void CDlgGreateyesSet::OnBnClickedButtonH()
{
	stuGreateyesCCD.HMirror=!stuGreateyesCCD.HMirror;
}

// CDlgAndorSet 消息处理程序
NS_MFC_Res_END