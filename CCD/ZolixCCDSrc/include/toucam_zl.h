#ifndef  PRO_H
#define  PRO_H
#include "toucampro.h"
#include <string>

class zl_toucampro
{
public:
	zl_toucampro();
	~zl_toucampro();
public:
	bool SetDllPath(char* buf);
	bool Open(PVOID);
	bool OpenEx(PVOID, DWORD);
	bool ListDevices(PVOID, DWORD&);
	bool Close();
	bool Read(LPVOID, DWORD, LPDWORD);
	bool Write(LPVOID, DWORD, LPDWORD);
	bool ResetDevice();
	bool Purge();
	bool SetTimeouts(ULONG, ULONG);
	/*bool GetQueueStatus(LPDWORD);
	bool GetDriverVersion(LPDWORD);*/

	BYTE chkSumInt(BYTE*nbuf,int num);
	BYTE WriteReg(BYTE*nbuf,int num);
	BYTE ReadReg();
	int GetXSize();
	int GetYSize();
	void GetInit(int nMode);
	void GetMfData();
	float GetExpTime();
	void SetExpTime(float fExpTime);
	float GetTemp();
	void  SetTemp(int nTemp);
	void  SetBin(int nMode,int nYF,int nYE);//0-None 7-FVB;
	void  SetOneFrameNumber();
	void  SetReadFraneNumber();
	void  SetReadoutTriggerMode();  // Set usb ctrl bit to start acquisition
	void  getSingleFrameImage(BYTE* pBuffer,int XSize,int YSize);
	void  getSingleFrame(double* pBuffer,int XSize,int YSize,int offset);
	void  AbortCCD();
	void  SetGain(int nIndex);
	void  SetRun(bool bRet);
	void  clear();
private:
	toucam_ccdproc m_proc;
	FT_HANDLE m_ftHandle;

	int nNo;
	int nADC[2];
	int nDAC[2];
	int nDate[3];
	float TEC_DAC_gradient ;
	float TEC_DAC_intercept ;
	float TEC_ADC_gradient;
	float TEC_ADC_intercept ;
	
	
};
#endif