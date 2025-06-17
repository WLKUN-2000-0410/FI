#pragma once
#include "MFCResMacros.h"
#include "..\common\resource.h"

// CDlgAndorSet 对话框
NS_MFC_Res_BEGIN
class AFX_EXT_CLASS CDlgOTOSet : public CDialog
{
	DECLARE_DYNAMIC(CDlgOTOSet)

public:
	CDlgOTOSet(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgOTOSet();

// 对话框数据
	enum { IDD = ID_DLG_T};

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	int m_RadioX;
	int m_RadioMode;

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonAdv();
	afx_msg void OnClose();
	afx_msg void OnBnClickedCheckBk();
	afx_msg void OnBnClickedCheckLine();
	afx_msg void OnBnClickedCheckIntensity();
	afx_msg void OnBnClickedCheckAbintensity();
	afx_msg void OnBnClickedCheckCool();
	afx_msg void OnBnClickedRadioZolix();
	afx_msg void OnBnClickedRadioOriginal();
	afx_msg void OnBnClickedRadioRaw();
	afx_msg void OnBnClickedRadioCorr();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
public:
	void ShowMode(int iMode);
};
NS_MFC_Res_END