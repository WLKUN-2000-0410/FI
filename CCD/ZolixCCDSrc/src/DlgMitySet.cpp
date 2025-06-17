// DlgAndorSet.cpp : 实现文件
//

#include "stdafx.h"
#include "ZolixCCD.h"
#include "DlgMitySet.h"
#include "afxdialogex.h"
#include "MityCamera.h"
#include "Language.h"
#include "DlgPassword.h"
#include "DlgAdvMity.h"

NS_MFC_Res_BEGIN
// CDlgMitySet 对话框
CDlgMitySet *g_pDlg=NULL;
UINT CCDImageMity(LPVOID lParam);

IMPLEMENT_DYNAMIC(CDlgMitySet, CDialog)

CDlgMitySet::CDlgMitySet(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgMitySet::IDD, pParent)
	, m_StaticX(_T(""))
	, m_StaticY(_T(""))
{
	m_CameraState=FALSE;
	fTime=0.1;
}

CDlgMitySet::~CDlgMitySet()
{
}

void CDlgMitySet::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_BINMIN, iBinMin);	//中心
	DDX_Text(pDX, IDC_EDIT_BINMAX, iBinMax);
	DDX_Text(pDX, IDC_EDIT_TIME, fTime);
	DDX_Text(pDX, IDC_STATIC_X, m_StaticX);
	DDX_Text(pDX, IDC_STATIC_Y, m_StaticY);
}


BEGIN_MESSAGE_MAP(CDlgMitySet, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_TEMP, &CDlgMitySet::OnBnClickedButtonTemp)
	ON_BN_CLICKED(IDC_BUTTON_BIN, &CDlgMitySet::OnBnClickedButtonBin)
	ON_CBN_SELCHANGE(IDC_COMBO_MODE, &CDlgMitySet::OnCbnSelchangeComboMode)
	ON_BN_CLICKED(IDC_BUTTON_ADV, &CDlgMitySet::OnBnClickedButtonAdv)
	ON_BN_CLICKED(IDC_BUTTON_IMAGE, &CDlgMitySet::OnBnClickedButtonImage)
	ON_EN_CHANGE(IDC_EDIT_TIME, &CDlgMitySet::OnEnChangeEditOffset)
	ON_BN_CLICKED(IDC_BUTTON_STOP, &CDlgMitySet::OnBnClickedButtonStop)
	ON_WM_CLOSE()
	ON_CBN_SELCHANGE(IDC_COMBO_GAIN, &CDlgMitySet::OnCbnSelchangeComboGain)
	ON_BN_CLICKED(IDC_CHECK_CURSOR, &CDlgMitySet::OnBnClickedCheckCursor)
	ON_BN_CLICKED(IDC_BUTTON_BMP, &CDlgMitySet::OnBnClickedButtonBmp)
	ON_WM_CTLCOLOR()
	ON_WM_TIMER()
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_BUTTON_H, &CDlgMitySet::OnBnClickedButtonH)
END_MESSAGE_MAP()

BOOL CDlgMitySet::OnInitDialog()
{
	CDialog::OnInitDialog();
	pMityCamera->ReadAllIni();
	InitComboGain();
	InitOffset();
	InitFVB();
	GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_H)->EnableWindow(FALSE);
	CLanguage mLag;
	CDialog *pDlg;
	pDlg=(CDialog *)this;
	mLag.UpdateLanguage(pDlg,ID_DLG_M,mLag.szDig[nLanguage],g_strPath);

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

	
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}
void CDlgMitySet::OnBnClickedButtonTemp()
{
	CString str;
	GetDlgItem(IDC_EDIT_TEMP)->GetWindowText(str);
	if (atoi(str)<1)
	{
		str=_T("1");
		GetDlgItem(IDC_EDIT_TEMP)->SetWindowText(str);

	}
	pMityCamera->SetOffset(atoi(str));
	stuMityCCD.iOffset=atoi(str);
}
void CDlgMitySet::OnBnClickedButtonBin()
{
	UpdateData(TRUE);
	if (iBinMax<0||iBinMax>=stuMityCCD.nGYPix||iBinMin<0||iBinMin>stuMityCCD.nGYPix)
	{
		return;
	}
	if (iBinMax<iBinMin)
	{
		int temp=iBinMin;
		iBinMin=iBinMax;
		iBinMax=temp;
	}
	stuMityCCD.BinMax=iBinMax;
	stuMityCCD.BinMin=iBinMin;
	UpdateData(FALSE);
}
void CDlgMitySet::InitComboGain()
{
	CString str;
	float fGain[8];
	int Num=0,CurNum=0;
	((CComboBox *)GetDlgItem(IDC_COMBO_GAIN))->ResetContent();
	pMityCamera->GetGain(&Num,fGain,&CurNum);

	g_iCurrentGain = CurNum; // lcz new 22/10/24 记录当前的增益值

	for (int i=0;i<Num;i++)
	{
    	str.Format(_T("%.1fX"),fGain[i]);
		((CComboBox *)GetDlgItem(IDC_COMBO_GAIN))->AddString(str);
	}
	((CComboBox *)GetDlgItem(IDC_COMBO_GAIN))->SetCurSel(stuMityCCD.iGain);
	if (stuMityCCD.iGain!=CurNum)
	{
		pMityCamera->SetGain(stuMityCCD.iGain);
	}
}
//Mity不需要设置温度 预留
void CDlgMitySet::InitOffset()
{
	CString str;
	str.Format(_T("%d"),stuMityCCD.iOffset);
	GetDlgItem(IDC_EDIT_TEMP)->SetWindowText(str);
}
void CDlgMitySet::InitFVB()
{
	((CComboBox *)GetDlgItem(IDC_COMBO_MODE))->ResetContent();
	((CComboBox *)GetDlgItem(IDC_COMBO_MODE))->AddString(_T("FVB"));
	((CComboBox *)GetDlgItem(IDC_COMBO_MODE))->AddString(_T("Multi"));
	((CComboBox *)GetDlgItem(IDC_COMBO_MODE))->SetCurSel(stuMityCCD.nReadMode);
	ShowBin(stuMityCCD.nReadMode);
}
void CDlgMitySet::ShowBin(int nMode)
{
	GetDlgItem(IDC_STATIC_BIN)->ShowWindow(nMode);
	GetDlgItem(IDC_STATIC_TO)->ShowWindow(nMode);
	GetDlgItem(IDC_BUTTON_BIN)->ShowWindow(nMode);
	GetDlgItem(IDC_EDIT_BINMIN)->ShowWindow(nMode);
	GetDlgItem(IDC_EDIT_BINMAX)->ShowWindow(nMode);
	CString str;
	str.Format(_T("%d"),stuMityCCD.BinMin);
	GetDlgItem(IDC_EDIT_BINMIN)->SetWindowText(str);
	str.Format(_T("%d"),stuMityCCD.BinMax);
	GetDlgItem(IDC_EDIT_BINMAX)->SetWindowText(str);
}
void CDlgMitySet::OnCbnSelchangeComboMode()
{
	int nMode;
	nMode=((CComboBox *)GetDlgItem(IDC_COMBO_MODE))->GetCurSel();
	stuMityCCD.nReadMode=nMode;
	ShowBin(stuMityCCD.nReadMode);
}
void CDlgMitySet::OnCbnSelchangeComboGain()
{
	stuMityCCD.iGain=((CComboBox *)GetDlgItem(IDC_COMBO_GAIN))->GetCurSel();

	stuMityCCD.iGain=((CComboBox *)GetDlgItem(IDC_COMBO_GAIN))->GetCurSel();// lcz new 22/10/24

	pMityCamera->SetGain(stuMityCCD.iGain);
}

void CDlgMitySet::OnBnClickedButtonAdv()
{
	CDlgPassword dlg;
	if (dlg.DoModal()==IDOK)
	{
		CDlgAdvMity dlg;
		dlg.DoModal();
	}
}
void CDlgMitySet::OnEnChangeEditOffset()
{
	UpdateData(TRUE);
}
void CDlgMitySet::OnBnClickedButtonImage()
{
	// lcz new 22/10/24
	g_bStartDetect = true;// 开始影像为真

	m_CameraState=TRUE;
	SetButtomState(m_CameraState);
	AfxBeginThread(CCDImageMity,(LPVOID)this);
}
void CDlgMitySet::OnBnClickedButtonStop()
{
	// lcz new 22/10/24
	g_bStartDetect = false;

	m_CameraState=FALSE;
	pMityCamera->SetImageAccState(m_CameraState);
	SetButtomState(m_CameraState);
}


UINT CCDImageMity(LPVOID lParam)
{
	
	int x,y;
	pMityCamera->GetDevSize(&x,&y);
	pMityCamera->SetImageAccState(g_pDlg->m_CameraState);
	unsigned short *lpData1=new unsigned short[x*y];
	BYTE *pData1=new BYTE[x*y*3];
	memset(lpData1,10,x*y*sizeof(unsigned short));
	memset(pData1,10,x*y*3);
	int m_acqStatus;
	while (g_pDlg->m_CameraState==TRUE)
	{
        pMityCamera->SetImageMode(g_pDlg->fTime);
		g_pDlg->dlgImage.SetPicSize(x,y);
		pMityCamera->ImageDataAcquisition(lpData1,pData1,x,y);	
		memcpy(g_pDlg->dlgImage.pBuffer,pData1,x*y*3);
		g_pDlg->dlgImage.OnDisplay();
	}
	
	delete []lpData1;
	delete []pData1;
	return 0;

}
void CDlgMitySet::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	// lcz new 22/10/24
	if(g_bStartDetect)
		OnBnClickedButtonStop();
	g_bOpenDlg = false;
	//OnBnClickedButtonStop();
	//Sleep(500);
	pMityCamera->WriteAllIni();

	// lcz new 22/10/24
	Sleep(1000);

	KillTimer(1);
	CDialog::OnClose();
}

void CDlgMitySet::OnBnClickedButtonBmp()
{
	if (m_CameraState==FALSE)
	{
		dlgImage.SavePicBmp();
	}

}
void CDlgMitySet::SetString(int x,int y)
{
	CString str;
	str.Format(_T("X:%4d"),x);
	m_StaticX=str;
	str.Format(_T("Y:%4d"),y);
	m_StaticY=str;

}
void CDlgMitySet::OnBnClickedCheckCursor()
{
	int state =((CButton *)GetDlgItem(IDC_CHECK_CURSOR))->GetCheck();
	dlgImage.SetMark(state);
}
BOOL CDlgMitySet::PreTranslateMessage(MSG* pMsg)
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
void CDlgMitySet::SetButtomState(BOOL bSta)
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
void CDlgMitySet::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent==1)
	{
		GetDlgItem(IDC_STATIC_X)->SetWindowText(m_StaticX);
		GetDlgItem(IDC_STATIC_Y)->SetWindowText(m_StaticY);
	}

	CDialog::OnTimer(nIDEvent);
}
void CDlgMitySet::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	// TODO: 在此处添加消息处理程序代码
}

void CDlgMitySet::OnBnClickedButtonH()
{
	stuMityCCD.HMirror=!stuMityCCD.HMirror;
}

// CDlgMitySet 消息处理程序
NS_MFC_Res_END