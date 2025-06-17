// DenoiseDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "INScan-Lite.h"
#include "INScan-LiteDlg.h"
#include "DenoiseDlg.h"
#include "..\file\TeeChart2011\series.h"
#include "..\file\TeeChart2011\valuelist.h"
#include "..\file\TeeChart2011\lineseries.h"
#include "..\file\TeeChart2011\pen.h"
#include <vector>

#include <numeric>
#include <algorithm>
#include<cmath>

// CDenoiseDlg 对话框

IMPLEMENT_DYNAMIC(CDenoiseDlg, CDialogEx)

CDenoiseDlg::CDenoiseDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDenoiseDlg::IDD, pParent)
	, nDenLineNum(-1)
	, nDenStart(0)
	, nDenEnd(0)
	, nStartLineNumber(0)
	, nEndLineNumber(0)
	, m_dNumArea(0)
	, m_dNumHeight(0)
	, m_dMaxFwhm(_T("0.0"))
{

}

CDenoiseDlg::~CDenoiseDlg()
{
}

void CDenoiseDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_CBIndex(pDX, IDC_COMBO_DEN_LINE, nDenLineNum);
	DDX_Text(pDX, IDC_EDIT_DEN_START, nDenStart);
	DDX_Text(pDX, IDC_EDIT_DEN_END, nDenEnd);
	DDX_Control(pDX, IDC_COMBO_DEN_LINE, pCtrlDenLines);
	DDX_Text(pDX, IDC_EDIT_NUM_AREA, m_dNumArea);
	DDX_Text(pDX, IDC_EDIT_NUM_HEIGHT, m_dNumHeight);
	//DDX_Control(pDX, IDC_BUTTON_DEN_ARE, m_ctrlBtnDenAre);
	DDX_Control(pDX, IDC_BUTTON_DEN_HIGHT, m_ctrlBtnDenHight);
	DDX_Control(pDX, IDC_BUTTON_LOOK_MINTOMAX, m_ctrlButtonLook);
	DDX_Control(pDX, IDC_LIST1, m_ListNe);
	DDX_Control(pDX, IDC_BTN_NE, m_ctrlBtnCptNe);
	DDX_Text(pDX, IDC_EDIT_MAX_NE, m_dMaxFwhm);
}


BEGIN_MESSAGE_MAP(CDenoiseDlg, CDialogEx)
	//ON_BN_CLICKED(IDC_BUTTON_DEN_ARE, &CDenoiseDlg::OnBnClickedButtonDenAre)
	ON_BN_CLICKED(IDC_BUTTON_DEN_HIGHT, &CDenoiseDlg::OnBnClickedButtonDenHight)
	ON_WM_CTLCOLOR()
	ON_WM_PAINT()
	ON_CBN_SELCHANGE(IDC_COMBO_DEN_LINE, &CDenoiseDlg::OnCbnSelchangeComboDenLine)
	ON_BN_CLICKED(IDC_BUTTON_LOOK_MINTOMAX, &CDenoiseDlg::OnBnClickedButtonLookMintomax)
	//ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BTN_NE, &CDenoiseDlg::OnBnClickedBtnNe)
END_MESSAGE_MAP()



//计算峰面积
//void CDenoiseDlg::OnBnClickedButtonDenAre()
//{
	// TODO: 在此添加控件通知处理程序代码
	//UpdateData(TRUE);
	//if (nDenLineNum<0) return;
	//if (nDenEnd<=nDenStart) return;
	//CINScanLiteDlg * pDlg=(CINScanLiteDlg*)AfxGetApp()->GetMainWnd();
	//CValueList list_x = pDlg->m_DlgShow.m_Tchart.Series(nDenLineNum).GetXValues();
	//CValueList list_y = pDlg->m_DlgShow.m_Tchart.Series(nDenLineNum).GetYValues();
	//vector<double> x_newList;
	//vector<double> y_newList;
	//int size = list_x.GetCount();
	//for (int i = 0;i<size;i++)
	//{
	//	if (list_x.GetValue(i)>=nDenStart&&list_x.GetValue(i)<=nDenEnd)
	//	{
	//		x_newList.push_back(list_x.GetValue(i));
	//		y_newList.push_back(list_y.GetValue(i));
	//	}
	//}
	//double* aa = new double[x_newList.size()*2];
	//for (int j =0;j<x_newList.size();j++)
	//{
	//	aa[j] = x_newList[j];
	//	aa[x_newList.size()+j] = y_newList[j];
	//}
	//double* data_area = new double[10];
	//int area_size = 0; 
	//
	//CString modeType=pDlg->GetIniInfo("Math","ModeType");
	//main_cal_peak_area(aa,x_newList.size()*2,atoi(modeType), 5, 1, 7, data_area);
	//m_dNumArea = data_area[0];
	//delete[] aa;
	//delete[] data_area;
	//UpdateData(FALSE);
	
//}

//计算半高宽和峰面积
void CDenoiseDlg::OnBnClickedButtonDenHight()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	m_dNumArea = 0;
	m_dNumHeight = 0;
	UpdateData(FALSE);
	if (nDenLineNum<0) return;
	if (nDenEnd<=nDenStart) return;
	yTemp.clear();
	CINScanLiteDlg * pDlg=(CINScanLiteDlg*)AfxGetApp()->GetMainWnd();
	CValueList list_x = pDlg->m_DlgShow.m_Tchart.Series(nDenLineNum).GetXValues();
	CValueList list_y = pDlg->m_DlgShow.m_Tchart.Series(nDenLineNum).GetYValues();
	int size = list_x.GetCount();
	//  获取原始的数据
	double* aa = new double[size*2];
	for (int j =0; j < size;  j++)
	{
		aa[j] = list_x.GetValue(j);
		aa[size + j] =  list_y.GetValue(j);
	}
	int start = 0;
	int end = 0;
	for (int i = 0;i<size;i++)
	{
		if (list_x.GetValue(i)>=nDenStart)
		{
			start = start==0?i:start;
		}
		if (list_x.GetValue(i)>=nDenEnd)
		{
			end = end==0?i:end;
		}
	}
	if(start >= end)
		return;
	//  选出所截取波段的极值点，求其索引
	//  截取波段的数据存入vector
	int bsize = end - start+1;
	for (int i = 0;i<bsize;i++)
	{
		yTemp.push_back(list_y.GetValue(start+i));
	}
	// 求取vector的最大值的索引，得到对应的峰位值
	int maxIndex = max_element(yTemp.begin(), yTemp.end()) - yTemp.begin() + start;
	double *peakShift = new double[1];
	peakShift[0] = list_x.GetValue(maxIndex);

	double flag = 0;
	double *fwhms = dll_calFwhmArea(aa, size * 2, peakShift, 1, flag );
	if (flag == 0)
	{
		CString tempTEXT = pDlg->LoadStringMeau(_T("ID_PEAK_NONE"));
		AfxMessageBox(tempTEXT, MB_ICONINFORMATION);
		return;
	}
	m_dNumArea = roundDouble(fwhms[1], 2);
	m_dNumHeight = roundDouble(fwhms[0], 2);
	dll_deleteFwhmArea();
	UpdateData(FALSE);
	if(aa)
	{
		delete[] aa;
		aa = NULL;
	}
	if(peakShift)
	{
		delete[] peakShift;
		peakShift = NULL;
	}
}

BOOL CDenoiseDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	SetButtonState(&m_ctrlBtnCptNe);
	SetButtonState(&m_ctrlBtnDenHight);
	SetButtonState(&m_ctrlButtonLook);
	CINScanLiteDlg * pDlg=(CINScanLiteDlg*)AfxGetApp()->GetMainWnd();
	int nSeriesCount = pDlg->m_DlgShow.m_Tchart.GetSeriesCount();
	if (nSeriesCount==0) return TRUE;
	for (int i = 0 ;i<nSeriesCount;i++)
	{
		CString strSeriesName = pDlg->m_DlgShow.m_Tchart.Series(i).GetTitle();
		pCtrlDenLines.AddString(strSeriesName);
	}

	//  lcz new 08/17  显示最新一条曲线
	pDlg->m_DlgShow.showSingleLine(nSeriesCount - 1);
	pCtrlDenLines.SetCurSel(nSeriesCount - 1);
	OnCbnSelchangeComboDenLine();

	UpdateLanguage();
	//  lcz new
	renewList();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDenoiseDlg::renewList()
{
	m_ListNe.DeleteAllItems();
	while (m_ListNe.DeleteColumn(0));	//清空所有表头
	CRect RECT;
	m_ListNe.GetClientRect(RECT);
	// TODO:  在此添加额外的初始化
	m_vPeaks.clear();
	CString boshu, FWHM;
	if (sInfo.nLanguage == 0)
	{
		boshu = TEXT("波数");
		FWHM = TEXT("半高宽");
	}
	else if (sInfo.nLanguage == 1)
	{
		boshu = TEXT("WaveNum");
		FWHM = TEXT("FWHM");
	}

	//  lcz new 23/05/12  如果是1062， 这三个峰是172  1476.7  2076.3
	CINScanLiteDlg * pDlg = (CINScanLiteDlg*)AfxGetApp()->GetMainWnd();
	if (pDlg->m_b1064)
	{
		CString strs[] = { boshu, TEXT("172"), TEXT("1476.7"), TEXT("2076.3") };  //  532mn
		m_vPeaks.push_back(172);
		m_vPeaks.push_back(1476.7);
		m_vPeaks.push_back(2076.3);
		for (int i = 0; i < sizeof(strs) / sizeof(CString); i++)
		{
			//设置表头，第一行标题，后面的数字表示列宽
			m_ListNe.InsertColumn(i, strs[i], LVCFMT_CENTER, RECT.right / (sizeof(strs) / sizeof(CString)));
		}
	}
	else
	{

		//  如果是532，则列表显示1710.3、2518.5、2839.1； 如果是785，则列表显示967.7、1777.5、2384.5
		if (sCInfo.Laser - 532 < 10)
		{
			CString s532rdWaveNum;
			s532rdWaveNum.Format("%.1f", sSInfo.d532rdWaveNum);
			CString strs[] = { boshu, TEXT("1710.3"), TEXT("2518.5"), s532rdWaveNum };  //  532mn
			m_vPeaks.push_back(1710.3);
			m_vPeaks.push_back(2518.5);
			m_vPeaks.push_back(sSInfo.d532rdWaveNum);
			for (int i = 0; i < sizeof(strs) / sizeof(CString); i++)
			{
				//设置表头，第一行标题，后面的数字表示列宽
				m_ListNe.InsertColumn(i, strs[i], LVCFMT_CENTER, RECT.right / (sizeof(strs) / sizeof(CString)));
			}
		}
		else
		{
			CString s785rdWaveNum;
			s785rdWaveNum.Format("%.1f", sSInfo.d785rdWaveNum);
			CString strs[] = { boshu, TEXT("967.7"), TEXT("1777.5"), s785rdWaveNum };  //  785nm
			m_vPeaks.push_back(967.7);
			m_vPeaks.push_back(1777.5);
			m_vPeaks.push_back(sSInfo.d785rdWaveNum);
			for (int i = 0; i < sizeof(strs) / sizeof(CString); i++)
			{
				//设置表头，第一行标题，后面的数字表示列宽
				m_ListNe.InsertColumn(i, strs[i], LVCFMT_CENTER, RECT.right / (sizeof(strs) / sizeof(CString)));
			}
		}

	}


	m_ListNe.InsertItem(0, FWHM);

	m_ListNe.SetExtendedStyle(m_ListNe.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);


	//  将第一列内容居中显示
	LVCOLUMN lvc;
	lvc.mask = LVCF_FMT;
	m_ListNe.GetColumn(0, &lvc);
	lvc.fmt &=~ LVCFMT_JUSTIFYMASK; 
	lvc.fmt |= LVCFMT_CENTER;
	m_ListNe.SetColumn(0, &lvc);
}

void CDenoiseDlg::UpdateLanguage()
{
	CINScanLiteDlg * pDlg=(CINScanLiteDlg*)AfxGetApp()->GetMainWnd();
	pDlg->m_ZLanguage.SetCreateLanguageState(FALSE);
	pDlg->m_ZLanguage.SetDialogStrings((long)(CDialog*)this,IDD);

	//SetControlFont();
}

HBRUSH CDenoiseDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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

void CDenoiseDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialogEx::OnPaint()
	CRect rect;
	GetClientRect(rect);
	dc.FillSolidRect(rect,clr);
	CDialog::OnPaint();
}

void CDenoiseDlg::addSeries(double nDenStart, double nDenEnd, double dMinNum ,double dMaxNum)
{
	CINScanLiteDlg * pDlg=(CINScanLiteDlg*)AfxGetApp()->GetMainWnd();
	if (nStartLineNumber==0)
	{
		nStartLineNumber = pDlg->m_DlgShow.m_Tchart.AddSeries(0);
	}
	COLORREF color = RGB(0,0,0);
	//pDlg->m_DlgShow.m_Tchart.Series(nStartLineNumber).GetAsLine().GetLinePen().SetWidth(1);
	pDlg->m_DlgShow.m_Tchart.Series(nStartLineNumber).Clear();
	pDlg->m_DlgShow.m_Tchart.Series(nStartLineNumber).SetActive(true);
	pDlg->m_DlgShow.m_Tchart.Series(nStartLineNumber).AddXY(nDenStart,dMinNum,_T(""),color);
	pDlg->m_DlgShow.m_Tchart.Series(nStartLineNumber).AddXY(nDenStart,dMaxNum,_T(""),color);
	pDlg->m_DlgShow.m_Tchart.Series(nStartLineNumber).SetShowInLegend(FALSE);


	if (nEndLineNumber==0)
	{
		nEndLineNumber = pDlg->m_DlgShow.m_Tchart.AddSeries(0);
	}
	pDlg->m_DlgShow.m_Tchart.Series(nEndLineNumber).Clear();
	pDlg->m_DlgShow.m_Tchart.Series(nEndLineNumber).SetActive(true);
	pDlg->m_DlgShow.m_Tchart.Series(nEndLineNumber).AddXY(nDenEnd,dMinNum,_T(""),color);
	pDlg->m_DlgShow.m_Tchart.Series(nEndLineNumber).AddXY(nDenEnd,dMaxNum,_T(""),color);
	pDlg->m_DlgShow.m_Tchart.Series(nEndLineNumber).SetShowInLegend(FALSE);
}
//  下拉框切换曲线时触发
void CDenoiseDlg::OnCbnSelchangeComboDenLine()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (nDenLineNum<0) return;
	CINScanLiteDlg * pDlg=(CINScanLiteDlg*)AfxGetApp()->GetMainWnd();
	//  根据选择的曲线，框图里只显示一条曲线
	pDlg->m_DlgShow.showSingleLine(nDenLineNum);
	CValueList xList = pDlg->m_DlgShow.m_Tchart.Series(nDenLineNum).GetXValues();
	CValueList yList = pDlg->m_DlgShow.m_Tchart.Series(nDenLineNum).GetYValues();
	double dMaxNum = yList.GetMaximum();
	nDenStart = xList.GetValue(0);
	nDenStart = (floor(nDenStart * 100 ) )/100;
	nDenEnd = xList.GetLast();
	nDenEnd = (floor(nDenEnd * 100 ) )/100;
	double dMinNum = yList.GetMinimum();
	//addSeries(nDenStart,nDenEnd,dMinNum,dMaxNum);
	m_dNumArea = 0;
	m_dNumHeight = 0;
	renewList();
	m_dMaxFwhm = TEXT("0.0");
	UpdateData(FALSE);

}

//  查看范围
void CDenoiseDlg::OnBnClickedButtonLookMintomax()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (nDenLineNum<0) return;
	CINScanLiteDlg * pDlg=(CINScanLiteDlg*)AfxGetApp()->GetMainWnd();
	CValueList yList = pDlg->m_DlgShow.m_Tchart.Series(nDenLineNum).GetYValues();
	double dMaxNum = yList.GetMaximum();
	double dMinNum = yList.GetMinimum();
	addSeries(nDenStart,nDenEnd,dMinNum,dMaxNum);
}

void CDenoiseDlg::RemoveLine()
{
	CINScanLiteDlg * pDlg=(CINScanLiteDlg*)AfxGetApp()->GetMainWnd();
	
	if (nEndLineNumber>0)
	{
		pDlg->m_DlgShow.m_Tchart.RemoveSeries(nEndLineNumber);
		nEndLineNumber = -1;
	}
	if (nStartLineNumber>0)
	{
		pDlg->m_DlgShow.m_Tchart.RemoveSeries(nStartLineNumber);
		nStartLineNumber = -1;
	}
}

void CDenoiseDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
 	CINScanLiteDlg * pDlg=(CINScanLiteDlg*)AfxGetApp()->GetMainWnd();
	
	if (nEndLineNumber>0)
	{
		pDlg->m_DlgShow.m_Tchart.RemoveSeries(nEndLineNumber);
	}
	if (nStartLineNumber>0)
	{
		pDlg->m_DlgShow.m_Tchart.RemoveSeries(nStartLineNumber);
	}
	if (pDlg->pDenoiseDlg->GetSafeHwnd()!=NULL)
	{
		delete pDlg->pDenoiseDlg;
		pDlg->pDenoiseDlg = new CDenoiseDlg(pDlg);
	}

	//CDialogEx::OnClose();
}

BOOL CDenoiseDlg::PreTranslateMessage(MSG* pMsg)
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

//  计算氖灯谱线的半高宽
void CDenoiseDlg::OnBnClickedBtnNe()
{
	renewList();
	m_dMaxFwhm = TEXT("0.0");
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(FALSE);
	if (nDenLineNum<0) return;
	if (nDenEnd<=nDenStart) return;
	CINScanLiteDlg * pDlg=(CINScanLiteDlg*)AfxGetApp()->GetMainWnd();
	CValueList list_x = pDlg->m_DlgShow.m_Tchart.Series(nDenLineNum).GetXValues();
	CValueList list_y = pDlg->m_DlgShow.m_Tchart.Series(nDenLineNum).GetYValues();
	int size = list_x.GetCount();
	//  获取原始的数据                                  aa 待释放
	double* aa = new double[size*2];
	for (int j =0; j < size;  j++)
	{
		aa[j] = list_x.GetValue(j);
		aa[size + j] =  list_y.GetValue(j);
	}
	//  将峰位值导入数组
	int numOfPeaks = m_vPeaks.size();
	double *Peaks = new double[numOfPeaks]; //  Peaks 待释放
	for(int i = 0; i < numOfPeaks; i++)
	{
		Peaks[i] = m_vPeaks[i];
	}
	double flag = 0;
	double * fwhms = dll_calFwhmArea(aa, size * 2, Peaks, numOfPeaks, flag); //  fwhms 待释放
	if (flag == 0)
	{
		if (aa)
		{
			delete[] aa;
			aa = NULL;
		}
		if (Peaks)
		{
			delete[] Peaks;
			Peaks = NULL;
		}
		dll_deleteFwhmArea();
		CString tempTEXT = pDlg->LoadStringMeau(_T("ID_FWHM_ERROR"));
		AfxMessageBox(tempTEXT, MB_ICONINFORMATION);
		return;
	}
	else
	{
		//  如果计算半高宽成功，则将其显示出来，并显示最大值
		CString strValue = TEXT("");
		double maxFwhm = 0.0;
		for(int i = 0; i < numOfPeaks; i++)
		{
			strValue.Format(_T("%.2f"), fwhms[i]);
			m_ListNe.SetItemText(0, i + 1, strValue);
			if(fwhms[i] > maxFwhm)
				maxFwhm = fwhms[i];
		}
		m_dMaxFwhm.Format(_T("%.2f"), maxFwhm);
		UpdateData(FALSE);
		//  释放内存
		if (aa)
		{
			delete[] aa;
			aa = NULL;
		}
		if (Peaks)
		{
			delete[] Peaks;
			Peaks = NULL;
		}
		dll_deleteFwhmArea();
	}
	
}


//void CDenoiseDlg::OnCancel()
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


void CDenoiseDlg::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类
	OnClose();

	//CDialogEx::OnCancel();
}
