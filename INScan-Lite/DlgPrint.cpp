// DlgPrint.cpp : 实现文件
//

#include "stdafx.h"
#include "INScan-Lite.h"
#include "INScan-LiteDlg.h"
#include "DlgPrint.h"
#include "afxdialogex.h"
#include "PrintFrame.h"
#include "PrintView.h"
#include "..\file\TeeChart2011\TeeChartDefines.h"
#include "..\file\TeeChart2011\Titles.h"
#include "..\file\TeeChart2011\Gradient.h"
#include "..\file\TeeChart2011\printer.h"
// CDlgPrint 对话框

IMPLEMENT_DYNAMIC(CDlgPrint, CDialog)

CDlgPrint::CDlgPrint(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgPrint::IDD, pParent)
	, m_EditP(0)
{

}

CDlgPrint::~CDlgPrint()
{
}

void CDlgPrint::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_PRINT, m_BtnPrint);
	DDX_Control(pDX, IDC_BUTTON_PRINTVIEW, m_BtnPrintV);
	DDX_Text(pDX, IDC_EDIT_POW, m_EditP);
	DDX_Text(pDX, IDC_EDIT_TIME, m_EditT);
	DDX_Text(pDX, IDC_EDIT_A, m_EditA);
}


BEGIN_MESSAGE_MAP(CDlgPrint, CDialog)
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BUTTON_PRINT, &CDlgPrint::OnBnClickedButtonPrint)
	ON_BN_CLICKED(IDC_BUTTON_PRINTVIEW, &CDlgPrint::OnBnClickedButtonPrintview)
	ON_MESSAGE(WM_BEGIN_PRINTING,OnBeginPrinting)
	ON_MESSAGE(WM_END_PRINTING,OnEndPrinting)
	ON_MESSAGE(WM_MY_PRINT,OnMyPrint)
	ON_MESSAGE(WM_MY_PRINT_S,OnMyPrintS)
END_MESSAGE_MAP()


// CDlgPrint 消息处理程序


void CDlgPrint::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	CRect rect;
	GetClientRect(rect);
	dc.FillSolidRect(rect,clr);
	CDialog::OnPaint();
}


HBRUSH CDlgPrint::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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


BOOL CDlgPrint::PreTranslateMessage(MSG* pMsg)
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

void CDlgPrint::ReadData()
{
	GetDlgItem(IDC_EDIT_MAN)->GetWindowText(sPInfo.Human);
	GetDlgItem(IDC_EDIT_NAME)->GetWindowText(sPInfo.ProName);
	GetDlgItem(IDC_DATETIMEPICKER1)->GetWindowText(sPInfo.strSetTime);
	CString str;
	GetDlgItem(IDC_EDIT_TIME)->GetWindowText(str);
	sPInfo.dTime=atof(str);
	GetDlgItem(IDC_EDIT_A)->GetWindowText(str);
	sPInfo.AccNum=atoi(str);
	GetDlgItem(IDC_EDIT_POW)->GetWindowText(str);
	sPInfo.Pow=atoi(str);
}
void CDlgPrint::UpdateLanguage()
{
	CINScanLiteDlg *m_app=(CINScanLiteDlg*)AfxGetApp()->GetMainWnd();
	m_app->m_ZLanguage.SetCreateLanguageState(FALSE);
	m_app->m_ZLanguage.SetDialogStrings((long)(CDialog*)this,IDD);
}
BOOL CDlgPrint::OnInitDialog()
{
	CDialog::OnInitDialog();
	UpdateLanguage();
	SetButtonState(&m_BtnPrint);
	SetButtonState(&m_BtnPrintV);
	GetDlgItem(IDC_EDIT_MAN)->SetWindowText(sPInfo.Human);
	GetDlgItem(IDC_EDIT_NAME)->SetWindowText(sPInfo.ProName);
	GetDlgItem(IDC_DATETIMEPICKER1)->GetWindowText(sPInfo.strSysTime);
	if (sTInfo.KLMode==0)
	{
		m_EditP=sTInfo.KPower[sTInfo.KPowerMode];
		int Data=sTInfo.KShowTime*1000;
		m_EditT=(double)Data/1000;
		m_EditA=1;
	}
	else if (sTInfo.KLMode==1)
	{
		m_EditP=sTInfo.JPower;
		m_EditT=sTInfo.JTime;
		m_EditA=sTInfo.JAccNum;
	}
	else
	{
		m_EditP=0;
		m_EditT=0;
		m_EditA=0;
	}

	bPrint=FALSE;
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDlgPrint::OnBnClickedButtonPrintview()
{
	GetDlgItem(IDC_BUTTON_PRINTVIEW)->EnableWindow(FALSE);
	ReadData();
	if (bPrint==FALSE)
	{
		bPrint=TRUE;
		CPrintFrame *pFrame = new CPrintFrame;
		pFrame->m_pCallerDlg = this;
		pFrame->Create(NULL,"",WS_OVERLAPPEDWINDOW,CRect(500,200,1000,500));
		pFrame->ShowWindow(SW_SHOW);
		pFrame->m_pView->OnMyPrintPreview();
	}
	GetDlgItem(IDC_BUTTON_PRINTVIEW)->EnableWindow(TRUE);

}

void CDlgPrint::OnBnClickedButtonPrint()
{
	ReadData();
	if (bPrint==FALSE)
	{
		bPrint=TRUE;
		CPrintFrame *pFrame = new CPrintFrame;
		pFrame->m_pCallerDlg = this;
		pFrame->Create(NULL," ",WS_OVERLAPPEDWINDOW,CRect(0,0,0,0));
		pFrame->ShowWindow(SW_SHOW);
		pFrame->m_pView->OnMyPrint();
	}
	
}

LRESULT CDlgPrint::OnBeginPrinting(WPARAM wParam,LPARAM lParam)
{
	CDC* pDC= (CDC*)wParam;
	CPrintInfo* pInfo	= (CPrintInfo *)lParam;
	if(m_fontPrinter.m_hObject==NULL)
		m_fontPrinter.CreatePointFont(120,"FixedSys",pDC);
	TEXTMETRIC tm;
	CFont* pOldFont = pDC->SelectObject (&m_fontPrinter);
	pDC->GetTextMetrics (&tm);
	m_cyPrinter = tm.tmHeight + tm.tmExternalLeading;
	CSize size = pDC->GetTextExtent (_T ("---------------------------" \
		"----------------------------------------------------------"), 81);
	pDC->SelectObject (pOldFont);
	m_cxWidth = size.cx;

	m_nLinesPerPage = (pDC->GetDeviceCaps (VERTRES) -
		(m_cyPrinter * (3 + (2 * PRINTMARGIN)))) / m_cyPrinter;
	pInfo->SetMaxPage (1);//最大页为2页

	m_cxOffset = (pDC->GetDeviceCaps (HORZRES) - size.cx) / 2;


	return TRUE;
}
void CDlgPrint::PrintPageHeader(CDC* pDC, UINT nPageNumber, CPrintInfo* pInfo)//打印页眉
{
	CINScanLiteDlg * pDlg=(CINScanLiteDlg*)GetParent();

	CFont *pOldFont;
	CFont fontHeader;
	fontHeader.CreatePointFont(100,"FixedSys",pDC);
	pOldFont=(CFont *)(pDC->SelectObject(&fontHeader));
	int nPosY,nPosX;
	nPosY=m_cyPrinter*3;
	nPosX=m_cxOffset;
	pDC->TextOut(nPosX,nPosY,CString(pDlg->LoadStringMeau(_T("ID_LOGO"))));
	CString str;
	str=pDlg->LoadStringMeau(_T("ID_DATE"))+sPInfo.strSysTime;
	CSize size=pDC->GetTextExtent(str);
	nPosX=m_cxOffset+m_cxWidth-size.cx;
	pDC->TextOut(nPosX,nPosY,str);
	pDC->MoveTo(m_cxOffset,4*m_cyPrinter);
	pDC->LineTo(m_cxOffset+m_cxWidth,4*m_cyPrinter);
	pDC->SelectObject(pOldFont);
	fontHeader.DeleteObject();
}
LRESULT CDlgPrint::OnEndPrinting(WPARAM wParam,LPARAM lParam)
{

	if(m_fontPrinter.m_hObject!=NULL)
	{	
		m_fontPrinter.DeleteObject ();

	}
	return TRUE;
}
LRESULT CDlgPrint::OnMyPrintS(WPARAM wParam,LPARAM lParam)
{
	bPrint=FALSE;
	return TRUE;
}
LRESULT CDlgPrint::OnMyPrint(WPARAM wParam,LPARAM lParam)
{
	CINScanLiteDlg * pDlg=(CINScanLiteDlg*)GetParent();
	int K=180;//间隔
	CString strText;
	CFont *pOldFont;
	CFont fontHeader1;
	CFont fontHeader2;
	CFont fontHeader3;
	CDC* pDC			= (CDC*)wParam;
	CPrintInfo* pInfo	= (CPrintInfo *)lParam;//存储打印和打印预览信息
    fontHeader1.CreatePointFont(300,"FixedSys",pDC);
    fontHeader2.CreatePointFont(180,"FixedSys",pDC);
	fontHeader3.CreatePointFont(150,"FixedSys",pDC);
	pOldFont=(CFont *)(pDC->SelectObject(&fontHeader1));

	strText=pDlg->LoadStringMeau(_T("ID_TITLE"));

	int iWhere = pDC->DrawText(strText, strText.GetLength(), CRect(pInfo->m_rectDraw.Width()/2-600, 550, 
		pInfo->m_rectDraw.Width() - 100, pInfo->m_rectDraw.Height()),
		DT_WORDBREAK);
	///////////////////////////第一个部分//////////////////////////////////
	pOldFont=(CFont *)(pDC->SelectObject(&fontHeader2));
	strText=pDlg->LoadStringMeau(_T("ID_TEST_INFO"));
	
	pDC->DrawText(strText, strText.GetLength(), CRect(m_cxOffset, 850, 
		pInfo->m_rectDraw.Width() - 100, pInfo->m_rectDraw.Height()),
		DT_WORDBREAK);
	pOldFont=(CFont *)(pDC->SelectObject(&fontHeader3));
	strText=pDlg->LoadStringMeau(_T("ID_TEST_MAN"))+sPInfo.Human ; 

	
	pDC->DrawText(strText, strText.GetLength(), CRect(m_cxOffset+300, 1050, 
		pInfo->m_rectDraw.Width() - 100, pInfo->m_rectDraw.Height()),
		DT_WORDBREAK);
	strText=pDlg->LoadStringMeau(_T("ID_TEST_TIME"))+sPInfo.strSetTime; 

	iWhere=1050+K;
	pDC->DrawText(strText, strText.GetLength(), CRect(m_cxOffset+300, iWhere, 
		pInfo->m_rectDraw.Width() - 100, pInfo->m_rectDraw.Height()),
		DT_WORDBREAK);
	strText=pDlg->LoadStringMeau(_T("ID_TEST_NAME"))+sPInfo.ProName; 
	iWhere+=K;
	pDC->DrawText(strText, strText.GetLength(), CRect(m_cxOffset+300, iWhere, 
	pInfo->m_rectDraw.Width() - 100, pInfo->m_rectDraw.Height()),
		DT_WORDBREAK);
	//////////////////////////////////////////////////////////////////////////
	pOldFont=(CFont *)(pDC->SelectObject(&fontHeader2));
	strText=pDlg->LoadStringMeau(_T("ID_TEST_CONDITION"));
	pDC->DrawText(strText, strText.GetLength(), CRect(m_cxOffset, 1600, 
		pInfo->m_rectDraw.Width() - 100, pInfo->m_rectDraw.Height()),
		DT_WORDBREAK);

	CString str;
	str.Format(_T("%dW"),sPInfo.Pow);
	pOldFont=(CFont *)(pDC->SelectObject(&fontHeader3));
	strText=pDlg->LoadStringMeau(_T("ID_POWER"))+str;
	pDC->DrawText(strText, strText.GetLength(), CRect(m_cxOffset+300, 1800, 
		pInfo->m_rectDraw.Width() - 100, pInfo->m_rectDraw.Height()),
		DT_WORDBREAK);
	iWhere=1800+K;
	str.Format(_T("%.3fS"),sPInfo.dTime);
	strText=pDlg->LoadStringMeau(_T("ID_TIME"))+str;
	pDC->DrawText(strText, strText.GetLength(), CRect(m_cxOffset+300, iWhere, 
		pInfo->m_rectDraw.Width() - 100, pInfo->m_rectDraw.Height()),
		DT_WORDBREAK);
    iWhere+=K;
	str.Format(_T("%d"),sPInfo.AccNum);
	strText=pDlg->LoadStringMeau(_T("ID_ACC"))+str;
	pDC->DrawText(strText, strText.GetLength(), CRect(m_cxOffset+300, iWhere, 
		pInfo->m_rectDraw.Width() - 100, pInfo->m_rectDraw.Height()),
		DT_WORDBREAK);
	iWhere+=K;

	strText=pDlg->LoadStringMeau(_T("ID_EQ_TYPE"))+sPInfo.Model;
	pDC->DrawText(strText, strText.GetLength(), CRect(m_cxOffset+300, iWhere, 
		pInfo->m_rectDraw.Width() - 100, pInfo->m_rectDraw.Height()),
		DT_WORDBREAK);
	iWhere+=K;
	strText=pDlg->LoadStringMeau(_T("ID_EQ_NUM"))+sPInfo.Number;
	pDC->DrawText(strText, strText.GetLength(), CRect(m_cxOffset+300, iWhere, 
		pInfo->m_rectDraw.Width() - 100, pInfo->m_rectDraw.Height()),
		DT_WORDBREAK);
	//////////////////////////////////////////////////////////////////////////////
	pOldFont=(CFont *)(pDC->SelectObject(&fontHeader2));
	strText=pDlg->LoadStringMeau(_T("ID_RESULT"));
	pDC->DrawText(strText, strText.GetLength(), CRect(m_cxOffset, 2700, 
		pInfo->m_rectDraw.Width() - 100, pInfo->m_rectDraw.Height()),
		DT_WORDBREAK);



	/////////////////////////////////画图/////////////////////////////////////////
	if (!pInfo->m_bPreview)
	{	
		long w;
		if (pDlg->m_DlgShow.m_Tchart.GetPrinter().GetOrientation() == poLandscape)
		{
			w = pDlg->m_DlgShow.m_Tchart.GetPrinter().GetPageHeight();
		}
		else
		{
			w = pDlg->m_DlgShow.m_Tchart.GetPrinter().GetPageWidth();
		}
		pDlg->m_DlgShow.m_Tchart.GetPrinter().PrintPartialHandle(COleVariant(LONG(pDC->m_hDC)), m_cxOffset, 2910, 
			m_cxOffset+m_cxWidth, 
			pInfo->m_rectDraw.Height()*3/4);
	}
	else
	{
		int iSavedDC = pDC->SaveDC();	// save the DC 3150
	    pDlg->m_DlgShow.m_Tchart.Draw(LONG(pDC->m_hDC), m_cxOffset, 2910, 
			 m_cxOffset+m_cxWidth, pInfo->m_rectDraw.Height()*3/4);
	    pDC->RestoreDC(iSavedDC);
	}

	//////////////////////////////////////////////////////////////////////////////////////////////////
	strText=pDlg->LoadStringMeau(_T("ID_SQL"));

	pDC->DrawText(strText, strText.GetLength(), CRect(m_cxOffset, 5280, 
		pInfo->m_rectDraw.Width() - 100, pInfo->m_rectDraw.Height()),
		DT_WORDBREAK);
	

	pOldFont=(CFont *)(pDC->SelectObject(&fontHeader3));

	iWhere=5480;
	for (int i=0;i<sSInfo.DataPerNum;i++)
	{
		if (sInfo.nLanguage==0)
		{
			strText.Format(_T("%d. %s___%s   %.1f%%"),i+1,sDInfo[i].name_ch,sDInfo[i].code,sDInfo[i].fSimilarNum*100);

		}
		else
		{
			strText.Format(_T("%d. %s___%s   %.1f%%"),i+1,sDInfo[i].name_eh,sDInfo[i].code,sDInfo[i].fSimilarNum*100);

		}
			pDC->DrawText(strText, strText.GetLength(), CRect(m_cxOffset+300, iWhere, 
			pInfo->m_rectDraw.Width(), pInfo->m_rectDraw.Height()),
			DT_WORDBREAK);
		iWhere=iWhere+K;
	}
	



	strText=pDlg->LoadStringMeau(_T("ID_TEST_HUMAN"));
	pDC->DrawText(strText, strText.GetLength(), CRect(m_cxOffset, 6460, 
		pInfo->m_rectDraw.Width()/2, pInfo->m_rectDraw.Height()),
		DT_WORDBREAK);
	strText=pDlg->LoadStringMeau(_T("ID_TEST_LEADER"));
	pDC->DrawText(strText, strText.GetLength(), CRect(pInfo->m_rectDraw.Width()/2+300, 6460, 
		pInfo->m_rectDraw.Width(), pInfo->m_rectDraw.Height()),
		DT_WORDBREAK);
	strText=pDlg->LoadStringMeau(_T("ID_TEST_DATE"));
	pDC->DrawText(strText, strText.GetLength(), CRect(m_cxOffset, 6580, 
		pInfo->m_rectDraw.Width()/2, pInfo->m_rectDraw.Height()),
		DT_WORDBREAK);
	///////////////////////////////////////////////////////////////////////////////////////////////////
	PrintPageHeader(pDC,pInfo->m_nCurPage,  pInfo);
	fontHeader1.DeleteObject();
	fontHeader2.DeleteObject();
	fontHeader3.DeleteObject();
	return TRUE;
}



void CDlgPrint::OnCancel()
{
	CINScanLiteDlg * pDlg=(CINScanLiteDlg*)GetParent();
	if (bPrint==FALSE)
	{
		pDlg->m_DlgShow.SetSize();
		CDialog::OnCancel();
	}

}
