// DlgAdvMity.cpp : 实现文件
//

#include "stdafx.h"
#include "ZolixCCD.h"
#include "DlgAdvToucan.h"
#include "afxdialogex.h"
#include "IniFile.h"
#include "DlgAdvAndor.h"
#include "Language.h"
#include "CameraDll.h"

// CDlgAdvMity 对话框
NS_MFC_Res_BEGIN
IMPLEMENT_DYNAMIC(CDlgAdvToucan, CDialog)

CDlgAdvToucan::CDlgAdvToucan(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgAdvToucan::IDD, pParent)
{

}

CDlgAdvToucan::~CDlgAdvToucan()
{
}

void CDlgAdvToucan::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgAdvToucan, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_CCD, &CDlgAdvToucan::OnBnClickedButtonCcd)
	ON_CBN_SELCHANGE(IDC_COMBO_MIRROR, &CDlgAdvToucan::OnCbnSelchangeComboMirror)
	ON_WM_CLOSE()
END_MESSAGE_MAP()
void CDlgAdvToucan::OnClose()
{
	CString str;
	GetDlgItem(IDC_EDIT1)->GetWindowText(str);
	stuToucanCCD.offset=atoi(str);
	GetDlgItem(IDC_EDIT2)->GetWindowText(str);
	stuToucanCCD.bkoffset=atoi(str);
	ManageZolixIni->WriteIni();
	CDialog::OnClose();
}
BOOL CDlgAdvToucan::OnInitDialog()
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
	((CComboBox *)GetDlgItem(IDC_COMBO_MIRROR))->SetCurSel(stuToucanCCD.isMirror);
	str.Format("%d",stuToucanCCD.iCbpNum);
	GetDlgItem(IDC_EDIT_CCDNUM)->SetWindowText(str);
	GetDlgItem(IDC_EDIT_CCD)->SetWindowText(stuToucanCCD.strCbp);

	GetDlgItem(IDC_EDIT1)->ShowWindow(TRUE);
	GetDlgItem(IDC_EDIT2)->ShowWindow(TRUE);
	GetDlgItem(IDC_STATIC_OF)->ShowWindow(TRUE);
	GetDlgItem(IDC_STATIC_BKOF)->ShowWindow(TRUE);
	str.Format("%d",stuToucanCCD.offset);
	GetDlgItem(IDC_EDIT1)->SetWindowText(str);
	str.Format("%d",stuToucanCCD.bkoffset);
	GetDlgItem(IDC_EDIT2)->SetWindowText(str);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}
#include "ManageZolixIni.h"
void CDlgAdvToucan::OnBnClickedButtonCcd()
{
	CString str;
	GetDlgItem(IDC_EDIT_CCD)->GetWindowText(str);
	stuToucanCCD.strCbp=str;
	GetDlgItem(IDC_EDIT_CCDNUM)->GetWindowText(str);
	stuToucanCCD.iCbpNum=atoi(str);
	ManageZolixIni->WriteIni();
}
void CDlgAdvToucan::OnCbnSelchangeComboMirror()
{
	stuToucanCCD.isMirror=((CComboBox *)GetDlgItem(IDC_COMBO_MIRROR))->GetCurSel();
}

BOOL CDlgAdvToucan::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg->wParam==VK_RETURN || pMsg->wParam==VK_ESCAPE ) 
		return TRUE;
	else    
		return CDialog::PreTranslateMessage(pMsg);
}

// CDlgAdvMity 消息处理程序
NS_MFC_Res_END