#include "stdafx.h"

int APIENTRY DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	UNREFERENCED_PARAMETER(lpReserved);

	switch (dwReason)
	{
	case DLL_PROCESS_ATTACH:
		{
			{
				TRACE0("ZolixCCD.DLL 正在初始化!\n");

				
				if (!AfxInitExtensionModule(ZOLIX_DLL_MFC_ResDLL, hInstance))
					return 0;



				new CDynLinkLibrary(ZOLIX_DLL_MFC_ResDLL);

			}
		}
		break;
	case DLL_THREAD_ATTACH:break;
	case DLL_THREAD_DETACH:break;
	case DLL_PROCESS_DETACH:
		{
			TRACE0("ZolixCCD.DLL 正在终止!\n");

			AfxTermExtensionModule(ZOLIX_DLL_MFC_ResDLL);
		}
	 	break;
	}
	return 1;   // 确定
}

CString  strIniName=_T("ZolixCCD.ini");
CString  sAndor=_T("A");
CString  sMity=_T("M");
CString  sOTO=_T("T");
int  nLanguage=0;
CString  g_strPath;


BOOL Split(CString content, CString pattern, CStringArray& strlist)
{
	int iPos = content.Find(pattern), iStart = 0;
	while (iPos != -1)
	{
		strlist.Add(content.Mid(iStart, iPos - iStart));
		iStart = iPos + pattern.GetLength();
		iPos = content.Find(pattern, iStart);
	}

	if (iStart <= content.GetLength())
		strlist.Add(content.Mid(iStart));
	return TRUE;
}