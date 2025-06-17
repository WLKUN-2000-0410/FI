#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// CFitSmoothDlg 对话框

class CFitSmoothDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CFitSmoothDlg)

public:
	CFitSmoothDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CFitSmoothDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_FIT_SMOOTH };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_ctrlComboFitLineC;
	virtual BOOL OnInitDialog();
	int nFitIndex;
	CComboBox m_ctrlComboFitNum;
	void OnCbnSelchangeComboFitNum();
	afx_msg void OnBnClickedButtonFitClean();
	afx_msg void OnBnClickedButtonFitDefine();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnPaint();
	CMFCButton m_ctrlBtnFitClean;
	CMFCButton m_ctrlBtnFitDefine;
	afx_msg void OnCbnSelchangeComboFitLineChoose();
	afx_msg void OnDestroy();
	afx_msg void OnClose();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	void UpdateLanguage();
	void RemoveLine();
	//bool m_bPreScreen;//  是否预览
	//virtual void OnCancel();
	virtual void OnCancel();
};
