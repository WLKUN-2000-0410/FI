#pragma once


// CDlgLanguage 对话框

class CDlgLanguage : public CDialog
{
	DECLARE_DYNAMIC(CDlgLanguage)

public:
	CDlgLanguage(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgLanguage();

// 对话框数据
	enum { IDD = IDD_LANGEUAGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnPaint();
	afx_msg void OnBnClickedButtonTw();
	afx_msg void OnBnClickedButtonCh();
	afx_msg void OnBnClickedButtonEn();
};
