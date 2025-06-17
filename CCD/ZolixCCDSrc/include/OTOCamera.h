#pragma once
#include "CameraDll.h"
class  COTOCamera :public CCameraDll
{
public: 
	static COTOCamera * GetInstance()
	{
		if(spOTOCamera==NULL){
			spOTOCamera=new COTOCamera();
			spOTOCamera->fData=NULL;
			spOTOCamera->pData=NULL;
		}
		return spOTOCamera;
	}; 
private:
	COTOCamera(){}; 
	static COTOCamera * spOTOCamera;
	class CGarbo 
	{
	public:
		~CGarbo()
		{ 
			if (COTOCamera::spOTOCamera)
				delete COTOCamera::spOTOCamera;
		}
	};
	static CGarbo Garbo; 
public:
	//COTOCamera(void);
	~COTOCamera(void);
	
	BOOL nInitState;
	int Connect();
	int DisConnect();
	BOOL GetInitState();
	bool SetInitState(BOOL Init);
	CString GetDevName();
	void GetDevSize(int *xpixSize,int *ypixSize);
	void GetPixSize(float *fPix);
	void GetExpTime(float *fTime);
	bool GetTemper(int *iTemp);
	void SetExpTime(float fTime);
	void SetCooler(int iMode);
	int ShowCameraSetDlg();
	void DataAcquisition(double *pd,int nPixSize);
	void ReadAllIni();
	void WriteAllIni();
	void XAxisData(double *pd,double *coeff,int Num,int nPixSize);

	void SetPWM(int duty);
	void SetExternalPort(int port);
	void * handle;
	
	float *fData;
	double *pData;
private:
	/*OTO型号名称
	  HB-2034,2054
	  SE-2020,2030,2040,2050,2052,2060,2070,2080,2090
	  EE-2063,2061,2053
	  OTO适用光谱波长
	  FUV:180-850
	  FUV2:180-500
	  FUVN:180-1100
	  DUVN:200-1100
	  VNIR:350-1020
	  NIR1:790-1010
	  NIR4:790-1090
	*/
	int GetModalName();
};

extern COTOCamera *pOTOCamera;