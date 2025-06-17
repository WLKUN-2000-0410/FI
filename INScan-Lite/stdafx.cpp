
// stdafx.cpp : 只包括标准包含文件的源文件
// INScan-Lite.pch 将作为预编译头
// stdafx.obj 将包含预编译类型信息

#include "stdafx.h"

#include <vector>


void  ReadOemMaxMin()
{
	CStringArray sa;
	CString tPath,str,strDuty;
	GetModuleFileName(NULL,tPath.GetBufferSetLength (MAX_PATH+1),MAX_PATH);
	tPath.ReleaseBuffer();
	int nPos=tPath.ReverseFind ('\\');
	str=tPath.Left (nPos);
	strDuty = str + _T("\\powerToDuty.txt");

	CStdioFile file;
	if (!file.Open(strDuty,CFile::modeRead|CFile::typeText)) 
	{
		return ;
	}  	
	file.ReadString(str);
	file.ReadString(str);
	str=str.Right(str.GetLength()-8);
	Split(str,"\t",sa);

	sSInfo.LASERPOWERMIN=atof(sa.GetAt(0));
	for (int i=0;i<sa.GetSize();i++)
	{
		if (atof(sa.GetAt(i))==0)
		{
			sa.RemoveAt(i);
			i--;
		}
	}
	sSInfo.LASERPOWERMAX=atof(sa.GetAt(sa.GetSize()-1));

}

BOOL LoadFileData(CString strFathName,int *Data,double *x,double* y)
{

	CStdioFile fp;
	int isOpen=fp.Open(strFathName,CFile::modeRead);
	if (isOpen==0)
	{
		return FALSE;
	}
	CString str,strData,strSQL,strDataY,strDataX;
	CString strFullData=_T("");

	//fp.ReadString(str);

	int Dataflag=0;
	while(fp.ReadString(str))
	{
		int iPos = str.Find(_T("\t")), iStart = 0;
		if (iPos==-1)
		{
			break;
		}
		strDataX=str.Mid(iStart,iPos - iStart);
		if (strDataX==_T(""))
		{
			break;
		}
		iStart = iPos + 1;
		strDataY=str.Mid(iStart);

		x[Dataflag]=atof(strDataX);
		y[Dataflag]=atof(strDataY);


		Dataflag++;
	}
	fp.Close();
	*Data=Dataflag;
	return TRUE;
}
double CaulRealNum(double lValueX,double*x,double*y,int *Data)
{
	double dbValue=1.0;
	for (int i=0;i<*Data;i++)
	{
		if((lValueX>=x[i]) && (lValueX<x[i+1]))
		{
			double k;
			k=((y[i+1]-y[i])/(x[i+1]-x[i]))*(lValueX-x[i])+y[i];
			dbValue=k;
			i=*Data;
		}
	}
	return dbValue;
}

void SetButtonState(CMFCButton *m_button)
{
	//m_button->SetFaceColor(RGB(153,217,234));
	//RGB(141, 215, 247)
	m_button->SetFaceColor(clr);
	m_button->SetMouseCursorHand();		
}
void SetCButtonState(CMFCButton *m_button)
{
	//m_button->SetFaceColor(RGB(153,217,234));
	//RGB(141, 215, 247)
	m_button->SetFaceColor(clrSet);	
}
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
int Split(CString content, CString pattern, COleSafeArray& strlistX)
{
	int iPos = content.Find(pattern), iStart = 0;
	CString xx;
	double dd;
	long i=0;
	while (iPos != -1)
	{
		dd=atof(content.Mid(iStart, iPos - iStart));
		strlistX.PutElement(&i,&dd);
		iStart = iPos + pattern.GetLength();
		iPos = content.Find(pattern, iStart);
		i++;
	}

	if (iStart <= content.GetLength())
	{
		dd=atof(content.Mid(iStart));
		strlistX.PutElement(&i,&dd);
	}

	return i+1;
}
BOOL IsNumber(CString &str)
{
	int len;
	str.TrimLeft();
	str.TrimRight();
	len=str.GetLength();
	if (len==0)
	{
		return TRUE;
	}

	TCHAR ch=str.GetAt(0);

	if ((ch!='-')&&(ch!='+')&&(ch!='.'))
	{
		if (ch<'0'||ch>'9')
		{
			return FALSE;
		}
	}
	for(int i=0;i<len;i++)
	{
		ch=str.GetAt(i);
		if (ch<'0'||ch>'9')
		{
			switch(ch) {
			case 'E':
			case 'e':
			case '+':
			case '-':
			case '.':
			case '\t':
				continue;
			default:
				return FALSE;
			}
		}
	}
	return TRUE;
}
int StrNumber(CString &str)
{
	int len;
	int nCount=0;
	TCHAR ch=str.GetAt(0);
	len=str.GetLength();
	for(int i=0;i<len;i++)
	{
		ch=str.GetAt(i);
		if (ch=='\t')
		{
			nCount++;		
		}
	}
	return nCount;
}

CString GetExePath()
{
	CString tPath,str,strTemp,strPath;
	GetModuleFileName(NULL,tPath.GetBufferSetLength (MAX_PATH+1),MAX_PATH);
	tPath.ReleaseBuffer();
	int nPos=tPath.ReverseFind ('\\');
	str=tPath.Left (nPos);
	return str;
}

BOOL m_bAutoRunning=FALSE;
BOOL m_bAutoRunningStatue=FALSE;
int nFontSize=140;
int nFontSizeEH=120;
COLORREF clr=RGB(163, 194, 209); 
//RGB(104, 33, 122)
//COLORREF clr=RGB(91, 154, 166); 
COLORREF clrSet=RGB(25, 121, 202);
//COLORREF clr=RGB(135, 206, 235);
CString strFontChinese=_T("宋体");
CString strFontEngish=_T("Arial");
CString strSoftName=_T("INScan-Lite");
ManageInfo sInfo;
CorrectInfo sCInfo;
SetInfo sSInfo;
TestInfo sTInfo;
PrintInfo sPInfo;
TestINTimeInfo sINInfo;

// lcz new 22/10/20
DataShow sDataShowInfo;

OffsetFileInfo sOffsetFileInfo;
std::vector<DataInfo> sDInfo;
std::vector<AllRes>sALLRes;
ZOLIX_DLL_MFC_Res::CZolixCCD m_ZolixCCD;
map<int, S_DATAINFO> g_mapDataInfo;
CString g_strCurLineName=_T("NULL");
int g_iScanOrder = 2;
int g_iCurBtnID = 1300;
int g_iPriorStep_X = 1000;
int g_iPriorStep_Y = 1000;
CString g_strDefSaveFilePath="";
CString g_strLogPath=GetExePath() + L"\\logs\\";

bool g_bCloseLaser = false;

//CRITICAL_SECTION g_cs_temp;
bool g_bGetTemp = false;
int g_iTemp = 0;

bool g_bCorrNumLen = false;//  true为波数，false为波长校正，通过点击“应用系数”进行修改
bool g_bUSBDiscon = false;// usb异常断开

//  保留两位小数，四舍五入   roundDouble2
double roundDouble(double a, int n)
{
	double bs = 10;
	int bs_n = (int)pow(bs, n);
	if(a > 0)
		return (floor(a * bs_n  + 0.5 ) )/bs_n;
	else if(a < 0)
		return (ceil(a * bs_n  - 0.5 ) )/bs_n;
	else
		return a;
}

void otoXAxisData( double *pd,double *coeff,int Num,int nPixSize )
{
	for (int i=0;i<nPixSize;i++)
	{
		for (int j=0;j<Num;j++)
		{
			pd[i]+=coeff[j]*pow(i*1.0, j);
		}
	}
}

double g_XRange_left = 0.0;
double g_XRange_Right = 0.0;
// 程序是否关闭（目前看只用于温度查询）
bool g_bClose = false;
int g_iConnectCCDOkNum = 0;
bool g_bOpenCamera = false;

bool g_bAndorTestingDiscon = false;

double* g_pBuffer = NULL;

bool g_bOpenCCDSet = false;

bool g_bFindCamera = false;

bool g_bCameraOpen2Close = false;

bool g_bFirstlyDataException = true;

bool g_bSingleCollectFinish = true;

bool g_bWaveLenCorr = true;// lcz new 03/17  暂时修改为false以切换到波数校正

bool g_bReconnectCCD = false;// 是否重连了CCD

int g_iAddPoint = 0;

bool g_bSingleTestFinish = true;

CMutex g_tmpMutex;