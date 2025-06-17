#pragma once
#include "CameraDll.h"
class  CMityCamera :public CCameraDll
{
public: 
	static CMityCamera * GetInstance(){
		if(spMityCamera==NULL){
			spMityCamera=new CMityCamera();
		}
		return spMityCamera;
	};  
private:
	CMityCamera(){}; 
	static CMityCamera * spMityCamera;
	class CGarbo 
	{
	public:
		~CGarbo()
		{ 
			if (CMityCamera::spMityCamera)
				delete CMityCamera::spMityCamera;
		}
	};
	static CGarbo Garbo; 

public:
	//CMityCamera(void);
	~CMityCamera(void);

	BOOL nInitState;
	int Connect();
	int DisConnect();
	BOOL GetInitState();
	CString GetDevName();
	void GetDevSize(int *xpixSize,int *ypixSize);
	void GetPixSize(float *fPix);
	void GetExpTime(float *fTime);
	bool GetTemper(int *iTemp);
	void GetBinRange(int *BinMin,int *BinMax);
	void GetGain(int *Index,float *fGain,int *nCurrentSet);

	void SetOffset(int nTarget);
	void SetGain(int Index);
	void SetCooler(int iMode);
	void SetBinRange(int BinMin,int BinMax);
	void SetExpTime(float fTime);
	void SetTemper(int iTemp);
	bool SetInitState(BOOL Init);

	int ShowCameraSetDlg();
	void DataAcquisition(double *pd,int nPixSize);
	void ReadAllIni();
	void WriteAllIni();
	///状态控制 如需中断至FALSE
	void ImageDataAcquisition(unsigned short*plBuffer,BYTE* pBuffer,int XSize,int YSize);
	void SetImageMode(float ftime);//设置图像模式并设置积分时间
	void SetImageAccState(BOOL bState);

	void TerminateData();
private:
	int CameraHandle;//mity相机句柄
	
	BOOL gState;
};
extern CMityCamera *pMityCamera;
