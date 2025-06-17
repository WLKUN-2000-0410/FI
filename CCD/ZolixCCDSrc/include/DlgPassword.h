#pragma once


// CDlgPassword 对话框
#include "MFCResMacros.h"
#include "..\common\resource.h"

NS_MFC_Res_BEGIN
class CDlgPassword : public CDialog
{
	DECLARE_DYNAMIC(CDlgPassword)

public:
	CDlgPassword(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgPassword();

// 对话框数据
	enum { IDD = ID_DLG_PASSWORD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonCcd();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
NS_MFC_Res_END