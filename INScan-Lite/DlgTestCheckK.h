#pragma once


// CDlgTestCheckK 对话框

class CDlgTestCheckK : public CDialog
{
	DECLARE_DYNAMIC(CDlgTestCheckK)

public:
	CDlgTestCheckK(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgTestCheckK();

// 对话框数据
	enum { IDD = IDD_DLG_TEST_K };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CFont m_FontChinese;
	CFont m_FontEnglish;
	HANDLE	m_Handle;
	void  SetControlFont();
	void  UpdateLanguage();
	void  BtnEnable(BOOL bShow);
	void  SetData();
public:
	CMFCButton m_BtnStart;
	CMFCButton m_BtnStop;
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnPaint();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	int m_RadioL;
	afx_msg void OnBnClickedRadio1();
	afx_msg void OnBnClickedRadio2();
	afx_msg void OnBnClickedRadio3();
	afx_msg void OnBnClickedButtonStart();
	afx_msg void OnBnClickedButtonStop();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
