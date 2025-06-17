#pragma once
#include "TeeChart2011\tchart.h"

// ShareChart 对话框

class ShareChart : public CDialogEx
{
	DECLARE_DYNAMIC(ShareChart)

public:
	ShareChart(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~ShareChart();

	// 当前图表
	CTChart m_tchart;

	afx_msg void OnSize(UINT nType, int cx, int cy);

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_CHART };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()



};
