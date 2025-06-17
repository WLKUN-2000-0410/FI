
// INScanLiteConfigDlg.h : 头文件
//

#pragma once
#include "afxwin.h"


// CINScanLiteConfigDlg 对话框
class CINScanLiteConfigDlg : public CDialogEx
{
// 构造
public:
	CINScanLiteConfigDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_INSCANLITECONFIG_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonSave();
	// 软件属性
	CComboBox m_softAttr;
	// 摄像头显示方式
	CComboBox m_showMethod;
	// 是否显示摄像头
	CComboBox m_isShowCamera;
	CEdit m_left;
	CEdit m_top;
	CEdit m_right;
	CEdit m_bottom;
	CEdit m_hoffset;
	CEdit m_voffset;
	CEdit m_w;
	CEdit m_h;
	afx_msg void OnCbnSelchangeComboShowMethod();
};
