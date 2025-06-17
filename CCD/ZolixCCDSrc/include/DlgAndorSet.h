#pragma once
#include "MFCResMacros.h"
#include "..\common\resource.h"
#include "DlgImage.h"
// CDlgAndorSet 对话框
NS_MFC_Res_BEGIN
class AFX_EXT_CLASS CDlgAndorSet : public CDialog
{
	DECLARE_DYNAMIC(CDlgAndorSet)

public:
	CDlgAndorSet(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgAndorSet();

// 对话框数据
	enum { IDD = ID_DLG_A };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	void InitComboGain();
	void InitComboSpeed();
	void InitComboPixSpeed();
	void InitTemp();
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
	afx_msg void OnEnChangeEditTime();
	afx_msg void OnBnClickedButtonStop();
	afx_msg void OnClose();
	afx_msg void OnCbnSelchangeComboGain();
	afx_msg void OnCbnSelchangeComboSpeed();
	afx_msg void OnCbnSelchangeComboCcdspeed();
	afx_msg void OnBnClickedCheckCursor();
	afx_msg void OnBnClickedButtonBmp();
	afx_msg void OnBnClickedCheckShutter();
	afx_msg void OnBnClickeButtonClose();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	CString m_StaticX;
	CString m_StaticY;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedButtonH();

};
NS_MFC_Res_END