#pragma once


// CDeleteOne 对话框

class CDeleteOne : public CDialog
{
	DECLARE_DYNAMIC(CDeleteOne)

public:
	CDeleteOne(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDeleteOne();

// 对话框数据
	enum { IDD = IDD_DLG_SET_COLOR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedSet();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnPaint();
	CMFCButton m_ctrlBtUpdate;
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	void UpdateLanguage();
	afx_msg void OnCbnSelchangeCombo1();
	CComboBox m_Combo;
};
