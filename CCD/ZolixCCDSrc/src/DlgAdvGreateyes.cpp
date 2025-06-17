// DlgAdvAndor.cpp : 实现文件
//

#include "stdafx.h"
#include "ZolixCCD.h"
#include "DlgAdvGreateyes.h"
#include "afxdialogex.h"
#include "IniFile.h"
#include "DlgAdvAndor.h"
#include "Language.h"
#include "CameraDll.h"
NS_MFC_Res_BEGIN
// CDlgAdvAndor 对话框

IMPLEMENT_DYNAMIC(CDlgAdvGreateyes, CDialog)

CDlgAdvGreateyes::CDlgAdvGreateyes(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgAdvGreateyes::IDD, pParent)
{

}

CDlgAdvGreateyes::~CDlgAdvGreateyes()
{
}

void CDlgAdvGreateyes::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgAdvGreateyes, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_CCD, &CDlgAdvGreateyes::OnBnClickedButtonCcd)
	ON_CBN_SELCHANGE(IDC_COMBO_MIRROR, &CDlgAdvGreateyes::OnCbnSelchangeComboMirror)
END_MESSAGE_MAP()

BOOL CDlgAdvGreateyes::OnInitDialog()
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
	((CComboBox *)GetDlgItem(IDC_COMBO_MIRROR))->SetCurSel(stuAndorCCD.isMirror);
	str.Format("%d",stuAndorCCD.iCbpNum);
	GetDlgItem(IDC_EDIT_CCDNUM)->SetWindowText(str);
	GetDlgItem(IDC_EDIT_CCD)->SetWindowText(stuAndorCCD.strCbp);

	ManageZolixIni->ReadIni();

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}
void CDlgAdvGreateyes::OnBnClickedButtonCcd()
{
	CString str;
	GetDlgItem(IDC_EDIT_CCD)->GetWindowText(str);
	stuAndorCCD.strCbp=str;
	GetDlgItem(IDC_EDIT_CCDNUM)->GetWindowText(str);
	stuAndorCCD.iCbpNum=atoi(str);
	ManageZolixIni->WriteIni();
}
void CDlgAdvGreateyes::OnCbnSelchangeComboMirror()
{
	stuAndorCCD.isMirror=((CComboBox *)GetDlgItem(IDC_COMBO_MIRROR))->GetCurSel();
}
BOOL CDlgAdvGreateyes::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg->wParam==VK_RETURN || pMsg->wParam==VK_ESCAPE ) 
		return TRUE;
	else    
		return CDialog::PreTranslateMessage(pMsg);
}
// CDlgAdvAndor 消息处理程序
NS_MFC_Res_END