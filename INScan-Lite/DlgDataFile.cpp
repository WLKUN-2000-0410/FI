// DlgDataFile.cpp : 实现文件
//

#include "stdafx.h"
#include "INScan-Lite.h"
#include "DlgDataFile.h"
#include "afxdialogex.h"
#include "INScan-LiteDlg.h"

#include "..\file\TeeChart2011\series.h"
#include "..\file\TeeChart2011\valuelist.h"

// CDlgDataFile 对话框

IMPLEMENT_DYNAMIC(CDlgDataFile, CDialog)

CDlgDataFile::CDlgDataFile(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgDataFile::IDD, pParent)
{

}

CDlgDataFile::~CDlgDataFile()
{
}

void CDlgDataFile::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_CUT_OUT_LINE, m_BtnSetName);
	DDX_Control(pDX, IDC_BUTTON_FIT, m_BtnColor);
	DDX_Control(pDX, IDC_BUTTON_PEAK, m_BtnOne);
	DDX_Control(pDX, IDC_BUTTON_SMOOTH, m_BtnAll);
	DDX_Control(pDX, IDC_BUTTON_MATH, m_BtnAuto);
	DDX_Control(pDX, IDC_BTN_DelUnsel, m_Btn_DelUnsel);
}


BEGIN_MESSAGE_MAP(CDlgDataFile, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_CUT_OUT_LINE, &CDlgDataFile::OnBnClickedButtonCutOutLine)
	ON_BN_CLICKED(IDC_BUTTON_FIT, &CDlgDataFile::OnBnClickedButtonFit)
	ON_BN_CLICKED(IDC_BUTTON_PEAK, &CDlgDataFile::OnBnClickedButtonPeak)
	ON_BN_CLICKED(IDC_BUTTON_SMOOTH, &CDlgDataFile::OnBnClickedButtonSmooth)
	ON_BN_CLICKED(IDC_BUTTON_MATH, &CDlgDataFile::OnBnClickedButtonMath)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BTN_DelUnsel, &CDlgDataFile::OnBnClickedBtnDelunsel)
END_MESSAGE_MAP()


// CDlgDataFile 消息处理程序

#include "SetNameDlg.h"
#include "SetColor.h"
#include "DeleteOne.h"
#include "SetColor.h"
void CDlgDataFile::OnBnClickedButtonCutOutLine()
{
	CINScanLiteDlg * pDlg=(CINScanLiteDlg*)AfxGetApp()->GetMainWnd();

	if (pDlg->m_DlgShow.GetTeeCount()>0)
	{
		CSetNameDlg dlg;
		dlg.DoModal();
	}
	
}


void CDlgDataFile::OnBnClickedButtonFit()
{
	CINScanLiteDlg * pDlg=(CINScanLiteDlg*)AfxGetApp()->GetMainWnd();

	if (pDlg->m_DlgShow.GetTeeCount()>0)
	{
		CSetColor dlg;
		dlg.DoModal();
	}
}



void CDlgDataFile::OnBnClickedButtonSmooth()
{
	CINScanLiteDlg * pDlg=(CINScanLiteDlg*)AfxGetApp()->GetMainWnd();

	if (MessageBox(pDlg->LoadStringMeau(_T("ID_DELETE_ALL")), _T(""), MB_YESNO | MB_ICONQUESTION)==IDYES)
	{	
		pDlg->OnDelAll();
	}
	

}

//  自动缩放
void CDlgDataFile::OnBnClickedButtonMath()
{
	CINScanLiteDlg * pDlg=(CINScanLiteDlg*)AfxGetApp()->GetMainWnd();
	pDlg->OnAutoZoom();
}


void CDlgDataFile::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	CRect rect;
	GetClientRect(rect);
	dc.FillSolidRect(rect,clr);
}


BOOL CDlgDataFile::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_FontChinese.CreatePointFont(nFontSize,strFontChinese);
	m_FontEnglish.CreatePointFont(nFontSize,strFontEngish);

	SetButtonState(&m_BtnAll);
	SetButtonState(&m_BtnOne);
	SetButtonState(&m_BtnAuto);
	SetButtonState(&m_BtnSetName);
	SetButtonState(&m_BtnColor);
	SetButtonState(&m_Btn_DelUnsel);
	SetControlFont();
	UpdateLanguage();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
void CDlgDataFile::UpdateLanguage()
{
	CINScanLiteDlg * pDlg=(CINScanLiteDlg*)AfxGetApp()->GetMainWnd();
	pDlg->m_ZLanguage.SetCreateLanguageState(FALSE);
	pDlg->m_ZLanguage.SetDialogStrings((long)(CDialog*)this,IDD);
	SetControlFont();
}

void CDlgDataFile::SetControlFont()
{
	CFont *m_Font;

	if (sInfo.nLanguage==1)
		m_Font=&m_FontEnglish;
	else
		m_Font=&m_FontChinese;

	GetDlgItem(IDC_BUTTON_CUT_OUT_LINE)->SetFont(m_Font);
	GetDlgItem(IDC_BUTTON_FIT)->SetFont(m_Font);
	GetDlgItem(IDC_BUTTON_PEAK)->SetFont(m_Font);
	GetDlgItem(IDC_BUTTON_SMOOTH)->SetFont(m_Font);
	GetDlgItem(IDC_BUTTON_MATH)->SetFont(m_Font);
	GetDlgItem(IDC_BTN_DelUnsel)->SetFont(m_Font);
}

//  lcz nwe : 由删除当前曲线变为删除已选曲线
void CDlgDataFile::OnBnClickedButtonPeak()
{
	CINScanLiteDlg * pDlg=(CINScanLiteDlg*)AfxGetApp()->GetMainWnd();

	if (pDlg->m_DlgShow.GetTeeCount()>0)
	{
		//CDeleteOne dlg;
		//dlg.DoModal();
		for (int i = 0; i < pDlg->m_DlgShow.GetTeeCount(); i++)
		{
			if(pDlg->m_DlgShow.m_Tchart.Series(i).GetActive())
				break;
			//  如果没有曲线被勾选
			if(i == pDlg->m_DlgShow.GetTeeCount() - 1)
			{
				MessageBox(pDlg->LoadStringMeau(_T("ID_DELETE_NOSELECT")));
				return;
			}
		}

		// 看是否是全部曲线都被勾选，如果都被勾选，则留最后一条不删除，但不勾选
		int numOfChecked = 0;
		bool allChecked = false;
		for(int i = 0; i < pDlg->m_DlgShow.GetTeeCount(); i++)
		{
			if(pDlg->m_DlgShow.m_Tchart.Series(i).GetActive())
				numOfChecked++;
		}
		if(numOfChecked == pDlg->m_DlgShow.GetTeeCount() )
			allChecked = true;


		if (MessageBox(pDlg->LoadStringMeau(_T("ID_DELETE_SELECT")), _T(""), MB_YESNO | MB_ICONQUESTION)==IDYES)
		{	
			// 删除已勾选曲线
			for(int i = 0; i < pDlg->m_DlgShow.GetTeeCount(); )
			{
				int c = pDlg->m_DlgShow.GetTeeCount();
				if(pDlg->m_DlgShow.m_Tchart.Series(i).GetActive() || pDlg->m_DlgShow.m_Tchart.Series(i).GetShowInLegend() == false)
				{
					//// 如果所有都被勾选，删除到还剩一条曲线的时候不删除，显示图例为"Legend List"，图线数据为空
					/*if(allChecked && c == 1)
					{
						pDlg->m_DlgShow.m_Tchart.Series(i).SetActive(false);
						pDlg->m_DlgShow.m_Tchart.Series(i).Clear();
						pDlg->m_DlgShow.m_Tchart.Series(i).SetLegendTitle(TEXT("Legend List"));
						return;
					}
					else*/
						pDlg->m_DlgShow.m_Tchart.RemoveSeries(i);
				}
				else
					i++;
			}
			// 如果删除后还有曲线，那就默认都显示
			//if(pDlg->m_DlgShow.GetTeeCount() > 0)
			//{
			//	for(int i = 0; i < pDlg->m_DlgShow.GetTeeCount(); i++)
			//	{
			//		CValueList list_x = pDlg->m_DlgShow.m_Tchart.Series(i).GetXValues();
			//		CValueList list_y = pDlg->m_DlgShow.m_Tchart.Series(i).GetYValues();
			//		int color = pDlg->m_DlgShow.m_Tchart.Series(i).GetColor();
			//		pDlg->m_DlgShow.m_Tchart.Series(i).SetActive(TRUE);
			//		int nXSize = list_x.GetCount();
			//		for(int j = 0; j < nXSize;  j++)
			//		{
			//			pDlg->m_DlgShow.m_Tchart.Series(i).AddXY(list_x.GetValue(j), list_y.GetValue(j), _T(""),color);
			//		}
			//	}
			//}
		}	
	}

}

// lcz new ： 删除未勾选曲线
void CDlgDataFile::OnBnClickedBtnDelunsel()
{
	// TODO: 在此添加控件通知处理程序代码
	CINScanLiteDlg * pDlg=(CINScanLiteDlg*)AfxGetApp()->GetMainWnd();

	if (pDlg->m_DlgShow.GetTeeCount()>0)
	{
		//CDeleteOne dlg;
		//dlg.DoModal();
		for (int i = 0; i < pDlg->m_DlgShow.GetTeeCount(); i++)
		{
			//  如果有任意条未勾选，则停止
			if( !pDlg->m_DlgShow.m_Tchart.Series(i).GetActive())
				break;
			//  如果所有曲线都被勾选
			if(i == pDlg->m_DlgShow.GetTeeCount() - 1)
			{
				MessageBox(pDlg->LoadStringMeau(_T("ID_DELETE_ALLSELECT")));
				return;
			}
		}

		// 看是否是全部曲线都没被勾选，如果都没被勾选，则留最后一条不删除
		int numOfNotChecked = 0;
		bool allNotChecked = false;
		for(int i = 0; i < pDlg->m_DlgShow.GetTeeCount(); i++)
		{
			if( !pDlg->m_DlgShow.m_Tchart.Series(i).GetActive())
				numOfNotChecked++;
		}
		if(numOfNotChecked == pDlg->m_DlgShow.GetTeeCount() )
			allNotChecked = true;

		if (MessageBox(pDlg->LoadStringMeau(_T("ID_DELETE_UNSELECT")), _T(""), MB_YESNO | MB_ICONQUESTION)==IDYES)
		{	
			for(int i = 0; i < pDlg->m_DlgShow.GetTeeCount(); )
			{
				int c = pDlg->m_DlgShow.GetTeeCount();
				// 如果曲线未被勾选，则删除
				if(!pDlg->m_DlgShow.m_Tchart.Series(i).GetActive() || pDlg->m_DlgShow.m_Tchart.Series(i).GetShowInLegend() == false)
					//pDlg->m_DlgShow.m_Tchart.RemoveSeries(i);
				{
					// 如果所有都没被勾选，并且只剩最后一条曲线，显示图例为"Legend List"，图线数据为空
					//if(allNotChecked && c == 1)
					//{
					//	pDlg->m_DlgShow.m_Tchart.Series(i).SetActive(false);
					//	pDlg->m_DlgShow.m_Tchart.Series(i).Clear();
					//	pDlg->m_DlgShow.m_Tchart.Series(i).SetLegendTitle(TEXT("Legend List"));
					//	return;
					//}
					//else
						pDlg->m_DlgShow.m_Tchart.RemoveSeries(i);
				}
				else
					i++;
			}
			// 如果删除后还有曲线，那就默认都显示
			//if(pDlg->m_DlgShow.GetTeeCount() > 0)
			//{
			//	for(int i = 0; i < pDlg->m_DlgShow.GetTeeCount(); i++)
			//	{
			//		CValueList list_x = pDlg->m_DlgShow.m_Tchart.Series(i).GetXValues();
			//		CValueList list_y = pDlg->m_DlgShow.m_Tchart.Series(i).GetYValues();
			//		int color = pDlg->m_DlgShow.m_Tchart.Series(i).GetColor();
			//		pDlg->m_DlgShow.m_Tchart.Series(i).SetActive(TRUE);
			//		int nXSize = list_x.GetCount();
			//		for(int j = 0; j < nXSize;  j++)
			//		{
			//			pDlg->m_DlgShow.m_Tchart.Series(i).AddXY(list_x.GetValue(j), list_y.GetValue(j), _T(""),color);
			//		}
			//	}
			//}
		}	
	}
}
