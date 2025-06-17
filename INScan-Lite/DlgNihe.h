#pragma once
#include "INScan-LiteDlg.h"
#include "afxbutton.h"

// CDlgNihe 对话框

class CDlgNihe : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgNihe)

public:
	CDlgNihe(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgNihe();
public:
	CINScanLiteDlg *m_app;
// 对话框数据
	enum { IDD = IDD_NIHE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	void UpdataEdit(void);
	afx_msg void OnBnClickedClear();
	afx_msg void OnClose();
	afx_msg void OnBnClickedSub();
	afx_msg void OnBnClickedAutofit();
	afx_msg void OnBnClickedRecover();
	virtual void OnOK();
	void UpdateLanguage();
	afx_msg void OnNMCustomdrawMicishu(NMHDR *pNMHDR, LRESULT *pResult);
	int oldvalue;
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	afx_msg void OnPaint();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	CMFCButton m_BtnSub;
	CMFCButton m_BtnAu;
	CMFCButton m_BtnRe;
	CMFCButton m_BtnCl;
};
