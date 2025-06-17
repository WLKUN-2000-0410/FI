#pragma once
#include "DlgDataCom.h"
#include "DlgDataFile.h"
// CDlgFile 对话框

class CDlgFile : public CDialog
{
	DECLARE_DYNAMIC(CDlgFile)

public:
	CDlgFile(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgFile();

// 对话框数据
	enum { IDD = IDD_DLG_FILE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CFont m_FontChinese;
	CFont m_FontEnglish;
	CMFCButton m_BtnOpen;
	CMFCButton m_BtnSave;
	CMFCButton m_BtnPrint;
	CMFCButton m_BtnSelect;
	CDlgDataCom m_DlgDataCom;
	CDlgDataFile m_DlgDataPro;
    void InitDialog();
	void InitDialogShow(int nID);
	void SetControlFont();
	void UpdateLanguage();

public:
	afx_msg void OnBnClickedButtonOpen();
	afx_msg void OnPaint();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	
	afx_msg void OnBnClickedButtonSave();
	afx_msg void OnBnClickedButtonPrint();
	afx_msg void OnBnClickedButtonSelect();

private:
	bool m_bSelectAll;
};
