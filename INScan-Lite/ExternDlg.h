#pragma once


// ExternDlg 对话框

class ExternDlg : public CDialogEx
{
	DECLARE_DYNAMIC(ExternDlg)

public:
	ExternDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~ExternDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ExternDlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
