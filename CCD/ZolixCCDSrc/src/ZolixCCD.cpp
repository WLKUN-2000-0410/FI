#include "stdafx.h"
#include "ZolixCCD.h"
#include "CameraDll.h"
#include "MityCamera.h"
#include "OTOCamera.h"
#include "AndorCamera.h"
#include "GreateyesCamera.h"
#include "ToucanCamera.h"
#include "A1Camera.h"

NS_MFC_Res_BEGIN
int Num=0;

CZolixCCD::CZolixCCD(void)
{

	pAndorCamera=CAndorCamera::GetInstance();
	pMityCamera=CMityCamera::GetInstance();
	pOTOCamera=COTOCamera::GetInstance();
	pGreateyesCamera = CGreateyesCamera::GetInstance();
	pToucanCamera = CToucanCamera::GetInstance();
	ManageZolixIni=new CManageZolixIni();

	//2025.2.19new
	pA1Camera = CA1Camera::GetInstance();
}


CZolixCCD::~CZolixCCD(void)
{
	if (pAndorCamera!=NULL)
	{
		delete pAndorCamera;
		pAndorCamera=NULL;
		delete pMityCamera;
		pMityCamera=NULL;
		delete pOTOCamera;
		pOTOCamera=NULL;
		delete pGreateyesCamera;
		pGreateyesCamera=NULL;
		delete pToucanCamera;
		pToucanCamera=NULL;
	}
	delete ManageZolixIni;
}
int CZolixCCD::InitType(int Num)
{
	switch(Num)
	{
	case 10:
		pCameraDll=pAndorCamera;
		break;   
	case 15:
		pCameraDll=pMityCamera;
		break;
	case 20:
		pCameraDll=pOTOCamera;
		break;
	case 25:
		pCameraDll=pGreateyesCamera;
		break;
	case 30:
		pCameraDll=pToucanCamera;
		break;
	case 50:                   //2025.2.19从50开始加
		pCameraDll = pA1Camera;
		break;
	default:
		pCameraDll=NULL;
		return DATA_ERROR;
		break;
	}

	pCameraDll->SetInitState(TRUE);
	return DATA_SUCCESS;
}
int CZolixCCD::Connect()
{
	if (pCameraDll==NULL)
	    return DATA_UNINIT;
	 
	if (pCameraDll->Connect()==0)
	{
		return DATA_SUCCESS;
	}
	else
	{
		pCameraDll=NULL;
		return DATA_ERROR;
	}
	
}
int CZolixCCD::DisConnect()
{
	if (pCameraDll==NULL)
		return DATA_UNINIT;

	return pCameraDll->DisConnect();
}
CString CZolixCCD::GetDevName()
{
	if (pCameraDll==NULL)
        return NULL;
	if (pCameraDll->GetDevName().IsEmpty())
	{
		return _T("Camera");
	}
	return pCameraDll->GetDevName();
}
void CZolixCCD::GetDevSize(int *xpixSize,int *ypixSize)
{
	if (pCameraDll==NULL)
	    {return;}

	pCameraDll->GetDevSize(xpixSize,ypixSize);
}
void CZolixCCD::GetPixSize(float *fPix)
{
	if (pCameraDll==NULL)
		return ;

	pCameraDll->GetPixSize(fPix);
}
void CZolixCCD::GetExpTime(float *fTime)
{
	if (pCameraDll==NULL)
		return ;

	pCameraDll->GetExpTime(fTime);
}
bool CZolixCCD::GetTemper(int *iTemp)
{
	if (pCameraDll==NULL)
		return false;

	 return pCameraDll->GetTemper(iTemp);
	 
}
void CZolixCCD::GetBinRange(int *BinMin,int *BinMax)
{
	if (pCameraDll==NULL)
		return ;

	pCameraDll->GetBinRange(BinMin,BinMax);
}

void CZolixCCD::SetBinRange(int BinMin,int BinMax)
{
	if (pCameraDll==NULL)
		return ;

	pCameraDll->SetBinRange(BinMin,BinMax);
}
void CZolixCCD::SetExpTime(float fTime)
{
	if (pCameraDll==NULL)
		return ;

	pCameraDll->SetExpTime(fTime);
}
void CZolixCCD::SetTemper(int iTemp)
{
	if (pCameraDll==NULL)
		return ;

	pCameraDll->SetTemper(iTemp);
}
void CZolixCCD::SetCooler(int iMode)
{
	if (pCameraDll==NULL)
		return ;

	pCameraDll->SetCooler(iMode);
}
int CZolixCCD::ShowCameraSetDlg()
{
	if (pCameraDll==NULL)
		return DATA_UNINIT;

	return pCameraDll->ShowCameraSetDlg();
}
void CZolixCCD::SetLanguage(int nL)
{
	if (pCameraDll==NULL)
		return ;

	pCameraDll->SetLanguage(nL);
}
void CZolixCCD::DataAcquisition(double *pd,int nPixSize)
{
	if (pCameraDll==NULL)
		return ;

	pCameraDll->DataAcquisition(pd,nPixSize);
}
void CZolixCCD::XAxisData(double *pd,double *coeff,int Num,int nPixSize)
{
	if (pCameraDll==NULL)
		return ;

	pCameraDll->XAxisData(pd,coeff,Num,nPixSize);
}
void CZolixCCD::CameraType(CString *strName,int *iType,int *Num)
{
	strName[0]=_T("A"); iType[0]=10;
	strName[1]=_T("M");  iType[1]=15;
	strName[2]=_T("T");   iType[2]=20;
	//strName[3]=_T("G");   iType[3]=25;
	*Num=3;
}

int CZolixCCD::GetRangeDataX(int nNMCM,int iSzie,double dLaser,double dRangeLeft, double dRangeRight,double *inputD,double *outputD,int *CutF)
{
	double Laser;
	int rSzie=0;
	for (int i=0;i<iSzie;i++)
	{
		double dXdata;
		if (nNMCM==0)
			dXdata=inputD[i];
		else
			dXdata=10000000*((1/dLaser)-(1/inputD[i]));
		if (dXdata>=dRangeLeft&&dXdata<=dRangeRight)
		{
			if (rSzie==0)
			{
				*CutF=i;
			}
			outputD[rSzie]=dXdata;
			rSzie++;
		}
		
	}

	return rSzie;
}
void CZolixCCD::GetRangeDataY(int CutF,int rSize,double *inputD,double *outputD)
{
	for (int i=0;i<rSize;i++)
	{
		outputD[i]=inputD[CutF+i];
	}
}
void CZolixCCD::TerminateData()
{
	if (pCameraDll==NULL)
		return ;

	pCameraDll->TerminateData();
}

void CZolixCCD::SetPWM(int duty)//设置激光功率
{
	if (pCameraDll==NULL)
		return ;

	pCameraDll->SetPWM(duty);
}
void CZolixCCD::SetExternalPort(int port,HANDLE handle)//设置外触发接口
{
	if (pCameraDll==NULL)
		return ;

	pCameraDll->SetExternalPort(port,handle);
}

//  lcz new 22/10/24 返回是否打开高级设置
bool CZolixCCD::getStartOrNot()
{
	return g_bOpenDlg;
}


NS_MFC_Res_END