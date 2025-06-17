#pragma once
#include "CameraDll.h"
#include <Windows.h>
class CA1Camera : public CCameraDll {
public:
	static CA1Camera * GetInstance() {
		if (spA1Camera == NULL) {
			spA1Camera = new CA1Camera();
		}
		return spA1Camera;
	}
private:
	CA1Camera() {};                 //构造函数私有化,确保外部无法直接创建实例,只能通过getinstance()获取
	static CA1Camera * spA1Camera;  //用于保存单例对象的指针

	class CGarbo {
	public:
		~CGarbo() {
			if (CA1Camera::spA1Camera)
				delete CA1Camera::spA1Camera;
		}
	};
	static CGarbo Garbo;

public:
	BOOL nInitState;
	~CA1Camera(void);
	

	int Connect();                                        //纯虚函数,必须强制替代
	int DisConnect();                                     //纯虚函数
	BOOL GetInitState();                                  //纯虚函数
	bool SetInitState(BOOL Init);
	CString GetDevName();                                 //获取设备名称
	void GetDevSize(int *xpixSize, int *ypixSize);        //获取x轴,y轴像素数
	void GetPixSize(float *fPix);                         //获取像素尺寸
	
	void SetTemper(int iTemp);    //设置制冷温度
	void SetCooler(int iMode);    //开启关闭制冷
	bool GetTemper(int *iTemp);   //获取温度

	void ReadAllIni();
	void WriteAllIni();

	void GetExpTime(float *fTime);  //获取积分时间
	void SetExpTime(float fTime);   //设置积分时间
	
	void DataAcquisition(double *pd, int nPixSize);   //图像采集
	void ImageDataAcquisition(unsigned short * plBuffer, BYTE* pBuffer, int XSize, int YSize);

	int  WriteToLog(char* str);//用于测试,日志

	int ShowCameraSetDlg();

	void GetGain(char id, short* gain);
	void SetGainA1(short Index);

	bool GetSpeed(ptr_readout_speed speed);
	bool SetSpeed(readout_speed speed);

	bool isMirr();

	int GetXLen();

	bool GetBin(short * bin_h,short * bin_v);
	
	bool SetReadoutMode(readout_mode mode);

	bool m_CameraState;


	//void RemoveGhosting(unsigned short* plBuffer, int XSize, int YSize, const CString& ghostRegionStr);
	template <typename T>
	void RemoveGhosting(T* plBuffer, int XSize, int YSize, const CString& ghostRegionStr);
private:
	BOOL gState;
	char m_handle;

	std::vector<unsigned short> _x;
	std::vector<unsigned short> _y;
	bool _ex_trigger;               //是否为外触发
	std::vector<std::vector<unsigned short>> _buff;

};
struct OpenThreadParam
{
	char serial[32];   // 要打开设备的序列号
	char handle;       // 保存 dfield_open 的返回句柄
	bool finished;     // 标记线程是否结束
	HANDLE hCancelEvent;   // 用来协作式通知线程退出
};
extern CA1Camera * pA1Camera;
