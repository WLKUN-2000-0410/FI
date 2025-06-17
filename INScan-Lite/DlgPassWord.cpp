// DlgPassWord.cpp : 实现文件
//

#include "stdafx.h"
#include "INScan-LiteDlg.h"
#include "DlgPassWord.h"
#include "afxdialogex.h"


// CDlgPassWord 对话框

IMPLEMENT_DYNAMIC(CDlgPassWord, CDialog)

CDlgPassWord::CDlgPassWord(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgPassWord::IDD, pParent)
{

}

CDlgPassWord::~CDlgPassWord()
{
}

void CDlgPassWord::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_INPUT, m_BtnPss);
}


BEGIN_MESSAGE_MAP(CDlgPassWord, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_INPUT, &CDlgPassWord::OnBnClickedButtonInput)
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CDlgPassWord 消息处理程序


BOOL CDlgPassWord::OnInitDialog()
{
	CDialog::OnInitDialog();
	UpdateLanguage();
	//UpdateData();
	((CEdit *)GetDlgItem(IDC_EDIT_PAS))->SetSel(0,0,TRUE);
	((CEdit *)GetDlgItem(IDC_EDIT_PAS))->SetFocus();

	((CEdit *)GetDlgItem(IDC_EDIT_PAS))->SendMessage(WM_LBUTTONDOWN,0, MAKELONG(0,0));
	((CEdit *)GetDlgItem(IDC_EDIT_PAS))->SendMessage(WM_LBUTTONUP,0, MAKELONG(0,0));
	
	SetButtonState(&m_BtnPss);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
void CDlgPassWord::UpdateLanguage()
{
	CINScanLiteDlg *m_app=(CINScanLiteDlg*)AfxGetApp()->GetMainWnd();
	m_app->m_ZLanguage.SetCreateLanguageState(FALSE);
	m_app->m_ZLanguage.SetDialogStrings((long)(CDialog*)this,IDD);
}

void CDlgPassWord::OnBnClickedButtonInput()
{
	CINScanLiteDlg * pMainFrame = (CINScanLiteDlg *)AfxGetApp()->GetMainWnd();

	CString str;
	GetDlgItem(IDC_EDIT_PAS)->GetWindowText(str);
	if(str!=_T("jc")&&str!=_T("1"))
	{
		MessageBox(pMainFrame->LoadStringMeau(_T("ID_PASSWORD_ERROR")));
		return;
	}
	CDialog::OnOK();
}


void CDlgPassWord::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	CRect rect;
	GetClientRect(rect);
	dc.FillSolidRect(rect,clr);
	CDialog::OnPaint();
}


HBRUSH CDlgPassWord::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	if (nCtlColor==CTLCOLOR_STATIC)
	{
		pDC->SetBkColor(clr);
		HBRUSH b =CreateSolidBrush(clr);
		return b;
	}
	return hbr;
}

//重写虚函数PreTranslateMessage() 用于回车键确认
BOOL CDlgPassWord::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN)
	{
		switch(pMsg->wParam)
		{
		case VK_RETURN:
			OnBnClickedButtonInput();
			return  TRUE;
		case VK_ESCAPE:
			return TRUE;
		default:
			break;
		}
	}

	return CDialog::PreTranslateMessage(pMsg);
}


//void CDlgPassWord::OnOK()
//{
//	// TODO: 在此添加专用代码和/或调用基类
//	OnBnClickedButtonInput();
//	//CDialog::OnOK();
//}
