#pragma once
#include "afxwin.h"
#include "afxdialogex.h"
//#include "lines\BaselineCorrect.h"
//#include "lines\LineMath.h"
//  lcz  new
//#include "lines\zolix_algorithm.h"
#include "lines\zolix_cal_fit.h"
#include "afxcmn.h"
// CDenoiseDlg 对话框

class CDenoiseDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CDenoiseDlg)

public:
	CDenoiseDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDenoiseDlg();

// 对话框数据
	enum { IDD = IDD_DLG_DENOISE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	// 生成的消息映射函数
	
	DECLARE_MESSAGE_MAP()
public:
	int nDenLineNum;
	double nDenStart;
	double nDenEnd;
	int nStartLineNumber;
	int nEndLineNumber;
	//afx_msg void OnBnClickedButtonDenAre();
	afx_msg void OnBnClickedButtonDenHight();
	void addSeries(double nDenStart, double nDenEnd, double dMinNum , double dMaxNum);
	CComboBox pCtrlDenLines;
	virtual BOOL OnInitDialog();
	double m_dNumArea;
	double m_dNumHeight;
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnPaint();
	//CMFCButton m_ctrlBtnDenAre;
	CMFCButton m_ctrlBtnDenHight;
	CMFCButton m_ctrlBtnCptNe;
	afx_msg void OnCbnSelchangeComboDenLine();
	afx_msg void OnBnClickedButtonLookMintomax();
	CMFCButton m_ctrlButtonLook;
	afx_msg void OnClose();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	void UpdateLanguage();
	void RemoveLine();
	vector<double> yTemp;
	CListCtrl m_ListNe;
	void renewList();
	vector<double> m_vPeaks; //  存放峰位
	CString m_dMaxFwhm;
	afx_msg void OnBnClickedBtnNe();
	//virtual void OnCancel();
	virtual void OnCancel();
};
