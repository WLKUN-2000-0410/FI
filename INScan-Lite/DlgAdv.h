#pragma once
//#include "DlgLaCorr.h"

// CDlgAdv 对话框

class CDlgAdv : public CDialog
{
	DECLARE_DYNAMIC(CDlgAdv)

public:
	CDlgAdv(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgAdv();

// 对话框数据
	enum { IDD = IDD_DLG_ADV };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CMFCButton m_BtnAbout;
	CMFCButton m_BtnLan;
	CMFCButton m_BtnOff;
	CMFCButton m_BtnSet;
	CFont m_FontChinese;
	CFont m_FontEnglish;
	//CDlgLaCorr m_DlgLaCorr;
	void SetControlFont();
	//void InitDialog();
	//void InitDialogShow(BOOL bShow);
	void UpdateLanguage();
	void SetBtnEnable(BOOL bShow);
public:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnPaint();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBnClickedButtonAbout();
	afx_msg void OnBnClickedButtonLan();
	afx_msg void OnBnClickedButtonSet();
	afx_msg void OnBnClickedButtonOff();
};
