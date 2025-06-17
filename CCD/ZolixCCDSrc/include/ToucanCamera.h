#pragma once
#include "CameraDll.h"
#include "toucam_zl.h"
class CToucanCamera:public CCameraDll
{
//使用单例模式 限制每次每种相机只能连一个,现一台谱仪只有一个相机很合适
//单例模式规划还不算太好，几个了重复代码考虑使用继承或其他会更好
//后续修改
public: 
	static CToucanCamera * GetInstance(){
		if(spToucanCamera==NULL){
			spToucanCamera=new CToucanCamera();
		}
		return spToucanCamera;
	};   
private:
	CToucanCamera(){}; 
	static CToucanCamera * spToucanCamera;
	//回收指针
	class CGarbo 
	{
	public:
		~CGarbo()
		{ 
			if (CToucanCamera::spToucanCamera)
				delete CToucanCamera::spToucanCamera;
		}
	};
	static CGarbo Garbo;
public:
	//CAndorCamera(void);
	~CToucanCamera(void);
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
	void GetSpeed( int typ,int Index,float *fSpeed);
	int GetPixSpeedNum();
	void GetPixSpeed(int Index,float *fSpeed);

	void SetGain(int Index);
	void SetSpeed(int typ,int Index);
	void SetPixSpeed(int Index);
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
	void ImageDataAcquisition(BYTE* pBuffer,int XSize,int YSize);
	void SetImageMode(float ftime);//设置图像模式并设置积分时间
	void SetImageAccState(BOOL bState);
	void SetShutterAuto(int nMode,int nClose=0,int nOpen=0);
	void TerminateData();
	void SetBinning(int nMode);
	//zl_toucampro m_ccda;
private:
	BOOL gState;
	zl_toucampro m_ccda;
	zl_toucampro m_ccdb;
};

extern CToucanCamera *pToucanCamera;