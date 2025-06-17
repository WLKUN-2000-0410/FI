#pragma once


// CDlgAdvAndor 对话框
#include "MFCResMacros.h"
#include "..\common\resource.h"

NS_MFC_Res_BEGIN
class CDlgAdvAndor : public CDialog
{
	DECLARE_DYNAMIC(CDlgAdvAndor)

public:
	CDlgAdvAndor(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgAdvAndor();

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
	
	afx_msg void OnBnClickedButtonGhost();
};
NS_MFC_Res_END