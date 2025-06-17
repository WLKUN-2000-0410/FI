#pragma once
//#include "d:\program files (x86)\microsoft visual studio 10.0\vc\atlmfc\include\afxwin.h"
//#include "d:\program files (x86)\microsoft visual studio 10.0\vc\atlmfc\include\afxwin.h"
//#include "d:\program files (x86)\microsoft visual studio 10.0\vc\atlmfc\include\afxwin.h"
#include "afxwin.h"

// CFileInfo 对话框

class CFileInfo : public CDialog
{
	DECLARE_DYNAMIC(CFileInfo);

public:
	CFileInfo(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CFileInfo();

// 对话框数据
	enum { IDD = IDD_DLG_FILEINFO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	CMFCButton m_BtnPath;
	CMFCButton m_BtnSet;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonSet();
	afx_msg void OnBnClickedButtonPath();
	afx_msg void OnEnChangeEditHead();
	afx_msg void OnBnClickedButtonSave();
	CMFCButton M_ButtonNfoSave;
	afx_msg void OnBnClickedButtonNfoSave();
	BOOL m_bCheckSmooth;
	BOOL m_bCheckFit;
	BOOL m_bCheckCutOut;
	CEdit m_ctrlNumStart;
	CEdit m_ctrlNumEnd;
	double m_dNumStart;
	double m_dNumEnd;
	afx_msg void OnBnClickedCheckCutOut();
	CComboBox m_ctrlComboFileType;
	int m_nSmoothNum;
	int m_nFitNum;
	afx_msg void OnBnClickedCheckSmooth();
	afx_msg void OnBnClickedCheckFit();
	CEdit m_ctrlEditSoomthNum;
	CEdit m_ctrlFitEditNum;
	void UpdateLanguage();
	afx_msg void OnBnClickedCheckHeader();
	bool m_bCheckHeader;
};
