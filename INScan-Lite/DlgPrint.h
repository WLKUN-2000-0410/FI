#pragma once


// CDlgPrint 对话框

class CDlgPrint : public CDialog
{
	DECLARE_DYNAMIC(CDlgPrint)

public:
	CDlgPrint(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgPrint();

// 对话框数据
	enum { IDD = IDD_DLG_PRINT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CMFCButton m_BtnPrint;
	CMFCButton m_BtnPrintV;
	int m_EditP;
	double m_EditT;
	int m_EditA;
	BOOL bPrint;
	void ReadData();
	void UpdateLanguage();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	afx_msg void OnPaint();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBnClickedButtonPrint();
	afx_msg void OnBnClickedButtonPrintview();
	LRESULT OnBeginPrinting(WPARAM wParam,LPARAM lParam);
	LRESULT OnEndPrinting(WPARAM wParam,LPARAM lParam);
	LRESULT OnMyPrint(WPARAM wParam,LPARAM lParam);
	LRESULT OnMyPrintS(WPARAM wParam,LPARAM lParam);
	void PrintPageHeader(CDC* pDC, UINT nPageNumber, CPrintInfo* pInfo);//打印页眉;
	CFont		m_ListFont;
	CFont		m_fontPrinter;
	UINT m_cxWidth;
	UINT m_cxOffset;
	UINT m_nLinesPerPage;
	UINT m_cyPrinter;

};
