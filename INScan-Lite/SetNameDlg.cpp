// SetNameDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "INScan-Lite.h"
#include "SetNameDlg.h"
#include "afxdialogex.h"
#include "INScan-LiteDlg.h"
#include "..\file\TeeChart2011\series.h"
#include "..\file\TeeChart2011\valuelist.h"
#include "..\file\TeeChart2011\lineseries.h"
#include "..\file\TeeChart2011\pen.h"

// CSetNameDlg 对话框

IMPLEMENT_DYNAMIC(CSetNameDlg, CDialogEx)

CSetNameDlg::CSetNameDlg(CWnd* pParent /*=NULL*/, CString strCurName)
	: CDialogEx(CSetNameDlg::IDD, pParent)
	, strUpdateName(_T(""))
	, m_strCurName(strCurName)
{

}

CSetNameDlg::~CSetNameDlg()
{
}

void CSetNameDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_UPDATE_NAME, strUpdateName);
	DDX_Control(pDX, IDOK_SET, m_ctrlBtUpdate);
	DDX_Control(pDX, IDC_COMBO1, m_Combo);
}


BEGIN_MESSAGE_MAP(CSetNameDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CSetNameDlg::OnBnClickedOk)
	ON_WM_CTLCOLOR()
	ON_WM_PAINT()
	ON_BN_CLICKED(IDOK_SET, &CSetNameDlg::OnBnClickedSet)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CSetNameDlg::OnCbnSelchangeCombo1)
END_MESSAGE_MAP()


// CSetNameDlg 消息处理程序


void CSetNameDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	/*CINScanLiteDlg * pDlg=(CINScanLiteDlg*)AfxGetApp()->GetMainWnd();
	pDlg->m_DlgShow.updateName(strUpdateName);*/
}


HBRUSH CSetNameDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

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


void CSetNameDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialogEx::OnPaint()
	CRect rect;
	GetClientRect(rect);
	dc.FillSolidRect(rect,clr);
	CDialog::OnPaint();
}


BOOL CSetNameDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	SetButtonState(&m_ctrlBtUpdate);
	CINScanLiteDlg * pDlg=(CINScanLiteDlg*)AfxGetApp()->GetMainWnd();
	strUpdateName = pDlg->m_DlgShow.m_Tchart.Series(0).GetTitle();
	
	int nSeriesCount = pDlg->m_DlgShow.m_Tchart.GetSeriesCount();
	if (nSeriesCount==0) return TRUE;
	int iCurPos = 0;
	for (int i = 0 ;i<nSeriesCount;i++)
	{
		CString strSeriesName = pDlg->m_DlgShow.m_Tchart.Series(i).GetTitle();
		if(m_strCurName.CompareNoCase(strSeriesName) == 0)
			iCurPos = i;
		m_Combo.InsertString(i,strSeriesName);
	}
	m_Combo.SetCurSel(nSeriesCount - 1);
	CString strCurName;
	m_Combo.GetWindowTextA(strCurName);
	strUpdateName = strCurName;

	UpdateData(FALSE);

	UpdateLanguage();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


BOOL CSetNameDlg::PreTranslateMessage(MSG* pMsg)
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
	return CDialogEx::PreTranslateMessage(pMsg);
}

//   数据处理-----重命名
void CSetNameDlg::OnBnClickedSet()
{
	UpdateData(TRUE);
	CINScanLiteDlg * pDlg=(CINScanLiteDlg*)AfxGetApp()->GetMainWnd();
	int nSeriesCount = pDlg->m_DlgShow.m_Tchart.GetSeriesCount();
	for (int i = 0 ;i<nSeriesCount;i++)
	{
		CString strSeriesName = pDlg->m_DlgShow.m_Tchart.Series(i).GetTitle();
		if(strSeriesName.CompareNoCase(strUpdateName) == 0)
		{
			MessageBox(pDlg->LoadStringMeau(_T("ID_RENAME_FAIL")),pDlg->LoadStringMeau(_T("ID_WARNING")),MB_OK|MB_ICONWARNING);
			return;
		}
	}

	int nNum=m_Combo.GetCurSel();
	
	pDlg->m_DlgShow.updateName(strUpdateName,m_Combo.GetCurSel());


	m_Combo.ResetContent();
	for (int i = 0 ;i<nSeriesCount;i++)
	{
		CString strSeriesName = pDlg->m_DlgShow.m_Tchart.Series(i).GetTitle();
		m_Combo.InsertString(i,strSeriesName);
	}
	m_Combo.SetCurSel(nNum);
}
void CSetNameDlg::UpdateLanguage()
{
	CINScanLiteDlg * pDlg=(CINScanLiteDlg*)AfxGetApp()->GetMainWnd();
	pDlg->m_ZLanguage.SetCreateLanguageState(FALSE);
	pDlg->m_ZLanguage.SetDialogStrings((long)(CDialog*)this,IDD);

	//SetControlFont();
}

void CSetNameDlg::OnCbnSelchangeCombo1()
{
	m_Combo.GetLBText(m_Combo.GetCurSel(),strUpdateName);
	//GetDlgItem(IDC_COMBO1)->GetWindowText(strUpdateName);
	UpdateData(FALSE);
}
