#pragma once


// CTestDlgShow 对话框

class CTestDlgShow : public CDialogEx
{
	DECLARE_DYNAMIC(CTestDlgShow)

public:
	CTestDlgShow(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CTestDlgShow();

// 对话框数据
	enum { IDD = IDD_DIALOG4 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
