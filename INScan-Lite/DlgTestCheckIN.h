#pragma once


// DlgTestCheckIN 对话框

class CDlgTestCheckIN : public CDialog
{
	DECLARE_DYNAMIC(CDlgTestCheckIN)

public:
	CDlgTestCheckIN(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgTestCheckIN();

// 对话框数据
	enum { IDD = IDD_DLG_TEST_IN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CFont m_FontChinese;
	CFont m_FontEnglish;
	HANDLE	m_Handle;
	void  SetControlFont();
	void  UpdateLanguage();
	void  BtnEnable(BOOL bShow);
	void  SetData();
	void  SetCheckBk(int nBk);
public:
	CMFCButton m_BtnStart;
	CMFCButton m_BtnStop;
	CMFCButton m_BtnPara;
	CMFCButton m_BtnSet;
	CMFCButton m_BtnSet2;
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnPaint();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBnClickedButtonStart();
	afx_msg void OnBnClickedButtonStop();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedButtonSet2();
	afx_msg void OnBnClickedButtonPara();
	afx_msg void OnBnClickedCheckAcc();
	afx_msg void OnBnClickedButtonSet();
	afx_msg void OnEnChangeEditTime();
	afx_msg void OnEnChangeEditAcc();
	int m_EditTime;
	int m_EditAcc;

	void AutoScan_96();
	void SetAutoScanBtnStatus(bool bStatus);
	void StartScan(){AutoScan_96();m_bStop = false;m_bAutoOver_96 = false;m_bAutoScan_96 = true;}
	bool GetStopStatus(){return m_bStop;}
	void SetAutoOver(bool var){m_bAutoOver_96 = var;}

	//static UINT IntervalTest(LPVOID pParam);
private:
	bool m_bStop;
	bool m_bAutoOver_96;
	bool m_bAutoScan_96;
};
