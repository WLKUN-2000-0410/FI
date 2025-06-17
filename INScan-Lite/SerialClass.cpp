#include "stdafx.h"
#include "SerialClass.h"
#include <stdlib.h>


CSerialClass::CSerialClass(void)
	:m_new_board(false)
{
}

CSerialClass::~CSerialClass(void)
{
}


////打开摄像头灯
BOOL CSerialClass::OpenCameraLigth(UINT nDev,CString str)
{
	CString strResult;
	char pbuf[30];
	int len;
	memset(pbuf,0,sizeof(pbuf));
	int nNowPort =nDev;

	int strLength2 = str.GetLength() + 1;
	strncpy(pbuf, str, strLength2);	
	len = strlen(pbuf);			
	int bn=sio_write(nNowPort, pbuf, len);		//发送数据						/* Output Data function */
	return true;
}


int CSerialClass::WriteToLog_S(const char* str)
{

	char exePath[MAX_PATH] = { 0 };
	
	GetModuleFileNameA(NULL, exePath, MAX_PATH);

	
	char* pSlash = strrchr(exePath, '\\');
	if (pSlash)
	{
		*pSlash = '\0'; 
	}

	char logFilePath[MAX_PATH] = { 0 };
	sprintf_s(logFilePath, "%s\\FI-PC-Log.txt", exePath);


	FILE* pfile = nullptr;
	errno_t err = fopen_s(&pfile, logFilePath, "a+");
	if (err != 0 || pfile == nullptr)
	{
		return -1; 
	}

	
	SYSTEMTIME tm;
	GetLocalTime(&tm);

	char szDateTime[64] = { 0 };
	sprintf_s(szDateTime, "%02d:%02d:%02d.%03d",
		tm.wHour, tm.wMinute, tm.wSecond, tm.wMilliseconds);

	fprintf_s(pfile, "%s:%s\n", szDateTime, str);

	fclose(pfile);

	return 0;
}

///高瑀含添加，控制激光器电压
BOOL CSerialClass::SetVolt(UINT nDev,CString volt)
{
	CString strResult;
	char pbuf[30];
	int len;
	memset(pbuf,0,sizeof(pbuf));
	int nNowPort =nDev;
	int strLength2 = volt.GetLength();

	if (sSInfo.nLaserComMode==0)
	{
		WriteToLog_S("Laser--杏林--next sio_write(SetVolt)");
		switch (strLength2)
		{
		case 0:
		{
			strLength2 = 3;
			volt = "000";
		}
		break;
		case 1:
		{
			strLength2 = 3;
			volt = "00" + volt;
		}
		break;
		case 2:
		{
			strLength2 = 3;
			volt = "0" + volt;
		}
		break;
		case 3:
			break;
		}


		////strncpy(pbuf,volt,4);
		//if(m_new_board && volt == "000")
		//	strncpy(pbuf, volt, 3);
		//else
		strncpy(pbuf, volt, strLength2);
		len = strlen(pbuf) + 1;

		char logBuf[100] = { 0 };
		sprintf(logBuf, "Laser--sio_write(%s)", pbuf);
		WriteToLog_S(logBuf);
		int bn = sio_write(nNowPort,pbuf, len);		//发送数据						/* Output Data function */

		int duty = Duty(atoi(volt));

		//m_ZolixCCD.SetPWM(duty);
	}
	else   //"新产业,需按规定的格式发送"
	{
		WriteToLog_S("Laser--分支--新产业准备SetLaserRData");
		SetLaserRData(nNowPort,atoi(volt));
	}

	
	return true;
}
int  CSerialClass::Duty(int volt)
{
	CStringArray sa;
	CString tPath,str,strDuty;
	GetModuleFileName(NULL,tPath.GetBufferSetLength (MAX_PATH+1), MAX_PATH);
	tPath.ReleaseBuffer();
	int nPos=tPath.ReverseFind ('\\');
	str=tPath.Left (nPos);
	strDuty = str + _T("\\powerToDuty.txt");

	CStdioFile file;
	if (!file.Open(strDuty,CFile::modeRead|CFile::typeText)) 
	{
		return 0;
	}  	
	file.ReadString(str);
	Split(str,"\t",sa);

	int duty=atof(sa.GetAt(0))+ volt*atof(sa.GetAt(1))+
		volt*volt*atof(sa.GetAt(2))+
		volt*volt*volt*atof(sa.GetAt(3));

	return duty;
}

//  打开激光器串口
BOOL CSerialClass::OpenLaser(UINT nDev)
{
	CString strResult;
	
	int nNowPort =nDev;

	sio_close(nNowPort);
	int ret = sio_open(nNowPort);
	{
		CString logStr;
		logStr.Format(_T("激光器--连接结果sio_open(%d) returned: %d"), nNowPort, ret);
		WriteToLog_S((char*)(LPCTSTR)logStr);
	}
	if (ret != 0x00) {    // 如果激光器连接失败
		WriteToLog_S("激光器--sio_open(nNowPort)失败");
		return false;
	}
	
	int vf=sio_ioctl(nNowPort, B9600, P_NONE | BIT_8 | STOP_1);	/* Port Control, set baud, parity... */
	Sleep(200);

	if (!SetLaserRData(nNowPort, 0)) {
		WriteToLog_S("激光器--没有收到来自激光器的消息");
		m_receivedDataHex = "";
	}
	

	if (m_receivedDataHex == "55 AA 05 00 01 F4 FA ")//"新产业" m_receivedDataHex为执行SetLaserRData后接收到的数据
	{
		sSInfo.nLaserComMode = 1;
		sSInfo.LASERPOWERMIN = sSInfo.LaserPowerMin_XCY;
		sSInfo.LASERPOWERMAX = sSInfo.LaserPowerMax_XCY;
		return true;
	}
	else   //"杏林"
	{
	/*	char pbuf[30];
		int len;
		memset(pbuf, 0, sizeof(pbuf));
		Sleep(500);*/

		sSInfo.nLaserComMode = 0;
		sSInfo.LASERPOWERMIN = sSInfo.LaserPowerMin_XL;
		sSInfo.LASERPOWERMAX = sSInfo.LaserPowerMax_XL;
		return true;
		//sio_write(11, "000", 3);
		//Sleep(500);
		/*if (sio_read(nNowPort, pbuf,7) > 0)
		{
			WriteToLog_S("分支2.2");
			Sleep(20);
			if ((pbuf[0] == 82 || pbuf[0] == 0))
			{
				WriteToLog_S("分支2");
				return true;
			}
		}*/
				
	}
	//如果连接失败,关闭通信口
	sio_close(nNowPort);
	return false;
	
}
void CSerialClass::OpenLaserPower(UINT nDev)
{
	
	if (sSInfo.HLMODE==0)
	{
		SetVolt(nDev,_T("LDON"));
	}
	else
	{

		SetVolt(nDev,_T("LDOFF"));
	}

	if(sInfo.OminiType != 10)
		m_ZolixCCD.SetExternalPort(2, NULL);
}

void CSerialClass::CloseLaserPower(UINT nDev)
{
	if (sSInfo.HLMODE==0)
	{
		SetVolt(nDev,_T("LDOFF"));
	}
	else
	{
		SetVolt(nDev,_T("LDON"));
	}

	//if(sInfo.OminiType != 10)
		m_ZolixCCD.SetExternalPort(0, NULL);
}
void CSerialClass::CloseLaser(UINT nDev)
{
	CString strResult;
	char pbuf[30];
	int len;
	memset(pbuf,0,sizeof(pbuf));
	int nNowPort =nDev;
	int io=sio_close(nNowPort);//关闭串口
}

//  打开电机串口
BOOL CSerialClass::OpenLed(UINT nDev)
{
	
	CString strResult;
	char pbuf[30];
	int len;
	memset(pbuf,0,sizeof(pbuf));
	int nNowPort =nDev;

	if (sio_open(nNowPort)!=0x00){

		//AfxMessageBox(pMainFrame->LoadStringMeau("ID_COMPORT_OPEN_FAIL"));//串口打开失败
		return false;
	}
	int vf=sio_ioctl(nNowPort, B19200, P_NONE | BIT_8 | STOP_1);	/* Port Control, set baud, parity... */
	return true;
}
BOOL CSerialClass::SetLed(UINT nDev,CString volt)
{
	CString strResult;
	char pbuf[30];
	int len;
	memset(pbuf,0,sizeof(pbuf));
	int nNowPort =nDev;
	//volt += TEXT("\r");
	//volt += TEXT("\n");
	int strLength2 = volt.GetLength();

	strncpy(pbuf,volt,strLength2);	
	len = strLength2;

	//pbuf[len - 1]=0x0d;// \r
	pbuf[len] = 0x0d;// \r
	//  如果连接的是激光器的串口，当volt为NX,1时会打开激光

	{
		CString logStr;
		logStr.Format(_T("LOGLED05--(setled)Before sio_write: port=%d, pbuf content='%hs', size=%d"), nNowPort, pbuf, 30);
		WriteToLog_S((char*)(LPCTSTR)logStr);
	}
	int bn=sio_write(nNowPort, pbuf, len+1);		//发送数据						/* Output Data function */ 5 0 5 0 3 0(X) 5 3 5 3  3 5(√）
	Sleep(1000);
	int cn = sio_read(nNowPort, pbuf,30);// 如果cn不为0，表示能正确连接
	// lcz new 05/11
	if(cn != 0){
		WriteToLog_S("LOGLED06--(setled)cn!=0,能正常连接");
		return true;
	}
	else
		WriteToLog_S("LOGLED06--(setled)cn==0,不能正常连接");

		return false;
}

BOOL CSerialClass::SetLed_is_new(UINT nDev, CString volt)
{
    CString strResult;
	char pbuf[30];
	int len;
	memset(pbuf, 0, sizeof(pbuf));
	int nNowPort = nDev;
	//volt += TEXT("\r");
	//volt += TEXT("\n");
	int strLength2 = volt.GetLength();

	strncpy(pbuf, volt, strLength2);
	len = strLength2;

	//pbuf[len] = 0x0d;// \r
					 //  如果连接的是激光器的串口，当volt为NX,1时会打开激光
	int bn = sio_write(nNowPort, pbuf, len+1);		//发送数据						// Output Data function  5 0 5 0 3 0(X) 5 3 5 3  3 5(√）
	Sleep(1000);
	int cn = sio_read(nNowPort, pbuf, 30);// 如果cn不为0，表示能正确连接
										  
	if (cn != 0 && -1 != string(pbuf).find("OK"))
		return true;
	else
		return false;
}


// lcz new 23/05/23   读取当前步数
int CSerialClass::ReadLed(int nNowPort)
{
	CString volt=_T("?X");
	char pbuf[30];
	int len;
	//  lcz new 23/05/23  加上\r\n 
	//volt += TEXT(("\r"););
	//volt += TEXT(("\n"););
	memset(pbuf,0,sizeof(pbuf));
	int strLength2 = volt.GetLength();

	strncpy(pbuf,volt,strLength2);	
	len = strlen(pbuf);
	pbuf[len] = 0x0d;//  加\r
	int bn=sio_write(nNowPort, pbuf, len+1);
	Sleep(2000);
	char pData[30];
	int tempRes = sio_read(nNowPort, pbuf,30);

	for (int i=3;i<30;i++)
	{
		pData[i-3]=pbuf[i];
		if(pbuf[i]==0x0d)break;

	}

	return atoi(pData);
}
//  电机移动
void CSerialClass::MoveLed(bool bSt)
{
	WriteToLog_S("LOGLED02--MoveLed");
	CString strVolt;
	if(bSt)
	{
		WriteToLog_S("LOGLED03--(moveled)bst == true ,表示要开灯,接下来先发送?X");

		SetLed(sSInfo.nCOMLED,_T("?X"));
		strVolt.Format("+X,%d",sSInfo.LockSet);
	}
	else
	{
		WriteToLog_S("LOGLED04--(moveled)bst == FALSE");

		strVolt.Format("HX");
	}

	SetLed(sSInfo.nCOMLED,strVolt);
}

//RS232 设置 通讯

bool CSerialClass::SetLaserRData(int nNowPort,int nPower)
{
	m_receivedDataHex.clear();

	int x = nPower;
	char pbuf[30];

	pbuf[0] = 0x55;
	pbuf[1] = 0xAA;
	pbuf[2] = 0x05;
	pbuf[3] = 0x00;
	pbuf[4] = x>>8&0xff;
	pbuf[5] = x & 0xff;
	pbuf[6] = (pbuf[2]+ pbuf[4]+ pbuf[5]) & 0xff;
	
	{
		CString hexStr;
		for (int i = 0; i < 7; i++)
		{
			// AppendFormat：%02X 表示两位大写十六进制数字
			hexStr.AppendFormat(_T("%02X "), (unsigned char)pbuf[i]);
		}
		CString logStr;
		logStr.Format(_T("激光器--(SetLaserRData) 将要发送的数据: port=%d, pbuf bytes: %s"), nNowPort, hexStr);
		WriteToLog_S((char*)(LPCTSTR)logStr);
	}
	int bn = sio_write(nNowPort, pbuf, 7);		//发送数据	  					/* Output Data function */
	Sleep(20);
	
	DWORD startTime = GetTickCount();  // 开始时间
	int received = 0;
	char recvBuf[256];                 
	memset(recvBuf, 0, sizeof(recvBuf));
	while (true)
	{
		char ch;
		int bn = sio_read(nNowPort, &ch, 1);  // 逐字节读取
		if (bn > 0)
		{
			recvBuf[received++] = ch;
			startTime = GetTickCount();  // 重置超时时间
	
		}
		//超时判断
		if (GetTickCount() - startTime > 100)
		{
			break;  
		}

	}
	char hexStr[512] = { 0 };
	char* ptr = hexStr;
	for (int i = 0; i < received; i++)
	{
		sprintf(ptr, "%02X ", (unsigned char)recvBuf[i]);
		ptr += 3;  
	}
	m_receivedDataHex = hexStr;
	{
		CString logStr;
		logStr.Format(_T("激光器--(SetLaserRData) 接收到的数据Receive %d bytes: %s"), received, hexStr);
		WriteToLog_S((char*)(LPCTSTR)logStr);
	}
	return (received > 0);  
}