#include "StdAfx.h"
#include "OTOCamera.h"
#include "UserApplication.h"
#include "DlgOTOSet.h"
#include "DataProcess.h"

COTOCamera *COTOCamera::spOTOCamera=NULL;
COTOCamera *pOTOCamera=NULL;


COTOCamera::~COTOCamera(void)
{
	if (fData!=NULL)
	{
		delete []fData;
	}
	if(pData!=NULL)
	{
		delete []pData;
	}
}
bool COTOCamera::SetInitState(BOOL Init)
{
	nInitState=Init;
	return true;
}
BOOL COTOCamera::GetInitState()
{
	return nInitState;
}
int COTOCamera::Connect()
{
	if (nInitState==FALSE)
		return DATA_UNINIT;
	
	fData=NULL;
	pData=NULL;
	ReadAllIni();
	unsigned int status, device_num, i;
	//get compatible VID and PID
	unsigned int buffersize = 2;
	unsigned int VIDPID[4];
	//VIDPID = new unsigned int [4];
	VIDPID[0] = 4292;
	VIDPID[1] = 60001;
	VIDPID[2] = 1592;
	VIDPID[3] = 2732;
	unsigned short tempvalue = 0;
	//get all device numbers
	for(unsigned int j=0; j<buffersize*2; j=j+2)
	{
		device_num = 0;
		status = UAI_SpectrometerGetDeviceAmount(VIDPID[j], VIDPID[j+1], &device_num);
		if( status==0 && device_num>0 ) // API_SUCCESS == status
		{
			for( i=0; i<device_num; i++)
			{
				status = UAI_SpectrometerOpen(i,&handle, VIDPID[j], VIDPID[j+1]);
				if( status == 0 )
				{
  				    if (stuOTOCCD.iRadioMode==0)
  				    {
						status = UAI_SpectromoduleGetFrameSizeRaw(handle,&tempvalue);
						tempvalue=2095;
  				    }
					else
					{
						status = UAI_SpectromoduleGetFrameSize(handle,&tempvalue);
					}
					if (status!=0||tempvalue==0)
					{	
						return DATA_ERROR;
					}

					break;
				}
			}
			unsigned char *sName=NULL;
			UAI_LibraryGetName(sName);
			CString str;
			str.Format(_T("T%d"),tempvalue);
			stuOTOCCD.sDevName=str;
			stuOTOCCD.nGXPix=tempvalue;
			stuOTOCCD.nGYPix=1;
			stuOTOCCD.fXPixSize=14;//根据厂家提供信息赋值，没有读取函数
			stuOTOCCD.bConnectS=TRUE;
			stuOTOCCD.fExpTime=1;
			fData=new float[tempvalue*2];
			pData=new double[tempvalue*2];
	        stuOTOCCD.HardWareMode=GetModalName();
			return DATA_SUCCESS;
		}
	}	

	return DATA_ERROR;
}
int COTOCamera::DisConnect()
{	
	if (stuOTOCCD.bConnectS==TRUE)
	{
		UAI_SpectrometerClose(handle);
		delete []fData;
		fData=NULL;
		delete []pData;	
		pData=NULL;
		stuOTOCCD.bConnectS=FALSE;
		stuOTOCCD.HardWareMode=0;
	}
	
	return DATA_SUCCESS;
}
CString COTOCamera::GetDevName()
{
	return stuOTOCCD.sDevName;
}
void COTOCamera::GetDevSize(int *xpixSize,int *ypixSize)
{
	*xpixSize=stuOTOCCD.nGXPix;
	*ypixSize=stuOTOCCD.nGYPix;
}
void COTOCamera::GetPixSize(float *fPix)
{
	*fPix=stuOTOCCD.fXPixSize;
}
void COTOCamera::GetExpTime(float *fTime)
{
	*fTime=stuOTOCCD.fExpTime;
}

void COTOCamera::SetExpTime(float fTime)
{
	stuOTOCCD.fExpTime=fTime;
}

int COTOCamera::ShowCameraSetDlg()
{  
	if ((GetInitState()==FALSE)||(stuOTOCCD.bConnectS==FALSE))
		return DATA_UNCONNECT;

	ZOLIX_DLL_MFC_Res::CDlgOTOSet dlg;
	dlg.DoModal();
	return DATA_SUCCESS;
}


void COTOCamera::ReadAllIni()
{
	/*CIniFile file;
	file.FindFile(strIniName);
	g_strPath=file.FindPath();
	file.Open();
	CString str;
	str=file.Find(sOTO,_T("isMirror"));
	stuOTOCCD.isMirror=atoi(str);

	str=file.Find(sOTO,_T("iCheckBK"));
	stuOTOCCD.iCheckBK=atoi(str);
	str=file.Find(sOTO,_T("iCheckLine"));
	stuOTOCCD.iCheckLine=atoi(str);
	str=file.Find(sOTO,_T("iCheckInitensity"));
	stuOTOCCD.iCheckInitensity=atoi(str);
	str=file.Find(sOTO,_T("iCheckAbInitensity"));
	stuOTOCCD.iCheckAbInitensity=atoi(str); 
	str=file.Find(sOTO,_T("iCheckCool"));
	stuOTOCCD.iCheckCool=atoi(str); 
	str=file.Find(sOTO,_T("iRadioX"));
	stuOTOCCD.iRadioX=atoi(str);
	str=file.Find(sOTO,_T("iRadioMode"));
	stuOTOCCD.iRadioMode=atoi(str);
	str=file.Find(sOTO,_T("CCDNUM"));
	stuOTOCCD.iCbpNum=atoi(str);
	str=file.Find(sOTO,_T("CCD"));
	stuOTOCCD.strCbp=str;
	file.Close();*/
	ManageZolixIni->ReadIni();
}
void COTOCamera::WriteAllIni()
{
	/*CIniFile file;
	file.FindFile(strIniName);
	file.Open();
	CString str;
	str.Format(_T("%d"),stuOTOCCD.isMirror);
	file.WriteValue(sOTO,_T("isMirror"),str);

	str.Format(_T("%d"),stuOTOCCD.iCheckBK);
	file.WriteValue(sOTO,_T("iCheckBK"),str);
	str.Format(_T("%d"),stuOTOCCD.iCheckLine);
	file.WriteValue(sOTO,_T("iCheckLine"),str);
	str.Format(_T("%d"),stuOTOCCD.iCheckInitensity);
	file.WriteValue(sOTO,_T("iCheckInitensity"),str);
	str.Format(_T("%d"),stuOTOCCD.iCheckAbInitensity);
	file.WriteValue(sOTO,_T("iCheckAbInitensity"),str);
	str.Format(_T("%d"),stuOTOCCD.iCheckCool);
	file.WriteValue(sOTO,_T("iCheckCool"),str);
	str.Format(_T("%d"),stuOTOCCD.iRadioX);
	file.WriteValue(sOTO,_T("iRadioX"),str);
	str.Format(_T("%d"),stuOTOCCD.iRadioMode);
	file.WriteValue(sOTO,_T("iRadioMode"),str);
	str.Format(_T("%d"),stuOTOCCD.iCbpNum);
	file.WriteValue(sOTO,_T("CCDNUM"),str);
	file.WriteValue(sOTO,_T("CCD"),stuOTOCCD.strCbp);
	file.Close();*/
	ManageZolixIni->WriteIni();
}

int COTOCamera::GetModalName()
{
	unsigned char chName[500];
	UAI_SpectrometerGetModelName(handle, chName);

	CString str;
	str=chName;
	if (stuOTOCCD.iRadioMode==1)
	{
		if (str.Left(2)==_T("EE"))
		{
			return 1;
		}
		
	}
	return 0;

}
void COTOCamera::DataAcquisition(double *pd,int nPixSize)
{
	int m_acqStatus=0;
	unsigned short MaxSize;
	/*float *fData=new float[nPixSize];
	double *pData=new double[nPixSize];*/
	memset(pd,0,sizeof(long)*nPixSize);
	memset(pData,0,sizeof(long)*nPixSize);

	if (stuOTOCCD.iRadioMode==0)
	{
		UAI_SpectromoduleGetFrameSizeRaw(handle,&MaxSize);
		MaxSize=2095;
	}
	else
	{
		UAI_SpectromoduleGetFrameSize(handle,&MaxSize);
	}

	try
	{
		if (MaxSize==nPixSize)
		{
			do 
			{
				UAI_SpectrometerSetIntegrationTime(handle,stuOTOCCD.fExpTime*1000000);
				if (stuOTOCCD.iRadioMode==0)
				{
					UAI_SpectrometerDataOneshotRaw(handle,stuOTOCCD.fExpTime*1000000,fData,1);
				}
				else
				{				
					UAI_SpectrometerDataOneshot(handle,stuOTOCCD.fExpTime*1000000,fData,1);
					if(stuOTOCCD.iCheckBK==1)
					{
						UAI_BackgroundRemove(handle,stuOTOCCD.fExpTime*1000000,fData);
					}
					if(stuOTOCCD.iCheckLine==1)
					{
						UAI_LinearityCorrection(handle,nPixSize, fData);
					}
					if (stuOTOCCD.iCheckInitensity==1)
					{
						UAI_ContrastIntensityCorrection(handle, fData);
					}
					if (stuOTOCCD.iCheckAbInitensity==1)
					{
						UAI_AbsoluteIntensityCorrection(handle, fData,stuOTOCCD.fExpTime*1000000);
					}
				}		
				m_acqStatus++;
			} while (m_acqStatus==0);

			Float2Long<float,double,int>(fData,pData,nPixSize);
			pData=CamPixProcessing<double,int,CString>(pData,nPixSize,stuOTOCCD.iCbpNum,stuOTOCCD.strCbp);
			CIsMirror<int,double>(stuOTOCCD.isMirror,nPixSize,pData,pd);	
		}
	}
	catch(CMemoryException* e)
	{
		e->Delete();
	}
	catch(CException* e)
	{
		e->Delete();
	}
	/*delete []fData;
	fData=NULL;
	delete []pData;	
	pData=NULL;*/
}
void COTOCamera::XAxisData(double *pd,double *coeff,int Num,int nPixSize)
{
	try
	{
		float *fXData=new float[nPixSize];
		if (stuOTOCCD.iRadioX==0||stuOTOCCD.iRadioMode==0)
		{
			for (int i=0;i<nPixSize;i++)
			{
				/*for (int j=0;j<Num;j++)
				{
				pd[i]+=coeff[j]*pow(i*1.0,j);
				}*/
				pd[i]=coeff[0]+coeff[1]*i+coeff[2]*i*i+coeff[3]*i*i*i+coeff[4]*i*i*i*i+coeff[5]*i*i*i*i*i;
			    CString str;
				str.Format("%f",pd[i]);
			}
		}
		if (stuOTOCCD.iRadioX==1&&stuOTOCCD.iRadioMode==1)
		{
			UAI_SpectrometerWavelengthAcquire(handle, fXData);
			Float2Long<float,double,int>(fXData,pd,nPixSize);
		}
		delete []fXData;
		fXData=NULL;
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
bool COTOCamera::GetTemper(int *iTemp)
{
	float temperature=0;

	if (stuOTOCCD.iCheckCool==1&&stuOTOCCD.HardWareMode==1)
	{
		//UAI_SpectrometerGetTECDAC(handle,&temperature);
		UAI_SpectrometerGetTECTemperature(handle,&temperature);
		*iTemp = (int)temperature;
		return true;
	}
	else
	{
		return false;

	}
	//UAI_SpectrometerGetTECTemperature(handle,&temperature);
	
}
void COTOCamera::SetCooler(int iMode)
{
	if (stuOTOCCD.iCheckCool==1)
	{	
		UAI_SpectrometerSetTECOnOff(handle,iMode);
		UAI_SpectrometerSetTECTargetTemperature(handle,0);//目标零度
	}
	
}

void COTOCamera::SetPWM(int duty)
{
	UAI_SpectrometerSetPWM(handle, 0,1000, duty);

}
void COTOCamera::SetExternalPort(int port)
{
	UAI_SpectrometerSetExternalPort(handle, port);
}
