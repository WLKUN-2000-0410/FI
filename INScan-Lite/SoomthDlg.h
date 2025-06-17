#pragma once

// CSoomthDlg 对话框

class CSoomthDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSoomthDlg)

public:
	CSoomthDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSoomthDlg();

// 对话框数据
	enum { IDD = IDD_DLG_SOOMTH };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	void OnCbnSelchangeComboSmoothNum();
	afx_msg void OnBnClickedButtonSmoothClean();
	afx_msg void OnBnClickedButtonSmoothDefine();
	CComboBox m_ctrlSoomthLine;
	CComboBox m_ctrlSoomthNum;
	int nSmoothIndex;
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnPaint();
	CMFCButton m_ctrlBtnSmoothClean;
	CMFCButton m_ctrlBtnSmoothDefine;
	afx_msg void OnClose();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnCbnSelchangeComboSoomathLineChoose();
	void UpdateLanguage();
	void RemoveLine();
	//virtual void OnCancel();
	virtual void OnCancel();
};
