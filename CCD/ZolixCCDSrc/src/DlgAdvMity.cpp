// DlgAdvMity.cpp : 实现文件
//

#include "stdafx.h"
#include "ZolixCCD.h"
#include "DlgAdvMity.h"
#include "afxdialogex.h"
#include "IniFile.h"
#include "DlgAdvAndor.h"
#include "Language.h"
#include "CameraDll.h"

// CDlgAdvMity 对话框
NS_MFC_Res_BEGIN
IMPLEMENT_DYNAMIC(CDlgAdvMity, CDialog)

CDlgAdvMity::CDlgAdvMity(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgAdvMity::IDD, pParent)
{

}

CDlgAdvMity::~CDlgAdvMity()
{
}

void CDlgAdvMity::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgAdvMity, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_CCD, &CDlgAdvMity::OnBnClickedButtonCcd)
	ON_CBN_SELCHANGE(IDC_COMBO_MIRROR, &CDlgAdvMity::OnCbnSelchangeComboMirror)
END_MESSAGE_MAP()

BOOL CDlgAdvMity::OnInitDialog()
{
	CDialog::OnInitDialog();
	CString str;
	CLanguage mLag;
	CDialog *pDlg;
	pDlg=(CDialog *)this;
	mLag.UpdateLanguage(pDlg,ID_DLG_ADV,mLag.szDig[nLanguage],g_strPath);
	((CComboBox *)GetDlgItem(IDC_COMBO_MIRROR))->ResetContent();
	str=mLag.ReadID(_T("ID_MIRROR_Z"),mLag.szDig[nLanguage]);
	((CComboBox *)GetDlgItem(IDC_COMBO_MIRROR))->AddString(str);
	str=mLag.ReadID(_T("ID_MIRROR_F"),mLag.szDig[nLanguage]);
	((CComboBox *)GetDlgItem(IDC_COMBO_MIRROR))->AddString(str);
	((CComboBox *)GetDlgItem(IDC_COMBO_MIRROR))->SetCurSel(stuMityCCD.isMirror);
	str.Format("%d",stuMityCCD.iCbpNum);
	GetDlgItem(IDC_EDIT_CCDNUM)->SetWindowText(str);
	GetDlgItem(IDC_EDIT_CCD)->SetWindowText(stuMityCCD.strCbp);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}
#include "ManageZolixIni.h"
void CDlgAdvMity::OnBnClickedButtonCcd()
{
	CString str;
	GetDlgItem(IDC_EDIT_CCD)->GetWindowText(str);
	stuMityCCD.strCbp=str;
	GetDlgItem(IDC_EDIT_CCDNUM)->GetWindowText(str);
	stuMityCCD.iCbpNum=atoi(str);
	ManageZolixIni->WriteIni();
}
void CDlgAdvMity::OnCbnSelchangeComboMirror()
{
	stuMityCCD.isMirror=((CComboBox *)GetDlgItem(IDC_COMBO_MIRROR))->GetCurSel();
}

BOOL CDlgAdvMity::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg->wParam==VK_RETURN || pMsg->wParam==VK_ESCAPE ) 
		return TRUE;
	else    
		return CDialog::PreTranslateMessage(pMsg);
}

// CDlgAdvMity 消息处理程序
NS_MFC_Res_END