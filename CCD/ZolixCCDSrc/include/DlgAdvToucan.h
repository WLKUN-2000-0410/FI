#pragma once


// CDlgAdvMity 对话框
#include "MFCResMacros.h"
#include "..\common\resource.h"

NS_MFC_Res_BEGIN
class CDlgAdvToucan : public CDialog
{
	DECLARE_DYNAMIC(CDlgAdvToucan)

public:
	CDlgAdvToucan(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgAdvToucan();

// 对话框数据
	enum { IDD = ID_DLG_ADV };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonCcd();
	afx_msg void OnCbnSelchangeComboMirror();
	afx_msg void OnClose();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
NS_MFC_Res_END