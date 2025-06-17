#pragma once


// CDlgAdvAndor 对话框
#include "MFCResMacros.h"
#include "..\common\resource.h"

NS_MFC_Res_BEGIN
class CDlgAdvGreateyes : public CDialog
{
	DECLARE_DYNAMIC(CDlgAdvGreateyes)

public:
	CDlgAdvGreateyes(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgAdvGreateyes();

// 对话框数据
	enum { IDD = ID_DLG_ADV };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonCcd();
	afx_msg void OnCbnSelchangeComboMirror();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
NS_MFC_Res_END