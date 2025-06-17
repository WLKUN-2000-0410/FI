#include "StdAfx.h"
#include "ToucanCamera.h"
#include "FTD2XX.H"
#include "toucam_zl.h"
#include "toucampro.h"
#include "DlgToucanSet.h"
#include "DataProcess.h"
CToucanCamera *CToucanCamera::spToucanCamera=NULL;
CToucanCamera *pToucanCamera=NULL;


CToucanCamera::~CToucanCamera(void)
{
}
bool CToucanCamera::SetInitState(BOOL Init)
{
	bool bRet2;
	bRet2=m_ccda.SetDllPath("ftd2xx.dll");
	bRet2=m_ccdb.SetDllPath("ftd2xx.dll");
	nInitState=Init;
	gState=FALSE;
	if(bRet2==false)
		return DATA_ERROR;
	else
		return DATA_SUCCESS;
	 
}
BOOL CToucanCamera::GetInitState()
{
	return nInitState;
}
int CToucanCamera::Connect()
{
	
	char *BufPtrs1[64]; // pointer to array of 64 pointers
	DWORD d1;
	for( d1=0; d1<10; d1++)
		BufPtrs1[d1] = new char[64];
	BufPtrs1[d1] = NULL;

	bool bRet1,bRet2;
	bRet1=m_ccda.SetDllPath("ftd2xx.dll");
	DWORD nNum1=0;
	bRet1=m_ccda.ListDevices(BufPtrs1,nNum1);
	
	CString str1="FT6YDMEWA",str2="FT6YDMEWB";
	for(int i=0;i<nNum1;i++)
	{
		CString str;
		str.Format("%s", BufPtrs1[i]);
		
		if(str.Right(1)=="B")
		{
			str2=str;
			str1=str2.Left(str2.GetLength()-1)+"A";
			stuToucanCCD.sDevName=str1;
		}

	}
	

	for(int d=0; d<nNum1; d++)
	{
		delete BufPtrs1[d];  
	}
	
	//CString str1="FT6YDMEWA",str2="FT6YDMEWB";
	//bool bRet1,bRet2;
	bRet1=m_ccda.Open((PVOID)(LPCTSTR)str1);
	bRet2=m_ccdb.Open((PVOID)(LPCTSTR)str2);
	stuToucanCCD.bConnectS=bRet1;
	m_ccda.GetInit(0);
	m_ccda.GetMfData();
	m_ccda.GetInit(1);
	//stuToucanCCD.nGXPix=m_ccda.GetXSize();
	//stuToucanCCD.nGYPix=m_ccda.GetYSize();
	
	
	stuToucanCCD.nGXPix=2048;
	stuToucanCCD.nGYPix=264;
	ReadAllIni();
	if (bRet1==0)
	{
		return DATA_ERROR;
	}
	else
	{
		m_ccdb.clear();
		SetBinning(stuToucanCCD.nReadMode);
		m_ccda.SetOneFrameNumber();
		m_ccda.SetReadFraneNumber();
		SetTemper(stuToucanCCD.nTemp);
		SetGain(stuToucanCCD.iGain);
		return DATA_SUCCESS;
	}
	
}

int CToucanCamera::DisConnect()
{
	m_ccda.Close();
	m_ccdb.Close();
	stuToucanCCD.bConnectS=FALSE;
	return DATA_SUCCESS;
}
CString CToucanCamera::GetDevName()
{
	//stuToucanCCD.sDevName="FT6YDMEWA";
	return stuToucanCCD.sDevName;
}
void CToucanCamera::GetDevSize(int *xpixSize,int *ypixSize)
{
	//stuToucanCCD.nGYPix=m_ccda.GetYSize();
	*xpixSize=stuToucanCCD.nGXPix;
	*ypixSize=stuToucanCCD.nGYPix;
}
void CToucanCamera::GetPixSize(float *fPix)
{
	stuToucanCCD.fXPixSize=15;
	*fPix=stuToucanCCD.fXPixSize;
}
void CToucanCamera::GetExpTime(float *fTime)
{
	stuToucanCCD.fExpTime=m_ccda.GetExpTime();
	*fTime=stuToucanCCD.fExpTime;
}
bool CToucanCamera::GetTemper(int *iTemp)
{	
	*iTemp=(int)m_ccda.GetTemp();
	return true;
}
void CToucanCamera::GetBinRange(int *BinMin,int *BinMax)
{
	*BinMin=stuToucanCCD.BinMin;
	*BinMax=stuToucanCCD.BinMax;
}

void CToucanCamera::SetBinRange(int BinMin,int BinMax)
{
	int position[2];
	position[0]=BinMin;
	position[1]=BinMax;
	
	stuToucanCCD.BinMin=BinMin;
	stuToucanCCD.BinMax=BinMax;
}
void CToucanCamera::SetExpTime(float fTime)
{
	m_ccda.SetExpTime(fTime);
	//m_ccda.SetReadoutTriggerMode(); 
	stuToucanCCD.fExpTime=fTime;
}
void CToucanCamera::SetTemper(int iTemp)
{
	m_ccda.SetTemp(iTemp);
	stuToucanCCD.nTemp=iTemp;
}
void CToucanCamera::SetCooler(int iMode)
{
	/*if (iMode==0)

	if (iMode==1)
	{SetTemper(stuToucanCCD.nTemp);	
	;
	}*/
}
int CToucanCamera::ShowCameraSetDlg()
{  
	if ((GetInitState()==FALSE)||(stuToucanCCD.bConnectS==FALSE))
		return DATA_UNCONNECT;

	// lcz new 22/10/24
	g_bOpenDlg = true;// 打开设置框，就为真

	ZOLIX_DLL_MFC_Res::CDlgToucanSet dlg;
	dlg.DoModal();
	return DATA_SUCCESS;
}
int CToucanCamera::GetGainNum()
{
	int Num=2;

	return Num;
}
void CToucanCamera::GetGain(int Index,float *fGain)
{

}

void CToucanCamera::SetGain(int Index)
{
	m_ccda.SetGain(Index);//0-high 1-low
}
int CToucanCamera::GetSpeedNum()
{
	int Num=0;

	return Num;
}
void CToucanCamera::GetSpeed( int typ,int Index,float *fSpeed)
{
	
}
void CToucanCamera::SetSpeed(int typ,int Index)
{

}
int CToucanCamera::GetPixSpeedNum()
{
	return 0;
}
void CToucanCamera::GetPixSpeed(int Index,float *fSpeed)
{
	
}
void CToucanCamera::SetPixSpeed(int Index)
{
	
}
void CToucanCamera::ReadAllIni()
{

	CString str;

	ManageZolixIni->ReadIni();

	if (stuToucanCCD.BinMin<0)
	    stuToucanCCD.BinMin=0;
	if (stuToucanCCD.BinMax>=stuToucanCCD.nGYPix)
        stuToucanCCD.BinMax=stuToucanCCD.nGYPix-1; 
	//file.Close();
}
void CToucanCamera::WriteAllIni()
{

	//file.Close();
	ManageZolixIni->WriteIni();
}
void CToucanCamera::DataAcquisition(double *pd,int nPixSize)
{
	try
	{
		unsigned short MaxSize=nPixSize;
		double *pData=new double[nPixSize];
		memset(pData,0,nPixSize);

		if(stuToucanCCD.BinMin<0)
		{
			stuToucanCCD.BinMin=0;
		}
		if(stuToucanCCD.BinMax<=0)
		{
			stuToucanCCD.BinMax=1;
		}
		//m_ccda.SetBin(1,0,263);
		//m_ccda.SetBin(2,0,0);
		//int YSize=m_ccda.GetYSize();
		//m_ccda.SetBin(stuToucanCCD.nReadMode,stuToucanCCD.BinMin,stuToucanCCD.BinMax);

		//int YSize=m_ccda.GetYSize();	

	/*	m_ccda.SetOneFrameNumber();
		m_ccda.SetReadFraneNumber();*/

		m_ccda.SetReadoutTriggerMode();  // Set usb ctrl bit to start acquisition
		if (stuToucanCCD.nReadMode==1&&(stuToucanCCD.BinMax-stuToucanCCD.BinMin+1)==128)
		{
			m_ccdb.getSingleFrame(pData,nPixSize,2,stuToucanCCD.bkoffset);
		}
		else
		{
			m_ccdb.getSingleFrame(pData,nPixSize,1,stuToucanCCD.bkoffset);
		}
		

		

		pData=CamPixProcessing<double,int,CString>(pData,nPixSize,stuToucanCCD.iCbpNum,stuToucanCCD.strCbp);
		CIsMirror<int,double>(stuToucanCCD.isMirror,nPixSize,pData,pd);
		delete []pData;
		pData=NULL;
	

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
void CToucanCamera::ImageDataAcquisition(BYTE* pBuffer,int XSize,int YSize)
{
	try
	{
		BYTE *Buffer=new BYTE[XSize*YSize*3];
		/*m_ccda.SetBin(1,0,263);
		m_ccda.SetBin(2,0,0);*/
		m_ccda.SetOneFrameNumber();
		m_ccda.SetReadFraneNumber();
		m_ccda.SetReadoutTriggerMode();  // Set usb ctrl bit to start acquisition
		m_ccdb.getSingleFrameImage(Buffer,XSize,YSize);

		if (stuToucanCCD.HMirror==true)
		{
			for (int i=0;i<YSize;i++)
			{
				for (int j=0;j<XSize;j++)
				{
					pBuffer[i*XSize*3+j*3]=Buffer[i*XSize*3+(XSize-j-1)*3];
					pBuffer[i*XSize*3+j*3+1]=Buffer[i*XSize*3+(XSize-j-1)*3+1];
					pBuffer[i*XSize*3+j*3+2]=Buffer[i*XSize*3+(XSize-j-1)*3+2];
				}
			}
		}
		else
		{
			memcpy(pBuffer,Buffer,XSize*YSize*3);
		}
		delete [] Buffer;
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
void CToucanCamera::SetImageMode(float ftime)
{
	//m_ccda.SetBin(1,0,263);
	//m_ccda.SetBin(2,0,0);
	//m_ccda.SetBin(0,0,0);
	//Sleep(2000);
	SetExpTime(ftime);
	m_ccda.SetBin(1,0,263);
	m_ccda.SetBin(2,0,0);
	
	m_ccda.SetOneFrameNumber();
	m_ccda.SetReadFraneNumber();
	int YSize=m_ccda.GetYSize();
}
void CToucanCamera::SetImageAccState(BOOL bState)
{
	gState=bState;
	m_ccdb.SetRun(bState);
}
void CToucanCamera::TerminateData()
{
	m_ccdb.AbortCCD();
	m_ccdb.clear();
}
void CToucanCamera::SetShutterAuto(int nMode,int nClose,int nOpen)
{

	
}
void CToucanCamera::SetBinning(int nMode)//0-FVB 1-mulit
{
	//m_ccda.SetBin(1,0,263);
	if(stuToucanCCD.BinMin<0)
	{
		stuToucanCCD.BinMin=0;
	}
	if(stuToucanCCD.BinMax<=0)
	{
		stuToucanCCD.BinMax=1;
	}
	m_ccda.SetBin(1,0,263);
	m_ccda.SetBin(2,0,0);
	int YSize=m_ccda.GetYSize();
	m_ccda.SetBin(stuToucanCCD.nReadMode,stuToucanCCD.BinMin+stuToucanCCD.offset,stuToucanCCD.BinMax+stuToucanCCD.offset);
    //m_ccda.GetYSize();

}

