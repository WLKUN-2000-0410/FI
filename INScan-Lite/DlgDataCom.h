#pragma once
#include "afxwin.h"


// CDlgDataCom 对话框

class CDlgDataCom : public CDialog
{
	DECLARE_DYNAMIC(CDlgDataCom)

public:
	CDlgDataCom(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgDataCom();

// 对话框数据
	enum { IDD = IDD_DLG_DATACOM };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:

	CMFCButton m_BtnFit;
	CMFCButton m_BtnPeak;
	CMFCButton m_BtnSmooth;
	CMFCButton m_CtrlButtonMath;
	CMFCButton m_BtnCom;
	CMFCButton m_ctrlButtonCutOutLine;
	CFont m_FontChinese;
	CFont m_FontEnglish;
	void SetControlFont();
	void UpdateLanguage();
	
	
public:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnPaint();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBnClickedButtonFit();
	afx_msg void OnBnClickedButtonSmooth();
	afx_msg void OnBnClickedButtonPeak();
	afx_msg void OnBnClickedButtonMath();
	
	afx_msg void OnBnClickedButtonCutOutLine();
	void closeDlgs();

	
	afx_msg void OnBnClickedButtonSql();
	//afx_msg void OnTimer(UINT_PTR nIDEvent);// lcz 05/12
	afx_msg void OnBnClickedButtonConstruct();
	CMFCButton m_constructBtn;
	

	//2024.12.25 WLK
	CMFCButton m_qualityAnalyse;
	afx_msg void OnBnClickedButtonQualityAnalyse();
};
