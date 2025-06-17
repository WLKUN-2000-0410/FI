#include "StdAfx.h"
#include "CameraDll.h"
#include "ManageZolixIni.h"

CManageZolixIni *ManageZolixIni = NULL;
CManageZolixIni::CManageZolixIni(void)
{

}

CManageZolixIni::~CManageZolixIni(void)
{
}
//int WriteToLog(char* str)
//{
//	FILE* pfile;
//	fopen_s(&pfile, "D:\\FILog.txt", "a+");   //FILE_PATH 是个宏, 是文件的位置,如#define FILE_PATH "D:\\FILog.txt"
//
//	if (pfile == NULL)
//	{
//		return -1;
//	}
//
//	char szDateTime[20];
//	SYSTEMTIME tm;
//	GetLocalTime(&tm);
//	sprintf(szDateTime, "%02d:%02d:%02d.%03d", tm.wHour, tm.wMinute, tm.wSecond, tm.wMilliseconds);
//	fprintf_s(pfile, "%s:%s\n", szDateTime, str);
//
//	fclose(pfile);
//
//	return 0;
//}
int WriteToLog(char* str)
{

	char exePath[MAX_PATH] = { 0 };

	GetModuleFileNameA(NULL, exePath, MAX_PATH);


	char* pSlash = strrchr(exePath, '\\');
	if (pSlash)
	{
		*pSlash = '\0';
	}

	char logFilePath[MAX_PATH] = { 0 };
	sprintf_s(logFilePath, "%s\\FI-PC-Log.txt", exePath);


	FILE* pfile = nullptr;
	errno_t err = fopen_s(&pfile, logFilePath, "a+");
	if (err != 0 || pfile == nullptr)
	{
		return -1;
	}


	SYSTEMTIME tm;
	GetLocalTime(&tm);

	char szDateTime[64] = { 0 };
	sprintf_s(szDateTime, "%02d:%02d:%02d.%03d",
		tm.wHour, tm.wMinute, tm.wSecond, tm.wMilliseconds);

	fprintf_s(pfile, "%s:%s\n", szDateTime, str);

	fclose(pfile);

	return 0;
}
bool CManageZolixIni::ReadIni()
{
	CString tPath,str,str1,str2,str3,szText;
	GetModuleFileName(NULL,tPath.GetBufferSetLength (MAX_PATH+1),MAX_PATH);
	tPath.ReleaseBuffer();
	int nPos=tPath.ReverseFind ('\\');
	str1=tPath.Left (nPos);
	str3 = str1 + _T("\\")+strIniName;
	//str3 = str2 + _T("");
	g_strPath=str3;
	CString szDefault = _T("0");
	DWORD dwSize = 1000;
	char *pData =new char [dwSize];
	
	GetPrivateProfileString(sToucan,_T("offset"),szDefault,pData,dwSize,str3);
	szText = pData;
	stuToucanCCD.offset=atoi(szText.GetBuffer());

	GetPrivateProfileString(sToucan,_T("bkoffset"),szDefault,pData,dwSize,str3);
	szText = pData;
	stuToucanCCD.bkoffset=atoi(szText.GetBuffer());

	GetPrivateProfileString(sToucan,_T("iGain"),szDefault,pData,dwSize,str3);
	szText = pData;
	stuToucanCCD.iGain=atoi(szText.GetBuffer());

	GetPrivateProfileString(sToucan,_T("nID"),szDefault,pData,dwSize,str3);
	szText = pData;
	stuToucanCCD.nID=atoi(szText.GetBuffer());

	GetPrivateProfileString(sToucan,_T("iSInedx"),szDefault,pData,dwSize,str3);
	szText = pData;
	stuToucanCCD.iSInedx=atoi(szText.GetBuffer());


	GetPrivateProfileString(sToucan,_T("nTemp"),szDefault,pData,dwSize,str3);
	szText = pData;
	stuToucanCCD.nTemp=atoi(szText.GetBuffer());
	GetPrivateProfileString(sToucan,_T("isMirror"),szDefault,pData,dwSize,str3);
	szText = pData;
	stuToucanCCD.isMirror=atoi(szText.GetBuffer());
	GetPrivateProfileString(sToucan,_T("CCDNUM"),szDefault,pData,dwSize,str3);
	szText = pData;
	stuToucanCCD.iCbpNum=atoi(szText.GetBuffer());
	GetPrivateProfileString(sToucan,_T("CCD"),szDefault,pData,dwSize,str3);
	szText = pData;
	stuToucanCCD.strCbp=szText.GetBuffer();


	GetPrivateProfileString(sToucan,_T("HMirror"),szDefault,pData,dwSize,str3);
	szText = pData;
	stuToucanCCD.HMirror=atoi(szText.GetBuffer());

	GetPrivateProfileString(sToucan,_T("BinMin"),szDefault,pData,dwSize,str3);
	szText = pData;
	stuToucanCCD.BinMin=atoi(szText.GetBuffer());
	GetPrivateProfileString(sToucan,_T("BinMax"),szDefault,pData,dwSize,str3);
	szText = pData;
	stuToucanCCD.BinMax=atoi(szText.GetBuffer());

	GetPrivateProfileString(sToucan,_T("nReadMode"),szDefault,pData,dwSize,str3);
	szText = pData;
	stuToucanCCD.nReadMode=atoi(szText.GetBuffer());
	///////////////////////////////////////G/////////////////////////////////
	GetPrivateProfileString(sGreateyes,_T("nID"),szDefault,pData,dwSize,str3);
	szText = pData;
	stuGreateyesCCD.nID=atoi(szText.GetBuffer());

	GetPrivateProfileString(sGreateyes,_T("iSInedx"),szDefault,pData,dwSize,str3);
	szText = pData;
	stuGreateyesCCD.iSInedx=atoi(szText.GetBuffer());


	GetPrivateProfileString(sGreateyes,_T("nTemp"),szDefault,pData,dwSize,str3);
	szText = pData;
	stuGreateyesCCD.nTemp=atoi(szText.GetBuffer());
	GetPrivateProfileString(sGreateyes,_T("isMirror"),szDefault,pData,dwSize,str3);
	szText = pData;
	stuGreateyesCCD.isMirror=atoi(szText.GetBuffer());
	GetPrivateProfileString(sGreateyes,_T("CCDNUM"),szDefault,pData,dwSize,str3);
	szText = pData;
	stuGreateyesCCD.iCbpNum=atoi(szText.GetBuffer());
	GetPrivateProfileString(sGreateyes,_T("CCD"),szDefault,pData,dwSize,str3);
	szText = pData;
	stuGreateyesCCD.strCbp=szText.GetBuffer();


	GetPrivateProfileString(sGreateyes,_T("HMirror"),szDefault,pData,dwSize,str3);
	szText = pData;
	stuGreateyesCCD.HMirror=atoi(szText.GetBuffer());

	GetPrivateProfileString(sGreateyes,_T("BinMin"),szDefault,pData,dwSize,str3);
	szText = pData;
	stuGreateyesCCD.BinMin=atoi(szText.GetBuffer());
	GetPrivateProfileString(sGreateyes,_T("BinMax"),szDefault,pData,dwSize,str3);
	szText = pData;
	stuGreateyesCCD.BinMax=atoi(szText.GetBuffer());

	GetPrivateProfileString(sGreateyes,_T("nReadMode"),szDefault,pData,dwSize,str3);
	szText = pData;
	stuGreateyesCCD.nReadMode=atoi(szText.GetBuffer());


	//////////////////////////////////////o///////////////////////////
	GetPrivateProfileString(sOTO,_T("isMirror"),szDefault,pData,dwSize,str3);
	szText = pData;
	stuOTOCCD.isMirror=atoi(szText.GetBuffer());

	GetPrivateProfileString(sOTO,_T("iCheckBK"),szDefault,pData,dwSize,str3);
	szText = pData;
	stuOTOCCD.iCheckBK=atoi(szText.GetBuffer());
	
	GetPrivateProfileString(sOTO,_T("iCheckLine"),szDefault,pData,dwSize,str3);
	szText = pData;
	stuOTOCCD.iCheckLine=atoi(szText.GetBuffer());
	
	GetPrivateProfileString(sOTO,_T("iCheckInitensity"),szDefault,pData,dwSize,str3);
	szText = pData;
	stuOTOCCD.iCheckInitensity=atoi(szText.GetBuffer());
	
	
	GetPrivateProfileString(sOTO,_T("iCheckAbInitensity"),szDefault,pData,dwSize,str3);
	szText = pData;
	stuOTOCCD.iCheckAbInitensity=atoi(szText.GetBuffer()); 

	GetPrivateProfileString(sOTO,_T("iCheckCool"),szDefault,pData,dwSize,str3);
	szText = pData;
	stuOTOCCD.iCheckCool=atoi(szText.GetBuffer()); 

	GetPrivateProfileString(sOTO,_T("iRadioX"),szDefault,pData,dwSize,str3);
	szText = pData;
	stuOTOCCD.iRadioX=atoi(szText.GetBuffer());

	GetPrivateProfileString(sOTO,_T("iRadioMode"),szDefault,pData,dwSize,str3);
	szText = pData;
	stuOTOCCD.iRadioMode=atoi(szText.GetBuffer());

	GetPrivateProfileString(sOTO,_T("CCDNUM"),szDefault,pData,dwSize,str3);
	szText = pData;
	stuOTOCCD.iCbpNum=atoi(szText.GetBuffer());

	GetPrivateProfileString(sOTO,_T("CCD"),szDefault,pData,dwSize,str3);
	szText = pData;
	stuOTOCCD.strCbp=szText.GetBuffer();


	////////m//////////////////////////////////////////////////////////////////
	GetPrivateProfileString(sMity,_T("nReadMode"),szDefault,pData,dwSize,str3);
	szText = pData;
	stuMityCCD.nReadMode=atoi(szText.GetBuffer());
	GetPrivateProfileString(sMity,_T("nTemp"),szDefault,pData,dwSize,str3);
	szText = pData;
	stuMityCCD.nTemp=atoi(szText.GetBuffer());
	GetPrivateProfileString(sMity,_T("iGain"),szDefault,pData,dwSize,str3);
	szText = pData;
	stuMityCCD.iGain=atoi(szText.GetBuffer());
	GetPrivateProfileString(sMity,_T("iOffset"),szDefault,pData,dwSize,str3);
	szText = pData;
	stuMityCCD.iOffset=atoi(szText.GetBuffer());
 	GetPrivateProfileString(sMity,_T("isMirror"),szDefault,pData,dwSize,str3);
	szText = pData;
	stuMityCCD.isMirror=atoi(szText.GetBuffer());
	
	GetPrivateProfileString(sMity,_T("CCDNUM"),szDefault,pData,dwSize,str3);
	szText = pData;
	stuMityCCD.iCbpNum=atoi(szText.GetBuffer());
	GetPrivateProfileString(sMity,_T("CCD"),szDefault,pData,dwSize,str3);
	szText = pData;
	stuMityCCD.strCbp=szText.GetBuffer();

	GetPrivateProfileString(sMity,_T("BinMin"),szDefault,pData,dwSize,str3);
	szText = pData;
	stuMityCCD.BinMin=atoi(szText.GetBuffer());
	GetPrivateProfileString(sMity,_T("BinMax"),szDefault,pData,dwSize,str3);
	szText = pData;
	stuMityCCD.BinMax=atoi(szText.GetBuffer());

	GetPrivateProfileString(sMity,_T("HMirror"),szDefault,pData,dwSize,str3);
	szText = pData;
	stuMityCCD.HMirror=atoi(szText.GetBuffer());

	//////////////////////////a////////////////////
	GetPrivateProfileString(sAndor,_T("nReadMode"),szDefault,pData,dwSize,str3);
	szText = pData;
	stuAndorCCD.nReadMode=atoi(szText.GetBuffer());
	GetPrivateProfileString(sAndor,_T("iGain"),szDefault,pData,dwSize,str3);
	szText = pData;
	stuAndorCCD.iGain=atoi(szText.GetBuffer());
	GetPrivateProfileString(sAndor,_T("iSInedx"),szDefault,pData,dwSize,str3);
	szText = pData;
	stuAndorCCD.iSInedx=atoi(szText.GetBuffer());
	GetPrivateProfileString(sAndor,_T("iPixSInedx"),szDefault,pData,dwSize,str3);
	szText = pData;
	stuAndorCCD.iPixSInedx=atoi(szText.GetBuffer());
	GetPrivateProfileString(sAndor,_T("nTemp"),szDefault,pData,dwSize,str3);
	szText = pData;
	stuAndorCCD.nTemp=atoi(szText.GetBuffer());
	GetPrivateProfileString(sAndor,_T("isMirror"),szDefault,pData,dwSize,str3);
	szText = pData;
	stuAndorCCD.isMirror=atoi(szText.GetBuffer());
	GetPrivateProfileString(sAndor,_T("CCDNUM"),szDefault,pData,dwSize,str3);
	szText = pData;
	stuAndorCCD.iCbpNum=atoi(szText.GetBuffer());
	GetPrivateProfileString(sAndor,_T("CCD"),szDefault,pData,dwSize,str3);
	szText = pData;
	stuAndorCCD.strCbp=szText.GetBuffer();

	GetPrivateProfileString(sAndor,_T("BinMin"),szDefault,pData,dwSize,str3);
	szText = pData;
	stuAndorCCD.BinMin=atoi(szText.GetBuffer());
	GetPrivateProfileString(sAndor,_T("BinMax"),szDefault,pData,dwSize,str3);
	szText = pData;
	stuAndorCCD.BinMax=atoi(szText.GetBuffer());

	GetPrivateProfileString(sAndor,_T("HMirror"),szDefault,pData,dwSize,str3);
	szText = pData;
	stuAndorCCD.HMirror=atoi(szText.GetBuffer());

	GetPrivateProfileString(sAndor,_T("ShutterMode"),szDefault,pData,dwSize,str3);
	szText = pData;
	stuAndorCCD.ShutterMode=atoi(szText.GetBuffer());

	GetPrivateProfileString(sAndor, _T("ghostImageStrRelative"), szDefault, pData, dwSize, str3);
	szText = pData;
	stuAndorCCD.ghostImageStrRelative = szText.GetBuffer();

	GetPrivateProfileString(sAndor, _T("ghostImageStrAbsolute"), szDefault, pData, dwSize, str3);
	szText = pData;
	stuAndorCCD.ghostImageStrAbsolute = szText.GetBuffer(); 

	//2025.2.27新增A1A3相机的
	GetPrivateProfileString(sA1, _T("nTemp"), szDefault, pData, dwSize, str3);
	szText = pData;
	stuA1CCD.nTemp = atoi(szText.GetBuffer());

	GetPrivateProfileString(sA1, _T("isMirror"), szDefault, pData, dwSize, str3);
	szText = pData;
	stuA1CCD.isMirror = atoi(szText.GetBuffer());

	GetPrivateProfileString(sA1, _T("HMirror"), szDefault, pData, dwSize, str3);
	szText = pData;
	stuA1CCD.HMirror = atoi(szText.GetBuffer());

	GetPrivateProfileString(sA1, _T("CCDNUM"), szDefault, pData, dwSize, str3);
	szText = pData;
	stuA1CCD.iCbpNum = atoi(szText.GetBuffer());
	
	GetPrivateProfileString(sA1, _T("CCD"), szDefault, pData, dwSize, str3);
	szText = pData;
	stuA1CCD.strCbp = szText.GetBuffer();

	//这里是从配置文件中读增益,改为了从设备中读取增益
	/*GetPrivateProfileString(sA1, _T("iGain"), szDefault, pData, dwSize, str3);
	szText = pData;
	stuA1CCD.iGain = atoi(szText.GetBuffer());
*/

	GetPrivateProfileString(sA1, _T("ghostImageStrRelative"), szDefault, pData, dwSize, str3);
	szText = pData;
	stuA1CCD.ghostImageStrRelative = szText.GetBuffer();

	GetPrivateProfileString(sA1, _T("ghostImageStrAbsolute"), szDefault, pData, dwSize, str3);
	szText = pData;
	stuA1CCD.ghostImageStrAbsolute = szText.GetBuffer();

	delete [] pData;
	return true;
}
bool CManageZolixIni::WriteIni()
{
	CString tPath,str1,str2,str3,szText;
	GetModuleFileName(NULL,tPath.GetBufferSetLength (MAX_PATH+1),MAX_PATH);
	tPath.ReleaseBuffer();
	int nPos=tPath.ReverseFind ('\\');
	str1=tPath.Left (nPos);
	str3 = str1 + _T("\\")+strIniName;



	CString str;


	str.Format(_T("%d"),stuToucanCCD.offset);
	WritePrivateProfileString(sToucan,_T("offset"),str,str3);

	str.Format(_T("%d"),stuToucanCCD.bkoffset);
	WritePrivateProfileString(sToucan,_T("bkoffset"),str,str3);


	str.Format(_T("%d"),stuToucanCCD.iGain);
	WritePrivateProfileString(sToucan,_T("iGain"),str,str3);
	
	str.Format(_T("%d"),stuToucanCCD.nReadMode);
	WritePrivateProfileString(sToucan,_T("nReadMode"),str,str3);

	str.Format(_T("%d"),stuToucanCCD.BinMin);
	WritePrivateProfileString(sToucan,_T("BinMin"),str,str3);
	str.Format(_T("%d"),stuToucanCCD.BinMax);
	WritePrivateProfileString(sToucan,_T("BinMax"),str,str3);



	str.Format(_T("%d"),stuToucanCCD.iSInedx);
	WritePrivateProfileString(sToucan,_T("iSInedx"),str,str3);


	str.Format(_T("%d"),stuToucanCCD.nTemp);
	WritePrivateProfileString(sToucan,_T("nTemp"),str,str3);

	str.Format(_T("%d"),stuToucanCCD.isMirror);
	WritePrivateProfileString(sToucan,_T("isMirror"),str,str3);

	str.Format(_T("%d"),stuToucanCCD.HMirror);
	WritePrivateProfileString(sToucan,_T("HMirror"),str,str3);

	str.Format(_T("%d"),stuToucanCCD.iCbpNum);
	WritePrivateProfileString(sToucan,_T("CCDNUM"),str,str3);
	WritePrivateProfileString(sToucan,_T("CCD"),stuToucanCCD.strCbp,str3);
	///////////////////////////////G//////////////////////////////////
	str.Format(_T("%d"),stuGreateyesCCD.nReadMode);
	WritePrivateProfileString(sGreateyes,_T("nReadMode"),str,str3);
	
	str.Format(_T("%d"),stuGreateyesCCD.BinMin);
	WritePrivateProfileString(sGreateyes,_T("BinMin"),str,str3);
	str.Format(_T("%d"),stuGreateyesCCD.BinMax);
	WritePrivateProfileString(sGreateyes,_T("BinMax"),str,str3);



	str.Format(_T("%d"),stuGreateyesCCD.iSInedx);
	WritePrivateProfileString(sGreateyes,_T("iSInedx"),str,str3);


	str.Format(_T("%d"),stuGreateyesCCD.nTemp);
	WritePrivateProfileString(sGreateyes,_T("nTemp"),str,str3);
	
	str.Format(_T("%d"),stuGreateyesCCD.isMirror);
	WritePrivateProfileString(sGreateyes,_T("isMirror"),str,str3);

	str.Format(_T("%d"),stuGreateyesCCD.HMirror);
	WritePrivateProfileString(sGreateyes,_T("HMirror"),str,str3);

	str.Format(_T("%d"),stuGreateyesCCD.iCbpNum);
	WritePrivateProfileString(sGreateyes,_T("CCDNUM"),str,str3);
	WritePrivateProfileString(sGreateyes,_T("CCD"),stuGreateyesCCD.strCbp,str3);
	///oto//////////////////////////////////////////////////////////////////////////

	str.Format(_T("%d"),stuOTOCCD.isMirror);
	WritePrivateProfileString(sOTO,_T("isMirror"),str,str3);

	str.Format(_T("%d"),stuOTOCCD.iCheckBK);
	WritePrivateProfileString(sOTO,_T("iCheckBK"),str,str3);
	str.Format(_T("%d"),stuOTOCCD.iCheckLine);
	WritePrivateProfileString(sOTO,_T("iCheckLine"),str,str3);
	str.Format(_T("%d"),stuOTOCCD.iCheckInitensity);
	WritePrivateProfileString(sOTO,_T("iCheckInitensity"),str,str3);
	str.Format(_T("%d"),stuOTOCCD.iCheckAbInitensity);
	WritePrivateProfileString(sOTO,_T("iCheckAbInitensity"),str,str3);
	str.Format(_T("%d"),stuOTOCCD.iCheckCool);
	WritePrivateProfileString(sOTO,_T("iCheckCool"),str,str3);
	str.Format(_T("%d"),stuOTOCCD.iRadioX);
	WritePrivateProfileString(sOTO,_T("iRadioX"),str,str3);
	str.Format(_T("%d"),stuOTOCCD.iRadioMode);
	WritePrivateProfileString(sOTO,_T("iRadioMode"),str,str3);
	str.Format(_T("%d"),stuOTOCCD.iCbpNum);
	WritePrivateProfileString(sOTO,_T("CCDNUM"),str,str3);
	WritePrivateProfileString(sOTO,_T("CCD"),stuOTOCCD.strCbp,str3);

	
	//m////////////////////////////////////

	str.Format(_T("%d"),stuMityCCD.nReadMode);
	WritePrivateProfileString(sMity,_T("nReadMode"),str,str3);
	str.Format(_T("%d"),stuMityCCD.iGain);
	WritePrivateProfileString(sMity,_T("iGain"),str,str3);
	str.Format(_T("%d"),stuMityCCD.iOffset);
	WritePrivateProfileString(sMity,_T("iOffset"),str,str3);
	str.Format(_T("%d"),stuMityCCD.nTemp);
	WritePrivateProfileString(sMity,_T("nTemp"),str,str3);
	str.Format(_T("%d"),stuMityCCD.BinMin);
	WritePrivateProfileString(sMity,_T("BinMin"),str,str3);
	str.Format(_T("%d"),stuMityCCD.BinMax);
	WritePrivateProfileString(sMity,_T("BinMax"),str,str3);
	str.Format(_T("%d"),stuMityCCD.isMirror,str3);
	WritePrivateProfileString(sMity,_T("isMirror"),str,str3);
	str.Format(_T("%d"),stuMityCCD.iCbpNum);
	WritePrivateProfileString(sMity,_T("CCDNUM"),str,str3);
	str.Format(_T("%d"),stuMityCCD.HMirror);
	WritePrivateProfileString(sMity,_T("HMirror"),str,str3);	
	WritePrivateProfileString(sMity,_T("CCD"),stuMityCCD.strCbp,str3);
	//////////////////////////////////////////////////////////////////////////
	str.Format(_T("%d"),stuAndorCCD.nReadMode);
	WritePrivateProfileString(sAndor,_T("nReadMode"),str,str3);
	str.Format(_T("%d"),stuAndorCCD.iGain);
	WritePrivateProfileString(sAndor,_T("iGain"),str,str3);
	str.Format(_T("%d"),stuAndorCCD.iSInedx);
	WritePrivateProfileString(sAndor,_T("iSInedx"),str,str3);
	str.Format(_T("%d"),stuAndorCCD.iPixSInedx);
	WritePrivateProfileString(sAndor,_T("iPixSInedx"),str,str3);
	str.Format(_T("%d"),stuAndorCCD.nTemp);
	WritePrivateProfileString(sAndor,_T("nTemp"),str,str3);
	str.Format(_T("%d"),stuAndorCCD.BinMin);
	WritePrivateProfileString(sAndor,_T("BinMin"),str,str3);
	str.Format(_T("%d"),stuAndorCCD.BinMax);
	WritePrivateProfileString(sAndor,_T("BinMax"),str,str3);
	str.Format(_T("%d"),stuAndorCCD.isMirror);
	WritePrivateProfileString(sAndor,_T("isMirror"),str,str3);
	
	str.Format(_T("%d"),stuAndorCCD.HMirror);
	WritePrivateProfileString(sAndor,_T("HMirror"),str,str3);

	str.Format(_T("%d"),stuAndorCCD.ShutterMode);
	WritePrivateProfileString(sAndor,_T("ShutterMode"),str,str3);
	
	str.Format(_T("%d"),stuAndorCCD.iCbpNum);
	WritePrivateProfileString(sAndor,_T("CCDNUM"),str,str3);
	WritePrivateProfileString(sAndor,_T("CCD"),stuAndorCCD.strCbp,str3);

	WritePrivateProfileString(sAndor, _T("ghostImageStrRelative"), stuAndorCCD.ghostImageStrRelative, str3);
	WritePrivateProfileString(sAndor, _T("ghostImageStrAbsolute"), stuAndorCCD.ghostImageStrAbsolute, str3);



	//=====A1 A3=====
	str.Format(_T("%d"), stuA1CCD.iCbpNum);
	WritePrivateProfileString(sA1, _T("CCDNUM"), str, str3);
	WritePrivateProfileString(sA1, _T("CCD"), stuA1CCD.strCbp, str3);

	str.Format(_T("%d"), stuA1CCD.nTemp, str, str3);
	WritePrivateProfileString(sA1, _T("nTemp"), str, str3);

	str.Format(_T("%d"), stuA1CCD.isMirror, str, str3);
	WritePrivateProfileString(sA1, _T("isMirror"), str, str3);

	str.Format(_T("%d"), stuA1CCD.HMirror, str, str3);
	WritePrivateProfileString(sA1, _T("HMirror"), str, str3);


	WritePrivateProfileString(sA1, _T("ghostImageStrRelative"), stuA1CCD.ghostImageStrRelative, str3);
	WritePrivateProfileString(sA1, _T("ghostImageStrAbsolute"), stuA1CCD.ghostImageStrAbsolute, str3);


	return true;
}