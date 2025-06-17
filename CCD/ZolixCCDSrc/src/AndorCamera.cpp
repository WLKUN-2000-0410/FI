#include "StdAfx.h"
#include "AndorCamera.h"
#include "ATMCD32D.H"
#include "DlgAndorSet.h"
#include "DataProcess.h"
CAndorCamera *CAndorCamera::spAndorCamera=NULL;
CAndorCamera *pAndorCamera=NULL;
//CAndorCamera::CAndorCamera(void):CCameraDll()
//{
//	nInitState=FALSE;
//}
//#pragma comment(lib,"ATMCD32M.lib")
#pragma comment(lib,"atmcd32m.lib")
CAndorCamera::~CAndorCamera(void)
{
}
bool CAndorCamera::SetInitState(BOOL Init)
{
	nInitState=Init;
	gState=FALSE;
	return DATA_SUCCESS;
}
BOOL CAndorCamera::GetInitState()
{
	return nInitState;
}
int CAndorCamera::Connect()
{
	    if (!GetInitState())
		return DATA_UNINIT;
	
	    AndorCapabilities caps;
		char 	cBuffer[256];
		GetCurrentDirectoryA(256,cBuffer);					// Look in current working directory

		if(Initialize(cBuffer)!=DRV_SUCCESS){
			return DATA_ERROR;}									// CCD Initialize Error
		
		char	cModel[32];
		if(GetHeadModel(cModel)!=DRV_SUCCESS){
			return DATA_ERROR;}								// CCD Get Head Model information Error
		
		//stuAndorCCD.sDevName=cModel;			
		
		if(GetDetector(&stuAndorCCD.nGXPix,&stuAndorCCD.nGYPix)!=DRV_SUCCESS){
			return DATA_ERROR;}									// CCD Get Detector information Error
		
		if(SetTriggerMode(0)!=DRV_SUCCESS){
			return DATA_ERROR;}									// Set Trigger Mode Error
		
		//--------------------------------------------------------------------------------------
		int VSnumber=0;										// 建议索引
		float speed	=0;										// 建议最快速度
		//建议最快的垂直位移速度,每像素偏移速度(微秒)
		GetFastestRecommendedVSSpeed(&VSnumber, &speed);	// Set Vertical speed to recommended
		SetVSSpeed(VSnumber);
		/*if(SetVSSpeed(VSnumber)!=DRV_SUCCESS){
			return DATA_ERROR;}		*/							// CCD Set Vertical Speed Error
		//--------------------------------------------------------------------------------------

		float STemp		=0;
		float HSnumber	=0;
		float ADnumber	=0;
		int	iSpeed, iAD, nAD, index;

		if (GetNumberADChannels(&nAD) != DRV_SUCCESS){
			return DATA_ERROR;									// CCD Get number AD Channel Error
		}
		else {
			for (iAD = 0; iAD < nAD; iAD++) {
				GetNumberHSSpeeds(iAD, 0, &index);
				for (iSpeed = 0; iSpeed < index; iSpeed++) {
					GetHSSpeed(iAD, 0, iSpeed, &speed);
					//		Speed.Add(speed);
					if(speed > STemp)
					{
						STemp = speed;
						HSnumber = (float)iSpeed;
						ADnumber = (float)iAD;
					}
				}
			}
		}
	    stuAndorCCD.nAD=ADnumber;
		if(SetADChannel((int )ADnumber)!=DRV_SUCCESS){
			return DATA_ERROR;	}								// CCD Set AD Channel Error
	
		GetCapabilities(&caps);
		if (caps.ulSetFunctions & AC_SETFUNCTION_BASELINECLAMP) {
			SetBaselineClamp(1);
			//if(SetBaselineClamp(1)!=DRV_SUCCESS){
			//	return DATA_ERROR;								// CCD Set Baseline Clamp Error
			//}
		}

	float fXPixS,fYPixS;
	if(GetPixelSize(&stuAndorCCD.fXPixSize,&fYPixS)!=DRV_SUCCESS){
		return DATA_ERROR;
	}
	SetShutter(1,1,0,0);// Shutter permanently open
	//pAndorCamera->SetShutterAuto(stuAndorCCD.ShutterMode);
	stuAndorCCD.bConnectS=TRUE;
	ReadAllIni();
	CString str;
	str.Format(_T("A%d"),stuAndorCCD.nGXPix);
	stuAndorCCD.sDevName=str;
	SetGain(stuAndorCCD.iGain);
	SetExternalPort(0,NULL);
	stuAndorCCD.nTrigger=0;
	return DATA_SUCCESS;
}

int CAndorCamera::DisConnect()
{
	ShutDown();	
	stuAndorCCD.bConnectS=FALSE;
	return DATA_SUCCESS;
}
CString CAndorCamera::GetDevName()
{
	return stuAndorCCD.sDevName;
}
void CAndorCamera::GetDevSize(int *xpixSize,int *ypixSize)
{
	*xpixSize=stuAndorCCD.nGXPix;
	*ypixSize=stuAndorCCD.nGYPix;
}
void CAndorCamera::GetPixSize(float *fPix)
{
	*fPix=stuAndorCCD.fXPixSize;
}
void CAndorCamera::GetExpTime(float *fTime)
{
	*fTime=stuAndorCCD.fExpTime;
}
bool CAndorCamera::GetTemper(int *iTemp)
{
	GetTemperature(iTemp);

	return true;
}
void CAndorCamera::GetBinRange(int *BinMin,int *BinMax)
{
	*BinMin=stuAndorCCD.BinMin;
	*BinMax=stuAndorCCD.BinMax;
}

void CAndorCamera::SetBinRange(int BinMin,int BinMax)
{
	int position[2];
	position[0]=BinMin;
	position[1]=BinMax;
	SetRandomTracks(1,position);
	stuAndorCCD.BinMin=BinMin;
	stuAndorCCD.BinMax=BinMax;
}
void CAndorCamera::SetExpTime(float fTime)
{
	SetExposureTime(fTime);
	stuAndorCCD.fExpTime=fTime;
}
void CAndorCamera::SetTemper(int iTemp)
{
	SetTemperature(iTemp);
	stuAndorCCD.nTemp=iTemp;
}
void CAndorCamera::SetCooler(int iMode)
{
	if (iMode==0)
		CoolerOFF();
	if (iMode==1)
	    {SetTemper(stuAndorCCD.nTemp);	
	     CoolerON();}
}
int CAndorCamera::ShowCameraSetDlg()
{  
	if ((GetInitState()==FALSE)||(stuAndorCCD.bConnectS==FALSE))
		return DATA_UNCONNECT;

	// lcz new 22/10/24
	g_bOpenDlg = true;// 打开设置框，就为真
		
	ZOLIX_DLL_MFC_Res::CDlgAndorSet dlg;
	dlg.DoModal();
	return DATA_SUCCESS;
}
int CAndorCamera::GetGainNum()
{
	int Num=0;
	GetNumberPreAmpGains(&Num);
	return Num;
}
void CAndorCamera::GetGain(int Index,float *fGain)
{
	GetPreAmpGain(Index,fGain);
}

void CAndorCamera::SetGain(int Index)
{
	SetPreAmpGain(Index);
}
int CAndorCamera::GetSpeedNum()
{
	int Num=0;
	GetNumberHorizontalSpeeds(&Num);
	return Num;
}
void CAndorCamera::GetSpeed( int typ,int Index,float *fSpeed)
{
	GetHSSpeed(stuAndorCCD.nAD, typ, Index, fSpeed);
}
void CAndorCamera::SetSpeed(int typ,int Index)
{
	SetHSSpeed(typ,Index);
}
int CAndorCamera::GetPixSpeedNum()
{
	int Num=0;
	GetNumberVSSpeeds(&Num);
	return Num;
}
void CAndorCamera::GetPixSpeed(int Index,float *fSpeed)
{
	GetVSSpeed(Index,fSpeed);
}
void CAndorCamera::SetPixSpeed(int Index)
{
	SetVSSpeed(Index);
}
void CAndorCamera::ReadAllIni()
{

	CString str;

	ManageZolixIni->ReadIni();

	if (stuAndorCCD.BinMin<0)
	    stuAndorCCD.BinMin=0;
	if (stuAndorCCD.BinMax>=stuAndorCCD.nGYPix)
        stuAndorCCD.BinMax=stuAndorCCD.nGYPix-1; 
	//file.Close();
}
void CAndorCamera::WriteAllIni()
{
	
	ManageZolixIni->WriteIni();
}
void CAndorCamera::DataAcquisition(double *pd,int nPixSize)
{
	try
	{
		int position[2]={0};
		int m_errorAndorCCD1;
		SetImageFlip(0,0);
		switch (stuAndorCCD.nReadMode)
		{
		case  0:
			{   SetReadMode(0);
				break;
			}
		case 1:
			{ m_errorAndorCCD1= SetReadMode(2); 	 
			position[0]=stuAndorCCD.BinMin+1;
			position[1]=stuAndorCCD.BinMax+1;
			m_errorAndorCCD1=SetRandomTracks(1,position);
			m_errorAndorCCD1=SetCustomTrackHBin(1);
			break;}
		default:
			break;		  
		}
		const int siz=nPixSize;
		int m_acqStatus;
		long *lData=new long[nPixSize];
		double *pData=new double[nPixSize];
ReStart:	

		if (stuAndorCCD.nTrigger==1)
		{
			long vf, vl;
			GetStatus(&m_acqStatus);
			if (m_acqStatus != DRV_IDLE)
			{   
				GetNumberNewImages(&vf, &vl);				//Signal main application
				long va, vb;
				int waveCount =nPixSize;
				int errorWalue = GetImages(vl, vl, lData, waveCount, &va, &vb);

				GetAcquiredData(lData, nPixSize);
			}
		}

		else{

			int textTime=0;
			int m_errorAndorCCD=StartAcquisition();
			if(m_errorAndorCCD!=DRV_SUCCESS)//Acquisition started
			{ 
				AbortAcquisition();
			}
			else
			{  
					do
					{
						//if ( (textTime*0.1 ) > stuAndorCCD.fExpTime )
						/*{
							if (AbortAcquisition()==DRV_SUCCESS)
							{
								goto ReStart;
							}
						}*/
						//Sleep(500);
						if (GetStatus(&m_acqStatus)==DRV_SUCCESS)
						{
							if(m_acqStatus==DRV_IDLE)//Status returned//空闲等待命令
							{

								m_errorAndorCCD=GetAcquiredData(lData,nPixSize);
								if(m_errorAndorCCD!=DRV_SUCCESS)//Data copied.
								{
									return ;
								}
							}
						}
						//textTime++;
					}while(m_acqStatus!=DRV_IDLE);			
			}
		}
		Float2Long<long,double,int>(lData,pData,nPixSize);
		pData=CamPixProcessing<double,int,CString>(pData,nPixSize,stuAndorCCD.iCbpNum,stuAndorCCD.strCbp);
		CIsMirror<int,double>(stuAndorCCD.isMirror,nPixSize,pData,pd);
		
		RemoveGhosting1D_CrossFade(pd, nPixSize, stuAndorCCD.ghostImageStrAbsolute);
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
int WriteToLog_Andor(char* str)
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
void CAndorCamera::ImageDataAcquisition(long*plBuffer, BYTE* pBuffer,int XSize,int YSize)
{
	
	try
	{
		int nNum=XSize*YSize;
		//StartAcquisition();	
		int m_acqStatus=0;
		int ui_error=StartAcquisition();
		SetImageFlip(stuAndorCCD.HMirror, 1);
		//long *plBuffer;
		int iMin=65535,iMax=0;
		do
		{				
			ui_error=GetStatus(&m_acqStatus);
			if (!gState)
			{
				AbortAcquisition();
				//去鬼像

				RemoveGhosting(plBuffer, XSize, YSize,stuAndorCCD.ghostImageStrAbsolute);
						
			}
			if (ui_error==DRV_SUCCESS&&m_acqStatus==DRV_IDLE)
			{
				GetMostRecentImage(plBuffer,nNum);

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

				int www;
				www =iMax-iMin;
				if (www==0)
				{
					www=1;
				}
				for (int i=0;i<nNum;i++)
				{
					pBuffer[3*i]=(plBuffer[i]-iMin)*255/www;
					pBuffer[3*i+1]=(plBuffer[i]-iMin)*255/www;
					pBuffer[3*i+2]=(plBuffer[i]-iMin)*255/www;
				}
			}

		}while(m_acqStatus!=DRV_IDLE&&gState==TRUE);
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
void CAndorCamera::SetImageMode(float ftime)
{
	int  ret;
	SetTriggerMode(0);
	ret=SetReadMode(4);
	ret=SetFullImage(1,1); 
	ret=SetAcquisitionMode(1);
	ret=SetExposureTime(ftime);
}
void CAndorCamera::SetImageAccState(BOOL bState)
{
	gState=bState;
}
void CAndorCamera::TerminateData()
{
	AbortAcquisition();
}
void CAndorCamera::SetShutterAuto(int nMode,int nClose,int nOpen)
{

	if (nMode==0)
	{
		SetShutter(1,1,0,0);//1-open
	}
	if (nMode==1)
	{
		SetShutter(1,0,nClose,nOpen);//0-Auto
	}
	if (nMode==2)
	{
		SetShutter(1,2,0,0);
	}
}
void CAndorCamera::SetExternalPort(int port,HANDLE handle)//设置外触发接口
{
	if (port == 0)
	{
		stuAndorCCD.nTrigger=0;
		SetAcquisitionMode(1);
		SetTriggerMode(0);
		SetDriverEvent(NULL);
	}
	else
	{ 
		stuAndorCCD.nTrigger=1;	
		SetAcquisitionMode(5);
		SetReadMode(0);
		SetTriggerMode(1);
		SetDriverEvent(handle);
		StartAcquisition();
	}
}


// 去鬼像函数（支持多个区域修复，CString）
void CAndorCamera::RemoveGhosting(long* plBuffer, int XSize, int YSize, const CString& ghostRegionStr) {
	CString msg;
	msg.Format(_T("图像修正：ghostRegionStr = %s"), ghostRegionStr);
	WriteToLog_Andor(CT2A(msg));
	// 解析鬼像区域字符串（如 "500,700;1000,1200"）
	CString strCopy = ghostRegionStr;
	int startPos = 0;

	// 遍历每个鬼像区域
	while (startPos < strCopy.GetLength()) {
		CString region = strCopy.Tokenize(_T(";"), startPos);
		if (region.IsEmpty()) break;

		// 解析单个区域（如 "500,700"）
		int commaPos = region.Find(_T(","));
		if (commaPos != -1) {
			int x_start = _ttoi(region.Left(commaPos));
			int x_end = _ttoi(region.Mid(commaPos + 1));
			int d = x_end - x_start;

			// 从右往左修复，避免区域覆盖
			for (int row = 0; row < YSize; ++row) {
				for (int j = d - 1; j >= 0; --j) {
					int left_idx = row * XSize + (x_start - d + j);
					int right_idx = row * XSize + (x_end + j);
					int target_idx = row * XSize + (x_start + j);

					// 边界保护，确保修复区域在图像范围内
					if (left_idx >= 0 && right_idx < XSize * YSize) {
						plBuffer[target_idx] = static_cast<long>(
							0.5 * plBuffer[left_idx] + 0.5 * plBuffer[right_idx]);
					}
				}
			}
		}
	}
}
