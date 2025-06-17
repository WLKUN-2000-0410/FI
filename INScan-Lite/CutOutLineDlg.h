#pragma once
#include "afxbutton.h"


// CCutOutLineDlg 对话框

class CCutOutLineDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CCutOutLineDlg)

public:
	CCutOutLineDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CCutOutLineDlg();


// 对话框数据
	enum { IDD = IDD_DLG_CUTOUT_LINE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_ctrlCutOutLineChoose;
	double m_dCutOutLineEnd;
	double m_dCutOutLineStart;
	int m_nCutOutLineIndex;
	afx_msg void OnBnClickedMfcbuttonCutoutLineClean();
	afx_msg void OnBnClickedMfcbuttonCutoutLineDefine();
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	CMFCButton m_ctrlButtonCutOutLineClean;
	CMFCButton m_ctrlBtnCutOutLineDefine;
	afx_msg void OnCbnSelchangeComboCutoutLineChoose();
	afx_msg void OnClose();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	void UpdateLanguage();
	void RemoveLine();
	//virtual void OnCancel();
	virtual void OnCancel();
};
