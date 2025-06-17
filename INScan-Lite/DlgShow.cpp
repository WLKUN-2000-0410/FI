// DlgShow.cpp : 实现文件
//

#include "stdafx.h"
#include "INScan-Lite.h"
#include "INScan-LiteDlg.h"
#include "DlgShow.h"
#include "SetNameDlg.h"
#include "..\file\TeeChart2011\valuelist.h"
#include "..\file\TeeChart2011\series.h"
#include "..\file\TeeChart2011\zoom.h"
#include "..\file\TeeChart2011\lineseries.h"
#include "..\file\TeeChart2011\axes.h"
#include "..\file\TeeChart2011\pen.h"
#include "..\file\TeeChart2011\axis.h"
#include "..\file\TeeChart2011\marks.h"
#include "..\file\TeeChart2011\teefont.h"
#include "..\file\TeeChart2011\markscallout.h"
#include "..\file\TeeChart2011\printer.h"
#include "..\file\TeeChart2011\legend.h"
#include ".\log\SimpleLog.h"

#include<ctime>// lcz new 22/10/11

#include <vector>
SimpleLog	g_DlgShowlog;    

#define SERIES_MAX 100
int g_iRGB[] = {
	RGB(0, 0, 139),RGB(0, 139, 139),RGB(139, 0, 139),RGB(139, 0, 0),RGB(139, 126, 102),RGB(28, 28, 28),RGB(93, 71, 139),RGB(139, 102, 139),RGB(139, 58, 98),RGB(255, 20, 147),
	RGB(139, 35, 35),RGB(205, 102, 29),RGB(139, 105, 20),RGB(139, 139, 0),RGB(205, 92, 92),RGB(0, 139, 0),RGB(255, 69, 0),RGB(0, 134, 139),RGB(106, 90, 205),RGB(0, 191, 255),
	RGB(0, 100, 0),RGB(219, 112, 147),RGB(72, 118, 255),RGB(255, 99, 71),RGB(16, 78, 139),RGB(132, 112, 255),RGB(85, 107, 47),RGB(210, 105, 30),RGB(0, 0, 255),RGB(255, 0, 255),
	RGB(0, 0, 139),RGB(0, 139, 139),RGB(139, 0, 139),RGB(139, 0, 0),RGB(139, 126, 102),RGB(28, 28, 28),RGB(93, 71, 139),RGB(139, 102, 139),RGB(139, 58, 98),RGB(255, 20, 147),
	RGB(139, 35, 35),RGB(205, 102, 29),RGB(139, 105, 20),RGB(139, 139, 0),RGB(205, 92, 92),RGB(0, 139, 0),RGB(255, 69, 0),RGB(0, 134, 139),RGB(106, 90, 205),RGB(0, 191, 255),
	RGB(0, 100, 0),RGB(219, 112, 147),RGB(72, 118, 255),RGB(255, 99, 71),RGB(16, 78, 139),RGB(132, 112, 255),RGB(85, 107, 47),RGB(210, 105, 30),RGB(0, 0, 255),RGB(255, 0, 255),
	RGB(0, 0, 139),RGB(0, 139, 139),RGB(139, 0, 139),RGB(139, 0, 0),RGB(139, 126, 102),RGB(28, 28, 28),RGB(93, 71, 139),RGB(139, 102, 139),RGB(139, 58, 98),RGB(255, 20, 147),
	RGB(139, 35, 35),RGB(205, 102, 29),RGB(139, 105, 20),RGB(139, 139, 0),RGB(205, 92, 92),RGB(0, 139, 0),RGB(255, 69, 0),RGB(0, 134, 139),RGB(106, 90, 205),RGB(0, 191, 255),
	RGB(0, 100, 0),RGB(219, 112, 147),RGB(72, 118, 255),RGB(255, 99, 71),RGB(16, 78, 139),RGB(132, 112, 255),RGB(85, 107, 47),RGB(210, 105, 30),RGB(0, 0, 255),RGB(255, 0, 255),
	RGB(0, 0, 139),RGB(0, 139, 139),RGB(139, 0, 139),RGB(139, 0, 0),RGB(139, 126, 102),RGB(28, 28, 28),RGB(93, 71, 139),RGB(139, 102, 139),RGB(139, 58, 98),RGB(255, 20, 147)
};
vector<int> g_vec_MaxY;
CRITICAL_SECTION g_cs; 
bool g_bNewTest = false;
bool g_bStopTest = false;

CDlgShow *pDlg=NULL;
#define DataMax   3000

int g_r = 0;
int g_g = 10;
int g_b = 20;
int WriteToLog(char* str)
{
	FILE* pfile;
	fopen_s(&pfile, FILE_PATH, "a+");

	if (pfile == NULL)
	{
		return -1;
	}

	char szDateTime[20];
	SYSTEMTIME tm;
	GetLocalTime(&tm);
	sprintf(szDateTime, "%02d:%02d:%02d.%03d", tm.wHour, tm.wMinute, tm.wSecond, tm.wMilliseconds);
	fprintf_s(pfile, "%s:%s\n", szDateTime, str);

	fclose(pfile);

	return 0;
}
int WriteToLog_S(const char* str)
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
int cutFitSmooth(double * bufferXR, double * bufferYR, int CutSzie,double * newlist_x, double * newlist_y)
{

	vector<double> x_newList;
	vector<double> y_newList;
	int j = 0;
	//截取
	if (sINInfo.bCheakCut)
	{
		for (int i = 0;i<CutSzie;i++)
		{
			if (bufferXR[i]>sINInfo.dEditStart&&bufferXR[i]<=sINInfo.dEditEnd)
			{
				x_newList.push_back(bufferXR[i]);
				y_newList.push_back(bufferYR[i]);
			}
		}

	}else{
		for (int i = 0;i<CutSzie;i++)
		{
			x_newList.push_back(bufferXR[i]);
			y_newList.push_back(bufferYR[i]);
		}
	}
	double* aa = new double[x_newList.size()*2];
	double* correctsignal = new double[y_newList.size()*2];
	int nResultSize = x_newList.size()*2;
	for (int g = 0; g < x_newList.size();g++)
	{
		aa[g] = x_newList[g];
		aa[x_newList.size()+g] = y_newList[g];
		correctsignal[g] = x_newList[g];
		correctsignal[x_newList.size()+g] = y_newList[g];
	}
	//拟合
	if (sINInfo.bCheakFit)
	{
		main_baseline_correct(aa,x_newList.size()*2,sINInfo.nFit,correctsignal,&nResultSize);
		for (int w = 0; w < x_newList.size()*2;w++)
		{
			//lcz new 07/15
			//aa[w] = correctsignal[w];
			aa[w] = aa[w] - correctsignal[w];
			correctsignal[w] = aa[w];
		}
	}
	//平滑
	if (sINInfo.bCheakSmooth)
	{
		main_denoise2(aa,x_newList.size()*2,sINInfo.nSmooth,correctsignal,&nResultSize);
	}

	for (int c=0;c<nResultSize/2;c++)
	{
		newlist_x[c] = x_newList[c];
		newlist_y[c] = correctsignal[x_newList.size()+c];
		double y=0;
		y=correctsignal[x_newList.size()+c];
	}
	delete[] aa;
	delete[] correctsignal;

	return nResultSize/2;
}

DWORD WINAPI  ThreadGetDataK(LPVOID lpParam)
{
	pDlg->ReMoveSeries(0);
	pDlg->m_Tchart.AddSeries(0);
	pDlg->OnSetAutoRepaint(FALSE);

	CValueList list_y;
	double *buffer=new double[DataMax];
	double *bufferX=new double[DataMax];
	double *bufferBkData=new double[DataMax];//暗背景数据
	double *bufferR=new double[DataMax];
	double *bufferXR=new double[DataMax];
	double *bufferBkDataR=new double[DataMax];//暗背景数据

	memset(bufferBkData,0,sizeof(double)*DataMax);
	memset(buffer,0,sizeof(double)*DataMax);
	memset(bufferX,0,sizeof(double)*DataMax);
	memset(bufferBkDataR,0,sizeof(double)*DataMax);
	memset(bufferR,0,sizeof(double)*DataMax);
	memset(bufferXR,0,sizeof(double)*DataMax);

	pDlg->SetLawerPower(sTInfo.KPower[sTInfo.KPowerMode],FALSE);

	m_ZolixCCD.XAxisData(bufferX,sSInfo.a,6,sInfo.sDInfo.nGXPix);
	sCInfo.CutSzie=m_ZolixCCD.GetRangeDataX(sCInfo.NMCM,sInfo.sDInfo.nGXPix,sCInfo.Laser,sSInfo.XLeft,
		sSInfo.XRight,bufferX,bufferXR,&sCInfo.CutF);
	int test=0,j=0;
	double dFxTime=sTInfo.KFTime;
	while(m_bAutoRunning&&test<sTInfo.KNum)
	{
		m_ZolixCCD.SetExpTime(dFxTime);
		m_ZolixCCD.DataAcquisition(buffer,sInfo.sDInfo.nGXPix);
		m_ZolixCCD.GetRangeDataY(sCInfo.CutF,sCInfo.CutSzie,buffer,bufferR);

		double dMaxData=pDlg->ReturnMaxData(buffer,sInfo.sDInfo.nGXPix);

		if (dMaxData>=sTInfo.KRangeX&&dMaxData<=sTInfo.KRangeS)
		{
			pDlg->bReturn=TRUE;
			break;
		}

		dFxTime=dFxTime*(pDlg->ReturnK(dMaxData));
		//在15-20秒中扫描一次如果仍不符合则直接设置为20s
		if (dFxTime>(sTInfo.KOutTime-5)&&dFxTime<sTInfo.KOutTime)
		{
			j++;
			if (j==2)
			{
				dFxTime=sTInfo.KOutTime;
			}
		}

		if (dFxTime<sTInfo.KOutTimeLow||dFxTime>=sTInfo.KOutTime||dMaxData<=0||test==5)
		{

			if (dFxTime<sTInfo.KOutTimeLow){
				dFxTime=sTInfo.KOutTimeLow;
				pDlg->bReturn=FALSE;}
			else{
				dFxTime=sTInfo.KOutTime;}

			if (dFxTime!=0)
			{
				//dFxTime=dFxTime/(pDlg->ReturnK(dMaxData));			
				m_ZolixCCD.SetExpTime(dFxTime);
				m_ZolixCCD.DataAcquisition(buffer,sInfo.sDInfo.nGXPix);
				m_ZolixCCD.GetRangeDataY(sCInfo.CutF,sCInfo.CutSzie,buffer,bufferR);

			}	
			break;
		}
		pDlg->OnSetAutoRepaint(TRUE);
		test++;
	}

	pDlg->SetLawerPower(CloseLaserPow,FALSE);

	if (sSInfo.Back==1)
	{
		m_ZolixCCD.SetExpTime(dFxTime);
		m_ZolixCCD.DataAcquisition(bufferBkData,sInfo.sDInfo.nGXPix);
		m_ZolixCCD.GetRangeDataY(sCInfo.CutF,sCInfo.CutSzie,bufferBkData,bufferBkDataR);

	}


	for (int i=0;i<sCInfo.CutSzie;i++)
	{
		double dYdata=bufferR[i]-bufferBkDataR[i];

		dYdata=pDlg->ScanOffset(bufferXR[i],dYdata);
		pDlg->m_Tchart.Series(0).AddXY(bufferXR[i],dYdata,NULL,clrSet);
	}
	sTInfo.KShowTime=dFxTime;
	pDlg->OnSetAutoRepaint(TRUE);

	m_bAutoRunning=FALSE;
	m_bAutoRunningStatue=FALSE;

	delete [] buffer;
	delete [] bufferX;
	delete [] bufferBkData;
	delete [] bufferR;
	delete [] bufferXR;
	delete [] bufferBkDataR;
	return 0;
}
DWORD WINAPI ThreadGetDataA1K(LPVOID lpParam)
{
	// 获取主对话框指针
	CINScanLiteDlg* pDlgF = (CINScanLiteDlg*)AfxGetApp()->GetMainWnd();

	// 禁止自动重绘，待后续统一刷新图表
	pDlg->OnSetAutoRepaint(FALSE);

	// ---------------------------
	// 1. 初始化X轴数据（光谱横坐标）
	// ---------------------------
	double bufferX[DataMax];
	double* bufferXR = new double[DataMax];  // 用于存放截取后的横坐标数据
	memset(bufferX, 0, sizeof(double) * DataMax);
	memset(bufferXR, 0, sizeof(double) * DataMax);

	// 采用 K 模式下的校正参数（此处调用时选择的参数与 ThreadGetDataK 类似）
	m_ZolixCCD.XAxisData(bufferX, sSInfo.a, 6, sInfo.sDInfo.nGXPix);

	// 根据 sSInfo.XLeft 和 sSInfo.XRight 截取有效区域
	int count = 0;
	for (int i = 0; i < sInfo.sDInfo.nGXPix; i++)
	{
		if (bufferX[i] < sSInfo.XLeft)
			sCInfo.CutF = i + 1;
		if (bufferX[i] <= sSInfo.XRight)
			count++;
	}
	sCInfo.CutSzie = count - sCInfo.CutF;
	for (int i = 0; i < sCInfo.CutSzie; i++)
	{
		bufferXR[i] = bufferX[sCInfo.CutF + i];
	}

	// ---------------------------
	// 2. 分配数据采集及背景扣除所需缓冲区
	// ---------------------------
	// 用于采集光谱数据
	double* buffer = new double[DataMax];
	memset(buffer, 0, sizeof(double) * DataMax);

	// 用于存放暗噪声数据（原始与截取后的）
	double* bufferBkData = new double[DataMax];
	double* bufferBkDataR = new double[DataMax];
	memset(bufferBkData, 0, sizeof(double) * DataMax);
	memset(bufferBkDataR, 0, sizeof(double) * DataMax);

	// 用于存放背景数据（如需要扣除暗背景）
	double* bufferS1 = new double[DataMax];
	double* bufferS1R = new double[DataMax];
	memset(bufferS1, 0, sizeof(double) * DataMax);
	memset(bufferS1R, 0, sizeof(double) * DataMax);

	// ---------------------------
	// 3. 预先清空数据（若相机已打开）
	// ---------------------------
	if (m_bAutoRunning == 0 || g_bUSBDiscon)
		return 0;
	else
	{
		// 使用短曝光（例如0.2s）采集一次数据，以清除残留数据
		dfield_set_exposure(pDlgF->_handle, 0.2 * 1e3 * 1e3);
		m_ZolixCCD.DataAcquisition(buffer, sInfo.sDInfo.nGXPix);
		memset(buffer, 0, sizeof(double) * DataMax);
	}

	// ---------------------------
	// 4. 背景/暗噪声数据采集（如果勾选了扣除暗噪声或暗背景）
	// ---------------------------
	// 若勾选扣除暗噪声（sSInfo.Back==1），则采集暗噪声数据
	double dFxTime = sTInfo.KFTime;
	if (sSInfo.Back == 1)
	{
		g_bSingleCollectFinish = false;
		pDlg->SetLawerPower(CloseLaserPow, FALSE);  // 先关闭激光器
		Sleep(100); // 等待激光器稳定关闭
		m_ZolixCCD.SetExpTime(dFxTime);  // 使用K模式下的积分时间
		m_ZolixCCD.DataAcquisition(bufferBkData, sInfo.sDInfo.nGXPix);
		// 提取有效区域的暗噪声数据
		m_ZolixCCD.GetRangeDataY(sCInfo.CutF, sCInfo.CutSzie, bufferBkData, bufferBkDataR);
		g_bSingleCollectFinish = true;
	}
	// 若勾选扣除暗背景（例如 sTInfo.KBack == 1），则采集背景数据
	if (sTInfo.JBack == 1)
	{
		if (AfxMessageBox(pDlgF->LoadStringMeau(_T("ID_DATA_BACK")), MB_OKCANCEL) == IDCANCEL)
		{
			m_bAutoRunning = FALSE;
			goto nEnd;
		}
		g_bSingleCollectFinish = false;
		pDlg->SetLawerPower(CloseLaserPow, FALSE);  // 关闭激光器，确保无激光照射
		Sleep(100);
		m_ZolixCCD.SetExpTime(dFxTime);
		m_ZolixCCD.DataAcquisition(bufferS1, sInfo.sDInfo.nGXPix);
		// 提取有效区域的背景数据
		m_ZolixCCD.GetRangeDataY(sCInfo.CutF, sCInfo.CutSzie, bufferS1, bufferS1R);
		g_bSingleCollectFinish = true;

		if (AfxMessageBox(pDlgF->LoadStringMeau(_T("ID_DATA_SAMPLE")), MB_OKCANCEL) == IDCANCEL)
		{
			m_bAutoRunning = FALSE;
			goto nEnd;
		}
	}

	// ---------------------------
	// 5. 开启激光器（使用 K 模式下的激光功率）
	// ---------------------------
	pDlg->SetLawerPower(sTInfo.KPower[sTInfo.KPowerMode], FALSE);

	// ---------------------------
	// 6. 在图表中新增一个系列（仅创建一次）
	// ---------------------------
	int nIndex = pDlg->m_Tchart.AddSeries(0);
	{
		int iTemp = nIndex;
		CString strTempName = g_strCurLineName + L"-";
		int iNameRepeat = 0;
		while (iTemp > 0)
		{
			CString strName = pDlg->m_Tchart.Series(iTemp - 1).GetTitle();
			if (strName.CompareNoCase(g_strCurLineName) == 0)
				iNameRepeat++;
			if (strName.Find(strTempName) >= 0)
				iNameRepeat++;
			pDlg->m_Tchart.Series(iTemp - 1).SetActive(FALSE);
			iTemp--;
		}
		nIndex = nIndex >= 0 ? nIndex : 0;
		int color = g_iRGB[nIndex % SERIES_MAX];
		pDlg->m_Tchart.Series(nIndex).SetColor(color);

		CString seriesName;
		if (g_strCurLineName.CompareNoCase(_T("NULL")) == 0)
			seriesName = pDlg->m_Tchart.Series(nIndex).GetName();
		else
		{
			seriesName = g_strCurLineName;
			if (iNameRepeat > 0)
			{
				CString strName;
				strName.Format(_T("-%d"), iNameRepeat);
				seriesName += strName;
			}
		}
		pDlg->m_Tchart.Series(nIndex).SetTitle(seriesName);
	}

	// ---------------------------
	// 7. 连续采集、累积平均并实时更新图表
	// ---------------------------
	// 清零全局累积缓冲区（g_pBuffer）并初始化累计次数
	memset(g_pBuffer, 0, sizeof(double) * 3000);
	int test = 0; // 累计采集次数
	int color = pDlg->m_Tchart.Series(nIndex).GetColor();

	while (m_bAutoRunning)
	{
		// 根据界面设定的积分时间采集，单位转换为 ns（K模式下使用 sTInfo.KTime）
		dfield_set_exposure(pDlgF->_handle, dFxTime * 1e3 * 1e3);

		g_bSingleCollectFinish = false;
		m_ZolixCCD.DataAcquisition(buffer, sInfo.sDInfo.nGXPix);
		g_bSingleCollectFinish = true;  // 一次采集结束

										// 若有额外补点需求，可在此处处理（此处设置为0）
		g_iAddPoint = 0;

		// 第一次采集时直接赋值，后续则进行累积平均
		if (test == 0)
		{
			for (int i = 0; i < sCInfo.CutSzie + g_iAddPoint; i++)
			{
				double dDataY = buffer[i];
				if (sSInfo.Back == 1 && sTInfo.JBack == 0)
					dDataY = buffer[i] - bufferBkDataR[i];
				if (sSInfo.Back == 0 && sTInfo.JBack == 1)
					dDataY = buffer[i] - bufferS1R[i];
				if (sSInfo.Back == 1 && sTInfo.JBack == 1)
					dDataY = buffer[i] - sTInfo.JBkK * bufferS1R[i] - (1 - sTInfo.JBkK) * bufferBkDataR[i];
				// 如需要额外校正，可调用：dDataY = pDlg->ScanOffset(bufferXR[i], dDataY);
				g_pBuffer[i] = dDataY;
			}
		}
		else
		{
			for (int i = 0; i < sCInfo.CutSzie + g_iAddPoint; i++)
			{
				double dDataY = buffer[i];
				if (sSInfo.Back == 1 && sTInfo.JBack == 0)
					dDataY = buffer[i] - bufferBkDataR[i];
				if (sSInfo.Back == 0 && sTInfo.JBack == 1)
					dDataY = buffer[i] - bufferS1R[i];
				if (sSInfo.Back == 1 && sTInfo.JBack == 1)
					dDataY = buffer[i] - sTInfo.JBkK * bufferS1R[i] - (1 - sTInfo.JBkK) * bufferBkDataR[i];
				// dDataY = pDlg->ScanOffset(bufferXR[i], dDataY);
				// 进行累积平均
				g_pBuffer[i] = (g_pBuffer[i] * test + dDataY) / (test + 1);
			}
		}

		// 更新显示数据结构并发送消息给主界面刷新图表
		sDataShowInfo.color = color;
		sDataShowInfo.seriesNum = nIndex;
		sDataShowInfo.x = bufferXR;
		sDataShowInfo.y = g_pBuffer;
		sDataShowInfo.pointNum = sCInfo.CutSzie + g_iAddPoint;
		::PostMessage(pDlg->m_hWnd, WM_SHOWDATA, 0, (LPARAM)(&sDataShowInfo));

		// 检查相机状态，若超时则退出采集
		if (!g_bOpenCamera && WaitForSingleObject(pDlg->m_hEvent, 5000) == WAIT_TIMEOUT)
		{
			goto nEnd;
		}

		test++;  // 累计采集次数增加

				 // 如需要，可加入适当延时以控制采集频率
				 // Sleep(适当延时);
	}

nEnd:
	// ---------------------------
	// 8. 退出处理
	// ---------------------------
	// 关闭激光器
	pDlg->SetLawerPower(CloseLaserPow, FALSE);
	// 自动保存当前图表数据（保存文件时使用系列名称）
	pDlg->AutoSaveFile(pDlg->m_Tchart.Series(nIndex).GetTitle());

	m_bAutoRunning = FALSE;
	m_bAutoRunningStatue = FALSE;

	// 释放动态分配的内存
	delete[] buffer;
	delete[] bufferXR;

	// 重置相关标志
	g_bGetTemp = false;
	g_bSingleTestFinish = true;

	return 0;
}


//  精检——连续  1064  lcz new 23/05/12 
DWORD WINAPI  ThreadGetDataL1064(LPVOID lpParam)
{
	g_bNewTest = true;
	g_bStopTest = false;

	g_DlgShowlog.WriteDebugLog(_T("ThreadGetDataL1064 Begin!"));
	CINScanLiteDlg *pDlgF = (CINScanLiteDlg*)AfxGetApp()->GetMainWnd();

	pDlg->OnSetAutoRepaint(FALSE);

	CValueList list_y;
	double *bufferX = new double[DataMax]; //  根据校正系数计算得到的波长值（目前是波长，535-635 nm)
	double *bufferXR = new double[DataMax];//  截取的X轴数据，坐标为波数，201.3cm-1起
	memset(bufferX, 0, sizeof(double)*DataMax);
	memset(bufferXR, 0, sizeof(double)*DataMax);
	unsigned short* buffer = new unsigned short[DataMax];
	memset(buffer, 0, sizeof(unsigned short)*DataMax);


	//  分波数校正还是波长校正
	sCInfo.CutF = 0;
	//if (g_bCorrNumLen)// 如果是波数校正
	//{
		otoXAxisData(bufferX, sSInfo.b, 4, sInfo.sDInfo.nGXPix);

		int count = 0;
		//  按照所截取的波数范围进行设置
		for (int i = 0; i < sInfo.sDInfo.nGXPix; i++)
		{
			if (bufferX[i] < sSInfo.XLeft)
			{
				sCInfo.CutF = i + 1;
			}
			if (bufferX[i] <= sSInfo.XRight)
			{
				count++;
			}
		}
		sCInfo.CutSzie = count - sCInfo.CutF;
		//  截取后的波数范围值
		for (int i = 0; i < sCInfo.CutSzie; i++)
			bufferXR[i] = bufferX[sCInfo.CutF + i];
	
	if (m_bAutoRunning == 0 || g_bUSBDiscon)
		return 0;
	else
	{
		bool b = dfield_set_exposure(pDlgF->_handle, 0.2 * 1e3 * 1e3);// 0.2s = 200ms = 200 * 1e3 us
		//  根据像素点数读取数据，存放在buffer里
		//std::vector<unsigned short> _y = std::vector<unsigned short>(sInfo.sDInfo.nGXPix, 0);
		//dfield_get_data(pDlgF->_handle, &(_y[0]), sInfo.sDInfo.nGXPix);
		pDlg->show_dfield_get_data(pDlgF->_handle, buffer, sInfo.sDInfo.nGXPix, sCInfo.CutF);
		memset(buffer, 0, sizeof(unsigned short)*DataMax);
	}

	//  点击连续，默认激光器未打开

	pDlg->SetLawerPower(sTInfo.JPower, FALSE);


	int test = -1;
	int nIndex = 0;
	int color = 0;
	int JAccNum = sTInfo.JAccNum;
	CString seriesName;
	while (m_bAutoRunning)
	{
		Sleep(800);
		while (m_bAutoRunning)
		{
			if (g_bClose)
			{
				break;
			}
			//Sleep(sTInfo.JTime*1000);

			if (m_bAutoRunning == 0)
				return 0;
			else
			{
				if (g_bCameraOpen2Close) // 如果测试中相机从打开到关闭，下次获取数据前需要清空次数据
				{
					g_bCameraOpen2Close = false;
					//m_ZolixCCD.SetExpTime(0.015);
					bool b = dfield_set_exposure(pDlgF->_handle, 0.02 * 1e3 * 1e3);// 0.2s = 200ms = 200 * 1e3 us
					pDlg->show_dfield_get_data(pDlgF->_handle, buffer, sInfo.sDInfo.nGXPix, sCInfo.CutF);
					memset(buffer, 0, sizeof(unsigned short)*DataMax);// 数据清零
				}

				//memset(buffer, 0, sizeof(double) * 3000);
				g_bSingleCollectFinish = false;
				dfield_set_exposure(pDlgF->_handle, sTInfo.JTime * 1e3 * 1e3);// 0.2s = 200ms = 200 * 1e3 us
				dfield_capture(pDlgF->_handle);//  连续采集时，需添加此行代码才可以，不然数据不会刷新
				memset(buffer, 0, sizeof(unsigned short)*DataMax);// 数据清零
				pDlg->show_dfield_get_data(pDlgF->_handle, buffer, sInfo.sDInfo.nGXPix, sCInfo.CutF);
				//  如果添加了镜像
				if (sTInfo.b1064ccdFlip)
				{
					double temp = 0.0;
					for (int i = 0; i < sInfo.sDInfo.nGXPix / 2; i++)
					{
						double temp = buffer[i];  
						buffer[i] = buffer[sInfo.sDInfo.nGXPix - i - 1];
						buffer[sInfo.sDInfo.nGXPix - i - 1] = temp;
					}
				}

				//m_ZolixCCD.SetExpTime(sTInfo.JTime);
				//  根据像素点数读取数据，存放在buffer里-----这行获取的数据出现了问题
				//m_ZolixCCD.DataAcquisition(buffer, sInfo.sDInfo.nGXPix);

				g_bSingleCollectFinish = true;


			}
			//  对拉曼强度数据进行截取，截取后的数据存放在bufferR中
			//sCInfo.CutF = 0;
			//sCInfo.CutSzie = sInfo.sDInfo.nGXPix; 
			//m_ZolixCCD.GetRangeDataY(sCInfo.CutF, sCInfo.CutSzie, buffer, bufferR);
			g_DlgShowlog.WriteDebugLog(_T("Get Data Once!"));

			// lcz new 22/10/11
			g_iAddPoint = 0;

			if (test == -1)
			{
				nIndex = pDlg->m_Tchart.AddSeries(0);//  添加第一条线
				int iTemp = nIndex;
				while (iTemp > 0)
				{
					pDlg->m_Tchart.Series(iTemp - 1).SetActive(FALSE);
					iTemp--;
				}
				//color = pDlg->m_Tchart.Series(nIndex).GetColor();
				//  设置第 nIndex 条曲线的名称、标题
				nIndex = nIndex >= 0 ? nIndex : 0;
				color = g_iRGB[nIndex%SERIES_MAX];
				pDlg->m_Tchart.Series(nIndex).SetColor(color);
				seriesName = pDlg->m_Tchart.Series(nIndex).GetName();
				pDlg->m_Tchart.Series(nIndex).SetTitle(seriesName);
				pDlg->m_Tchart.Series(nIndex).SetShowInLegend(true);
			}
			test++;

			memset(g_pBuffer, 0, sizeof(double) * 3000);
			if (test == 0)
			{
				int i;
				color = pDlg->m_Tchart.Series(nIndex).GetColor();
				for (i = 0; i<sCInfo.CutSzie + g_iAddPoint; i++)// lcz new 22/10/11
				{
					g_pBuffer[i] = buffer[i];
				}
			}
			else
			{
				list_y = pDlg->m_Tchart.Series(nIndex).GetYValues();
				int i;
				for (i = 0; i<sCInfo.CutSzie + g_iAddPoint; i++)
				{
					g_pBuffer[i] = buffer[i];
				}
			}

			// lcz new 22/10/20  谱图刷新
			pDlg->OnSetAutoRepaint(TRUE);//  必须加上这行
			sDataShowInfo.color = color;
			sDataShowInfo.seriesNum = nIndex;
			sDataShowInfo.x = bufferXR;
			sDataShowInfo.y = g_pBuffer;
			sDataShowInfo.pointNum = sCInfo.CutSzie + g_iAddPoint;
			::PostMessage(pDlg->m_hWnd, WM_SHOWDATA, 0, (LPARAM)(&sDataShowInfo));
			if (g_bOpenCamera == false && WaitForSingleObject(pDlg->m_hEvent, 5000) == WAIT_TIMEOUT)
			{
				//return 0;
				goto nEnd;
			}


			if (sSInfo.CorrMode == 1)
			{
				m_bAutoRunning = FALSE;
				//pDlgF->m_DlgAdv.m_DlgLaCorr.SetButtonS(TRUE);
				sSInfo.CorrMode = 0;
			}
		}
	}

nEnd:
	g_bStopTest = true;

	//g_bOpenCamera = false;
	pDlg->SetLawerPower(CloseLaserPow, FALSE);//  先关激光再保存文件
	g_DlgShowlog.WriteDebugLog(_T("ThreadGetDataL Over!!"));
	pDlg->AutoSaveFile(seriesName);
	m_bAutoRunning = FALSE;
	m_bAutoRunningStatue = FALSE;

	delete[] buffer;
	buffer = NULL;
	delete[] bufferX;
	bufferX = NULL;
	delete[] bufferXR;
	bufferXR = NULL;

	g_bGetTemp = false;

	return 0;


}
//DWORD WINAPI  ThreadGetDataLA1(LPVOID lpParam) {
//	CINScanLiteDlg* pDlgF = (CINScanLiteDlg*)AfxGetApp()->GetMainWnd();
//	pDlg->OnSetAutoRepaint(FALSE);
//
//	int test = 0;                    //采谱次数
//	int JAccNum = sTInfo.JAccNum;    //累计次数
//	CString seriesName;
//
//	//光谱横坐标的数组 类似于伟哥写的_X
//	//动态分配的bufferXR用于存放截取后的横坐标数据
//	//清空
//	CValueList list_y;
//
//	double bufferX[DataMax];
//	double* bufferXR = new double[DataMax];
//	memset(bufferX, 0, sizeof(double) * DataMax);
//	memset(bufferXR, 0, sizeof(double) * DataMax);
//
//	// buffer用于接收采集的光谱数据
//	double * buffer = new double[DataMax];
//	memset(buffer, 0, sizeof(unsigned short) * DataMax);
//
//	double* bufferBkData = new double[DataMax];   // 原始暗噪声数据
//	double* bufferBkDataR = new double[DataMax];    // 截取区域的暗噪声数据
//	double *bufferS1 = new double[DataMax];
//	double * bufferS1R = new double[DataMax];
//	memset(bufferBkData, 0, sizeof(double) * DataMax);
//	memset(bufferBkDataR, 0, sizeof(double) * DataMax);
//	memset(bufferS1, 0, sizeof(double)*DataMax);
//	memset(bufferS1R, 0, sizeof(double)*DataMax);
//
//	m_ZolixCCD.XAxisData(bufferX, sSInfo.b, 4, sInfo.sDInfo.nGXPix);
//
//	// 根据 XLeft、XRight 截取所需范围，把截取区域的横坐标数据复制到bufferXR中，供后续显示使用
//	int count = 0;
//	for (int i = 0; i < sInfo.sDInfo.nGXPix; i++)
//	{
//		if (bufferX[i] < sSInfo.XLeft)
//		{
//			sCInfo.CutF = i + 1;
//		}
//		if (bufferX[i] <= sSInfo.XRight)
//		{
//			count++;
//		}
//	}
//	sCInfo.CutSzie = count - sCInfo.CutF;
//	for (int i = 0; i < sCInfo.CutSzie; i++)
//	{
//		bufferXR[i] = bufferX[sCInfo.CutF + i];
//	}
//
//	if (m_bAutoRunning == 0 || g_bUSBDiscon)
//		return 0;
//	else
//	{
//		// 设置积分时间 0.2s 并获取一次数据并清空
//		dfield_set_exposure(pDlgF->_handle, 0.2 * 1e3 * 1e3);
//		m_ZolixCCD.DataAcquisition(buffer, sInfo.sDInfo.nGXPix);
//		memset(buffer, 0, sizeof(double) * DataMax);
//	}
//	//判断是否勾选了扣除暗噪声
//	if (sSInfo.Back == 1) {
//		g_bSingleCollectFinish = false;
//		pDlg->SetLawerPower(CloseLaserPow, FALSE);  //先关激光
//		Sleep(100); // 等待激光器稳定关闭
//		m_ZolixCCD.SetExpTime(sTInfo.JTime);
//		m_ZolixCCD.DataAcquisition(bufferBkData, sInfo.sDInfo.nGXPix);
//		//提取有效范围的暗噪声数据
//		m_ZolixCCD.GetRangeDataY(sCInfo.CutF, sCInfo.CutSzie, bufferBkData, bufferBkDataR);
//		g_bSingleCollectFinish = true;
//	}
//	//判断是否勾选了扣除暗背景
//	if (sTInfo.JBack == 1) {
//
//		if (AfxMessageBox(pDlgF->LoadStringMeau(_T("ID_DATA_BACK")), MB_OKCANCEL) == IDCANCEL) {
//			m_bAutoRunning = FALSE;
//			goto nEnd;
//		}
//		g_bSingleCollectFinish = false;
//		pDlg->SetLawerPower(CloseLaserPow, FALSE);  // 关闭激光器，确保无激光照射
//		Sleep(100); // 等待激光器稳定关闭
//
//		m_ZolixCCD.SetExpTime(sTInfo.JTime);
//		m_ZolixCCD.DataAcquisition(bufferS1, sInfo.sDInfo.nGXPix);
//		// 提取有效范围的暗背景数据到 bufferBkDataR
//		m_ZolixCCD.GetRangeDataY(sCInfo.CutF, sCInfo.CutSzie, bufferS1, bufferS1R);
//		g_bSingleCollectFinish = true;
//
//		if (AfxMessageBox(pDlgF->LoadStringMeau(_T("ID_DATA_SAMPLE")), MB_OKCANCEL) == IDCANCEL) {
//			m_bAutoRunning = FALSE;
//			goto nEnd;
//		}
//
//	}
//	pDlg->SetLawerPower(sTInfo.JPower, FALSE);  //重新开启激光供后续采集
//
//												//循环采集
//	while (m_bAutoRunning)
//	{
//		// =====在图表中新增一个序列，处理重名问题，设置序列颜色和名称======
//		int nIndex = pDlg->m_Tchart.AddSeries(0);
//		int iTemp = nIndex;
//		CString strTempName = g_strCurLineName + L"-";
//		int iNameRepeat = 0;
//
//		while (iTemp > 0)
//		{
//			CString strName = pDlg->m_Tchart.Series(iTemp - 1).GetTitle();
//			if (strName.CompareNoCase(g_strCurLineName) == 0)
//				iNameRepeat++;
//			if (strName.Find(strTempName) >= 0)
//				iNameRepeat++;
//
//			pDlg->m_Tchart.Series(iTemp - 1).SetActive(FALSE);
//			iTemp--;
//		}
//
//		nIndex = nIndex >= 0 ? nIndex : 0;
//		int color = g_iRGB[nIndex % SERIES_MAX];
//		pDlg->m_Tchart.Series(nIndex).SetColor(color);
//
//		// 设置序列名称
//		if (g_strCurLineName.CompareNoCase(_T("NULL")) == 0)
//		{
//			seriesName = pDlg->m_Tchart.Series(nIndex).GetName();
//		}
//		else
//		{
//			seriesName = g_strCurLineName;
//			if (iNameRepeat > 0)
//			{
//				CString strName;
//				strName.Format(_T("-%d"), iNameRepeat);
//				seriesName += strName;
//			}
//		}
//		pDlg->m_Tchart.Series(nIndex).SetTitle(seriesName);
//		//== == == = 在图表中新增一个序列，处理重名问题，设置序列颜色和名称 == == == =
//
//		//== == == ==采集部分== == == ==
//		memset(g_pBuffer, 0, sizeof(double) * 3000);
//		dfield_set_exposure(pDlgF->_handle, sTInfo.JTime * 1e3 * 1e3);//根据界面设定的积分时间采集
//
//
//		while (m_bAutoRunning)
//		{
//			g_bSingleCollectFinish = false;
//			m_ZolixCCD.DataAcquisition(buffer, sInfo.sDInfo.nGXPix);
//
//			//镜像后续写
//
//			g_bSingleCollectFinish = true;  //一次采集结束
//											// 若有额外补点数量
//			g_iAddPoint = 0;
//
//			// 累积或直接赋值到 g_pBuffer
//			if (test == 0)
//			{
//				for (int i = 0; i < sCInfo.CutSzie + g_iAddPoint; i++)
//				{
//					double dDataY = buffer[i];
//					if (sSInfo.Back == 1 && sTInfo.JBack == 0) {
//						dDataY = buffer[i] - bufferBkDataR[i];
//					}
//					if (sSInfo.Back == 0 && sTInfo.JBack == 1) {
//						dDataY = buffer[i] - bufferS1R[i];
//					}
//					if (sSInfo.Back == 1 && sTInfo.JBack == 1) {
//						dDataY = buffer[i] - sTInfo.JBkK*bufferS1R[i] - (1 - sTInfo.JBkK)*bufferBkDataR[i];
//					}
//					//dDataY = pDlg->ScanOffset(bufferXR[i], dDataY);//根据配置文件进行额外校正,后续需要再加上
//					g_pBuffer[i] = dDataY;
//				}
//			}
//			else
//			{
//				list_y = pDlg->m_Tchart.Series(nIndex).GetYValues();
//				for (int i = 0; i < sCInfo.CutSzie + g_iAddPoint; i++) {
//					double dDataY = buffer[i];
//					if (sSInfo.Back == 1 && sTInfo.JBack == 0) {
//						dDataY = buffer[i] - bufferBkDataR[i];
//					}
//					if (sSInfo.Back == 0 && sTInfo.JBack == 1) {
//						dDataY = buffer[i] - bufferS1R[i];
//					}
//					if (sSInfo.Back == 1 && sTInfo.JBack == 1) {
//						dDataY = buffer[i] - sTInfo.JBkK*bufferS1R[i] - (1 - sTInfo.JBkK)*bufferBkDataR[i];
//					}
//					//dDataY = pDlg->ScanOffset(bufferXR[i], dDataY);
//					// 进行累积平均
//					g_pBuffer[i] = (g_pBuffer[i] * test + dDataY) / (test + 1);
//				}
//			}
//			sDataShowInfo.color = color;
//			sDataShowInfo.seriesNum = nIndex;
//			sDataShowInfo.x = bufferXR;
//			sDataShowInfo.y = g_pBuffer;
//			sDataShowInfo.pointNum = sCInfo.CutSzie + g_iAddPoint;
//			::PostMessage(pDlg->m_hWnd, WM_SHOWDATA, 0, (LPARAM)(&sDataShowInfo));
//			if (!g_bOpenCamera && WaitForSingleObject(pDlg->m_hEvent, 5000) == WAIT_TIMEOUT)
//			{
//				goto nEnd;
//			}
//			test++;
//			if (test >= sTInfo.JAccNum)
//				m_bAutoRunning = FALSE;
//		}
//	}
//nEnd:
//	pDlg->SetLawerPower(CloseLaserPow, FALSE);
//	pDlg->AutoSaveFile(seriesName);
//
//	m_bAutoRunning = FALSE;
//	m_bAutoRunningStatue = FALSE;
//
//	delete[] buffer;
//	buffer = NULL;
//	delete[] bufferXR;
//	bufferXR = NULL;
//	
//	g_bGetTemp = false;
//	g_bSingleTestFinish = true;
//	return 0;
//}
DWORD WINAPI ThreadGetDataLA1(LPVOID lpParam)
{
	CINScanLiteDlg* pDlgF = (CINScanLiteDlg*)AfxGetApp()->GetMainWnd();
	pDlg->OnSetAutoRepaint(FALSE);

	int JAccNum = sTInfo.JAccNum;    //累计次数
	CString seriesName;

	//光谱横坐标的数组 类似于伟哥写的_X
	//动态分配的bufferXR用于存放截取后的横坐标数据
	//清空
	CValueList list_y;

	double bufferX[DataMax];
	double* bufferXR = new double[DataMax];
	memset(bufferX, 0, sizeof(double) * DataMax);
	memset(bufferXR, 0, sizeof(double) * DataMax);


	// buffer用于接收采集的光谱数据
	double * buffer = new double[DataMax];
	memset(buffer, 0, sizeof(unsigned short) * DataMax);

	double* bufferBkData = new double[DataMax];   // 原始暗噪声数据
	double* bufferBkDataR = new double[DataMax];    // 截取区域的暗噪声数据
	double *bufferS1 = new double[DataMax];
	double * bufferS1R = new double[DataMax];
	memset(bufferBkData, 0, sizeof(double) * DataMax);
	memset(bufferBkDataR, 0, sizeof(double) * DataMax);
	memset(bufferS1, 0, sizeof(double)*DataMax);
	memset(bufferS1R, 0, sizeof(double)*DataMax);

	m_ZolixCCD.XAxisData(bufferX, sSInfo.b, 4, sInfo.sDInfo.nGXPix);

	// 根据 XLeft、XRight 截取所需范围，把截取区域的横坐标数据复制到bufferXR中，供后续显示使用
	int count = 0;
	for (int i = 0; i < sInfo.sDInfo.nGXPix; i++)
	{
		if (bufferX[i] < sSInfo.XLeft)
		{
			sCInfo.CutF = i + 1;
		}
		if (bufferX[i] <= sSInfo.XRight)
		{
			count++;
		}
	}
	sCInfo.CutSzie = count - sCInfo.CutF;
	for (int i = 0; i < sCInfo.CutSzie; i++)
	{
		bufferXR[i] = bufferX[sCInfo.CutF + i];
	}

	if (m_bAutoRunning == 0 || g_bUSBDiscon)
		return 0;
	else
	{
		//// 设置积分时间 0.2s 并获取一次数据并清空
		//dfield_set_exposure(pDlgF->_handle, 0.2 * 1e3 * 1e3);
		////m_ZolixCCD.sete
		//m_ZolixCCD.DataAcquisition(buffer, sInfo.sDInfo.nGXPix);
		//memset(buffer, 0, sizeof(double) * DataMax);
	}
	m_ZolixCCD.SetExpTime(sTInfo.JTime);
	//判断是否勾选了扣除暗噪声
	if (sSInfo.Back == 1) {
		g_bSingleCollectFinish = false;
		pDlg->SetLawerPower(CloseLaserPow, FALSE);  //先关激光
		Sleep(100); // 等待激光器稳定关闭
		m_ZolixCCD.DataAcquisition(bufferBkData, sInfo.sDInfo.nGXPix);
		//提取有效范围的暗噪声数据
		m_ZolixCCD.GetRangeDataY(sCInfo.CutF, sCInfo.CutSzie, bufferBkData, bufferBkDataR);
		g_bSingleCollectFinish = true;
	}
	//判断是否勾选了扣除暗背景
	if (sTInfo.JBack == 1) {

		if (AfxMessageBox(pDlgF->LoadStringMeau(_T("ID_DATA_BACK")), MB_OKCANCEL) == IDCANCEL) {
			m_bAutoRunning = FALSE;
			goto nEnd;
		}
		g_bSingleCollectFinish = false;
		pDlg->SetLawerPower(CloseLaserPow, FALSE);  // 关闭激光器，确保无激光照射
		Sleep(100); // 等待激光器稳定关闭

		m_ZolixCCD.DataAcquisition(bufferS1, sInfo.sDInfo.nGXPix);
		// 提取有效范围的暗背景数据到 bufferBkDataR
		m_ZolixCCD.GetRangeDataY(sCInfo.CutF, sCInfo.CutSzie, bufferS1, bufferS1R);
		g_bSingleCollectFinish = true;

		if (AfxMessageBox(pDlgF->LoadStringMeau(_T("ID_DATA_SAMPLE")), MB_OKCANCEL) == IDCANCEL) {
			m_bAutoRunning = FALSE;
			goto nEnd;
		}

	}
	pDlg->SetLawerPower(sTInfo.JPower, FALSE);  //重新开启激光供后续采集

												//循环采集
	
	// 先创建好图表系列，只执行一次
	int nIndex = pDlg->m_Tchart.AddSeries(0);
	{
		int iTemp = nIndex;
		CString strTempName = g_strCurLineName + L"-";
		int iNameRepeat = 0;
		while (iTemp > 0)
		{
			CString strName = pDlg->m_Tchart.Series(iTemp - 1).GetTitle();
			if (strName.CompareNoCase(g_strCurLineName) == 0)
				iNameRepeat++;
			if (strName.Find(strTempName) >= 0)
				iNameRepeat++;
			pDlg->m_Tchart.Series(iTemp - 1).SetActive(FALSE);
			iTemp--;
		}
		nIndex = nIndex >= 0 ? nIndex : 0;
		int color = g_iRGB[nIndex % SERIES_MAX];
		pDlg->m_Tchart.Series(nIndex).SetColor(color);

		// 设置序列名称
		CString seriesName;
		if (g_strCurLineName.CompareNoCase(_T("NULL")) == 0)
		{
			seriesName = pDlg->m_Tchart.Series(nIndex).GetName();
		}
		else
		{
			seriesName = g_strCurLineName;
			if (iNameRepeat > 0)
			{
				CString strName;
				strName.Format(_T("-%d"), iNameRepeat);
				seriesName += strName;
			}
		}
		pDlg->m_Tchart.Series(nIndex).SetTitle(seriesName);
	}

	// 清零累积缓冲区，并初始化累积计数
	memset(g_pBuffer, 0, sizeof(double) * 3000);
	int test = 0; // 累计采集次数
	int color = pDlg->m_Tchart.Series(nIndex).GetColor();

	// 循环不断采集并更新图表，只有外部条件改变 m_bAutoRunning 时退出
	while (m_bAutoRunning)
	{

		g_bSingleCollectFinish = false;
		m_ZolixCCD.DataAcquisition(buffer, sInfo.sDInfo.nGXPix);
		g_bSingleCollectFinish = true;  // 一次采集结束

										// 若有额外补点需求，可在此处处理
		g_iAddPoint = 0;

		// 第一次采集时直接赋值，后续进行累积平均
		if (test == 0)
		{
			for (int i = 0; i < sCInfo.CutSzie + g_iAddPoint; i++)
			{
				double dDataY = buffer[i];
				if (sSInfo.Back == 1 && sTInfo.JBack == 0)
				{
					dDataY = buffer[i] - bufferBkDataR[i];
				}
				if (sSInfo.Back == 0 && sTInfo.JBack == 1)
				{
					dDataY = buffer[i] - bufferS1R[i];
				}
				if (sSInfo.Back == 1 && sTInfo.JBack == 1)
				{
					dDataY = buffer[i] - sTInfo.JBkK * bufferS1R[i] - (1 - sTInfo.JBkK) * bufferBkDataR[i];
				}
				// 如有需要额外校正，可调用：dDataY = pDlg->ScanOffset(bufferXR[i], dDataY);
				g_pBuffer[i] = dDataY;
			}
		}
		else
		{
			for (int i = 0; i < sCInfo.CutSzie + g_iAddPoint; i++)
			{
				double dDataY = buffer[i];
				if (sSInfo.Back == 1 && sTInfo.JBack == 0)
				{
					dDataY = buffer[i] - bufferBkDataR[i];
				}
				if (sSInfo.Back == 0 && sTInfo.JBack == 1)
				{
					dDataY = buffer[i] - bufferS1R[i];
				}
				if (sSInfo.Back == 1 && sTInfo.JBack == 1)
				{
					dDataY = buffer[i] - sTInfo.JBkK * bufferS1R[i] - (1 - sTInfo.JBkK) * bufferBkDataR[i];
				}
				//dDataY = pDlg->ScanOffset(bufferXR[i], dDataY);
				// 进行累积平均
				g_pBuffer[i] = (g_pBuffer[i] * test + dDataY) / (test + 1);
			}
		}

		// 更新显示数据结构
		sDataShowInfo.color = color;
		sDataShowInfo.seriesNum = nIndex;
		sDataShowInfo.x = bufferXR;
		sDataShowInfo.y = g_pBuffer;
		sDataShowInfo.pointNum = sCInfo.CutSzie + g_iAddPoint;
		::PostMessage(pDlg->m_hWnd, WM_SHOWDATA, 0, (LPARAM)(&sDataShowInfo));

		// 检查相机状态，超时则退出采集
		if (!g_bOpenCamera && WaitForSingleObject(pDlg->m_hEvent, 5000) == WAIT_TIMEOUT)
		{
			goto nEnd;
		}

		test++;  // 累计采集次数增加

				 // 可根据需要增加适当延时，避免采集过快
				 // Sleep(适当延时);
	}
	
nEnd:
	// 关闭激光器
	pDlg->SetLawerPower(CloseLaserPow, FALSE);
	// 自动保存
	pDlg->AutoSaveFile(seriesName);

	m_bAutoRunning = FALSE;
	m_bAutoRunningStatue = FALSE;

	// 释放内存
	delete[] buffer;
	buffer = NULL;
	delete[] bufferXR;
	bufferXR = NULL;

	g_bGetTemp = false;
	g_bSingleTestFinish = true;
	return 0;

}
DWORD WINAPI  ThreadGetData_D(LPVOID lpParam) {
	//========1.对象获取========  9.11--9.20
	CINScanLiteDlg *pDlgF = (CINScanLiteDlg*)AfxGetApp()->GetMainWnd();//获取主窗口指针
	pDlg->OnSetAutoRepaint(FALSE);                                     //OnShowData中设为TRUE


	//========2.准备数据缓存与x轴校正========



	//========3.读取或生成背景数据========



	//========4.开始自动测量的循环========



	//========5.结束测量并清理资源========


	return 0;
}

DWORD WINAPI  ThreadGetDataDA1(LPVOID lpParam) {
	CINScanLiteDlg* pDlgF = (CINScanLiteDlg*)AfxGetApp()->GetMainWnd();
	
	pDlg->OnSetAutoRepaint(FALSE);



	int test = 0;                    //采谱次数
	int JAccNum = sTInfo.JAccNum;    //累计次数
	CString seriesName;              

	//光谱横坐标的数组 类似于伟哥写的_X
	//动态分配的bufferXR用于存放截取后的横坐标数据
	//清空

	CValueList list_y;

	double bufferX[DataMax];
	double* bufferXR = new double[DataMax];
	memset(bufferX, 0, sizeof(double) * DataMax);     
	memset(bufferXR, 0, sizeof(double) * DataMax);

	
	// buffer用于接收采集的光谱数据
	double * buffer = new double[DataMax];
	memset(buffer, 0, sizeof(unsigned short) * DataMax);

	double* bufferBkData = new double[DataMax];   // 原始暗噪声数据
	double* bufferBkDataR = new double[DataMax];    // 截取区域的暗噪声数据
	double *bufferS1 = new double[DataMax];
	double * bufferS1R = new double[DataMax];
	memset(bufferBkData, 0, sizeof(double) * DataMax);
	memset(bufferBkDataR, 0, sizeof(double) * DataMax);
	memset(bufferS1, 0, sizeof(double)*DataMax);
	memset(bufferS1R, 0, sizeof(double)*DataMax);

	m_ZolixCCD.XAxisData(bufferX, sSInfo.b, 4, sInfo.sDInfo.nGXPix);

	// 根据 XLeft、XRight 截取所需范围，把截取区域的横坐标数据复制到bufferXR中，供后续显示使用
	int count = 0;
	for (int i = 0; i < sInfo.sDInfo.nGXPix; i++)
	{
		if (bufferX[i] < sSInfo.XLeft)
		{
			sCInfo.CutF = i + 1;
		}
		if (bufferX[i] <= sSInfo.XRight)
		{
			count++;
		}
	}
	sCInfo.CutSzie = count - sCInfo.CutF;
	for (int i = 0; i < sCInfo.CutSzie; i++)
	{
		bufferXR[i] = bufferX[sCInfo.CutF + i];
	}

	WriteToLog("17");

	if (m_bAutoRunning == 0 || g_bUSBDiscon)
		return 0;
	else
	{
		// 设置积分时间 0.2s 并获取一次数据并清空

		//m_ZolixCCD.SetExpTime(0.2);
		//m_ZolixCCD.DataAcquisition(buffer, sInfo.sDInfo.nGXPix);
		//memset(buffer, 0, sizeof(double) * DataMax);
	}
	m_ZolixCCD.SetExpTime(sTInfo.JTime);
	WriteToLog("18");

	//判断是否勾选了扣除暗噪声	
	if (sSInfo.Back == 1) {
		g_bSingleCollectFinish = false;
		pDlg->SetLawerPower(CloseLaserPow, FALSE);  //先关激光
		Sleep(100); // 等待激光器稳定关闭
		m_ZolixCCD.DataAcquisition(bufferBkData, sInfo.sDInfo.nGXPix);
		//提取有效范围的暗噪声数据
		m_ZolixCCD.GetRangeDataY(sCInfo.CutF, sCInfo.CutSzie, bufferBkData, bufferBkDataR);
		g_bSingleCollectFinish = true;
	}
	WriteToLog("19");



	//判断是否勾选了扣除暗背景
	if (sTInfo.JBack == 1) {
		
		if (AfxMessageBox(pDlgF->LoadStringMeau(_T("ID_DATA_BACK")), MB_OKCANCEL) == IDCANCEL) {
			m_bAutoRunning = FALSE;
			goto nEnd;
		}
		g_bSingleCollectFinish = false;
		pDlg->SetLawerPower(CloseLaserPow, FALSE);  // 关闭激光器，确保无激光照射
		Sleep(100); // 等待激光器稳定关闭

		m_ZolixCCD.DataAcquisition(bufferS1, sInfo.sDInfo.nGXPix);
		// 提取有效范围的暗背景数据到 bufferBkDataR
		m_ZolixCCD.GetRangeDataY(sCInfo.CutF, sCInfo.CutSzie, bufferS1, bufferS1R);
		g_bSingleCollectFinish = true;

		if (AfxMessageBox(pDlgF->LoadStringMeau(_T("ID_DATA_SAMPLE")), MB_OKCANCEL) == IDCANCEL) {
			m_bAutoRunning = FALSE;
			goto nEnd;
		}

	}
	WriteToLog("20");

	pDlg->SetLawerPower(sTInfo.JPower, FALSE);  //重新开启激光供后续采集

	WriteToLog("21");


	
	WriteToLog("22");


    //循环采集
	while (m_bAutoRunning)
	{
		// =====在图表中新增一个序列，处理重名问题，设置序列颜色和名称======
		int nIndex = pDlg->m_Tchart.AddSeries(0);
		int iTemp = nIndex;
		CString strTempName = g_strCurLineName + L"-";
		int iNameRepeat = 0;

		while (iTemp > 0)
		{
			CString strName = pDlg->m_Tchart.Series(iTemp - 1).GetTitle();
			if (strName.CompareNoCase(g_strCurLineName) == 0)
				iNameRepeat++;
			if (strName.Find(strTempName) >= 0)
				iNameRepeat++;

			pDlg->m_Tchart.Series(iTemp - 1).SetActive(FALSE);
			iTemp--;
		}

		nIndex = nIndex >= 0 ? nIndex : 0;
		int color = g_iRGB[nIndex % SERIES_MAX];
		pDlg->m_Tchart.Series(nIndex).SetColor(color);

		// 设置序列名称
		if (g_strCurLineName.CompareNoCase(_T("NULL")) == 0)
		{
			seriesName = pDlg->m_Tchart.Series(nIndex).GetName();
		}
		else
		{
			seriesName = g_strCurLineName;
			if (iNameRepeat > 0)
			{
				CString strName;
				strName.Format(_T("-%d"), iNameRepeat);
				seriesName += strName;
			}
		}
		pDlg->m_Tchart.Series(nIndex).SetTitle(seriesName);
		//== == == = 在图表中新增一个序列，处理重名问题，设置序列颜色和名称 == == == =

	    //== == == ==采集部分== == == ==
		memset(g_pBuffer, 0, sizeof(double) * 3000);
		//dfield_set_exposure(pDlgF->_handle, sTInfo.JTime * 1e3 * 1e3);//根据界面设定的积分时间采集
		//WriteToLog("我在这里设置积分时间");
		WriteToLog("23");


		while (m_bAutoRunning && test < sTInfo.JAccNum)
		{
			g_bSingleCollectFinish = false;

			m_ZolixCCD.DataAcquisition(buffer, sInfo.sDInfo.nGXPix);
				// 从设备中获取数据
			
			//镜像后续写

			g_bSingleCollectFinish = true;  //一次采集结束
											// 若有额外补点数量
			g_iAddPoint = 0;

			// 累积或直接赋值到 g_pBuffer
			if (test == 0)
			{
				for (int i = 0; i < sCInfo.CutSzie + g_iAddPoint; i++)
				{
					double dDataY = buffer[i];
					if (sSInfo.Back == 1 && sTInfo.JBack == 0) {
						dDataY = buffer[i] - bufferBkDataR[i];
					}
					if (sSInfo.Back == 0 && sTInfo.JBack == 1) {
						dDataY = buffer[i] - bufferS1R[i];
					}
					if (sSInfo.Back == 1 && sTInfo.JBack == 1) {
						dDataY = buffer[i] - sTInfo.JBkK*bufferS1R[i] - (1 - sTInfo.JBkK)*bufferBkDataR[i];
					}
					//dDataY = pDlg->ScanOffset(bufferXR[i], dDataY);//根据配置文件进行额外校正,后续需要再加上
					g_pBuffer[i] = dDataY;
				}
			}
			else
			{
				list_y = pDlg->m_Tchart.Series(nIndex).GetYValues();
				for (int i = 0; i < sCInfo.CutSzie + g_iAddPoint; i++) {
					double dDataY = buffer[i];
					if (sSInfo.Back == 1&&sTInfo.JBack ==0) {
						dDataY = buffer[i] - bufferBkDataR[i];
					}
					if (sSInfo.Back == 0 && sTInfo.JBack == 1) {
						dDataY = buffer[i]  - bufferS1R[i];
					}
					if (sSInfo.Back == 1 && sTInfo.JBack == 1) {
						dDataY = buffer[i] - sTInfo.JBkK*bufferS1R[i] - (1 - sTInfo.JBkK)*bufferBkDataR[i];
					}
					//dDataY = pDlg->ScanOffset(bufferXR[i], dDataY);
					// 进行累积平均
					g_pBuffer[i] = (g_pBuffer[i] * test + dDataY) / (test + 1);
				}
			}

			
				WriteToLog("24");

			sDataShowInfo.color = color;
			sDataShowInfo.seriesNum = nIndex;
			sDataShowInfo.x = bufferXR;
			sDataShowInfo.y = g_pBuffer;
		    sDataShowInfo.pointNum = sCInfo.CutSzie + g_iAddPoint;
			//sDataShowInfo.pointNum = sCInfo.CutSzie;
			::PostMessage(pDlg->m_hWnd, WM_SHOWDATA, 0, (LPARAM)(&sDataShowInfo));
			if (!g_bOpenCamera && WaitForSingleObject(pDlg->m_hEvent, 5000) == WAIT_TIMEOUT)
			{
				goto nEnd;
			}
			test++;
			if (test >= sTInfo.JAccNum)
				m_bAutoRunning = FALSE;
		}
	}
	WriteToLog("25");

nEnd:
	// 关闭激光器
	pDlg->SetLawerPower(CloseLaserPow, FALSE);
	// 自动保存
	pDlg->AutoSaveFile(seriesName);

	m_bAutoRunning = FALSE;
	m_bAutoRunningStatue = FALSE;

	// 释放内存
	delete[] buffer;
	buffer = NULL;
	delete[] bufferXR;
	bufferXR = NULL;

	g_bGetTemp = false;
	g_bSingleTestFinish = true;
	return 0;

}
//精检-单谱   1064  lcz new 23/05/12
DWORD WINAPI  ThreadGetDataD1064(LPVOID lpParam)
{
	g_DlgShowlog.WriteDebugLog(_T("ThreadGetDataD1064 Begin!"));
	CINScanLiteDlg *pDlgF = (CINScanLiteDlg*)AfxGetApp()->GetMainWnd();

	pDlg->OnSetAutoRepaint(FALSE);

	CValueList list_y;
	double bufferX[DataMax];
	double *bufferXR = new double[DataMax];
	memset(bufferX, 0, sizeof(double)*DataMax);
	memset(bufferXR, 0, sizeof(double)*DataMax);
	unsigned short* buffer = new unsigned short[DataMax];
	memset(buffer, 0, sizeof(unsigned short)*DataMax);
	//  分波数校正还是波长校正
	//if (g_bCorrNumLen)
	//{
		otoXAxisData(bufferX, sSInfo.b, 4, sInfo.sDInfo.nGXPix);
		


		int count = 0;
		//  按照所截取的波数范围进行设置
		for (int i = 0; i < sInfo.sDInfo.nGXPix; i++)
		{
			if (bufferX[i] < sSInfo.XLeft)
			{
				sCInfo.CutF = i + 1;
			}
			if (bufferX[i] <= sSInfo.XRight)
			{
				count++;
			}
		}
		sCInfo.CutSzie = count - sCInfo.CutF;
		//  截取后的波数范围值
		for (int i = 0; i < sCInfo.CutSzie; i++)
			bufferXR[i] = bufferX[sCInfo.CutF + i];

	if (m_bAutoRunning == 0 || g_bUSBDiscon)
		return 0;
	else
	{
		//m_ZolixCCD.SetExpTime(0.2);
		bool b = dfield_set_exposure(pDlgF->_handle, 0.2 * 1e3 * 1e3);// 0.2s = 200ms = 200 * 1e3 us
		//  根据像素点数读取数据，存放在buffer里
		//std::vector<unsigned short> _y = std::vector<unsigned short>(sInfo.sDInfo.nGXPix, 0);
		//dfield_get_data(pDlgF->_handle, &(_y[0]), sInfo.sDInfo.nGXPix);
		pDlg->show_dfield_get_data(pDlgF->_handle, buffer, sInfo.sDInfo.nGXPix, sCInfo.CutF);
		//m_ZolixCCD.DataAcquisition(buffer, sInfo.sDInfo.nGXPix);
		memset(buffer, 0, sizeof(unsigned short)*DataMax);
	}

	pDlg->SetLawerPower(sTInfo.JPower, FALSE);
	//XSleep(2000); // 激光器功率开启后增加到稳定值需要的时间



	int test = 0;
	int JAccNum = sTInfo.JAccNum;
	CString seriesName;
	while (m_bAutoRunning)
	{

		int nIndex = pDlg->m_Tchart.AddSeries(0);
		int iTemp = nIndex;
		CString strTempName = g_strCurLineName + L"-";
		int iNameRepeat = 0;
		while (iTemp > 0)
		{
			CString strName = pDlg->m_Tchart.Series(iTemp - 1).GetTitle();
			if (strName.CompareNoCase(g_strCurLineName) == 0)
				iNameRepeat++;
			if (strName.Find(strTempName) >= 0)
				iNameRepeat++;

			pDlg->m_Tchart.Series(iTemp - 1).SetActive(FALSE);
			iTemp--;
		}
		nIndex = nIndex >= 0 ? nIndex : 0;
		int color = g_iRGB[nIndex%SERIES_MAX];
		pDlg->m_Tchart.Series(nIndex).SetColor(color);
		//int color  = pDlg->m_Tchart.Series(nIndex).GetColor();

		if (g_strCurLineName.CompareNoCase("NULL") == 0)
			seriesName = pDlg->m_Tchart.Series(nIndex).GetName();
		else
		{
			seriesName = g_strCurLineName;
			if (iNameRepeat > 0)
			{
				CString strName;
				strName.Format(_T("-%d"), iNameRepeat);
				seriesName += strName;
			}
		}

		pDlg->m_Tchart.Series(nIndex).SetTitle(seriesName);

		memset(g_pBuffer, 0, sizeof(double) * 3000);

		bool b = dfield_set_exposure(pDlgF->_handle, sTInfo.JTime * 1e3 * 1e3);// 0.2s = 200ms = 200 * 1e3 us
		while (m_bAutoRunning&&test<sTInfo.JAccNum)
		{
			g_bSingleCollectFinish = false;
			dfield_capture(pDlgF->_handle);//  1064探测器单谱采集也得加上这行代码
			pDlg->show_dfield_get_data(pDlgF->_handle, buffer, sInfo.sDInfo.nGXPix, sCInfo.CutF);
			//  如果添加了镜像
			if (sTInfo.b1064ccdFlip)
			{
				double temp = 0.0;
				for (int i = 0; i < sInfo.sDInfo.nGXPix / 2; i++)
				{
					double temp = buffer[i];
					buffer[i] = buffer[sInfo.sDInfo.nGXPix - i - 1];
					buffer[sInfo.sDInfo.nGXPix - i - 1] = temp;
				}
			}
			g_bSingleCollectFinish = true;

			//  lcz new 08/10  取消根据CCD返回的数据来判断CCD是否异常断开
			// lcz new 06/07  如果测试的数据很多是0，或者数据是其他值稳定不变，则表示测试中断开
			//int j = 0, k = 0;
			//double firstNum = buffer[0];
			//for(int i = 0; i < sInfo.sDInfo.nGXPix; i++)
			//{
			//	if( (buffer[i] > 70000 || buffer[i] < -70000) &&(!g_bOpenCamera) )
			//	{
			//		g_bUSBDiscon = true;
			//		Sleep(100);
			//		//AfxMessageBox(TEXT("测试中（未开白光或已清除数据）数据超过±70000"));
			//		return 0;
			//	}
			//	else if(buffer[i] == firstNum && firstNum != 65535 &&(!g_bOpenCamera) )
			//	{
			//		k++;
			//	}
			//}
			//if( j > sInfo.sDInfo.nGXPix * 0.5 || k > sInfo.sDInfo.nGXPix * 0.5)
			//{
			//	g_bUSBDiscon = true;
			//	Sleep(100);
			//	//AfxMessageBox(TEXT("测试中（未开白光或已清除数据），数据很多为0或者保持不变"));
			//	return 0;
			//}

			//m_ZolixCCD.GetRangeDataY(sCInfo.CutF, sCInfo.CutSzie, buffer, bufferR);

			// lcz new 22/10/11
			g_iAddPoint = 0;
			//if (bufferXR[sCInfo.CutSzie - 1 - sCInfo.CutF] <= 3200 && sSInfo.XRight > bufferXR[sCInfo.CutSzie - 1] + 0.5 && sSInfo.iAddPointNum > 0)
			//{
			//	double tempGap = bufferXR[sCInfo.CutSzie - 1] - bufferXR[sCInfo.CutSzie - 2]; //  得到间距
			//																				  //  添加100 个数据点
			//	for (int i = 0; i < sSInfo.iAddPointNum; i++)
			//	{
			//		bufferXR[i + sCInfo.CutSzie] = bufferXR[sCInfo.CutSzie - 1] + tempGap * (i + 1); //  横坐标添加了100个点，范围到了3240 cm-1
			//		if (bufferXR[i + sCInfo.CutSzie] < sSInfo.XRight)
			//			g_iAddPoint++;
			//	}
			//	//  添加100个点的强度值
			//	//double * diffArray = new double[100];
			//	for (int i = 0; i < sSInfo.iAddPointNum; i++)
			//	{
			//		//double tempDiff = buffer[ sCInfo.CutSzie - sSInfo.iAddPointNum + i ] - buffer[sCInfo.CutSzie - sSInfo.iAddPointNum - 1 + i ];
			//		//double tempDiff = buffer[ i + 1] - buffer[ i ];
			//		double tempDiff = rand() % 50 - 25;
			//		bufferR[sCInfo.CutSzie + i] = bufferR[sCInfo.CutSzie - 1] + tempDiff;
			//		if (bufferR[sCInfo.CutSzie + i] < 0)// 如果强度降到0以下，则变为0
			//			bufferR[sCInfo.CutSzie + i] = 0;
			//	}
			//	//if(diffArray)
			//	//{
			//	//	delete[] diffArray;
			//	//	diffArray = NULL;
			//	//}
			//}//
			 /************************************************************************/

			if (test == 0)
			{
				for (int i = 0; i<sCInfo.CutSzie + g_iAddPoint; i++)// lcz new 22/10/11
				{
					double dDataY;
					//  lcz new 08/30  样品数据 - k*背景数据 - (1-k)*暗噪声数据      bufferR - sTInfo.JBkK * bufferS1 
					//dDataY=bufferR[i]-bufferBkDataR[i]-bufferS1[i]*sTInfo.JBkK;
					// lcz new 09/29
					//double dYdata = 0.0;
					//if (sTInfo.JBack == 1)
					//	dDataY = bufferR[i] - sTInfo.JBkK * bufferS1[i] - (1 - sTInfo.JBkK) * bufferBkDataR[i];
					//else
					//	dDataY = bufferR[i] - bufferBkDataR[i];

					////dDataY=bufferR[i] - sTInfo.JBkK * bufferS1[i] - (1- sTInfo.JBkK) * bufferBkDataR[i];
					//dDataY = pDlg->ScanOffset(bufferXR[i], dDataY);

					// lcz new 22/10/20
					//pDlg->m_Tchart.Series(nIndex).AddXY(bufferXR[i],dDataY,NULL,color);
					g_pBuffer[i] = buffer[i];
					// lc new 22/11/08  测试会否会出现负峰
					//if(dDataY < -1000)
					//{
					//	AfxMessageBox(TEXT("异常-1000"));
					//}
				}
			}
			else
			{

				list_y = pDlg->m_Tchart.Series(nIndex).GetYValues();
				for (int i = 0; i<sCInfo.CutSzie + g_iAddPoint; i++)
				{
					double dDataY;
					//  lcz new 08/30  样品数据 - k*背景数据 - (1-k)*暗噪声数据      bufferR - sTInfo.JBkK * bufferS1 
					//dDataY=bufferR[i]-bufferBkDataR[i]-bufferS1[i]*sTInfo.JBkK;
					// lcz new 09/29
					//if (sTInfo.JBack == 1)
					//	dDataY = (list_y.GetValue(i)*test + bufferR[i] - sTInfo.JBkK * bufferS1[i] - (1- sTInfo.JBkK) * bufferBkDataR[i]) / (test+1);
					//else
					//	dDataY = (list_y.GetValue(i)*test + bufferR[i] -  bufferBkDataR[i]) / (test+1); 
					/*if (sTInfo.JBack == 1)
						dDataY = (g_pBuffer[i] * test + bufferR[i] - sTInfo.JBkK * bufferS1[i] - (1 - sTInfo.JBkK) * bufferBkDataR[i]) / (test + 1);
					else
						dDataY = (g_pBuffer[i] * test + bufferR[i] - bufferBkDataR[i]) / (test + 1);*/

					//dDataY=(list_y.GetValue(i)*test + bufferR[i] - sTInfo.JBkK * bufferS1[i] - (1- sTInfo.JBkK) * bufferBkDataR[i]) / (test+1);
					//dDataY = pDlg->ScanOffset(bufferXR[i], dDataY);

					//if (list_y.GetCount()>=sCInfo.CutSzie + g_iAddPoint)
					//{
					//	list_y.SetValue(i,dDataY);
					//}
					g_pBuffer[i] = buffer[i];
					// lc new 22/11/08  测试会否会出现负峰
					//if(dDataY < -1000)
					//{
					//	AfxMessageBox(TEXT("异常-1000"));
					//}
				}
			}
			g_DlgShowlog.WriteDebugLog(_T("ThreadGetDataD1064 GetData!"));
			//  lcz new 22/10/20
			sDataShowInfo.color = color;
			sDataShowInfo.seriesNum = nIndex;
			sDataShowInfo.x = bufferXR;
			sDataShowInfo.y = g_pBuffer;
			sDataShowInfo.pointNum = sCInfo.CutSzie + g_iAddPoint;
			::PostMessage(pDlg->m_hWnd, WM_SHOWDATA, 0, (LPARAM)(&sDataShowInfo));
			if (g_bOpenCamera == false && WaitForSingleObject(pDlg->m_hEvent, 5000) == WAIT_TIMEOUT)
			{
				//return 0;
				goto nEnd;
			}
			g_DlgShowlog.WriteDebugLog(_T("ThreadGetDataD ShowData!"));
			//pDlg->m_Tchart.Series(nIndex).SetShowInLegend(true);
			//pDlg->OnSetAutoRepaint(TRUE);
			//pDlg->m_Tchart.Series(nIndex).SetShowInLegend(TRUE);// lcz new 06/22  添加图例
			/************************************************************************/
			test++;
			if (test >= sTInfo.JAccNum)
				m_bAutoRunning = FALSE;
		}
	}
nEnd:

	pDlg->SetLawerPower(CloseLaserPow, FALSE);
	pDlg->AutoSaveFile(seriesName);


	m_bAutoRunning = FALSE;
	m_bAutoRunningStatue = FALSE;
	delete[] buffer;
	buffer = NULL;
	delete[] bufferXR;
	bufferXR = NULL;

	g_bGetTemp = false;

	// lcz new 22/10/20  测试完后，g_bSingleTestFinish为真
	g_bSingleTestFinish = true;

	return 0;
}


//精检-单谱
DWORD WINAPI  ThreadGetDataD(LPVOID lpParam)
{
	CString logStr;
	logStr.Format(_T("单谱测量开始, 当前使能为%d(0高1低)"), sSInfo.HLMODE);
	WriteToLog_S((char*)(LPCTSTR)logStr);

	CINScanLiteDlg *pDlgF = (CINScanLiteDlg*)AfxGetApp()->GetMainWnd();
	//if (pDlg->m_Tchart.GetSeriesCount()>=15)
	//{
	//	pDlg->m_Tchart.RemoveAllSeries();
	//}

	//	pDlg->ReMoveSeries(0);
	//	pDlg->m_Tchart.AddSeries(0);
	pDlg->OnSetAutoRepaint(FALSE);

	CValueList list_y;
	double *buffer=new double[DataMax];
	double bufferX[DataMax];
	double *bufferBkData=new double[DataMax];//暗背景数据
	double *bufferR=new double[DataMax];
	double *bufferS1=new double[DataMax];
	double *bufferXR=new double[DataMax];
	double *bufferBkDataR=new double[DataMax];//暗背景数据
	memset(bufferBkData,0,sizeof(double)*DataMax);
	memset(buffer,0,sizeof(double)*DataMax);
	memset(bufferX,0,sizeof(double)*DataMax);
	memset(bufferBkDataR,0,sizeof(double)*DataMax);
	memset(bufferR,0,sizeof(double)*DataMax);
	memset(bufferXR,0,sizeof(double)*DataMax);
	memset(bufferS1,0,sizeof(double)*DataMax);


	if(g_bCorrNumLen)
	{
		if( sInfo.OminiType != 20)
			m_ZolixCCD.XAxisData(bufferX, sSInfo.b, 4, sInfo.sDInfo.nGXPix);
		else
			otoXAxisData(bufferX, sSInfo.b, 4, sInfo.sDInfo.nGXPix);

		int count = 0;
		//  按照所截取的波数范围进行设置
		for(int i = 0; i < sInfo.sDInfo.nGXPix; i++)
		{
			if (bufferX[i] < sSInfo.XLeft )
			{
				sCInfo.CutF = i + 1 ;

			}
			if (bufferX[i] <= sSInfo.XRight)
			{
				count++;
			}
		}
		sCInfo.CutSzie = count - sCInfo.CutF;
		//  截取后的波数范围值
		for(int i = 0; i < sCInfo.CutSzie; i++)
			bufferXR[i] = bufferX[sCInfo.CutF + i];
	}
	else
	{

		if( sInfo.OminiType != 20)
			m_ZolixCCD.XAxisData(bufferX, sSInfo.a, 4, sInfo.sDInfo.nGXPix);
		else
			otoXAxisData(bufferX, sSInfo.a, 4, sInfo.sDInfo.nGXPix);

		sCInfo.CutSzie=m_ZolixCCD.GetRangeDataX(sCInfo.NMCM, sInfo.sDInfo.nGXPix, sCInfo.Laser, sSInfo.XLeft,
			sSInfo.XRight,bufferX,bufferXR,&sCInfo.CutF);
	}

	//int offset = sCInfo.CutF;

		if( m_bAutoRunning == 0 || g_bUSBDiscon)
			return 0;
		else
		{
			m_ZolixCCD.SetExpTime(0.2);
			//  根据像素点数读取数据，存放在buffer里
			m_ZolixCCD.DataAcquisition(buffer, sInfo.sDInfo.nGXPix);
			memset(buffer,0,sizeof(double)*DataMax);// 数据清零
		}
	//	g_bOpenCamera = false;
	//}
	//  如果点击了扣除暗噪声，暗噪声数据bufferBkData=====
	if (sSInfo.Back==1)
	{
		g_bSingleCollectFinish = false;

		m_ZolixCCD.SetExpTime(sTInfo.JTime);
		m_ZolixCCD.DataAcquisition(bufferBkData,sInfo.sDInfo.nGXPix);

		g_bSingleCollectFinish = true;


		// lcz new 08/10 取消根据CCD返回的数据来判断是否USB断开
		// lcz new 06/07  如果测试的数据很多是0，或者数据是其他值稳定不变，则表示测试中断开
		//int j = 0, k = 0;
		//double firstNum = bufferBkData[0];
		//for(int i = 0; i < sInfo.sDInfo.nGXPix; i++)
		//{
		//	if( (bufferBkData[i] > 70000 || bufferBkData[i] < -70000) &&(!g_bOpenCamera) )
		//	{
		//		g_bUSBDiscon = true;
		//		Sleep(100);
		//		//AfxMessageBox(TEXT("测试中（未开白光或已清除数据）数据超过±70000"));
		//		return 0;
		//	}
		//	else if(bufferBkData[i] == firstNum && firstNum != 65535 &&(!g_bOpenCamera) )
		//	{
		//		k++;
		//	}
		//}
		//if( j > sInfo.sDInfo.nGXPix * 0.5 || k > sInfo.sDInfo.nGXPix * 0.9)
		//{
		//	g_bUSBDiscon = true;
		//	Sleep(100);
		//	//AfxMessageBox(TEXT("测试中（未开白光或已清除数据），数据很多为0或者保持不变"));
		//	return 0;
		//}

		m_ZolixCCD.GetRangeDataY(sCInfo.CutF,sCInfo.CutSzie,bufferBkData,bufferBkDataR);

		// lcz new 22/10/11
		if(bufferXR[sInfo.sDInfo.nGXPix - 1 - sCInfo.CutF] <= 3200 && sSInfo.XRight > bufferXR[sCInfo.CutSzie - 1] + 0.5 && sSInfo.iAddPointNum > 0)
		{
			srand((unsigned)time(NULL));//time()用系统时间初始化种。为rand()生成不同的随机种子。	
			for(int i = 0; i < sSInfo.iAddPointNum; i++)
			{
				//double tempDiff = bufferBkDataR[ sCInfo.CutSzie - sSInfo.iAddPointNum + i ] - bufferBkDataR[ sCInfo.CutSzie - sSInfo.iAddPointNum - 1 + i  ];
				//double tempDiff = bufferBkDataR[ i + 1] - bufferBkDataR[ i ];
				double tempDiff = rand()%50 - 25;
				bufferBkDataR[sCInfo.CutSzie + i ] = bufferBkDataR[sCInfo.CutSzie - 1] + tempDiff;
				if(bufferBkDataR[sCInfo.CutSzie + i ] < 0)
					bufferBkDataR[sCInfo.CutSzie + i ] = 0;
			}
		}//

	}
	// lcz new 08/30 如果勾选了背景扣除但没勾选扣除暗噪声
	if(sTInfo.JBack && !sSInfo.Back)
	{

		g_bSingleCollectFinish = false;
		m_ZolixCCD.SetExpTime(sTInfo.JTime);
		m_ZolixCCD.DataAcquisition(bufferBkData,sInfo.sDInfo.nGXPix);
		g_bSingleCollectFinish = true;
		m_ZolixCCD.GetRangeDataY(sCInfo.CutF,sCInfo.CutSzie,bufferBkData,bufferBkDataR);

		// lcz new 22/10/11
		if(bufferXR[sInfo.sDInfo.nGXPix - 1 - sCInfo.CutF] <= 3200 && sSInfo.XRight > bufferXR[sCInfo.CutSzie - 1] + 0.5 && sSInfo.iAddPointNum > 0)
		{
			srand((unsigned)time(NULL));//time()用系统时间初始化种。为rand()生成不同的随机种子。
			for(int i = 0; i < sSInfo.iAddPointNum; i++)
			{
				//double tempDiff = bufferBkDataR[ sCInfo.CutSzie - sSInfo.iAddPointNum + i ] - bufferBkDataR[ sCInfo.CutSzie - sSInfo.iAddPointNum - 1 + i  ];
				double tempDiff = rand()%50 - 25;
				bufferBkDataR[sCInfo.CutSzie + i ] = bufferBkDataR[sCInfo.CutSzie - 1] + tempDiff;
				if(bufferBkDataR[sCInfo.CutSzie + i ] < 0)
					bufferBkDataR[sCInfo.CutSzie + i ] = 0;
			}
		}//
	}


	pDlg->SetLawerPower(sTInfo.JPower,FALSE);
	//XSleep(2000); // 激光器功率开启后增加到稳定值需要的时间

	

	int test=0;
	int JAccNum=sTInfo.JAccNum;
	CString seriesName ;
	while(m_bAutoRunning)
	{
		
		//  如果勾选了背景扣除
		if(sTInfo.JBack==1&&m_bAutoRunning&&AfxMessageBox(pDlgF->LoadStringMeau(_T("ID_DATA_BACK")),MB_OKCANCEL)==IDCANCEL)
		{
			m_bAutoRunning=FALSE;
		}
		if(sTInfo.JBack==1&&m_bAutoRunning)
		{

			pDlg->GetBackData(bufferS1,bufferBkData);// bufferBkData是暗噪声数据，bufferS1是背景光谱

			// lcz new 22/10/11
			if(bufferXR[sInfo.sDInfo.nGXPix - 1 - sCInfo.CutF] <= 3200 && sSInfo.XRight > bufferXR[sCInfo.CutSzie - 1] + 0.5 && sSInfo.iAddPointNum > 0)
			{
				srand((unsigned)time(NULL));//time()用系统时间初始化种。为rand()生成不同的随机种子。
				for(int i = 0; i < sSInfo.iAddPointNum; i++)
				{
					//double tempDiff = bufferS1[ sCInfo.CutSzie - sSInfo.iAddPointNum + i ] - bufferS1[ sCInfo.CutSzie - sSInfo.iAddPointNum - 1 + i ];
					//double tempDiff = bufferS1[ i + 1] - bufferS1[ i ];
					double tempDiff = rand()%50 - 25;
					bufferS1[sCInfo.CutSzie + i ] = bufferS1[sCInfo.CutSzie - 1] + tempDiff;
					if(bufferS1[sCInfo.CutSzie + i ] < 0)
						bufferS1[sCInfo.CutSzie + i ] = 0;
				}
			}

			test=0;
		}

		if(sTInfo.JBack==1&&m_bAutoRunning&&AfxMessageBox(pDlgF->LoadStringMeau(_T("ID_DATA_SAMPLE")),MB_OKCANCEL)==IDCANCEL)
		{
			m_bAutoRunning=FALSE;
		}

		int nIndex = pDlg->m_Tchart.AddSeries(0);
		int iTemp = nIndex;
		CString strTempName = g_strCurLineName+L"-";
		int iNameRepeat = 0;
		while(iTemp > 0)
		{
			CString strName = pDlg->m_Tchart.Series(iTemp-1).GetTitle();
			if(strName.CompareNoCase(g_strCurLineName) == 0)
				iNameRepeat++;
			if(strName.Find(strTempName)>=0)
				iNameRepeat++;

			pDlg->m_Tchart.Series(iTemp-1).SetActive(FALSE);
			iTemp--;
		}
		nIndex = nIndex >=0 ? nIndex : 0;
		int color = g_iRGB[nIndex%SERIES_MAX];
		pDlg->m_Tchart.Series(nIndex).SetColor(color);
		//int color  = pDlg->m_Tchart.Series(nIndex).GetColor();

		if(g_strCurLineName.CompareNoCase("NULL") == 0)
			seriesName = pDlg->m_Tchart.Series(nIndex).GetName();
		else
		{
			seriesName = g_strCurLineName;
			if(iNameRepeat > 0)
			{
				CString strName;
				strName.Format(_T("-%d"), iNameRepeat);
				seriesName += strName;	
			}
		}

		pDlg->m_Tchart.Series(nIndex).SetTitle(seriesName);

		memset(g_pBuffer,0,sizeof(double)*3000);

		while(m_bAutoRunning&&test<sTInfo.JAccNum)
		{
			g_bSingleCollectFinish = false;

			m_ZolixCCD.SetExpTime(sTInfo.JTime);
			m_ZolixCCD.DataAcquisition(buffer,sInfo.sDInfo.nGXPix);

			g_bSingleCollectFinish = true;

			//  lcz new 08/10  取消根据CCD返回的数据来判断CCD是否异常断开
			// lcz new 06/07  如果测试的数据很多是0，或者数据是其他值稳定不变，则表示测试中断开
			//int j = 0, k = 0;
			//double firstNum = buffer[0];
			//for(int i = 0; i < sInfo.sDInfo.nGXPix; i++)
			//{
			//	if( (buffer[i] > 70000 || buffer[i] < -70000) &&(!g_bOpenCamera) )
			//	{
			//		g_bUSBDiscon = true;
			//		Sleep(100);
			//		//AfxMessageBox(TEXT("测试中（未开白光或已清除数据）数据超过±70000"));
			//		return 0;
			//	}
			//	else if(buffer[i] == firstNum && firstNum != 65535 &&(!g_bOpenCamera) )
			//	{
			//		k++;
			//	}
			//}
			//if( j > sInfo.sDInfo.nGXPix * 0.5 || k > sInfo.sDInfo.nGXPix * 0.5)
			//{
			//	g_bUSBDiscon = true;
			//	Sleep(100);
			//	//AfxMessageBox(TEXT("测试中（未开白光或已清除数据），数据很多为0或者保持不变"));
			//	return 0;
			//}

			m_ZolixCCD.GetRangeDataY(sCInfo.CutF,sCInfo.CutSzie,buffer,bufferR);

			// lcz new 22/10/11
			g_iAddPoint = 0;
			if(bufferXR[sCInfo.CutSzie - 1 - sCInfo.CutF] <= 3200 && sSInfo.XRight > bufferXR[sCInfo.CutSzie - 1] + 0.5 && sSInfo.iAddPointNum > 0)
			{
				double tempGap = bufferXR[sCInfo.CutSzie - 1] - bufferXR[sCInfo.CutSzie - 2]; //  得到间距
				//  添加100 个数据点
				for(int i = 0; i < sSInfo.iAddPointNum; i ++)
				{
					bufferXR[i + sCInfo.CutSzie] = bufferXR[sCInfo.CutSzie - 1] + tempGap * (i + 1); //  横坐标添加了100个点，范围到了3240 cm-1
					if(bufferXR[i + sCInfo.CutSzie] < sSInfo.XRight)
						g_iAddPoint++;
				}
				//  添加100个点的强度值
				//double * diffArray = new double[100];
				for(int i = 0; i < sSInfo.iAddPointNum; i++)
				{
					//double tempDiff = buffer[ sCInfo.CutSzie - sSInfo.iAddPointNum + i ] - buffer[sCInfo.CutSzie - sSInfo.iAddPointNum - 1 + i ];
					//double tempDiff = buffer[ i + 1] - buffer[ i ];
					double tempDiff = rand()%50 - 25;
					bufferR[sCInfo.CutSzie + i] = bufferR[sCInfo.CutSzie - 1] + tempDiff;
					if(bufferR[sCInfo.CutSzie + i ] < 0)// 如果强度降到0以下，则变为0
						bufferR[sCInfo.CutSzie + i ] = 0;
				}
				//if(diffArray)
				//{
				//	delete[] diffArray;
				//	diffArray = NULL;
				//}
			}//
			/************************************************************************/
			
			if (test==0)
			{
				for (int i=0;i<sCInfo.CutSzie+g_iAddPoint ;i++)// lcz new 22/10/11
				{
					double dDataY;
					//  lcz new 08/30  样品数据 - k*背景数据 - (1-k)*暗噪声数据      bufferR - sTInfo.JBkK * bufferS1 
					//dDataY=bufferR[i]-bufferBkDataR[i]-bufferS1[i]*sTInfo.JBkK;
					// lcz new 09/29
					//double dYdata = 0.0;
					if (sTInfo.JBack == 1)
						dDataY = bufferR[i] - sTInfo.JBkK * bufferS1[i] - (1- sTInfo.JBkK) * bufferBkDataR[i];
					else
						dDataY = bufferR[i] -  bufferBkDataR[i];
				/*	if (i == 192) {
						Sleep(1);
					}*/
					//dDataY=bufferR[i] - sTInfo.JBkK * bufferS1[i] - (1- sTInfo.JBkK) * bufferBkDataR[i];
					dDataY=pDlg->ScanOffset(bufferXR[i],dDataY);

					// lcz new 22/10/20
					//pDlg->m_Tchart.Series(nIndex).AddXY(bufferXR[i],dDataY,NULL,color);
					g_pBuffer[i] = dDataY;
					// lc new 22/11/08  测试会否会出现负峰
					//if(dDataY < -1000)
					//{
					//	AfxMessageBox(TEXT("异常-1000"));
					//}
				}
			}
			else
			{

				list_y=pDlg->m_Tchart.Series(nIndex).GetYValues();
				for (int i=0;i<sCInfo.CutSzie+g_iAddPoint;i++)
				{
					double dDataY;
					//  lcz new 08/30  样品数据 - k*背景数据 - (1-k)*暗噪声数据      bufferR - sTInfo.JBkK * bufferS1 
					//dDataY=bufferR[i]-bufferBkDataR[i]-bufferS1[i]*sTInfo.JBkK;
					// lcz new 09/29
					//if (sTInfo.JBack == 1)
					//	dDataY = (list_y.GetValue(i)*test + bufferR[i] - sTInfo.JBkK * bufferS1[i] - (1- sTInfo.JBkK) * bufferBkDataR[i]) / (test+1);
					//else
					//	dDataY = (list_y.GetValue(i)*test + bufferR[i] -  bufferBkDataR[i]) / (test+1); 
					if (sTInfo.JBack == 1)
						dDataY = (g_pBuffer[i] *test + bufferR[i] - sTInfo.JBkK * bufferS1[i] - (1- sTInfo.JBkK) * bufferBkDataR[i]) / (test+1);
					else
						dDataY = (g_pBuffer[i] *test + bufferR[i] -  bufferBkDataR[i]) / (test+1); 

					//dDataY=(list_y.GetValue(i)*test + bufferR[i] - sTInfo.JBkK * bufferS1[i] - (1- sTInfo.JBkK) * bufferBkDataR[i]) / (test+1);
					
					
					dDataY=pDlg->ScanOffset(bufferXR[i],dDataY);

					//if (list_y.GetCount()>=sCInfo.CutSzie + g_iAddPoint)
					//{
					//	list_y.SetValue(i,dDataY);
					//}
					g_pBuffer[i] = dDataY;
					// lc new 22/11/08  测试会否会出现负峰
					//if(dDataY < -1000)
					//{
					//	AfxMessageBox(TEXT("异常-1000"));
					//}
				}
			}
			g_DlgShowlog.WriteDebugLog(_T("ThreadGetDataD GetData!"));
			//  lcz new 22/10/20
			sDataShowInfo.color = color;
			sDataShowInfo.seriesNum = nIndex;
			sDataShowInfo.x = bufferXR;
			sDataShowInfo.y = g_pBuffer;
			sDataShowInfo.pointNum = sCInfo.CutSzie+g_iAddPoint;
			::PostMessage(pDlg->m_hWnd,WM_SHOWDATA, 0, (LPARAM)(&sDataShowInfo));
			if (g_bOpenCamera==false&&WaitForSingleObject(pDlg->m_hEvent, 5000) == WAIT_TIMEOUT)
            {
				//return 0;
				goto nEnd;
            }
			g_DlgShowlog.WriteDebugLog(_T("ThreadGetDataD ShowData!"));
			//pDlg->m_Tchart.Series(nIndex).SetShowInLegend(true);
			//pDlg->OnSetAutoRepaint(TRUE);
			//pDlg->m_Tchart.Series(nIndex).SetShowInLegend(TRUE);// lcz new 06/22  添加图例
			/************************************************************************/
			test++;
			if(test>=sTInfo.JAccNum)
				m_bAutoRunning=FALSE;
		}
	}
nEnd:
	WriteToLog_S("单谱测量结束");

	pDlg->SetLawerPower(CloseLaserPow,FALSE);
	pDlg->AutoSaveFile(seriesName);

	//偏移量
	CString strOffset;
	strOffset.Format(_T("%d"), sCInfo.CutF);
	CString iniPath = sInfo.Path + _T("\\ZolixCCD.ini");

	if (sInfo.OminiType == 50) {
		WritePrivateProfileString(_T("A1"), _T("dataOffset"), strOffset, iniPath);
	}
	if (sInfo.OminiType == 10) {
		WritePrivateProfileString(_T("A"), _T("dataOffset"), strOffset, iniPath);
	}

	m_bAutoRunning=FALSE;
	m_bAutoRunningStatue=FALSE;
	delete [] buffer;
	buffer = NULL;
	//delete [] bufferX;
	delete [] bufferBkData;
	bufferBkData = NULL;
	delete [] bufferR;
	bufferR = NULL;
	delete [] bufferXR;
	bufferXR = NULL;
	delete [] bufferBkDataR;
	bufferBkDataR = NULL;
	delete [] bufferS1;
	bufferS1 = NULL;


	g_bGetTemp = false;

	// lcz new 22/10/20  测试完后，g_bSingleTestFinish为真
	g_bSingleTestFinish = true;

	return 0;
}



//  精检——连续
DWORD WINAPI  ThreadGetDataL(LPVOID lpParam)
{
	CString logStr;
	logStr.Format(_T("连续测量开始, 当前使能为%d(0高1低)"), sSInfo.HLMODE);
	WriteToLog_S((char*)(LPCTSTR)logStr);

	g_bNewTest  = true;
	g_bStopTest = false;
	//while(true)
	//{
	//    int a = 0;
	//}
	g_DlgShowlog.WriteDebugLog(_T("ThreadGetDataL Begin!"));
	CINScanLiteDlg *pDlgF = (CINScanLiteDlg*)AfxGetApp()->GetMainWnd();
	//if (pDlg->m_Tchart.GetSeriesCount()>=15)
	//{
	//	pDlg->m_Tchart.RemoveAllSeries();
	//}

	//	pDlg->ReMoveSeries(0);
	pDlg->OnSetAutoRepaint(FALSE);
	//	pDlg->m_Tchart.AddSeries(0);
	//m_ZolixCCD.setStarted(true);

	CValueList list_y;
	double *buffer=new double[DataMax]; //  读取的光谱数据（强度值）
	double *bufferX=new double[DataMax]; //  根据校正系数计算得到的波长值（目前是波长，535-635 nm)
	double *bufferBkData=new double[DataMax];// 暗噪声数据
	double *bufferR=new double[DataMax];   //  截取的 光谱数据（强度值）
	double *bufferS1=new double[DataMax]; //  截取的背景 数据
	double *bufferXR=new double[DataMax];//  截取的X轴数据，坐标为波数，201.3cm-1起
	double *bufferBkDataR=new double[DataMax];//截取的暗噪声数据
	memset(bufferBkData,0,sizeof(double)*DataMax);
	memset(buffer,0,sizeof(double)*DataMax);
	memset(bufferX,0,sizeof(double)*DataMax);
	memset(bufferBkDataR,0,sizeof(double)*DataMax);
	memset(bufferR,0,sizeof(double)*DataMax);
	memset(bufferXR,0,sizeof(double)*DataMax);
	memset(bufferS1,0,sizeof(double)*DataMax);
	//  分波数校正还是波长校正
	sCInfo.CutF = 0;
	if(g_bCorrNumLen)// 如果是波数校正
	{
		if( sInfo.OminiType != 20)
			m_ZolixCCD.XAxisData(bufferX, sSInfo.b, 4, sInfo.sDInfo.nGXPix);
		//  OTO的CCD调用该方法有误
		else
			otoXAxisData(bufferX, sSInfo.b, 4, sInfo.sDInfo.nGXPix);

		int count = 0;
		//  按照所截取的波数范围进行设置
		for(int i = 0; i < sInfo.sDInfo.nGXPix; i++)
		{
			if (bufferX[i] < sSInfo.XLeft )
			{
				sCInfo.CutF = i + 1 ;
			}
			if (bufferX[i] <= sSInfo.XRight)
			{
				count++;
			}
		}
		sCInfo.CutSzie = count - sCInfo.CutF;
		//  截取后的波数范围值
		for(int i = 0; i < sCInfo.CutSzie; i++)
			bufferXR[i] = bufferX[sCInfo.CutF + i];
	}
	else
	{
		//  bufferX是根据校正系数计算得到的波长值（目前是波长），长度与像素点个数一致
		//  sCInfo.NMCM 截取范围是按波长还是波数计算  0-波长 1-波数
		//  bufferX是从535.22nm到653.55nm，
		//  bufferXR是波数，从112.03 cm-1 到 3494.69 cm-1
		//   根据激光波长将其转为波数后，再根据波数范围进行截取，CutF是截取的起点
		if( sInfo.OminiType != 20)
			m_ZolixCCD.XAxisData(bufferX, sSInfo.a, 4, sInfo.sDInfo.nGXPix);
		else
			otoXAxisData(bufferX, sSInfo.a, 4, sInfo.sDInfo.nGXPix);
		sCInfo.CutSzie=m_ZolixCCD.GetRangeDataX(sCInfo.NMCM, sInfo.sDInfo.nGXPix, sCInfo.Laser, sSInfo.XLeft,
			sSInfo.XRight, bufferX, bufferXR, &sCInfo.CutF);
	}
	int offset = sCInfo.CutF;
	// lcz new 05/24  若在采集之前，相机打开过，则先将CCD数据清空 暂时定时0.015s
	// lcz new 06/20 改为每次才几千都进行清空
	//if(g_bOpenCamera)
	//{
	//	Sleep(500);
		if( m_bAutoRunning == 0 || g_bUSBDiscon)
			return 0;
		else
		{
			m_ZolixCCD.SetExpTime(0.2);
			//  根据像素点数读取数据，存放在buffer里
			m_ZolixCCD.DataAcquisition(buffer, sInfo.sDInfo.nGXPix);
			memset(buffer,0,sizeof(double)*DataMax);// 数据清零
		}
	//	g_bOpenCamera = false;
	//}

	if (sSInfo.Back==1)
	{
		g_bSingleCollectFinish = false;

		m_ZolixCCD.SetExpTime(sTInfo.JTime);
		m_ZolixCCD.DataAcquisition(bufferBkData, sInfo.sDInfo.nGXPix);

		g_bSingleCollectFinish = true;

		//  bufferBkData是包含暗噪声的数据，bufferBkDataR是包含暗噪声的截取的数据；
		m_ZolixCCD.GetRangeDataY(sCInfo.CutF, sCInfo.CutSzie, bufferBkData, bufferBkDataR);

		// lcz new 22/10/11
		if(bufferXR[sInfo.sDInfo.nGXPix - 1 - sCInfo.CutF] <= 3200 && sSInfo.XRight > bufferXR[sCInfo.CutSzie - 1] + 0.5 && sSInfo.iAddPointNum > 0)
		{
			for(int i = 0; i < sSInfo.iAddPointNum; i++)
			{
				//double tempDiff = bufferBkDataR[ sInfo.sDInfo.nGXPix - sSInfo.iAddPointNum + i   - sCInfo.CutF] - bufferBkDataR[ sInfo.sDInfo.nGXPix - sSInfo.iAddPointNum - 1 + i  - sCInfo.CutF];
				//double tempDiff = bufferBkDataR[ i + 1] - bufferBkDataR[ i ];
				double tempDiff = rand()%50 - 25;
				bufferBkDataR[sInfo.sDInfo.nGXPix + i - sCInfo.CutF] = bufferBkDataR[sInfo.sDInfo.nGXPix - 1 - sCInfo.CutF] + tempDiff;
				if(bufferBkDataR[sInfo.sDInfo.nGXPix + i - sCInfo.CutF] < 0)
					bufferBkDataR[sInfo.sDInfo.nGXPix + i - sCInfo.CutF] = 0;
			}
		}//
	}

	// lcz new 08/30 如果勾选了背景扣除但没勾选扣除暗噪声
	if(sTInfo.JBack && !sSInfo.Back)
	{
		
		g_bSingleCollectFinish = false;
		m_ZolixCCD.SetExpTime(sTInfo.JTime);
		m_ZolixCCD.DataAcquisition(bufferBkData,sInfo.sDInfo.nGXPix);
		g_bSingleCollectFinish = true;
		m_ZolixCCD.GetRangeDataY(sCInfo.CutF,sCInfo.CutSzie,bufferBkData,bufferBkDataR);

		// lcz new 22/10/11
		if(bufferXR[sInfo.sDInfo.nGXPix - 1 - sCInfo.CutF] <= 3200 && sSInfo.XRight > bufferXR[sCInfo.CutSzie - 1] + 0.5 && sSInfo.iAddPointNum > 0)
		{
			for(int i = 0; i < sSInfo.iAddPointNum; i++)
			{
				//double tempDiff = bufferBkDataR[ sInfo.sDInfo.nGXPix - sSInfo.iAddPointNum + i   - sCInfo.CutF] - bufferBkDataR[ sInfo.sDInfo.nGXPix - sSInfo.iAddPointNum - 1 + i  - sCInfo.CutF];
				//double tempDiff = bufferBkDataR[ i + 1] - bufferBkDataR[ i ];
				double tempDiff = rand()%50 - 25;
				bufferBkDataR[sInfo.sDInfo.nGXPix + i - sCInfo.CutF] = bufferBkDataR[sInfo.sDInfo.nGXPix - 1 - sCInfo.CutF] + tempDiff;
				if(bufferBkDataR[sInfo.sDInfo.nGXPix + i - sCInfo.CutF] < 0)
					bufferBkDataR[sInfo.sDInfo.nGXPix + i - sCInfo.CutF] = 0;
			}
		}//
	}

	//  点击连续，默认激光器未打开
	pDlg->SetLawerPower(sTInfo.JPower,TRUE);

	int test=-1;
	int nIndex=0;
	int color=0;
	int JAccNum=sTInfo.JAccNum;
	CString seriesName;
	while(m_bAutoRunning)
	{

		//  如果是不校正；扣除背景（背景+样品）；正在运行；取消放置背景，那么则停止运行
		if(sSInfo.CorrMode==0&&sTInfo.JBack==1&&m_bAutoRunning&&AfxMessageBox(pDlgF->LoadStringMeau(_T("ID_DATA_BACK")),MB_OKCANCEL)==IDCANCEL)
		{
			m_bAutoRunning=FALSE;
		}
		//  如果是不校正；扣除背景；正在运行，则先获取背景的光谱数据
		if(sSInfo.CorrMode==0&&sTInfo.JBack==1&&m_bAutoRunning)
		{
			//  获取背景数据，bufferS1是截取的背景数据，bufferBkData是0
			pDlg->GetBackData(bufferS1,bufferBkData);

			// lcz new 22/10/11
			if(bufferXR[sInfo.sDInfo.nGXPix - 1 - sCInfo.CutF] <= 3200 && sSInfo.XRight > bufferXR[sCInfo.CutSzie - 1] + 0.5 && sSInfo.iAddPointNum > 0)
			{
				for(int i = 0; i < sSInfo.iAddPointNum; i++)
				{
					//double tempDiff = bufferS1[ sCInfo.CutSzie - sSInfo.iAddPointNum + i ] - bufferS1[ sCInfo.CutSzie - sSInfo.iAddPointNum - 1 + i ];
					//double tempDiff = bufferS1[ i + 1] - bufferS1[ i ];
					double tempDiff = rand()%50 - 25;	
					bufferS1[sCInfo.CutSzie + i ] = bufferS1[sCInfo.CutSzie - 1] + tempDiff;
					if(bufferS1[sCInfo.CutSzie + i ] < 0)
						bufferS1[sCInfo.CutSzie + i ] = 0;
				}
			}//
		}
		//  如果是不校正；扣除背景（背景+样品）；正在运行；取消放置样品，那么则停止运行
		if(sSInfo.CorrMode==0&&sTInfo.JBack==1&&m_bAutoRunning&&AfxMessageBox(pDlgF->LoadStringMeau(_T("ID_DATA_SAMPLE")),MB_OKCANCEL)==IDCANCEL)
		{
			m_bAutoRunning=FALSE;
		}
		//  下面这行sleep(800)很关键，如果断开连接，m_bAutoRunning为false，但是不等待（阻塞）下，状态没来得及更新，CCD去读取数据会出错
		Sleep(800);
		while(m_bAutoRunning)
		{
			if (g_bClose)
			{
				break;
			}
			//Sleep(sTInfo.JTime*1000);
			
			if( m_bAutoRunning == 0)
				return 0;
			else
			{
				if(g_bCameraOpen2Close) // 如果测试中相机从打开到关闭，下次获取数据前需要清空次数据
				{
					g_bCameraOpen2Close = false;
					m_ZolixCCD.SetExpTime(0.015);
					m_ZolixCCD.DataAcquisition(buffer,sInfo.sDInfo.nGXPix);
					memset(buffer,0,sizeof(double)*DataMax);// 数据清零
				}

				//memset(buffer, 0, sizeof(double) * 3000);
				g_bSingleCollectFinish = false;

				m_ZolixCCD.SetExpTime(sTInfo.JTime);
				//  根据像素点数读取数据，存放在buffer里-----这行获取的数据出现了问题
				m_ZolixCCD.DataAcquisition(buffer, sInfo.sDInfo.nGXPix);
				
				g_bSingleCollectFinish = true;



			}
			//  对拉曼强度数据进行截取，截取后的数据存放在bufferR中
			//sCInfo.CutF = 0;
			//sCInfo.CutSzie = sInfo.sDInfo.nGXPix; 
			m_ZolixCCD.GetRangeDataY(sCInfo.CutF, sCInfo.CutSzie, buffer, bufferR);
			g_DlgShowlog.WriteDebugLog(_T("Get Data Once!"));

			// lcz new 22/10/11
			g_iAddPoint = 0;
			if(bufferXR[sInfo.sDInfo.nGXPix - 1 - sCInfo.CutF] <= 3200 && sSInfo.XRight > bufferXR[sCInfo.CutSzie - 1] + 0.5 && sSInfo.iAddPointNum > 0)
			{
				double tempGap = bufferXR[sCInfo.CutSzie -  1 ] - bufferXR[sCInfo.CutSzie - 2 ]; //  寰楀埌闂磋窛
				//  娣诲姞100 涓暟鎹偣
				for(int i = 0; i < sSInfo.iAddPointNum; i ++)
				{
					bufferXR[i + sCInfo.CutSzie] = bufferXR[sCInfo.CutSzie - 1] + tempGap * (i + 1); //  妯潗鏍囨坊鍔犱簡100涓偣锛岃寖鍥村埌浜?240 cm-1
					if(bufferXR[i + sCInfo.CutSzie] < sSInfo.XRight)
						g_iAddPoint++;
				}
				//  娣诲姞100涓偣鐨勫己搴﹀�?
				//double * diffArray = new double[100];
				for(int i = 0; i < sSInfo.iAddPointNum; i++)
				{
					//double tempDiff = (buffer[ sCInfo.CutSzie - sSInfo.iAddPointNum + i ] - buffer[ sCInfo.CutSzie - sSInfo.iAddPointNum - 1 + i ])/2;
					//double tempDiff = buffer[ i + 1] - buffer[ i ];
					//double tempDiff1 =  (buffer[ i + 1 ] - buffer[ i ])/2;
					//if(abs(tempDiff) > 100 || buffer[ sCInfo.CutSzie - sSInfo.iAddPointNum + i ] > 500)
					//	tempDiff = tempDiff1;
					double tempDiff = rand()%50 - 25;
					bufferR[sCInfo.CutSzie + i ] = bufferR[sCInfo.CutSzie- 1] + tempDiff;
					if(bufferR[sCInfo.CutSzie + i ] < 0)// 濡傛灉寮哄害闄嶅埌0浠ヤ笅锛屽垯鍙樹负0
						bufferR[sCInfo.CutSzie + i ] = 0;
				}
				//if(diffArray)
				//{
				//	delete[] diffArray;
				//	diffArray = NULL;
				//}
			}//
			
			if (test==-1)
			{
				nIndex = pDlg->m_Tchart.AddSeries(0);//  添加第一条线
				int iTemp = nIndex;
				while(iTemp > 0)
				{
					pDlg->m_Tchart.Series(iTemp-1).SetActive(FALSE);
					iTemp--;
				}
				//color = pDlg->m_Tchart.Series(nIndex).GetColor();
				//  设置第 nIndex 条曲线的名称、标题
				nIndex = nIndex >=0 ? nIndex : 0;
				color = g_iRGB[nIndex%SERIES_MAX];
				pDlg->m_Tchart.Series(nIndex).SetColor(color);
				seriesName = pDlg->m_Tchart.Series(nIndex).GetName();
				pDlg->m_Tchart.Series(nIndex).SetTitle(seriesName);
				pDlg->m_Tchart.Series(nIndex).SetShowInLegend(true);
			}
			test++;

			memset(g_pBuffer,0,sizeof(double)*3000);
			if (test==0)
			{
				int i;
				color = pDlg->m_Tchart.Series(nIndex).GetColor();
				for (i=0;i<sCInfo.CutSzie + g_iAddPoint;i++)// lcz new 22/10/11
				{
					//  截取后的强度数据；包含暗噪声（如果点了去除暗噪声，不然是0）的截取后的数据；背景数据；JBkK倍的背景数据（如果点了扣除背景，不然是0）
					//  lcz new 08/30  样品数据 - k*背景数据 - (1-k)*暗噪声数据      bufferR - sTInfo.JBkK * bufferS1 
					//double dYdata=bufferR[i]-bufferBkDataR[i]-bufferS1[i]*sTInfo.JBkK;
					double dYdata = 0.0;
					if (sTInfo.JBack == 1)
						dYdata = bufferR[i] - sTInfo.JBkK * bufferS1[i] - (1- sTInfo.JBkK) * bufferBkDataR[i];
					else
						dYdata = bufferR[i] -  bufferBkDataR[i];

					dYdata=pDlg->ScanOffset(bufferXR[i],dYdata);

					//  lcz new 22/10/21
					//color = pDlg->m_Tchart.Series(nIndex).GetColor();
					//pDlg->m_Tchart.Series(nIndex).AddXY(bufferXR[i],dYdata,NULL,color);
					g_pBuffer[i] = dYdata;
				}
			}
			else
			{
				list_y=pDlg->m_Tchart.Series(nIndex).GetYValues();
				int i;
				for (i=0;i<sCInfo.CutSzie + g_iAddPoint;i++)
				{
					//  lcz new 08/30  样品数据 - k*背景数据 - (1-k)*暗噪声数据      bufferR - sTInfo.JBkK * bufferS1 
					//double dDataY = (bufferR[i]-bufferBkDataR[i]-bufferS1[i]*sTInfo.JBkK);
					double dDataY = 0.0;
					if (sTInfo.JBack == 1)
						dDataY = bufferR[i] - sTInfo.JBkK * bufferS1[i] - (1- sTInfo.JBkK) * bufferBkDataR[i];
					else
						dDataY = bufferR[i] -  bufferBkDataR[i];
					//double dDataY = bufferR[i] - sTInfo.JBkK * bufferS1[i] - (1- sTInfo.JBkK) * bufferBkDataR[i];
					dDataY=pDlg->ScanOffset(bufferXR[i],dDataY);

					//  lcz new 22/10/21
					//if (list_y.GetCount()>=sCInfo.CutSzie + g_iAddPoint)
					//{
					//	list_y.SetValue(i,dDataY);
					//}

					//  lcz new 22/10/21
					g_pBuffer[i] = dDataY;
				}
			}

			// lcz new 22/10/20  谱图刷新
			pDlg->OnSetAutoRepaint(TRUE);//  必须加上这行
			sDataShowInfo.color = color;
			sDataShowInfo.seriesNum = nIndex;
			sDataShowInfo.x = bufferXR;
			sDataShowInfo.y = g_pBuffer;
			sDataShowInfo.pointNum = sCInfo.CutSzie+g_iAddPoint;
			::PostMessage(pDlg->m_hWnd,WM_SHOWDATA, 0, (LPARAM)(&sDataShowInfo));
			if (g_bOpenCamera == false && WaitForSingleObject(pDlg->m_hEvent, 5000) == WAIT_TIMEOUT)
            {
				goto nEnd;
            }


			if (sSInfo.CorrMode==1)
			{
				m_bAutoRunning=FALSE;
				//pDlgF->m_DlgAdv.m_DlgLaCorr.SetButtonS(TRUE);
				sSInfo.CorrMode=0;
			}
		}
	}
	g_bStopTest = true;

nEnd :
	WriteToLog_S("连续测量结束");
	CString strOffset;
	strOffset.Format(_T("%d"), sCInfo.CutF);
	CString iniPath = sInfo.Path + _T("\\ZolixCCD.ini");

	if (sInfo.OminiType == 50) {
		WritePrivateProfileString(_T("A1"), _T("dataOffset"), strOffset, iniPath);
	}
	if (sInfo.OminiType == 10) {
		WritePrivateProfileString(_T("A"), _T("dataOffset"), strOffset, iniPath);
	}

	//g_bOpenCamera = false;
	pDlg->SetLawerPower(CloseLaserPow,FALSE);//  先关激光再保存文件
	g_DlgShowlog.WriteDebugLog(_T("ThreadGetDataL Over!!"));
	pDlg->AutoSaveFile(seriesName);
	m_bAutoRunning=FALSE;
	m_bAutoRunningStatue=FALSE;

	delete [] buffer;
	buffer = NULL;
	delete [] bufferX;
	bufferX = NULL;
	delete [] bufferBkData;//暗背景数据
	bufferBkData = NULL;
	delete [] bufferR;
	bufferR = NULL;
	delete [] bufferS1;
	bufferS1 = NULL;
	delete [] bufferXR;
	bufferXR = NULL;
	delete [] bufferBkDataR;
	bufferBkDataR = NULL;

	g_bGetTemp = false;

	return 0;

	
}

//间隔
DWORD WINAPI  ThreadGetDataIN(LPVOID lpParam)
{
	CINScanLiteDlg *pDlgF = (CINScanLiteDlg*)AfxGetApp()->GetMainWnd();

	CValueList list_y;
	double *buffer=new double[DataMax];
	double *bufferX=new double[DataMax];
	double *bufferBkData=new double[DataMax];//暗背景数据
	double *bufferR=new double[DataMax];
	double *bufferXRs=new double[DataMax];
	double *bufferYRs=new double[DataMax];
	double *bufferXR=new double[DataMax];
	double *bufferYR=new double[DataMax];
	double *bufferBkDataR=new double[DataMax];//暗背景数据

	// lcz new 05/24  若在采集之前，相机打开过，则先将CCD数据清空 暂时定时0.015s
	// lcz new 06/20  修改为每次测试前都进行数据清空
	//if(g_bOpenCamera)
	//{
		if( m_bAutoRunning == 0 || g_bUSBDiscon)
			return 0;
		else
		{
			m_ZolixCCD.SetExpTime(0.02);
			//  根据像素点数读取数据，存放在buffer里
			m_ZolixCCD.DataAcquisition(buffer, sInfo.sDInfo.nGXPix);
			memset(buffer,0,sizeof(double)*DataMax);// 数据清零
		}
	//	g_bOpenCamera = false;
	//}

	int test=0;
	int repect = 0;
	int JAccNum=sTInfo.JAccNum;// 精检里设置的累计次数
	DWORD dwBegin=0,dwEnd=GetTickCount();

	bool flag = true;// 每次只画一条曲线
	int index = 0;

	while(m_bAutoRunning)
	{
		if(sINInfo.bCheak==true&&repect>=sINInfo.AccNum)// 如果启用累计停止，并且重复次数大于等于累计次数，则改变标志位
		{
			m_bAutoRunning=FALSE;
			break;
		}

		if((dwEnd-dwBegin)>(sINInfo.Time*1000))//  间隔时间
		{
			TRACE(L"dwBegin %d  %d  %d\n", dwEnd, dwBegin, dwEnd-dwBegin);
			
			//Sleep(2000);

			memset(bufferBkData,0,sizeof(double)*DataMax);
			memset(buffer,0,sizeof(double)*DataMax);
			memset(bufferX,0,sizeof(double)*DataMax);
			memset(bufferBkDataR,0,sizeof(double)*DataMax);
			memset(bufferR,0,sizeof(double)*DataMax);
			memset(bufferXR,0,sizeof(double)*DataMax);
			//  分波数校正还是波长校正
			if(g_bCorrNumLen)
			{
				if( sInfo.OminiType != 20)
					m_ZolixCCD.XAxisData(bufferX, sSInfo.b, 4, sInfo.sDInfo.nGXPix);
				else
					otoXAxisData(bufferX, sSInfo.b, 4, sInfo.sDInfo.nGXPix);
				//m_ZolixCCD.XAxisData(bufferX, sSInfo.b, 4, sInfo.sDInfo.nGXPix);
				int count = 0;
				//  按照所截取的波数范围进行设置
				for(int i = 0; i < sInfo.sDInfo.nGXPix; i++)
				{
					if (bufferX[i] < sSInfo.XLeft )
					{
						sCInfo.CutF = i + 1 ;
					}
					if (bufferX[i] <= sSInfo.XRight)
					{
						count++;
					}
				}
				sCInfo.CutSzie = count - sCInfo.CutF;
				//  截取后的波数范围值
				for(int i = 0; i < sCInfo.CutSzie; i++)
					bufferXR[i] = bufferX[sCInfo.CutF + i];
			} 
			else
			{
				// 读取X轴数据 XAxisData// void *pd：初始化全部为0,返回内容X数据为波长// coeff：校正系数// Num：校正系数长度// nPixSize:X轴像素长度
				if( sInfo.OminiType != 20)
					m_ZolixCCD.XAxisData(bufferX, sSInfo.a, 4, sInfo.sDInfo.nGXPix);
				else
					otoXAxisData(bufferX, sSInfo.a, 4, sInfo.sDInfo.nGXPix);
				// 读取X轴截取数据 GetRangeDataX// nNMCM:截取范围是按波长还是波数计算  0-波长 1-波数// iSzie:X轴数据长度// Laser;激光器波长// dRangeLeft:左边范围 单位与nNMCM统一// dRangeRight:右边范围// *inputD：输入X// *outputD：输出X// *CutF:输出截取后第一个位置 从0开始
				sCInfo.CutSzie=m_ZolixCCD.GetRangeDataX(sCInfo.NMCM,sInfo.sDInfo.nGXPix,sCInfo.Laser,sSInfo.XLeft,
					sSInfo.XRight,bufferX,bufferXR,&sCInfo.CutF);
			}

			if (sSInfo.Back==1)//  如果设置了去除暗噪声
			{
				g_bSingleCollectFinish = false;

				m_ZolixCCD.SetExpTime(sTInfo.JTime);// 设置CCD积分时间
				m_ZolixCCD.DataAcquisition(bufferBkData,sInfo.sDInfo.nGXPix);// 读取谱图数据 DataAcquisition

				g_bSingleCollectFinish = true;

				//  lcz new 08/10
				// lcz new 06/07  如果测试的数据很多是0，或者数据是其他值稳定不变，则表示测试中断开
				//int j = 0, k = 0;
				//double firstNum = bufferBkData[0];
				//for(int i = 0; i < sInfo.sDInfo.nGXPix; i++)
				//{
				//	if( (bufferBkData[i] > 70000 || bufferBkData[i] < -70000) &&(!g_bOpenCamera) )
				//	{
				//		g_bUSBDiscon = true;
				//		Sleep(100);
				//		//AfxMessageBox(TEXT("测试中（未开白光或已清除数据）数据超过±70000"));
				//		return 0;
				//	}
				//	else if(bufferBkData[i] == firstNum && firstNum != 65535 &&(!g_bOpenCamera) )
				//	{
				//		k++;
				//	}
				//}
				//if( j > sInfo.sDInfo.nGXPix * 0.5 || k > sInfo.sDInfo.nGXPix * 0.5)
				//{
				//	g_bUSBDiscon = true;
				//	Sleep(100);
				//	//AfxMessageBox(TEXT("测试中（未开白光或已清除数据），数据很多为0或者保持不变"));
				//	return 0;
				//}

				// 读取X轴截取数据 GetRangeDataY该函数与GetRangeDataX连用 // CutF:输入截取后第一个位置 从0开始// rSize:截取数据长度// *inputD：输入Y// *outputD：输出Y
				m_ZolixCCD.GetRangeDataY(sCInfo.CutF,sCInfo.CutSzie,bufferBkData,bufferBkDataR);

				// lcz new 22/10/11  暗噪声数据
				if(bufferXR[sInfo.sDInfo.nGXPix - 1 - sCInfo.CutF] <= 3200 && sSInfo.XRight > bufferXR[sCInfo.CutSzie - 1] + 0.5 && sSInfo.iAddPointNum > 0)
				{
					for(int i = 0; i < sSInfo.iAddPointNum ; i++)
					{
						//double tempDiff = bufferBkDataR[ sCInfo.CutSzie - 100 + i ] - bufferBkDataR[ sCInfo.CutSzie - 101 + i ];
						//double tempDiff = bufferBkDataR[ i + 1] - bufferBkDataR[ i ];
						double tempDiff = rand()%50 - 25;
						bufferBkDataR[sCInfo.CutSzie + i ] = bufferBkDataR[sCInfo.CutSzie - 1 ] + tempDiff;
						if(bufferBkDataR[sCInfo.CutSzie + i ] < 0)
							bufferBkDataR[sCInfo.CutSzie + i ] = 0;
					}
				}//====
			}
			//  lcz new 08/24
			pDlg->SetLawerPower(sTInfo.JPower,FALSE);//  精检里设置的功率
			//  每次只增画一条曲线
			if(flag)
			{
				index = pDlg->m_Tchart.AddSeries(0);
				flag = false;
			}

			int iTemp = index;
			CString strTempName = g_strCurLineName+L"-";
			int iNameRepeat = 0;
			while(iTemp > 0)
			{
				CString strName = pDlg->m_Tchart.Series(iTemp-1).GetTitle();
				if(strName.CompareNoCase(g_strCurLineName) == 0)
					iNameRepeat++;
				if(strName.Find(strTempName)>=0)
					iNameRepeat++;

				pDlg->m_Tchart.Series(iTemp-1).SetActive(FALSE);
				iTemp--;
			}
			pDlg->m_iSeriesIndex = index;
			//  设置teechart绘图的索引、颜色和标题
			index = index >=0 ? index : 0;
			int color = g_iRGB[index%SERIES_MAX];
			//int color = RGB(g_r, g_g, g_b);
			pDlg->m_Tchart.Series(index).SetColor(color);
			//int color =pDlg->m_Tchart.Series(index).GetColor();

			CString seriesName ;
			CString tempIndex; //  记录当前进行的序号
			tempIndex.Format(TEXT("_%d"), repect);

			if(g_strCurLineName.CompareNoCase("NULL") == 0)
				seriesName = pDlg->m_Tchart.Series(index).GetName() + tempIndex;
			else
			{
				seriesName = g_strCurLineName;
				if(iNameRepeat > 0)
				{
					CString strName;
					strName.Format(_T("-%d"), iNameRepeat);
					seriesName += strName;	
					seriesName += tempIndex;
				}
			}
			pDlg->m_Tchart.Series(index).SetTitle(seriesName);
			pDlg->m_Tchart.Series(index).SetShowInLegend(true);

			// 每次间隔测试，只显示一张谱图，第一次测试，显示曲线和图例，第二次测试，不显示第一次曲线和图例
			if(repect >= 1 && repect < sINInfo.AccNum)
			{
				//pDlg->m_Tchart.RemoveSeries(index - 1);
				//pDlg->m_Tchart.Series(index - 1).SetShowInLegend(false);
				pDlg->m_Tchart.Series(index).Clear();
			}

			test = 0;
			DWORD t1=GetTickCount();
			while(m_bAutoRunning && test < JAccNum)//  JAccNum：精检的累计次数
			{
				if(g_bCameraOpen2Close) // 如果测试中相机从打开到关闭，下次获取数据前需要清空次数据
				{
					g_bCameraOpen2Close = false;
					m_ZolixCCD.SetExpTime(0.015);
					m_ZolixCCD.DataAcquisition(buffer,sInfo.sDInfo.nGXPix);
					memset(buffer,0,sizeof(double)*DataMax);// 数据清零
				}

				g_bSingleCollectFinish = false;

				m_ZolixCCD.SetExpTime(sTInfo.JTime);
				m_ZolixCCD.DataAcquisition(buffer,sInfo.sDInfo.nGXPix);

				g_bSingleCollectFinish = true;

				//  lcz new 08/10  取消根据CCD返回的数据判断USB是否断开
				// lcz new 06/07  如果测试的数据很多是0，或者数据是其他值稳定不变，则表示测试中断开
				//int j = 0, k = 0;
				//double firstNum = buffer[0];
				//for(int i = 0; i < sInfo.sDInfo.nGXPix; i++)
				//{
				//	if( (buffer[i] > 70000 || buffer[i] < -70000) &&(!g_bOpenCamera) )
				//	{
				//		g_bUSBDiscon = true;
				//		Sleep(100);
				//		//AfxMessageBox(TEXT("测试中（未开白光或已清除数据）数据超过±70000"));
				//		return 0;
				//	}
				//	else if(buffer[i] == firstNum && firstNum != 65535 &&(!g_bOpenCamera)  )
				//	{
				//		k++;
				//	}
				//}
				//if( j > sInfo.sDInfo.nGXPix * 0.5 || k > sInfo.sDInfo.nGXPix * 0.5)
				//{
				//	g_bUSBDiscon = true;
				//	Sleep(100);
				//	//AfxMessageBox(TEXT("测试中（未开白光或已清除数据），数据保持不变"));
				//	return 0;
				//}

				m_ZolixCCD.GetRangeDataY(sCInfo.CutF,sCInfo.CutSzie,buffer,bufferR);
				//  lcz new 22/10/11
				g_iAddPoint = 0;
				if(bufferXR[sInfo.sDInfo.nGXPix - 1 - sCInfo.CutF] <= 3200 && sSInfo.XRight > bufferXR[sCInfo.CutSzie - 1] + 0.5 && sSInfo.iAddPointNum > 0)
				{
					double tempGap = bufferXR[sCInfo.CutSzie - 1] - bufferXR[sCInfo.CutSzie - 2]; //  寰楀埌闂磋窛
					//  娣诲姞100 涓暟鎹偣
					for(int i = 0; i < sSInfo.iAddPointNum; i ++)
					{
						bufferXR[i + sCInfo.CutSzie] = bufferXR[sCInfo.CutSzie - 1] + tempGap * (i + 1); //  妯潗鏍囨坊鍔犱簡100涓偣锛岃寖鍥村埌浜?240 cm-1
						if(bufferXR[i + sCInfo.CutSzie] < sSInfo.XRight)
							g_iAddPoint++;
					}

					for(int i = 0; i < sSInfo.iAddPointNum; i++)
					{
						double tempDiff = rand()%50 - 25;
						bufferR[sCInfo.CutSzie + i ] = bufferR[sCInfo.CutSzie - 1] + tempDiff;
						if(bufferR[sCInfo.CutSzie + i ] < 0)// 濡傛灉寮哄害闄嶅埌0浠ヤ笅锛屽垯鍙樹负0
							bufferR[sCInfo.CutSzie + i] = 0;
					}
				}//=====

				pDlg->OnSetAutoRepaint(FALSE);
				memset(g_pBuffer,0,sizeof(double)*3000);
				if (test==0)
				{
					for (int i=0;i<sCInfo.CutSzie+g_iAddPoint;i++)
					{
						double dYdata;
						dYdata=(bufferR[i]-bufferBkDataR[i]);					
						dYdata=pDlg->ScanOffset(bufferXR[i],dYdata);

						bufferYR[i]=dYdata;
						//pDlg->m_Tchart.Series(0).AddXY(bufferXR[i],dYdata,NULL,clrSet);
					}
					//  返回截取后的长度，里面根据基线校正和去噪等参数进行了处理
					cutFitSmooth(bufferXR,bufferYR,sCInfo.CutSzie+g_iAddPoint,bufferXRs,bufferYRs);
					for (int i=0;i<sCInfo.CutSzie+ g_iAddPoint;i++)
					{
						// lcz new 22/10/20
						g_pBuffer[i] = bufferYRs[i];
						//pDlg->m_Tchart.Series(index).AddXY(bufferXRs[i],bufferYRs[i],NULL,color);
					}
					//pDlg->OnSetAutoRepaint(TRUE);
					//pDlg->AutoSaveFile();
				}
				else
				{
					list_y=pDlg->m_Tchart.Series(index).GetYValues();
					for (int i=0;i<sCInfo.CutSzie;i++)
					{
						double dDataY;
						dDataY=(bufferR[i]-bufferBkDataR[i]);
						dDataY=pDlg->ScanOffset(bufferXR[i],dDataY);

						bufferYR[i] = dDataY;
						//if (list_y.GetCount()>=sCInfo.CutSzie+ g_iAddPoint)
						//{
						//	list_y.SetValue(i,dDataY);
						//}
					}
					cutFitSmooth(bufferXR,bufferYR,sCInfo.CutSzie,bufferXRs,bufferYRs);

					for (int i=0;i<sCInfo.CutSzie+ g_iAddPoint;i++)
					{
						double dDataY;
						dDataY=(bufferYRs[i]*test+bufferR[i])/(test+1);
						dDataY=pDlg->ScanOffset(bufferXR[i],dDataY);

						//if (list_y.GetCount()>=sCInfo.CutSzie+ g_iAddPoint)
						//{
						//	list_y.SetValue(i,dDataY);
						//}
						g_pBuffer[i] = dDataY;
					}
				}

				//  lcz new 22/10/20
				sDataShowInfo.color = color;
				sDataShowInfo.seriesNum = index;
				sDataShowInfo.x = bufferXRs;
				sDataShowInfo.y = g_pBuffer;
				sDataShowInfo.pointNum = sCInfo.CutSzie+g_iAddPoint;
				::PostMessage(pDlg->m_hWnd,WM_SHOWDATA, 0, (LPARAM)(&sDataShowInfo));
				if (g_bOpenCamera == false && WaitForSingleObject(pDlg->m_hEvent, 5000) == WAIT_TIMEOUT)
				{
					//return 0;
					goto nEnd;
				}

				test++;
				pDlg->OnSetAutoRepaint(TRUE);
				if (sSInfo.CorrMode==1)
				{
					m_bAutoRunning=FALSE;
					//					pDlgF->m_DlgAdv.m_DlgLaCorr.SetButtonS(TRUE);
					sSInfo.CorrMode=0;
				}
				//dwBegin=dwEnd;			
			}
			dwBegin=GetTickCount();
			DWORD t2=GetTickCount();
			TRACE(L"Once %d  %d  %d\n", t2, t1, t2-t1);
			//pDlg->OnSetAutoRepaint(TRUE);
			repect++;
			pDlg->SetLawerPower(CloseLaserPow,FALSE);
			pDlg->AutoSaveFile(seriesName);
		}
		dwEnd=GetTickCount();
	}

nEnd:
	// lcz new 09/29 采集完数据后，再恢复温度的读取。
	g_bGetTemp = false;


	pDlg->SetLawerPower(CloseLaserPow,FALSE);
	m_bAutoRunning=FALSE;
	m_bAutoRunningStatue=FALSE;
	delete [] buffer;
	delete [] bufferX;
	delete [] bufferBkData;
	delete [] bufferR;
	delete [] bufferXR;
	delete [] bufferYR;
	delete [] bufferXRs;
	delete [] bufferYRs;
	delete [] bufferBkDataR;

	return 0;
}

// 间隔采集1064
DWORD WINAPI  ThreadGetDataD1064IN(LPVOID lpParam)
{

	g_bNewTest = true;
	g_bStopTest = false;

	g_DlgShowlog.WriteDebugLog(_T("ThreadGetDataL1064 Begin!"));

	CINScanLiteDlg *pDlgF = (CINScanLiteDlg*)AfxGetApp()->GetMainWnd();

	pDlg->OnSetAutoRepaint(FALSE);

	CValueList list_y;

	double *bufferX = new double[DataMax]; //  根据校正系数计算得到的波长值（目前是波长，535-635 nm)
	double *bufferXR = new double[DataMax];//  截取的X轴数据，坐标为波数，201.3cm-1起

	memset(bufferX, 0, sizeof(double)*DataMax);
	memset(bufferXR, 0, sizeof(double)*DataMax);
	unsigned short* buffer = new unsigned short[DataMax];
	memset(buffer, 0, sizeof(unsigned short)*DataMax);



	//  分波数校正还是波长校正
	sCInfo.CutF = 0;
	//if (g_bCorrNumLen)// 如果是波数校正

	//{
	otoXAxisData(bufferX, sSInfo.b, 4, sInfo.sDInfo.nGXPix);

	int count = 0;
	//  按照所截取的波数范围进行设置
	for (int i = 0; i < sInfo.sDInfo.nGXPix; i++)
	{
		if (bufferX[i] < sSInfo.XLeft)
		{
			sCInfo.CutF = i + 1;
		}
		if (bufferX[i] <= sSInfo.XRight)
		{
			count++;
		}
	}
	sCInfo.CutSzie = count - sCInfo.CutF;
	//  截取后的波数范围值
	for (int i = 0; i < sCInfo.CutSzie; i++)
		bufferXR[i] = bufferX[sCInfo.CutF + i];

	if (m_bAutoRunning == 0 || g_bUSBDiscon)
		return 0;
	else
	{

		bool b = dfield_set_exposure(pDlgF->_handle, 0.2 * 1e3 * 1e3);// 0.2s = 200ms = 200 * 1e3 us
																	  //  根据像素点数读取数据，存放在buffer里
																	  //std::vector<unsigned short> _y = std::vector<unsigned short>(sInfo.sDInfo.nGXPix, 0);
																	  //dfield_get_data(pDlgF->_handle, &(_y[0]), sInfo.sDInfo.nGXPix);
		pDlg->show_dfield_get_data(pDlgF->_handle, buffer, sInfo.sDInfo.nGXPix, sCInfo.CutF);
		memset(buffer, 0, sizeof(unsigned short)*DataMax);
	}

	//  点击连续，默认激光器未打开
	pDlg->SetLawerPower(sTInfo.JPower, FALSE);

	int test = -1;
	int nIndex = 0;
	int color = 0;

	int JAccNum = sTInfo.JAccNum;
	CString seriesName;
	while (m_bAutoRunning)
	{
		// Sleep(800);
		while (m_bAutoRunning)
		{
			if (g_bClose)
			{
				break;
			}
			//Sleep(sTInfo.JTime*1000);

			if (m_bAutoRunning == 0)
				return 0;
			else
			{
				if (g_bCameraOpen2Close) // 如果测试中相机从打开到关闭，下次获取数据前需要清空次数据
				{
					g_bCameraOpen2Close = false;
					//m_ZolixCCD.SetExpTime(0.015);
					bool b = dfield_set_exposure(pDlgF->_handle, 0.02 * 1e3 * 1e3);// 0.2s = 200ms = 200 * 1e3 us
					pDlg->show_dfield_get_data(pDlgF->_handle, buffer, sInfo.sDInfo.nGXPix, sCInfo.CutF);
					memset(buffer, 0, sizeof(unsigned short)*DataMax);// 数据清零
				}

				//memset(buffer, 0, sizeof(double) * 3000);
				g_bSingleCollectFinish = false;
				dfield_set_exposure(pDlgF->_handle, sTInfo.JTime * 1e3 * 1e3);// 0.2s = 200ms = 200 * 1e3 us
				dfield_capture(pDlgF->_handle);//  连续采集时，需添加此行代码才可以，不然数据不会刷新
				memset(buffer, 0, sizeof(unsigned short)*DataMax);// 数据清零
				pDlg->show_dfield_get_data(pDlgF->_handle, buffer, sInfo.sDInfo.nGXPix, sCInfo.CutF);
				//  如果添加了镜像
				if (sTInfo.b1064ccdFlip)
				{
					double temp = 0.0;
					for (int i = 0; i < sInfo.sDInfo.nGXPix / 2; i++)
					{
						double temp = buffer[i];
						buffer[i] = buffer[sInfo.sDInfo.nGXPix - i - 1];
						buffer[sInfo.sDInfo.nGXPix - i - 1] = temp;
					}
				}

				//m_ZolixCCD.SetExpTime(sTInfo.JTime);
				//  根据像素点数读取数据，存放在buffer里-----这行获取的数据出现了问题
				//m_ZolixCCD.DataAcquisition(buffer, sInfo.sDInfo.nGXPix);

				g_bSingleCollectFinish = true;


			}
			//  对拉曼强度数据进行截取，截取后的数据存放在bufferR中
			//sCInfo.CutF = 0;
			//sCInfo.CutSzie = sInfo.sDInfo.nGXPix; 
			//m_ZolixCCD.GetRangeDataY(sCInfo.CutF, sCInfo.CutSzie, buffer, bufferR);
			g_DlgShowlog.WriteDebugLog(_T("Get Data Once!"));

			// lcz new 22/10/11
			g_iAddPoint = 0;

			if (test == -1)
			{
				nIndex = pDlg->m_Tchart.AddSeries(0);//  添加第一条线
				int iTemp = nIndex;
				while (iTemp > 0)
				{
					pDlg->m_Tchart.Series(iTemp - 1).SetActive(FALSE);
					iTemp--;
				}
				//color = pDlg->m_Tchart.Series(nIndex).GetColor();
				//  设置第 nIndex 条曲线的名称、标题
				nIndex = nIndex >= 0 ? nIndex : 0;
				color = g_iRGB[nIndex%SERIES_MAX];
				pDlg->m_Tchart.Series(nIndex).SetColor(color);
				pDlg->m_Tchart.Series(nIndex).SetShowInLegend(true);
			}
			test++;
			// 设置曲线标题
			long count = pDlg->m_Tchart.GetSeriesCount();
			seriesName.Format("%s%d_%d", "Series", count,test);
			pDlg->m_Tchart.Series(nIndex).SetTitle(seriesName);

			memset(g_pBuffer, 0, sizeof(double) * 3000);
			if (test == 0)
			{
				int i;
				color = pDlg->m_Tchart.Series(nIndex).GetColor();
				for (i = 0; i<sCInfo.CutSzie + g_iAddPoint; i++)// lcz new 22/10/11
				{
					g_pBuffer[i] = buffer[i];
				}
			}
			else
			{
				list_y = pDlg->m_Tchart.Series(nIndex).GetYValues();
				int i;
				for (i = 0; i<sCInfo.CutSzie + g_iAddPoint; i++)
				{
					g_pBuffer[i] = buffer[i];
				}
			}

			// lcz new 22/10/20  谱图刷新
			pDlg->OnSetAutoRepaint(TRUE);//  必须加上这行

			sDataShowInfo.color = color;
			sDataShowInfo.seriesNum = nIndex;
			sDataShowInfo.x = bufferXR;
			sDataShowInfo.y = g_pBuffer;
			sDataShowInfo.pointNum = sCInfo.CutSzie + g_iAddPoint;
			::PostMessage(pDlg->m_hWnd, WM_SHOWDATA, 0, (LPARAM)(&sDataShowInfo));
			if (g_bOpenCamera == false && WaitForSingleObject(pDlg->m_hEvent, 5000) == WAIT_TIMEOUT)
			{
				//return 0;
				goto nEnd;
			}
			// 保存文件
			pDlg->AutoSaveFile(seriesName);
			if (sSInfo.CorrMode == 1)
			{
				m_bAutoRunning = FALSE;
				//pDlgF->m_DlgAdv.m_DlgLaCorr.SetButtonS(TRUE);
				sSInfo.CorrMode = 0;
			}

			//  间隔时间
			if (test >= sINInfo.AccNum - 1 && sINInfo.bCheak)
			{
				m_bAutoRunning = FALSE;
				break;
			}
			else
			{
				Sleep(sINInfo.Time * 1000);
			}
			
		}
	}

nEnd:

	g_bStopTest = true;

	//g_bOpenCamera = false;
	pDlg->SetLawerPower(CloseLaserPow, FALSE);//  先关激光再保存文件
	g_DlgShowlog.WriteDebugLog(_T("ThreadGetDataL Over!!"));
	
	m_bAutoRunning = FALSE;
	m_bAutoRunningStatue = FALSE;

	delete[] buffer;
	buffer = NULL;
	delete[] bufferX;
	bufferX = NULL;
	delete[] bufferXR;
	bufferXR = NULL;

	g_bGetTemp = false;
	return 0;
}

UINT SELECTSQLNEW(LPVOID lParam)//单波长光谱
{
	CINScanLiteDlg *pDlgF = (CINScanLiteDlg*)AfxGetApp()->GetMainWnd();
	pDlgF->pDlgWait->ShowWindow(SW_SHOW);

	int len=pDlg->m_Tchart.Series(sSInfo.SeriesNum).GetCount();
	double * intensity;
	intensity=new double[len];
	pDlg->OriginalData(intensity);
	double * intensityX;
	intensityX=new double[len];
	pDlg->OriginalDataX(intensityX);

	bool bRet = false;
	if (pDlg->mDataCompare.InitSQL()&&len<20000)
	{
		pDlg->mDataCompare.VectorInit();
		bRet = pDlg->mDataCompare.DataCompare(intensityX,intensity,len);
		pDlg->mDataCompare.ShowData(&sDInfo);
	}
	pDlgF->pDlgWait->ShowWindow(SW_HIDE);
	//pDlgF->m_DlgFile.m_DlgDataPro.ShowList();
	CString str=pDlgF->LoadStringMeau(_T("ID_DB_FAIL"));
	if(!bRet)
		AfxMessageBox(str,MB_ICONWARNING);
	sSInfo.bSQL=true;

	delete [] intensity;// 析构 内存
	intensity=NULL;
	delete [] intensityX;// 析构 内存
	intensityX=NULL;



	return TRUE;
}
/// CDlgShow 对话框

IMPLEMENT_DYNAMIC(CDlgShow, CDialog)

	CDlgShow::CDlgShow(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgShow::IDD, pParent)
{

	nCurChartLine = -1;
	m_iMouseX = 0;
	m_iMouseY = 0;
	m_bEnterSeries = false;
	m_iEnterSeriesIndex = -1;
	g_DlgShowlog.SetLog(g_strLogPath,_T("DlgShow"),LL_ALL,1,10);
	g_DlgShowlog.WriteInfoLog(_T("DlgShow=================================="));
	m_iZoomCount = 0;

	m_hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
}

CDlgShow::~CDlgShow()
{
}

void CDlgShow::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TCHART_MAIN_SHOW, m_Tchart);
	//DDX_Control(pDX, IDC_TCHART_MAIN_SHOW_2017, m_Tchart);
}

BEGIN_MESSAGE_MAP(CDlgShow, CDialog)
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_MESSAGE(WM_SHOWDATA,OnShowData)
END_MESSAGE_MAP()


void CDlgShow::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	CRect rect;
	GetClientRect(rect);
	dc.FillSolidRect(rect,RGB(255,255,255));
	CDialog::OnPaint();
}

BOOL CDlgShow::OnInitDialog()
{
	CDialog::OnInitDialog();
	pDlg=this;
	// TODO:  在此添加额外的初始化
	nSeries=0;
	chickline=FALSE;
	is_HandMark=FALSE;
	is_DelMark=FALSE;
	//SetTimer(1, 10, NULL);
	InitializeCriticalSection( & g_cs); 
	//AfxBeginThread(&TestThread, this);
	//AfxBeginThread(&MoniThread, this);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

UINT CDlgShow::MoniThread(LPVOID pParam)
{
	CDlgShow* pthis = (CDlgShow*)pParam;
	double fMax = 0;
	g_bNewTest = true;
	while(1)
	{
		fMax++;
		EnterCriticalSection( & g_cs);
		g_vec_MaxY.push_back(fMax);
		LeaveCriticalSection( & g_cs);
		Sleep(sTInfo.JTime*1000);
	}
	return 0;
}

UINT CDlgShow::TestThread(LPVOID pParam)
{
	CDlgShow* pthis = (CDlgShow*)pParam;
	if(pthis)
		pthis->Test();
	return 0;
}
void CDlgShow::Test()
{
	CINScanLiteDlg * pDlg = (CINScanLiteDlg *)AfxGetApp()->GetMainWnd();
	while(1)
	{
		CString strPathName;
		CStdioFile file;
		bool bOpen;
		if(g_bNewTest)
		{
			g_bNewTest = false;
			if (sINInfo.strPath==_T("")||sINInfo.strPath==_T("0"))
			{
				sINInfo.strPath=sInfo.Path+_T("\\Data");
			}

			if(!PathIsDirectory(sINInfo.strPath))
			{
				::CreateDirectory(sINInfo.strPath,0);
			}

			CString strExt = _T(".txt");
			CString strName = FileNameFormat(_T("MaxY"));
			//str=sINInfo.strPath+GetCurTimeFile();
			strPathName = sINInfo.strPath + _T("\\")+ strName + strExt;
			bOpen = file.Open(strPathName,CFile::modeCreate|CFile::modeWrite);
			if (!bOpen) 
			{
				AfxMessageBox(pDlg->LoadStringMeau(_T("ID_FILE_FAIL")),MB_ICONWARNING);
				continue;
			}
			int iCount = 0;
			while(!g_bStopTest)
			{
				if(g_vec_MaxY.size() > 10)
				{
					if(bOpen)
					{
						CString strContext;
						CString strTemp;
						EnterCriticalSection( & g_cs);
						for(int i = 0; i < g_vec_MaxY.size(); i++)
						{
							++iCount;
							strTemp.Format("%f, %d\n",sTInfo.JTime*iCount,g_vec_MaxY[i]);
							strContext += strTemp;
						}
						g_vec_MaxY.clear();
						LeaveCriticalSection( & g_cs);
						file.Seek(0,CFile::end);
						file.WriteString(strContext);
						file.Close();
						bOpen = false;
					}
					else
					{
						bOpen = file.Open(strPathName,CFile::modeNoTruncate |CFile::modeWrite);
					}
				}
			}

		}
		Sleep(100);
	}
}

void CDlgShow::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);
	SetSize();	
}
void CDlgShow::SetSize()
{

	CRect rect;
	GetClientRect(&rect);
	ScreenToClient(&rect);
	CWnd *pWnd;
	pWnd = GetDlgItem(IDC_TCHART_MAIN_SHOW); //获取控件指针，IDC_BUTTON1为控件ID号
	//pWnd = GetDlgItem(IDC_TCHART_MAIN_SHOW_2017); //获取控件指针，IDC_BUTTON1为控件ID号
	if (pWnd!=NULL)
	{
		pWnd->SetWindowPos(&wndTop,0,0,rect.Width(),rect.Height(),SWP_SHOWWINDOW); //把按钮移到窗口的(50,80)处

	}
}

void CDlgShow::SetLawerPower(int po,BOOL bS)
{

	CINScanLiteDlg *pDlg = (CINScanLiteDlg*)AfxGetApp();
	CString strvalue;
	//  如果百分比为0，激光器未打开，先打开激光器，然后不出激光
	if (po==0 && bS==FALSE)
	{
		Sleep(1000);
		pDlg->m_DlgSet.m_Serial.SetVolt(sSInfo.nCOM,TEXT("0"));
		pDlg->m_DlgSet.m_Serial.CloseLaserPower(sSInfo.nCOM);   //因为用杏林785测试--即使上一步把功率设为0,还是会有微弱激光,所以直接在这里关使能
		return;
	}
	//  如果百分比为0，激光器已打开，则不出激光
	else if(po==0 && bS==TRUE)
	{

		pDlg->m_DlgSet.m_Serial.SetVolt(sSInfo.nCOM,TEXT("0"));
		Sleep(sSInfo.Delay);
		return;
	}
	
	//po不为0时走这里
	po = SendLaser(po);
	strvalue.Format("%d",po);
	pDlg->m_DlgSet.m_Serial.SetVolt(sSInfo.nCOM,strvalue);
	Sleep(sSInfo.Delay);
}
int CDlgShow::SendLaser(int nlaser)
{
	//CINScanLiteDlg *pDlg = (CINScanLiteDlg*)AfxGetApp();   //2025可能为预留接口或遗留代码
	if (nlaser >= 100)
	{
		nlaser = sSInfo.LASERPOWERMAX;
	}
	else
	{
		nlaser = (int)(((sSInfo.LASERPOWERMAX - sSInfo.LASERPOWERMIN) / 100.0)*nlaser*1.0 + sSInfo.LASERPOWERMIN);
	}	


	return nlaser;
}
double CDlgShow::ReturnMaxData(double *buffer,int nSize)
{
	double dData=0;

	for (int i=0;i<nSize;i++)
	{
		if (buffer[i]>dData)
		{
			dData=buffer[i];
		}
	}

	return dData;
}
double CDlgShow::ReturnK(double dData)
{
	if (dData==0)
	{
		return 0;
	}
	return (sTInfo.KRangeS+sTInfo.KRangeX)/(2*dData);
}

//  BkData是背景数据    pDlg->GetBackData(bufferS1,bufferBkData);
void CDlgShow::GetBackData(double *optData,double *BkData)
{
	double *bufferX=new double[DataMax];
	double *bufferR=new double[DataMax];
	memset(bufferR,0,sizeof(double)*DataMax);
	memset(bufferX,0,sizeof(double)*DataMax);

	m_ZolixCCD.SetExpTime(sTInfo.JTime);
	for (int i=0;i<sTInfo.JAccNum;i++)
	{
		//  bufferX是每次背景的数据，bufferR是累加的背景的数据
		m_ZolixCCD.DataAcquisition(bufferX,sInfo.sDInfo.nGXPix);
		for(int j=0;j<sInfo.sDInfo.nGXPix;j++)
		{
			bufferR[j]=bufferR[j]+bufferX[j];
		}
	}
	//  bufferX = bufferR / JAccNum - BkData;
	for(int w=0;w<sInfo.sDInfo.nGXPix;w++)
	{
		// lcz new 08/30 取消计算背景光谱时扣除暗噪声
		//bufferX[w]=bufferR[w]/sTInfo.JAccNum-BkData[w];
		bufferX[w]=bufferR[w]/sTInfo.JAccNum;
	}
	//m_ZolixCCD.DataAcquisition(bufferX,sInfo.sDInfo.nGXPix);
	m_ZolixCCD.GetRangeDataY(sCInfo.CutF,sCInfo.CutSzie,bufferX,optData);
	delete [] bufferX;
	delete [] bufferR;
}
int CDlgShow::GetTeeCount()
{
	return m_Tchart.GetSeriesCount();
}
//  如果是键盘按下：如果是按下回车键或者Esc键，都返回true
BOOL CDlgShow::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN)
	{
		switch(pMsg->wParam)
		{
		case VK_RETURN:
			return  TRUE;
		case VK_ESCAPE:
			return TRUE;
		default:
			break;
		}
	}

	return CDialog::PreTranslateMessage(pMsg);
}
BEGIN_EVENTSINK_MAP(CDlgShow, CDialog)
	// 	ON_EVENT(CDlgShow, IDC_TCHART_MAIN_SHOW, 6, CDlgShow::OnClickLegendTchartMainShow, VTS_I4 VTS_I4 VTS_I4 VTS_I4)
	// 	ON_EVENT(CDlgShow, IDC_TCHART_MAIN_SHOW, 19, CDlgShow::OnMouseDownTchartMainShow, VTS_I4 VTS_I4 VTS_I4 VTS_I4)
	// 	ON_EVENT(CDlgShow, IDC_TCHART_MAIN_SHOW, 21, CDlgShow::OnMouseUpTchartMainShow, VTS_I4 VTS_I4 VTS_I4 VTS_I4)
	// 	ON_EVENT(CDlgShow, IDC_TCHART_MAIN_SHOW, 20, CDlgShow::OnMouseMoveTchartMainShow, VTS_I4 VTS_I4 VTS_I4)
	//ON_EVENT(CDlgShow, IDC_TCHART_MAIN_SHOW_2017, 6, CDlgShow::OnClickLegendTchartMainShow, VTS_I4 VTS_I4 VTS_I4 VTS_I4)
	//ON_EVENT(CDlgShow, IDC_TCHART_MAIN_SHOW_2017, 19, CDlgShow::OnMouseDownTchartMainShow, VTS_I4 VTS_I4 VTS_I4 VTS_I4)
	//ON_EVENT(CDlgShow, IDC_TCHART_MAIN_SHOW_2017, 21, CDlgShow::OnMouseUpTchartMainShow, VTS_I4 VTS_I4 VTS_I4 VTS_I4)
	//ON_EVENT(CDlgShow, IDC_TCHART_MAIN_SHOW_2017, 20, CDlgShow::OnMouseMoveTchartMainShow, VTS_I4 VTS_I4 VTS_I4)
	//ON_EVENT(CDlgShow, IDC_TCHART_MAIN_SHOW_2017, 201, CDlgShow::OnMouseEnterSeriesTchartMainShow2017, VTS_I4)
	//ON_EVENT(CDlgShow, IDC_TCHART_MAIN_SHOW_2017, 202, CDlgShow::OnMouseLeaveSeriesTchartMainShow2017, VTS_I4)
	ON_EVENT(CDlgShow, IDC_TCHART_MAIN_SHOW, 6, CDlgShow::OnClickLegendTchartMainShow, VTS_I4 VTS_I4 VTS_I4 VTS_I4)
	ON_EVENT(CDlgShow, IDC_TCHART_MAIN_SHOW, 19, CDlgShow::OnMouseDownTchartMainShow, VTS_I4 VTS_I4 VTS_I4 VTS_I4)
	ON_EVENT(CDlgShow, IDC_TCHART_MAIN_SHOW, 21, CDlgShow::OnMouseUpTchartMainShow, VTS_I4 VTS_I4 VTS_I4 VTS_I4)
	ON_EVENT(CDlgShow, IDC_TCHART_MAIN_SHOW, 20, CDlgShow::OnMouseMoveTchartMainShow, VTS_I4 VTS_I4 VTS_I4)
	ON_EVENT(CDlgShow, IDC_TCHART_MAIN_SHOW, 201, CDlgShow::OnMouseEnterSeriesTchartMainShow2017, VTS_I4)
	ON_EVENT(CDlgShow, IDC_TCHART_MAIN_SHOW, 202, CDlgShow::OnMouseLeaveSeriesTchartMainShow2017, VTS_I4)

	ON_EVENT(CDlgShow, IDC_TCHART_MAIN_SHOW, 26, CDlgShow::OnUndoZoomTchartMainShow, VTS_NONE)
	ON_EVENT(CDlgShow, IDC_TCHART_MAIN_SHOW, 27, CDlgShow::OnZoomTchartMainShow, VTS_NONE)
END_EVENTSINK_MAP()


void CDlgShow::OnDblClickTchart1()
{

	CINScanLiteDlg *pDlg = (CINScanLiteDlg*)AfxGetApp()->GetMainWnd();

	if (chickline)
	{
		if(MessageBox(pDlg->LoadStringMeau(_T("ID_DATA_DELETE")), "",MB_YESNO)==IDYES)
		{
			if (nSeries!=0)
			{
				m_Tchart.RemoveSeries(nSeries);
			}
			else
			{
				m_Tchart.RemoveAllSeries();
				m_Tchart.Repaint();
				pDlg->SetWindowText(strSoftName);
				//pDlg->m_DlgFile.m_DlgDataPro.ClearList();
			}

		}	

	}
	m_Tchart.GetZoom().SetEnable(TRUE);
	chickline =FALSE;



}


void CDlgShow::OnClickSeriesTchart1(long SeriesIndex, long ValueIndex, long Button, long Shift, long X, long Y)
{
	double xvalue =m_Tchart.Series(0).XScreenToValue(X);
	nSeries=SeriesIndex;
	chickline=TRUE;
	/*if(is_HandMark==TRUE)
	{AddPointMarks(SeriesIndex,X,Y);}
	if (is_DelMark==TRUE)
	{
	DeleteChMarks(xvalue);
	}*/
}

BOOL CDlgShow::ImportFullLine(LPCTSTR txtFileName)
{
	CStdioFile file;
	CString txtFilename = txtFileName;
	int i;
	int nSeriesIndex = m_Tchart.AddSeries(0);
	//m_Tchart.Series(nSeriesIndex).SetColor(RGB(255,0,0));//自定义曲线颜色
	//int color = m_Tchart.Series(nSeriesIndex).GetColor();
	nSeriesIndex = nSeriesIndex >=0 ? nSeriesIndex : 0;
	int color = g_iRGB[nSeriesIndex%SERIES_MAX];
	m_Tchart.Series(nSeriesIndex).SetColor(color);
	CString str,pattern=txtFilename.Right(3)==_T("csv")?",":"\t";

	if (!file.Open(txtFilename,CFile::modeRead|CFile::typeText)) 
	{
		return FALSE;
	}  	

	bool isTxt = false;
	// 如果是txt格式的，则需要兼容\t  , 和空格这三类分隔符
	if(txtFilename.Right(3) == _T("txt"))
		isTxt = true;

	CStringArray sa;
	long saCount=0,nIndexCount=0,Is3D=0;
	CString strFileName = file.GetFileName().Left(file.GetFileName().GetLength()-4);
	m_Tchart.Series(nSeriesIndex).SetTitle(strFileName);
	//m_Tchart.Series(nSeriesIndex).SetActive(FALSE);
	while (file.ReadString(str))
	{
		int pos;
		CString tmp,tmpstr=str;
		tmpstr.TrimLeft();
		pos=tmpstr.Find(pattern);
		if (pos<1)
		{
			// 如果是txt, 需要添加判断pattern是否为 , 和 空格
			if(isTxt)
			{
				pattern = TEXT(",");
				pos=tmpstr.Find(pattern);
				if(pos<1)
				{
					pattern = TEXT(" ");
					pos=tmpstr.Find(pattern);
					if(pos<1)
						continue;
				}

			}
			//continue;
		}		
		tmp=tmpstr.Left(pos);
		//  判断是否添加了表头，如果有表头，则跳过该行
		if(atof(tmp) == 0)
			continue;

		tmpstr=tmpstr.Mid(pos+1);
		if (IsNumber(tmp)&&IsNumber(tmpstr))
		{			
			if(!tmp.IsEmpty()&&StrNumber(tmpstr)==0)
			{
				saCount=2;			

				break;
			}
			return FALSE;
		}
	}
	if(saCount==0){return FALSE;}
	file.Seek(-str.GetLength()-2,CFile::current );
	if(Is3D==0)
	{
		while (file.ReadString(str))
		{
			Split(str,pattern,sa);
			CString strtmpX,strtmpY;
			for(i=0;i<saCount/2;i++)
			{
				strtmpX=sa.GetAt(2*i);
				strtmpY=sa.GetAt(2*i+1);
				if(!strtmpX.IsEmpty())
				{

					m_Tchart.Series(nSeriesIndex).AddXY(atof(strtmpX),atof(strtmpY),NULL,color);
				}			
			}
			sa.RemoveAll();
		}
	}

	file.Close();

	OnSetAutoRepaint(TRUE);
	OnAutomatic();

	return TRUE;
}

BOOL CDlgShow::ExportTxtFile(CString strFilename)
{
	CINScanLiteDlg * pDlg = (CINScanLiteDlg *)AfxGetApp()->GetMainWnd();
	CString strData,str, header;//  lcz new 04/25 添加表头信息
	CString strinterval = strFilename.Right(3)==_T("csv")?",":"\t";
	CString strRight = strFilename.Right(3);
	CString strLeft  = strFilename.Left(strFilename.GetLength()-4);
	CStdioFile file;
	CString tmp;
	long serierCounts=m_Tchart.GetSeriesCount();
	for (int k = 0;k<serierCounts;k++)
		//int k = serierCounts-1;
	{
		CString newfilePathName = strFilename;
		// 		newfilePathName.Format(_T("%s-%d.%s"),strLeft,k,strRight);
		if (m_Tchart.Series(k).GetActive())
		{
			if (!file.Open(newfilePathName,CFile::modeCreate|CFile::modeWrite)) 
			{
				str=pDlg->LoadStringMeau(_T("ID_FILE_FAIL"));
				AfxMessageBox(str,MB_ICONWARNING);
				return FALSE;
			}
			//  添加表头信息
			if (sInfo.nLanguage == 0 && sTInfo.AddHeader)
			{	
				header.Format("%s%s%.1f%s\n", TEXT("激光波长："), strinterval, sCInfo.Laser, TEXT(" nm"));
				file.WriteString(header);
				if(strinterval == TEXT("\t"))//  如果是txt格式
				{
					header.Format("%s%s%s%d%s\n", TEXT("占 空 比："), strinterval, strinterval, (int)sTInfo.JPower, TEXT("\%"));
					file.WriteString(header);
				}
				else
				{
					header.Format("%s%s%d%s\n", TEXT("占 空 比："), strinterval, (int)sTInfo.JPower, TEXT("\%"));
					file.WriteString(header);
				}
				header.Format("%s%s%.2f%s\n", TEXT("积分时间："), strinterval, sTInfo.JTime, TEXT(" s"));
				file.WriteString(header);
				header.Format("%s%s%d\n", TEXT("累计次数："), strinterval, sTInfo.JAccNum);
				file.WriteString(header);
				header.Format("%s%s%d\n", TEXT("像素点数："), strinterval, sInfo.sDInfo.nGXPix);
				file.WriteString(header);
				header.Format("%s%s%d%s%d%s\n", TEXT("波数范围："), strinterval, (int)sSInfo.XLeft, TEXT("~"), (int)sSInfo.XRight, TEXT(" cm-1"));
				file.WriteString(header);
			}
			else if(sInfo.nLanguage == 1 && sTInfo.AddHeader)
			{// Laser wavelength：Duty ratio：Integration time：Cumulative number：Number of pixels：Wavenumber range：
				header.Format("%s%s%.1f%s\n", TEXT("Laser wavelength："), strinterval, sCInfo.Laser, TEXT(" nm"));
				file.WriteString(header);
				header.Format("%s%s%d%s\n", TEXT("Duty ratio："), strinterval, (int)sTInfo.JPower, TEXT("\%"));
				file.WriteString(header);
				header.Format("%s%s%.2f%s\n", TEXT("Integration time："), strinterval, sTInfo.JTime, TEXT(" s"));
				file.WriteString(header);
				header.Format("%s%d\n", TEXT("Cumulative number："), sTInfo.JAccNum);
				file.WriteString(header);
				header.Format("%s%s%d\n", TEXT("Number of pixels："), strinterval, sInfo.sDInfo.nGXPix);
				file.WriteString(header);
				header.Format("%s%d%s%d%s\n", TEXT("Wavenumber range："), (int)sSInfo.XLeft, TEXT(" ~ "), (int)sSInfo.XRight, TEXT(" cm-1"));
				file.WriteString(header);
			}

			double x,y;

			long counts[500];
			long maxCounts=0;
			int maxIndex=0;
			long ActiveSer[500];
			int i=0;

			CValueList list_x=m_Tchart.Series(k).GetXValues();
			CValueList list_y=m_Tchart.Series(k).GetYValues();
			for (int j=0;j<list_x.GetCount();j++)
			{
				x=list_x.GetValue(j);
				y=list_y.GetValue(j);

				str.Format("%f%s%f\n",x,strinterval,y);

				file.WriteString(str);
			}


			Sleep(1000);
			file.Close();

		}
	}


	return TRUE;
}

int CDlgShow::ExportTxtFiles(CString fileFolderPath,CString fileExt, bool bForce)
{
	CINScanLiteDlg * pDlg = (CINScanLiteDlg *)AfxGetApp()->GetMainWnd();
	CString strData,str;
	CString strinterval = fileExt==_T("csv")?",":"\t";
	CString strRight = fileExt;
	CStdioFile file;
	CString tmp;
	long serierCounts=m_Tchart.GetSeriesCount();
	for (int k = 0;k<serierCounts;k++)
	{

		if (m_Tchart.Series(k).GetActive())
		{
			CString newfilePathName;
			bool bExist = true;
			do
			{
				newfilePathName.Format(_T("%s//%s.%s"),fileFolderPath,m_Tchart.Series(k).GetTitle(),fileExt);
				CFileFind fFind;
				if(!bForce)
				{
					if(fFind.FindFile(newfilePathName))
					{
						CString strLog = newfilePathName + pDlg->LoadStringMeau(_T("ID_RENAME_FILE"));
						if(MessageBox(strLog, NULL, MB_YESNO) == IDYES)
						{
							CSetNameDlg dlg(this, m_Tchart.Series(k).GetTitle());
							dlg.DoModal();
						}
						else
							break;
					}
					else
						bExist = false;
				}
			}while(bExist);


			if (!file.Open(newfilePathName,CFile::modeCreate|CFile::modeWrite | CFile::shareExclusive)) 
			{
				str=pDlg->LoadStringMeau(_T("ID_FILE_FAIL"));
				AfxMessageBox(str,MB_ICONWARNING);
				return 0;
			}

			double x,y;

			long counts[500];
			long maxCounts=0;
			int maxIndex=0;
			long ActiveSer[500];
			int i=0;

			CValueList list_x=m_Tchart.Series(k).GetXValues();
			CValueList list_y=m_Tchart.Series(k).GetYValues();
			for (int j=0;j<list_x.GetCount();j++)
			{
				x=list_x.GetValue(j);
				y=list_y.GetValue(j);

				str.Format("%f%s%f\n",x,strinterval,y);

				file.WriteString(str);
			}


			Sleep(1000);
			file.Close();

		}
	}


	return 1;
}
void CDlgShow::OnMouseMoveTchart1(long Shift, long X, long Y)
{
	//摘抄后续修改
	int lc = m_Tchart.GetSeriesCount();
	int count = m_Tchart.Series(nSeries).GetCount();
	if (lc<1&&count<5)
	{
		return;
	}

	int in_x = 0;
	double nowx,nowy;
	nowx =m_Tchart.Series(nSeries).XScreenToValue(X);
	CValueList xv = m_Tchart.Series(nSeries).GetXValues();

	if (nowx<m_Tchart.Series(nSeries).GetXValues().GetMinimum()||nowx>m_Tchart.Series(nSeries).GetXValues().GetMaximum())
	{
		return;
	}
	double offset = xv.GetValue(1)-xv.GetValue(0);/////计算两个点之间的距离
	if (offset<0)
	{
		offset= -1*offset;
	}
	for (int i=0;i<count;i++)
	{
		//if (xv.GetValue(i)>=nowx-offset&&xv.GetValue(i)<=nowx+offset)
		if(nowx>=xv.GetValue(i)&&nowx<=xv.GetValue(i)+offset)
		{
			if (xv.GetValue(i)+offset/2>=nowx)
			{
				in_x=i;
			}
			else
			{
				if (i!=count-1)///最后一个点
				{
					in_x=i+1;
				}
				else
				{
					in_x = i;
				}

			}
			break;
		}
	}
	CValueList yv = m_Tchart.Series(nSeries).GetYValues();

	nowy =yv.GetValue(in_x);
	nowx =xv.GetValue(in_x);

	CString str;
	str.Format("X=%.1f,Y=%.1f",nowx,nowy);


	CINScanLiteDlg *pDlg = (CINScanLiteDlg*)AfxGetApp()->GetMainWnd();
	pDlg->GetDlgItem(IDC_STATIC_XY)->SetWindowText(str);

}
void CDlgShow::OnSmooth()
{
	int index= m_Tchart.GetSeriesCount();
	if (index<1)return;
	ReMoveSeries(1);
	double yv = 0.00;
	yv=yv+1-1;

	int b = m_Tchart.Series(0).GetCount();
	if (b>2)
	{		
		CValueList xl = m_Tchart.Series(0).GetXValues();
		CValueList yl = m_Tchart.Series(0).GetYValues();

		for (int i=1;i<index;i++)
		{
			m_Tchart.RemoveSeries(i);
		}

		int co = yl.GetCount();
		double *y = new double[co+1];

		for (int j=0;j<co;j++)
		{
			y[j] =  yl.GetValue(j);
		}
		JohnSmooth(co,y);

		for (int j=0;j<co;j++)
		{
			yv= y[j];
			yl.SetValue(j,yv);
		}
		delete[] y;

	}
}
void CDlgShow::JohnSmooth(SHORT LineSize, DOUBLE* YArray)
{
	double nm=0;
	double value=0;
	double len=1;
	for(int i=0;i<LineSize;i++)
	{
		if(i<9)
		{
			nm=nm+YArray[i];
			len=i+1;
		}
		else
		{
			nm=0;                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                    
			for(int j=i-5;j<i+5;j++)
			{
				if (j<LineSize-5)
				{
					nm=nm+YArray[j];
				}
				else
				{
					nm=0;
					for(int j=i;j>i-10;j--)
					{
						nm=nm+YArray[j];
					}
					len=10;
				}
			}
			len=10;
		}
		value=nm/len;
		YArray[i]=value;
	}
}
void CDlgShow::Nihe(int cvalue)
{

	int sercont = m_Tchart.GetSeriesCount();
	if (sercont>=1)
	{
		int cont = m_Tchart.Series(0).GetCount();
		if (cont>20)
		{	
			PolynomSubChart(1,cvalue);

		}
	}
}

void CDlgShow::PolynomSubChart(int width,int degree)
{
	//	baseline_corrects
	CINScanLiteDlg *pDlg= (CINScanLiteDlg *)AfxGetApp()->GetMainWnd();
	long serierCounts=0;
	serierCounts=m_Tchart.GetSeriesCount();
	if (serierCounts>1)
	{
		for (int a=0;a<serierCounts-1;a++)
		{
			m_Tchart.RemoveSeries(1);
		}

	}
	if (serierCounts>0)
	{

		CValueList list_y=m_Tchart.Series(0).GetYValues();
		CValueList list_x=m_Tchart.Series(0).GetXValues();
		SaveLineValue();
		niheVector.clear();
		int linesize =list_x.GetCount();
		double *yarray,*xarray,*farray;
		//xarray=(double *)calloc(linesize,sizeof(double));
		//yarray=(double *)calloc(linesize,sizeof(double)); 
		//farray=(double *)calloc(10,sizeof(double));
		xarray = new double[linesize];
		yarray = new double[linesize];
		farray = new double[linesize];
		memset(xarray, 0.0, sizeof(double) *linesize );
		memset(yarray, 0.0, sizeof(double) *linesize );
		memset(farray, 0.0, sizeof(double) *linesize );
		double testx=0;
		double testy=0;
		for(int i=0;i<linesize;i++)
		{
			xarray[i]=list_x.GetValue(i);
			testx = xarray[i];
			yarray[i]=list_y.GetValue(i);
			testy = yarray[i];
		}
		pDlg->m_polyfit.ZolixProcessing(linesize,xarray,yarray,degree);

		double valuey=0;
		double valuex=0;
		double valuef = 0;
		int flag=m_Tchart.AddSeries(0);
		m_Tchart.Series(flag).GetAsLine().GetLinePen().SetWidth(width);
		//int  color = m_Tchart.Series(flag).GetColor();
		flag = flag >=0 ? flag : 0;
		int color = g_iRGB[flag%SERIES_MAX];
		for (int j=0;j<linesize;j++)
		{
			//valuef = farray[j];
			valuey = yarray[j];
			niheVector.push_back(valuey);
			valuex = list_x.GetValue(j);
			m_Tchart.Series(flag).AddXY(valuex,valuey,_T(""),color);
		}
		if(xarray)
		{
			delete[] xarray;
			xarray=NULL; 
		}
		if(yarray)
		{
			delete[] yarray;
			yarray=NULL;
		}
		if(farray)
		{
			delete[] farray;
			farray=NULL;
		}
	}
}

void CDlgShow::SaveLineValue()
{
	CValueList XLIST =m_Tchart.Series(0).GetXValues();
	CValueList YLIST =m_Tchart.Series(0).GetYValues();
	XVector.clear();
	YVector.clear();
	int xlength = YLIST.GetCount();
	double x,y;
	for (int i=0;i<xlength;i++)
	{
		x=XLIST.GetValue(i);
		XVector.push_back(x);
		y=YLIST.GetValue(i);	
		YVector.push_back(y);
	}
}
void CDlgShow::ReMoveSeries(int nR)
{	
	int size=m_Tchart.GetSeriesCount();

	/*while(m_Tchart.GetSeriesCount()>nR)
	{
	for (int i=nR;i<size;i++)
	{
	m_Tchart.RemoveSeries(i);
	}
	}*/
	m_Tchart.RemoveAllSeries();

	OnAutomatic();

}
void CDlgShow::Recover()
{
	double x,y;
	int xlength =XVector.size() ;
	m_Tchart.RemoveAllSeries();
	int flag=m_Tchart.AddSeries(0);
	int color = m_Tchart.Series(flag).GetColor();
	for (int i=0;i<xlength;i++)
	{
		x = XVector[i];
		y = YVector[i];
		m_Tchart.Series(flag).AddXY(x,y,_T(""),clrSet);
	}
	XVector.clear();
	YVector.clear();
}
void CDlgShow::clearline()
{
	long serierCounts=0;
	serierCounts=m_Tchart.GetSeriesCount();
	if (serierCounts>0)
	{
		for (int i=0;i<serierCounts;i++)
		{
			if (i>0)
			{
				m_Tchart.RemoveSeries(1);
			}
		}
	}
}
int CDlgShow::GetCount()
{
	return m_Tchart.GetSeriesCount();
}

void CDlgShow::subline()
{

	CValueList list_x1=m_Tchart.Series(0).GetXValues();
	CValueList list_y1=m_Tchart.Series(0).GetYValues();
	CValueList list_y2=m_Tchart.Series(1).GetYValues();
	int linesize1 = list_y1.GetCount();
	int linesize2 = list_y2.GetCount();
	//	pThreadApp->m_AndorView->m_Tchart.RemoveAllSeries();
	int flag=m_Tchart.AddSeries(0);
	int color = m_Tchart.Series(flag).GetColor();
	if (linesize1==linesize2)
	{
		double ych;
		double xx;
		for (int i=0;i<linesize2;i++)
		{
			xx = list_x1.GetValue(i);
			ych = list_y1.GetValue(i)-list_y2.GetValue(i);
			m_Tchart.Series(flag).AddXY(xx,ych,_T(""),clrSet);
		}
		m_Tchart.RemoveSeries(0);
		m_Tchart.RemoveSeries(0);
	}

}
void CDlgShow::AddPointMarks(long SeriesIndex,long x,double y)
{
	////先判断 防止标峰出界
	int sercon =m_Tchart.GetSeriesCount();
	BOOL isnum=FALSE;
	for (int i=0;i<sercon;i++)
	{
		int p = m_Tchart.Series(i).GetCount();
		if (p==1)
		{
			isnum=TRUE;
			break;
		}
	}
	if (isnum==FALSE)
	{
		double max_yvalue = m_Tchart.Series(0).GetYValues().GetMaximum();
		double min_yvalue = m_Tchart.Series(0).GetYValues().GetMinimum();
		//lim_ydata = (max_yvalue - min_yvalue)*in_maxvalue/100+min_yvalue;
		m_Tchart.GetAxis().GetLeft().SetAutomaticMaximum(FALSE);
		m_Tchart.GetAxis().GetLeft().SetMaximum(max_yvalue+(max_yvalue-min_yvalue)/2);
	} 


	int al = m_Tchart.AddSeries(0);
	double minxvalue=0.00;
	double maxxvalue=0.00;
	double rxvalue= 0.00;
	double ryvalue = 0.00;

	std::vector<double> idex,idey;
	/////添加鼠标点击的X范围判断
	minxvalue = m_Tchart.Series(SeriesIndex).XScreenToValue(x-10);
	maxxvalue = m_Tchart.Series(SeriesIndex).XScreenToValue(x+10);

	CValueList xl = m_Tchart.Series(SeriesIndex).GetXValues();
	CValueList yl = m_Tchart.Series(SeriesIndex).GetYValues();
	int linecount = m_Tchart.Series(SeriesIndex).GetCount();
	for (int i=0;i<linecount;i++)
	{

		if (xl.GetValue(i)>=minxvalue&&xl.GetValue(i)<=maxxvalue)
		{
			if (ryvalue<yl.GetValue(i))
			{
				rxvalue = xl.GetValue(i);
				ryvalue = yl.GetValue(i);
			}

		}
	}
	///判断结束

	m_Tchart.Series(al).AddXY(rxvalue,ryvalue,NULL,RGB(255,0,0));
	m_Tchart.Series(al).GetMarks().SetBackColor(RGB(255,255,255));
	m_Tchart.Series(al).GetMarks().SetTextAlign(0);			///标记点的左侧 当前是竖直状态所以要左侧 如果是水平应该是中心
	m_Tchart.Series(al).GetMarks().GetFont().SetSize(10);	////标记点的文本大小
	m_Tchart.Series(al).GetMarks().GetFont().SetBold(TRUE); ///数据加粗
	m_Tchart.Series(al).GetMarks().SetTransparent(FALSE);	///不加框
	m_Tchart.Series(al).GetMarks().SetArrowLength(50);
	m_Tchart.Series(al).GetMarks().GetArrow().SetMode(0);
	/*m_Tchart.Series(al).GetMarks().GetArrow().SetStyle(0);*/
	m_Tchart.Series(al).GetMarks().GetArrow().SetColor(RGB(0,0,0));
	m_Tchart.Series(al).GetMarks().GetArrow().SetStyle(1);
	m_Tchart.Series(al).GetMarks().GetArrow().SetWidth(1);
	//	m_Tchart.Series(al).GetMarks().GetArrow().SetEndStyle(3);
	m_Tchart.Series(al).GetMarks().GetCallout().SetArrowHead(1);////设置箭头样式
	m_Tchart.Series(al).GetMarks().GetCallout().SetArrowHeadSize(6);///箭头大小
	m_Tchart.Series(al).GetMarks().GetArrow().SetVisible(TRUE);

	m_Tchart.Series(al).GetMarks().SetStyle(8);//8显示的是X 
	m_Tchart.Series(al).GetMarks().SetAngle(90);//标签旋转90度
	m_Tchart.Series(al).GetMarks().SetVisible(TRUE);	
}

//////标峰
void CDlgShow::MarksValue(int in_width,int in_maxvalue )
{

	int SeriesCount=0;
	ReMoveSeries(1);
	SeriesCount = m_Tchart.GetSeriesCount();

	double lim_ydata = 0.00;
	if (SeriesCount<1)
	{
		return;
	} 
	else
	{
		///开始标峰
		int a = m_Tchart.GetSeriesCount();
		if(a>1)
		{
			for (int w=0;w<a-1;w++)
			{
				m_Tchart.RemoveSeries(1);
			}
		}


		double yvalue = 0.00;
		//	for (int i=0;i<a;i++)
		//{
		double max_yvalue = m_Tchart.Series(0).GetYValues().GetMaximum();
		double min_yvalue = m_Tchart.Series(0).GetYValues().GetMinimum();
		lim_ydata = (max_yvalue - min_yvalue)*in_maxvalue/100+min_yvalue;
		m_Tchart.GetAxis().GetLeft().SetAutomaticMaximum(FALSE);
		m_Tchart.GetAxis().GetLeft().SetMaximum(max_yvalue+(max_yvalue-min_yvalue)/2);
		CValueList ylist = m_Tchart.Series(0).GetYValues();
		CValueList xlist =m_Tchart.Series(0).GetXValues();
		int cont = m_Tchart.Series(0).GetCount();
		//double *qd_value = new double[cont];///强度后的值
		std::vector<double> qd_value;
		qd_value.clear();
		////////// 趋势   增大:1, 减小:2, 峰值:3, 保持:0, 无效:-1
		for (int j=0;j<cont;j++)
		{
			yvalue = ylist.GetValue(j);
			if (j==0)
			{
				if (ylist.GetValue(0)>ylist.GetValue(1))
				{
					//qd_value[j]=2;
					qd_value.push_back(2);
				} 
				else if(ylist.GetValue(0)<ylist.GetValue(1))
				{
					//qd_value[j]=1;
					qd_value.push_back(1);
				}
				else
				{
					//qd_value[j]=0;
					qd_value.push_back(0);
				}
			}
			else
			{
				if(ylist.GetValue(j)>ylist.GetValue(j-1))
				{
					//qd_value[j]=1; 
					qd_value.push_back(1);
				} 
				else if(ylist.GetValue(j)<ylist.GetValue(j-1))
				{
					// qd_value[j]=2;
					qd_value.push_back(2);
				}
				else
				{
					//qd_value[j]=0;
					qd_value.push_back(0);
				}
			}
		}
		/////到此  数组里应该是 0  1  2 三种数据
		//double *max_value = new double[cont];///用来放峰的位置
		std::vector<double> maxnum;
		maxnum.clear();
		for (int p=1;p<cont-1;p++)
		{

			if (qd_value[p]==1&&qd_value[p+1]==2)
			{
				//qd_value[p]=3;
				qd_value[p]=3;
				//max_value[midmax] = p;
				maxnum.push_back(p);	
			}
		}
		//}
		////开始标峰
		int maxsize = maxnum.size();
		in_width = (int)((in_width-1)/2);
		for (int q=0;q<maxsize;q++)
		{
			int ddx = maxnum[q];
			double newxv = xlist.GetValue(ddx);
			double newyv = ylist.GetValue(ddx);
			if (newyv>lim_ydata)//首先判断强度是否满足 然后判断
			{
				///开始判断宽度	
				if (ddx>=in_width&&in_width<=cont-ddx)//先判断和起始点和终止点的距离的距离
				{
					int bj=0;
					for (int k=1;k<in_width+1;k++)
					{
						////修改程序  此处应该是数据宽度大于规定宽度
						//if(qd_value.GetAt(ddx+k)-qd_value.GetAt(ddx-k)!=1)
						if(ylist.GetValue(ddx+k)>newyv||newyv<ylist.GetValue(ddx-k))
						{
							bj=1;
							break;
						}

					}
					if (bj==0)
					{
						//////加点

						if (m_Tchart.GetSeriesCount()<30)
						{

							int al = m_Tchart.AddSeries(0);
							m_Tchart.Series(al).AddXY(newxv,newyv,NULL,RGB(255,0,0));
							m_Tchart.Series(al).GetMarks().SetBackColor(RGB(255,255,255));
							m_Tchart.Series(al).GetMarks().SetTextAlign(0);			///标记点的左侧 当前是竖直状态所以要左侧 如果是水平应该是中心
							m_Tchart.Series(al).GetMarks().GetFont().SetSize(8);	////标记点的文本大小
							m_Tchart.Series(al).GetMarks().GetFont().SetBold(TRUE); ///数据加粗
							m_Tchart.Series(al).GetMarks().SetTransparent(FALSE);	///不加框
							m_Tchart.Series(al).GetMarks().SetArrowLength(50);
							m_Tchart.Series(al).GetMarks().GetArrow().SetMode(0);

							m_Tchart.Series(al).GetMarks().GetArrow().SetColor(RGB(0,0,0));
							m_Tchart.Series(al).GetMarks().GetArrow().SetStyle(1);
							m_Tchart.Series(al).GetMarks().GetArrow().SetWidth(1);

							m_Tchart.Series(al).GetMarks().GetCallout().SetArrowHead(1);////设置箭头样式
							m_Tchart.Series(al).GetMarks().GetCallout().SetArrowHeadSize(6);///箭头大小
							m_Tchart.Series(al).GetMarks().GetArrow().SetVisible(TRUE);

							m_Tchart.Series(al).GetMarks().SetStyle(8);//8显示的是X 
							m_Tchart.Series(al).GetMarks().SetAngle(90);//标签旋转90度
							m_Tchart.Series(al).GetMarks().SetVisible(TRUE);
						}

					}
				}
			}

		}
		//	delete[] qd_value;

	}
}
void CDlgShow::OnSetAutoRepaint(BOOL bSet)
{
	if( !g_bOpenCamera)// 如果电机复位了，才开始重新绘图
	{
		m_Tchart.SetAutoRepaint(bSet);
		m_Tchart.Repaint();
	}
	else
	{
		m_Tchart.SetAutoRepaint(FALSE);
	}
}

void CDlgShow::OnAutomatic()
{

	m_Tchart.GetAxis().GetLeft().SetAutomatic(TRUE);
	m_Tchart.GetAxis().GetBottom().SetAutomatic(TRUE);
	m_Tchart.GetAxis().GetRight().SetAutomatic(TRUE);
	m_Tchart.GetAxis().GetTop().SetAutomatic(TRUE);
}

void CDlgShow::DeleteShx()
{
	if (GetCount()<1)
		return;

	for (int i=0;i<2;i++)
	{
		int count = m_Tchart.GetSeriesCount();
		for (int j=0;j<count;j++)
		{
			int convalue = m_Tchart.Series(j).GetCount();
			if (convalue==2)
			{
				m_Tchart.RemoveSeries(j);
				break;
			}
		}
	}
}

void CDlgShow::OnClickBackgroundTchart1(long Button, long Shift, long X, long Y)
{

	/*double xvalue =m_Tchart.Series(0).XScreenToValue(X);
	if(is_HandMark==TRUE){
	AddPointMarks(Shift,X,Y);}
	if (is_DelMark==TRUE){
	DeleteChMarks(xvalue);}*/
}
void CDlgShow::DeleteChMarks(double xvalue)
{
	int a = m_Tchart.GetSeriesCount();
	for (int i=0;i<a;i++)
	{
		int co = m_Tchart.Series(i).GetCount();
		if (co==1)
		{  
			CValueList xx;
			xx  = m_Tchart.Series(i).GetXValues();
			double xv = xx.GetValue(0);
			if (xv>xvalue-5&&xv<=xvalue+5)
			{
				m_Tchart.RemoveSeries(i);
				break;
			}
		}
	}
}
void CDlgShow::OriginalData(double * intensity)
{

	CValueList list_y=m_Tchart.Series(sSInfo.SeriesNum).GetYValues();
	for (int i=0;i<list_y.GetCount();i++)
	{
		intensity[i]=list_y.GetValue(i);
	}
}
void CDlgShow::OriginalDataX(double * intensity)
{
	CValueList list_x=m_Tchart.Series(sSInfo.SeriesNum).GetXValues();
	for (int i=0;i<list_x.GetCount();i++)
	{
		intensity[i]=list_x.GetValue(i);
	}
}
//  连接数据库
BOOL CDlgShow::SelectSql()
{
	if (GetCount()<1)
	{
		return FALSE;
	}
	AfxBeginThread(SELECTSQLNEW,(LPVOID)this);
	return TRUE;
}
void CDlgShow::AddSqlSer(int nChoose)
{
	OnAutomatic();
	DWORD len=0;
	COleSafeArray strx;
	COleSafeArray stry;
	strx.Clear();
	stry.Clear();
	len=sDInfo[nChoose].datalen;
	strx.Create(VT_R8, 1, &len); 
	stry.Create(VT_R8, 1, &len); 
	Split(sDInfo[nChoose].strXData,"\t",strx);
	Split(sDInfo[nChoose].strYData,"\t",stry);
	m_Tchart.AddSeries(0);
	int count=m_Tchart.GetSeriesCount();
	m_Tchart.Series(count-1).AddArray(len,stry,strx);
}

CString GetCurTimeFile()
{
	SYSTEMTIME st;

	CString strDate, strTime;
	GetLocalTime(&st);

	strDate.Format(_T("%s_%4d%02d%02d"),sINInfo.strName, st.wYear, st.wMonth, st.wDay);
	strTime.Format(_T("_%02d%02d%02d"), st.wHour, st.wMinute, st.wSecond);

	return (strDate  + strTime);
}

CString CDlgShow::FileNameFormat(CString strLineName)
{
	CString strName;
	//样品名称_激光功率_积分时间_测试时间.txt
	strName.Format(_T("-%s-%.0f%%-%.2fs"),strLineName, sTInfo.JPower, sTInfo.JTime);
	strName = GetCurTimeFile() + strName;
	return strName;
}

void CDlgShow::AutoSaveFile(CString strLineName)
{
	CString str;

	if (sINInfo.strPath==_T("")||sINInfo.strPath==_T("0"))
	{
		sINInfo.strPath=sInfo.Path+_T("\\Data");
	}

	if(!PathIsDirectory(sINInfo.strPath))
	{
		::CreateDirectory(sINInfo.strPath,0);
	}

	CString strExt = sINInfo.nFileType==0?_T(".txt"):_T(".csv");
	CString strName = FileNameFormat(strLineName);
	//str=sINInfo.strPath+GetCurTimeFile();
	str = sINInfo.strPath + _T("\\")+ strName + strExt;
	ExportTxtFile(str);
}
double CDlgShow::CaulNum(double lValueX)
{

	double lReNum=0.01;
	BOOL bRet=FALSE;
	//校正文件读取一次后 更新需要再重新读。在导入校正导入成功后更新校正状态。加入变量判断
	bRet=LoadFileData(sInfo.strPathOffset,
		&sOffsetFileInfo.FscDataNum,
		sOffsetFileInfo.dFscDataX,
		sOffsetFileInfo.dFscDataY);

	if (bRet==TRUE)
	{
		lReNum=CaulRealNum(lValueX,sOffsetFileInfo.dFscDataX,
			sOffsetFileInfo.dFscDataY,
			&sOffsetFileInfo.FscDataNum);
	}
	if (lReNum==0)
	{
		lReNum=0.01;
	}
	return lReNum;
}

double CDlgShow::ScanOffset(double x,double y)
{
	double Ydata=y;
	if(sSInfo.CheckOffset)
	{
		if (x>=sOffsetFileInfo.dFscDataX[0]&&x<=sOffsetFileInfo.dFscDataX[sOffsetFileInfo.FscDataNum-1])
		{ 
			Ydata=Ydata*pDlg->CaulNum(x);
		}
	}
	return Ydata;
}

void CDlgShow::OnClickLegendTchartMainShow(long Button, long Shift, long X, long Y)
{
	return;

	CINScanLiteDlg * pDlg=(CINScanLiteDlg*)AfxGetApp()->GetMainWnd();
	if (pDlg->pFitSmoothDlg->GetSafeHwnd()==NULL&&
		pDlg->pSmoothDlg->GetSafeHwnd()==NULL&&
		pDlg->pCutLineDlg->GetSafeHwnd()==NULL)
	{
		//if (pDlg->pFitSmoothDlg.GetSafeHwnd()==NULL&&
		//	pDlg->pSmoothDlg.GetSafeHwnd()==NULL&&
		//	pDlg->pCutLineDlg.GetSafeHwnd()==NULL)
		//{
		if (Button==2)
		{
			
			nCurChartLine=m_Tchart.GetLegend().Clicked(X,Y);
			m_Tchart.Series(nCurChartLine).SetActive(1);

			CRect chartRect;
			m_Tchart.GetWindowRect(&chartRect);
			CMenu menu;
			if (sInfo.nLanguage==0)
			{
				VERIFY(menu.LoadMenu(IDR_POPUP_MENU_CH1));
			}
			else
			{
				VERIFY(menu.LoadMenu(IDR_POPUP_MENU_EN1));
			}

			CMenu* pPopup = menu.GetSubMenu(0);
			ASSERT(pPopup != NULL);
			CWnd* pWndPopupOwner = this;
			while (pWndPopupOwner->GetStyle() & WS_CHILD)
				pWndPopupOwner = pWndPopupOwner->GetParent();
			pPopup->TrackPopupMenu(TPM_RIGHTALIGN | TPM_RIGHTBUTTON, chartRect.left+X,chartRect.top+Y,
				pWndPopupOwner);
		}
	}

}

void CDlgShow::OnUpdateName()
{
	// TODO: 在此添加命令处理程序代码
	nCurChartLine;
	//MessageBox(_T("重命名"));
}

void CDlgShow::updateName(CString strUpdateName,int nSer)
{
	m_Tchart.Series(nSer).SetTitle(strUpdateName);
}

void CDlgShow::updateName(CString strUpdateName,CString strOldName)
{
	int iCount = m_Tchart.GetSeriesCount();
	while(iCount >0)
	{
		CString strCurName = m_Tchart.Series(iCount-1).GetTitle();
		CString strTemp = strCurName+"-";
		if(/*strCurName.CompareNoCase(strOldName)==0 || */strCurName.Find(strOldName)>=0)
		{
			strCurName.Replace(strOldName, strUpdateName);
			m_Tchart.Series(iCount-1).SetTitle(strCurName);
			//break;
		}
		iCount--;
	}

}

void CDlgShow::showLine(CString strName)
{
	int iCount = m_Tchart.GetSeriesCount();
	while(iCount >0)
	{
		CString strCurName = m_Tchart.Series(iCount-1).GetTitle();
		if(/*strCurName.CompareNoCase(strOldName)==0 || */strCurName.Find(strName)>=0)
		{
			m_Tchart.Series(iCount-1).SetActive(true);
			//break;
		}
		iCount--;
	}
}

void CDlgShow::setColor(int R,int G,int B,int nNum)
{
	long color=m_Tchart.Series(nNum).GetColor();
	CColorDialog dlg;

	if(dlg.DoModal()==IDOK)
	{
		color=dlg.GetColor();
	}
	m_Tchart.Series(nNum).SetColor(color);
	for (int i=0;i<m_Tchart.Series(nNum).GetCount();i++)
	{
		m_Tchart.Series(nNum).SetPointColor(i,color);
	}

	//COLORREF clrSet=RGB(R, G, B);
	//m_Tchart.Series(nCurChartLine).SetColor(clrSet);
	//for (int i=0;i<m_Tchart.Series(nCurChartLine).GetCount();i++)
	//{
	//	m_Tchart.Series(nCurChartLine).SetPointColor(i,clrSet);
	//}
}

void CDlgShow::del(int nSer)
{
	m_Tchart.RemoveSeries(nSer);
}

void CDlgShow::delAll()
{
	//while(m_Tchart.GetSeriesCount()>0){
	//	m_Tchart.RemoveAllSeries();
	//}

	// 不删除所有曲线，保留一条
	CINScanLiteDlg * pDlg=(CINScanLiteDlg*)AfxGetApp()->GetMainWnd();
	// 删除已勾选曲线
			for(int i = 0; i < pDlg->m_DlgShow.GetTeeCount(); )
			{
				int c = pDlg->m_DlgShow.GetTeeCount();
				//if(pDlg->m_DlgShow.m_Tchart.Series(i).GetActive())
				//{
					//// 如果所有都被勾选，删除到还剩一条曲线的时候不删除，显示图例为"Legend List"，图线数据为空
					//if( c == 1)
					//{
					//	pDlg->m_DlgShow.m_Tchart.Series(i).SetActive(false);//  不勾选
					//	pDlg->m_DlgShow.m_Tchart.Series(i).Clear();
					//	pDlg->m_DlgShow.m_Tchart.Series(i).SetLegendTitle(TEXT("Legend List"));
					//	//pDlg->m_DlgShow.m_Tchart.GetLegend().SetLegendStyle(2);
					//	return;
					//}
					//else
						pDlg->m_DlgShow.m_Tchart.RemoveSeries(i);
				//}
				//else
				//	i++;
			}

}

void CDlgShow::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if(nIDEvent == 1)
	{
		double dX = m_Tchart.GetMousePosition().GetX();
		double dY = m_Tchart.GetMousePosition().GetY();
		CString str;
		str.Format("X=%.1f,Y=%.1f", dX, dY);
		//SetDlgItemText(IDC_STATIC_XY, str);
		TRACE("          XY:%s\n", str);
	}
	CDialog::OnTimer(nIDEvent);
}

// 鼠标按下 触发事件
void CDlgShow::OnMouseDownTchartMainShow(long Button, long Shift, long X, long Y)
{
	// TODO: 在此处添加消息处理程序代码
	m_iMouseX = X;
}

//长按鼠标触发事件
void CDlgShow::OnMouseUpTchartMainShow(long Button, long Shift, long X, long Y)
{
	// TODO: 在此处添加消息处理程序代码
	// 	if(X < m_iMouseX)
	// 	{
	// 		m_Tchart.GetAxis().GetLeft().SetAutomatic(TRUE);
	// 		m_Tchart.GetAxis().GetBottom().SetAutomatic(TRUE);
	// 		m_Tchart.SetAutoRepaint(TRUE);
	// 		m_Tchart.RefreshData();
	// 		m_Tchart.Repaint();
	// 	}
}

//  鼠标放在框图上触发事件，如果放在曲线上，则在底部显示X、Y值
void CDlgShow::OnMouseMoveTchartMainShow(long Shift, long X, long Y)
{
	// TODO: 在此处添加消息处理程序代码
	double fX=0.0;
	double fY=0.0;
	CINScanLiteDlg * pDlg = (CINScanLiteDlg *)AfxGetApp()->GetMainWnd();
	if(m_bEnterSeries)
	{
		if(m_iEnterSeriesIndex >= 0)
		{
			fX = m_Tchart.Series(m_iEnterSeriesIndex).XScreenToValue(X);
			fY = m_Tchart.Series(m_iEnterSeriesIndex).YScreenToValue(Y);
			pDlg->ShowMouseXY(fX, fY);
		}
	}
}

//  选择曲线时触发事件（鼠标放在曲线上触发事件）
void CDlgShow::OnMouseEnterSeriesTchartMainShow2017(long SeriesIndex)
{
	// TODO: 在此处添加消息处理程序代码
	m_iEnterSeriesIndex = SeriesIndex;
	m_bEnterSeries = true;
}

//  鼠标离开曲线，触发事件
void CDlgShow::OnMouseLeaveSeriesTchartMainShow2017(long SeriesIndex)
{
	// TODO: 在此处添加消息处理程序代码
	m_bEnterSeries = false;
	m_iEnterSeriesIndex = -1;
	CINScanLiteDlg * pDlg = (CINScanLiteDlg *)AfxGetApp()->GetMainWnd();
	pDlg->ShowMouseXY(0.0, 0.0);
}

//  鼠标斜拉缩放表格
void CDlgShow::OnUndoZoomTchartMainShow()
{
	// TODO: 在此处添加消息处理程序代码
	m_iZoomCount = 0;
	CINScanLiteDlg * pDlg = (CINScanLiteDlg *)AfxGetApp()->GetMainWnd();
	pDlg->OnAutoZoom();
}

void CDlgShow::showSingleLine( int index )
{
	int lineCount = m_Tchart.GetSeriesCount();
	if(index < 0)
		return;
	while(lineCount > 0)
	{
		if(lineCount - 1 == index)
			m_Tchart.Series(lineCount - 1).SetActive(TRUE);
		else
			m_Tchart.Series(lineCount - 1).SetActive(FALSE);
		lineCount--;
	}
}

void CDlgShow::OnZoomTchartMainShow()
{

}

// CDlgShow 消息处理程序
LRESULT CDlgShow::OnShowData(WPARAM wParam, LPARAM lParam)
{
	DataShow *ds;
	ds = (DataShow*)lParam;
	
	m_Tchart.Series(ds->seriesNum).Clear();

	
	double minVal = ds->y[0];
	double maxVal = ds->y[0];
	int minIdx = 0;
	int maxIdx = 0;

	for (int i = 0; i < ds->pointNum; i++)
	{
		// 更新 min、max
		if (ds->y[i] < minVal) {
			minVal = ds->y[i];
			minIdx = i;  // 更新最小值下标
		}		if (ds->y[i] > maxVal) { maxVal = ds->y[i];  maxIdx = i; }// 更新最小值下标}

		// 然后绘制数据点
		m_Tchart.Series(ds->seriesNum).AddXY(ds->x[i], ds->y[i], NULL, ds->color);
	}



	OnSetAutoRepaint(TRUE);
	m_Tchart.Series(ds->seriesNum).SetShowInLegend(TRUE);

	SetEvent(m_hEvent);

	return 0;

}
template<class T1, class T2, class T3>
T1 * CamPixProcessing(T1 *intensityData, T2 MaxSize, T2 iCCDNUM, T3 strCCD)
{
	CString strvalue;
	int num = 0;
	T1 first = 0;
	int m_Num = 0;
	BOOL nSt = TRUE;
	CStringArray sGataWave;
	if (iCCDNUM>0)
	{
		sGataWave.RemoveAll();
		Split(strCCD, ";", sGataWave);
		for (int i = 0; i<sGataWave.GetSize(); i++)
		{
			if (i >= (sGataWave.GetSize() - 1))
			{
				nSt = FALSE;
			}
			while (nSt && (atoi(sGataWave.GetAt(i)) + 1) == atoi(sGataWave.GetAt(i + 1)))
			{
				num++;
				i++;
				if (i == (sGataWave.GetSize() - 1))
				{
					nSt = FALSE;
				}
			}

			if (num == 0)
			{
				if (atoi(sGataWave.GetAt(i))<MaxSize - 1)
				{
					if (atoi(sGataWave.GetAt(i)) == 0)
					{
						first = intensityData[1];
					}
					else
					{
						first = intensityData[atoi(sGataWave.GetAt(i)) - 1];
					}
					intensityData[atoi(sGataWave.GetAt(i))] = (first + intensityData[atoi(sGataWave.GetAt(i)) + 1]) / 2;
				}
				else
				{
					intensityData[atoi(sGataWave.GetAt(i))] = (intensityData[atoi(sGataWave.GetAt(i)) - 2] + intensityData[atoi(sGataWave.GetAt(i)) - 1]) / 2;
				}
			}
			else
			{
				m_Num = num;
				while (m_Num != -1)
				{
					if (atoi(sGataWave.GetAt(i))<MaxSize - 1)
					{
						if ((atoi(sGataWave.GetAt(i)) - num) == 0)
						{
							first = 0.0;
						}
						else
						{
							first = intensityData[atoi(sGataWave.GetAt(i)) - num - 1];
						}
						intensityData[atoi(sGataWave.GetAt(i)) - m_Num] = first + (num + 1 - m_Num)*(intensityData[atoi(sGataWave.GetAt(i)) + 1] - first) / (num + 2);
					}
					else
					{
						intensityData[atoi(sGataWave.GetAt(i)) - m_Num] = (intensityData[atoi(sGataWave.GetAt(i)) - m_Num - 2] + intensityData[atoi(sGataWave.GetAt(i)) - m_Num - 1]) / 2;

					}
					m_Num--;
				}
			}
			num = 0;
		}
	}
	return intensityData;
}
void CDlgShow::show_dfield_get_data(char id, unsigned short* buff, int buff_size,int cutf_size)
{
	unsigned short* ubuff = new unsigned short[3000];
	memset(ubuff, 0, 3000 * sizeof(unsigned short));
	
	if (dfield_get_data(id, ubuff, buff_size)==true)
	{
		//dfield_get_data(id, ubuff, buff_size);
		int iCbpNum;
		CString strCbp, strResult;
		GetPrivateProfileString("Dfield", "ccdnum", "0", strResult.GetBuffer(1024), 1024, sInfo.strPath);
		iCbpNum = atoi(strResult);
		GetPrivateProfileString("Dfield", "ccd", "0", strCbp.GetBuffer(1024), 1024, sInfo.strPath);
		ubuff = CamPixProcessing<unsigned short, int, CString>(ubuff, buff_size, iCbpNum, strCbp);

		GetPrivateProfileString("Dfield", "Mirr", "0", strCbp.GetBuffer(1024), 1024, sInfo.strPath);
		if (atoi(strCbp) == 1)
		{
			reverse(ubuff, (ubuff + buff_size));
		}
		memcpy(buff, ubuff + cutf_size, (buff_size - cutf_size) * sizeof(unsigned short));
	}
	else
	{
		AfxMessageBox("错误");
	}

	delete [] ubuff;
}