// DlgPassword.cpp : 实现文件
//

#include "stdafx.h"
#include "ZolixCCD.h"
#include "DlgPassword.h"
#include "afxdialogex.h"
#include "Language.h"
NS_MFC_Res_BEGIN
// CDlgPassword 对话框

IMPLEMENT_DYNAMIC(CDlgPassword, CDialog)

CDlgPassword::CDlgPassword(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgPassword::IDD, pParent)
{

}

CDlgPassword::~CDlgPassword()
{
}

void CDlgPassword::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgPassword, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_CCD, &CDlgPassword::OnBnClickedButtonCcd)
END_MESSAGE_MAP()

BOOL CDlgPassword::OnInitDialog()
{
	CDialog::OnInitDialog();

	CLanguage mLag;
	CDialog *pDlg;
	pDlg=(CDialog *)this;
	mLag.UpdateLanguage(pDlg,ID_DLG_PASSWORD,mLag.szDig[nLanguage],g_strPath);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
void CDlgPassword::OnBnClickedButtonCcd()
{
	CString str;
	GetDlgItem(IDC_EDIT_CCD)->GetWindowText(str);
	if(str==_T("zolix")||str==_T("Raman")||str==_T("PES")||str==_T("12345"))
	{
		CDialog::OnOK();
	}
	else{
		return;
	}
	
}
BOOL CDlgPassword::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message==WM_KEYDOWN&& pMsg->wParam==VK_ESCAPE)
	{
		return TRUE;
	}
	if (pMsg->message==WM_KEYDOWN&& pMsg->wParam==VK_RETURN)
	{
		return TRUE;
	}
	
        return CDialog::PreTranslateMessage(pMsg);
	
	
}

// CDlgPassword 消息处理程序
NS_MFC_Res_END