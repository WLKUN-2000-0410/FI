#pragma once
#include "CameraDll.h"
class CGreateyesCamera:public CCameraDll
{
//使用单例模式 限制每次每种相机只能连一个,现一台谱仪只有一个相机很合适
//单例模式规划还不算太好，几个了重复代码考虑使用继承或其他会更好
//后续修改
public: 
	static CGreateyesCamera * GetInstance(){
		if(spGreateyesCamera==NULL){
			spGreateyesCamera=new CGreateyesCamera();
		}
		return spGreateyesCamera;
	};   
private:
	CGreateyesCamera(){}; 
	static CGreateyesCamera * spGreateyesCamera;
	//回收指针
	class CGarbo 
	{
	public:
		~CGarbo()
		{ 
			if (CGreateyesCamera::spGreateyesCamera)
				delete CGreateyesCamera::spGreateyesCamera;
		}
	};
	static CGarbo Garbo;
public:
	//CAndorCamera(void);
	~CGreateyesCamera(void);
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
	int GetGainNum();
	void GetGain(int Index,float *fGain);
	int GetSpeedNum();
	void GetSpeed( int &Index);

	void SetGain(int Index);
	void SetSpeed(int Index);

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
	void ImageDataAcquisition(int*plBuffer,BYTE* pBuffer,int XSize,int YSize);
	void SetImageMode(float ftime);//设置图像模式并设置积分时间
	void SetImageAccState(BOOL bState);

	void TerminateData();
private:
	BOOL gState;
};

extern CGreateyesCamera *pGreateyesCamera;