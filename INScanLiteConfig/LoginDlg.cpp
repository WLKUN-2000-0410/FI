// LoginDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "INScanLiteConfig.h"
#include "LoginDlg.h"
#include "afxdialogex.h"


// LoginDlg 对话框

IMPLEMENT_DYNAMIC(LoginDlg, CDialogEx)

LoginDlg::LoginDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_LOGIN, pParent)
{

}

LoginDlg::~LoginDlg()
{
}

void LoginDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_PWD, m_pwdEdit);
}


BEGIN_MESSAGE_MAP(LoginDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_LOGIN, &LoginDlg::OnBnClickedButtonLogin)
END_MESSAGE_MAP()


// LoginDlg 消息处理程序


void LoginDlg::OnBnClickedButtonLogin()
{
	// TODO: 在此添加控件通知处理程序代码
	
}
