#pragma once
#include "dfield.h"
#define  DATA_SUCCESS    0
#define  DATA_ERROR      1
#define  DATA_UNINIT     2
#define  DATA_UNCONNECT  3
#define  DATA_NULL       0xFF

class CCameraDll
{
public:
	CCameraDll(void);
	~CCameraDll(void);

	virtual int Connect()=0;
	virtual int DisConnect()=0;
	virtual BOOL GetInitState()=0;
	virtual CString GetDevName();
	virtual void GetDevSize(int *xpixSize,int *ypixSize);
	virtual void GetPixSize(float *fPix);
	virtual void GetExpTime(float *fTime);
	virtual bool GetTemper(int *iTemp);
	virtual void GetBinRange(int *BinMin,int *BinMax);
	
	virtual void SetCooler(int iMode);//0-关制冷 1-开制冷
	virtual void SetBinRange(int BinMin,int BinMax);
	virtual void SetExpTime(float fTime);
	virtual void SetTemper(int iTemp);
	virtual bool SetInitState(BOOL Init);
	virtual void SetLanguage(int nL);//0 中 1-英文
	virtual int ShowCameraSetDlg();
	virtual void DataAcquisition(double *pd,int nPixSize);
	virtual void XAxisData(double *pd,double *coeff,int Num,int nPixSize);
	virtual void TerminateData();
	virtual void SetPWM(int duty);//设置激光功率
	virtual void SetExternalPort(int port,HANDLE handle);//设置外触发接口
};
extern CCameraDll *pCameraDll;

struct AndorCCD
{
	BOOL bConnectS;         //连接状态
	int nReadMode;          //0-FVB OR 1-Bin 
	int iGain;
	int iSInedx;             //读取速率
	int iPixSInedx;             //读取速率
	int nTemp;				//温度
	int nGXPix;			//pixels x 个数
	int nGYPix;				//pixels y
	float fXPixSize;		    //像素尺寸
	float fExpTime;         // 积分时间
	int BinMin;             // bin模式下限
	int BinMax;         // bin模式上限
	CString sDevName;    //设备名称
	int nAD;               //通道 目前为1
	int isMirror;         //是否镜像
	int iCbpNum;        //相机坏点数量
	CString strCbp;        //相机坏点字符串 数据格式为XX;XX  XX为Pix像素，从0开始
	int HMirror; 
    int ShutterMode; //0-open 1-auto
	int nTrigger;
	CString ghostImageStrAbsolute;    //绝对位置
	CString ghostImageStrRelative;    //相对位置

};
struct MityCCD
{
	BOOL bConnectS;         //连接状态
	int nReadMode;          //0-FVB OR 1-Bin 2-Image 
    int iGain;
	int iOffset;             //偏置
	int nTemp;				//温度
	int nGXPix;			//pixels x 个数
	int nGYPix;				//pixels y
	float fXPixSize;		    //像素尺寸
	float fExpTime;         // 积分时间
	int BinMin;             // bin模式下限
	int BinMax;         // bin模式上限
	CString sDevName;    //设备名称
	int isMirror;         //是否镜像
	int iCbpNum;        //相机坏点数量
	CString strCbp;        //相机坏点字符串
	int HMirror; 
};

struct GreateyesCCD
{
	BOOL bConnectS;         //连接状态
	int nReadMode;          //0-FVB OR 1-Bin 
	int iGain;
	int iSInedx;             //读取速率
	int iPixSInedx;             //读取速率
	int nTemp;				//温度
	int nGXPix;			//pixels x 个数
	int nGYPix;				//pixels y
	float fXPixSize;		    //像素尺寸
	float fExpTime;         // 积分时间
	int BinMin;             // bin模式下限
	int BinMax;         // bin模式上限
	CString sDevName;    //设备名称
	int nAD;               //通道 目前为1
	int isMirror;         //是否镜像
	int iCbpNum;        //相机坏点数量
	CString strCbp;        //相机坏点字符串 数据格式为XX;XX  XX为Pix像素，从0开始
	int HMirror; 
	
    int nID;//设备ID
	int BinXMode;         // X轴Bin
	int BinYMode;         // Y轴Bin
};

struct ToucanCCD
{
	BOOL bConnectS;         //连接状态
	int nReadMode;          //0-FVB OR 1-Bin 
	int iGain;
	int iSInedx;             //读取速率
	int iPixSInedx;             //读取速率
	int nTemp;				//温度
	int nGXPix;			//pixels x 个数
	int nGYPix;				//pixels y
	float fXPixSize;		    //像素尺寸
	float fExpTime;         // 积分时间
	int BinMin;             // bin模式下限
	int BinMax;         // bin模式上限
	CString sDevName;    //设备名称
	int nAD;               //通道 目前为1
	int isMirror;         //是否镜像
	int iCbpNum;        //相机坏点数量
	CString strCbp;        //相机坏点字符串 数据格式为XX;XX  XX为Pix像素，从0开始
	int HMirror; 

	int nID;//设备ID
	int BinXMode;         // X轴Bin
	int BinYMode;         // Y轴Bin

	int offset;
	int bkoffset;
};

struct OTOCCD
{
	BOOL bConnectS;         //连接状态
	int nReadMode;          //0-FVB OR 1-Bin 2-Image 
	int iSpeed;             //读取速率
	int nTemp;				//温度
	int nGXPix;			//pixels x 个数
	int nGYPix;				//pixels y
	float fXPixSize;		    //像素尺寸
	float fExpTime;         // 积分时间
	int BinMin;             // bin模式下限
	int BinMax;             // bin模式上限
	CString sDevName;    //设备名称
	int isMirror;         //是否镜像
	int iCbpNum;        //相机坏点数量
	CString strCbp;        //相机坏点字符串
	int iCheckBK;         //扣除背景
	int iCheckLine;         //线性校正
	int iCheckInitensity;    //对比强度校正
	int iCheckAbInitensity;   //绝对强度校正
	int iCheckCool;         //是否有制冷模块
	int iRadioX;           //X轴读取方式
	int iRadioMode;       //硬件板子不带校正模块-0 带校正模块-1
	int HardWareMode;     //1-ee 0-hb/se
};
struct A1CCD {
	BOOL bConnectS;      //连接状态
	CString sDevName;    //设备名称
	
	int nGXPix;			 //x方向像素数
	int nGYPix;			 //y方向像素数
	float fXPixSize;		    //像素尺寸

	int nTemp;				//温度

	float fExpTime;         // 积分时间

	short iGain;

	readout_speed speed;   //读取速率
	int isMirror;         //是否镜像 -- 光谱
	bool HMirror;         //水平翻转 -- 影像

	int iCbpNum;           //相机坏点数量
	CString strCbp;        //相机坏点字符串  数据格式为XX;XX  XX为Pix像素 ，从0开始

	
	CString ghostImageStrAbsolute;    //绝对位置
	CString ghostImageStrRelative;    //相对位置
};
extern A1CCD    stuA1CCD;
extern AndorCCD stuAndorCCD;
extern MityCCD  stuMityCCD;
extern OTOCCD   stuOTOCCD;
extern GreateyesCCD   stuGreateyesCCD;
extern ToucanCCD stuToucanCCD;