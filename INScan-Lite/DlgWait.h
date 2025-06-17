#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "PictureEx.h"

// CDlgLogin 对话框

class CDlgWait : public CDialog
{
	DECLARE_DYNAMIC(CDlgWait)

public:
	CDlgWait(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgWait();

// 对话框数据
	enum { IDD = IDD_WAIT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();

	CPictureEx m_StaticP;
	afx_msg void OnStnClickedStaticPic();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL DestroyWindow();
};
