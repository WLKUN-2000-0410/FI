#pragma once
#include "afxcmn.h"


// OpencvCameraConfig 对话框

class OpencvCameraConfig : public CDialogEx
{
	DECLARE_DYNAMIC(OpencvCameraConfig)

public:
	OpencvCameraConfig(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~OpencvCameraConfig();

	afx_msg BOOL OnInitDialog();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG6 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CSliderCtrl m_left;
	CSliderCtrl m_top;
	CSliderCtrl m_right;
	CSliderCtrl m_bottom;
	afx_msg void OnNMCustomdrawSliderLeft(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButton1();
	afx_msg void OnNMReleasedcaptureSliderP(NMHDR *pNMHDR, LRESULT *pResult);

	// 从配置ini文件当中刷新配置
	// 刷新滚动条
	// 刷新配置文件
	void refreshParamFromIni();
};
