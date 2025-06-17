// SoomthDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "SoomthDlg.h"
#include "afxwin.h"
#include "afxdialogex.h"
#include "INScan-Lite.h"
#include "INScan-LiteDlg.h"
//#include "lines\BaselineCorrect.h"
//#include "lines\LineMath.h"
//  lcz  new
#include "lines\zolix_algorithm.h"
#include "..\file\TeeChart2011\series.h"
#include "..\file\TeeChart2011\valuelist.h"
#include "..\file\TeeChart2011\lineseries.h"
#include "..\file\TeeChart2011\pen.h"


// CSoomthDlg 对话框

IMPLEMENT_DYNAMIC(CSoomthDlg, CDialogEx)

	CSoomthDlg::CSoomthDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSoomthDlg::IDD, pParent)
{
	nSmoothIndex = 0;
}

CSoomthDlg::~CSoomthDlg()
{
}

void CSoomthDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_SOOMATH_LINE_CHOOSE, m_ctrlSoomthLine);
	DDX_Control(pDX, IDC_COMBO_SMOOTH_NUM, m_ctrlSoomthNum);
	DDX_Control(pDX, IDC_BUTTON_SMOOTH_CLEAN, m_ctrlBtnSmoothClean);
	DDX_Control(pDX, IDC_BUTTON_SMOOTH_DEFINE, m_ctrlBtnSmoothDefine);
}


BEGIN_MESSAGE_MAP(CSoomthDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_SMOOTH_CLEAN, &CSoomthDlg::OnBnClickedButtonSmoothClean)
	ON_BN_CLICKED(IDC_BUTTON_SMOOTH_DEFINE, &CSoomthDlg::OnBnClickedButtonSmoothDefine)
	ON_WM_CTLCOLOR()
	ON_WM_PAINT()
	//ON_WM_CLOSE()
	ON_CBN_SELCHANGE(IDC_COMBO_SOOMATH_LINE_CHOOSE, &CSoomthDlg::OnCbnSelchangeComboSoomathLineChoose)
END_MESSAGE_MAP()


// CSoomthDlg 消息处理程序


void CSoomthDlg::OnCbnSelchangeComboSmoothNum()
{
	// TODO: 在此添加控件通知处理程序代码



}

//  平滑---预览
void CSoomthDlg::OnBnClickedButtonSmoothClean()
{
	// TODO: 在此添加控件通知处理程序代码
	int nSmoothNum = m_ctrlSoomthNum.GetCurSel()+1; 
	//m_ctrlSliderSmoothNum.GetPos();
	int nSerieSize = m_ctrlSoomthLine.GetCount();
	if (nSerieSize==0) return ;
	int nSelected = m_ctrlSoomthLine.GetCurSel();
	if (nSelected<0) return ;
	CINScanLiteDlg * pDlg=(CINScanLiteDlg*)AfxGetApp()->GetMainWnd();
	// 	if (pDlg->m_DlgShow.m_Tchart.GetSeriesCount()>15)
	// 	{
	// 		MessageBox(_T("线的条数超过十五条"));
	// 		return ;
	// 	}
	CSeries series = pDlg->m_DlgShow.m_Tchart.Series(nSelected);
	CString strSeriesName = series.GetTitle();
	CValueList x_List = series.GetXValues();
	CValueList y_List = series.GetYValues();

	int nXSize = x_List.GetCount();
	double* aa = new double[nXSize];
	double* smoothedSignal = new double[nXSize];
	for (int i= 0; i<nXSize;i++)
	{
		aa[i] = y_List.GetValue(i);
	}
	//int nResult = 0;
	//main_denoise2(aa, nXSize, nSmoothNum, correctsignal, &nResult);
	dll_SG_Smooth(aa, nXSize, nSmoothNum, smoothedSignal);

	if (nSmoothIndex<=0)
	{
		nSmoothIndex = pDlg->m_DlgShow.m_Tchart.AddSeries(0);

	} 
	CString strNewSeriesName;
	strNewSeriesName.Format("%s_%s_%d",strSeriesName,"SOOMTH",nSmoothNum);

	pDlg->m_DlgShow.m_Tchart.Series(nSmoothIndex).SetTitle(strNewSeriesName);
	int color = pDlg->m_DlgShow.m_Tchart.Series(nSmoothIndex).GetColor();
	//pDlg->m_DlgShow.m_Tchart.Series(nSmoothIndex).GetAsLine().GetLinePen().SetWidth(1);
	pDlg->m_DlgShow.m_Tchart.Series(nSmoothIndex).Clear();
	pDlg->m_DlgShow.m_Tchart.Series(nSmoothIndex).SetActive(TRUE);
	for (int j = 0; j<nXSize;j++)
	{

		//pDlg->m_DlgShow.m_Tchart.Series(nSmoothIndex).AddXY(correctsignal[j],correctsignal[nResult/2+j],_T(""),color);
		//  lcz  new  原先数据处理得到的信号，前半数据位横坐标值，但有误，导致绘图错误。
		pDlg->m_DlgShow.m_Tchart.Series(nSmoothIndex).AddXY(x_List.GetValue(j), smoothedSignal[j],_T(""),color);

	}

	pDlg->m_DlgShow.m_Tchart.Series(nSmoothIndex).SetShowInLegend(FALSE);

	delete[] aa;
	delete[] smoothedSignal;

}

void CSoomthDlg::UpdateLanguage()
{
	CINScanLiteDlg * pDlg=(CINScanLiteDlg*)AfxGetApp()->GetMainWnd();
	pDlg->m_ZLanguage.SetCreateLanguageState(FALSE);
	pDlg->m_ZLanguage.SetDialogStrings((long)(CDialog*)this,IDD);

	//SetControlFont();
}

//  平滑---确定
void CSoomthDlg::OnBnClickedButtonSmoothDefine()
{
	// TODO: 在此添加控件通知处理程序代码
	CINScanLiteDlg * pDlg=(CINScanLiteDlg*)AfxGetApp()->GetMainWnd();
	
	//  lcz new 07/18判断现有的条数是否为maxLineNum,，如果已经存在maxLineNum条曲线，提示先删除部分曲线
	int numOfLineShowed = 0;
	for(int i = 0; i < pDlg->m_DlgShow.m_Tchart.GetSeriesCount(); i++)
	{
		if(pDlg->m_DlgShow.m_Tchart.Series(i).GetShowInLegend() == true)
			numOfLineShowed++;
	}
	if(numOfLineShowed >= maxLineNum)
	{
		MessageBox(pDlg->LoadStringMeau(_T("ID_OUTOF_LINE_RANGE")));
		return;
	}// lcz new

	//  如果没有预览，则先预览
	if (nSmoothIndex==0)
	{
		int nSmoothNum = m_ctrlSoomthNum.GetCurSel()+1; 
		//m_ctrlSliderSmoothNum.GetPos();
		int nSerieSize = m_ctrlSoomthLine.GetCount();
		if (nSerieSize==0) return ;
		int nSelected = m_ctrlSoomthLine.GetCurSel();
		if (nSelected<0) return ;

		CSeries series = pDlg->m_DlgShow.m_Tchart.Series(nSelected);
		CString strSeriesName = series.GetTitle();
		CValueList x_List = series.GetXValues();
		CValueList y_List = series.GetYValues();

		int nXSize = x_List.GetCount();
		double* aa = new double[nXSize];
		double* smoothedSignal = new double[nXSize];
		for (int i= 0; i<nXSize;i++)
		{
			aa[i] = y_List.GetValue(i);
		}
		//int nResult = 0;
		//main_denoise2(aa, nXSize, nSmoothNum, correctsignal, &nResult);
		dll_SG_Smooth(aa, nXSize, nSmoothNum, smoothedSignal);
		if (nSmoothIndex==0)
		{
			nSmoothIndex = pDlg->m_DlgShow.m_Tchart.AddSeries(0);
		} 

		CString strNewSeriesName;
		strNewSeriesName.Format("%s_%s_%d",strSeriesName,"SOOMTH",nSmoothNum);
		pDlg->m_DlgShow.m_Tchart.Series(nSmoothIndex).SetTitle(strNewSeriesName);
		int color = pDlg->m_DlgShow.m_Tchart.Series(nSmoothIndex).GetColor();
		//pDlg->m_DlgShow.m_Tchart.Series(nSmoothIndex).GetAsLine().GetLinePen().SetWidth(1);
		pDlg->m_DlgShow.m_Tchart.Series(nSmoothIndex).Clear();
		pDlg->m_DlgShow.m_Tchart.Series(nSmoothIndex).SetActive(TRUE);
		for (int j = 0; j<nXSize; j++)
		{
			pDlg->m_DlgShow.m_Tchart.Series(nSmoothIndex).AddXY(x_List.GetValue(j), smoothedSignal[j], _T(""),color);
		}

		delete[] aa;
		delete[] smoothedSignal;
	}


	pDlg->m_DlgShow.m_Tchart.Series(nSmoothIndex).SetShowInLegend(TRUE);
	CString strTitleName = pDlg->m_DlgShow.m_Tchart.Series(nSmoothIndex).GetTitle();
	m_ctrlSoomthLine.InsertString(m_ctrlSoomthLine.GetCount(),strTitleName);
	if (pDlg->pFitSmoothDlg->GetSafeHwnd()!=NULL)
	{
		pDlg->pFitSmoothDlg->m_ctrlComboFitLineC.InsertString(pDlg->pFitSmoothDlg->m_ctrlComboFitLineC.GetCount(),strTitleName);
	}
	if (pDlg->pCutLineDlg->GetSafeHwnd()!=NULL)
	{
		pDlg->pCutLineDlg->m_ctrlCutOutLineChoose.InsertString(pDlg->pCutLineDlg->m_ctrlCutOutLineChoose.GetCount(),strTitleName);
	}
	// lcz new 05/12
	//if (pDlg->pFitSmoothDlg.GetSafeHwnd()!=NULL)
	//{
	//	pDlg->pFitSmoothDlg.m_ctrlComboFitLineC.InsertString(pDlg->pFitSmoothDlg.m_ctrlComboFitLineC.GetCount(),strTitleName);
	//}
	//if (pDlg->pCutLineDlg.GetSafeHwnd()!=NULL)
	//{
	//	pDlg->pCutLineDlg.m_ctrlCutOutLineChoose.InsertString(pDlg->pCutLineDlg.m_ctrlCutOutLineChoose.GetCount(),strTitleName);
	//}

	nSmoothIndex = 0;


}


BOOL CSoomthDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	SetButtonState(&m_ctrlBtnSmoothDefine);
	SetButtonState(&m_ctrlBtnSmoothClean);
	CString name;
	for (int i = 1;i<51;i++)
	{
		name.Format(_T("%d"),i);
		m_ctrlSoomthNum.InsertString(i-1,name);
	}
	m_ctrlSoomthNum.SetCurSel(5);
	CINScanLiteDlg * pDlg=(CINScanLiteDlg*)AfxGetApp()->GetMainWnd();
	int nSeriesCount = pDlg->m_DlgShow.m_Tchart.GetSeriesCount();
	if (nSeriesCount==0) return TRUE;
	for (int i = 0 ;i<nSeriesCount;i++)
	{
		CString strSeriesName = pDlg->m_DlgShow.m_Tchart.Series(i).GetTitle();
		m_ctrlSoomthLine.InsertString(i,strSeriesName);
	}
	m_ctrlSoomthLine.SetCurSel(nSeriesCount - 1);
	UpdateLanguage();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


HBRUSH CSoomthDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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


void CSoomthDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialogEx::OnPaint()
	CRect rect;
	GetClientRect(rect);
	dc.FillSolidRect(rect,clr);
	CDialog::OnPaint();
}


void CSoomthDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CINScanLiteDlg * pDlg=(CINScanLiteDlg*)AfxGetApp()->GetMainWnd();
	if (nSmoothIndex>0)
	{
		pDlg->m_DlgShow.m_Tchart.RemoveSeries(nSmoothIndex);
	}
	if (pDlg->pSmoothDlg->GetSafeHwnd() !=NULL)
	{
		delete pDlg->pSmoothDlg;
		pDlg->pSmoothDlg = new CSoomthDlg(pDlg);
	}
	//CDialogEx::OnClose();
}

void CSoomthDlg::RemoveLine()
{
	CINScanLiteDlg * pDlg=(CINScanLiteDlg*)AfxGetApp()->GetMainWnd();

	if (nSmoothIndex>0)
	{
		pDlg->m_DlgShow.m_Tchart.RemoveSeries(nSmoothIndex);
		nSmoothIndex = -1;
	}
}


BOOL CSoomthDlg::PreTranslateMessage(MSG* pMsg)
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


void CSoomthDlg::OnCbnSelchangeComboSoomathLineChoose()
{
	// TODO: 在此添加控件通知处理程序代码
	CINScanLiteDlg * pDlg=(CINScanLiteDlg*)AfxGetApp()->GetMainWnd();
	pDlg->m_DlgShow.showSingleLine(m_ctrlSoomthLine.GetCurSel());
}


//void CSoomthDlg::OnCancel()
//{
//	// TODO: 在此添加专用代码和/或调用基类
//	CINScanLiteDlg * pDlg=(CINScanLiteDlg*)AfxGetApp()->GetMainWnd();
//	if (nSmoothIndex > 0)
//	{
//		pDlg->m_DlgShow.m_Tchart.RemoveSeries(nSmoothIndex);
//	}
//	nSmoothIndex = 0;
//
//	CDialogEx::OnCancel();
//}


void CSoomthDlg::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类
	OnClose();

	//CDialogEx::OnCancel();
}
