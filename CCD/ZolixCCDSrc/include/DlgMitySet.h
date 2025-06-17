#pragma once
#include "MFCResMacros.h"
#include "..\common\resource.h"
#include "DlgImage.h"
// CDlgAndorSet 对话框
NS_MFC_Res_BEGIN
class AFX_EXT_CLASS CDlgMitySet : public CDialog
{
	DECLARE_DYNAMIC(CDlgMitySet)

public:
	CDlgMitySet(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgMitySet();

// 对话框数据
	enum { IDD = ID_DLG_M };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	void InitComboGain();
	void InitOffset();
	void InitFVB();
	void ShowBin(int nMode);
	void SetString(int x,int y);
	void SetButtomState(BOOL bSta);
	int iBinMin;
	int iBinMax;
	float fTime;
	CDlgImage dlgImage;
	BOOL m_CameraState;
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonTemp();
	afx_msg void OnBnClickedButtonBin();
	afx_msg void OnCbnSelchangeComboMode();
	afx_msg void OnBnClickedButtonAdv();
	afx_msg void OnBnClickedButtonImage();
	afx_msg void OnEnChangeEditOffset();
	afx_msg void OnBnClickedButtonStop();
	afx_msg void OnClose();
	afx_msg void OnCbnSelchangeComboGain();
	afx_msg void OnBnClickedCheckCursor();
	afx_msg void OnBnClickedButtonBmp();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CString m_StaticX;
	CString m_StaticY;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnBnClickedButtonH();

};
NS_MFC_Res_END