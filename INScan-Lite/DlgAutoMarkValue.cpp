// DlgAutoMarkValue.cpp : 实现文件
//
//#ifdef _DEBUG
//#define new DEBUG_NEW
//#undef THIS_FILE
//static char THIS_FILE[] = __FILE__;
//#endif

#include "stdafx.h"
#include "DlgAutoMarkValue.h"
#include "afxdialogex.h"
#include <math.h>
#include "INScan-LiteDlg.h"
#include "..\file\TeeChart2011\valuelist.h"
#include "..\file\TeeChart2011\series.h"
#include "..\file\TeeChart2011\axis.h"
#include "..\file\TeeChart2011\axes.h"
#include "..\file\TeeChart2011\zoom.h"
#include "..\file\TeeChart2011\lineseries.h"
#include "..\file\TeeChart2011\printer.h"
#include "..\file\TeeChart2011\pen.h"
#include "..\file\TeeChart2011\marks.h"
#include "..\file\TeeChart2011\markscallout.h"
#include "..\file\TeeChart2011\teefont.h"
#define e 2.718281828459 
//  写文件，中文部分所需
#include <locale> 


// CDlgAutoMarkValue 对话框

IMPLEMENT_DYNAMIC(CDlgAutoMarkValue, CDialogEx)

CDlgAutoMarkValue::CDlgAutoMarkValue(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgAutoMarkValue::IDD, pParent)
	, m_dPeakInt(50)
	, m_dPeakPro(50)
	, m_iBaselineTimes(20)
	, m_iSGtimes(3)
	, m_bBaseline(true)
	, m_bSGSM(true)
	, oldPeakInt(50)
	, oldPeakPro(50)
	, nAddBaselineIndex(0)
	, nAddSGSMIndex(0)
	, twoLines(1)
{
	selectSeriesSize = 0;
	peakInfo[0] = -1; peakInfo[1] = -1;peakInfo[2] = -1; peakInfo[3] = -1;
	saveCSVName = TEXT("");
	listColor = RGB(230, 230, 230);
	previewLineTotal = 1;
}

CDlgAutoMarkValue::~CDlgAutoMarkValue()
{
	
	//this->DestroyWindow();
}

void CDlgAutoMarkValue::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	//DDX_Text(pDX, IDC_SLIDERVALUE, m_sigvalue);
	DDX_Control(pDX, IDC_TCHART_S, m_ctrlTchart);
	DDX_Control(pDX, IDC_COMBO_LINE_CHOOSE, m_ctrlComboLineChoose);

	DDX_Control(pDX, IDC_BTN_PREVIEW_Z, m_BTN_preview);
	DDX_Control(pDX, IDC_BTN_ASSURE_Z, m_BTN_assure);

	DDX_Control(pDX, IDC_COMBO_BASELINE_Z, m_combo_baseline);
	DDX_Control(pDX, IDC_COMBO_SG_SM_Z, m_combo_sg_sm);
	DDX_Control(pDX, IDC_LIST_PEAK_INFO, m_list_peakInfo);
	DDX_Control(pDX, IDC_BTN_OPT_DATA, m_BTN_opt_data);
	DDX_Control(pDX, IDC_BTN_DEL_ALL_PEAKS, m_BTN_del_all_peaks);
}


BEGIN_MESSAGE_MAP(CDlgAutoMarkValue, CDialogEx)
	//ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_HANDONE, &CDlgAutoMarkValue::OnBnClickedHandone)
	ON_BN_CLICKED(IDC_DELALL, &CDlgAutoMarkValue::OnBnClickedDelall)
	ON_BN_CLICKED(IDC_DELONE, &CDlgAutoMarkValue::OnBnClickedDelone)
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
	//ON_CBN_SELCHANGE(IDC_COMBO1, &CDlgAutoMarkValue::OnCbnSelchangeCombo1)
	ON_CBN_SELCHANGE(IDC_COMBO_LINE_CHOOSE, &CDlgAutoMarkValue::OnCbnSelchangeComboLineChoose)

	ON_CBN_SELCHANGE(IDC_COMBO_BASELINE_Z, &CDlgAutoMarkValue::OnCbnSelchangeComboBaselineZ)
	ON_CBN_SELCHANGE(IDC_COMBO_SG_SM_Z, &CDlgAutoMarkValue::OnCbnSelchangeComboSgSmZ)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_PeakInt_Z, &CDlgAutoMarkValue::OnNMReleasedcaptureSliderPeakintZ)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_PeakPro_Z, &CDlgAutoMarkValue::OnNMReleasedcaptureSliderPeakproZ)
	ON_BN_CLICKED(IDC_CHECK_BASELINE_Z, &CDlgAutoMarkValue::OnBnClickedCheckBaselineZ)
	ON_BN_CLICKED(IDC_CHECK_SG_SM_Z, &CDlgAutoMarkValue::OnBnClickedCheckSgSmZ)
	ON_BN_CLICKED(IDC_BTN_PREVIEW_Z, &CDlgAutoMarkValue::OnBnClickedBtnPreviewZ)
	ON_BN_CLICKED(IDC_BTN_ASSURE_Z, &CDlgAutoMarkValue::OnBnClickedBtnAssureZ)

	ON_NOTIFY( NM_CUSTOMDRAW, IDC_LIST_PEAK_INFO, OnDrawColorForMyList ) 
	ON_BN_CLICKED(IDC_BTN_OPT_DATA, &CDlgAutoMarkValue::OnBnClickedBtnOptData)
	ON_BN_CLICKED(IDC_BTN_DEL_ALL_PEAKS, &CDlgAutoMarkValue::OnBnClickedBtnDelAllPeaks)
END_MESSAGE_MAP()


// CDlgAutoMarkValue 消息处理程序


//void CDlgAutoMarkValue::OnClose()
//{
//	// TODO: 在此添加消息处理程序代码和/或调用默认值
//
//	is_HandMark=FALSE;
//	is_DelMark=FALSE;
//	//CDialogEx::OnClose();
//}


BOOL CDlgAutoMarkValue::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	//  设置复选框、滑片的初始值
	((CButton*)GetDlgItem(IDC_CHECK_BASELINE_Z))->SetCheck(TRUE);
	((CButton*)GetDlgItem(IDC_CHECK_SG_SM_Z))->SetCheck(TRUE);
	((CButton*)GetDlgItem(IDC_HANDONE))->SetCheck(TRUE);

	((CSliderCtrl *)GetDlgItem(IDC_SLIDER_PeakInt_Z))->SetRange(1,100);
	((CSliderCtrl *)GetDlgItem(IDC_SLIDER_PeakPro_Z))->SetRange(1,100);
	((CSliderCtrl *)GetDlgItem(IDC_SLIDER_PeakInt_Z))->SetPos(50);
	((CSliderCtrl *)GetDlgItem(IDC_SLIDER_PeakInt_Z))->SetPageSize(5);
	GetDlgItem(IDC_EDIT_PeakInt_Z)->SetWindowText("50");

	((CSliderCtrl *)GetDlgItem(IDC_SLIDER_PeakPro_Z))->SetPos(50);
	((CSliderCtrl *)GetDlgItem(IDC_SLIDER_PeakPro_Z))->SetPageSize(5);
	GetDlgItem(IDC_EDIT_PeakPro_Z)->SetWindowText("5.4");
	//  设置预处理里下拉框的内容
	CString name;
	for (int i = 1;i<51;i++)
	{
		name.Format(_T("%d"),i);
		m_combo_baseline.InsertString(i-1,name);
		m_combo_sg_sm.InsertString(i-1,name);
	}
	m_combo_baseline.SetCurSel(19);
	m_combo_sg_sm.SetCurSel(2);

	CINScanLiteDlg * pThreadApp=(CINScanLiteDlg*)AfxGetApp()->GetMainWnd();

	is_HandMark=TRUE;
	is_DelMark=FALSE;
	
	int nSeriesCount = pThreadApp->m_DlgShow.m_Tchart.GetSeriesCount();
	if (nSeriesCount==0) return TRUE;
	for (int i = 0 ;i<nSeriesCount;i++)
	{
		CString strSeriesName = pThreadApp->m_DlgShow.m_Tchart.Series(i).GetTitle();
		m_ctrlComboLineChoose.InsertString(i, strSeriesName);
	}
	//  显示第一条曲线, 根据数据点数来显示数据，横坐标从1到num, 以表示像素值
	//ShowData(0);
	ShowData(nSeriesCount - 1);
	m_ctrlComboLineChoose.SetCurSel(nSeriesCount - 1);
	OnCbnSelchangeComboLineChoose();

	selectSeriesSize = m_ctrlTchart.GetSeriesCount();
	UpdateLanguage();
	
	SetButtonState(&m_BTN_preview);
	SetButtonState(&m_BTN_assure);
	SetButtonState(&m_BTN_set_t);
	SetButtonState(&m_BTN_opt_data);
	SetButtonState(&m_BTN_del_all_peaks);
	//  设置list表格的背景色
	m_list_peakInfo.SetBkColor(listColor);
	
	m_vPeakInfo.clear();
	renewList();
	UpdateData(TRUE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDlgAutoMarkValue::UpdateLanguage()
{
	CINScanLiteDlg * pThreadApp=(CINScanLiteDlg*)AfxGetApp()->GetMainWnd();

	pThreadApp->m_ZLanguage.SetCreateLanguageState(FALSE);
	pThreadApp->m_ZLanguage.SetDialogStrings((long)(CDialog*)this,IDD);
}


void CDlgAutoMarkValue::OnBnClickedHandone()
{
	// TODO: 在此添加控件通知处理程序代码
	is_HandMark=TRUE;
	is_DelMark=FALSE;
}

void CDlgAutoMarkValue::OnBnClickedDelall()
{
	// TODO: 在此添加控件通知处理程序代码
	CINScanLiteDlg * pThreadApp=(CINScanLiteDlg*)AfxGetApp()->GetMainWnd();
	is_HandMark=TRUE;
	is_DelMark=FALSE;
	((CButton*)GetDlgItem(IDC_HANDONE))->SetCheck(TRUE);
	((CButton*)GetDlgItem(IDC_DELALL))->SetCheck(FALSE);

	/*double maxvalue = m_ctrlTchart.Series(0).GetYValues().GetMaximum();
	m_ctrlTchart.GetAxis().GetLeft().SetMaximum(maxvalue);*/
	//  由于可能只显示预处理后的曲线，所以来个自动缩放
	//OnUndoZoomTchartS();
	m_ctrlTchart.GetAxis().GetLeft().SetAutomatic(TRUE);
	m_ctrlTchart.GetAxis().GetBottom().SetAutomatic(TRUE);

	if(previewLineTotal > twoLines)// 如果点了预览
	{
		// 如果预览前已进行了寻峰，第二条数据就是峰
		if(m_ctrlTchart.Series(m_ctrlTchart.GetSeriesCount()>1?1:0).GetCount() == 1)
			DeleteAllSer(twoLines);
		else
			DeleteAllSer(previewLineTotal);
	}
	else
		DeleteAllSer(twoLines);
	renewList();
	Sleep(500);	
	//m_ctrlTchart.Repaint();
}

void CDlgAutoMarkValue::OnBnClickedDelone()
{
	// TODO: 在此添加控件通知处理程序代码

	is_HandMark=FALSE;
	is_DelMark=TRUE;
}

void CDlgAutoMarkValue::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	CRect rect;
	GetClientRect(rect);
	dc.FillSolidRect(rect,clr);
	CDialog::OnPaint();
}

HBRUSH CDlgAutoMarkValue::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
	if (nCtlColor==CTLCOLOR_STATIC)
	{
		pDC->SetBkColor(clr);
		HBRUSH b =CreateSolidBrush(clr);
		return b;
	}
	return hbr;
}


void CDlgAutoMarkValue::DeleteAllSer(int nR)
{

	int size=m_ctrlTchart.GetSeriesCount();

	while(m_ctrlTchart.GetSeriesCount()>nR)
	{
		//for (int i=nR;i<m_ctrlTchart.GetSeriesCount();i++)
		//{
		//	if(m_ctrlTchart.Series(i).GetCount() == 1)
		//		m_ctrlTchart.RemoveSeries(i);
		//}
		for (int i=nR;i<m_ctrlTchart.GetSeriesCount();)
		{
			if(m_ctrlTchart.Series(m_ctrlTchart.GetSeriesCount() - 1).GetCount() == 1)
				m_ctrlTchart.RemoveSeries(m_ctrlTchart.GetSeriesCount() - 1);
		}
	}
	//m_ctrlTchart.GetAxis().GetLeft().SetAutomatic(TRUE);
	//m_ctrlTchart.GetAxis().GetBottom().SetAutomatic(TRUE);
	//m_ctrlTchart.GetAxis().GetRight().SetAutomatic(TRUE);
	//m_ctrlTchart.GetAxis().GetTop().SetAutomatic(TRUE);
}

void CDlgAutoMarkValue::ReMoveSeries(int param1)
{

	if ( m_ctrlTchart.GetSeriesCount()<1)
		return;
	//  删除标记的谱峰，下列代码有bug
	/*for (int i=0;i<m_ctrlTchart.GetSeriesCount();i++)
	{
	int count = m_ctrlTchart.GetSeriesCount();
	for (int j=0;j<count;j++)
	{
	int convalue = m_ctrlTchart.Series(j).GetCount();
	if (convalue==1)
	{
	m_ctrlTchart.RemoveSeries(j);
	break;
	}
	}
	}*/
	int temp1 = m_ctrlTchart.GetSeriesCount();
	for(int i = 0; i < temp1; i++)
	{
		int temp2 = m_ctrlTchart.GetSeriesCount();//  记录有多少条线
		//int convalue = m_ctrlTchart.Series(temp2 - 1).GetCount();//  记录最后一条线的点数
		//// 如果最后一条线的点数为1，则表示是标记的谱峰点，将其删除
		//if (convalue==1)
		//{
		//	m_ctrlTchart.RemoveSeries(temp2 - 1);
		//}
		//else
		//	break;
		//  lcz new 04/25
		if(temp2 > twoLines)
			m_ctrlTchart.RemoveSeries(temp2 - 1);
	}

	m_ctrlTchart.GetAxis().GetLeft().SetAutomatic(TRUE);
	m_ctrlTchart.GetAxis().GetBottom().SetAutomatic(TRUE);
	m_ctrlTchart.GetAxis().GetRight().SetAutomatic(TRUE);
	m_ctrlTchart.GetAxis().GetTop().SetAutomatic(TRUE);
}

//  自动缩放
void CDlgAutoMarkValue::OnUndoZoomTchartS()
{
	// TODO: 在此处添加消息处理程序代码
	int temp2 = m_ctrlTchart.GetSeriesCount();
	if(temp2 <= twoLines)//  说明没有添加标峰点
	{
		m_ctrlTchart.GetAxis().GetLeft().SetAutomatic(TRUE);
		m_ctrlTchart.GetAxis().GetBottom().SetAutomatic(TRUE);
		return;
	}

	int temp1 = m_ctrlTchart.GetSeriesCount();
	double maxValue = 0, minValue = 65500;
	for(int i = 0; i < temp1; i++)
	{
		if(m_ctrlTchart.Series(i).GetActive())
		{
			if( m_ctrlTchart.Series(i).GetCount()>1 )
			{
				double max_yvalue = m_ctrlTchart.Series(i).GetYValues().GetMaximum();
				double min_yvalue = m_ctrlTchart.Series(i).GetYValues().GetMinimum();
				if(max_yvalue > maxValue)
					maxValue = max_yvalue;
				if(min_yvalue < minValue)
					minValue = min_yvalue;
				m_ctrlTchart.GetAxis().GetLeft().SetAutomaticMaximum(FALSE);
				if(twoLines == 2)//  如果有原曲线和预处理后的曲线，若原曲线极大值大于1.8倍预处理后曲线极大值，则图标极大值为原曲线极大值
					m_ctrlTchart.GetAxis().GetLeft().SetMaximum(maxValue > 1.8 * max_yvalue? maxValue : 1.8 * max_yvalue);
				else
					m_ctrlTchart.GetAxis().GetLeft().SetMaximum(maxValue * 1.8);
				m_ctrlTchart.GetAxis().GetLeft().SetMinimum(minValue);
			}
		}
	}


	//m_ctrlTchart.GetAxis().GetLeft().SetAutomatic(TRUE);
	m_ctrlTchart.GetAxis().GetBottom().SetAutomatic(TRUE);
	//m_ctrlTchart.GetAxis().GetTop().SetAutomatic(TRUE);

	//  自动缩放后也更新显示谱峰，防止谱峰显示越界以及漏显示
	showPeaks();
}

//  自动标峰
void CDlgAutoMarkValue::MarksValue(int in_width,double in_maxvalue)
{
		int temp1 = m_ctrlTchart.GetSeriesCount();
		renewList();//  每次标峰都需要初始化表格
	    ReMoveSeries(selectSeriesSize);//  将之前标的谱峰都删除
		//ShowData(m_ctrlComboLineChoose.GetCurSel());
		int temp3 = m_ctrlTchart.GetSeriesCount();
		double yvalue = 0.00;

		double lim_ydata = 0.00;
	
		double max_yvalue = m_ctrlTchart.Series(0).GetYValues().GetMaximum();
		double min_yvalue = m_ctrlTchart.Series(0).GetYValues().GetMinimum();

		CINScanLiteDlg * pThreadApp=(CINScanLiteDlg*)AfxGetApp()->GetMainWnd();

		m_ctrlTchart.GetAxis().GetLeft().SetAutomaticMaximum(FALSE);
		m_ctrlTchart.GetAxis().GetLeft().SetMaximum(max_yvalue + (max_yvalue-min_yvalue)/2);
		CValueList ylist = m_ctrlTchart.Series(0).GetYValues();
		CValueList xlist = m_ctrlTchart.Series(0).GetXValues();
		int cont = m_ctrlTchart.Series(0).GetCount();
		//  生成待处理的数据
		//  lcz new 04/19  如果在寻峰前未进行平滑(或者次数小于3），则将数据先进行平滑;
		double *resData = new double[cont * 2];
		//if(twoLines == 1 || (twoLines == 2 && !m_bSGSM ) || (twoLines == 2 && m_bSGSM && m_iSGtimes < 3 ))
		//{
			double *temp_resY = new double[cont];
			memset(temp_resY, 0.0, sizeof(double) * cont);
			double *temp_SmoothedY = new double[cont];
			memset(temp_SmoothedY, 0.0, sizeof(double) * cont);
			for(int i = 0; i < cont; i++)
				temp_resY[i] = m_vRawData[i];// 读取原始数据
			dll_SG_Smooth(temp_resY, cont, 3, temp_SmoothedY);//  进行平滑
			for(int i = 0; i < cont; i++)
			{
				resData[i] = xlist.GetValue(i);
				resData[i + cont] = temp_SmoothedY[i];
			}
			// 22/08/01 之前忘记释放这段内存
			if(temp_resY)
			{
				delete[] temp_resY;  temp_resY = NULL;
			}
			if(temp_SmoothedY)
			{
				delete[] temp_SmoothedY;  temp_SmoothedY = NULL;
			}
		//}
		//else
		//{
		//	for(int i = 0; i < cont; i++)
		//	{
		//		resData[i] = xlist.GetValue(i);
		//		resData[i + cont] = m_vResData[i];//  未平滑的数据
		//	}
		//}
		//  新的寻峰算法
		double peakNum = 0;
		double *peakInfo = dll_findPeaksG(resData, cont * 2, ((double)m_dPeakInt) / 100, m_dPeakPro / 100, &peakNum);
		//  得到处理后的数据的最大值
		double maxPeakInt = 0.0;
		for(int i = 0; i < m_vResData.size(); i++)
		{
			if(maxPeakInt < m_vResData[i])
				maxPeakInt = m_vResData[i];
		}
		//double *peakInfo = dll_findPeaksG(resData, cont * 2, 0.1, 0.08, &peakNum);
		//in_width = (int)((in_width-1)/2);
		//  x, xLen, y, yLen, 敏感度, 强度阈值, 半高宽阈值, 平滑宽度, 平滑类型（默认7,3）, 峰组数, 寻峰结果, 谱峰个数
		//main_findpeaksG(pX, cont, pY, cont, m_fValue_L, lim_ydata, in_width, m_fValue_PW, iSoomthType, m_fValue_N, pOutY, &fOutSize);
		//  lcz  new 由于寻峰里添加了平滑项，因此检测到的谱峰的强度会偏低，改为利用检测的峰位，在原始数据里获取峰强
		for(int i = 0; i < peakNum * 7; i++)
			TRACE(L"%d : %.2f\n", i, peakInfo[i]);

		m_vPeakInfo.clear(); //  清空容器
		int peakCount = 0;
		for(int i = 0; i < peakNum; i++)
		{
			
			// lcz  算法结果异常点，得到的谱峰的横坐标超出了曲线范围
			//if(pOutY[i*7+1] > xlist.GetValue(cont - 1))
			//	continue;
			//m_ctrlTchart.Series(al).AddXY(pOutY[i*7+1], pOutY[i*7+2], NULL, RGB(255,0,0));
			//m_ctrlTchart.Series(al).AddXY(roundDouble(pOutY[i*7+1], 1), roundDouble(realPeakInt[i], 1), NULL, RGB(255,0,0));
			//  1. 找到谱峰对应的像素点
			double peakShift = peakInfo[i * 7+1];
			int peakPixel = 0;
			for(int j = 0; j < cont - 1; j ++)
			{
				if( ( xlist.GetValue(j) <= peakShift ) && ( xlist.GetValue(j + 1) >= peakShift ))
				{
					if(abs(xlist.GetValue(j) - peakShift) < abs(xlist.GetValue(j + 1) - peakShift))
						peakPixel = j;
					else
						peakPixel = j + 1;
					break;
				}
				
			}
			//  2. 找到最接近的波数/波长对应的像素值后，在其左右五个点找极值
			vector<double> seriesInstisy;//  seriesInstisy 记录该像素点附近的约11个点的强度值
			int startIndex = 0;
			if(peakPixel < 5)
			{
				for(int j = 0; j <= peakPixel + 5; j++)
					seriesInstisy.push_back(m_vResData[j]);
			}
			else if(peakPixel >= cont - 5)
			{
				startIndex = peakPixel - 5;
				for(int j = peakPixel - 5; j < cont; j++)
					seriesInstisy.push_back(m_vResData[j]);
			}
			else
			{
				startIndex = peakPixel - 5;
				for(int j = peakPixel - 5; j <= peakPixel + 5; j++)
					seriesInstisy.push_back(m_vResData[j]);
			}
			int maxIndex = max_element(seriesInstisy.begin(), seriesInstisy.end()) - seriesInstisy.begin() + startIndex;
			vector<double> seriesInstisyOfMaxPixel;//  seriesInstisy 记录极值点对应的像素点附近的约5个点的强度值
			vector<double> seriesShiftOfMaxPixel;//  记录五个点的像素值
			if(maxIndex < 3)
			{
				for(int j = 0; j <= maxIndex + 3; j++)
				{
					seriesShiftOfMaxPixel.push_back(xlist.GetValue(j));
					seriesInstisyOfMaxPixel.push_back(m_vResData[j]);
				}
			}
			else if(maxIndex >= cont - 3)
			{
				startIndex = maxIndex - 3;
				for(int j = maxIndex - 3; j < cont; j++)
				{
					seriesShiftOfMaxPixel.push_back(xlist.GetValue(j));
					seriesInstisyOfMaxPixel.push_back(m_vResData[j]);
				}
			}
			else
			{
				startIndex = maxIndex - 3;
				for(int j = maxIndex - 3; j <= maxIndex + 3; j++)
				{
					seriesShiftOfMaxPixel.push_back(xlist.GetValue(j));
					seriesInstisyOfMaxPixel.push_back(m_vResData[j]);
				}
			}
			double realPeakShift = 0.0;
			double realPeakInt = 0.0;
			if(seriesInstisyOfMaxPixel.size() > 0)
			{
				double* aa = new double[seriesInstisyOfMaxPixel.size() * 2];
				for(int i = 0; i < seriesInstisyOfMaxPixel.size(); i++)
				{
					aa[i] = seriesShiftOfMaxPixel[i];
					aa[i + seriesInstisyOfMaxPixel.size()] = seriesInstisyOfMaxPixel[i];
				}
				double pos = 0.0, height = 0.0, width = 0.0;
				dll_GuassFitAll(aa, seriesInstisyOfMaxPixel.size() * 2, &pos, &height, &width);
				//  lcz new  四舍五入，保留两位小数（只对正数有效）
				realPeakShift = roundDouble(pos, 1);
				realPeakInt = height;
				if(aa)
				{
					delete[] aa;
					aa = NULL;
				}
			}
			else
			{
				realPeakShift = peakInfo[ 7*i + 1 ];
				realPeakInt = peakInfo[ 7*i + 2 ];
			}

			TRACE(L"%.1f  : %.1f\n", peakInfo[ 7*i + 1 ], realPeakShift);
			

			// lcz new 04/25  平滑后的强度可能会降低
			if( realPeakInt > maxPeakInt * m_dPeakInt / 100 - 50 )
			{
				peakCount++;
				int al = m_ctrlTchart.AddSeries(0);
				m_ctrlTchart.Series(al).AddXY(roundDouble(realPeakShift, 1), roundDouble(realPeakInt, 1), NULL, RGB(255,0,0));

				m_ctrlTchart.Series(al).GetMarks().SetBackColor(RGB(255,255,255));
				m_ctrlTchart.Series(al).GetMarks().SetTextAlign(0);			///标记点的左侧 当前是竖直状态所以要左侧 如果是水平应该是中心
				m_ctrlTchart.Series(al).GetMarks().GetFont().SetSize(11);	////标记点的文本大小
				m_ctrlTchart.Series(al).GetMarks().GetFont().SetBold(TRUE); ///数据加粗
				m_ctrlTchart.Series(al).GetMarks().SetTransparent(TRUE);	///不加框
				m_ctrlTchart.Series(al).GetMarks().SetArrowLength(30);
				m_ctrlTchart.Series(al).GetMarks().GetArrow().SetMode(0);//  0 箭头为黑色， 1 箭头为白色

				m_ctrlTchart.Series(al).GetMarks().GetArrow().SetColor(RGB(0,0,0));
				m_ctrlTchart.Series(al).GetMarks().GetArrow().SetStyle(0);// 箭头为虚线，0为实线
				m_ctrlTchart.Series(al).GetMarks().GetArrow().SetWidth(2);

				m_ctrlTchart.Series(al).GetMarks().GetCallout().SetArrowHead(1);////设置箭头样式，0不带箭头（纯线段），1 带箭头
				m_ctrlTchart.Series(al).GetMarks().GetCallout().SetArrowHeadSize(8);///箭头大小
				m_ctrlTchart.Series(al).GetMarks().GetArrow().SetVisible(TRUE);
				m_ctrlTchart.Series(al).SetShowInLegend(FALSE);
				m_ctrlTchart.Series(al).GetMarks().SetStyle(9);//8显示的是X 
				m_ctrlTchart.Series(al).GetMarks().SetAngle(90);//标签旋转90度
				m_ctrlTchart.Series(al).GetMarks().SetVisible(TRUE);

				// 将谱峰信息存入m_vPeakInfo
				m_vPeakInfo.push_back(peakCount); // 存入序号
				//m_vPeakInfo.push_back(roundDouble(peakInfo[i * 7+1], 1)); // 存入峰位
				//m_vPeakInfo.push_back(roundDouble(peakInfo[i * 7+2], 1)); // 存入峰强
				m_vPeakInfo.push_back(roundDouble(realPeakShift, 1)); // 存入峰位
				m_vPeakInfo.push_back(roundDouble(realPeakInt, 1)); // 存入峰强
				m_vPeakInfo.push_back(roundDouble(peakInfo[i * 7+3], 1)); // 存入半高宽
				m_vPeakInfo.push_back(roundDouble(peakInfo[i * 7+4], 1)); // 存入峰面积
			}
		}
		int temp2 = m_ctrlTchart.GetSeriesCount();
		delete[] resData; resData = NULL;
		dll_deletePeakInfo();

		OnUndoZoomTchartS();
		fillList(); //  更新表格
}



BEGIN_EVENTSINK_MAP(CDlgAutoMarkValue, CDialogEx)
	ON_EVENT(CDlgAutoMarkValue, IDC_TCHART_S, 5, CDlgAutoMarkValue::OnClickBackgroundTchartS, VTS_I4 VTS_I4 VTS_I4 VTS_I4)
	ON_EVENT(CDlgAutoMarkValue, IDC_TCHART_S, 7, CDlgAutoMarkValue::OnClickSeriesTchartS, VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_I4)
	ON_EVENT(CDlgAutoMarkValue, IDC_TCHART_S, 26, CDlgAutoMarkValue::OnUndoZoomTchartS, VTS_NONE)
	ON_EVENT(CDlgAutoMarkValue, IDC_TCHART_S, 27, CDlgAutoMarkValue::OnZoomTchartS, VTS_NONE)
	ON_EVENT(CDlgAutoMarkValue, IDC_TCHART_S, 21, CDlgAutoMarkValue::OnMouseUpTchartS, VTS_I4 VTS_I4 VTS_I4 VTS_I4)
	ON_EVENT(CDlgAutoMarkValue, IDC_TCHART_S, 2, CDlgAutoMarkValue::OnAllowScrollTchartS, VTS_I4 VTS_PR8 VTS_PR8 VTS_PBOOL)
END_EVENTSINK_MAP()


//  点击图像上的一点进行手动标峰或者手动删峰
void CDlgAutoMarkValue::OnClickBackgroundTchartS(long Button, long Shift, long X, long Y)
{
	int nSeriesIndex = m_ctrlComboLineChoose.GetCurSel();
	double xvalue =m_ctrlTchart.Series(twoLines - 1).XScreenToValue(X);
	if(is_HandMark==TRUE){
		AddPointMarks(nSeriesIndex,X,Y);}
	if (is_DelMark==TRUE){
		DeleteChMarks(xvalue);}
}


void CDlgAutoMarkValue::OnClickSeriesTchartS(long SeriesIndex, long ValueIndex, long Button, long Shift, long X, long Y)
{
	int nSeriesIndex = m_ctrlComboLineChoose.GetCurSel();
	//double xvalue =m_ctrlTchart.Series(nSeriesIndex).XScreenToValue(X);
	double xvalue =m_ctrlTchart.Series(twoLines - 1).XScreenToValue(X);
	if(is_HandMark==TRUE){
		AddPointMarks(nSeriesIndex,X,Y);}
	if (is_DelMark==TRUE){
		DeleteChMarks(xvalue);}
}

//  删除所标的某个峰
void CDlgAutoMarkValue::DeleteChMarks(double xvalue)
{
	int a = m_ctrlTchart.GetSeriesCount();
	for (int i=0;i<a;i++)
	{
		int co = m_ctrlTchart.Series(i).GetCount();
		if (co==1)
		{  
			CValueList xx;
			xx  = m_ctrlTchart.Series(i).GetXValues();
			double xv = xx.GetValue(0);
			if (xv > xvalue - 10 && xv <= xvalue + 10)
			{
				m_ctrlTchart.RemoveSeries(i);
				int peakIndex = i - twoLines;
				m_vPeakInfo.erase(m_vPeakInfo.begin() + 5 * peakIndex, m_vPeakInfo.begin() + 5 * peakIndex + 5);
				fillList();
				break;
			}
		}
	}
}

//  手动标峰
void CDlgAutoMarkValue::AddPointMarks(long SeriesIndex,long x,double y)
{
	////先判断 防止标峰出界
	SeriesIndex = twoLines - 1;// 如果只有原始曲线，SeriesIndex = 0； 如果有原始有预览，SeriesIndex = 1
	int sercon =m_ctrlTchart.GetSeriesCount();
	BOOL isnum=FALSE;
	for (int i=0;i<sercon;i++)
	{
		int p = m_ctrlTchart.Series(i).GetCount();
		if (p==1)
		{
			isnum=TRUE;
			break;
		}
	}
	if (isnum==FALSE)
	{
		//double max_yvalue = m_ctrlTchart.Series(0).GetYValues().GetMaximum();
		//double min_yvalue = m_ctrlTchart.Series(0).GetYValues().GetMinimum();
		////lim_ydata = (max_yvalue - min_yvalue)*in_maxvalue/100+min_yvalue;
		//m_ctrlTchart.GetAxis().GetLeft().SetAutomaticMaximum(FALSE);
		//m_ctrlTchart.GetAxis().GetLeft().SetMaximum(max_yvalue+(max_yvalue-min_yvalue)/2);
	} 

	CINScanLiteDlg * pThreadApp=(CINScanLiteDlg*)AfxGetApp()->GetMainWnd();

	/*double max_yvalue = pThreadApp->m_DlgShow.m_Tchart.GetAxis().GetLeft().GetMaximum();
	double min_yvalue = pThreadApp->m_DlgShow.m_Tchart.GetAxis().GetLeft().GetMinimum();
	m_ctrlTchart.GetAxis().GetLeft().SetAutomaticMaximum(FALSE);
	m_ctrlTchart.GetAxis().GetLeft().SetMaximum(max_yvalue+(max_yvalue-min_yvalue)/2);*/
	
	
	double minxvalue=0.00;
	double maxxvalue=0.00;
	double rxvalue= 0.00;
	double ryvalue = 0.00;

	std::vector<double> idex,idey;
	/////添加鼠标点击的X范围判断, 在左右范围内的十个数据点里找极大值
	double xx = m_ctrlTchart.Series(SeriesIndex).XScreenToValue(x);
	minxvalue = xx - 10;
	maxxvalue = xx + 10;

	CValueList xl = m_ctrlTchart.Series(SeriesIndex).GetXValues();
	CValueList yl = m_ctrlTchart.Series(SeriesIndex).GetYValues();
	int linecount = m_ctrlTchart.Series(SeriesIndex).GetCount();

	//  使用的是m_vResData而不是yl
	double *data = new double[linecount * 2];
	for (int i = 0; i < linecount; i++)
	{
		data[i] = xl.GetValue(i);
		data[i + linecount] = m_vResData[i];

		if (xl.GetValue(i)>=minxvalue && xl.GetValue(i)<=maxxvalue)
		{
			if (ryvalue<yl.GetValue(i))
			{
				rxvalue = xl.GetValue(i); // 极大值处的X值
				ryvalue = yl.GetValue(i);
			}
		}

	}
	///判断结束
	if(rxvalue == 0)
		return;
	else if(rxvalue > xl.GetValue(linecount - 1))
		return;
	//  如果该X值有效，则计算手动标峰的值 
	double flag = 0;

	double maxPeakInt = 0.0, maxRawInt = 0.0;
	for(int i = 0; i < m_vResData.size(); i++)
	{
		if(maxPeakInt < m_vResData[i])
			maxPeakInt = m_vResData[i];
	}
	for(int i = 0; i < m_vRawData.size(); i++)
	{
		if(maxRawInt < m_vRawData[i])
			maxRawInt = m_vRawData[i];
	}
	double ratio = maxRawInt / maxPeakInt;

	dll_markPeak(data, linecount * 2, rxvalue, 0.08, 0.04 * ratio , peakInfo, &flag);
	delete[] data; data = NULL;
	bool notFound = true;//  默认未被寻到
	if(peakInfo[0] == -1)
		return;
	else
	{
		//  比较该峰是否已经寻到
		for (int i = 0; i < m_vPeakInfo.size()/5; i++)
		{
			double tempX = m_vPeakInfo[ i * 5 + 1];
			if(peakInfo[0] - tempX >= -6 && peakInfo[0] - tempX <= 6)
			{
				notFound = false;
				break;
			}
		}
	}
	if(notFound) //  如果有峰，需要先与已经寻到的谱峰进行比较，如果不在里面再进行标记，避免重复
	{
		
		int al = m_ctrlTchart.AddSeries(0);
		m_ctrlTchart.Series(al).AddXY(roundDouble(peakInfo[0], 1), roundDouble(peakInfo[1], 1), NULL, RGB(255,0,0));
		//m_ctrlTchart.Series(al).AddXY(roundDouble(rxvalue, 1), roundDouble(ryvalue, 1), NULL, RGB(255,0,0));
		m_ctrlTchart.Series(al).GetMarks().SetBackColor(RGB(255,255,255));
		m_ctrlTchart.Series(al).GetMarks().SetTextAlign(0);			///标记点的左侧 当前是竖直状态所以要左侧 如果是水平应该是中心
		m_ctrlTchart.Series(al).GetMarks().GetFont().SetSize(11);	////标记点的文本大小
		m_ctrlTchart.Series(al).GetMarks().GetFont().SetBold(TRUE); ///数据加粗
		m_ctrlTchart.Series(al).GetMarks().SetTransparent(TRUE);	///不加框
		m_ctrlTchart.Series(al).GetMarks().SetArrowLength(30);
		m_ctrlTchart.Series(al).GetMarks().GetArrow().SetMode(0);
		/*m_Tchart.Series(al).GetMarks().GetArrow().SetStyle(0);*/
		m_ctrlTchart.Series(al).GetMarks().GetArrow().SetColor(RGB(0,0,0));
		m_ctrlTchart.Series(al).GetMarks().GetArrow().SetStyle(0);//  手动标峰也改为实线
		m_ctrlTchart.Series(al).GetMarks().GetArrow().SetWidth(2);
		//	m_Tchart.Series(al).GetMarks().GetArrow().SetEndStyle(3);
		m_ctrlTchart.Series(al).GetMarks().GetCallout().SetArrowHead(1);////设置箭头样式
		m_ctrlTchart.Series(al).GetMarks().GetCallout().SetArrowHeadSize(6);///箭头大小
		m_ctrlTchart.Series(al).GetMarks().GetArrow().SetVisible(TRUE);
		m_ctrlTchart.Series(al).SetShowInLegend(FALSE);
		m_ctrlTchart.Series(al).GetMarks().SetStyle(9);//8显示的是X 
		m_ctrlTchart.Series(al).GetMarks().SetAngle(90);//标签旋转90度
		m_ctrlTchart.Series(al).GetMarks().SetVisible(TRUE);	
		//  并且存入m_vPeakInfo中
		m_vPeakInfo.push_back(0);
		for(int i = 0; i < 4; i++)
			m_vPeakInfo.push_back(peakInfo[i]);
		fillList();
		OnUndoZoomTchartS();//  自动缩放
	}

}


//  选择不同曲线：显示不同曲线，更新列表
void CDlgAutoMarkValue::OnCbnSelchangeComboLineChoose()
{
	// TODO: 在此添加控件通知处理程序代码
	ShowData(m_ctrlComboLineChoose.GetCurSel());
	renewList();
	twoLines = 1;// 没有预处理后的曲线，所以是1
	GetDlgItem(IDC_BTN_ASSURE_Z)->EnableWindow(TRUE);// 切换不同曲线时，确定按钮可用
	CINScanLiteDlg * pThreadApp=(CINScanLiteDlg*)AfxGetApp()->GetMainWnd();
	saveCSVName =pThreadApp->m_DlgShow.m_Tchart.Series(m_ctrlComboLineChoose.GetCurSel()).GetTitle();//  读取文件名
	nAddBaselineIndex = 0;
	nAddSGSMIndex = 0;
}



//  根据选择曲线的不同，显示不同的曲线   m_ctrlTchart 里面每次只保存一条曲线
void CDlgAutoMarkValue::ShowData( int nSerr )
{
	CINScanLiteDlg *pDlg = (CINScanLiteDlg*)AfxGetApp()->GetMainWnd();
	//  先将小图框里的曲线都清空，添加第一条曲线，将大图框里的第一条曲线显示出来
	m_vResData.clear(); //  每次都先将原始数据存入m_vResData
	m_vRawData.clear();
	if(pDlg->m_DlgShow.GetTeeCount()>0)
	{
		m_ctrlTchart.RemoveAllSeries();
		m_ctrlTchart.AddSeries(0);
		CValueList yvaluelist;
		//int serr=0;
		int ax =pDlg->m_DlgShow.m_Tchart.GetSeriesCount();
		yvaluelist = pDlg->m_DlgShow.m_Tchart.Series(nSerr).GetYValues();
		int count=pDlg->m_DlgShow.m_Tchart.Series(nSerr).GetCount();
		CString strSeriesName = pDlg->m_DlgShow.m_Tchart.Series(nSerr).GetTitle();
		m_ctrlTchart.Series(0).SetTitle(strSeriesName);
		double yxis=0;
		CValueList xvaluelist;
		xvaluelist=pDlg->m_DlgShow.m_Tchart.Series(nSerr).GetXValues();

		for(int i=0;i<count;i++)
		{
			m_ctrlTchart.Series(0).AddXY(xvaluelist.GetValue(i), yvaluelist.GetValue(i), NULL, clrSet);
			m_vResData.push_back(yvaluelist.GetValue(i));
			m_vRawData.push_back(yvaluelist.GetValue(i));
		}

	}
	//  设置自动缩放
	m_ctrlTchart.GetAxis().GetLeft().SetAutomatic(TRUE);
	m_ctrlTchart.GetAxis().GetBottom().SetAutomatic(TRUE);
	m_ctrlTchart.GetAxis().GetRight().SetAutomatic(TRUE);
	m_ctrlTchart.GetAxis().GetTop().SetAutomatic(TRUE);
}

//  更新列表
void CDlgAutoMarkValue::renewList()
{
	m_list_peakInfo.DeleteAllItems();
	while (m_list_peakInfo.DeleteColumn(0));	//清空所有表头
	CRect RECT;
	m_list_peakInfo.GetClientRect(RECT);
	// TODO:  在此添加额外的初始化

	if (sInfo.nLanguage == 0)
	{	
		//CFont font; 
		//font.CreatePointFont(120, "楷体");
		//m_list_peakInfo.SetFont(&font);
		CString strs[] = {TEXT("序号"), TEXT("峰位"), TEXT("峰强"), TEXT("半高宽"), TEXT("峰面积")};
		for(int i = 0; i < sizeof(strs)/sizeof(CString); i++)
		{
			//设置表头，第一行标题，后面的数字表示列宽
			m_list_peakInfo.InsertColumn(i, strs[i], LVCFMT_CENTER, RECT.right/(sizeof(strs)/sizeof(CString)));
		}
	}
	else
	{
		//CFont font; 
		//font.CreatePointFont(120, "Times New Roman");
		//m_list_peakInfo.SetFont(&font);
		CString strs[]  = { TEXT("Index"), TEXT("Shift"), TEXT("Intensity"), TEXT("FWHM"), TEXT("Area")};
		for(int i = 0; i < sizeof(strs)/sizeof(CString); i++)
		{
			//设置表头，第一行标题，后面的数字表示列宽
			m_list_peakInfo.InsertColumn(i, strs[i], LVCFMT_CENTER, RECT.right/(sizeof(strs)/sizeof(CString)));
		}
	}
	m_list_peakInfo.SetExtendedStyle(m_list_peakInfo.GetExtendedStyle() | LVS_EX_FULLROWSELECT |  LVS_EX_GRIDLINES | LVS_EX_DOUBLEBUFFER);
	
	//  将第一列内容居中显示
	LVCOLUMN lvc;
	lvc.mask = LVCF_FMT;
	m_list_peakInfo.GetColumn(0, &lvc);
	lvc.fmt &=~ LVCFMT_JUSTIFYMASK; 
	lvc.fmt |= LVCFMT_CENTER;
	m_list_peakInfo.SetColumn(0, &lvc);
}


void CDlgAutoMarkValue::OnCbnSelchangeComboBaselineZ()
{
	// TODO: 在此添加控件通知处理程序代码
	m_iBaselineTimes = m_combo_baseline.GetCurSel() + 1;
	GetDlgItem(IDC_BTN_ASSURE_Z)->EnableWindow(TRUE);
}


void CDlgAutoMarkValue::OnCbnSelchangeComboSgSmZ()
{
	// TODO: 在此添加控件通知处理程序代码
	m_iSGtimes = m_combo_sg_sm.GetCurSel() + 1;
	GetDlgItem(IDC_BTN_ASSURE_Z)->EnableWindow(TRUE);
}


void CDlgAutoMarkValue::OnNMReleasedcaptureSliderPeakintZ(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_ctrlComboLineChoose.GetCurSel()<0) return;
	m_dPeakInt =  ((CSliderCtrl *)GetDlgItem(IDC_SLIDER_PeakInt_Z))->GetPos();
	m_dPeakPro =  1 + (( (CSliderCtrl *)GetDlgItem(IDC_SLIDER_PeakPro_Z))->GetPos() - 1) * 0.09;
	CString P1;
	P1.Format(TEXT("%d"), m_dPeakInt);
	GetDlgItem(IDC_EDIT_PeakInt_Z)->SetWindowText(P1);
	//  如果峰强阈值发生了变化
	if (m_dPeakInt != oldPeakInt)
	{
		oldPeakInt = m_dPeakInt;
		MarksValue(m_dPeakInt , m_dPeakPro);
	}

	*pResult = 0;
}


void CDlgAutoMarkValue::OnNMReleasedcaptureSliderPeakproZ(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_ctrlComboLineChoose.GetCurSel()<0) return;
	m_dPeakInt =  ((CSliderCtrl *)GetDlgItem(IDC_SLIDER_PeakInt_Z))->GetPos();
	m_dPeakPro =  1 + (( (CSliderCtrl *)GetDlgItem(IDC_SLIDER_PeakPro_Z))->GetPos() - 1) * 0.09; //   1 ~ 9.91
	CString P1;
	P1.Format(TEXT("%.1f"), m_dPeakPro);
	GetDlgItem(IDC_EDIT_PeakPro_Z)->SetWindowText(P1);
	int peakProPos =  ((CSliderCtrl *)GetDlgItem(IDC_SLIDER_PeakPro_Z))->GetPos();
	if (peakProPos != oldPeakPro)
	{
		oldPeakPro = peakProPos;
		MarksValue(m_dPeakInt , m_dPeakPro);
	}

	*pResult = 0;
}


void CDlgAutoMarkValue::OnBnClickedCheckBaselineZ()
{
	// TODO: 在此添加控件通知处理程序代码
	m_bBaseline = ((CButton*)GetDlgItem(IDC_CHECK_BASELINE_Z))->GetCheck();
}


void CDlgAutoMarkValue::OnBnClickedCheckSgSmZ()
{
	// TODO: 在此添加控件通知处理程序代码
	m_bSGSM = ((CButton*)GetDlgItem(IDC_CHECK_SG_SM_Z))->GetCheck();
}


void CDlgAutoMarkValue::OnBnClickedBtnPreviewZ()
{
	previewLineTotal = 1;
	// TODO: 在此添加控件通知处理程序代码
	GetDlgItem(IDC_BTN_ASSURE_Z)->EnableWindow(TRUE);
	GetDlgItem(IDC_SLIDER_PeakInt_Z)->EnableWindow(FALSE);
	GetDlgItem(IDC_SLIDER_PeakPro_Z)->EnableWindow(FALSE);
	renewList();

	if(!m_bBaseline && !m_bSGSM)
		return;
	CINScanLiteDlg * pDlg=(CINScanLiteDlg*)AfxGetApp()->GetMainWnd();
	int nSelected = m_ctrlComboLineChoose.GetCurSel();//  待拟合的曲线所在的序号
	//  点击预览，先清除掉所有曲线，只保留所选曲线
	ShowData(nSelected);
	m_vPeakInfo.clear(); //  清空容器
	CSeries series = pDlg->m_DlgShow.m_Tchart.Series(nSelected);
	CValueList x_List = series.GetXValues();
	CValueList y_List = series.GetYValues();
	int nXSize = x_List.GetCount();
	double* aa = new double[nXSize];
	double *baselineData = new double[nXSize];
	double *smoothedData = new double[nXSize];
	memset(baselineData, 0.0, sizeof(double) * nXSize);
	memset(smoothedData, 0.0, sizeof(double) * nXSize);
	for (int i= 0; i<nXSize;i++)
	{
		aa[i] = y_List.GetValue(i);
	}
	//  如果点击了基线拟合
	if (m_bBaseline)
	{
		dll_baseline_correct(aa, nXSize, m_iBaselineTimes, baselineData);
		//  nAddBaselineIndex ++；
		if (nAddBaselineIndex == 0)
		{
			nAddBaselineIndex = m_ctrlTchart.AddSeries(0);
		}
		int color = m_ctrlTchart.Series(nAddBaselineIndex).GetColor();
		m_ctrlTchart.Series(nAddBaselineIndex).Clear();
		m_ctrlTchart.Series(nAddBaselineIndex).SetActive(TRUE);
		m_ctrlTchart.Series(nAddBaselineIndex).SetShowInLegend(FALSE);
		//  绘制基线
		for(int j = 0; j < nXSize;  j++)
		{
			m_ctrlTchart.Series(nAddBaselineIndex).AddXY(x_List.GetValue(j), baselineData[j], _T(""),color);
		}
		previewLineTotal++;
	}
	//  如果点击了平滑
	if(m_bSGSM)
	{
		dll_SG_Smooth(aa, nXSize, m_iSGtimes, smoothedData);
		//  nAddBaselineIndex ++；
		if (nAddSGSMIndex == 0)
		{
			nAddSGSMIndex = m_ctrlTchart.AddSeries(0);
		}
		int color = m_ctrlTchart.Series(nAddSGSMIndex).GetColor();
		m_ctrlTchart.Series(nAddSGSMIndex).Clear();
		m_ctrlTchart.Series(nAddSGSMIndex).SetActive(TRUE);
		m_ctrlTchart.Series(nAddSGSMIndex).SetShowInLegend(FALSE);
		//  绘制平滑后的曲线
		for(int j = 0; j < nXSize;  j++)
		{
			m_ctrlTchart.Series(nAddSGSMIndex).AddXY(x_List.GetValue(j), smoothedData[j], _T(""),color);
		}
		previewLineTotal++;
	}
	delete[] aa;
	delete[] baselineData;
	delete[] smoothedData;

	nAddBaselineIndex = 0;
	nAddSGSMIndex = 0;
}

//  确定
void CDlgAutoMarkValue::OnBnClickedBtnAssureZ()
{
	// TODO: 在此添加控件通知处理程序代码
	
	GetDlgItem(IDC_BTN_ASSURE_Z)->EnableWindow(FALSE);

	GetDlgItem(IDC_SLIDER_PeakInt_Z)->EnableWindow(TRUE);
	GetDlgItem(IDC_SLIDER_PeakPro_Z)->EnableWindow(TRUE);

	if(!m_bBaseline && !m_bSGSM)
		return;
	CINScanLiteDlg * pDlg=(CINScanLiteDlg*)AfxGetApp()->GetMainWnd();
	int nSelected = m_ctrlComboLineChoose.GetCurSel();//  待拟合的曲线所在的序号
	ShowData(nSelected); //  先只显示原始曲线
	m_vPeakInfo.clear(); //  清空容器
	CSeries series = pDlg->m_DlgShow.m_Tchart.Series(nSelected);
	CString strSeriesName = series.GetTitle();
	CValueList x_List = series.GetXValues();
	CValueList y_List = series.GetYValues();
	int nXSize = x_List.GetCount();
	double* aa = new double[nXSize];
	double *baselineData = new double[nXSize];
	double *resDataTemp = new double[nXSize];
	double *resData = new double[nXSize];
	memset(baselineData, 0.0, sizeof(double) * nXSize);
	for (int i= 0; i<nXSize;i++)
	{
		aa[i] = y_List.GetValue(i);
		resData[i] = aa[i];
	}
	//  如果点击了基线拟合
	if (m_bBaseline)
	{
		dll_baseline_correct(aa, nXSize, m_iBaselineTimes, baselineData);
		for(int j = 0; j < nXSize;  j++)
		{
			resDataTemp[j] = aa[j] - baselineData[j];
		}
	}
	if (m_bSGSM)
	{
		if (m_bBaseline)//  如果扣除基线和平滑
		{
			dll_SG_Smooth(resDataTemp, nXSize, m_iSGtimes, resData);
			saveCSVName.Format("%s_%s_%d_%s_%d", strSeriesName, "F", m_iBaselineTimes, "S", m_iSGtimes);
		}
		else//  如果只平滑
		{
			dll_SG_Smooth(aa, nXSize, m_iSGtimes, resData);
			saveCSVName.Format("%s_%s_%d", strSeriesName, "S", m_iSGtimes);
		}
	}
	else// 如果只扣基线
	{
		for(int j = 0; j < nXSize;  j++)
		{
			resData[j] =resDataTemp[j];
			saveCSVName.Format("%s_%s_%d", strSeriesName, "F", m_iBaselineTimes);
		}
	}
	//  只有点击了“确定”，才将m_vResData更新，将预处理之后的数据存入m_vResData
	m_vResData.clear();
	for(int j = 0; j < nXSize;  j++)
	{
		m_vResData.push_back(resData[j]);
	}

	if (nAddBaselineIndex == 0 || nAddSGSMIndex == 0)//如果在点击确定之前没有预览，则直接显示扣除基线或平滑之后的光谱数据
	{
		nAddBaselineIndex = m_ctrlTchart.AddSeries(0);
		//CString strNewSeriesName;
		//strNewSeriesName.Format("%s_%s_%d_%s_%d", strSeriesName, "F", m_iBaselineTimes, "S", m_iSGtimes);
		m_ctrlTchart.Series(nAddBaselineIndex).SetTitle(saveCSVName);
		m_ctrlTchart.Series(nAddBaselineIndex).SetColor(RGB(255,0,0));
		//int color = m_ctrlTchart.Series(nAddBaselineIndex).GetColor();
		m_ctrlTchart.Series(nAddBaselineIndex).Clear();
		m_ctrlTchart.Series(nAddBaselineIndex).SetActive(TRUE);
		for (int j = 0; j < nXSize; j++)
		{
			m_ctrlTchart.Series(nAddBaselineIndex).AddXY(x_List.GetValue(j), resData[j], _T(""), RGB(255,0,0));
		}
	}
	//  lcz  new     如果在点击确定之前点了预览
	else
	{
		m_ctrlTchart.Series(nAddBaselineIndex).Clear(); //  清除掉原先的基线
		if(nAddSGSMIndex > 0)//  如果也进行平滑过
			m_ctrlTchart.RemoveSeries(nAddSGSMIndex);
		//CString strNewSeriesName;
		//strNewSeriesName.Format("%s_%s_%d_%s_%d", strSeriesName, "F", m_iBaselineTimes, "S", m_iSGtimes);
		m_ctrlTchart.Series(nAddBaselineIndex).SetTitle(saveCSVName);
		//int color = m_ctrlTchart.Series(nAddBaselineIndex).GetColor();
		m_ctrlTchart.Series(nAddBaselineIndex).SetColor(RGB(255,0,0));
		m_ctrlTchart.Series(nAddBaselineIndex).Clear();
		for (int j = 0; j < nXSize; j++)
		{
			m_ctrlTchart.Series(nAddBaselineIndex).AddXY(x_List.GetValue(j), resData[j], _T(""), RGB(255,0,0));
		}
	}
	m_ctrlTchart.Series(nAddBaselineIndex).SetShowInLegend(TRUE);

	nAddBaselineIndex = 0; 
	nAddSGSMIndex = 0;
	twoLines = 2;
	previewLineTotal = 2;

	delete[] aa;
	delete[] baselineData;
	delete[] resData;
}

void CDlgAutoMarkValue::fillList()
{
	//  如果谱峰信息非空，则补充表格
	renewList();
	if( !m_vPeakInfo.empty())
	{
		int peakNum = m_vPeakInfo.size() / 5;
		CString temp;
		for (int i = 0; i < peakNum; i++)
		{
			temp.Format("%d", i + 1);
			m_list_peakInfo.InsertItem( i, temp);
			for(int j = 1; j <= 4; j++)
			{
				m_vPeakInfo[ 5 * i  + j] = roundDouble(m_vPeakInfo[ 5 * i  + j], 1);
				temp.Format("%.1f", m_vPeakInfo[ 5 * i  + j]);
				m_list_peakInfo.SetItemText( i, j, temp);
			}
		}
	}
}

void CDlgAutoMarkValue::OnDrawColorForMyList( NMHDR* pNmHdr, LRESULT *pResult )
{
	NMLVCUSTOMDRAW* pLVCD = reinterpret_cast<NMLVCUSTOMDRAW*>( pNmHdr ); 
	*pResult = CDRF_DODEFAULT;
	if ( CDDS_PREPAINT == pLVCD->nmcd.dwDrawStage ) 
	{
		*pResult = CDRF_NOTIFYITEMDRAW;
	}
	else if ( CDDS_ITEMPREPAINT == pLVCD->nmcd.dwDrawStage )
	{
		*pResult = CDRF_NOTIFYSUBITEMDRAW; 
	}
	else if ( (CDDS_ITEMPREPAINT | CDDS_SUBITEM) == pLVCD->nmcd.dwDrawStage ) 
	{
		//设置背景色 
		COLORREF clrNewTextColor = RGB( 0, 0, 0 );
		COLORREF clrNewBkColor = listColor;
		int nItem = static_cast<int>( pLVCD->nmcd.dwItemSpec ); 
		
		pLVCD->clrText = clrNewTextColor; 
		pLVCD->clrTextBk = clrNewBkColor; 
		*pResult = CDRF_DODEFAULT;
	}
}


//  保存表格数据到csv
void CDlgAutoMarkValue::OnBnClickedBtnOptData()
{
	// TODO: 在此添加控件通知处理程序代码
	if(saveCSVName.IsEmpty())
		return;
	//  如果没有标记的谱峰，直接返回
	if(m_ctrlTchart.GetSeriesCount() <= twoLines)
		return;
	SYSTEMTIME st;
	CString strDate, strTime;
	GetLocalTime(&st);
	strDate.Format(_T("_%4d%02d%02d"),st.wYear, st.wMonth, st.wDay);
	strTime.Format(_T("_%02d%02d%02d.csv"), st.wHour, st.wMinute, st.wSecond);

	CString resSavePath;
	resSavePath = saveCSVName + strDate + strTime;

	CFileDialog dlg(FALSE,_T("csv"), resSavePath, OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,"value(*.csv)|*.csv|value(*.xlsx)|*.xlsx||");
	CString str;
	dlg.m_ofn.lpstrTitle=str;
	DWORD osversion;
	osversion=::GetVersion();
	if(osversion<0x80000000)
		dlg.m_ofn.lStructSize=88;
	else dlg.m_ofn.lStructSize=76;

	UINT nResult=dlg.DoModal();

	if (nResult!=IDOK)	
	{
		return;
	}
	CString filename=dlg.GetPathName();
	CString fileStyle = filename.Right(filename.GetLength()-filename.ReverseFind('.')-1);
	CStdioFile file;
	CString tmp;
	if (!file.Open(filename,CFile::modeCreate|CFile::modeWrite)) 
	{
		str="Error:";
		AfxMessageBox(str,MB_ICONWARNING);
	}

	//CString strResult;
	//DWORD	dwSize=1024;

	//  写中文所需，以及头文件
	setlocale(LC_CTYPE, ("chs"));
	if (sInfo.nLanguage == 0)
	{	
		CString title;
		if(fileStyle == TEXT("csv"))
			title.Format(_T("序号,峰位,峰强,半高宽,峰面积\n"));
		else
			title.Format(_T("序号\t峰位\t峰强\t半高宽\t峰面积\n"));
		file.WriteString(title);
	}
	else
	{	
		CString title;
		if(fileStyle == TEXT("csv"))
			title.Format(_T("Index,Shift,Intensity,FWHM,Area\n"));
		else
			title.Format(_T("Index\tShift\tIntensity\tFWHM\tArea\n"));
		file.WriteString(title);
	}
	CString eachLine;//  每行的信息
	for(int i = 0; i < m_vPeakInfo.size()/5; i++)
	{
		if(fileStyle == TEXT("csv"))
			eachLine.Format(_T("%d,%.1f,%.1f,%.1f,%.1f\n"), i+1, m_vPeakInfo[ 5 * i +1], m_vPeakInfo[ 5 * i + 2], m_vPeakInfo[ 5 * i +3], m_vPeakInfo[ 5 * i + 4]);
		else
			eachLine.Format(_T("%d\t%.1f\t%.1f\t%.1f\t%.1f\n"), i+1, m_vPeakInfo[ 5 * i +1], m_vPeakInfo[ 5 * i + 2], m_vPeakInfo[ 5 * i +3], m_vPeakInfo[ 5 * i + 4]);
		file.WriteString(eachLine);
	}

	file.Close();	
	Sleep(100);
	CINScanLiteDlg * pDlg=(CINScanLiteDlg*)AfxGetApp()->GetMainWnd();
	MessageBox(pDlg->LoadStringMeau(_T("ID_FINISH")));
}

//  放大曲线
void CDlgAutoMarkValue::OnZoomTchartS()
{
	// TODO: 在此处添加消息处理程序代码
	//  放大
	showPeaks();
}

//  鼠标抬起触发事件：如果曲线都没被勾选，那么谱峰都不显示
void CDlgAutoMarkValue::OnMouseUpTchartS(long Button, long Shift, long X, long Y)
{
	// TODO: 在此处添加消息处理程序代码
	//  读取X轴范围
	showPeaks();
	//  检测是否没有曲线被勾选，如果没有曲线被勾选以显示，则谱峰也都不显示
	if (twoLines == 1)
	{
		if ( !m_ctrlTchart.Series(0).GetActive())
		{
			for (int i = twoLines; i < m_ctrlTchart.GetSeriesCount(); i++)
			{
				m_ctrlTchart.Series(i).SetActive(FALSE);
			}
		}
	}
	else
	{
		if ( !m_ctrlTchart.Series(0).GetActive() && !m_ctrlTchart.Series(1).GetActive() )
		{
			for (int i = twoLines; i < m_ctrlTchart.GetSeriesCount(); i++)
			{
				m_ctrlTchart.Series(i).SetActive(FALSE);
			}
		}
	}

}


//  放大、缩小、右键移动、滚轮移动时自动显示或不显示谱峰
void CDlgAutoMarkValue::showPeaks()
{
	CAxes axis = m_ctrlTchart.GetAxis();
	CAxis bot = axis.GetBottom();
	leftBorder = bot.GetMinimum();
	rightBorder = bot.GetMaximum();
	CAxis top = axis.GetLeft();
	bottomBorder = top.GetMinimum();
	topBorder = top.GetMaximum();

	int size=m_ctrlTchart.GetSeriesCount();

	for (int i = twoLines; i < m_ctrlTchart.GetSeriesCount(); i++)
	{
		CValueList xx;
		xx  = m_ctrlTchart.Series(i).GetXValues();
		double xv = xx.GetValue(0);

		CValueList yy;
		yy  = m_ctrlTchart.Series(i).GetYValues();
		double yv = yy.GetValue(0);
		if( xv > rightBorder || xv < leftBorder || yv > topBorder || yv < bottomBorder)
			m_ctrlTchart.Series(i).SetActive(FALSE);
		else
			m_ctrlTchart.Series(i).SetActive(TRUE);
	}
}



void CDlgAutoMarkValue::OnAllowScrollTchartS(long Axis, double* AMin, double* AMax, BOOL* AllowScroll)
{
	// TODO: 在此处添加消息处理程序代码
	showPeaks();
}


void CDlgAutoMarkValue::OnBnClickedBtnDelAllPeaks()
{
	// TODO: 在此添加控件通知处理程序代码
	OnBnClickedDelall();
}
