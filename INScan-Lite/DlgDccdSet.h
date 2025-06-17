#pragma once


// CDlgDccdSet 对话框

class CDlgDccdSet : public CDialog
{
	DECLARE_DYNAMIC(CDlgDccdSet)

public:
	CDlgDccdSet(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgDccdSet();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_DCCD };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnCbnSelchangeCombo1();
	afx_msg void OnCbnSelchangeCombo2();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnCbnSelchangeCombo3();
	afx_msg void OnClose();
};
