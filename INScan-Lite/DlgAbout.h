#pragma once


// CDlgAbout 对话框

class CDlgAbout : public CDialog
{
	DECLARE_DYNAMIC(CDlgAbout)

public:
	CDlgAbout(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgAbout();

// 对话框数据
	enum { IDD = IDD_ABOUT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CFont m_FontChinese;
	CFont m_FontEnglish;
	void UpdateLanguage();
	void SetControlFont();
public:
	afx_msg void OnPaint();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnStnClickedStaticAbout();

	//  lcz new 23/05/17
	CRect m_rect; //用于保存原对话框大小
	void ChangeSize(UINT nID, int x, int y);
	void resize();
	int i_dlg_width;
	int i_dlg_height;
};
