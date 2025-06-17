#pragma once


// CRoundButton
enum Scan_Model_96
{
	Auto,
	Alone
};

class CRoundButton : public CButton
{
	DECLARE_DYNAMIC(CRoundButton)

public:
	CRoundButton();
	virtual ~CRoundButton();

protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);
	virtual void PreSubclassWindow();
	void CRoundButton::SetCurrentActive(bool val, int iStatus);

	void CRoundButton::SetGroup(bool val){m_bGroup = val;Invalidate();}

	void SetDefBtnName(CString strName){m_strDefName = strName;this->SetWindowTextA(strName);}

	void SetNewBtnName(CString strName){m_strNewName = strName;}

	void ResetBtn();
	
private:  
	CBrush m_normalBrush;  
	CPen m_Pen;  
	CBrush m_activeBrush;
	CBrush m_OverBrush;
	CBrush m_GroupBrush;

	bool m_bCurrent;
	bool m_bScanOver;
	bool m_bGroup;

	CString m_strCurName;
	CString m_strDefName;
	CString m_strNewName;
	bool m_bDef;
	int m_iTimer;
public:
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void On32787();
	afx_msg void On32788();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
};


