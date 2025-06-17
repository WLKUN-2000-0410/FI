#include "StdAfx.h"
#include "A1Camera.h"
#include "dfield.h"
#include <thread>
#include <mutex>
#include "DlgA1Set.h"
#include "DataProcess.h"
//#include "DataProcess.h"

std::mutex _mt;

// 静态成员定义（必须在 .cpp 中定义一次）
CA1Camera * CA1Camera::spA1Camera = NULL;
CA1Camera * pA1Camera = NULL;
CA1Camera::CGarbo CA1Camera::Garbo;


CA1Camera::~CA1Camera(void)
{
	DisConnect();
}

DWORD WINAPI OpenThreadFunc(LPVOID lpParam)
{
	OpenThreadParam* p = (OpenThreadParam*)lpParam;
	p->finished = false;

	p->handle = dfield_open(p->serial);

	p->finished = true;
	return 0;
}

//int CA1Camera::Connect()
//{
//	WriteToLog("进入连接函数");
//	if (!GetInitState()) {
//		return DATA_UNINIT;
//	}
//
//	dfield_usb_mode(false);                         //false:0 com口
//
//	unsigned short count = dfield_enum_dev_count(); //获取设备数量
//	if (count == 0) {
//		WriteToLog("Error:The number of devices obtained is 0 by dfield_enum_dev_count");
//		return E00;                                 //直接返回1,表示连接失败
//	}
//
//	const DWORD TIMEOUT_MS = 100;                   //超时时间150ms
//	
//
//	bool bFound = false;                            // 有任何一个打开成功
//													// 设备句柄
//	char serial[32] = { 0 };                        // 存储序列号
//
//	for (int i = 0; i < count; i++)
//	{
//		memset(serial, 0, sizeof(serial));
//		if (!dfield_enum_dev_sn((char)i, serial, 32)) {
//			continue;                               //未能获取序列号,继续尝试下一个设备
//		}
//
//		OpenThreadParam param;
//		memset(&param, 0, sizeof(param));
//		strcpy_s(param.serial, sizeof(param.serial), serial);
//		param.handle = -1;
//		param.finished = false;
//
//		HANDLE hThread = CreateThread(
//			NULL, 0,
//			[](LPVOID lpParam) -> DWORD {
//			OpenThreadParam* pParam = (OpenThreadParam*)lpParam;
//			// 调用底层函数
//			pParam->handle = dfield_open(pParam->serial);
//			pParam->finished = true;
//			return 0;
//		},
//			&param,
//			0,
//			NULL
//			);
//		if (hThread == NULL)
//		{
//			CString msg;
//			msg.Format(_T("Error:设备 %d: 创建线程失败 (serial=%s)"), i, serial);
//			CT2A asciiMsg(msg);
//			WriteToLog(asciiMsg);
//			continue;
//		}
//		DWORD waitResult = WaitForSingleObject(hThread, TIMEOUT_MS);
//		if (waitResult == WAIT_TIMEOUT)
//		{
//			// 超时，则强制终止线程
//			/*CString msg;
//			msg.Format(_T(" %s: dfield_open 调用超时，强制结束线程 "), serial);
//			CT2A asciiMsg(msg);
//			WriteToLog(asciiMsg);*/
//
//			TerminateThread(hThread, 0);
//			CloseHandle(hThread);
//			continue;
//		}
//		else
//		{
//			// 线程在超时前结束，关闭线程句柄
//			CloseHandle(hThread);
//		}
//
//
//		if (param.handle > 0)                             //表示连接成功
//		{
//
//			//CString msg;                            //测试用,后面删掉
//			//msg.Format(_T("成功打开设备 %d (serial=%s)"), i, serial);
//			//CT2A asciiMsg(msg);
//			//WriteToLog(asciiMsg);
//
//			m_handle = param.handle;
//			bFound = true;
//			break;                                   //不再尝试其他设备
//		}
//		else
//		{
//			continue;                                //能获取到序列号(com),但无法连接,尝试下一个设备
//		}
//
//		
//	}//for (int i = 0; i < count; i++)
//
//	if(!bFound)
//	{
//		WriteToLog("ERROR: 所有设备连接失败");
//		return DATA_ERROR;
//	}
//
//	//WriteToLog("相机连接成功");                    //能走到这里说明连接成功
//	stuA1CCD.bConnectS = TRUE;
//
//	//测试1--获取设备名称
//	//CString nametest;
//	//nametest = GetDevName();
//
//	//测试2--像素数量,像素尺寸
//	//int xpix = 0, ypix = 0;
//	//GetDevSize(&xpix, &ypix);
//
//	//测试3--获取单个像素尺寸
//	//float pixSize = 0.0f;
//	//GetPixSize(&pixSize);
//
//	//获取积分时间
//	GetExpTime(&stuA1CCD.fExpTime);
//
//
//	//读取配置文件
//	ReadAllIni();
//
//	dfield_set_readout_mode(m_handle, mode_fvb);//程序启动默认切换成光谱采集模式,只有设置界面才设置成影响采集模式,且会自动切回来
//	
//	return DATA_SUCCESS;
//}

//尝试新版本
//int CA1Camera::Connect() {
//	WriteToLog("进入连接函数");
//
//	if (!GetInitState()) {
//		return DATA_UNINIT;
//	}
//	dfield_usb_mode(false);   
//
//	unsigned short count = dfield_enum_dev_count(); //获取设备数量
//
//	const DWORD MAX_OPEN_WAIT_MS = 100;  // 第一段超时 -- 正常等待 dfield_open 完成的时间
//	const DWORD CANCEL_WAIT_MS = 50;     // 第二段超时 -- 通知它退出后再等它收尾的时间
//
//	bool bFound = false;                 // 记录是否有任何一个设备打开成功
//	char serial[32] = { 0 };
//
//	for (int i = 0; i < count; i++)
//	{
//		memset(serial, 0, sizeof(serial));
//		dfield_enum_dev_sn((char)i, serial, 32);  //获取设备序列号
//		{
//			CString msg;
//			msg.Format(_T("设备%d:获取序列号%s"), i, serial);
//			CT2A asciiMsg(msg);
//			WriteToLog(asciiMsg);
//		}
//
//		// ============== 准备线程参数与取消事件 ==============
//		OpenThreadParam param;
//		ZeroMemory(&param, sizeof(param));
//		strcpy_s(param.serial, sizeof(param.serial), serial);
//		param.handle = -1;
//		param.finished = false;
//
//		//创建一个取消事件对象
//		param.hCancelEvent = CreateEvent(nullptr, TRUE, FALSE, nullptr);
//		if (param.hCancelEvent == nullptr) {
//			CString msg;
//			msg.Format(_T("设备 %d (serial=%s): 创建取消事件失败"), i, serial);
//			CT2A asciiMsg(msg);
//			WriteToLog(asciiMsg);
//			continue;
//		}
//
//		//创建线程来执行事件
//		HANDLE hThread = CreateThread(nullptr,0,OpenThreadFunc,&param,0,nullptr);
//
//		// ============== 等待线程完成或超时 ==============
//		DWORD waitResult = WaitForSingleObject(hThread, MAX_OPEN_WAIT_MS);
//		if (waitResult == WAIT_TIMEOUT)
//		{
//			CString msg;
//			msg.Format(_T("设备 %d (serial=%s): dfield_open 超时，向线程发送中止信号,安全退出"), i, serial);
//			CT2A asciiMsg(msg);
//			WriteToLog(asciiMsg);
//
//			// 通知线程退出
//			SetEvent(param.hCancelEvent);
//			DWORD cancelWait = WaitForSingleObject(hThread, CANCEL_WAIT_MS);
//			if (cancelWait == WAIT_TIMEOUT)
//			{
//				// 线程仍然没有退出，这里再决定是否要强杀
//				// 不推荐，但如果必须:
//				// TerminateThread(hThread, 0);
//				CString msg2;
//				msg2.Format(_T("设备 %d (serial=%s): 线程未在中止信号后退出，可能需要强制结束"), i, serial);
//				CT2A asciiMsg2(msg2);
//				WriteToLog(asciiMsg2);
//			}
//			CloseHandle(hThread);
//			CloseHandle(param.hCancelEvent);
//			continue;
//		}
//		else
//		{
//			// waitResult == WAIT_OBJECT_0 或者 WAIT_FAILED
//			// 如果 waitResult == WAIT_FAILED, 可以用 GetLastError() 做更多处理
//			CloseHandle(hThread);
//		}
//
//		// 句柄>0 => 打开成功
//		if (param.handle > 0)
//		{
//			m_handle = param.handle;
//			bFound = true;
//
//			{
//				CString msg;
//				msg.Format(_T("成功打开设备 %d (serial=%s)"), i, serial);
//				CT2A asciiMsg(msg);
//				WriteToLog(asciiMsg);
//			}
//			// 成功则停止尝试其它设备
//			break;
//		}
//		else
//		{
//			CString msg;
//			msg.Format(_T("设备 %d (serial=%s): 连接失败"), i, serial);
//			CT2A asciiMsg(msg);
//			WriteToLog(asciiMsg);
//			// 继续尝试下一个设备
//		}
//
//
//	}
//
//	if (!bFound)
//	{
//		WriteToLog("ERROR: 所有设备连接失败");
//		return DATA_ERROR;
//	}
//
//	stuA1CCD.bConnectS = TRUE;
//
//	GetExpTime(&stuA1CCD.fExpTime);
//	ReadAllIni();
//	dfield_set_readout_mode(m_handle, mode_fvb);
//
//	return DATA_SUCCESS;
//
//}


//最简单可用版本
//int CA1Camera::Connect()
//{
//	WriteToLog("进入连接函数");
//	if (!GetInitState()) {
//		return DATA_UNINIT;
//	}
//	dfield_usb_mode(false);                     //false--com口
//
//	const DWORD TIMEOUT_MS = 100;   //超时时间
//
//	bool bFound = false;            //有任何一个打开成功  
//
//
//	 直接尝试打开 com6
//	m_handle = dfield_open("com10");
//	if (m_handle <= 0) {
//		WriteToLog("ERROR: 连接com10失败");
//		return DATA_ERROR;
//	}
//
//	 能执行到这里说明 com6 已经连接成功
//	WriteToLog("成功打开 com10");
//	stuA1CCD.bConnectS = TRUE;
//
//	 下面这些是你原先的后续初始化操作
//	 例如获取积分时间、读取配置等
//	GetExpTime(&stuA1CCD.fExpTime);
//	ReadAllIni();
//	dfield_set_readout_mode(m_handle, mode_fvb);
//
//	return DATA_SUCCESS;
//}
int CA1Camera::Connect()
{
	WriteToLog("进入连接函数");

	if (!GetInitState()) {
		return DATA_UNINIT;
	}

	// 使用 COM 口模式
	dfield_usb_mode(false);

	// 获取设备数量
	unsigned short count = dfield_enum_dev_count();
	if (count == 0) {
		WriteToLog("Error: 通过 dfield_enum_dev_count 获取的设备数量为 0");
		return E00;  // 直接返回错误码
	}
	else {
		CString msg;
		msg.Format(_T("通过 dfield_enum_dev_count 获取的设备数量为 %d"), count);
		CT2A asciiMsg(msg);
		WriteToLog(asciiMsg);
	}

	bool bFound = false;  // 标识是否有设备成功打开
	char serial[32] = { 0 };

	// 遍历所有设备
	for (int i = 0; i < count; i++)
	{
		// 清空序列号缓存
		memset(serial, 0, sizeof(serial));
		if (!dfield_enum_dev_sn((char)i, serial, 32)) {
			// 如果获取序列号失败，则记录日志，尝试下一个设备
			CString msg;
			msg.Format(_T("设备 %d: 未能获取序列号"), i);
			CT2A asciiMsg(msg);
			WriteToLog(asciiMsg);
			continue;
		}

		{
			CString msg;
			msg.Format(_T("设备 %d: 获取到序列号 %s"), i, serial);
			CT2A asciiMsg(msg);
			WriteToLog(asciiMsg);
		}
		int handle = dfield_open(serial);

		// 判断句柄是否有效 (> 0 表示打开成功)
		if (handle > 0)
		{
			m_handle = handle;
			bFound = true;

			{
				CString msg;
				msg.Format(_T("成功打开设备 %d (serial=%s)"), i, serial);
				CT2A asciiMsg(msg);
				WriteToLog(asciiMsg);
			}
			break;  // 成功后不再尝试其它设备
		}
		else
		{
			CString msg;
			msg.Format(_T("设备 %d (serial=%s): 连接失败"), i, serial);
			CT2A asciiMsg(msg);
			WriteToLog(asciiMsg);
			// 继续尝试下一个设备
		}
	}

	if (!bFound)
	{
		WriteToLog("ERROR: 所有设备连接失败");
		return DATA_ERROR;
	}

	// 标记连接成功
	stuA1CCD.bConnectS = TRUE;

	// 获取积分时间、读取配置文件及设置读出模式
	GetExpTime(&stuA1CCD.fExpTime);
	ReadAllIni();
	dfield_set_readout_mode(m_handle, mode_fvb);

	return DATA_SUCCESS;


}

int CA1Camera::DisConnect()
{
	if (m_handle > 0) {
		dfield_close(m_handle);
		//WriteToLog("A1相机断开连接成功");

		// 重置句柄
		m_handle = 0;
	}
	// TODO: 调用 A1 相机的底层 SDK 或驱动的断开函数
	stuA1CCD.bConnectS = FALSE;
	return DATA_SUCCESS;
}


BOOL CA1Camera::GetInitState()
{
	return nInitState;
}

bool CA1Camera::SetInitState(BOOL Init) {
	
	nInitState = Init;
	gState = FALSE;
	return DATA_SUCCESS;
}
//int CA1Camera::WriteToLog(char* str)
//{
//	FILE* pfile;
//	fopen_s(&pfile, "D:\\FILog.txt", "a+");   //FILE_PATH 是个宏, 是文件的位置,如#define FILE_PATH "D:\\FILog.txt"
//
//	if (pfile == NULL)
//	{
//		return -1;
//	}
//
//	char szDateTime[20];
//	SYSTEMTIME tm;
//	GetLocalTime(&tm);
//	sprintf(szDateTime, "%02d:%02d:%02d.%03d", tm.wHour, tm.wMinute, tm.wSecond, tm.wMilliseconds);
//	fprintf_s(pfile, "%s:%s\n", szDateTime, str);
//
//	fclose(pfile);
//
//	return 0;
//}
int CA1Camera::WriteToLog(char* str)
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
CString CA1Camera::GetDevName() {
	CString devName;

	dfield_dev_info devInfo;
	memset(&devInfo, 0, sizeof(devInfo));

	if (!dfield_get_info(m_handle, &devInfo))
	{
		WriteToLog("获取设备信息失败");
		devName = _T("未知设备");
		stuA1CCD.sDevName = devName;

		//log测试
		CString msg;
		msg.Format(_T("设备名称: %s"), devName);
		CT2A asciiMsg(msg);
		WriteToLog(asciiMsg);


		return devName;
	}
	//先假设设备名称存储在
	devName = devInfo.model;
	stuA1CCD.sDevName = devName;

	return devName;

}
void CA1Camera::GetDevSize(int *xpixSize, int *ypixSize)
{

	short sx = 0, sy = 0;
	dfield_get_detector_size(m_handle, &sx, &sy);

	*xpixSize = (int)sx;
	*ypixSize = (int)sy;

	stuA1CCD.nGXPix = *xpixSize;
	stuA1CCD.nGYPix = *ypixSize;

	// 写入日志
	CString msg;
	msg.Format(_T("设备分辨率: %d x %d 像素"), *xpixSize, *ypixSize);
	CT2A asciiMsg(msg);
	WriteToLog(asciiMsg);
}
void CA1Camera::GetPixSize(float *fPix)
{
	short width_um = 0, height_um = 0;
	dfield_get_pixel_size(m_handle, &width_um, &height_um);

	


	// 假设 x 和 y 方向的像素尺寸相同，则以 x 方向尺寸为准
	*fPix = static_cast<float>(width_um);

	// 更新结构体中的像素尺寸变量
	stuA1CCD.fXPixSize = *fPix;

	// 构造日志信息，显示宽度和高度两个值
	CString msg;
	msg.Format(_T("单个像素尺寸: 宽度 = %d 微米, 高度 = %d 微米"), width_um, height_um);
	CT2A asciiMsg(msg);
	WriteToLog(asciiMsg);

}
void CA1Camera::SetTemper(int iTemp)
{
	stuA1CCD.nTemp = iTemp;
}

void CA1Camera::SetCooler(int iMode)
{
	//SetTemper(stuA1CCD.nTemp);  
	if (!dfield_set_temperature(m_handle, static_cast<float>(stuA1CCD.nTemp))) {
		WriteToLog("设置温度失败");
	}
	else
	{
		// 设置成功，写入日志
		CString msg;
		msg.Format(_T("成功设置设备温度: %d°C"), stuA1CCD.nTemp);
		CT2A asciiMsg(msg);
		WriteToLog(asciiMsg);
	}

	if (iMode == 1)            //开启制冷
	{
		if (!dfield_start_cooling(m_handle))
		{
			WriteToLog("开启制冷失败");
		}
		else
		{
			CString msg;
			msg.Format(_T("成功开启制冷"));
			CT2A asciiMsg(msg);
			WriteToLog(asciiMsg);
		}
	}
	else if (iMode == 0)  // 关闭制冷
	{
		if (!dfield_stop_cooling(m_handle))
		{
			WriteToLog("关闭制冷失败");
		}
		else
		{
			CString msg;
			msg.Format(_T("成功关闭制冷"));
			CT2A asciiMsg(msg);
			WriteToLog(asciiMsg);
		}
	}
	
}
bool CA1Camera::GetTemper(int *iTemp)
{
	//// 获取实际温度（假设使用 dfield_get_real_temperature 来获取温度）
	float realTemperature = 0.0f;
	if (!dfield_get_real_temperature(m_handle, &realTemperature))
	{
		WriteToLog("获取实际温度失败");
		return false;
	}

	// 将实际温度转换为整数并返回
	*iTemp = static_cast<int>(realTemperature);

	/*CString msg;
	msg.Format(_T("当前实际温度: %d°C"), *iTemp);
	CT2A asciiMsg2(msg);
	WriteToLog(asciiMsg2);*/

	/*float real = 0, in = 0, out = 0;
	dfield_get_real_temperature(m_handle, &real);
	dfield_get_internal_temperature(m_handle, &in);
	dfield_get_external_temperature(m_handle, &out);

	CString title;
	title.Format(_T("DField-NIR-Dx [探测器温度: %.1f, 内部温度: %.1f, 外壳温度: %.1f]"), real, in, out);
	CT2A asciiMsg(title);
	WriteToLog(asciiMsg);*/
	
	return true;
}

void CA1Camera::SetExpTime(float fTime)
{
	if (fTime != stuA1CCD.fExpTime) {  //积分时间不重复设置

		// 将秒转换为微秒
		int expTimeMicroseconds = static_cast<int>(fTime * 1000000);

		if (dfield_set_exposure(m_handle, expTimeMicroseconds))
		{
			stuA1CCD.fExpTime = fTime;  // 更新结构体中的积分时间
			/*CString msg;
			msg.Format(_T("z成功设置积分时间: %.6f 秒"), fTime);
			CT2A asciiMsg(msg);
			WriteToLog(asciiMsg);*/
		}
		else
		{
			WriteToLog("z设置积分时间失败");
		}
	}
	
}
void CA1Camera::GetExpTime(float *fTime)
{
	
	int expTimeMicroseconds = 0; // 积分时间（微秒）
	if (dfield_get_exposure(m_handle, &expTimeMicroseconds))
	{
		*fTime = static_cast<float>(expTimeMicroseconds) / 1000000.0f;  // 将微秒转换为秒
		stuA1CCD.fExpTime = *fTime;  // 更新结构体中的积分时间
		/*CString msg;
		msg.Format(_T("当前积分时间: %.6f 秒"), *fTime);
		CT2A asciiMsg(msg);
		WriteToLog(asciiMsg);*/
	}
	else
	{
		*fTime = -1.0f;  // 设置无效值表示获取失败
		WriteToLog("获取积分时间失败");
	}
}
//坏点模拟
void SimulatePeakBadRegion(double* data, int dataSize, int centerIndex, double peakValue, int peakWidth)
{
	// 安全检查，确保参数有效
	if (!data || peakWidth <= 0 || centerIndex < 0 || centerIndex >= dataSize) {
		return;
	}

	// 计算尖峰影响的起始和结束索引，并进行边界检查，防止数组越界
	int startIndex = max(0, centerIndex - peakWidth);
	int endIndex = min(dataSize - 1, centerIndex + peakWidth);

	// 遍历受影响的区域
	for (int i = startIndex; i <= endIndex; ++i)
	{
		// 计算当前点到中心点的距离
		int distance = abs(i - centerIndex);

		// 根据距离计算衰减因子（线性衰减），中心点为1，边缘为0
		double falloff = 1.0 - (double)distance / peakWidth;

		// 计算当前点应该增加的值，并加到原始数据上
		// 这样做的好处是可以在原始信号的基础上叠加一个尖峰
		if (falloff > 0) {
			data[i] += peakValue * falloff;
		}
	}
}
void CA1Camera::DataAcquisition(double *pd, int nPixSize) {
	unsigned short* short_buff = new unsigned short[nPixSize];

		dfield_capture(m_handle);                                            // 执行图像采集

		bool dataSuccess = dfield_get_data(m_handle, short_buff, nPixSize);  // 从设备中获取数据
		if (!dataSuccess) {
			WriteToLog("数据获取失败");
			delete[] short_buff;
			return;  // 数据获取失败，返回
		}

		// 将采集到的 unsigned short 数据转换为 double 存储到 pd 中
		for (int i = 0; i < nPixSize; i++) {
			pd[i] = (double)short_buff[i];
		}

		//////坏点模拟 -- test --add bad point
		//int badPoints[] = { 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20 }; // 这些是坏点索引，可以根据需要调整
		//int numBadPoints = sizeof(badPoints) / sizeof(badPoints[0]);

		//for (int i = 0; i < numBadPoints; i++) {
		//	if (badPoints[i] >= 0 && badPoints[i] < nPixSize) {
		//		pd[badPoints[i]] = 7000;  
		//	}
		//}
		//pd[badPoints[0]] = 4000;
		//pd[badPoints[19]] = 5000;

		// ==================== 新的坏点区域模拟 - 开始 ====================
		// 您可以在这里自由调整参数来改变尖峰的位置、高度和胖瘦
		//int    peakCenter = 350;     // 尖峰的中心位置（例如在第350个像素点）
		//double peakHeight = 5000.0;  // 尖峰的高度（会叠加在原始信号上）
		//int    peakWidth = 20;       // 尖峰的单侧宽度（越小越尖锐，越大越胖）
		//SimulatePeakBadRegion(pd, nPixSize, peakCenter, peakHeight, peakWidth);

		//peakCenter = 1200;     // 尖峰的中心位置（例如在第350个像素点）
		//peakHeight = 5000.0;   // 尖峰的高度（会叠加在原始信号上）
		//peakWidth = 20;        // 尖峰的单侧宽度（越小越尖锐，越大越胖）
		//SimulatePeakBadRegion(pd, nPixSize, peakCenter, peakHeight, peakWidth);

		pd = CamPixProcessing<double, int, CString>(
			pd,  
			nPixSize,    
			stuA1CCD.iCbpNum,                          
			stuA1CCD.strCbp    
			);
		
		CIsMirror<int, double>(
			stuA1CCD.isMirror,  
			nPixSize,
			pd,                 
			pd                  
			);
		RemoveGhosting1D_CrossFade(pd, nPixSize, stuA1CCD.ghostImageStrAbsolute);
		delete[] short_buff;  // 释放临时缓冲区
}

int CA1Camera::ShowCameraSetDlg() {
	//if ((GetInitState()==FALSE)||(stuA1CCD.bConnectS==FALSE)) {  //必须在已连接的情况下
	//	return DATA_UNCONNECT;
	//}
	g_bOpenDlg = true; //表示打开设置框

	ZOLIX_DLL_MFC_Res::CDlgA1Set dlg;
	dlg.DoModal();
	g_bOpenDlg = false;
	return DATA_SUCCESS;

}
void CA1Camera::SetGainA1(short gain) {

	if (!dfield_set_pga_gain(m_handle, gain)) {

		AfxMessageBox("设置增益失败");
		
	}
	else {
		AfxMessageBox("设置增益成功");
		
	}



}
void CA1Camera::GetGain(char id, short* gain) {
	
	if (!dfield_get_pga_gain(m_handle,gain)) {
		AfxMessageBox(_T("读取增益失败."));
		return;
	}
	else {
		CString logMsg;
		logMsg.Format(_T("读取增益成功: %d"), *gain);
		CT2A asciiMsg(logMsg);
		WriteToLog(asciiMsg);

	}

}
//void CA1Camera::SetGain(int Index)
//{
//	// 将 int 转换为 short 类型
//	short shortIndex = static_cast<short>(Index);
//
//	// 将 shortIndex 作为参数传入
//	short a = 2;
//	dfield_set_pga_gain(m_handle, 2);
//
//	AfxMessageBox(_T("设置增益成功"));
//}

bool CA1Camera::GetSpeed(ptr_readout_speed speed)
{
	bool ret= dfield_get_readout_speed(m_handle, speed);  
	
	if (!ret) {  
		
		AfxMessageBox(_T("读取速率失败"));
		
	}
	return ret;
}

bool CA1Camera::SetSpeed(readout_speed speed)
{
    bool ret = dfield_set_readout_speed(m_handle, speed);  //ret = 1
	if (!ret) {  
		AfxMessageBox(_T("设置速率失败"));
	}
	
	return ret;
}

int CA1Camera::GetXLen() {
	short width = 0, height = 0;
	dfield_get_detector_size(m_handle, &width, &height);

	short bin_h = 1, bin_v = 1;
	dfield_get_bin(m_handle, &bin_h, &bin_v);

	int len = width / bin_h;


	return len;
}

bool CA1Camera::isMirr() {
	return stuA1CCD.isMirror;
}


void CA1Camera::ReadAllIni() {

	ManageZolixIni->ReadIni();

}
void CA1Camera::WriteAllIni() {

	ManageZolixIni->WriteIni();

}
void CA1Camera::ImageDataAcquisition(unsigned short * plBuffer, BYTE* pBuffer, int XSize, int YSize) {
	
	//1.获取数据
	dfield_capture(m_handle);
			

	if (!dfield_get_data(m_handle, plBuffer, XSize*YSize)) {
		AfxMessageBox("图像采集失败");
		//m_CameraState = false;
		return;
	}
	m_CameraState = true;

	//2.去鬼像
	RemoveGhosting(plBuffer, XSize, YSize, stuA1CCD.ghostImageStrAbsolute);
	
	//3.遍历 plBuffer 数组，计算图像中的最小值和最大值（动态范围）
	int nNum = XSize*YSize;
	int iMin = 65535, iMax = 0;
	for (int i = 0; i < nNum; i++) {
		if (plBuffer[i] > iMax)
			iMax = plBuffer[i];
		if (plBuffer[i] < iMin)
			iMin = plBuffer[i];
	}
	int range = iMax - iMin;
	if (range == 0) {
		range = 1; // 避免除零
	}


	//4.对采集数据进行归一化，将每个像素值映射到 0～255 范围
          //  并生成 RGB 格式数据（这里三个通道赋相同的灰度值）
	for (int i = 0; i < nNum; i++) {
		int value = (plBuffer[i] - iMin) * 255 / range;
		pBuffer[3 * i] = static_cast<BYTE>(value);
		pBuffer[3 * i + 1] = static_cast<BYTE>(value);
		pBuffer[3 * i + 2] = static_cast<BYTE>(value);
	}
	//5.影像翻转
	if (stuAndorCCD.HMirror) {
		// 每一行的宽度为 XSize 像素，每个像素有 3 个字节
		for (int row = 0; row < YSize; row++) {
			for (int col = 0; col < XSize / 2; col++) {
				int leftIndex = row * XSize + col;
				int rightIndex = row * XSize + (XSize - 1 - col);
				// 交换三个通道的数据
				for (int k = 0; k < 3; k++) {
					BYTE temp = pBuffer[3 * leftIndex + k];
					pBuffer[3 * leftIndex + k] = pBuffer[3 * rightIndex + k];
					pBuffer[3 * rightIndex + k] = temp;
				}
			}
		}
	}

}
bool CA1Camera::GetBin(short * bin_h, short * bin_v) {

	return dfield_get_bin(m_handle, bin_h, bin_v);

}

bool CA1Camera::SetReadoutMode(readout_mode mode) {

	return dfield_set_readout_mode(m_handle, mode);

}




//// 去鬼像函数
//void CA1Camera::RemoveGhosting(unsigned short* plBuffer, int XSize, int YSize, const CString& ghostRegionStr) {
//	// 解析鬼像区域字符串（如 "500,700;1000,1200"）
//	CString strCopy = ghostRegionStr;
//	int startPos = 0;
//
//	// 遍历每个鬼像区域
//	while (startPos < strCopy.GetLength()) {
//		CString region = strCopy.Tokenize(_T(";"), startPos);
//		if (region.IsEmpty()) break;
//
//		// 解析单个区域（如 "500,700"）
//		int commaPos = region.Find(_T(","));
//		if (commaPos != -1) {
//			int x_start = _ttoi(region.Left(commaPos));
//			int x_end = _ttoi(region.Mid(commaPos + 1));
//			int d = x_end - x_start;
//
//			// 从右往左修复，避免区域覆盖
//			for (int row = 0; row < YSize; ++row) {
//				for (int j = d - 1; j >= 0; --j) {
//					int left_idx = row * XSize + (x_start - d + j);
//					int right_idx = row * XSize + (x_end + j);
//					int target_idx = row * XSize + (x_start + j);
//
//					// 边界保护，确保修复区域在图像范围内
//					if (left_idx >= 0 && right_idx < XSize * YSize) {
//						plBuffer[target_idx] = static_cast<unsigned short>(
//							0.5 * plBuffer[left_idx] + 0.5 * plBuffer[right_idx]);
//					}
//				}
//			}
//		}
//	}
//}

template <typename T>
void CA1Camera::RemoveGhosting(T* plBuffer, int XSize, int YSize, const CString& ghostRegionStr)
{
	if (ghostRegionStr.IsEmpty()) {
		WriteToLog(_T("去鬼像处理跳过：ghostRegionStr 为空"));
		return;
	}
	else {
		WriteToLog(_T("进入鬼像处理"));
	}

	CString strCopy = ghostRegionStr;
	int startPos = 0;

	while (startPos < strCopy.GetLength()) {
		CString region = strCopy.Tokenize(_T(";"), startPos);
		if (region.IsEmpty()) continue;

		int commaPos = region.Find(_T(","));
		if (commaPos != -1) {
			int x_start = _ttoi(region.Left(commaPos));
			int x_end = _ttoi(region.Mid(commaPos + 1));
			int d = x_end - x_start;

			for (int row = 0; row < YSize; ++row) {
				for (int j = d - 1; j >= 0; --j) {
					int left_idx = row * XSize + (x_start - d + j);
					int right_idx = row * XSize + (x_end + j);
					int target_idx = row * XSize + (x_start + j);

					if (left_idx >= 0 && right_idx < XSize * YSize) {
						plBuffer[target_idx] = static_cast<T>(
							0.5 * plBuffer[left_idx] + 0.5 * plBuffer[right_idx]);
					}
				}
			}
		}
	}
}


