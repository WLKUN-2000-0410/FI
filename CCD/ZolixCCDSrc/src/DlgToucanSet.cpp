// DlgAndorSet.cpp : 实现文件
//

#include "stdafx.h"
#include "ZolixCCD.h"
#include "DlgToucanSet.h"
#include "afxdialogex.h"
#include "ToucanCamera.h"
#include "Language.h"
#include "DlgPassword.h"
#include "DlgAdvToucan.h"

NS_MFC_Res_BEGIN
// CDlgMitySet 对话框
CDlgToucanSet *g_pDlg=NULL;
UINT CCDImageToucan(LPVOID lParam);

IMPLEMENT_DYNAMIC(CDlgToucanSet, CDialog)

CDlgToucanSet::CDlgToucanSet(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgToucanSet::IDD, pParent)
	, m_StaticX(_T(""))
	, m_StaticY(_T(""))
{
	m_CameraState=FALSE;
	fTime=0.1;
}

CDlgToucanSet::~CDlgToucanSet()
{
}

void CDlgToucanSet::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_BINMIN, iBinMin);	//中心
	DDX_Text(pDX, IDC_EDIT_BINMAX, iBinMax);
	DDX_Text(pDX, IDC_EDIT_TIME, fTime);
	DDX_Text(pDX, IDC_STATIC_X, m_StaticX);
	DDX_Text(pDX, IDC_STATIC_Y, m_StaticY);
}


BEGIN_MESSAGE_MAP(CDlgToucanSet, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_TEMP, &CDlgToucanSet::OnBnClickedButtonTemp)
	ON_BN_CLICKED(IDC_BUTTON_BIN, &CDlgToucanSet::OnBnClickedButtonBin)
	ON_CBN_SELCHANGE(IDC_COMBO_MODE, &CDlgToucanSet::OnCbnSelchangeComboMode)
	ON_BN_CLICKED(IDC_BUTTON_ADV, &CDlgToucanSet::OnBnClickedButtonAdv)
	ON_BN_CLICKED(IDC_BUTTON_IMAGE, &CDlgToucanSet::OnBnClickedButtonImage)
	ON_EN_CHANGE(IDC_EDIT_TIME, &CDlgToucanSet::OnEnChangeEditOffset)
	ON_BN_CLICKED(IDC_BUTTON_STOP, &CDlgToucanSet::OnBnClickedButtonStop)
	ON_WM_CLOSE()
	ON_CBN_SELCHANGE(IDC_COMBO_GAIN, &CDlgToucanSet::OnCbnSelchangeComboGain)
	ON_BN_CLICKED(IDC_CHECK_CURSOR, &CDlgToucanSet::OnBnClickedCheckCursor)
	ON_BN_CLICKED(IDC_BUTTON_BMP, &CDlgToucanSet::OnBnClickedButtonBmp)
	ON_WM_CTLCOLOR()
	ON_WM_TIMER()
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_BUTTON_H, &CDlgToucanSet::OnBnClickedButtonH)
END_MESSAGE_MAP()

BOOL CDlgToucanSet::OnInitDialog()
{
	CDialog::OnInitDialog();
	pToucanCamera->ReadAllIni();
	InitComboGain();
	InitOffset();
	InitFVB();
	GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_H)->EnableWindow(FALSE);
	CLanguage mLag;
	CDialog *pDlg;
	pDlg=(CDialog *)this;
	mLag.UpdateLanguage(pDlg,ID_DLG_TOU,mLag.szDig[nLanguage],g_strPath);

	g_pDlg = this;

	CRect rect;
	GetDlgItem(IDC_STATIC_BMP)->GetWindowRect(&rect);
	GetDlgItem(IDC_STATIC_BMP)->ModifyStyle(0,WS_CLIPSIBLINGS|WS_CLIPCHILDREN);
	ScreenToClient(&rect);
	dlgImage.Create(ID_DLG_IMAGE,this);

	::SetWindowPos(dlgImage.m_hWnd,HWND_TOP,rect.left,rect.top,rect.Width(),rect.Height(),SWP_SHOWWINDOW|SWP_FRAMECHANGED);
	dlgImage.ModifyStyle(0,WS_CLIPSIBLINGS);
	dlgImage.SetParent(this);

	ManageZolixIni->ReadIni();
	SetString(0,0);
	SetTimer(1,100,NULL);

	CString str;
	int nBin=stuToucanCCD.BinMax-stuToucanCCD.BinMin+1;
	str.Format("%d",nBin);
	GetDlgItem(IDC_COMBO_BIN)->SetWindowText(str);
	
	pToucanCamera->TerminateData();
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}
void CDlgToucanSet::OnBnClickedButtonTemp()
{
	CString str;
	GetDlgItem(IDC_EDIT_TEMP)->GetWindowText(str);
	pToucanCamera->SetTemper(atoi(str));
	stuToucanCCD.nTemp=atoi(str);
}
void CDlgToucanSet::OnBnClickedButtonBin()
{
	UpdateData(TRUE);
	CString str;
	GetDlgItem(IDC_COMBO_BIN)->GetWindowText(str);

	
	iBinMax=iBinMin+atoi(str)-1;
	if (iBinMax<0||iBinMax>=stuToucanCCD.nGYPix||iBinMin<0||iBinMin>stuToucanCCD.nGYPix)
	{
		return;
	}
	if (iBinMax<iBinMin)
	{
		int temp=iBinMin;
		iBinMin=iBinMax;
		iBinMax=temp;
	}

	//CString str;
	GetDlgItem(IDC_COMBO_BIN)->GetWindowText(str);

	stuToucanCCD.BinMax=iBinMin+atoi(str)-1;
	//stuToucanCCD.BinMax=iBinMax;
	stuToucanCCD.BinMin=iBinMin;

	//pToucanCamera->SetBinning(0);



	UpdateData(FALSE);
}
void CDlgToucanSet::InitComboGain()
{
	/*CString str;
	float fGain[8];
	int Num=0,CurNum=0;
	((CComboBox *)GetDlgItem(IDC_COMBO_GAIN))->ResetContent();
	pToucanCamera->GetGain(&Num,fGain,&CurNum);
	for (int i=0;i<Num;i++)
	{
	str.Format(_T("%.1fX"),fGain[i]);
	((CComboBox *)GetDlgItem(IDC_COMBO_GAIN))->AddString(str);
	}
	((CComboBox *)GetDlgItem(IDC_COMBO_GAIN))->SetCurSel(stuMityCCD.iGain);
	if (stuMityCCD.iGain!=CurNum)
	{
	pToucanCamera->SetGain(stuMityCCD.iGain);
	}*/
	((CComboBox *)GetDlgItem(IDC_COMBO_GAIN))->SetCurSel(stuToucanCCD.iGain);
}
//Mity不需要设置温度 预留
void CDlgToucanSet::InitOffset()
{
	CString str;
	str.Format(_T("%d"),stuToucanCCD.nTemp);
	GetDlgItem(IDC_EDIT_TEMP)->SetWindowText(str);
}
void CDlgToucanSet::InitFVB()
{
	((CComboBox *)GetDlgItem(IDC_COMBO_MODE))->ResetContent();
	((CComboBox *)GetDlgItem(IDC_COMBO_MODE))->AddString(_T("FVB"));
	((CComboBox *)GetDlgItem(IDC_COMBO_MODE))->AddString(_T("Multi"));
	((CComboBox *)GetDlgItem(IDC_COMBO_MODE))->SetCurSel(stuToucanCCD.nReadMode);
	ShowBin(stuToucanCCD.nReadMode);
}
void CDlgToucanSet::ShowBin(int nMode)
{
	GetDlgItem(IDC_STATIC_BIN)->ShowWindow(nMode);
	GetDlgItem(IDC_STATIC_TO)->ShowWindow(nMode);
	GetDlgItem(IDC_BUTTON_BIN)->ShowWindow(nMode);
	GetDlgItem(IDC_EDIT_BINMIN)->ShowWindow(nMode);
	GetDlgItem(IDC_COMBO_BIN)->ShowWindow(nMode);
	//GetDlgItem(IDC_EDIT_BINMAX)->ShowWindow(nMode);
	CString str;
	str.Format(_T("%d"),stuToucanCCD.BinMin);
	GetDlgItem(IDC_EDIT_BINMIN)->SetWindowText(str);
	str.Format(_T("%d"),stuToucanCCD.BinMax);
	GetDlgItem(IDC_EDIT_BINMAX)->SetWindowText(str);

	
	/*if (nMode==6)
	{
		pToucanCamera->SetBinning(0);
	}
	else
	{
		pToucanCamera->SetBinning(nMode+1);
	}*/
     
	



}
void CDlgToucanSet::OnCbnSelchangeComboMode()
{
	int nMode;
	nMode=((CComboBox *)GetDlgItem(IDC_COMBO_MODE))->GetCurSel();
	stuToucanCCD.nReadMode=nMode;
	ShowBin(stuToucanCCD.nReadMode);
}
void CDlgToucanSet::OnCbnSelchangeComboGain()
{
	stuToucanCCD.iGain=((CComboBox *)GetDlgItem(IDC_COMBO_GAIN))->GetCurSel();
	pToucanCamera->SetGain(stuToucanCCD.iGain);
}

void CDlgToucanSet::OnBnClickedButtonAdv()
{
	CDlgPassword dlg;
	if (dlg.DoModal()==IDOK)
	{
		CDlgAdvToucan dlg;
		dlg.DoModal();
	}
}
void CDlgToucanSet::OnEnChangeEditOffset()
{
	UpdateData(TRUE);
}
void CDlgToucanSet::OnBnClickedButtonImage()
{
	//  lcz new 22/10/24 如果开始采集
	g_bStartDetect = true;

	m_CameraState=TRUE;
	SetButtomState(m_CameraState);
	AfxBeginThread(CCDImageToucan,(LPVOID)this);
}
void CDlgToucanSet::OnBnClickedButtonStop()
{
	//  lcz new 22/10/24 
	g_bStartDetect = false;

	m_CameraState=FALSE;
	pToucanCamera->SetImageAccState(m_CameraState);
	//pToucanCamera->TerminateData();
	Sleep(20);
	SetButtomState(m_CameraState);
	
}


UINT CCDImageToucan(LPVOID lParam)
{
	
	int x,y;
	pToucanCamera->SetImageMode(g_pDlg->fTime);

	pToucanCamera->GetDevSize(&x,&y);
	pToucanCamera->SetImageAccState(g_pDlg->m_CameraState);

	BYTE *pData1=new BYTE[x*y*3];

	memset(pData1,10,x*y*3);
	int m_acqStatus;
	while (g_pDlg->m_CameraState==TRUE)
	{
        //pToucanCamera->SetImageMode(g_pDlg->fTime);
		g_pDlg->dlgImage.SetPicSize(x,y);
		pToucanCamera->ImageDataAcquisition(pData1,x,y);

		if (g_pDlg->m_CameraState==FALSE)
		{
			//pToucanCamera->TerminateData();
			//pToucanCamera->SetBinning(stuToucanCCD.nReadMode);
			delete []pData1;
			return 0;
		}

		for (int i=0;i<y;i++)
		{

			memcpy(g_pDlg->dlgImage.pBuffer+i*x*3,pData1+(y-i-1)*x*3,x*3);
		}

		
		//memcpy(g_pDlg->dlgImage.pBuffer,pData1,x*y*3);
		g_pDlg->dlgImage.OnDisplay();
	}
	//pToucanCamera->TerminateData();
	//pToucanCamera->SetBinning(stuToucanCCD.nReadMode);
	delete []pData1;
	return 0;

}
void CDlgToucanSet::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	// lcz new 22/10/24
	if(g_bStartDetect)
		OnBnClickedButtonStop();
	Sleep(500);

	// lcz new 22/10/24
	g_bOpenDlg = false;

	pToucanCamera->WriteAllIni();
	KillTimer(1);
	pToucanCamera->TerminateData();
	pToucanCamera->SetBinning(stuToucanCCD.nReadMode);

	CDialog::OnClose();
}

void CDlgToucanSet::OnBnClickedButtonBmp()
{
	if (m_CameraState==FALSE)
	{
		dlgImage.SavePicBmp();
	}

}
void CDlgToucanSet::SetString(int x,int y)
{
	CString str;
	str.Format(_T("X:%4d"),x);
	m_StaticX=str;
	str.Format(_T("Y:%4d"),y);
	m_StaticY=str;

}
void CDlgToucanSet::OnBnClickedCheckCursor()
{
	int state =((CButton *)GetDlgItem(IDC_CHECK_CURSOR))->GetCheck();
	dlgImage.SetMark(state);
}
BOOL CDlgToucanSet::PreTranslateMessage(MSG* pMsg)
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

	//return CDialog::PreTranslateMessage(pMsg);
}
void CDlgToucanSet::SetButtomState(BOOL bSta)
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
void CDlgToucanSet::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent==1)
	{
		GetDlgItem(IDC_STATIC_X)->SetWindowText(m_StaticX);
		GetDlgItem(IDC_STATIC_Y)->SetWindowText(m_StaticY);
	}

	CDialog::OnTimer(nIDEvent);
}
void CDlgToucanSet::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	// TODO: 在此处添加消息处理程序代码
}

void CDlgToucanSet::OnBnClickedButtonH()
{
	stuToucanCCD.HMirror=!stuToucanCCD.HMirror;
}

// CDlgMitySet 消息处理程序
NS_MFC_Res_END