
#pragma once

#include "stdafx.h"


class CLanguage : public CObject  
{
protected:
	//CString g_strPath;
public:
	CLanguage(CWnd* pParent=NULL);
	virtual~ CLanguage();
	void UpdateLanguage(CWnd *pDlg,int nIDD,CString szSection,CString strPath);
    CString szDig[3];
	void Init();
	CString ReadID(CString strkey,CString szSection);


};
