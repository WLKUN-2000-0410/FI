 // FitSmoothDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "INScan-Lite.h"
#include "FitSmoothDlg.h"
#include "afxdialogex.h"
#include "INScan-LiteDlg.h"
#include "..\file\TeeChart2011\series.h"
#include "..\file\TeeChart2011\valuelist.h"
#include "..\file\TeeChart2011\lineseries.h"
#include "..\file\TeeChart2011\pen.h"




// CFitSmoothDlg 对话框

IMPLEMENT_DYNAMIC(CFitSmoothDlg, CDialogEx)

CFitSmoothDlg::CFitSmoothDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CFitSmoothDlg::IDD, pParent)
{

	nFitIndex = 0;
}

CFitSmoothDlg::~CFitSmoothDlg()
{
	//this->DestroyWindow();
}

void CFitSmoothDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_FIT_LINE_CHOOSE, m_ctrlComboFitLineC);
	//	DDX_Control(pDX, IDC_SLIDER_FIT_NUM, m_ctrlSliderFitNum);
	//	DDX_Control(pDX, IDC_SLIDER_SMOOTH_NUM, m_ctrlSliderSmoothNum);
	DDX_Control(pDX, IDC_COMBO_FIT_NUM, m_ctrlComboFitNum);
	DDX_Control(pDX, IDC_BUTTON_FIT_CLEAN, m_ctrlBtnFitClean);
	DDX_Control(pDX, IDC_BUTTON_FIT_DEFINE, m_ctrlBtnFitDefine);
}


BEGIN_MESSAGE_MAP(CFitSmoothDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_FIT_CLEAN, &CFitSmoothDlg::OnBnClickedButtonFitClean)
	ON_BN_CLICKED(IDC_BUTTON_FIT_DEFINE, &CFitSmoothDlg::OnBnClickedButtonFitDefine)
	ON_WM_CTLCOLOR()
	ON_WM_PAINT()
	ON_CBN_SELCHANGE(IDC_COMBO_FIT_LINE_CHOOSE, &CFitSmoothDlg::OnCbnSelchangeComboFitLineChoose)
	ON_WM_DESTROY()
	//ON_WM_CLOSE()
END_MESSAGE_MAP()


// CFitSmoothDlg 消息处理程序


BOOL CFitSmoothDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	SetButtonState(&m_ctrlBtnFitClean);
	SetButtonState(&m_ctrlBtnFitDefine);
	CString name;
	for (int i = 1;i<51;i++)
	{
		name.Format(_T("%d"),i);
		m_ctrlComboFitNum.InsertString(i-1,name);
	}
	m_ctrlComboFitNum.SetCurSel(5);
	CINScanLiteDlg * pDlg=(CINScanLiteDlg*)AfxGetApp()->GetMainWnd();
	int nSeriesCount = pDlg->m_DlgShow.m_Tchart.GetSeriesCount();
	if (nSeriesCount==0) return TRUE;
	for (int i = 0 ;i<nSeriesCount;i++)
	{
		CString strSeriesName = pDlg->m_DlgShow.m_Tchart.Series(i).GetTitle();
		m_ctrlComboFitLineC.InsertString(i,strSeriesName);
	}
	m_ctrlComboFitLineC.SetCurSel(nSeriesCount - 1);
	UpdateLanguage();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CFitSmoothDlg::OnCbnSelchangeComboFitNum()
{
	// TODO: 在此添加控件通知处理程序代码
	
	
}

//  基线拟合---预览
void CFitSmoothDlg::OnBnClickedButtonFitClean()
{
	// TODO: 在此添加控件通知处理程序代码
	
		/*CINScanLiteDlg * pDlg=(CINScanLiteDlg*)AfxGetApp()->GetMainWnd();
		pDlg->m_DlgShow.m_Tchart.RemoveSeries(nFitIndex);
		m_ctrlComboFitLineC.DeleteString(nFitIndex);
		nFitIndex = 0;*/

		int nFitNums =  m_ctrlComboFitNum.GetCurSel()+1;//  拟合的幂次数
		//m_ctrlSliderFitNum.GetPos();
		int nSerieSize = m_ctrlComboFitLineC.GetCount();// 选取的待拟合的曲线数目
		if (nSerieSize==0) return ;
		int nSelected = m_ctrlComboFitLineC.GetCurSel();//  待拟合的曲线所在的序号
		if (nSelected<0) return;
		CINScanLiteDlg * pDlg=(CINScanLiteDlg*)AfxGetApp()->GetMainWnd();
// 		if (pDlg->m_DlgShow.m_Tchart.GetSeriesCount()>15)
// 		{
// 			MessageBox(_T("线的条数超过十五条"));
// 			return ;
// 		}
		CSeries series = pDlg->m_DlgShow.m_Tchart.Series(nSelected);
		CString strSeriesName = series.GetTitle();
		CValueList x_List = series.GetXValues();
		CValueList y_List = series.GetYValues();
		int nXSize = x_List.GetCount();
		double* aa = new double[nXSize];
		double *baselineData = new double[nXSize];
		for (int i= 0; i<nXSize;i++)
		{
			aa[i] = y_List.GetValue(i);
		}
		dll_baseline_correct(aa, nXSize, nFitNums, baselineData);

		int resSize = 0;   double flag = 0;
		//dll_baseline_correct(aa, nXSize*2, nFitNums, &flag);

		if (nFitIndex==0)
		{
			nFitIndex = pDlg->m_DlgShow.m_Tchart.AddSeries(0);

		} 
		CString strNewSeriesName;
		strNewSeriesName.Format("%s_%s_%d",strSeriesName,"FIT",nFitNums);
		pDlg->m_DlgShow.m_Tchart.Series(nFitIndex).SetTitle(strNewSeriesName);
		int color = pDlg->m_DlgShow.m_Tchart.Series(nFitIndex).GetColor();
		//pDlg->m_DlgShow.m_Tchart.Series(nFitIndex).GetAsLine().GetLinePen().SetWidth(1);
		pDlg->m_DlgShow.m_Tchart.Series(nFitIndex).Clear(); //  原先代码
		pDlg->m_DlgShow.m_Tchart.Series(nFitIndex).SetActive(TRUE);

		for(int j = 0; j < nXSize;  j++)
		{
			pDlg->m_DlgShow.m_Tchart.Series(nFitIndex).AddXY(x_List.GetValue(j), baselineData[j], _T(""),color);
		}

		//pDlg->m_DlgShow.m_Tchart.Series(nFitIndex>1?(nFitIndex-1):1).SetShowInLegend(FALSE);
		pDlg->m_DlgShow.m_Tchart.Series(nFitIndex).SetShowInLegend(FALSE);

		delete[] aa;
		delete[] baselineData;
}

void CFitSmoothDlg::UpdateLanguage()
{
	CINScanLiteDlg * pDlg=(CINScanLiteDlg*)AfxGetApp()->GetMainWnd();
	pDlg->m_ZLanguage.SetCreateLanguageState(FALSE);
	pDlg->m_ZLanguage.SetDialogStrings((long)(CDialog*)this,IDD);

	//SetControlFont();
}

//  基线拟合---确定
void CFitSmoothDlg::OnBnClickedButtonFitDefine()
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
	}// lcz new 07/18

	int nSerieSize = m_ctrlComboFitLineC.GetCount();
	if (nSerieSize==0) return ;
	int nSelected = m_ctrlComboFitLineC.GetCurSel();
	if (nSelected<0) return;
// 	if (pDlg->m_DlgShow.m_Tchart.GetSeriesCount()>15)
// 	{
// 		MessageBox(_T("线的条数超过十五条"));
// 		return ;
// 	}
	//  如果在点击确定之前没有预览，则直接显示扣除基线之后的光谱数据
	//if (nFitIndex==0)//  原先代码
	//{
		int nFitNums =  m_ctrlComboFitNum.GetCurSel()+1;
		CSeries series = pDlg->m_DlgShow.m_Tchart.Series(nSelected);
		CString strSeriesName = series.GetTitle();
		CValueList x_List = series.GetXValues();
		CValueList y_List = series.GetYValues();
		int nXSize = x_List.GetCount();
		double* aa = new double[nXSize];
		double *baselineData = new double[nXSize];
		double *correctedData = new double[nXSize];
		for (int i= 0; i<nXSize;i++)
		{
			aa[i] = y_List.GetValue(i);
		}
		dll_baseline_correct(aa, nXSize, nFitNums, baselineData);
		//*    lcz  new
		for (int i = 0; i < nXSize; i++)
		{
			correctedData[i] = aa[i] - baselineData[i];
		}
	if (nFitIndex==0)//如果在点击确定之前没有预览，则直接显示扣除基线之后的光谱数据
	{
		nFitIndex = pDlg->m_DlgShow.m_Tchart.AddSeries(0);
		CString strNewSeriesName;
		strNewSeriesName.Format("%s_%s_%d",strSeriesName,"FIT",nFitNums);
		pDlg->m_DlgShow.m_Tchart.Series(nFitIndex).SetTitle(strNewSeriesName);
		int color = pDlg->m_DlgShow.m_Tchart.Series(nFitIndex).GetColor();
		//pDlg->m_DlgShow.m_Tchart.Series(nFitIndex).GetAsLine().GetLinePen().SetWidth(1);
		pDlg->m_DlgShow.m_Tchart.Series(nFitIndex).Clear();
		pDlg->m_DlgShow.m_Tchart.Series(nFitIndex).SetActive(TRUE);
		//for (int j = 0; j<nResult/2;j++)
		//{
		//	//pDlg->m_DlgShow.m_Tchart.Series(nFitIndex).AddXY(x_List.GetValue(j),correctsignal[nResult/2+j],_T(""),color);
		//	pDlg->m_DlgShow.m_Tchart.Series(nFitIndex).AddXY(x_List.GetValue(j),correctedsignal[nResult/2+j],_T(""),color);//  lcz  new

		//}
		for (int j = 0; j < nXSize; j++)
		{
			pDlg->m_DlgShow.m_Tchart.Series(nFitIndex).AddXY(x_List.GetValue(j), correctedData[j], _T(""),color);
		}
	}
	//  lcz  new     如果在点击确定之前点了预览
	else
	{
		pDlg->m_DlgShow.m_Tchart.Series(nFitIndex).Clear(); //  清除掉原先的基线
		CString strNewSeriesName;
		strNewSeriesName.Format("%s_%s_%d",strSeriesName,"FIT",nFitNums);
		pDlg->m_DlgShow.m_Tchart.Series(nFitIndex).SetTitle(strNewSeriesName);
		int color = pDlg->m_DlgShow.m_Tchart.Series(nFitIndex).GetColor();
		//pDlg->m_DlgShow.m_Tchart.Series(nFitIndex).GetAsLine().GetLinePen().SetWidth(1);
		pDlg->m_DlgShow.m_Tchart.Series(nFitIndex).Clear();

		for (int j = 0; j < nXSize; j++)
		{
			pDlg->m_DlgShow.m_Tchart.Series(nFitIndex).AddXY(x_List.GetValue(j), correctedData[j], _T(""),color);
		}
	}
	delete[] aa;
	delete[] baselineData;
	delete[] correctedData;

	pDlg->m_DlgShow.m_Tchart.Series(nFitIndex).SetShowInLegend(TRUE);
	//CString strSeriesName = pDlg->m_DlgShow.m_Tchart.Series(nFitIndex).GetTitle();
	strSeriesName = pDlg->m_DlgShow.m_Tchart.Series(nFitIndex).GetTitle();//  lcz  new
	m_ctrlComboFitLineC.InsertString(m_ctrlComboFitLineC.GetCount(),strSeriesName);
	if (pDlg->pSmoothDlg->GetSafeHwnd()!=NULL)
	{
		pDlg->pSmoothDlg->m_ctrlSoomthLine.InsertString(pDlg->pSmoothDlg->m_ctrlSoomthLine.GetCount(),strSeriesName);
	}
	if (pDlg->pCutLineDlg->GetSafeHwnd()!=NULL)
	{
		pDlg->pCutLineDlg->m_ctrlCutOutLineChoose.InsertString(pDlg->pCutLineDlg->m_ctrlCutOutLineChoose.GetCount(),strSeriesName);
	}

	// lcz new 05/12
	//if (pDlg->pSmoothDlg.GetSafeHwnd()!=NULL)
	//{
	//	pDlg->pSmoothDlg.m_ctrlSoomthLine.InsertString(pDlg->pSmoothDlg.m_ctrlSoomthLine.GetCount(),strSeriesName);
	//}
	//if (pDlg->pCutLineDlg.GetSafeHwnd()!=NULL)
	//{
	//	pDlg->pCutLineDlg.m_ctrlCutOutLineChoose.InsertString(pDlg->pCutLineDlg.m_ctrlCutOutLineChoose.GetCount(),strSeriesName);
	//}
	nFitIndex = 0;
	
}


HBRUSH CFitSmoothDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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


void CFitSmoothDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialogEx::OnPaint()
	CRect rect;
	GetClientRect(rect);
	dc.FillSolidRect(rect,clr);
	CDialog::OnPaint();
}


void CFitSmoothDlg::OnCbnSelchangeComboFitLineChoose()
{
	// TODO: 在此添加控件通知处理程序代码
	//  lcz -new
	CINScanLiteDlg * pDlg=(CINScanLiteDlg*)AfxGetApp()->GetMainWnd();
	pDlg->m_DlgShow.showSingleLine(m_ctrlComboFitLineC.GetCurSel());
}


void CFitSmoothDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
}

void CFitSmoothDlg::RemoveLine()
{
	CINScanLiteDlg * pDlg=(CINScanLiteDlg*)AfxGetApp()->GetMainWnd();
	
	if (nFitIndex>0)
	{
		pDlg->m_DlgShow.m_Tchart.RemoveSeries(nFitIndex);
		nFitIndex = -1;
	}
}

void CFitSmoothDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CINScanLiteDlg * pDlg=(CINScanLiteDlg*)AfxGetApp()->GetMainWnd();
	if (nFitIndex>0)
	{
		pDlg->m_DlgShow.m_Tchart.RemoveSeries(nFitIndex);
	}
	if (pDlg->pFitSmoothDlg->GetSafeHwnd() !=NULL)
	{
		delete pDlg->pFitSmoothDlg;
		pDlg->pFitSmoothDlg = new CFitSmoothDlg(pDlg);
	}
	//CDialogEx::OnClose();
}


BOOL CFitSmoothDlg::PreTranslateMessage(MSG* pMsg)
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


//void CFitSmoothDlg::OnCancel()
//{
//	// TODO: 在此添加专用代码和/或调用基类
//	CINScanLiteDlg * pDlg=(CINScanLiteDlg*)AfxGetApp()->GetMainWnd();
//	if (nFitIndex > 0)
//	{
//		pDlg->m_DlgShow.m_Tchart.RemoveSeries(nFitIndex);
//	}
//	nFitIndex = 0;
//
//	CDialogEx::OnCancel();
//}


void CFitSmoothDlg::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类
	OnClose();

	//CDialogEx::OnCancel();
}
