// DeleteOne.cpp : 实现文件
//

#include "stdafx.h"
#include "INScan-Lite.h"
#include "SetColor.h"
#include "afxdialogex.h"
#include "INScan-LiteDlg.h"
#include "..\file\TeeChart2011\series.h"
#include "..\file\TeeChart2011\valuelist.h"
#include "..\file\TeeChart2011\lineseries.h"
#include "..\file\TeeChart2011\pen.h"
// CDeleteOne 对话框

IMPLEMENT_DYNAMIC(CSetColor, CDialog)

	CSetColor::CSetColor(CWnd* pParent /*=NULL*/)
	: CDialog(CSetColor::IDD, pParent)
{

}

CSetColor::~CSetColor()
{
}

void CSetColor::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDOK_SET, m_ctrlBtUpdate);
	DDX_Control(pDX, IDC_COMBO1, m_Combo);
}


BEGIN_MESSAGE_MAP(CSetColor, CDialog)
	ON_BN_CLICKED(IDOK_SET, &CSetColor::OnBnClickedSet)
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDOK_SET, &CSetColor::OnBnClickedSet)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CSetColor::OnCbnSelchangeCombo1)
END_MESSAGE_MAP()


// CDeleteOne 消息处理程序

HBRUSH CSetColor::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性
	if (nCtlColor==CTLCOLOR_STATIC)
	{
		pDC->SetBkColor(clr);
		HBRUSH b =CreateSolidBrush(clr);
		return b;
	}
	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}


void CSetColor::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialogEx::OnPaint()
	CRect rect;
	GetClientRect(rect);
	dc.FillSolidRect(rect,clr);
	CDialog::OnPaint();
}


BOOL CSetColor::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	SetButtonState(&m_ctrlBtUpdate);
	CINScanLiteDlg * pDlg=(CINScanLiteDlg*)AfxGetApp()->GetMainWnd();

	int nSeriesCount = pDlg->m_DlgShow.m_Tchart.GetSeriesCount();
	if (nSeriesCount==0) return TRUE;
	for (int i = 0 ;i<nSeriesCount;i++)
	{
		CString strSeriesName = pDlg->m_DlgShow.m_Tchart.Series(i).GetTitle();
		m_Combo.InsertString(i,strSeriesName);
	}
	m_Combo.SetCurSel(nSeriesCount - 1);

	UpdateData(FALSE);

	UpdateLanguage();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


BOOL CSetColor::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (pMsg->message == WM_KEYDOWN)
	{
		switch(pMsg->wParam)
		{
		case VK_RETURN:
			return  TRUE;
		case VK_ESCAPE:
			return TRUE;
		default:
			break;
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}


void CSetColor::OnBnClickedSet()
{
	UpdateData(TRUE);
	CINScanLiteDlg * pDlg=(CINScanLiteDlg*)AfxGetApp()->GetMainWnd();

	int nNum=m_Combo.GetCurSel();
	pDlg->OnSetColor(nNum);
}
void CSetColor::UpdateLanguage()
{
	CINScanLiteDlg * pDlg=(CINScanLiteDlg*)AfxGetApp()->GetMainWnd();
	pDlg->m_ZLanguage.SetCreateLanguageState(FALSE);
	pDlg->m_ZLanguage.SetDialogStrings((long)(CDialog*)this,IDD);

	//SetControlFont();
}

void CSetColor::OnCbnSelchangeCombo1()
{

}
