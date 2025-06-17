#include "Language.h"

CLanguage::CLanguage(CWnd* pParent)
{
	Init();
}

CLanguage::~CLanguage()
{
}
void CLanguage::Init()
{
	szDig[0]=_T("Dialog_CH");
	szDig[1]=_T("Dialog_EH");
	szDig[2]=_T("Dialog_CH_F");
}
void CLanguage ::UpdateLanguage(CWnd *pDlg,int nIDD,CString szSection,CString strPath)
{

	DWORD dwSize = 1000;
	CString szKey,szDefault=_T("");
	TCHAR *pData =new TCHAR [dwSize];
	CString kk;
	//读对话框标题
	szKey.Format(_T("IDD%d_Title"),nIDD);
	if(GetPrivateProfileString(szSection,szKey,szDefault,
		pData,dwSize,strPath) != 0)
	{
		pDlg->SetWindowText(pData);
	}

	//写入各个子控件的标题文字
	CWnd* pWnd = pDlg->GetWindow(GW_CHILD);
	while(pWnd != NULL)
	{
		szKey.Format(_T("IDD%d_%d"),nIDD,pWnd->GetDlgCtrlID());
		if(GetPrivateProfileString(szSection,szKey,szDefault,
			pData,dwSize,strPath) != 0)
		{
			pWnd->SetWindowText(pData);
		}

		pWnd = pWnd->GetWindow(GW_HWNDNEXT);
	}
	g_strPath= strPath;
	delete [] pData;
}
CString CLanguage ::ReadID(CString strkey,CString szSection)
{
	CString strData=_T("");
	GetPrivateProfileString(szSection,strkey,"",strData.GetBuffer(1024),1024,g_strPath);
	return strData;		
}
