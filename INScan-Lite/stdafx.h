
// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件

#pragma once

#ifndef _SECURE_ATL
#define _SECURE_ATL 1
#endif

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // 从 Windows 头中排除极少使用的资料
#endif

//#include "targetver.h"


#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // 某些 CString 构造函数将是显式的

// 关闭 MFC 对某些常见但经常可放心忽略的警告消息的隐藏
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC 核心组件和标准组件
#include <afxext.h>         // MFC 扩展


#include <afxdisp.h>        // MFC 自动化类



#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC 对 Internet Explorer 4 公共控件的支持
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // MFC 对 Windows 公共控件的支持
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h>     // 功能区和控件条的 MFC 支持









#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif

#include "..\ZolixCCDSrc\common\MFCResGInclude.h"
//#include "..\ZolixCCDSrc\include\ZolixCCD.h"
#include "..\CCD\ZolixCCDSrc\include\ZolixCCD.h"
#include "Resource.h"
#include <afxdisp.h>

#define WM_UPDATENAME (WM_USER+100)
#define WM_SHOWLINE (WM_USER+101)

#define WM_MY_STATUSBAR WM_USER+264
#define CloseLaserPow 0
#define CM 10000000
#define KJJ 9 //96孔板 孔间距 9mm

// 限制当前图表的曲线数量，限制为15条
#define maxLineNum 15

struct DeviceInfo
{
	CString CCDName;//默认连接那个CCD
	int nTemp;				//温度
	int nGXPix;			//pixels x 个数
	int nGYPix;				//pixels y
	float fXPixSize;		    //像素尺寸

};

struct ManageInfo
{
	int nLanguage;//0-Ch 1-En 2-Tw
	int AdvShowID;//3-校正
	CString strLangPathIni;
	CString Path;//EXE
	CString strPath;//INScan配置文件路径INScan-Lite.ini
	CString strPathCCD;//ZolixCCD.ini
	CString strPathOffset;//ZolixOffsetFile.bm
	int OminiType;//见动态库
	double dDataRange;
 	int statusBar;//进度条

	int CConnect;
	int LConnect;
	int LockConnect;
	int PriorConnect;

	struct DeviceInfo sDInfo;
};

struct CorrectInfo
{
	double TargetNum;
	double CorrectNum;
	double Laser;
	int NMCM;//切换 0-波长 1-波数
	int CutF;
	int CutSzie;
	double CorrectHJW;//环己烷最后一个峰的校正系数
};

struct PrintInfo
{
	CString Human;
	CString ProName;
	CString Model;//仪器型号
	CString Number;//仪器编号
	CString strSysTime;//系统时间
	CString strSetTime;
	int AccNum;
	double dTime;
	int Pow;
};

struct TestInfo
{
	int   KLMode;//判断模块 0 -k 1-j 2-通过文件打开
	int   JDLMode;//判断模块 0 -jd 1-jl
	int   KPowerMode;
	double   KPower[3];
	double   KOutTime;
	double   KShowTime;
	double   KOutTimeLow;
	double   KRangeX;
	double	 KRangeS;
	int  	 KNum;
	double 	 KFTime;
	double	 JTime;
	int KAccNum;
	int JAccNum;
	int	JBack;
	double	JPower;
	int	JPowerL;
	int	JowerB;
	double JBkK;
	bool AddHeader;
	int CrossX;
	int CrossY;
	int AutoTest;
	bool b1064ccdFlip;
};
 
struct TestINTimeInfo
{
	int Time;
	int AccNum;//重复次数
	bool bCheak;
	bool bCheakSmooth;
	int nSmooth;
	bool bCheakFit;
	int nFit;
	bool bCheakCut;
	int nFileType;
	double dEditStart;
	double dEditEnd;
	CString strName;
	CString strPath;
};

struct SetInfo
{
	int    nCOM;
	int	 HLMODE;
	int	Temp;
	int	Delay;
	double	XLeft;
	double	XRight;
	int	Back;
	double	LASERPOWERMIN;
	double	LASERPOWERMAX;
	int Cool;//是否制冷
	int CorrMode;//0-不校正 1-校正
	double a[4];//  存放波长校正的系数
	double b[4];//  存放波数校正的系数
	double DataPer;//相似度阈值
	int DataPerNum;//显示多少个
	bool CheckLed;
	int ComboLed;// 相机连接的索引，0,1,2
	bool CheckOffset;

	int nCOMLED;
	int LockSet;
	int isOEM;//0-oem 1-串口
	int nLaserComMode;//0-杏林 1-长春新产业 串口激光器类型

	int nComPrior;

	///数据库
	int nMode;//T=0,K=1;
	int SeriesNum;
	bool bSQL;
	int nSqlMode;

	int iInitLaserPower;//  设置里开激光的功率默认百分比
	double d532rdWaveNum;
	double d785rdWaveNum;

	int CoolTemp;//  lcz new 23/05/12  1064制冷温度

	int iAddPointNum;// lcz new 22/10/11
	int isCheckLed;//监视相机电机是否存在

	int firstOminiType;
	int LaserPowerMin_XL;
	int LaserPowerMax_XL;

	int LaserPowerMin_XCY;
	int LaserPowerMax_XCY;
};
//  数据库里数据的信息，序号；相似度；中文名；英文名；编码；x轴数据；y轴数据；数据长度
struct DataInfo
{
	long  num;
	float    fSimilarNum;
	CString  name_ch;
	CString  name_eh;
	CString  code;
	CString  strXData;
	CString  strYData;
	int      datalen;
};
//  对比的结果：序号；对比结果（相似度）
struct AllRes 
{
	int id;
	double res[2];
};

struct OffsetFileInfo
{
	int FscDataNum;
	double *dFscDataX;
	double *dFscDataY;
};


struct S_DATAINFO
{
	CString strFilePath;
	CString strLineName;
	int iStatus;//0-当前数据（黄色）  1-已完成（蓝色）
	int iPosX;
	int iPosY;
	int iPosZ;
	int iOrder;//自动扫描顺序

	S_DATAINFO()
	{
		iStatus = 0;
		iPosX = 0;
		iPosY = 0;
		iPosZ = 0;
		iOrder = 0;
	}
};

// lcz new 22/10/20
struct DataShow
{
	int seriesNum;
	int color;
	double* x;
	double* y;
	int pointNum;
};

extern map<int, S_DATAINFO> g_mapDataInfo;
extern  BOOL m_bAutoRunning;
extern  BOOL m_bAutoRunningStatue;
extern int nFontSize;
extern int nFontSizeEH;
extern COLORREF clr;
extern COLORREF clrSet;
extern  ManageInfo sInfo;
extern  CorrectInfo sCInfo;
extern  SetInfo sSInfo;
extern  TestInfo sTInfo;
extern  PrintInfo sPInfo;
// lcz new 22/10/20
extern  DataShow sDataShowInfo;

extern TestINTimeInfo sINInfo;
extern OffsetFileInfo sOffsetFileInfo;
extern CString strFontChinese;
extern CString strFontEngish;
extern CString strSoftName;
extern std::vector<DataInfo>sDInfo;
extern std::vector<AllRes>sALLRes;
extern void SetButtonState(CMFCButton *m_button);
extern void SetCButtonState(CMFCButton *m_button);
extern BOOL Split(CString content, CString pattern, CStringArray& strlist);
extern int Split(CString content, CString pattern, COleSafeArray& strlistX);
extern BOOL IsNumber(CString &str);
extern int StrNumber(CString &str);
extern BOOL LoadFileData(CString strFathName,int *Data,double *x,double* y);

extern double CaulRealNum(double lValueX,double*x,double*y,int *Data);
extern void ReadOemMaxMin();
//using namespace  ZOLIX_DLL_MFC_Res;
extern ZOLIX_DLL_MFC_Res::CZolixCCD m_ZolixCCD;
extern CString g_strCurLineName;
extern int g_iScanOrder;
extern int g_iCurBtnID;
extern int g_iPriorStep_X;
extern int g_iPriorStep_Y;
extern CString g_strDefSaveFilePath;
extern CString g_strLogPath;

extern bool g_bCloseLaser;// 设置里，可以手动开关激光，当页面切换时自动关激光

extern bool g_bGetTemp;
extern int g_iTemp;

//extern CRITICAL_SECTION g_cs_temp; //  读取温度的临界区（锁）

extern bool g_bCorrNumLen;//  波数校正为true,波长校正为false
extern bool g_bUSBDiscon;// 正常连接下，突然断开连接；

extern double roundDouble(double, int);
extern void otoXAxisData(double *pd,double *coeff,int Num,int nPixSize);

extern double g_XRange_left;
extern double g_XRange_Right;
extern bool g_bClose;

extern int g_iConnectCCDOkNum;
extern bool g_bOpenCamera;// 是否打开相机

extern bool g_bAndorTestingDiscon;

extern double *g_pBuffer;// 存放未测试时采集的数据

extern bool g_bOpenCCDSet;// 是否打开CCD设置框

extern bool g_bFindCamera;// 是否找到外部相机

extern bool g_bCameraOpen2Close;//  表示相机由打开到关闭，在连续测试时若打开了相机，需要在下次测试前进行CCD的数据清空

extern bool g_bFirstlyDataException;

extern bool g_bSingleCollectFinish;// 表示单次采集是否结束

extern bool g_bWaveLenCorr;

extern bool g_bReconnectCCD;

extern int g_iAddPoint;// lcz new 22/10/11

extern bool g_bSingleTestFinish;//单谱采集是否结束

extern CMutex g_tmpMutex; // 温度查寻互斥锁