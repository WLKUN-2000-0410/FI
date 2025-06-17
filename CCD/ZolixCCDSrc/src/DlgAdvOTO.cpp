// DlgAdvOTO.cpp : 实现文件
//

#include "stdafx.h"
#include "ZolixCCD.h"
#include "DlgAdvOTO.h"
#include "Language.h"
#include "CameraDll.h"

// CDlgAdvOTO 对话框
NS_MFC_Res_BEGIN
	IMPLEMENT_DYNAMIC(CDlgAdvOTO, CDialog)

	CDlgAdvOTO::CDlgAdvOTO(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgAdvOTO::IDD, pParent)
{

}

CDlgAdvOTO::~CDlgAdvOTO()
{
}

void CDlgAdvOTO::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgAdvOTO, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_CCD, &CDlgAdvOTO::OnBnClickedButtonCcd)
	ON_CBN_SELCHANGE(IDC_COMBO_MIRROR, &CDlgAdvOTO::OnCbnSelchangeComboMirror)
END_MESSAGE_MAP()

BOOL CDlgAdvOTO::OnInitDialog()
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
	((CComboBox *)GetDlgItem(IDC_COMBO_MIRROR))->SetCurSel(stuOTOCCD.isMirror);
	str.Format("%d",stuOTOCCD.iCbpNum);
	GetDlgItem(IDC_EDIT_CCDNUM)->SetWindowText(str);
	GetDlgItem(IDC_EDIT_CCD)->SetWindowText(stuOTOCCD.strCbp);
	
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}
#include "ManageZolixIni.h"
void CDlgAdvOTO::OnBnClickedButtonCcd()
{
	CString str;
	GetDlgItem(IDC_EDIT_CCD)->GetWindowText(str);
	stuOTOCCD.strCbp=str;
	GetDlgItem(IDC_EDIT_CCDNUM)->GetWindowText(str);
	stuOTOCCD.iCbpNum=atoi(str);
	ManageZolixIni->WriteIni();
}
void CDlgAdvOTO::OnCbnSelchangeComboMirror()
{
	stuOTOCCD.isMirror=((CComboBox *)GetDlgItem(IDC_COMBO_MIRROR))->GetCurSel();
}
BOOL CDlgAdvOTO::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg->wParam==VK_RETURN || pMsg->wParam==VK_ESCAPE ) 
		return TRUE;
	else    
		return CDialog::PreTranslateMessage(pMsg);
}

// CDlgAdvMity 消息处理程序
NS_MFC_Res_END