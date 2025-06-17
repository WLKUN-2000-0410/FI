#pragma once
#include "DlgTestCheckJ.h"
#include "DlgTestCheckK.h"
#include "DlgTestCheckIN.h"
#include "DlgAloneParams.h"
#include "DlgAutoParams.h"
#include "DlgPriorCtrl.h"
#include "afxbutton.h"
#include "afxwin.h"

#include "TestDlgShow.h"
// CDlgTest 对话框

class CDlgTest : public CDialog
{
	DECLARE_DYNAMIC(CDlgTest)

public:
	CDlgTest(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgTest();

// 对话框数据
	enum { IDD = IDD_DLG_TEST };

	BOOL HasRestarted() const
	{
		return m_bHasRestarted;
	}

	// 处理原因是因为会出现CCD采集出现很多0的情况
	void restartCCD()
	{
		m_bHasRestarted = TRUE;

		// 设置CCD类型
		m_ZolixCCD.InitType(sInfo.OminiType);

		m_ZolixCCD.DisConnect();
		m_ZolixCCD.Connect();
		m_ZolixCCD.SetCooler(sSInfo.Cool);
	}

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	int m_iCheck96;
	CDlgAloneParams *m_dlgAlone;
	CDlgAutoParams	*m_dlgAuto;
	CDlgPriorCtrl m_dlgPriorCtrl;
	volatile long		m_lMoveFinsh;
	int m_iScanModel;
	bool m_bStop;
	int m_iOrder;

	CTestDlgShow m_testshow;
	
	// 是否重连过
	BOOL m_bHasRestarted = FALSE;


public:
	CFont m_FontChinese;
	CFont m_FontEnglish;
	CDlgTestCheckJ m_DlgTestCheckJ;
	CDlgTestCheckIN m_DlgTestCheckK;
	void InitDialog();
	void InitDialogShow(int nID);
	void SetControlFont();
	void UpdateLanguage();
	void EnableBtn(BOOL bShow);
public:
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	CMFCButton m_BtnCkeckK;
	CMFCButton m_BtnCkeckJ;
	afx_msg void OnBnClickedButtonCheckK();
	afx_msg void OnBnClickedButtonCheckJ();
	CMFCButton m_btn_alone;
	CMFCButton m_btn_auto;
	CMFCButton m_btn_priorctrl;

	//CMFCButton m_check_96;
	afx_msg void OnBnClickedCheck96();
	afx_msg void OnBnClickedButtonAlone();
	
	afx_msg void OnBnClickedButtonAuto();
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	static UINT CtrlPriorThread(LPVOID pParam);
	void CtrlPrior();
	void ReleaseResource();
	afx_msg void OnBnClickedButtonPrior();
};
