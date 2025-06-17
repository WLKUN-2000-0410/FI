#pragma once
#include "afxwin.h"


// CSetNameDlg 对话框

class CSetNameDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSetNameDlg)

public:
	CSetNameDlg(CWnd* pParent = NULL, CString strCurName = "");   // 标准构造函数
	virtual ~CSetNameDlg();

// 对话框数据
	enum { IDD = IDD_DLG_SET_NAME };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString strUpdateName;
	afx_msg void OnBnClickedOk();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnPaint();
	CMFCButton m_ctrlBtUpdate;
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedSet();
	void UpdateLanguage();
	afx_msg void OnCbnSelchangeCombo1();
	CComboBox m_Combo;
private:
	CString m_strCurName;
};
