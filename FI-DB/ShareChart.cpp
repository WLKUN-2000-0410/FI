// ShareChart.cpp : 实现文件
//

#include "stdafx.h"
#include "FI-DB.h"
#include "ShareChart.h"
#include "afxdialogex.h"


// ShareChart 对话框

IMPLEMENT_DYNAMIC(ShareChart, CDialogEx)

ShareChart::ShareChart(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_CHART, pParent)
{

}

ShareChart::~ShareChart()
{
}

void ShareChart::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TCHART_SHARE, m_tchart);
}

BEGIN_MESSAGE_MAP(ShareChart, CDialogEx)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// ShareChart 消息处理程序
void ShareChart::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);
	CRect rect;
	GetWindowRect(&rect);
	::MoveWindow(m_tchart.m_hWnd, 0, 0, rect.Width(), rect.Height(), TRUE);
}