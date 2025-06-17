#pragma once
#include "afxcmn.h"
#include "roundbutton.h"
#include <map>
using namespace std;
#define CIRCLE
// CDlgAloneParams 对话框


class CDlgAloneParams : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgAloneParams)

public:
	CDlgAloneParams(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgAloneParams();

// 对话框数据
	enum { IDD = IDD_DLG_ALONEPARAMS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnPaint();
	void SetControlFont();
	void SetFontLan(CFont *Chinese, CFont *English){m_FontChinese = Chinese; m_FontEnglish = English;}
private:
	CMFCButton m_btn_repect;
	CMFCButton m_btn_startscan;
	CMFCButton m_btn_goout;

	CFont *m_FontChinese;
	CFont *m_FontEnglish;
	void ResetCurBtn();
	int m_iBtnID;
	map<int, CRoundButton*> m_mapBtn;

	CRoundButton m_btn1 ;
	CRoundButton m_btn2 ;
	CRoundButton m_btn3 ;
	CRoundButton m_btn4 ;
	CRoundButton m_btn5 ;
	CRoundButton m_btn6 ;
	CRoundButton m_btn7 ;
	CRoundButton m_btn8 ;
	CRoundButton m_btn9 ;
	CRoundButton m_btn10;
	CRoundButton m_btn11;
	CRoundButton m_btn12;
	CRoundButton m_btn13;
	CRoundButton m_btn14;
	CRoundButton m_btn15;
	CRoundButton m_btn16;
	CRoundButton m_btn17;
	CRoundButton m_btn18;
	CRoundButton m_btn19;
	CRoundButton m_btn20;
	CRoundButton m_btn21;
	CRoundButton m_btn22;
	CRoundButton m_btn23;
	CRoundButton m_btn24;
	CRoundButton m_btn25;
	CRoundButton m_btn26;
	CRoundButton m_btn27;
	CRoundButton m_btn28;
	CRoundButton m_btn29;
	CRoundButton m_btn30;
	CRoundButton m_btn31;
	CRoundButton m_btn32;
	CRoundButton m_btn33;
	CRoundButton m_btn34;
	CRoundButton m_btn35;
	CRoundButton m_btn36;
	CRoundButton m_btn37;
	CRoundButton m_btn38;
	CRoundButton m_btn39;
	CRoundButton m_btn40;
	CRoundButton m_btn41;
	CRoundButton m_btn42;
	CRoundButton m_btn43;
	CRoundButton m_btn44;
	CRoundButton m_btn45;
	CRoundButton m_btn46;
	CRoundButton m_btn47;
	CRoundButton m_btn48;
	CRoundButton m_btn49;
	CRoundButton m_btn50;
	CRoundButton m_btn51;
	CRoundButton m_btn52;
	CRoundButton m_btn53;
	CRoundButton m_btn54;
	CRoundButton m_btn55;
	CRoundButton m_btn56;
	CRoundButton m_btn57;
	CRoundButton m_btn58;
	CRoundButton m_btn59;
	CRoundButton m_btn60;
	CRoundButton m_btn61;
	CRoundButton m_btn62;
	CRoundButton m_btn63;
	CRoundButton m_btn64;
	CRoundButton m_btn65;
	CRoundButton m_btn66;
	CRoundButton m_btn67;
	CRoundButton m_btn68;
	CRoundButton m_btn69;
	CRoundButton m_btn70;
	CRoundButton m_btn71;
	CRoundButton m_btn72;
	CRoundButton m_btn73;
	CRoundButton m_btn74;
	CRoundButton m_btn75;
	CRoundButton m_btn76;
	CRoundButton m_btn77;
	CRoundButton m_btn78;
	CRoundButton m_btn79;
	CRoundButton m_btn80;
	CRoundButton m_btn81;
	CRoundButton m_btn82;
	CRoundButton m_btn83;
	CRoundButton m_btn84;
	CRoundButton m_btn85;
	CRoundButton m_btn86;
	CRoundButton m_btn87;
	CRoundButton m_btn88;
	CRoundButton m_btn89;
	CRoundButton m_btn90;
	CRoundButton m_btn91;
	CRoundButton m_btn92;
	CRoundButton m_btn93;
	CRoundButton m_btn94;
	CRoundButton m_btn95;
	CRoundButton m_btn96;
	
	public:
	afx_msg void OnBnClickedButtonX(UINT uID);
	afx_msg void OnBnClickedButtonStartscan();
	afx_msg void OnBnClickedButtonRepect();
	afx_msg void OnBnClickedButtonGoout();
};
