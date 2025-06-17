#include "StdAfx.h"
#include "MityCamera.h"
#include "DataProcess.h"
#include "clcamiface.h"
#include "DlgMitySet.h"
CMityCamera *CMityCamera::spMityCamera=NULL;
CMityCamera *pMityCamera=NULL;
#define  MitySize 2048
//int g_openHPUSBCamera;//新增

CMityCamera::~CMityCamera(void)
{
}
bool CMityCamera::SetInitState(BOOL Init)
{
	nInitState=Init;
	gState=FALSE;
	return DATA_SUCCESS;
}
BOOL CMityCamera::GetInitState()
{
	return nInitState;
}

// lcz new 05/18
	DWORD WINAPI f1(LPVOID lpParameter)
{
	g_openHPUSBCamera = CLOpenHSUSBCamera(0);
	return 0;
}

int CMityCamera::Connect()
{

	unsigned char BinCodes[MitySize];
	unsigned long AreaRDInterval;
	unsigned long BinnedRDInterval;
	unsigned long AreaRDInterval2;
	unsigned long AreaRDInterval3;
	TCLCameraHardware Hardware;
	bool bMityState=false;
	int stSize = sizeof(TCLCameraHardware);

	//CameraHandle = CLOpenHSUSBCamera(0);
	//lcz new 05/18  如果CLOpenHSUSBCamera(0)：打开打开使用高速USB串行端口接口一个相机所用时间超过2.5s，就认为连接失败
	HANDLE hThread1;
	hThread1 = CreateThread(NULL, 0, f1, 0, 0, NULL);
	int signal = WaitForSingleObject(hThread1, 2500);//
	if (signal != WAIT_OBJECT_0)
	{
		//cout << "time out" << endl;
		TerminateProcess(hThread1, -1);
		return DATA_ERROR;
	}
	TerminateProcess(hThread1, -1);
	CloseHandle(hThread1);

	CameraHandle = g_openHPUSBCamera;
	//======================

	if (CameraHandle>0)
	{
		bMityState=true;
		CLSetBinParam(CameraHandle, &BinCodes[0], 0);
		CLGetImageIntervals(CameraHandle, &AreaRDInterval, &BinnedRDInterval, &AreaRDInterval, &AreaRDInterval);
	}
	else
	{
		Sleep(100);
		CLCloseCamera(CameraHandle);
		Sleep(100);
		CameraHandle = CLOpenHSUSBCamera(0);
		if (CameraHandle>0)
		{
			CLSetBinParam(CameraHandle, &BinCodes[0], 0);
			bMityState=true;
			CLGetImageIntervals(CameraHandle, &AreaRDInterval, &BinnedRDInterval, &AreaRDInterval, &AreaRDInterval);
		}
		else
		{
			return DATA_ERROR;
		}
	}
	if (bMityState==true)
	{
		CString strvalue;
		CLGetCameraHardwareInfo(CameraHandle, &Hardware, &stSize);
		stuMityCCD.nGXPix=Hardware.ActiveCols;
		stuMityCCD.nGYPix=Hardware.ImageRows;
		stuMityCCD.fXPixSize=Hardware.PixelWidthNm/1000;
		CString str;
		str.Format(_T("M%d"),Hardware.ActiveCols);
		stuMityCCD.sDevName=str;

		for (int idx = 0; idx < stuMityCCD.nGYPix; idx++)
			BinCodes[idx] = BINCODE_BIN;
		BinCodes[stuMityCCD.nGYPix-1] = BINCODE_SEND;

		SetOffset(stuMityCCD.iOffset);
		CLSetBinParam(CameraHandle, &BinCodes[0],stuMityCCD.nGYPix);
		stuMityCCD.bConnectS=TRUE;
		ReadAllIni();

		// 设置默认增益0.7
		// 0 x2.1
		// 1 x0.7
		SetGain(1);
		
		return DATA_SUCCESS;
	}
	return DATA_ERROR;
}
int CMityCamera::DisConnect()
{
	CLCloseCamera(CameraHandle);
	stuMityCCD.bConnectS=FALSE;
	return DATA_SUCCESS;
}
CString CMityCamera::GetDevName()
{
	return stuMityCCD.sDevName;
}
void CMityCamera::GetDevSize(int *xpixSize,int *ypixSize)
{
	*xpixSize=stuMityCCD.nGXPix;
	*ypixSize=stuMityCCD.nGYPix;
}
void CMityCamera::GetPixSize(float *fPix)
{
	*fPix=stuMityCCD.fXPixSize;
}
void CMityCamera::GetExpTime(float *fTime)
{
	*fTime=stuMityCCD.fExpTime;
}
bool CMityCamera::GetTemper(int *iTemp)
{
	short temperature=0;
	CLGetCCDTemp(CameraHandle,&temperature);
	*iTemp = temperature/10;

	return true;
}
void CMityCamera::GetBinRange(int *BinMin,int *BinMax)
{
	*BinMin=stuMityCCD.BinMin;
	*BinMax=stuMityCCD.BinMax;
}

void CMityCamera::SetBinRange(int BinMin,int BinMax)
{
	int position[2];
	position[0]=BinMin;
	position[1]=BinMax;

	stuMityCCD.BinMin=BinMin;
	stuMityCCD.BinMax=BinMax;
}
void CMityCamera::SetExpTime(float fTime)
{
	CLSetExposure(CameraHandle,fTime*1000);//ms设置需乘1000
	stuMityCCD.fExpTime=fTime;
}
void CMityCamera::SetTemper(int iTemp)
{
	stuMityCCD.nTemp=iTemp;
}
void CMityCamera::SetCooler(int iMode)
{
	if (iMode==0)
	   {CLSetCooling(CameraHandle,0);}
	if (iMode==1)
	   {CLSetCooling(CameraHandle,1);
	    SetTemper(stuMityCCD.nTemp);}
}
int CMityCamera::ShowCameraSetDlg()
{  
	if ((GetInitState()==FALSE)||(stuMityCCD.bConnectS==FALSE))
		return DATA_UNCONNECT;

	// lcz new 22/10/24
	g_bOpenDlg = true;// 打开设置框，就为真

	ZOLIX_DLL_MFC_Res::CDlgMitySet dlg;
	dlg.DoModal();
	return DATA_SUCCESS;
}
void CMityCamera::SetOffset(int nTarget)
{
	CLSetCCDOffset(CameraHandle,nTarget,false);
}
void CMityCamera::GetGain(int *Index,float *fGain,int *nCurrentSet)
{
	CLGetGainConfig(CameraHandle,Index,fGain,nCurrentSet);
}

void CMityCamera::SetGain(int Index)
{
	CLSetGain(CameraHandle,Index);
}
void CMityCamera::ReadAllIni()
{
	/*CIniFile file;
	file.FindFile(strIniName);
	g_strPath=file.FindPath();
	file.Open();
	CString str;
	str=file.Find(sMity,_T("nReadMode"));
	stuMityCCD.nReadMode=atoi(str);
	str=file.Find(sMity,_T("nTemp"));
	stuMityCCD.nTemp=atoi(str);
	str=file.Find(sMity,_T("iGain"));
	stuMityCCD.iGain=atoi(str);
	str=file.Find(sMity,_T("iOffset"));
	stuMityCCD.iOffset=atoi(str);
	str=file.Find(sMity,_T("isMirror"));
	stuMityCCD.isMirror=atoi(str);
	str=file.Find(sMity,_T("CCDNUM"));
	stuMityCCD.iCbpNum=atoi(str);
	str=file.Find(sMity,_T("CCD"));
	stuMityCCD.strCbp=str;

	str=file.Find(sMity,_T("BinMin"));
	stuMityCCD.BinMin=atoi(str);
	str=file.Find(sMity,_T("BinMax"));
	stuMityCCD.BinMax=atoi(str);*/
	ManageZolixIni->ReadIni();
	if (stuMityCCD.BinMin<0)
		stuMityCCD.BinMin=0;
	if (stuMityCCD.BinMax>=stuMityCCD.nGYPix)
		stuMityCCD.BinMax=stuMityCCD.nGYPix-1; 
	//file.Close();
}
void CMityCamera::WriteAllIni()
{
	//CIniFile file;
	//file.FindFile(strIniName);
	//file.Open();
	//CString str;
	//str.Format(_T("%d"),stuMityCCD.nReadMode);
	//file.WriteValue(sMity,_T("nReadMode"),str);
	//str.Format(_T("%d"),stuMityCCD.iGain);
	//file.WriteValue(sMity,_T("iGain"),str);
	//str.Format(_T("%d"),stuMityCCD.iOffset);
	//file.WriteValue(sMity,_T("iOffset"),str);
	//str.Format(_T("%d"),stuMityCCD.nTemp);
	//file.WriteValue(sMity,_T("nTemp"),str);
	//str.Format(_T("%d"),stuMityCCD.BinMin);
	//file.WriteValue(sMity,_T("BinMin"),str);
	//str.Format(_T("%d"),stuMityCCD.BinMax);
	//file.WriteValue(sMity,_T("BinMax"),str);
	//str.Format(_T("%d"),stuMityCCD.isMirror);
	//file.WriteValue(sMity,_T("isMirror"),str);
	//str.Format(_T("%d"),stuMityCCD.iCbpNum);
	//file.WriteValue(sMity,_T("CCDNUM"),str);
	//file.WriteValue(sMity,_T("CCD"),stuMityCCD.strCbp);
	//file.Close();
	ManageZolixIni->WriteIni();
}
void CMityCamera::DataAcquisition(double *pd,int nPixSize)
{
	try
	{
		int position[2];
		switch (stuMityCCD.nReadMode)
		{
		case  0:
			{ 
				position[0]=0;
				position[1]=stuMityCCD.nGYPix-1;
				break;}
		case 1:
			{  	 
				position[0]=stuMityCCD.BinMin;
				position[1]=stuMityCCD.BinMax;

				break;}
		default:
			break;		  
		}
		int m_acqStatus=0;

		unsigned char BinCodes[MitySize];
		for (int idx = 0; idx < stuMityCCD.nGYPix; idx++)
			BinCodes[idx] = BINCODE_BIN;

		if (position[0]>0)
		{
			BinCodes[position[0]-1] = BINCODE_DISCARD;
		}
		BinCodes[position[1]] = BINCODE_SEND;
		CLSetBinParam(CameraHandle, &BinCodes[0],0);

		unsigned short MaxSize=nPixSize;
		long *lData=new long[nPixSize];
		double *pData=new double[nPixSize];
		do 
		{
			CLSetExposure(CameraHandle,stuMityCCD.fExpTime*1000);
			CLSetImageInterval(CameraHandle, stuMityCCD.fExpTime*1000+15);
			CLClearCCD(CameraHandle, 1, position[1]-position[0]);
			CLWaitCompletion(CameraHandle,stuMityCCD.fExpTime*1000*10);
			CLReadCCDBinned(CameraHandle, &lData[0], &MaxSize, 1, 0);		//one image, no clears
			CLWaitCompletion(CameraHandle,stuMityCCD.fExpTime*1000*10);
			m_acqStatus++;
		} while (m_acqStatus==0);

		Float2Long<long,double,int>(lData,pData,nPixSize);
		pData=CamPixProcessing<double,int,CString>(pData,nPixSize,stuMityCCD.iCbpNum,stuMityCCD.strCbp);
		CIsMirror<int,double>(stuMityCCD.isMirror,nPixSize,pData,pd);
		delete []pData;
		pData=NULL;
		delete []lData;
		lData=NULL;
	}
	catch (CMemoryException* e)
	{
		e->Delete();
	}
	catch (CException* e)
	{
		e->Delete();
	}
	

}
void CMityCamera::ImageDataAcquisition(unsigned short*plBuffer, BYTE* pBuffer,int XSize,int YSize)
{
	try
	{
		int nNum=XSize*YSize;
		int iMax=0,iMin=65535;
		int RowStart = 0;
		int ColStart = 0;
		int m_acqStatus=0;
		do 
		{
			CLSetCCDImageMirroring(CameraHandle,stuMityCCD.HMirror,1);
			CLGetCCDArea(CameraHandle,&RowStart,&ColStart, &YSize, &XSize);
			CLClearCCD(CameraHandle, 1, 0);
			CLWaitCompletion(CameraHandle, 20000);
			CLReadCCDArea(CameraHandle, plBuffer, 1, 0);
			CLWaitCompletion(CameraHandle, 20000);

			for (int i=0;i<nNum;i++)
			{
				if (plBuffer[i]>iMax)
				{
					iMax=plBuffer[i];
				}
				if (plBuffer[i]<iMin)
				{
					iMin=plBuffer[i];
				}
			}
			int iRange;
			iRange =iMax-iMin;
			if (iRange==0)
			{
				iRange=1;
			}
			for (int i=0;i<nNum;i++)
			{
				pBuffer[3*i]=(plBuffer[i]-iMin)*255/iRange;
				pBuffer[3*i+1]=(plBuffer[i]-iMin)*255/iRange;
				pBuffer[3*i+2]=(plBuffer[i]-iMin)*255/iRange;
			}	
			m_acqStatus++;
		} while(m_acqStatus==0&&gState==TRUE);
	}
	catch (CMemoryException* e)
	{
		e->Delete();
	}
	catch (CException* e)
	{
		e->Delete();
	}
	
}
void CMityCamera::SetImageMode(float ftime)
{
	CLSetExposure(CameraHandle,ftime*1000);
}
void CMityCamera::SetImageAccState(BOOL bState)
{
	gState=bState;
}
void CMityCamera::TerminateData()
{
	CLAbortRead(CameraHandle);
	CLClearCCD(CameraHandle,1,0);
	CLWaitCompletion(CameraHandle,0);
}