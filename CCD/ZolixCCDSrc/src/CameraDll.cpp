#include "StdAfx.h"
#include "CameraDll.h"
#include "math.h"
CCameraDll *pCameraDll=NULL;
AndorCCD stuAndorCCD;
MityCCD  stuMityCCD;
OTOCCD   stuOTOCCD;
GreateyesCCD   stuGreateyesCCD;
ToucanCCD stuToucanCCD;
A1CCD stuA1CCD;
CCameraDll::CCameraDll(void)
{
	
}


CCameraDll::~CCameraDll(void)
{

}

bool CCameraDll::SetInitState(BOOL Init)
{
	return DATA_SUCCESS;
}
BOOL CCameraDll::GetInitState()
{
	return DATA_ERROR;
}
int CCameraDll::Connect()
{
	return DATA_NULL;
}
int CCameraDll::DisConnect()
{
	return DATA_NULL;
}
 CString CCameraDll::GetDevName()
 {
	 return _T("Null");
 }
bool ListDevices(CString *str,int &num)
{
	return DATA_ERROR;
 }
 void CCameraDll::GetDevSize(int *xpixSize,int *ypixSize)
 {

 }
 void CCameraDll::GetPixSize(float *fPix)
 {

 }
 void CCameraDll::GetExpTime(float *fTime)
 {

 }
 bool CCameraDll::GetTemper(int *iTemp)
 {
	 return false;
 }
 void CCameraDll::GetBinRange(int *BinMin,int *BinMax)
 {

 }

 void CCameraDll::SetBinRange(int BinMin,int BinMax)
 {

 }
 void CCameraDll::SetExpTime(float fTime)
 {

 }
 void CCameraDll::SetTemper(int iTemp)
 {

 }
 void CCameraDll::SetCooler(int iMode)
 {

 }
 int CCameraDll::ShowCameraSetDlg()
 {
	 return DATA_NULL;
 }
 void CCameraDll:: SetLanguage(int nL)
 {
	 nLanguage=nL;
 }
 void CCameraDll::DataAcquisition(double *pd,int nPixSize)
 {
	 
 }
 void CCameraDll::XAxisData(double *pd,double *coeff,int Num,int nPixSize)
 {
      for (int i=0;i<nPixSize;i++)
      {
		  for (int j=0;j<Num;j++)
		  {
			  pd[i]+=coeff[j]*pow(i*1.0,j);
		  }
      }
 }
 void CCameraDll::TerminateData()
 {

 }

 void CCameraDll::SetPWM(int duty)//设置激光功率
 {
	
 }
 void CCameraDll::SetExternalPort(int port,HANDLE handle)//设置外触发接口
 {

 }