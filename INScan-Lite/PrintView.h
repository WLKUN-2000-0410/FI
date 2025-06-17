

/////////////////////////////////////////////////////////////////////////////
// CPrintView view
#pragma once
class CPrintView : public CView
{
public:
	CPrintView();           
	DECLARE_DYNCREATE(CPrintView)


public:
	BOOL	m_bPrint;

public:

	void OnMyPrint();
	void OnMyPrintPreview();
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrintPreview(CDC* pDC, CPrintInfo* pInfo, POINT point, CPreviewView* pView);
	protected:
	virtual void OnDraw(CDC* pDC);    

public:
	virtual ~CPrintView();


public:


	DECLARE_MESSAGE_MAP()
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};

/////////////////////////////////////////////////////////////////////////////

