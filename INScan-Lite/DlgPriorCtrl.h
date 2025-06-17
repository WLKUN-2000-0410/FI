#pragma once
#include "afxwin.h"


// CDlgPriorCtrl 对话框
enum PriorMove
{
	NO_MOVE = 0,
	MOVE_IN,
	MOVE_OUT,
	MOVE_LEFT,
	MOVE_RIGHT,
	MOVE_UP,
	MOVE_DOWN,
	MOVE_ALL,
	MOVE_ZERO_XY
};

class CDlgPriorCtrl : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgPriorCtrl)

public:
	CDlgPriorCtrl(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgPriorCtrl();

// 对话框数据
	enum { IDD = IDD_DIALOG_PRIOR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

	void ShowCurXYZ();

private:
	CMFCButton m_btn_up;
	CMFCButton m_btn_down;
	CMFCButton m_btn_left;
	CMFCButton m_btn_right;
	CMFCButton m_btn_zero;
	CMFCButton m_btn_in;
	CMFCButton m_btn_out;
	CMFCButton m_btn_stop;
	CMFCButton m_btn_move;


	CFont *m_FontChinese;
	CFont *m_FontEnglish;

	double m_dCurPos_X;
	double m_dCurPos_Y;
	double m_dCurPos_Z;
	int m_iPrvX;
	int m_iPrvY;
	int m_iPrvZ;

	int m_iPriorMove;

	bool m_bMoveFinsh;
	bool m_bRefushBtn;
	bool m_bStop;
	bool m_bRefushEdit;
	bool m_bExit;

public:
	static UINT PriorThread(LPVOID pParam);
	void CtrlPriorFun();
	void SetBtnStatus();

	void SetControlFont();
	void SetFontLan(CFont *Chinese, CFont *English){m_FontChinese = Chinese; m_FontEnglish = English;}
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnPaint();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedButtonIn();
	afx_msg void OnBnClickedButtonOut();
	afx_msg void OnBnClickedButtonLeft();
	afx_msg void OnBnClickedButtonRight();
	afx_msg void OnBnClickedButtonZero();
	afx_msg void OnBnClickedButtonUp();
	afx_msg void OnBnClickedButtonDown();
	afx_msg void OnBnClickedButtonStop();
	afx_msg void OnBnClickedButtonMove();
	double m_fStep_XY;
	double m_fStep_Z;
	double m_fMoveX;
	double m_fMoveY;
	double m_fMoveZ;
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	CEdit m_edit_curX;
	CEdit m_edit_curY;
	CEdit m_edit_curZ;
	CEdit m_edit_MoveX;
	CEdit m_edit_MoveY;
	CEdit m_edit_MoveZ;
	afx_msg void OnClose();
};
