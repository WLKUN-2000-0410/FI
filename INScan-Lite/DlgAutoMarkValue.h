#pragma once
#include "..\file\TeeChart2011\tchart.h"
#include "afxwin.h"
#include "afxcmn.h"

// CDlgAutoMarkValue 对话框

class CDlgAutoMarkValue : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgAutoMarkValue)

public:
	CDlgAutoMarkValue(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgAutoMarkValue();

	// 对话框数据
	enum { IDD = IDD_AUTOMAKRVALUE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	//afx_msg void OnClose();
	virtual BOOL OnInitDialog();

	int selectSeriesSize;

	void UpdateLanguage();
	afx_msg void OnBnClickedHandone();
	afx_msg void OnBnClickedDelall();
	afx_msg void OnBnClickedDelone();
	afx_msg void OnPaint();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	CTChart m_ctrlTchart;
	CComboBox m_ctrlComboLineChoose;

	BOOL is_HandMark;
	BOOL is_DelMark;
	DECLARE_EVENTSINK_MAP()
	//  二者类似，都是通过点击图像上的点进行标峰或者删峰
	void OnClickBackgroundTchartS(long Button, long Shift, long X, long Y);
	void OnClickSeriesTchartS(long SeriesIndex, long ValueIndex, long Button, long Shift, long X, long Y);

	afx_msg void OnCbnSelchangeComboLineChoose();//  选择不同曲线
	
	CMFCButton m_BTN_preview;
	CMFCButton m_BTN_assure;
	CMFCButton m_BTN_set_t;
	CMFCButton m_BTN_opt_data;
	CMFCButton m_BTN_del_all_peaks;
	CComboBox m_combo_baseline;
	CComboBox m_combo_sg_sm;
	CListCtrl m_list_peakInfo;
	void ShowData(int nSerr);
	int m_dPeakInt; //  峰强、峰突阈值
	double m_dPeakPro;
	int m_iBaselineTimes;
	int m_iSGtimes;
	void renewList(); //  初始化表格
	void fillList(); //  填充表格
	void MarksValue(int in_width,double in_maxvalue);//  自动标峰
	void AddPointMarks(long SeriesIndex,long x,double y);//  手动标峰
	void DeleteChMarks(double xvalue); //  删除所选峰
	void DeleteAllSer(int nR);//  删除所有谱峰
	void ReMoveSeries(int param1);//  删除所标记的谱峰
	vector<double> m_vPeakInfo; //  存放谱峰信息，包括峰位、峰强、半高宽、峰面积
	afx_msg void OnCbnSelchangeComboBaselineZ();
	afx_msg void OnCbnSelchangeComboSgSmZ();
	afx_msg void OnNMReleasedcaptureSliderPeakintZ(NMHDR *pNMHDR, LRESULT *pResult);
	int oldPeakInt;
	afx_msg void OnNMReleasedcaptureSliderPeakproZ(NMHDR *pNMHDR, LRESULT *pResult);
	int oldPeakPro;
	bool m_bBaseline;
	bool m_bSGSM;
	afx_msg void OnBnClickedCheckBaselineZ();//  点击  基线拟合
	afx_msg void OnBnClickedCheckSgSmZ();//  点击  平滑
	afx_msg void OnBnClickedBtnPreviewZ();//  预览
	int nAddBaselineIndex;
	int nAddSGSMIndex;
	afx_msg void OnBnClickedBtnAssureZ();//  确定
	int twoLines;
	vector<double> m_vResData; //  最终进行处理（寻峰）的数据
	vector<double> m_vRawData; //  原始数据
	//  设置列表行的颜色
	void OnDrawColorForMyList( NMHDR* pNmHdr, LRESULT *pResult );
	double peakInfo[4];
	afx_msg void OnBnClickedBtnOptData();
	CString saveCSVName;
	int listColor;
	void OnZoomTchartS();
	void OnUndoZoomTchartS();//  自动缩放
	void OnMouseUpTchartS(long Button, long Shift, long X, long Y);
	double leftBorder, rightBorder, bottomBorder, topBorder;
	void showPeaks();
	void OnAllowScrollTchartS(long Axis, double* AMin, double* AMax, BOOL* AllowScroll);
	int previewLineTotal;
	afx_msg void OnBnClickedBtnDelAllPeaks();
	
};
