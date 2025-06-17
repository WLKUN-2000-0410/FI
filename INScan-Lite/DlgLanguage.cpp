// DlgLanguage.cpp : 实现文件
//

#include "stdafx.h"
#include "INScan-Lite.h"
#include "DlgLanguage.h"
#include "afxdialogex.h"
#include "INScan-LiteDlg.h"

// CDlgLanguage 对话框

IMPLEMENT_DYNAMIC(CDlgLanguage, CDialog)

CDlgLanguage::CDlgLanguage(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgLanguage::IDD, pParent)
{

}

CDlgLanguage::~CDlgLanguage()
{
}

void CDlgLanguage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgLanguage, CDialog)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BUTTON_TW, &CDlgLanguage::OnBnClickedButtonTw)
	ON_BN_CLICKED(IDC_BUTTON_CH, &CDlgLanguage::OnBnClickedButtonCh)
	ON_BN_CLICKED(IDC_BUTTON_EN, &CDlgLanguage::OnBnClickedButtonEn)
END_MESSAGE_MAP()


// CDlgLanguage 消息处理程序


BOOL CDlgLanguage::OnInitDialog()
{
	CDialog::OnInitDialog();

	HBITMAP hBitmap;
	hBitmap=LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_BITMAP4));
	((CButton *)GetDlgItem(IDC_BUTTON_CH))->SetBitmap(hBitmap);
	
	hBitmap=LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_BITMAP5));
	((CButton *)GetDlgItem(IDC_BUTTON_EN))->SetBitmap(hBitmap);


	hBitmap=LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_BITMAP6));
	((CButton *)GetDlgItem(IDC_BUTTON_TW))->SetBitmap(hBitmap);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CDlgLanguage::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	CRect rect;
	GetClientRect(rect);
	dc.FillSolidRect(rect,clr);
	CDialog::OnPaint();
}


void CDlgLanguage::OnBnClickedButtonTw()
{
	sInfo.nLanguage=2;
	CINScanLiteDlg *pDlg =(CINScanLiteDlg *)GetParent();
	pDlg->InitLan();
}


void CDlgLanguage::OnBnClickedButtonCh()
{

	sInfo.nLanguage=0;
	CINScanLiteDlg *pDlg =(CINScanLiteDlg *)GetParent();
	pDlg->InitLan();
}


void CDlgLanguage::OnBnClickedButtonEn()
{

	sInfo.nLanguage=1;
	CINScanLiteDlg *pDlg =(CINScanLiteDlg *)GetParent();
	pDlg->InitLan();
}
BOOL CDlgLanguage::PreTranslateMessage(MSG* pMsg)
{
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
