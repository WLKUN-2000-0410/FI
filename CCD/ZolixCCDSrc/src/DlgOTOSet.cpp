// DlgAndorSet.cpp : 实现文件
//

#include "stdafx.h"
#include "ZolixCCD.h"
#include "DlgOTOSet.h"
#include "afxdialogex.h"
#include "OTOCamera.h"
#include "Language.h"
#include "DlgPassword.h"
#include "DlgAdvOTO.h"
NS_MFC_Res_BEGIN
// CDlgOTOSet 对话框


IMPLEMENT_DYNAMIC(CDlgOTOSet, CDialog)

CDlgOTOSet::CDlgOTOSet(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgOTOSet::IDD, pParent)
{

}

CDlgOTOSet::~CDlgOTOSet()
{
}

void CDlgOTOSet::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO_ZOLIX, m_RadioX);
	DDX_Radio(pDX, IDC_RADIO_RAW, m_RadioMode);
}


BEGIN_MESSAGE_MAP(CDlgOTOSet, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_ADV, &CDlgOTOSet::OnBnClickedButtonAdv)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_CHECK_BK, &CDlgOTOSet::OnBnClickedCheckBk)
	ON_BN_CLICKED(IDC_CHECK_LINE, &CDlgOTOSet::OnBnClickedCheckLine)
	ON_BN_CLICKED(IDC_CHECK_INTENSITY, &CDlgOTOSet::OnBnClickedCheckIntensity)
	ON_BN_CLICKED(IDC_RADIO_ZOLIX, &CDlgOTOSet::OnBnClickedRadioZolix)
	ON_BN_CLICKED(IDC_RADIO_ORIGINAL, &CDlgOTOSet::OnBnClickedRadioOriginal)
	ON_BN_CLICKED(IDC_CHECK_ABINTENSITY, &CDlgOTOSet::OnBnClickedCheckAbintensity)
	ON_BN_CLICKED(IDC_RADIO_RAW, &CDlgOTOSet::OnBnClickedRadioRaw)
	ON_BN_CLICKED(IDC_RADIO_CORR, &CDlgOTOSet::OnBnClickedRadioCorr)
	ON_BN_CLICKED(IDC_CHECK_COOL, &CDlgOTOSet::OnBnClickedCheckCool)
END_MESSAGE_MAP()

BOOL CDlgOTOSet::OnInitDialog()
{
	CDialog::OnInitDialog();
	pOTOCamera->ReadAllIni();
	
	CLanguage mLag;
	CDialog *pDlg;
	pDlg=(CDialog *)this;
	mLag.UpdateLanguage(pDlg,ID_DLG_T,mLag.szDig[nLanguage],g_strPath);

	((CButton *)GetDlgItem(IDC_CHECK_BK))->SetCheck(stuOTOCCD.iCheckBK);
	((CButton *)GetDlgItem(IDC_CHECK_LINE))->SetCheck(stuOTOCCD.iCheckLine);
	((CButton *)GetDlgItem(IDC_CHECK_INTENSITY))->SetCheck(stuOTOCCD.iCheckInitensity);
	((CButton *)GetDlgItem(IDC_CHECK_ABINTENSITY))->SetCheck(stuOTOCCD.iCheckAbInitensity);
	((CButton *)GetDlgItem(IDC_CHECK_COOL))->SetCheck(stuOTOCCD.iCheckCool);

	m_RadioX=stuOTOCCD.iRadioX;
	m_RadioMode=stuOTOCCD.iRadioMode;
	UpdateData(FALSE);
	ShowMode(stuOTOCCD.iRadioMode);	

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}
void CDlgOTOSet::OnBnClickedButtonAdv()
{
	CDlgPassword dlg;
	if (dlg.DoModal()==IDOK)
	{
		CDlgAdvOTO dlg;
		dlg.DoModal();
	}
}
void CDlgOTOSet::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值	
	pOTOCamera->WriteAllIni();
	CDialog::OnClose();
}

void CDlgOTOSet::OnBnClickedCheckBk()
{
	stuOTOCCD.iCheckBK =((CButton *)GetDlgItem(IDC_CHECK_BK))->GetCheck();
}


void CDlgOTOSet::OnBnClickedCheckLine()
{
	stuOTOCCD.iCheckLine  =((CButton *)GetDlgItem(IDC_CHECK_LINE))->GetCheck();
}


void CDlgOTOSet::OnBnClickedCheckIntensity()
{
	stuOTOCCD.iCheckInitensity =((CButton *)GetDlgItem(IDC_CHECK_INTENSITY))->GetCheck();
}

void CDlgOTOSet::OnBnClickedCheckAbintensity()
{
	stuOTOCCD.iCheckAbInitensity =((CButton *)GetDlgItem(IDC_CHECK_ABINTENSITY))->GetCheck();

}

void CDlgOTOSet::OnBnClickedRadioZolix()
{
	UpdateData(TRUE);
	stuOTOCCD.iRadioX=m_RadioX;
}


void CDlgOTOSet::OnBnClickedRadioOriginal()
{
	UpdateData(TRUE);
	stuOTOCCD.iRadioX=m_RadioX;	
}

void CDlgOTOSet::OnBnClickedRadioRaw()
{
	UpdateData(TRUE);
	stuOTOCCD.iRadioMode=m_RadioMode;
	ShowMode(stuOTOCCD.iRadioMode);
}


void CDlgOTOSet::OnBnClickedRadioCorr()
{
	UpdateData(TRUE);
	stuOTOCCD.iRadioMode=m_RadioMode;
	ShowMode(stuOTOCCD.iRadioMode);
}

void CDlgOTOSet::OnBnClickedCheckCool()
{
	stuOTOCCD.iCheckCool =((CButton *)GetDlgItem(IDC_CHECK_COOL))->GetCheck();
}

void CDlgOTOSet::ShowMode(int iMode)
{
	CLanguage mLag;
	CString str;
	str=mLag.ReadID(_T("ID_RADIO_T"),mLag.szDig[nLanguage]);
	MessageBox(str,_T(""));


	BOOL bRet;
	bRet=(iMode==0)?FALSE:TRUE;

	GetDlgItem(IDC_CHECK_BK)->EnableWindow(bRet);
	GetDlgItem(IDC_CHECK_LINE)->EnableWindow(bRet);
	GetDlgItem(IDC_CHECK_INTENSITY)->EnableWindow(bRet);
	GetDlgItem(IDC_CHECK_ABINTENSITY)->EnableWindow(bRet);
	GetDlgItem(IDC_STATIC_X)->EnableWindow(bRet);
	GetDlgItem(IDC_STATIC_CORR)->EnableWindow(bRet);
	GetDlgItem(IDC_RADIO_ZOLIX)->EnableWindow(bRet);
	GetDlgItem(IDC_RADIO_ORIGINAL)->EnableWindow(bRet);
	
	
}
BOOL CDlgOTOSet::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg->wParam==VK_RETURN || pMsg->wParam==VK_ESCAPE ) 
		return TRUE;
	else    
		return CDialog::PreTranslateMessage(pMsg);
}

// CDlgOTOSet 消息处理程序
NS_MFC_Res_END