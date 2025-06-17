// CutOutLineDlg.cpp : 实现文件
//

#include "stdafx.h"
#include <vector>
#include "INScan-Lite.h"
#include "CutOutLineDlg.h"
#include "afxdialogex.h"
#include "INScan-LiteDlg.h"
//#include "lines\BaselineCorrect.h"
//#include "lines\LineMath.h"
//  lcz  new
#include "lines\zolix_algorithm.h"
#include "..\file\TeeChart2011\series.h"
#include "..\file\TeeChart2011\valuelist.h"
#include "..\file\TeeChart2011\legend.h"



// CCutOutLineDlg 对话框

IMPLEMENT_DYNAMIC(CCutOutLineDlg, CDialogEx)

	CCutOutLineDlg::CCutOutLineDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CCutOutLineDlg::IDD, pParent)
{

	m_dCutOutLineEnd = 0;
	m_dCutOutLineStart = 0;
	m_nCutOutLineIndex = -1;
}

CCutOutLineDlg::~CCutOutLineDlg()
{
}

void CCutOutLineDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_CUTOUT_LINE_CHOOSE, m_ctrlCutOutLineChoose);
	DDX_Text(pDX, IDC_EDIT_CUTOUT_LINE_END, m_dCutOutLineEnd);
	DDX_Text(pDX, IDC_EDIT_CUTOUT_LINE_START, m_dCutOutLineStart);
	DDX_Control(pDX, IDC_MFCBUTTON_CUTOUT_LINE_CLEAN, m_ctrlButtonCutOutLineClean);
	DDX_Control(pDX, IDC_MFCBUTTON_CUTOUT_LINE_DEFINE, m_ctrlBtnCutOutLineDefine);
}


BEGIN_MESSAGE_MAP(CCutOutLineDlg, CDialogEx)
	ON_BN_CLICKED(IDC_MFCBUTTON_CUTOUT_LINE_CLEAN, &CCutOutLineDlg::OnBnClickedMfcbuttonCutoutLineClean)
	ON_BN_CLICKED(IDC_MFCBUTTON_CUTOUT_LINE_DEFINE, &CCutOutLineDlg::OnBnClickedMfcbuttonCutoutLineDefine)
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
	ON_CBN_SELCHANGE(IDC_COMBO_CUTOUT_LINE_CHOOSE, &CCutOutLineDlg::OnCbnSelchangeComboCutoutLineChoose)
	//ON_WM_CLOSE()
END_MESSAGE_MAP()


// CCutOutLineDlg 消息处理程序
void cutLine(CValueList x_list,CValueList y_list,double m_dCutOutLineStart,double m_dCutOutLineEnd,vector<double>* x,vector<double>* y)
{
	for (int i = 0;i <x_list.GetCount();i++)
	{
		if (x_list.GetValue(i)>m_dCutOutLineStart&&x_list.GetValue(i)<m_dCutOutLineEnd)
		{
			x->push_back(x_list.GetValue(i));
			y->push_back(y_list.GetValue(i));
		}
	}
}

//  预览截取后的曲线
void CCutOutLineDlg::OnBnClickedMfcbuttonCutoutLineClean()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	int nSelected = m_ctrlCutOutLineChoose.GetCurSel();
	if (nSelected<0) return;
	CString strSeriesName;
	m_ctrlCutOutLineChoose.GetLBText(nSelected,strSeriesName);

	CINScanLiteDlg * pDlg=(CINScanLiteDlg*)AfxGetApp()->GetMainWnd();
	CSeries series = pDlg->m_DlgShow.m_Tchart.Series(nSelected);
	CValueList x_List = series.GetXValues();
	CValueList y_List = series.GetYValues();
	vector<double> x_newList;
	vector<double> y_newList;
	cutLine(x_List,y_List,m_dCutOutLineStart,m_dCutOutLineEnd,&x_newList,&y_newList);
	if (x_newList.size()<=0)
	{
		MessageBox(_T("请输入合理的范围"));
		return;
	}
	// 	if ( pDlg->m_DlgShow.m_Tchart.GetSeriesCount()>=15)
	// 	{
	// 		MessageBox(_T("线的数据超过15条"));
	// 		return;
	// 	}
	if (m_nCutOutLineIndex<=0)
	{
		m_nCutOutLineIndex = pDlg->m_DlgShow.m_Tchart.AddSeries(0);
	}
	CString strNewSeriesName;
	strNewSeriesName.Format("%s_%s_%.2f-%.2f",strSeriesName,"CUT",m_dCutOutLineStart,m_dCutOutLineEnd);

	pDlg->m_DlgShow.m_Tchart.Series(m_nCutOutLineIndex).SetTitle(strNewSeriesName);
	int color = pDlg->m_DlgShow.m_Tchart.Series(m_nCutOutLineIndex).GetColor();
	pDlg->m_DlgShow.m_Tchart.Series(m_nCutOutLineIndex).Clear();
	pDlg->m_DlgShow.m_Tchart.Series(m_nCutOutLineIndex).SetActive(TRUE);
	for (int j=0;j<x_newList.size();j++)
	{
		pDlg->m_DlgShow.m_Tchart.Series(m_nCutOutLineIndex).AddXY(x_newList[j],y_newList[j],NULL,color);
	}
	//  SetShowInLegend(FALSE)则不将该曲线添加显示
	pDlg->m_DlgShow.m_Tchart.Series(m_nCutOutLineIndex).SetShowInLegend(FALSE);
}

//   确定截取
void CCutOutLineDlg::OnBnClickedMfcbuttonCutoutLineDefine()
{
	// TODO: 在此添加控件通知处理程序代码

	//  lcz new 07/18判断现有的条数是否为maxLineNum,，如果已经存在maxLineNum条曲线，提示先删除部分曲线
	CINScanLiteDlg * pDlg=(CINScanLiteDlg*)AfxGetApp()->GetMainWnd();
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
	}

	UpdateData(TRUE);
	//CINScanLiteDlg * pDlg=(CINScanLiteDlg*)AfxGetApp()->GetMainWnd();
	//  如果没有预览
	if (m_nCutOutLineIndex<0)
	{
		int nSelected = m_ctrlCutOutLineChoose.GetCurSel();
		CString strSeriesName;
		m_ctrlCutOutLineChoose.GetLBText(nSelected,strSeriesName);
		if (nSelected<0) return;
		CSeries series = pDlg->m_DlgShow.m_Tchart.Series(nSelected);
		CValueList x_List = series.GetXValues();
		CValueList y_List = series.GetYValues();
		vector<double> x_newList;
		vector<double> y_newList;
		cutLine(x_List,y_List,m_dCutOutLineStart,m_dCutOutLineEnd,&x_newList,&y_newList);
		if (x_newList.size()<=0)
		{
			MessageBox(_T("请输入合理的范围"));
			return;
		}
		// 		if ( pDlg->m_DlgShow.m_Tchart.GetSeriesCount()>=15)
		// 		{
		// 			MessageBox(_T("线的数据超过15条"));
		// 			return;
		// 		}
		if (m_nCutOutLineIndex<0)
		{
			m_nCutOutLineIndex = pDlg->m_DlgShow.m_Tchart.AddSeries(0);
		}
		CString strNewSeriesName;
		strNewSeriesName.Format("%s_%s_%.2f-%.2f",strSeriesName,"CUT",m_dCutOutLineStart,m_dCutOutLineEnd);

		pDlg->m_DlgShow.m_Tchart.Series(m_nCutOutLineIndex).SetTitle(strNewSeriesName);

		int color = pDlg->m_DlgShow.m_Tchart.Series(m_nCutOutLineIndex).GetColor();
		pDlg->m_DlgShow.m_Tchart.Series(m_nCutOutLineIndex).Clear();
		pDlg->m_DlgShow.m_Tchart.Series(m_nCutOutLineIndex).SetActive(TRUE);
		for (int j=0;j<x_newList.size();j++)
		{
			pDlg->m_DlgShow.m_Tchart.Series(m_nCutOutLineIndex).AddXY(x_newList[j],y_newList[j],NULL,color);
		}
	}

	pDlg->m_DlgShow.m_Tchart.Series(m_nCutOutLineIndex).SetShowInLegend(TRUE);
	CString strTitleName = pDlg->m_DlgShow.m_Tchart.Series(m_nCutOutLineIndex).GetTitle();
	m_ctrlCutOutLineChoose.InsertString(m_ctrlCutOutLineChoose.GetCount(),strTitleName);
	if (pDlg->pFitSmoothDlg->GetSafeHwnd()!=NULL)
	{
		pDlg->pFitSmoothDlg->m_ctrlComboFitLineC.InsertString(pDlg->pFitSmoothDlg->m_ctrlComboFitLineC.GetCount(),strTitleName);
	}
	if (pDlg->pSmoothDlg->GetSafeHwnd()!=NULL)
	{
		pDlg->pSmoothDlg->m_ctrlSoomthLine.InsertString(pDlg->pSmoothDlg->m_ctrlSoomthLine.GetCount(),strTitleName);
	}

	// lcz new 05/12
	//if (pDlg->pFitSmoothDlg.GetSafeHwnd()!=NULL)
	//{
	//	pDlg->pFitSmoothDlg.m_ctrlComboFitLineC.InsertString(pDlg->pFitSmoothDlg.m_ctrlComboFitLineC.GetCount(),strTitleName);
	//}
	//if (pDlg->pSmoothDlg.GetSafeHwnd()!=NULL)
	//{
	//	pDlg->pSmoothDlg.m_ctrlSoomthLine.InsertString(pDlg->pSmoothDlg.m_ctrlSoomthLine.GetCount(),strTitleName);
	//}
	m_nCutOutLineIndex = -1;

}


BOOL CCutOutLineDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	SetButtonState(&m_ctrlBtnCutOutLineDefine);
	SetButtonState(&m_ctrlButtonCutOutLineClean);

	CINScanLiteDlg * pDlg=(CINScanLiteDlg*)AfxGetApp()->GetMainWnd();
	int nSeriesCount = pDlg->m_DlgShow.m_Tchart.GetSeriesCount();
	if (nSeriesCount==0) return TRUE;
	for (int i = 0 ;i<nSeriesCount;i++)
	{
		CString strSeriesName = pDlg->m_DlgShow.m_Tchart.Series(i).GetTitle();
		m_ctrlCutOutLineChoose.InsertString(i,strSeriesName);
	}
	m_ctrlCutOutLineChoose.SetCurSel(nSeriesCount - 1);
	UpdateLanguage();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
void CCutOutLineDlg::UpdateLanguage()
{
	CINScanLiteDlg * pDlg=(CINScanLiteDlg*)AfxGetApp()->GetMainWnd();
	pDlg->m_ZLanguage.SetCreateLanguageState(FALSE);
	pDlg->m_ZLanguage.SetDialogStrings((long)(CDialog*)this,IDD);

	//SetControlFont();
}

void CCutOutLineDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialogEx::OnPaint()
	CRect rect;
	GetClientRect(rect);
	dc.FillSolidRect(rect,clr);
	CDialog::OnPaint();
}


HBRUSH CCutOutLineDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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


void CCutOutLineDlg::OnCbnSelchangeComboCutoutLineChoose()
{
	// TODO: 在此添加控件通知处理程序代码
	int nCursel = m_ctrlCutOutLineChoose.GetCurSel();
	CINScanLiteDlg * pDlg=(CINScanLiteDlg*)AfxGetApp()->GetMainWnd();
	CValueList xList = pDlg->m_DlgShow.m_Tchart.Series(nCursel).GetXValues();
	m_dCutOutLineStart = xList.GetValue(0);
	m_dCutOutLineEnd = xList.GetLast();
	//  lcz new 3/21 保留两位小数  四舍五入
	m_dCutOutLineStart = roundDouble(m_dCutOutLineStart, 1);
	m_dCutOutLineEnd = roundDouble(m_dCutOutLineEnd, 1);
	pDlg->m_DlgShow.showSingleLine(m_ctrlCutOutLineChoose.GetCurSel());
	UpdateData(FALSE);
}

void CCutOutLineDlg::RemoveLine()
{
	CINScanLiteDlg * pDlg=(CINScanLiteDlg*)AfxGetApp()->GetMainWnd();

	if (m_nCutOutLineIndex>=0)
	{
		pDlg->m_DlgShow.m_Tchart.RemoveSeries(m_nCutOutLineIndex);
		m_nCutOutLineIndex = -1;
	}
}

void CCutOutLineDlg::OnClose()
{
//	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CINScanLiteDlg * pDlg=(CINScanLiteDlg*)AfxGetApp()->GetMainWnd();
	if (m_nCutOutLineIndex>=0)
	{
		pDlg->m_DlgShow.m_Tchart.RemoveSeries(m_nCutOutLineIndex);
	}
	if (pDlg->pCutLineDlg->GetSafeHwnd()!=NULL)
	{
		delete pDlg->pCutLineDlg;
		pDlg->pCutLineDlg = new CCutOutLineDlg(pDlg);
	}
//	//CDialogEx::OnClose();
//
}

//void CCutOutLineDlg::OnCancel()
//{
//	// TODO: 在此添加专用代码和/或调用基类
//	CINScanLiteDlg * pDlg=(CINScanLiteDlg*)AfxGetApp()->GetMainWnd();
//	if (m_nCutOutLineIndex>=0)
//	{
//		pDlg->m_DlgShow.m_Tchart.RemoveSeries(m_nCutOutLineIndex);
//	}
//	m_nCutOutLineIndex = -1;
//
//	CDialogEx::OnCancel();
//}


BOOL CCutOutLineDlg::PreTranslateMessage(MSG* pMsg)
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





void CCutOutLineDlg::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类
	OnClose();

	//CDialogEx::OnCancel();
}
