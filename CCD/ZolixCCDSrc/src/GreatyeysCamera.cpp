#include "StdAfx.h"
#include "GreateyesCamera.h"
#include "greateyes.h"
#include "greateyesBeta.h"
#include "DlgGreateyesSet.h"
#include "DataProcess.h"
CGreateyesCamera *CGreateyesCamera::spGreateyesCamera=NULL;
CGreateyesCamera *pGreateyesCamera=NULL;

const int cameraAddr = 0;
int lastStatus = 0;
//#pragma comment(lib,"atmcd32m.lib")
CGreateyesCamera::~CGreateyesCamera(void)
{
	DisConnect();
}
bool CGreateyesCamera::SetInitState(BOOL Init)
{
	nInitState=Init;
	gState=FALSE;
	return DATA_SUCCESS;
}
BOOL CGreateyesCamera::GetInitState()
{
	return nInitState;
}
int CGreateyesCamera::Connect()
{
	int modelID = 0;
	char model[64];
	char *modelPtr = model;
	char* ip = "192.168.1.234";
	int connectionType = connectionType_USB;
	SetupCameraInterface(connectionType, ip, lastStatus, 0);
	int i=0;
	while(i<50)
	{
		if(GetNumberOfConnectedCams()>0)
		//if(ConnectToSingleCameraServer(cameraAddr))
		{
			if(ConnectCamera(modelID, modelPtr, lastStatus, 0))
			{
				InitCamera(lastStatus,cameraAddr);
				SetSpeed(stuGreateyesCCD.iSInedx);
				SetBitDepth(4,lastStatus,cameraAddr);
				stuGreateyesCCD.bConnectS=TRUE;
				break;
			}
		}
		else
		{
			Sleep(1000);
			i++;
			stuGreateyesCCD.bConnectS=FALSE;
		}
		
	}
	
	if(stuGreateyesCCD.bConnectS)
	{
		int bytesPerPixel=0;
		GetImageSize(stuGreateyesCCD.nGXPix, stuGreateyesCCD.nGYPix, bytesPerPixel, cameraAddr);	
		stuGreateyesCCD.fXPixSize=GetSizeOfPixel(cameraAddr);
		ReadAllIni();
		CString str;
		str.Format(_T("G%d"),stuGreateyesCCD.nGXPix);
		stuGreateyesCCD.sDevName=str;
	}
	

	return !stuGreateyesCCD.bConnectS;
}

int CGreateyesCamera::DisConnect()
{
  
	stuGreateyesCCD.bConnectS=FALSE;
	DisconnectCamera(lastStatus,cameraAddr);
	return DATA_SUCCESS;
}
CString CGreateyesCamera::GetDevName()
{
	return stuGreateyesCCD.sDevName;
}
void CGreateyesCamera::GetDevSize(int *xpixSize,int *ypixSize)
{
	*xpixSize=stuGreateyesCCD.nGXPix;
	*ypixSize=stuGreateyesCCD.nGYPix;
}
void CGreateyesCamera::GetPixSize(float *fPix)
{
	*fPix=stuGreateyesCCD.fXPixSize;
}
void CGreateyesCamera::GetExpTime(float *fTime)
{
	*fTime=stuGreateyesCCD.fExpTime;
}
bool CGreateyesCamera::GetTemper(int *iTemp)
{
	TemperatureControl_GetTemperature(0, *iTemp, lastStatus, cameraAddr);

	return true;
}
void CGreateyesCamera::GetBinRange(int *BinMin,int *BinMax)
{
	*BinMin=stuGreateyesCCD.BinMin;
	*BinMax=stuGreateyesCCD.BinMax;
}

void CGreateyesCamera::SetBinRange(int BinMin,int BinMax)
{
	int position[2];
	position[0]=BinMin;
	position[1]=BinMax;
	//SetRandomTracks(1,position);
	stuGreateyesCCD.BinMin=BinMin;
	stuGreateyesCCD.BinMax=BinMax;
}
void CGreateyesCamera::SetExpTime(float fTime)
{
	SetExposure(fTime*1000, lastStatus, cameraAddr);//µ¥Î»ms
	stuGreateyesCCD.fExpTime=fTime;
}
void CGreateyesCamera::SetTemper(int iTemp)
{
	TemperatureControl_SetTemperature(iTemp, lastStatus, cameraAddr);
	stuGreateyesCCD.nTemp=iTemp;
}
void CGreateyesCamera::SetCooler(int iMode)
{
	if (iMode==0)
		TemperatureControl_SwitchOff(lastStatus, cameraAddr);
	if (iMode==1)
	{
		int minTemperature = 0;
		int maxTemperature = 0;
		int coolingHardwareOption=stuGreateyesCCD.nID;
		TemperatureControl_Init(coolingHardwareOption, minTemperature, maxTemperature, lastStatus, cameraAddr);
		SetTemper(stuGreateyesCCD.nTemp);	    
	}
}
int CGreateyesCamera::ShowCameraSetDlg()
{  
	if ((GetInitState()==FALSE)||(stuGreateyesCCD.bConnectS==FALSE))
		return DATA_UNCONNECT;
		
	ZOLIX_DLL_MFC_Res::CDlgGreateyesSet dlg;
	dlg.DoModal();
	return DATA_SUCCESS;
}
int CGreateyesCamera::GetGainNum()
{
	int Num=0;
	//GetNumberPreAmpGains(&Num);
	return Num;
}
void CGreateyesCamera::GetGain(int Index,float *fGain)
{
	//GetPreAmpGain(Index,fGain);
}

void CGreateyesCamera::SetGain(int Index)
{
	//SetPreAmpGain(Index);
}
int CGreateyesCamera::GetSpeedNum()
{
	int Num=6;

	//readoutSpeed_1_MHz, readoutSpeed_3_MHz, 
	//readoutSpeed_500_kHz, readoutSpeed_250_kHz,
	//readoutSpeed_100_kHz, readoutSpeed_50_kHz
	return Num;
}
void CGreateyesCamera::GetSpeed( int &Index)
{
	 //GetHSSpeed(stuAndorCCD.nAD, typ, Index, fSpeed);
	Index = stuGreateyesCCD.iSInedx;
}
void CGreateyesCamera::SetSpeed(int Index)
{
	switch(Index)
	{
	case 0:
		SetReadOutSpeed(readoutSpeed_3_MHz, lastStatus, cameraAddr);
		break;
	case 1:
		SetReadOutSpeed(readoutSpeed_1_MHz, lastStatus, cameraAddr);
		break;
	case 2:
		SetReadOutSpeed(readoutSpeed_500_kHz, lastStatus, cameraAddr);
		break;
	case 3:
		SetReadOutSpeed(readoutSpeed_250_kHz, lastStatus, cameraAddr);
		break;
	case 4:
		SetReadOutSpeed(readoutSpeed_100_kHz, lastStatus, cameraAddr);
		break;
	case 5:
		SetReadOutSpeed(readoutSpeed_50_kHz, lastStatus, cameraAddr);
		break;
	default:
		break;
	}
	
}

void CGreateyesCamera::ReadAllIni()
{	
	ManageZolixIni->ReadIni();

	if (stuGreateyesCCD.BinMin<0)
	    stuGreateyesCCD.BinMin=0;
	if (stuGreateyesCCD.BinMax>=stuGreateyesCCD.nGYPix)
        stuGreateyesCCD.BinMax=stuGreateyesCCD.nGYPix-1; 
	//file.Close();
}
void CGreateyesCamera::WriteAllIni()
{
	ManageZolixIni->WriteIni();
}
void CGreateyesCamera::DataAcquisition(double *pd,int nPixSize)
{

	
		switch (stuGreateyesCCD.nReadMode)
		{
		case  0:
			{   
				SetBinningMode(1, stuGreateyesCCD.nGYPix-1,  lastStatus, cameraAddr);
			    break;
			}
		case 1:
			{ 	SetBinningMode(1, 1,  lastStatus, cameraAddr);

			break;}
		default:
			break;		  
		}
	


		const int siz=nPixSize;
		int m_acqStatus;
		BYTE *lData=new BYTE[nPixSize*stuGreateyesCCD.nGYPix*4];
		int *pData=new int[nPixSize];
ReStart:	

		int textTime=0;
		int m_errorAndorCCD=StartMeasurement(false, false, false, false, 300, lastStatus, cameraAddr);
		if(m_errorAndorCCD!=true)//Acquisition started
		{ 
			StopMeasurement(cameraAddr);
		}
		else
		{
			
			while ( DllIsBusy( cameraAddr ) )
			{
				if ( (textTime*0.1 ) > stuAndorCCD.fExpTime )
				{
					if (	StopMeasurement(cameraAddr)==true)
					{
						goto ReStart;
					}
				}
			}
			if ( GetMeasurementData_DynBitDepth(lData, lastStatus, cameraAddr) == false)
				return ;

		}
			
		if (stuGreateyesCCD.nReadMode==0)
		{
			memcpy(pData,lData,nPixSize*sizeof(int));
		}
		else
		{
			int *TempData=new int[nPixSize*(stuGreateyesCCD.BinMax-stuGreateyesCCD.BinMin)];
			memcpy(TempData,lData+stuGreateyesCCD.BinMin*nPixSize*4,nPixSize*(stuGreateyesCCD.BinMax-stuGreateyesCCD.BinMin)*sizeof(int));
			memset(pData,0,nPixSize*sizeof(int));
			for (int i=0;i<(stuGreateyesCCD.BinMax-stuGreateyesCCD.BinMin);i++)
			{
				for (int j=0;j<nPixSize;j++)
				{
					pData[j]+=TempData[j+i*nPixSize];

					if (i==(stuGreateyesCCD.BinMax-stuGreateyesCCD.BinMin-1))
					{
						pData[j]=pData[j]/(stuGreateyesCCD.BinMax-stuGreateyesCCD.BinMin);
					}
				}
			}
			delete [] TempData;
		}
		


		pData=CamPixProcessing<int,int,CString>(pData,nPixSize,stuGreateyesCCD.iCbpNum,stuGreateyesCCD.strCbp);
		double dTemp[3000];
		for (int ii=0;ii<nPixSize;ii++)
		{
			dTemp[ii]=pData[ii]*1.0;
		}
		CIsMirror<int,double>(stuGreateyesCCD.isMirror,nPixSize,dTemp,pd);
		delete []pData;
		pData=NULL;
		delete []lData;
		lData=NULL;
}


void CGreateyesCamera::ImageDataAcquisition(int*plBuffer, BYTE* pBuffer,int XSize,int YSize)
{

	SetBinningMode(1, 1,  lastStatus, cameraAddr);
	
	int m_acqStatus;
	int textTime=0;
	int *pData=new int[XSize*YSize];
	BYTE *bData=new BYTE[XSize*YSize*4];
	int m_errorAndorCCD=StartMeasurement(false, false, false, false, 300, lastStatus, cameraAddr);
	if(m_errorAndorCCD!=true)//Acquisition started
	{ 
		StopMeasurement(cameraAddr);
	}
	else
	{

		while ( DllIsBusy( cameraAddr ) )
		{
			if ( (textTime*0.1 ) > stuGreateyesCCD.fExpTime )
			{
				if (	StopMeasurement(cameraAddr)==true)
				{
					return;
				}
			}
		}
		if ( GetMeasurementData_DynBitDepth(bData, lastStatus, cameraAddr) == false)
			return ;

	}
	memcpy(pData,bData,XSize*YSize*sizeof(int));




	int iMin=65535,iMax=0;

	for (int i=0;i<XSize*YSize;i++)
	{
		if (pData[i]>iMax)
		{
			iMax=pData[i];
		}
		if (pData[i]<iMin)
		{
			iMin=pData[i];
		}
	}

	int www;
	www =iMax-iMin;
	if (www==0)
	{
		www=1;
	}

	for (int i=0;i<YSize;i++)
	{
		for (int j=0;j<XSize;j++)
		{
			if (stuGreateyesCCD.HMirror==false)
			{
				pBuffer[3*j+3*i*XSize]=(pData[j+i*XSize]-iMin)*255/www;
				pBuffer[3*j+3*i*XSize+1]=(pData[j+i*XSize]-iMin)*255/www;
				pBuffer[3*j+3*i*XSize+2]=(pData[j+i*XSize]-iMin)*255/www;
			}
			else
			{
				pBuffer[3*(XSize-j-1)+3*i*XSize]=(pData[j+i*XSize]-iMin)*255/www;
				pBuffer[3*(XSize-j-1)+3*i*XSize+1]=(pData[j+i*XSize]-iMin)*255/www;
				pBuffer[3*(XSize-j-1)+3*i*XSize+2]=(pData[j+i*XSize]-iMin)*255/www;
			}
			
			
		}

	}

	/*for (int i=0;i<YSize*XSize;i++)
	{
			pBuffer[3*i]=(pData[i]-iMin)*255/www;
			pBuffer[3*i+1]=(pData[i]-iMin)*255/www;
			pBuffer[3*i+2]=(pData[i]-iMin)*255/www;
	}*/

	delete  [] bData;
	delete  [] pData;
}
void CGreateyesCamera::SetImageMode(float ftime)
{
	//int  ret;
	//ret=SetReadMode(4);
	//ret=SetFullImage(1,1); 
	//ret=SetAcquisitionMode(1);
	SetExpTime(ftime);

}
void CGreateyesCamera::SetImageAccState(BOOL bState)
{
	gState=bState;
}
void CGreateyesCamera::TerminateData()
{
	StopMeasurement(cameraAddr);
}