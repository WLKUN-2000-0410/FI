
// INScan-LiteDlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#include "afxbutton.h"
#include "DlgTest.h"
#include "DlgAdv.h"
#include "DlgFile.h"
#include "DlgShow.h"
#include "DlgAbout.h"
#include "DlgSet.h"
#include "DlgLanguage.h"
#include "zlanguagectrl1.h"
#include "DlgLogin.h"
#include "DlgWait.h"
#include "zolixcurveprocesctrl1.h"
#include "DenoiseDlg.h"
#include "DlgAutoMarkValue.h"
#include "FitSmoothDlg.h"
#include "SoomthDlg.h"
#include "CutOutLineDlg.h"
//
//  lcz new 23/05/11  增加1064的头文件
//#include "..\ZolixCCDSrc\include\dfield.h"
#include "..\CCD\ZolixCCDSrc\include\dfield.h"

//  lcz new 23/02/02  使用openCV
#include "opencv2/opencv.hpp"
//#include "CvvImage.h"
#include "cv.h"
#include "cxcore.h"
#include "highgui.h"
#include "opencv2/highgui/highgui.hpp"  
#include "opencv2/imgproc/imgproc.hpp"


#define WM_UPDATE_TEMP (WM_USER + 101)



//using namespace std;
using namespace cv;




// CINScanLiteDlg 对话框
class CINScanLiteDlg : public CDialogEx
{
// 构造
public:
	CINScanLiteDlg(CWnd* pParent = NULL);	// 标准构造函数
	virtual ~CINScanLiteDlg();
// 对话框数据
	enum { IDD = IDD_INSCANLITE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持



// 实现
protected:
	HICON m_hIcon;
	bool m_bMove;
	CString m_strTemp_A1;
	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();

	afx_msg LRESULT OnMsgUpdateLineName(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMsgShowLine(WPARAM wParam, LPARAM lParam);
	
	DECLARE_MESSAGE_MAP()
public:
	CZlanguagectrl1 m_ZLanguage;

	CFont m_FontChinese;
	CFont m_FontEnglish;
	CBrush brush;
	CDlgAdv m_DlgAdv;
	CDlgFile m_DlgFile;
	CDlgTest m_DlgTest;
	CDlgAbout m_DlgAbout;
	CDlgSet m_DlgSet;
	CDlgShow m_DlgShow;
	CDlgLanguage m_DlgLanguage;
	CDlgLogin *pDlgLogin;
	CDlgWait *pDlgWait;
	CFitSmoothDlg* pFitSmoothDlg;
	CCutOutLineDlg* pCutLineDlg;
	CSoomthDlg* pSmoothDlg;
	CDenoiseDlg* pDenoiseDlg;
	//CDlgAutoMarkValue pAotumarkvalue ;// lcz new 05/16

    //CDialogQualityAnalyse* pQualityAnalyseDlg; //WLK 

	void InitDialog();
	void InitDialogShow(int nID);
	void SetControlFont();
	void SetAdvShow(int nID);
	void InitLan();
	void InitDeviceInfo();
	void InitLanguage(CString strLang);
	void UpdateLanguage();
	void SetBtnS();
	void SetBtnEnable(BOOL bShow);
	CString GetIniInfo(CString strT,CString strI);
	DWORD ClockTreadProc(BOOL bState,int nNum);
	LRESULT OnDlgLogin(WPARAM wParam,LPARAM lParam);
    CString LoadStringMeau(CString str);
	void WriteIniInfo(CString strT,CString strInfo,CString strPath);
	void WriteDeviceInfo();
public:
	CString strTemp;
	CMFCButton m_ButtonTest;
	CMFCButton m_ButtonFile;
	CMFCButton m_ButtonData;
	CMFCButton m_ButtonAdv;
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBnClickedButtonTest();
	afx_msg void OnBnClickedButtonFile();
	afx_msg void OnBnClickedButtonData();
	afx_msg void OnBnClickedButtonAdv();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual void OnCancel();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CZolixcurveprocesctrl1 m_polyfit;
	afx_msg void OnNcLButtonDown(UINT nHitTest, CPoint point);
	afx_msg void OnUpdateName();
	void OnSetColor(int nNum);
	void OnDel(int nSer);
	afx_msg void OnDelAll();
	afx_msg void OnAutoZoom();
	afx_msg void OnDestroy();
	void ShowMouseXY(double fX = 0.0, double fY = 0.0);
	CStatic m_static_ccd;
	CString m_strTempCCD;

	static int m_iTemp;
	bool g_bClose;

	VideoCapture cap;// lcz new 02/06

	char _handle = -1;//  lcz new 23/05/11  1064探测器的模拟句柄
	string _1064sn;//  lcz new 23/05/11  1064探测器的序列号
	//char* _1064c_sn;
	bool m_b1064;//  lcz new 23/05/11  true表示是1064，false表示不是
	BOOL _cooling;//  lcz new 23/05/12  1064是否在制冷
	CStatic m_groupBox;//  lcz new 23/05/16  给左侧栏添加变量
	CStatic m_bitGreen;//  lcz new 23/05/16  给控件添加变量以设置其位置和大小
	CStatic m_bitGray;
	CStatic m_staticXY;
	int i_Set_About_Page_Width;//  设置和关于界面的宽度
	int i_Set_About_Page_Height;//  设置和关于界面的高度

	CWinThread *m_getTemperThread = NULL;
	int WriteToLog(char* str);

};
