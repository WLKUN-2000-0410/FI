// TestDlgShow.cpp : 实现文件
//

#include "stdafx.h"
#include "INScan-Lite.h"
#include "TestDlgShow.h"
#include "afxdialogex.h"


// CTestDlgShow 对话框

IMPLEMENT_DYNAMIC(CTestDlgShow, CDialogEx)

CTestDlgShow::CTestDlgShow(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTestDlgShow::IDD, pParent)
{

}

CTestDlgShow::~CTestDlgShow()
{
}

void CTestDlgShow::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CTestDlgShow, CDialogEx)
END_MESSAGE_MAP()


// CTestDlgShow 消息处理程序
