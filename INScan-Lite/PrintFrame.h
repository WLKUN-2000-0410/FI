
/////////////////////////////////////////////////////////////////////////////
// CPrintFrame frame


#define	WM_MY_PRINT			(WM_USER+1003)
#define	WM_BEGIN_PRINTING		(WM_USER+1004)
#define	WM_END_PRINTING			(WM_USER+1005)
#define	WM_MY_PRINT_S			(WM_USER+1006)

#define PRINTMARGIN 2
#include "PrintView.h"
class CPrintFrame : public CFrameWnd
{
	DECLARE_DYNCREATE(CPrintFrame)
public:
	CPrintFrame();           


public:
	CPrintView	*m_pView;
	CDialog		*m_pCallerDlg;
	CWnd		*m_pWnd;

public:

	protected:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);

public:
	virtual ~CPrintFrame();

	void Print();
	afx_msg void OnDestroy();
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

