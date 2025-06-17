
// TestZolixDLLDlg.h : 头文件
//

#pragma once
#include "MFCResGInclude.h"
#include "ZolixCCD.h"
#include"file\TeeChart2011\tchart.h"
#include"file\TeeChart2011\series.h"
// CTestZolixDLLDlg 对话框
using namespace  ZOLIX_DLL_MFC_Res;

class CTestZolixDLLDlg : public CDialogEx
{
// 构造
public:
	CTestZolixDLLDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_TESTZOLIXDLL_DIALOG };

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
	CZolixCCD testBase1;
	CZolixCCD testBase2;
	CZolixCCD testBase3;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButton1();
	CString strEdit;
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton5();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedButton6();
	afx_msg void OnBnClickedButton7();
	afx_msg void OnBnClickedButton8();
	afx_msg void OnBnClickedButton9();
	afx_msg void OnBnClickedButton10();
	afx_msg void OnBnClickedButton11();
	afx_msg void OnBnClickedButton12();

	CTChart m_Tchar;
	afx_msg void OnBnClickedButton13();
};
