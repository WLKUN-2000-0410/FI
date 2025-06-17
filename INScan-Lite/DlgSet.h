#pragma once
#include "SerialClass.h"
#include "CaptureVideo.h"
//#include "d:\program files (x86)\microsoft visual studio 10.0\vc\atlmfc\include\afxbutton.h"
#include "afxbutton.h"
#include "afxwin.h"
// CDlgSet 对话框

#define FILE_PATH "D:\\FILog.txt" //信息输出文件 lcz new 01/31

class CDlgSet : public CDialog
{
	DECLARE_DYNAMIC(CDlgSet)

public:
	CDlgSet(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgSet();

// 对话框数据
	enum { IDD = IDD_SET };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	afx_msg void OnBnClickedCheckLed();
	afx_msg void OnCbnSelchangeComboLed();
	afx_msg void OnCbnDropdownComboLed();
	DECLARE_MESSAGE_MAP()
public:
	CSerialClass m_Serial;
	CSerialClass m_SerialLed;

	CFont m_FontChinese;
	CFont m_FontEnglish;
	CMFCButton m_BtnLCon;
	CMFCButton m_BtnLDis;
	CMFCButton m_BtnCDis;
	CMFCButton m_BtnCCon;
	CMFCButton m_BtnSet;
	CMFCButton m_BtnRSet;
	CMFCButton m_BtnIN;
	CMFCButton m_BtnCorr;
	CMFCButton m_BtnCCD;
	CMFCButton m_BtnUpData;
	CMFCButton m_BtnPSet;
	CMFCButton m_BtnPriorCon;
	CMFCButton m_BtnPriorDis;
	CMFCButton m_BtnPriorReset;
	CComboBox m_ComboLed;

	void UpdateLanguage();
	void SetControlFont();
	void SetData();
	void ConnectCCD();
	void OnBnState();
	void ConnectAll();
public:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnPaint();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

	int iRadio;
	afx_msg void OnBnClickedRadioH();
	afx_msg void OnBnClickedRadioL();
	afx_msg void OnCbnSelchangeComboCom();
	afx_msg void OnBnClickedCheckBg();
	afx_msg void OnBnClickedButtonRset();
	afx_msg void OnBnClickedButtonLcon();
	afx_msg void OnBnClickedButtonLdis();
	afx_msg void OnBnClickedButtonCcon();
	afx_msg void OnBnClickedButtonCdis();
	afx_msg void OnBnClickedButtonSet();
	afx_msg void OnBnClickedButtonCcd();
	afx_msg void OnBnClickedButtonUpdata();
	afx_msg void OnEnChangeEditRight();
	afx_msg void OnEnChangeEditLeft();
	afx_msg void OnEnChangeEditDelay();
	afx_msg void OnBnClickedButtonCorr();
	afx_msg void OnBnClickedButtonIn();
	afx_msg void OnBnClickedButtonPset();
	afx_msg void OnEnChangeEditM();
	afx_msg void OnEnChangeEditN();
	afx_msg void OnBnClickedButtonOffset();
	afx_msg void OnBnClickedCheckOffset();
	CMFCButton m_BtnOffset;
	CMFCButton m_BtnSetLed;
	CMFCButton m_BtnLConLed;
	CMFCButton m_BtnLDisLed;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButtonLcon2();
	afx_msg void OnBnClickedButtonLdis2();
	afx_msg void OnCbnSelchangeComboCom2();
	afx_msg void OnBnClickedCheck3();
	afx_msg void OnCbnSelchangeCombo4();
	afx_msg void OnBnClickedButtonLcon3();
	afx_msg void OnCbnSelchangeComboCom3();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedButtonLdis3();
	afx_msg void OnBnClickedButtonReset();
	afx_msg void OnBnClickedButtonOpenlaser();
	CMFCButton m_BtnOpenLaser;//开激光
	bool m_bCloseLaser;//  激光开启状态
	//int m_iReconCCD;//重连次数
	void GetComList_Reg(void);// 获取设备串口号
	map<int, CString>  m_vComInfo;
	void InitCamera();// 初始化相机
	int iUSBCameraID;// usb相机对应的索引号
	map<int, CString>  m_vCameraInfo;// 存储相机信息
	afx_msg void OnBnClickedRadioWavelenout();
	afx_msg void OnBnClickedRadioWavenumout();
	afx_msg void OnBnClickedBtnCamSet();

	int WriteToLog(char* str);// lcz new 01/31
	int WriteToLog_F(char* str);
	int LogForTest(char* str);

	//  lcz new 23/05/17
	CRect m_rect; //用于保存原对话框大小
	// 改变控件大小（需要特别注意下拉框！原因是下拉框需要更高，否则无法下拉！！！！！！！！！！！！！！！！）
	// 如果是下拉框，至少给予3倍高度
	void ChangeSize(UINT nID, int x, int y, BOOL isCom = FALSE);
	void resize();
	int i_dlg_width;
	int i_dlg_height;
	bool m_bnew_board = false;    //标识是新旧板子   new--true
	bool is_new_board() { return m_bnew_board; }
	CMFCButton m_cameraConfigBtn;
	afx_msg void OnBnClickedBtnCamSet1();
	afx_msg void OnBnClickedCheck1();
	afx_msg void OnStnClickedStaticCom2();
};
