#pragma once
#include "MiniCamera.h"

// CDlgTestCheckJ 对话框
class CINScanLiteDlg; // 只做一个前置声明，告诉编译器这个类的存在
class CDlgTestCheckJ : public CDialog
{
	DECLARE_DYNAMIC(CDlgTestCheckJ)

public:
	CDlgTestCheckJ(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgTestCheckJ();

// 对话框数据
	enum { IDD = IDD_DLG_TEST_J };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	
	DECLARE_MESSAGE_MAP()
public:

	CFont m_FontChinese;
	CFont m_FontEnglish;

	CMFCButton m_BtnL5;
	CMFCButton m_BtnL1;
	CMFCButton m_BtnR5;
	CMFCButton m_BtnR1;
	CMFCButton m_BtnSingle;
	CMFCButton m_BtnFollow;
	CMFCButton m_BtnStop;
	CMFCButton m_BtnSet;
	CMFCButton m_BtnKSet;
	CMFCButton m_BtnLED;
	void SetControlFont();
	void UpdateLanguage();
	void SetCheckBk(int nBk);
	void BtnEnable(BOOL bShow);
	void SetData();
	void SetLedButton(bool bS);
public:
	CMiniCamera *pMiniCamera;
	HANDLE m_Handle[2];
	int m_EditLaser;
	int m_EditLaserPower;
	int m_EditAcc;
	double m_EditK;	
	double m_EditTime;
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnPaint();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBnClickedButtonL5();
	afx_msg void OnBnClickedButtonL1();
	afx_msg void OnBnClickedButtonR1();
	afx_msg void OnBnClickedButtonR5();
	afx_msg void OnEnChangeEditTime();
	afx_msg void OnBnClickedButtonSet();
	afx_msg void OnEnChangeEditAcc();
	afx_msg void OnBnClickedCheckBk();
	afx_msg void OnEnChangeEditStep();
	afx_msg void OnNMReleasedcaptureSliderP(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEnChangeEditK();
	afx_msg void OnBnClickedButtonKset();
	afx_msg void OnBnClickedButtonD();
	afx_msg void OnBnClickedButtonStop();
	afx_msg void OnBnClickedButtonL();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedButtonLed();
	afx_msg void OnNMCustomdrawSliderP(NMHDR *pNMHDR, LRESULT *pResult);

	void AutoScan_96();
	void SetAutoScanBtnStatus(bool bStatus);
	void StartScan(){AutoScan_96();m_bStop = false;}
	bool GetStopStatus(){return m_bStop;}

	int WriteToLog(char* str);
private:
	bool m_bStop;
public:
	afx_msg void OnBnClickedBtnAutotest();
	afx_msg void OnBnClickedBtnStopauto();
	afx_msg void OnBnClickedBtnTestCamera();
	afx_msg void OnBnClickedBtnStopCamera();
	bool testCamera;

	void SetLaserVolt(CINScanLiteDlg* pDlg);//2025.2.25
	
	

};
