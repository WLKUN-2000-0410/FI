#pragma once
#include "afxwin.h"


// CSaveFilesDlg 对话框

class CSaveFilesDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSaveFilesDlg)

public:
	CSaveFilesDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSaveFilesDlg();

// 对话框数据
	enum { IDD = IDD_DLG_SAVE_FILES };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	
	afx_msg void OnBnClickedBtChooseFilePath();
	afx_msg void OnBnClickedBtSaveFiles();
	CComboBox m_ctrlCBFileType;
	CString m_strFilePath;
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnPaint();
	CMFCButton m_ctrlBtChooseFilePath;
	CMFCButton m_ctrlBtSaveFiles;
	void UpdateLanguage();
};
