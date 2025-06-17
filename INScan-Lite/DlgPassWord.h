#pragma once
#include "afxbutton.h"


// CDlgPassWord 对话框

class CDlgPassWord : public CDialog
{
	DECLARE_DYNAMIC(CDlgPassWord)

public:
	CDlgPassWord(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgPassWord();

// 对话框数据
	enum { IDD = IDD_DLG_PASSWORD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	void UpdateLanguage();
	afx_msg void OnBnClickedButtonInput();
	CMFCButton m_BtnPss;
	afx_msg void OnPaint();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//virtual void OnOK();
};
