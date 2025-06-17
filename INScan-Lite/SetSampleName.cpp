// SetSampleName.cpp : 实现文件
//

#include "stdafx.h"
#include "INScan-Lite.h"
#include "SetSampleName.h"
#include "afxdialogex.h"

// SetSampleName 对话框

IMPLEMENT_DYNAMIC(SetSampleName, CDialogEx)

SetSampleName::SetSampleName(CWnd* pParent /*=NULL*/)
	: CDialogEx(SetSampleName::IDD, pParent)
	, m_strName(_T(""))
{

}

SetSampleName::~SetSampleName()
{
}

void SetSampleName::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_SAMPLE_NAME, m_strName);
}


BEGIN_MESSAGE_MAP(SetSampleName, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &SetSampleName::OnBnClickedButton1)
END_MESSAGE_MAP()


// SetSampleName 消息处理程序


void SetSampleName::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	CString cc = m_strName;
	OnOK();
}
