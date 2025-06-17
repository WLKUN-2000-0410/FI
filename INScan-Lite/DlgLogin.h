#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CDlgLogin 对话框

class CDlgLogin : public CDialog
{
	DECLARE_DYNAMIC(CDlgLogin)

public:
	CDlgLogin(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgLogin();

// 对话框数据
	enum { IDD = IDD_LOGIN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	void SetPro();
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();

	CProgressCtrl m_ProgressCtrl;
	CStatic m_StaticP;
};
