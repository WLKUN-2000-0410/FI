#pragma once

#include "MFCResMacros.h"
#include "..\common\resource.h"
#include "DlgImage.h"
// CDlgA1Set 对话框
NS_MFC_Res_BEGIN

class AFX_EXT_CLASS CDlgA1Set : public CDialog
{
	DECLARE_DYNAMIC(CDlgA1Set)

public:
	enum { IDD = IDD_DLG_A1 };

	CDlgA1Set(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgA1Set();
	CDlgImage dlgImage;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	afx_msg LRESULT OnA1ThreadImage(WPARAM wParam, LPARAM lParam);  //消息处理函数


	DECLARE_MESSAGE_MAP()

public:
	BOOL m_CameraState;

	virtual BOOL OnInitDialog();
	void InitComboGain();   //初始化增益
	void InitSpeed();
	void InitTemp();
	afx_msg void OnSetGainButtonClicked();
	afx_msg void OnSetSpeedButtonClicked();
	afx_msg void OnSetCoolButtonClicked();
	afx_msg void OnA1ImageStart();
	afx_msg void OnBnClickedMIRR();
	afx_msg void OnBnClickedSaveImage();
	struct A1_THREAD_DATA
	{
		int width;
		int height;
		std::vector<unsigned short> imageBuffer;
		std::vector<BYTE>          byteBuffer;
	};
	afx_msg void OnBnClickedStopImage();
	CString m_StaticX;
	CString m_StaticY;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	void CDlgA1Set::SetString(int x, int y);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedCheckCursor();
	float fTime;
	afx_msg void OnBnClickedBadChange();

	int m_nGhostStart;
	int m_nGhostEnd;
	afx_msg void OnBnClickedBtnRemoveGhost();
	int m_nImageWidth = 0;
	int m_nImageHeight = 0;
	
	afx_msg void OnCbnSelchangeComboMirror();
};

NS_MFC_Res_END

