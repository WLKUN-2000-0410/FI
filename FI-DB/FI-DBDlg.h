
// FI-DBDlg.h : 头文件
//

#pragma once
#include "FrameWindowWnd.h"
#include "ShareChart.h"

// CFIDBDlg 对话框
class CFIDBDlg : public CDialogEx
{
// 构造
public:
	CFIDBDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FIDB_DIALOG };
#endif
	// 共享图表
	ShareChart m_schart;

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

private:

};
