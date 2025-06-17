#pragma once


// CDlgLaCorr 对话框

class CDlgLaCorr : public CDialog
{
	DECLARE_DYNAMIC(CDlgLaCorr)

public:
	CDlgLaCorr(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgLaCorr();

// 对话框数据
	enum { IDD = IDD_DLG_LACORR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CFont m_FontChinese;
	CFont m_FontEnglish;
	
	CMFCButton m_BtnSet;
	void SetData();
	void SetControlFont();
	void UpdateLanguage();
	void SetButtonS(BOOL bRet);
	void SetBtnLaEnable(BOOL bRet);
public:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnPaint();
	afx_msg void OnBnClickedButtonSet();
	afx_msg void OnEnChangeEditObj();
};
