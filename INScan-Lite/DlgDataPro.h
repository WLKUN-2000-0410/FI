#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "..\file\TeeChart2011\tchart.h"

// CDlgDataPro 对话框

class CDlgDataPro : public CDialog
{
	DECLARE_DYNAMIC(CDlgDataPro)

public:
	CDlgDataPro(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgDataPro();

// 对话框数据
	enum { IDD = IDD_DLG_DATAPRO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CMFCButton m_BtnCom;
	CMFCButton m_BtnSet;

	CFont m_FontChinese;
	CFont m_FontEnglish;
	void SetControlFont();
	void UpdateLanguage();
	void ShowList();
	void ClearList();
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CListCtrl m_ListCtrl;
	afx_msg void OnBnClickedButtonSet();
	afx_msg void OnBnClickedButtonCom();
	double m_Edit;

	afx_msg void OnEnChangeEditSet();

	afx_msg void OnNMDblclkListData(NMHDR *pNMHDR, LRESULT *pResult);

	CComboBox m_ComboName;
	CTChart m_Tchart2;

	void ShowData(int nSerr);

	afx_msg void OnCbnSelchangeCombo1();
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	void AddSqlSer(int nChoose);

	DECLARE_EVENTSINK_MAP()
	void OnUndoZoomTchart2();
};
