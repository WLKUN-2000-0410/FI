#pragma once
#include "afxwin.h"


// CDlgDataFile 对话框

class CDlgDataFile : public CDialog
{
	DECLARE_DYNAMIC(CDlgDataFile)

public:
	CDlgDataFile(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgDataFile();

// 对话框数据
	enum { IDD = IDD_DLG_DATAFILE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CMFCButton m_BtnSetName;
	CMFCButton m_BtnColor;
	CMFCButton m_BtnOne;
	CMFCButton m_BtnAll;
	CMFCButton m_BtnAuto;
	afx_msg void OnBnClickedButtonCutOutLine();
	afx_msg void OnBnClickedButtonFit();
	afx_msg void OnBnClickedButtonPeak();
	afx_msg void OnBnClickedButtonSmooth();
	afx_msg void OnBnClickedButtonMath();
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();
	CFont m_FontChinese;
	CFont m_FontEnglish;
	void SetControlFont();
	void UpdateLanguage();
	afx_msg void OnBnClickedBtnDelunsel();
	CMFCButton m_Btn_DelUnsel;
};
