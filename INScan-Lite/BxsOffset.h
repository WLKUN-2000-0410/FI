#pragma once
#include <vector>
#include "afxcmn.h"
#include "afxbutton.h"
#include "..\file\TeeChart2011\tchart.h"
#include "afxwin.h"



// CBxsOffset 对话框

class CBxsOffset : public CDialogEx
{
	DECLARE_DYNAMIC(CBxsOffset)

public:
	CBxsOffset(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CBxsOffset();

// 对话框数据
	enum { IDD = IDD_BXSNIHE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	//afx_msg void OnBnClickedButton1();
	double m_a0;
	double m_a1;
	double m_a2;
	double m_a3;
	//double m_a4;
	//double m_a5;
	//  lcz new
	//CDlgLaCorr m_DlgLaCorr;

	void UpdateLanguage();
	std::vector<double> LeftData,RightData,Data;
	CTChart m_tchart;
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedOuttext();//  导出系数
	afx_msg void OnBnClickedUsethisnum();//  应用系数
	//CTChart m_tchart;
	double a0;
	double a1;
	double a2;
	double a3;
	double a4;
	double a5;
	double a6;
	double a7;
	double a8;
	double a9;
	double a10;
	double a11;
	double a12;
	double a13;
	double a14;
	double b0;
	double b1;
	double b2;
	double b3;
	double b4;
	double b5;
	double b6;
	double b7;
	double b8;
	double b9;
	double b10;
	double b11;
	double b12;
	double b13;
	double b14;
	void ShowData(int nSerr);
	void ShowHightValue();
	void AutoCorrecting();
	void Correcting(CString str,int nValue);
	VOID gauss_solve(int n,double A[],double x[],double b[]);
	VOID Poly_Fitting(int n,double x[],double y[],int poly_n,double a[]);
	VOID ZolixProcessing(SHORT Arraylength, DOUBLE* XArray, DOUBLE* YArray, SHORT Degree,DOUBLE* Fvalue);

	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBnClickedJiaozheng();
	afx_msg void OnEnChangeY1();
	afx_msg void OnPaint();
	CMFCButton m_BtnCorr;
	CMFCButton m_BtnUse;
	CMFCButton m_BtnOut;
	afx_msg void OnCbnSelchangeComboName();
	CComboBox m_ComboName;
	afx_msg void OnEnKillfocusY14();
	afx_msg void OnEnKillfocusY13();
	CMFCButton m_BtnCenWaveLenCorr;
	bool m_bLenNum;// 波长还是波数校正，true代表波长校正，false代表波数校正
	afx_msg void OnBnClickedRadioWavelen();
	afx_msg void OnBnClickedRadioWavenum();
	void importCorrPara();
	//  四个vector来存放校正参数
	vector<double> waveLen;
	vector<double> waveLenPixel;
	vector<double> waveNum;
	vector<double> waveNumPixel;
	//  计算所用到的校正系数
	afx_msg void OnBnClickedBtnLaserCorr();
	DECLARE_EVENTSINK_MAP()
	void OnUndoZoomTchart1();
	afx_msg void OnBnClickedBtnAutofill();
	CMFCButton m_BtnAutoFill;
};
