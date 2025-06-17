#pragma once
#include "PCOMM.H"
#pragma comment(lib,"PCOMM.lib")
#ifdef _DEBUG
	#pragma comment(lib,"PCOMM.lib")
#endif

class CSerialClass
{
public:
	CSerialClass(void);
	~CSerialClass(void);
public:
	bool SetStep(UINT nDev,int nPost);	//串口发送
	bool ResetHome(UINT nDev);
	bool SetStepFilter(UINT nDev,int nPost);
	bool SetZolixXYZMotor();
	bool ResetHomeFilter(UINT nDev);
	BOOL SetVolt(UINT nDev,CString volt);
	// 打开激光器串口
	BOOL OpenLaser(UINT nDev);
	// 关闭激光器串口
	void CloseLaser(UINT nDev);
	// 关闭激光器
	void CloseLaserPower(UINT nDev);
	BOOL OpenCameraLigth(UINT nDev,CString str);
	// 打开激光器
	void OpenLaserPower(UINT nDev);

	BOOL SetLed(UINT nDev,CString volt);
	BOOL SetLed_is_new(UINT nDev, CString volt);
	void MoveLed(bool bSt);
	BOOL OpenLed(UINT nDev);
	int ReadLed(int nNowPort);

	int  Duty(int volt);
	
	void set_board_flag(bool flag) { m_new_board = flag; }

	bool SetLaserRData(int nNowPort, int nPower);
	int WriteToLog_S(const char* str);
private:
	bool m_new_board;
	std::string m_receivedDataHex;

};

