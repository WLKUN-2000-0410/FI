// INScan-LiteDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "INScan-Lite.h"
#include "INScan-LiteDlg.h"
#include "afxdialogex.h"
#include "SetNameDlg.h"
#include "..\file\TeeChart2011\axes.h"
#include "..\file\TeeChart2011\axis.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框 
class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()

int WriteToLog2(char* str)
{
	FILE* pfile;
	fopen_s(&pfile, FILE_PATH, "a+");   //FILE_PATH 是个宏, 是文件的位置,如#define FILE_PATH "D:\\FILog.txt"

	if (pfile == NULL)
	{
		return -1;
	}

	char szDateTime[20];
	SYSTEMTIME tm;
	GetLocalTime(&tm);
	sprintf(szDateTime, "%02d:%02d:%02d.%03d", tm.wHour, tm.wMinute, tm.wSecond, tm.wMilliseconds);
	fprintf_s(pfile, "%s:%s\n", szDateTime, str);

	fclose(pfile);

	return 0;
}
// CINScanLiteDlg 对话框
CINScanLiteDlg *pDlg=NULL;

UINT GetTemper(LPVOID lParam)
{
	int tempArray[2] = { 0, 0 };
	bool bFirst = true;// 是否是第一次读取温度

	while (!g_bClose) {
		g_tmpMutex.Lock();
		if (!pDlg)
		{
			g_tmpMutex.Unlock();
			break;
		}
		CString str = _T("N/A");

		// lcz new 23/05/12  先判断是不是1064
		if (pDlg->m_b1064)
		{
			if (!g_bGetTemp && !g_bUSBDiscon && !g_bOpenCCDSet)//  如果不是在采集或者设置CCD或已断开，则可读取温度
			{
				float real = 0, in = 0, out = 0;
				dfield_get_real_temperature(pDlg->_handle, &real);
				dfield_get_internal_temperature(pDlg->_handle, &in);
				dfield_get_external_temperature(pDlg->_handle, &out);
				g_iTemp = (int)real;
				str.Format("%d", g_iTemp);
				if (real == 0 && in == 0 && out == 0)
				{
					g_bUSBDiscon = true;
					str = TEXT("N/A");
				}
			}
			else
			{
				str.Format(_T("%d"), g_iTemp);
			}


		}
		else
		{

			//  lcz new 23/05/13  粘贴上来的
			if (sInfo.CConnect == 1 && sInfo.OminiType != 20)
			{
				int temp = 0;
				// lcz new 09/28  打开CCD高级设置时，不采集温度
				if (!g_bGetTemp && !g_bUSBDiscon && !g_bOpenCCDSet) //  点击测试，完成后才继续监测温度；
				//if (!g_bGetTemp && !g_bUSBDiscon ) //千万不要这样写
				{
					if (m_ZolixCCD.GetTemper(&temp))
					{
						
						// lcz new 05/28 解决读取的温度为0的bug
						if (bFirst)// 如果是第一次读取温度，数组两个数都存为温度
						{
							tempArray[0] = temp;
							tempArray[1] = temp;
							bFirst = false;
						}
						else
						{
							tempArray[0] = tempArray[1];
							tempArray[1] = temp;
							if (tempArray[1] == 0 && (tempArray[0] > 1 || tempArray[0] < -1))// 如果读取到的温度是0，但上次记录的温度值大于1或小于-1，则表示此次温度读取有误
								temp = tempArray[0];
						}
						g_iTemp = temp;
						sSInfo.Temp = g_iTemp;
						str.Format(_T("%4d "), temp);
						str = str + pDlg->LoadStringMeau(_T("ID_TEMP_S"));
						//  lcz new 09/27
						//  toucan CCD 断电或断开后，温度变为-184℃
						if (temp == -999) //  如果断开连接了,修改连接状态
						{
							if (m_bAutoRunning && sInfo.OminiType == 10)
							{
								g_bAndorTestingDiscon = true;
							}
							m_bAutoRunning = FALSE;//  停止采样
							m_bAutoRunningStatue = FALSE;
							//  将测试的按钮、主按钮都置灰
							pDlg->m_DlgTest.EnableBtn(FALSE);
							sInfo.CConnect = 0;
							sInfo.LConnect = 0;
							sInfo.LockConnect = 0;
							g_bUSBDiscon = true;
						}
					}
				}
				else
				{
					str.Format(_T("%4d "), g_iTemp);
					str = str + pDlg->LoadStringMeau(_T("ID_TEMP_S"));
				}
			}
			if (str == TEXT("N/A") && sInfo.CConnect == 1 && sInfo.OminiType != 20)
				g_bUSBDiscon = true;

		}
		pDlg->strTemp = pDlg->LoadStringMeau(_T("ID_TEMP")) + str;
		g_tmpMutex.Unlock();
		Sleep(1000);
		
	}
	return 0;
}


CINScanLiteDlg::CINScanLiteDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CINScanLiteDlg::IDD, pParent)
	, m_strTempCCD(_T(""))
{
#ifdef Release_LO
	m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON2);
#else
	m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON1);
#endif
	//m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON1);
	// lcz new 05/12
	pCutLineDlg = new CCutOutLineDlg(this);
	pFitSmoothDlg = new CFitSmoothDlg(this);
	pSmoothDlg = new CSoomthDlg(this);
	pDenoiseDlg = new CDenoiseDlg(this);
	m_bMove = false;
}
CINScanLiteDlg::~CINScanLiteDlg(){

	// 结束时停止查询温度
	g_bClose = true;
	if (!m_getTemperThread)
	{
		// 终止线程
		if (m_getTemperThread != nullptr)
		{
			m_getTemperThread->ExitInstance();
		}
	}

	// lcz new 05/12
	if (pCutLineDlg!=NULL)delete pCutLineDlg;
	if(pFitSmoothDlg!=NULL)delete pFitSmoothDlg;
	if(pSmoothDlg!=NULL)delete pSmoothDlg;
	if(pDenoiseDlg!=NULL)delete pDenoiseDlg;
	sInfo.CConnect=0;

	if(g_pBuffer)
	{
		delete[] g_pBuffer;
		g_pBuffer = NULL;
	}

	WriteDeviceInfo();

	// 释放摄像头资源
	cap.release();// lcz new 02/06

	g_tmpMutex.Lock();
	pDlg = NULL;
	g_tmpMutex.Unlock();
}
void CINScanLiteDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_BUTTON_TEST, m_ButtonTest);
	DDX_Control(pDX, IDC_BUTTON_FILE, m_ButtonFile);
	DDX_Control(pDX, IDC_BUTTON_DATA, m_ButtonData);
	DDX_Control(pDX, IDC_BUTTON_ADV, m_ButtonAdv);
	DDX_Control(pDX, IDC_ZLANGUAGECTRL1, m_ZLanguage);
	DDX_Control(pDX, IDC_ZOLIXCURVEPROCESCTRL1, m_polyfit);
	DDX_Control(pDX, IDC_STATIC_CCD, m_static_ccd);
	DDX_Text(pDX, IDC_STATIC_CCD, m_strTempCCD);
	DDX_Control(pDX, IDC_STATIC_SET, m_groupBox);
	DDX_Control(pDX, IDC_STATIC_GREEN, m_bitGreen);
	DDX_Control(pDX, IDC_STATIC_GRAY, m_bitGray);
	DDX_Control(pDX, IDC_STATIC_XY, m_staticXY);
}

BEGIN_MESSAGE_MAP(CINScanLiteDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CTLCOLOR()
	ON_MESSAGE(WM_MY_STATUSBAR,OnDlgLogin)
	ON_BN_CLICKED(IDC_BUTTON_TEST, &CINScanLiteDlg::OnBnClickedButtonTest)
	ON_BN_CLICKED(IDC_BUTTON_FILE, &CINScanLiteDlg::OnBnClickedButtonFile)
	ON_BN_CLICKED(IDC_BUTTON_DATA, &CINScanLiteDlg::OnBnClickedButtonData)
	ON_BN_CLICKED(IDC_BUTTON_ADV, &CINScanLiteDlg::OnBnClickedButtonAdv)
	ON_WM_TIMER()
	ON_WM_NCLBUTTONDOWN()
	ON_COMMAND(ID_UPDATE_NAME, &CINScanLiteDlg::OnUpdateName)
	ON_COMMAND(ID_DEL_ALL, &CINScanLiteDlg::OnDelAll)
	ON_COMMAND(ID_AUTO_ZOOM, &CINScanLiteDlg::OnAutoZoom)
	ON_WM_DESTROY()
	ON_MESSAGE(WM_UPDATENAME, OnMsgUpdateLineName)
	ON_MESSAGE(WM_SHOWLINE, OnMsgShowLine)
END_MESSAGE_MAP()


// CINScanLiteDlg 消息处理程序

BOOL CINScanLiteDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();




	// 将“关于...”菜单项添加到系统菜单中。
	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	//InitializeCriticalSection( & g_cs_temp); 

	int iWidth = GetSystemMetrics(SM_CXSCREEN); //获取屏幕水平分辨率
	int iHeight = GetSystemMetrics(SM_CYSCREEN); //获取屏幕垂直分辨率
	//  lcz new 23/05/16  注释掉
	//if(iWidth > 1280 && iHeight > 800)
	//	m_bMove = true;
	//CRect rect(0,0,1280,800);
	////CRect rect(0,0,1460,1028);
	//::SetWindowPos(this->m_hWnd,HWND_TOP,0,0,rect.Width(),rect.Height(),SWP_NOZORDER|SWP_NOMOVE);


	// 任务栏高度
	HWND hwnd = ::FindWindow("Shell_TrayWnd", "");
	RECT rect;
	::GetWindowRect(hwnd, &rect);
	int dutyHeight = 40;

	//  lcz new 23/05/16  获得屏幕大小（若屏幕上有任务栏，则减去任务栏的大小，获取工作区大小）
	CRect screen_rt;
	SystemParametersInfo(SPI_GETWORKAREA, 0, (PVOID)&screen_rt, 0);
	// ::SetWindowPos(this->m_hWnd, HWND_TOP, 0, 0, screen_rt.Width(), iHeight - dutyHeight, SWP_NOZORDER);//   直接全屏
	::MoveWindow(this->m_hWnd, 0, 0, screen_rt.Width(), iHeight - dutyHeight, TRUE);
	// 立即刷新
	Invalidate();
	UpdateWindow();

	// 窗口全局指针
	pDlg=this;

	pDlgLogin =new CDlgLogin(this);					// 打开
	pDlgLogin->Create(IDD_LOGIN,this);
	pDlgLogin->ShowWindow(SW_SHOW);
	pDlgLogin->CenterWindow();
	SendMessage(WM_MY_STATUSBAR,0,0);
	

	sInfo.nLanguage=0;
	sInfo.AdvShowID=3;
	m_FontChinese.CreatePointFont(250,strFontChinese);
	m_FontEnglish.CreatePointFont(250,strFontEngish);
	SetButtonState(&m_ButtonTest);
	SetButtonState(&m_ButtonFile);
	SetButtonState(&m_ButtonData);
	SetButtonState(&m_ButtonAdv);
    InitDeviceInfo();
	
	InitDialog();
	InitDialogShow(0);
	InitLan();	
	SetControlFont();

	//m_DlgShow.SetLawerPower(10, false);

	m_DlgSet.ConnectAll();
	//  100号定时器用于温度显示N/A

	SetTimer(100,1000,NULL);

	//  101 号定时器用于检测 CCD高级设置 页面是否打开
	SetTimer(101, 10,NULL);

	//  10号定时器用于在初始化时提示谱仪和激光器有没有连接
	SetTimer(10,1000,NULL);

	//  8号定时器用于检测是否异常断开USB连接线  如果g_bUSBDiscon为真，则断开连接，并弹出提示框
	SetTimer(8, 6000, NULL);
	
	// 18号定时器用于重新连接，每隔15秒  如果g_bUSBDiscon为真，先连激光器和电机
	SetTimer(18, 10000, NULL);

	// lcz new 08/10 取消非测试状态下，采集CCD数据，改为每隔五秒检查激光器串口是不是还在
	// lcz new 05/19 // 28号定时器用于检测非测试状态下，mityCCD是否断开连接
	g_pBuffer = new double[3000];
	memset(g_pBuffer,0,sizeof(double)*3000);
	// lcz new 09/27  修改了，仅mity CCD会外部采集以判断是否断开, toucan的不会   28号定时器用于检测非测试状态下，mityCCD是否断开连接
	if(	sInfo.OminiType == 15 )
		SetTimer(28, 5000, NULL);

	//Sleep(1000);
	SendMessage(WM_MY_STATUSBAR,0,0);

	pDlgWait =new CDlgWait(this);					// 打开
	pDlgWait->Create(IDD_WAIT,this);
	pDlgWait->ShowWindow(SW_HIDE);
	pDlgWait->CenterWindow();
	
	m_getTemperThread = AfxBeginThread(GetTemper, (LPVOID)this);
	
	

	sOffsetFileInfo.dFscDataX=new double[3000];
	sOffsetFileInfo.dFscDataY=new double[3000];

	LoadFileData(sInfo.strPathOffset,
		&sOffsetFileInfo.FscDataNum,
		sOffsetFileInfo.dFscDataX,
		sOffsetFileInfo.dFscDataY);

	ShowWindow(SW_SHOWMAXIMIZED);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

DWORD WINAPI f2(LPVOID lpParameter)
{
	//  lcz new 07/19  温度小于-9才开始进行采集判断是否断开；若温度降下来之前就进行数据采集，则数据可能会异常，在说明书里进行添加（需要重连CCD）
	if(!g_bOpenCCDSet  && g_bSingleCollectFinish && !m_bAutoRunning  && g_iTemp <= -7 ) 
	{//  设置是非测试状态下采集
		//m_ZolixCCD.setStarted(false);
		m_ZolixCCD.SetExpTime(0.02);
		//  根据像素点数读取数据，存放在buffer里
		//double *buffer=new double[3000]; 
		//memset(buffer,0,sizeof(double)*3000);
		m_ZolixCCD.DataAcquisition(g_pBuffer, sInfo.sDInfo.nGXPix);
		memset(g_pBuffer,0,sizeof(double)*3000);// 数据清零

		m_ZolixCCD.SetExpTime(0.1);
		m_ZolixCCD.DataAcquisition(g_pBuffer, sInfo.sDInfo.nGXPix);
	
		int j = 0, k = 0;  //  && ( !g_bOpenCamera ) 
		double firstNum = g_pBuffer[0];
		for(int i = 0; i < sInfo.sDInfo.nGXPix; i++)
		{
			//if(g_pBuffer[i] == 0  )
			//{
			//	j++;
			//	//g_bUSBDiscon = true;
			//	//return 0;
			//}
			if( (g_pBuffer[i] > 70000 || g_pBuffer[i] < -70000)  )
			{
				//AfxMessageBox(TEXT("非测试情况下（未开白光），数据超出±70000"));
			

				g_bUSBDiscon = true;
			}
			else if(g_pBuffer[i] == firstNum && firstNum != 65535 && firstNum != 0  )
			{
				k++;
			}
		}
		if( j > sInfo.sDInfo.nGXPix * 0.5 || k > sInfo.sDInfo.nGXPix * 0.5)
		{
			//AfxMessageBox(TEXT("非测试情况下（未开白光），很多数保持不变"));
			

			g_bUSBDiscon = true;
			//CString str; str.Format("%d", g_bOpenCCDSet);//  异常数据，确实是在相机关闭状态下的
			//AfxMessageBox(str);
		}
	}
	//if(buffer)
	//{
	//	delete [] buffer;
	//	buffer = NULL;
	//}
	return 0;
}

//  定时器任务
void CINScanLiteDlg::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent==100)
	{
		//m_static_ccd.SetWindowTextA(strTemp);
		//GetDlgItem(IDC_STATIC_CCD)->SetWindowText(strTemp);
		m_strTempCCD=strTemp;
		UpdateData(FALSE);
		if (pDlg->m_b1064 == false)
		{
			//  更新在CCD设置里是否开始采集影像
			g_bOpenCCDSet = m_ZolixCCD.getStartOrNot();
		}		
		Sleep(200);
	}

	//  更新 CCD高级设置 页面是否打开
	if (nIDEvent==101)
	{
		if (pDlg->m_b1064==false)
		{
			g_bOpenCCDSet = m_ZolixCCD.getStartOrNot();
			Sleep(5);
		}
		
	}
	//  10号定时器在打开软件时检测设备连接状态
	if (nIDEvent==10)
	{
		KillTimer(10);
		CString str;
		if (sInfo.CConnect==0)
		{
			str=LoadStringMeau(_T("ID_OMINI_CONNECT"));
			MessageBox(str);
			//g_bUSBDiscon = false;
		} 
		if (sInfo.LConnect==0)
		{
			str=LoadStringMeau(_T("ID_LASER_CONNECT"));
			MessageBox(str);
		} 
		 // lcz new 04/25  取消“相机未连接”的提示   只存在于4/25EN版本
		//if (sInfo.LockConnect==0)
		//2023
		//sInfo.LockConnect=1;
		if (sInfo.LockConnect == 0 && !m_DlgSet.is_new_board())
		{
			str=LoadStringMeau(_T("ID_CAMERA_CONNECT"));
			m_DlgTest.m_DlgTestCheckJ.GetDlgItem(IDC_BUTTON_LED)->EnableWindow(false);
			MessageBox(str);
		}
	}
	//  8号定时器检测是否断开连接，如果断开连接，则弹出提示   3秒
	if (nIDEvent==8)
	{
	
		if(g_bUSBDiscon)
		{
			m_bAutoRunning = FALSE;
			//  lcz new 08/16  第一次数据异常后，先检测能不能找到激光器串口，如果找不到，就认为断开了；如果找得到，先断开重连CCD，再采集数据
			////  如果是第一次数据异常，可能是降温后CCD采集的数据变为0，重连CCD
			if( g_bFirstlyDataException)
			{
				//AfxMessageBox(TEXT("第一次断开"));
				g_bFirstlyDataException = false;
				//  第一次断开后，检测激光器串口还在不在，如果在的话，断开重连CCD，然后再测试，如果不在，则提示“异常断开
				bool disConnect = true;
				pDlg->m_DlgSet.GetComList_Reg();
				for(map<int, CString>::iterator it = pDlg->m_DlgSet.m_vComInfo.begin(); it != pDlg->m_DlgSet.m_vComInfo.end(); it++)
				{
					if(it->second.Find(TEXT("Serial")) != -1 )//  如果找到了激光器的COM口，则将sSInfo.nCOM设置为该COM口，并连接激光器
					{
						if(it->first == 1)// 如果是com1，则跳过
							continue;
						else if(it->first == sSInfo.nCOM)
						{
							disConnect = false;
							break;
						}
					}
				}
				if (disConnect) {//  如果真找不到激光器串口，则真的断连

					g_bUSBDiscon = true;
				
				}
				else// 如果还找得到激光器串口，则断开重连CCD
				{
					m_ZolixCCD.DisConnect();
					m_ZolixCCD.Connect();
					//pDlg->m_DlgSet.OnBnClickedButtonLdis();
					//pDlg->m_DlgSet.OnBnClickedButtonLcon();
					HANDLE hThread1;
					hThread1 = CreateThread(NULL, 0, f2, 0, 0, NULL);
					int signal = WaitForSingleObject(hThread1, 2000);
					//  如果2秒没执行完数据采集，则关闭线程和定时器
					if (signal != WAIT_OBJECT_0)
					{
						//cout << "time out" << endl;
						TerminateProcess(hThread1, -1);
						//AfxMessageBox(TEXT("重连CCD后还是采集超时"));
						//CString str = m_ZolixCCD.GetDevName(); //  测试谱仪是否断开，结果谱仪未断开
						//AfxMessageBox(str);

					

						g_bUSBDiscon = true;
						//  下面这个延迟，得加上，当供电断开或USB断开，g_bUSBDiscon为真时，添加一个阻塞，跳转到8号定时器，给出提示框
						Sleep(100);
					}
					else
					{
						g_bUSBDiscon = false;
						//AfxMessageBox(TEXT("重连CCD后采集正常"));
					}

					TerminateProcess(hThread1, -1);
					CloseHandle(hThread1);

				}


				//HANDLE hThread1;
				//hThread1 = CreateThread(NULL, 0, f2, 0, 0, NULL);
				//int signal = WaitForSingleObject(hThread1, 2000);
				////  如果1秒没执行完数据采集，则认为真的断开了连接，否则认为
				//if (signal != WAIT_OBJECT_0)
				//{
				//	TerminateProcess(hThread1, -1);
				//}
				//else// 如果还能正常采集，说明CCD没有断开连接，这时需要断开重连次
				//{
				//	m_ZolixCCD.DisConnect();
				//	m_ZolixCCD.Connect();
				//	g_bUSBDiscon = false;
				//	//AfxMessageBox(TEXT("CCD 重连"));
				//	Sleep(100);
				//	return;
				//}
			}
			//KillTimer(8);
			//如果确实断连了
			if(g_bUSBDiscon)
			{
				KillTimer(8);
				CString str;
				str=LoadStringMeau(_T("ID_USBDiscon"));
				MessageBox(str);
				Sleep(2000);
				//  更改谱仪连接按钮
				CINScanLiteDlg * pDlg = (CINScanLiteDlg *)AfxGetApp()->GetMainWnd();
				// lcz new 05/27 需要断开激光器和电机串口连接
				pDlg->m_DlgSet.OnBnClickedButtonLdis();
				pDlg->m_DlgSet.OnBnClickedButtonLdis2();
				//sInfo.LConnect = pDlg->m_DlgSet.m_Serial.OpenLaser(sSInfo.nCOM);
				if( !g_bAndorTestingDiscon ) // 若AndorCCD测试时断开USB线，则不进行DisconnectCCD；或者mityCCD断电，也不断开CCD连接
				{
					//pDlg->m_DlgSet.OnBnClickedButtonCdis();
					//  如果激光器还能连上，说明不是USB线断了，而是供电断了，供电断了的话就不进行断连操作
					if(sInfo.LConnect)
					  m_ZolixCCD.DisConnect();
					sInfo.CConnect=0;
					pDlg->m_DlgSet.OnBnState();
				}
				pDlg->m_DlgSet.OnBnState();
				pDlg->SetBtnEnable(TRUE);// 使主按钮可用
			}
			
		}
	}

		//  lcz new 08/10 取消28号定时器间隔
	//  lcz new  05/19   28号定时器用于检测非测试状态下，mityCCD和OTO CCD是否断开连接
	if (nIDEvent == 28)
	{ 
		// lcz new 08/10  改为每隔五秒，检查激光器串口还能不能找到
		//if(sInfo.CConnect == 1 && sInfo.LConnect == 1)
		//{
		//	bool disConnect = true;
		//	pDlg->m_DlgSet.GetComList_Reg();
		//	for(map<int, CString>::iterator it = pDlg->m_DlgSet.m_vComInfo.begin(); it != pDlg->m_DlgSet.m_vComInfo.end(); it++)
		//	{
		//		if(it->second.Find(TEXT("Serial")) != -1 )//  如果找到了激光器的COM口，则将sSInfo.nCOM设置为该COM口，并连接激光器
		//		{
		//			if(it->first == 1)// 如果是com1，则跳过
		//				continue;
		//			//sSInfo.nCOM = it->first;
		//			////if(sSInfo.nCOM == it->first)//  如果激光器串口号正确
		//			////{
		//			//	sInfo.LConnect = m_Serial.OpenLaser(sSInfo.nCOM);//  1 表示成功连接， 0  表示连接失败
		//			//}
		//			//elsed
		//			//	sInfo.LConnect = 0;
		//			else if(it->first == sSInfo.nCOM)
		//			{
		//				disConnect = false;
		//				break;
		//			}
		//		}
		//	}
		//	if(disConnect)
		//			g_bUSBDiscon = true;

		//}

		//Sleep(100);
		//如果不在测试中；谱仪已连接；谱仪类型不是Andor；未打开CCD设置; 未打开相机
		if(m_bAutoRunning == FALSE && sInfo.CConnect == 1 && sInfo.OminiType != 10  && (!g_bOpenCCDSet) &&(!g_bOpenCamera)  )
		{
			HANDLE hThread1;
			hThread1 = CreateThread(NULL, 0, f2, 0, 0, NULL);
			int signal = WaitForSingleObject(hThread1, 2000);
			//  如果2秒没执行完数据采集，则关闭线程和定时器
			if (signal != WAIT_OBJECT_0)
			{
				//cout << "time out" << endl;
				//KillTimer(28);
				TerminateProcess(hThread1, -1);
				//AfxMessageBox(TEXT("谱仪采集数据超时"));
				//CString str = m_ZolixCCD.GetDevName(); //  测试谱仪是否断开，结果谱仪未断开
				//AfxMessageBox(str);

				

				g_bUSBDiscon = true;
				//  下面这个延迟，得加上，当供电断开或USB断开，g_bUSBDiscon为真时，添加一个阻塞，跳转到8号定时器，给出提示框
				Sleep(100);
				
			}

			TerminateProcess(hThread1, -1);
			CloseHandle(hThread1);

			// 如果是mityCCD 并且连接状态，非测试状态，断开重连
			//m_ZolixCCD.DisConnect();
			//Sleep(500);
			//if(m_ZolixCCD.Connect())//  如果重连失败，给出USB断开提示
			//{
			//	sInfo.CConnect == 0;
			//	m_ZolixCCD.DisConnect();
			//	g_bUSBDiscon = true;
			//	AfxMessageBox(TEXT("3"));
			//}
		}
	}

	//如果连接断开，则尝试连接激光器，若激光器能重连，说明线已接上
	if (nIDEvent == 18)
	{
		if(g_bUSBDiscon)
		{
			//  先重连相机
			pDlg->m_DlgSet.GetComList_Reg();
			int mc = pDlg->m_DlgSet.m_vComInfo.size();
			for(map<int, CString>::iterator it = pDlg->m_DlgSet.m_vComInfo.begin(); it != pDlg->m_DlgSet.m_vComInfo.end(); it++)
			{
				//if((it->second == "VCP0" || it->second == "VCP1" || it->second == "VCP2" || it->second == "VCP3") && sInfo.LockConnect == 0)//  如果找到了相机的COM口，则将sSInfo.nCOMLED设置为该COM口，并连接相机
				if((it->second.Find(TEXT("VCP")) != -1) && sInfo.LockConnect == 0)//  如果找到了相机的COM口，则将sSInfo.nCOMLED设置为该COM口，并连接相机
				{
					sSInfo.nCOMLED =  it->first;
					//if(sSInfo.nCOMLED == it->first)//  如果电机串口号正确，那么就正常连接
					//{
						sInfo.LockConnect = pDlg->m_DlgSet.m_SerialLed.OpenLed(sSInfo.nCOMLED);
						if (sInfo.LockConnect==1)//  如果相机连接成功，则对它进行初始化
						{
							bool a = pDlg->m_DlgSet.m_SerialLed.SetLed(sSInfo.nCOMLED,_T("PX,1"));
							if(a == false)// 如果电脑接了其他的串口线，而且是第一个接，名为vcp0
								sInfo.LockConnect = 0;
							bool b = pDlg->m_DlgSet.m_SerialLed.SetLed(sSInfo.nCOMLED,_T("NX,1"));
							if(b == false)
							{
								pDlg->m_DlgSet.m_SerialLed.CloseLaser(sSInfo.nCOMLED);
								sInfo.LockConnect = 0;
							}
							bool c = pDlg->m_DlgSet.m_SerialLed.SetLed(sSInfo.nCOMLED,_T("HX"));// lcz new 05/16 c的值不能作为能否连接的标志
							//if(c == false)
							//{
							//	m_SerialLed.CloseLaser(sSInfo.nCOMLED);
							//	sInfo.LockConnect = 0;
							//}
						}
				}

				//  连接激光器
				//else if((it->second == "Serial0" || it->second == "Serial1" || it->second == "Serial2" || it->second == "Serial3") && sInfo.LConnect == 0)//  如果找到了激光器的COM口，则将sSInfo.nCOM设置为该COM口，并连接激光器
				else if((it->second.Find(TEXT("Serial")) != -1 ) && sInfo.LConnect == 0)//  如果找到了激光器的COM口，则将sSInfo.nCOM设置为该COM口，并连接激光器
				{
					if(it->first == 1)// 如果是com1，则跳过
						continue;
					sSInfo.nCOM = it->first;
					//if(sSInfo.nCOM == it->first)//  如果激光器串口号正确
					//{
						sInfo.LConnect = pDlg->m_DlgSet.m_Serial.OpenLaser(sSInfo.nCOM);//  1 表示成功连接， 0  表示连接失败
					//}
					//else
					//	sInfo.LConnect = 0;
				}
			}

			//  如果都重连失败，说明线没接上，继续重连
			if (sInfo.LConnect==0 && sInfo.LockConnect == 0)
			{
				g_bUSBDiscon = true;
			}
			else
			{
				g_bGetTemp = false;
				g_bUSBDiscon = false;
			}
			//if(sInfo.CConnect == 0)
			//	pDlg->m_DlgSet.OnBnClickedButtonCcon();
			//// lcz new 05/27  重新初始化相机
			pDlg->m_DlgSet.InitCamera();

			pDlg->m_DlgSet.OnBnState();
			pDlg->m_DlgSet.SetData();
			pDlg->WriteDeviceInfo();
			//g_bUSBDiscon = false;

		}
	}



	CDialogEx::OnTimer(nIDEvent);
}

void CINScanLiteDlg::InitDialog()
{

	m_DlgAdv.Create(IDD_DLG_ADV,this);
	m_DlgFile.Create(IDD_DLG_FILE,this);
	m_DlgTest.Create(IDD_DLG_TEST,this);
	m_DlgShow.Create(IDD_DLG_SHOW,this);
	m_DlgAbout.Create(IDD_ABOUT,this);
	m_DlgSet.Create(IDD_SET,this);

	// 本来准备把m_DlgSet.Create(IDD_SET,this);放在初始化最前面；然后在CDlgTestCheckJ的初始化函数里进行设置，结果总是报错，只能改放在此
	//  lcz new 12/06 如果找不到外部相机，由原先“打开相机”不可用改为可用，生成临时版本12-6-T
	// m_DlgTest.m_DlgTestCheckJ.GetDlgItem(IDC_BUTTON_LED)->EnableWindow(g_bFindCamera);//  lcz new 03/20  不管找不找得到USB，都让“打开相机”按钮可用，因为后续要尝试500E的相机，名字不叫usb camera 了  lcz new 04/23  根据找不找得到相机，打不打得开来显示该按钮。
	//m_DlgTest.m_DlgTestCheckJ.GetDlgItem(IDC_BUTTON_LED)->EnableWindow(false);// lcz new 04/25 设置打开相机按钮不可用

	m_DlgLanguage.Create(IDD_LANGEUAGE,this);
	CRect rect,rectShow;//  lcz new 23/05/16   注释掉
	//GetDlgItem(IDC_STATIC_SET)->GetWindowRect(rect);
	//GetDlgItem(IDC_STATIC_SET)->ShowWindow(SW_HIDE);
	//GetDlgItem(IDC_STATIC_SHOW)->GetWindowRect(rectShow);
	//GetDlgItem(IDC_STATIC_SHOW)->ShowWindow(SW_HIDE);

	//  lcz new 23/05/16    调整左侧栏和绘图区的大小
	CRect fullWnd;
	GetWindowRect(fullWnd);
	ScreenToClient(&fullWnd);
	//  获取上方图片的大小。四个主按钮高度和其一致
	CRect rectP;
	GetDlgItem(IDC_STATIC_P)->GetClientRect(&rectP);
	//   左侧栏
	GetDlgItem(IDC_STATIC_SET)->GetWindowRect(rect);
	ScreenToClient(&rect);
	int menuHeight = GetSystemMetrics(SM_CYMENU);//  获取菜单栏的高度
	//  重新设置了左侧栏的位置和大小
	::SetWindowPos(m_groupBox.m_hWnd, HWND_TOP, rect.left + 5, menuHeight + rectP.Height() + 8, rect.Width(), fullWnd.Height() - rectP.Height() - menuHeight - 50, SWP_SHOWWINDOW | SWP_FRAMECHANGED);
	GetDlgItem(IDC_STATIC_SET)->GetWindowRect(rect);
	ScreenToClient(&rect);
	//  绘图区
	GetDlgItem(IDC_STATIC_SHOW)->GetWindowRect(rectShow);
	ScreenToClient(&rectShow);
	::SetWindowPos(m_DlgShow.m_hWnd, HWND_TOP, rectShow.left, rectShow.top + 10, fullWnd.Width() - rectP.Width() - 35, fullWnd.Height() - rectP.Height() - menuHeight - 65, SWP_SHOWWINDOW | SWP_FRAMECHANGED);//  重新设置了左侧栏的位置和大小
	GetDlgItem(IDC_STATIC_SHOW)->GetWindowRect(rectShow);
	ScreenToClient(&rectShow);
	GetDlgItem(IDC_STATIC_SET)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC_SHOW)->ShowWindow(SW_HIDE);
	ScreenToClient(&rect);
	ScreenToClient(&rectShow);
	//  lcz new 23/05/16  设置四个主按钮的长度
	CRect rectMainButton;
	GetDlgItem(IDC_BUTTON_TEST)->GetWindowRect(rectMainButton);
	int mainButtonWidth;
	mainButtonWidth = (fullWnd.Width() - rectP.Width() - 20) / 4;//  每个按钮的宽度
	::SetWindowPos(m_ButtonTest.m_hWnd, HWND_TOP, rectP.Width() + 5, rectP.top, mainButtonWidth, rectMainButton.Height(), SWP_SHOWWINDOW | SWP_FRAMECHANGED);
	::SetWindowPos(m_ButtonFile.m_hWnd, HWND_TOP, rectP.Width() + 5 + mainButtonWidth, rectP.top, mainButtonWidth, rectMainButton.Height(), SWP_SHOWWINDOW | SWP_FRAMECHANGED);
	::SetWindowPos(m_ButtonData.m_hWnd, HWND_TOP, rectP.Width() + 5 + mainButtonWidth*2, rectP.top, mainButtonWidth, rectMainButton.Height(), SWP_SHOWWINDOW | SWP_FRAMECHANGED);
	::SetWindowPos(m_ButtonAdv.m_hWnd, HWND_TOP, rectP.Width() + 5 + mainButtonWidth*3, rectP.top, mainButtonWidth - 5, rectMainButton.Height(), SWP_SHOWWINDOW | SWP_FRAMECHANGED);
	//  lcz new 23/05/16  重新调整大小
	::SetWindowPos(m_DlgTest.m_hWnd, HWND_TOP, rect.left, rect.top + 8, rect.Width(), rect.Height() - 10, SWP_SHOWWINDOW | SWP_FRAMECHANGED);
	::SetWindowPos(m_DlgFile.m_hWnd, HWND_TOP, rect.left, rect.top + 8, rect.Width(), rect.Height() - 10, SWP_SHOWWINDOW | SWP_FRAMECHANGED);
	::SetWindowPos(m_DlgAdv.m_hWnd, HWND_TOP, rect.left, rect.top + 8, rect.Width(), rect.Height() - 10, SWP_SHOWWINDOW | SWP_FRAMECHANGED);
	//::SetWindowPos(m_DlgShow.m_hWnd,HWND_TOP,rectShow.left,rectShow.top+8,rectShow.Width(),rectShow.Height()-10,SWP_SHOWWINDOW|SWP_FRAMECHANGED);
	//  lcz new 23/05/16  设置 关于、设置和语言 框的位置和大小
	::SetWindowPos(m_DlgAbout.m_hWnd, HWND_TOP, rectP.Width() + 8, rectP.Height() + 8, fullWnd.Width() - rectP.Width() - 35, fullWnd.Height() - rectP.Height() - menuHeight - 65, SWP_SHOWWINDOW | SWP_FRAMECHANGED);
	::SetWindowPos(m_DlgSet.m_hWnd, HWND_TOP, rectP.Width() + 8,rectP.Height() + 8, fullWnd.Width() - rectP.Width() - 35, fullWnd.Height() - rectP.Height() - menuHeight - 65, SWP_SHOWWINDOW | SWP_FRAMECHANGED);
	::SetWindowPos(m_DlgLanguage.m_hWnd, HWND_TOP, rectP.Width() + 8, rectP.Height() + 8, fullWnd.Width() - rectP.Width() - 35, fullWnd.Height() - rectP.Height() - menuHeight - 65, SWP_SHOWWINDOW | SWP_FRAMECHANGED);
	i_Set_About_Page_Width = fullWnd.Width() - rectP.Width() - 35;
	i_Set_About_Page_Height = fullWnd.Height() - rectP.Height() - menuHeight - 65;
	m_DlgSet.resize();
	m_DlgAbout.resize();

	//  lcz new 23/05/16  设置 左下方 仪器状态图（灰色或绿色标）的位置   和  温度显示框的位置   和 XY坐标的位置
	CRect rectBitMap, rectCCDTemp, rectXY;
	GetDlgItem(IDC_STATIC_GREEN)->GetWindowRect(rectBitMap);
	::SetWindowPos(m_bitGreen.m_hWnd, HWND_TOP, rect.left + 25, fullWnd.bottom - rectBitMap.Height() - 10, rectBitMap.Width(), rectBitMap.Height(), SWP_SHOWWINDOW | SWP_FRAMECHANGED);
	::SetWindowPos(m_bitGray.m_hWnd, HWND_TOP, rect.left + 25, fullWnd.bottom - rectBitMap.Height() - 10, rectBitMap.Width(), rectBitMap.Height(), SWP_SHOWWINDOW | SWP_FRAMECHANGED);
	GetDlgItem(IDC_STATIC_CCD)->GetWindowRect(rectCCDTemp);
	::SetWindowPos(m_static_ccd.m_hWnd, HWND_TOP, rect.left + 25 + rectBitMap.Width() + 15, fullWnd.bottom - rectBitMap.Height() - 10, rectCCDTemp.Width(), rectCCDTemp.Height(), SWP_SHOWWINDOW | SWP_FRAMECHANGED);
	GetDlgItem(IDC_STATIC_XY)->GetWindowRect(rectXY);
	::SetWindowPos(m_staticXY.m_hWnd, HWND_TOP, (fullWnd.Width() - rectXY.Width())/2, fullWnd.bottom - rectBitMap.Height() - 10, rectXY.Width(), rectXY.Height(), SWP_SHOWWINDOW | SWP_FRAMECHANGED);

	//  lcz new 23/05/16  注释掉
	//::SetWindowPos(m_DlgTest.m_hWnd, HWND_TOP, rect.left, rect.top + 8, rect.Width(), rect.Height() - 10, SWP_SHOWWINDOW | SWP_FRAMECHANGED);
	//::SetWindowPos(m_DlgFile.m_hWnd, HWND_TOP, rect.left, rect.top + 8, rect.Width(), rect.Height() - 10, SWP_SHOWWINDOW | SWP_FRAMECHANGED);
	//::SetWindowPos(m_DlgAdv.m_hWnd, HWND_TOP, rect.left, rect.top + 8, rect.Width(), rect.Height() - 10, SWP_SHOWWINDOW | SWP_FRAMECHANGED);
	//::SetWindowPos(m_DlgShow.m_hWnd, HWND_TOP, rectShow.left, rectShow.top + 8, rectShow.Width(), rectShow.Height() - 10, SWP_SHOWWINDOW | SWP_FRAMECHANGED);
	//::SetWindowPos(m_DlgAbout.m_hWnd, HWND_TOP, rectShow.left, rectShow.top + 8, rectShow.Width(), rectShow.Height() - 10, SWP_SHOWWINDOW | SWP_FRAMECHANGED);
	//::SetWindowPos(m_DlgSet.m_hWnd, HWND_TOP, rectShow.left, rectShow.top + 8, rectShow.Width(), rectShow.Height() - 10, SWP_SHOWWINDOW | SWP_FRAMECHANGED);
	//::SetWindowPos(m_DlgLanguage.m_hWnd, HWND_TOP, rectShow.left, rectShow.top + 8, rectShow.Width(), rectShow.Height() - 10, SWP_SHOWWINDOW | SWP_FRAMECHANGED);


	m_DlgTest.ModifyStyle(0,WS_CLIPSIBLINGS);
	m_DlgTest.SetParent(this);
	m_DlgAdv.ModifyStyle(0,WS_CLIPSIBLINGS);
	m_DlgAdv.SetParent(this);
	m_DlgFile.ModifyStyle(0,WS_CLIPSIBLINGS);
	m_DlgFile.SetParent(this);
	m_DlgShow.ModifyStyle(0,WS_CLIPSIBLINGS);
	m_DlgShow.SetParent(this);
	m_DlgAbout.ModifyStyle(0,WS_CLIPSIBLINGS);
	m_DlgAbout.SetParent(this);
	m_DlgSet.ModifyStyle(0,WS_CLIPSIBLINGS);
	m_DlgSet.SetParent(this);
	m_DlgLanguage.ModifyStyle(0,WS_CLIPSIBLINGS);
	m_DlgLanguage.SetParent(this);

}

void CINScanLiteDlg::InitDialogShow(int nID)
{
	m_DlgAdv.ShowWindow(SW_HIDE);
	m_DlgFile.ShowWindow(SW_HIDE);
	m_DlgTest.ShowWindow(SW_HIDE);
	m_DlgShow.ShowWindow(SW_SHOW);
	m_DlgLanguage.ShowWindow(SW_HIDE);
	m_DlgSet.ShowWindow(SW_HIDE);
	m_DlgAbout.ShowWindow(SW_HIDE);
	switch(nID)
	{
	  case 0:
         m_DlgTest.ShowWindow(SW_SHOW);
		 break;
	  case 1:
		 m_DlgFile.ShowWindow(SW_SHOW);
		 break;
	  case 2:
		 m_DlgFile.ShowWindow(SW_SHOW);
		 break;
	  case 3:
		 m_DlgAdv.ShowWindow(SW_SHOW);
		 SetAdvShow(sInfo.AdvShowID);
	  case 4:
		 break;
	}
	//m_DlgShow.ShowWindow(SW_HIDE);
}
void CINScanLiteDlg::SetAdvShow(int nID)
{
	m_DlgLanguage.ShowWindow(SW_HIDE);
	m_DlgSet.ShowWindow(SW_HIDE);
	m_DlgAbout.ShowWindow(SW_HIDE);
	m_DlgShow.ShowWindow(SW_HIDE);
	//m_DlgAdv.InitDialogShow(FALSE);
	switch(nID)
	{
	case 0:
		m_DlgAbout.ShowWindow(SW_SHOW);
		break;
	case 1:
		m_DlgLanguage.ShowWindow(SW_SHOW);
		break;
	case 2:
		m_DlgSet.ShowWindow(SW_SHOW);
		m_DlgSet.OnBnState();
		break;
	case 3:
		m_DlgShow.ShowWindow(SW_SHOW);
		//m_DlgAdv.InitDialogShow(TRUE);
		break;
	}
}
void CINScanLiteDlg::SetControlFont()
{
	CFont *m_Font;
	
	if (sInfo.nLanguage==1){
		m_Font=&m_FontEnglish;
	    GetDlgItem(IDC_STATIC_CCD)->SetFont(&m_DlgTest.m_FontEnglish);
		GetDlgItem(IDC_STATIC_XY)->SetFont(&m_DlgTest.m_FontEnglish);
	}
	else{
		m_Font=&m_FontChinese;
	    GetDlgItem(IDC_STATIC_CCD)->SetFont(&m_DlgTest.m_FontChinese);
		GetDlgItem(IDC_STATIC_XY)->SetFont(&m_DlgTest.m_FontChinese);
	}

	m_ButtonAdv.SetFont(m_Font);
	m_ButtonTest.SetFont(m_Font);
	m_ButtonData.SetFont(m_Font);
	m_ButtonFile.SetFont(m_Font);
}
void CINScanLiteDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CINScanLiteDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CRect rect;
		CPaintDC dc(this);
		GetClientRect(rect);
		dc.FillSolidRect(rect,RGB(255,255,255));
		CDialogEx::OnPaint();

		CBitmap bmp;  
#ifdef Release_LO
		if (bmp.LoadBitmap(IDB_BITMAP_LO))
#else
		if (bmp.LoadBitmap(IDB_BITMAP1))
		//if (bmp.LoadBitmap(IDB_BITMAP15))  // lcz new 04/14 15为只有zolix英文字母的表，1为带中文信息的表，根据需要选择
#endif
		{  
			BITMAP bmpInfo;  
			bmp.GetBitmap(&bmpInfo);  
			CDC dcMemory;  
			CDC* pDC=GetDlgItem(IDC_STATIC_P)->GetDC();  
			dcMemory.CreateCompatibleDC(pDC);  
			CBitmap* pOldBitmap = dcMemory.SelectObject(&bmp);  
			CRect rect;  
			GetDlgItem(IDC_STATIC_P)->GetClientRect(&rect);  

			pDC->SetStretchBltMode(COLORONCOLOR);  
			pDC->StretchBlt(0, 0,rect.Width(),rect.Height(),&dcMemory,0, 0, bmpInfo.bmWidth, bmpInfo.bmHeight,SRCCOPY);  
			dcMemory.SelectObject(pOldBitmap);  
			ReleaseDC(pDC);  
		} 
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CINScanLiteDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

HBRUSH CINScanLiteDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	if (nCtlColor==CTLCOLOR_STATIC)
	{
		//pDC->SetBkMode(TRANSPARENT);
		pDC->SetBkColor(RGB(255,255,255));
		HBRUSH b =CreateSolidBrush(RGB(255,255,255));
		return b;
	}

	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;

}


void CINScanLiteDlg::OnBnClickedButtonTest()
{
	InitDialogShow(0);
	SetControlFont();

	g_bCloseLaser = true;//  关闭设置里的激光
	Sleep(100);
}


void CINScanLiteDlg::OnBnClickedButtonFile()
{

	InitDialogShow(1);
	m_DlgFile.InitDialogShow(0);

	g_bCloseLaser = true;//  关闭设置里的激光
	Sleep(100);
}


void CINScanLiteDlg::OnBnClickedButtonData()
{
	InitDialogShow(2);
	m_DlgFile.InitDialogShow(1);

	g_bCloseLaser = true;//  关闭设置里的激光
	Sleep(100);
}


void CINScanLiteDlg::OnBnClickedButtonAdv()
{
	InitDialogShow(3);
}


void CINScanLiteDlg::OnCancel()
{
	
	//  lcz new 04/25 关闭软件，检查激光器是否开着，如果开着，关闭激光器，停止测试
	g_bClose = true;
	if (sInfo.CConnect==1)
	{
		m_bAutoRunning = FALSE;
		Sleep(1000);
		m_DlgSet.OnBnClickedButtonCdis();
	}
	if (sInfo.LConnect==1)
	{
		//  需要先关闭激光器，再断开连接
		WriteToLog("\n");
		WriteToLog("关闭软件--OnCancel()");
		m_DlgShow.SetLawerPower(CloseLaserPow,FALSE); 
		m_DlgSet.OnBnClickedButtonLdis();
	}
	KillTimer(100);
	KillTimer(10);
	WriteDeviceInfo();
	pDlgLogin->DestroyWindow();
	delete pDlgLogin;
	pDlgWait->DestroyWindow();
	delete pDlgWait;
	Sleep(500);

//	m_DlgAdv.m_DlgLaCorr.DestroyWindow();
	
//	delete m_DlgAdv.m_DlgLaCorr;
	m_DlgAdv.DestroyWindow();
	delete m_DlgAdv;
	m_DlgFile.m_DlgDataCom.DestroyWindow();
	delete m_DlgFile.m_DlgDataCom;
	
	m_DlgFile.m_DlgDataPro.DestroyWindow();
	delete m_DlgFile.m_DlgDataPro;

	m_DlgFile.DestroyWindow();
	delete m_DlgFile;
	m_DlgTest.m_DlgTestCheckJ.DestroyWindow();
	delete m_DlgTest.m_DlgTestCheckJ;
	m_DlgTest.m_DlgTestCheckK.DestroyWindow();
	delete m_DlgTest.m_DlgTestCheckK;
	m_DlgTest.DestroyWindow();
	delete m_DlgTest;
	m_DlgShow.DestroyWindow();
	delete m_DlgShow;
	m_DlgAbout.DestroyWindow();
	delete m_DlgAbout;
	m_DlgSet.DestroyWindow();
	delete m_DlgSet;
	m_DlgLanguage.DestroyWindow();
	delete m_DlgLanguage;

	
	delete [] sOffsetFileInfo.dFscDataX;
	delete [] sOffsetFileInfo.dFscDataY;

	CDialogEx::OnCancel();
}


HBRUSH CAboutDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性

	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}
void CINScanLiteDlg::InitLan()
{
	if (sInfo.nLanguage==0)
	{	
		InitLanguage(_T("Chinese"));
	}
	else if (sInfo.nLanguage==1)
	{
		InitLanguage(_T("English"));
	}	
	else 
	{   
		InitLanguage(_T("ChineseTW"));
	}
	UpdateLanguage();
}

void CINScanLiteDlg::InitLanguage(CString strLang)//稍后预留接口
{
	CString tPath,str,strTemp;
	GetModuleFileName(NULL,tPath.GetBufferSetLength (MAX_PATH+1),MAX_PATH);
	tPath.ReleaseBuffer();
	int nPos=tPath.ReverseFind ('\\');
	str=tPath.Left (nPos);
	strTemp = str + _T("\\LANGUAGE\\")+strLang+_T(".ini");
	sInfo.strLangPathIni=strTemp;
	BOOL ret=m_ZLanguage.Init(strTemp);
	BOOL ret2=m_ZLanguage.LoadLanguage(strLang);

	str.Format(_T("%d"),sInfo.nLanguage);

	WritePrivateProfileString(_T("System"),_T("Language"),str,sInfo.strPath);
}

void CINScanLiteDlg::InitDeviceInfo()//加载硬件信息
{
	CString tPath,str,strTemp,strPath;
	GetModuleFileName(NULL,tPath.GetBufferSetLength (MAX_PATH+1),MAX_PATH);
	tPath.ReleaseBuffer();
	int nPos=tPath.ReverseFind ('\\');
	str=tPath.Left (nPos);
	g_strLogPath = str;
	sInfo.Path=str;
	sInfo.strPath=str + _T("\\INScan-Lite.ini");
	sInfo.strPathCCD=str + _T("\\ZolixCCD.ini");
	sInfo.strPathOffset=str + _T("\\ZolixOffsetFile.bm");

	//sInfo.OminiType=atoi(GetIniInfo(_T("System"),_T("OminiType")));// lcz new 04/14 取消从配置文件读取ccd类型
	sInfo.nLanguage=atoi(GetIniInfo(_T("System"),_T("Language")));
	sInfo.dDataRange=atoi(GetIniInfo(_T("System"),_T("DataRange")));

	sCInfo.TargetNum=atof(GetIniInfo(_T("CORRECT"),_T("TargetNum")));
	sCInfo.CorrectNum=atof(GetIniInfo(_T("CORRECT"),_T("CorrectNum")));
	sCInfo.Laser=atof(GetIniInfo(_T("CORRECT"),_T("Laser")));
	sCInfo.NMCM=atof(GetIniInfo(_T("CORRECT"),_T("NMCM")));
	sCInfo.CorrectHJW = atof(GetIniInfo(_T("CORRECT"),_T("CorrectHJW")));

	sTInfo.KPowerMode=atoi(GetIniInfo(_T("Test"),_T("KPowerMode")));
	sTInfo.KPower[0]=atof(GetIniInfo(_T("Test"),_T("KPowerL")));
	sTInfo.KPower[1]=atof(GetIniInfo(_T("Test"),_T("KPowerM")));
	sTInfo.KPower[2]=atof(GetIniInfo(_T("Test"),_T("KPowerH")));	
	sTInfo.KOutTime=atof(GetIniInfo(_T("Test"),_T("KOutTime")));
	sTInfo.KOutTimeLow=atof(GetIniInfo(_T("Test"),_T("KOutTimeLow")));
	sTInfo.KRangeX=atof(GetIniInfo(_T("Test"),_T("KRangeX")));
	sTInfo.KRangeS=atof(GetIniInfo(_T("Test"),_T("KRangeS")));
	sTInfo.KNum=atoi(GetIniInfo(_T("Test"),_T("KNum")));
	sTInfo.KFTime=atof(GetIniInfo(_T("Test"),_T("KFTime")));
	sTInfo.KAccNum=1;
	sTInfo.JTime=atof(GetIniInfo(_T("Test"),_T("JTime")));
	sTInfo.JAccNum=atoi(GetIniInfo(_T("Test"),_T("JAccNum")));
	sTInfo.JBack=atoi(GetIniInfo(_T("Test"),_T("JBack")));
	sTInfo.JPower=atof(GetIniInfo(_T("Test"),_T("JPower")));
	sTInfo.JPowerL=atoi(GetIniInfo(_T("Test"),_T("JPowerL")));
	sTInfo.JowerB=atoi(GetIniInfo(_T("Test"),_T("JowerB")));
	sTInfo.JBkK=atof(GetIniInfo(_T("Test"),_T("JBkK")));
	sTInfo.AddHeader = atoi(GetIniInfo(_T("Test"),_T("AddHeader")));//  lcz new 04/25 是否添加表头
	sTInfo.CrossX = atoi(GetIniInfo(_T("Test"),_T("CrossX")));// lcz new 05/23 记录上次十字光标的位置
	sTInfo.CrossY = atoi(GetIniInfo(_T("Test"),_T("CrossY")));// lcz new 05/23 记录上次十字光标的位置
	sTInfo.AutoTest = atoi(GetIniInfo(_T("Test"),_T("AutoTest")));// 是否显示自动测试
	sTInfo.b1064ccdFlip = atoi(GetIniInfo(_T("Test"), _T("1064CCDFlip")));

	sSInfo.isOEM=atoi(GetIniInfo(_T("SET"),_T("OEM")));
	sSInfo.LockSet=atoi(GetIniInfo(_T("SET"),_T("LockSet")));
	//  监视相机串口号
	sSInfo.nCOMLED=atoi(GetIniInfo(_T("SET"),_T("COMLED")));//  点击串口
	sSInfo.nComPrior=atoi(GetIniInfo(_T("SET"),_T("COMPRIOR")));
	sSInfo.nCOM=atoi(GetIniInfo(_T("SET"),_T("COM")));// 激光器串口
	sSInfo.HLMODE=atoi(GetIniInfo(_T("SET"),_T("HLMODE")));
	sSInfo.Temp=atoi(GetIniInfo(_T("SET"),_T("Temp")));
	sSInfo.Delay=atoi(GetIniInfo(_T("SET"),_T("Delay")));
	sSInfo.XLeft=atoi(GetIniInfo(_T("SET"),_T("XLeft")));
	sSInfo.XRight=atoi(GetIniInfo(_T("SET"),_T("XRight")));
	sSInfo.Back=atoi(GetIniInfo(_T("SET"),_T("Back")));
	sSInfo.nSqlMode=atoi(GetIniInfo(_T("SET"),_T("SqlMode")));
	sSInfo.iInitLaserPower=atoi(GetIniInfo(_T("SET"),_T("SetInitLaserPower")));//  读取配置文件里默认的激光功率
	sSInfo.CoolTemp = atoi(GetIniInfo(_T("SET"), _T("CoolTemp")));//  lcz new 23/05/12  从配置文件里读取1064探测器设置的制冷温度
	//if (sSInfo.isOEM==1)  //2025.2.25去掉,不让它从配置文件读了,直接根据杏林或新产业来进行赋值
	//{
	//	sSInfo.LASERPOWERMIN=atoi(GetIniInfo(_T("SET"),_T("LASERPOWERMIN")));
	//	sSInfo.LASERPOWERMAX=atoi(GetIniInfo(_T("SET"),_T("LASERPOWERMAX")));
	//}
	if(sSInfo.isOEM == 0)
	{
		ReadOemMaxMin();
	}
	sSInfo.firstOminiType = atoi(GetIniInfo(_T("SET"), _T("FirstOminiType")));

	sSInfo.LaserPowerMin_XL = atoi(GetIniInfo(_T("SET"), _T("LaserPowerMin_XingLin")));
	sSInfo.LaserPowerMax_XL = atoi(GetIniInfo(_T("SET"), _T("LaserPowerMax_XingLin")));
	sSInfo.LaserPowerMin_XCY = atoi(GetIniInfo(_T("SET"), _T("LaserPowerMin_XinChanYe")));
	sSInfo.LaserPowerMax_XCY = atoi(GetIniInfo(_T("SET"), _T("LaserPowerMax_XinChanYe")));
	
	sSInfo.Cool=atoi(GetIniInfo(_T("SET"),_T("Cool")));
	sSInfo.DataPer=atof(GetIniInfo(_T("SET"),_T("DataPer")));
	sSInfo.CheckLed=atof(GetIniInfo(_T("SET"),_T("CheckLed")));
	sSInfo.ComboLed=atof(GetIniInfo(_T("SET"),_T("ComboLed")));
	sSInfo.CheckOffset=atof(GetIniInfo(_T("SET"),_T("CheckOffset")));
	sSInfo.d532rdWaveNum=atof(GetIniInfo(_T("SET"),_T("FI532rdWaveNum")));
	sSInfo.d785rdWaveNum=atof(GetIniInfo(_T("SET"),_T("FI785rdWaveNum")));
	sSInfo.iAddPointNum=atof(GetIniInfo(_T("SET"),_T("AddPointNum")));// lcz new 22/10/11
	sSInfo.isCheckLed = atoi(GetIniInfo(_T("SET"), _T("isCheckLed")));// lcz new 22/10/11

	sSInfo.a[0]=atof(GetIniInfo(_T("xs"),_T("a0")));
	sSInfo.a[1]=atof(GetIniInfo(_T("xs"),_T("a1")));
	sSInfo.a[2]=atof(GetIniInfo(_T("xs"),_T("a2")));
	sSInfo.a[3]=atof(GetIniInfo(_T("xs"),_T("a3")));
	sSInfo.b[0]=atof(GetIniInfo(_T("xsWaveNum"),_T("b0")));
	sSInfo.b[1]=atof(GetIniInfo(_T("xsWaveNum"),_T("b1")));
	sSInfo.b[2]=atof(GetIniInfo(_T("xsWaveNum"),_T("b2")));
	sSInfo.b[3]=atof(GetIniInfo(_T("xsWaveNum"),_T("b3")));


	sPInfo.Human=GetIniInfo(_T("PRINT"),_T("Human"));
	sPInfo.ProName=GetIniInfo(_T("PRINT"),_T("ProName"));
	sPInfo.Model=GetIniInfo(_T("PRINT"),_T("Model"));
	sPInfo.Number=GetIniInfo(_T("PRINT"),_T("Number"));

	sINInfo.Time=atoi(GetIniInfo(_T("INTIME"),_T("Time")));
	sINInfo.AccNum=atoi(GetIniInfo(_T("INTIME"),_T("AccNum")));
	sINInfo.bCheak=atoi(GetIniInfo(_T("INTIME"),_T("bCheak")));
	sINInfo.bCheakSmooth =atoi(GetIniInfo(_T("INTIME"),_T("bCheakSmooth")));
	sINInfo.nSmooth=atoi(GetIniInfo(_T("INTIME"),_T("nSmooth")));
	sINInfo.bCheakFit=atoi(GetIniInfo(_T("INTIME"),_T("bCheakFit")));
	sINInfo.nFit=atoi(GetIniInfo(_T("INTIME"),_T("nFit")));
	sINInfo.bCheakCut=atoi(GetIniInfo(_T("INTIME"),_T("bCheakCut")));
	sINInfo.nFileType=atoi(GetIniInfo(_T("INTIME"),_T("nFileType")));
	sINInfo.dEditStart=atof(GetIniInfo(_T("INTIME"),_T("dEditStart")));
	sINInfo.dEditEnd=atof(GetIniInfo(_T("INTIME"),_T("dEditEnd")));
	sINInfo.strName=GetIniInfo(_T("INTIME"),_T("strName"));
	sINInfo.strPath=GetIniInfo(_T("INTIME"),_T("strPath"));
	if(!PathIsDirectory(sINInfo.strPath))//判断路径是否存在  
	{
		CString tPath,str,strTemp;
		GetModuleFileName(NULL,tPath.GetBufferSetLength (MAX_PATH+1),MAX_PATH);
		tPath.ReleaseBuffer();
		tPath.ReleaseBuffer();
		int nPos=tPath.ReverseFind ('\\');
		str=tPath.Left (nPos);
		strTemp = str + _T("\\Data\\");
		sINInfo.strPath = strTemp;
		CreateDirectory(sINInfo.strPath,NULL);//新建文件夹
	}

}

CString CINScanLiteDlg::GetIniInfo(CString strT,CString strI)
{
	CString strTemp;
	GetPrivateProfileString(strT,strI,_T(""),strTemp.GetBuffer(1024),1024,sInfo.strPath);
	strTemp.ReleaseBuffer();
	return strTemp;
}

void CINScanLiteDlg::WriteIniInfo(CString strT,CString strInfo,CString strPath)
{
	WritePrivateProfileString(strT,strInfo,strPath,sInfo.strPath);
}

//向INScan-Lite.ini里写入硬件信息，相当于更新   关闭软件时执行
void CINScanLiteDlg::WriteDeviceInfo()
{
	CString strTemp;

	//strTemp.Format(_T("%d"),sInfo.OminiType);
	//WriteIniInfo(_T("System"),_T("OminiType"),strTemp);//  lcz new 04/14 取消配置文件中ccd类型的更新写入
	strTemp.Format(_T("%d"),sInfo.nLanguage);
	WriteIniInfo(_T("System"),_T("Language"),strTemp);
	strTemp.Format(_T("%d"),sInfo.dDataRange);
	WriteIniInfo(_T("System"),_T("DataRange"),strTemp);


	strTemp.Format(_T("%f"),sCInfo.TargetNum);
	WriteIniInfo(_T("CORRECT"),_T("TargetNum"),strTemp);
	strTemp.Format(_T("%f"),sCInfo.CorrectNum);
	WriteIniInfo(_T("CORRECT"),_T("CorrectNum"),strTemp);
	strTemp.Format(_T("%f"),sCInfo.Laser);
	WriteIniInfo(_T("CORRECT"),_T("Laser"),strTemp);

	strTemp.Format(_T("%d"),sTInfo.KPowerMode);
	WriteIniInfo(_T("Test"),_T("KPowerMode"),strTemp);
	strTemp.Format(_T("%f"),sTInfo.KOutTime);
	WriteIniInfo(_T("Test"),_T("KOutTime"),strTemp);
	strTemp.Format(_T("%f"),sTInfo.JTime);
	WriteIniInfo(_T("Test"),_T("JTime"),strTemp);
	strTemp.Format(_T("%d"),sTInfo.JAccNum);
	WriteIniInfo(_T("Test"),_T("JAccNum"),strTemp);
	strTemp.Format(_T("%d"),sTInfo.JBack);
	WriteIniInfo(_T("Test"),_T("JBack"),strTemp);
	strTemp.Format(_T("%f"),sTInfo.JPower);
	WriteIniInfo(_T("Test"),_T("JPower"),strTemp);
	strTemp.Format(_T("%f"),sTInfo.JBkK);
	WriteIniInfo(_T("Test"),_T("JBkK"),strTemp);
	//  保存是否添加表头的标识符
	strTemp.Format(_T("%d"),sTInfo.AddHeader);
	WriteIniInfo(_T("Test"),_T("AddHeader"),strTemp);
	// lcz new 05/23 添加保存十字光标的位置
	strTemp.Format(_T("%d"),sTInfo.CrossX);
	WriteIniInfo(_T("Test"),_T("CrossX"),strTemp);
	strTemp.Format(_T("%d"),sTInfo.CrossY);
	WriteIniInfo(_T("Test"),_T("CrossY"),strTemp);

	strTemp.Format(_T("%d"),sSInfo.nCOMLED);
	WriteIniInfo(_T("SET"),_T("COMLED"),strTemp);
	strTemp.Format(_T("%d"),sSInfo.LockSet);
	WriteIniInfo(_T("SET"),_T("LockSet"),strTemp);
	
	strTemp.Format(_T("%d"),sSInfo.nComPrior);
	WriteIniInfo(_T("SET"),_T("COMPRIOR"),strTemp);
	
	strTemp.Format(_T("%d"),sSInfo.isOEM);
	WriteIniInfo(_T("SET"),_T("OEM"),strTemp);
	strTemp.Format(_T("%d"),sSInfo.nCOM);
	WriteIniInfo(_T("SET"),_T("COM"),strTemp);
	strTemp.Format(_T("%d"),sSInfo.HLMODE);
	WriteIniInfo(_T("SET"),_T("HLMODE"),strTemp);
	strTemp.Format(_T("%f"),sSInfo.Temp);
	WriteIniInfo(_T("SET"),_T("Temp"),strTemp);
	strTemp.Format(_T("%d"),sSInfo.Delay);
	WriteIniInfo(_T("SET"),_T("Delay"),strTemp);
	strTemp.Format(_T("%f"),sSInfo.XLeft);
	WriteIniInfo(_T("SET"),_T("XLeft"),strTemp);
	strTemp.Format(_T("%f"),sSInfo.XRight);
	WriteIniInfo(_T("SET"),_T("XRight"),strTemp);
	strTemp.Format(_T("%d"),sSInfo.Back);
	WriteIniInfo(_T("SET"),_T("Back"),strTemp);
	strTemp.Format(_T("%f"),sSInfo.DataPer);
	WriteIniInfo(_T("SET"),_T("DataPer"),strTemp);
	strTemp.Format(_T("%d"),sSInfo.CheckLed);
	WriteIniInfo(_T("SET"),_T("CheckLed"),strTemp);
	strTemp.Format(_T("%d"),sSInfo.ComboLed);
	WriteIniInfo(_T("SET"),_T("ComboLed"),strTemp);
	strTemp.Format(_T("%d"),sSInfo.CheckOffset);
	WriteIniInfo(_T("SET"),_T("CheckOffset"),strTemp);
	strTemp.Format(_T("%d"),sSInfo.nSqlMode);
	WriteIniInfo(_T("SET"),_T("SqlMode"),strTemp);

	strTemp.Format(_T("%d"), sSInfo.isCheckLed);
	WriteIniInfo(_T("SET"), _T("isCheckLed"), strTemp);

	strTemp.Format(_T("%d"), sSInfo.LaserPowerMin_XL);
	WriteIniInfo(_T("SET"), _T("LaserPowerMin_XingLin"), strTemp);
	strTemp.Format(_T("%d"), sSInfo.LaserPowerMax_XL);
	WriteIniInfo(_T("SET"), _T("LaserPowerMax_XingLin"), strTemp);
	strTemp.Format(_T("%d"), sSInfo.LaserPowerMin_XCY);
	WriteIniInfo(_T("SET"), _T("LaserPowerMin_XinChanYe"), strTemp);
	strTemp.Format(_T("%d"), sSInfo.LaserPowerMax_XCY);
	WriteIniInfo(_T("SET"), _T("LaserPowerMax_XinChanYe"), strTemp);

	WriteIniInfo(_T("PRINT"),_T("Human"),sPInfo.Human);
	WriteIniInfo(_T("PRINT"),_T("ProName"),sPInfo.ProName);
	WriteIniInfo(_T("PRINT"),_T("Model"),sPInfo.Model);
	WriteIniInfo(_T("PRINT"),_T("Number"),sPInfo.Number);

	strTemp.Format(_T("%d"),sINInfo.Time);
	WriteIniInfo(_T("INTIME"),_T("Time"),strTemp);
	strTemp.Format(_T("%d"),sINInfo.AccNum);
	WriteIniInfo(_T("INTIME"),_T("AccNum"),strTemp);
	strTemp.Format(_T("%d"),sINInfo.bCheak);
	WriteIniInfo(_T("INTIME"),_T("bCheak"),strTemp);
	WriteIniInfo(_T("INTIME"),_T("strName"),sINInfo.strName);
	WriteIniInfo(_T("INTIME"),_T("strPath"),sINInfo.strPath);

	strTemp.Format(_T("%d"),sINInfo.bCheakSmooth);
	WriteIniInfo(_T("INTIME"),_T("bCheakSmooth"),strTemp);
	strTemp.Format(_T("%d"),sINInfo.nSmooth);
	WriteIniInfo(_T("INTIME"),_T("nSmooth"),strTemp);
	strTemp.Format(_T("%d"),sINInfo.bCheakFit);
	WriteIniInfo(_T("INTIME"),_T("bCheakFit"),strTemp);
	strTemp.Format(_T("%d"),sINInfo.nFit);
	WriteIniInfo(_T("INTIME"),_T("nFit"),strTemp);
	strTemp.Format(_T("%d"),sINInfo.bCheakCut);
	WriteIniInfo(_T("INTIME"),_T("bCheakCut"),strTemp);
	strTemp.Format(_T("%d"),sINInfo.nFileType);
	WriteIniInfo(_T("INTIME"),_T("nFileType"),strTemp);
	strTemp.Format(_T("%f"),sINInfo.dEditStart);
	WriteIniInfo(_T("INTIME"),_T("dEditStart"),strTemp);
	strTemp.Format(_T("%f"),sINInfo.dEditEnd);
	WriteIniInfo(_T("INTIME"),_T("dEditEnd"),strTemp);


}

void CINScanLiteDlg::UpdateLanguage()
{
	m_ZLanguage.SetCreateLanguageState(FALSE);
	m_ZLanguage.SetDialogStrings((long)(CDialog*)this,IDD);

	
	m_DlgAbout.UpdateLanguage();
	m_DlgAdv.UpdateLanguage();
	m_DlgFile.UpdateLanguage();
	m_DlgTest.UpdateLanguage();
	m_DlgSet.UpdateLanguage();

	SetControlFont();
}

DWORD CINScanLiteDlg:: ClockTreadProc(BOOL bState,int nNum)//f<100 t=100,nNum=0 才进行
{
	if (nNum==0)
	{
		if (bState==FALSE)
		{
			if(sInfo.statusBar<100)
			{
				sInfo.statusBar+=20;		
				SendMessage(WM_MY_STATUSBAR,0,0);
			}
		}
		else
		{
			SendMessage(WM_MY_STATUSBAR,0,0);
		}
	}

	return 0L;
}

LRESULT CINScanLiteDlg::OnDlgLogin(WPARAM wParam,LPARAM lParam)
{

	if (sInfo.statusBar==0)
	{
		CBitmap bitmap;
		HBITMAP hBmp;
		bitmap.LoadBitmap(IDB_BITMAP7);
		hBmp=(HBITMAP)bitmap.GetSafeHandle();
		pDlgLogin->m_StaticP.SetBitmap(hBmp);
		pDlgLogin->OnPaint();

	}
	pDlgLogin->SetPro();

	return TRUE;
}

CString CINScanLiteDlg::LoadStringMeau(CString str)
{
	CString strTemp=m_ZLanguage.LoadStringA(str,_T(""));
	return strTemp;
}


void CINScanLiteDlg::SetBtnS()
{
	if (sInfo.CConnect==1/*&&sInfo.LConnect==1*/)
	{
		GetDlgItem(IDC_STATIC_GREEN)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_GRAY)->ShowWindow(SW_HIDE);
		m_DlgTest.m_DlgTestCheckJ.BtnEnable(TRUE);
		m_DlgTest.m_DlgTestCheckK.BtnEnable(TRUE);
		m_DlgAdv.m_BtnOff.EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_STATIC_GREEN)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_GRAY)->ShowWindow(SW_SHOW);
		m_DlgTest.m_DlgTestCheckJ.BtnEnable(FALSE);
		m_DlgTest.m_DlgTestCheckK.BtnEnable(FALSE);
		m_DlgAdv.m_BtnOff.EnableWindow(FALSE);
	}

}

void CINScanLiteDlg::SetBtnEnable(BOOL bShow)
{
	GetDlgItem(IDC_BUTTON_TEST)->EnableWindow(bShow);
	GetDlgItem(IDC_BUTTON_FILE)->EnableWindow(bShow);
	GetDlgItem(IDC_BUTTON_DATA)->EnableWindow(bShow);
	GetDlgItem(IDC_BUTTON_ADV)->EnableWindow(bShow);
    
	m_DlgTest.EnableBtn(bShow);
}



BOOL CINScanLiteDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN)
	{
		switch(pMsg->wParam)
		{
		case VK_RETURN:
			return  TRUE;
		case VK_ESCAPE:
			return TRUE;
		default:
			break;
		}
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}

// 响应鼠标在非工作区左键单击消息
void CINScanLiteDlg::OnNcLButtonDown(UINT nHitTest, CPoint point)
{
	if(!m_bMove)
	{
		if (HTCAPTION == nHitTest)
		{
			return;
		}
	}
	
	CDialogEx::OnNcLButtonDown(nHitTest, point);
}


void CINScanLiteDlg::OnUpdateName()
{
	//重命名
	CSetNameDlg dlg;
	dlg.DoModal();

}


void CINScanLiteDlg::OnSetColor(int nNum)
{
	// TODO: 在此添加命令处理程序代码
	//设置颜色
	m_DlgShow.setColor(0,0,0,nNum);

}


void CINScanLiteDlg::OnDel(int nSer)
{
	// TODO: 在此添加命令处理程序代码
	//删除
	m_DlgShow.del(nSer);


}


void CINScanLiteDlg::OnDelAll()
{
	// TODO: 在此添加命令处理程序代码
	//删除全部
	m_DlgShow.delAll();
}

//  自动缩放功能实现
void CINScanLiteDlg::OnAutoZoom()
{
	// TODO: 在此添加命令处理程序代码
	m_DlgShow.m_Tchart.GetAxis().GetLeft().SetAutomatic(TRUE);
	m_DlgShow.m_Tchart.GetAxis().GetBottom().SetAutomatic(TRUE);
}


void CINScanLiteDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
	m_DlgTest.ReleaseResource();
}

void CINScanLiteDlg::ShowMouseXY(double fX /*= 0*/, double fY /*= 0*/)
{
	CString str;
	str.Format(_T("X=%.1f, Y=%.1f"),fX, fY);
	GetDlgItem(IDC_STATIC_XY)->SetWindowTextA(str);
}

LRESULT CINScanLiteDlg::OnMsgUpdateLineName(WPARAM wParam, LPARAM lParam)
{
  //MessageBox("recv msg success");
  //添加自己的消息处理
  CString* pStrOld = (CString*)wParam;
  CString* pStrNew = (CString*)lParam;

  m_DlgShow.updateName(*pStrNew, *pStrOld);
  return 0;  
}

LRESULT CINScanLiteDlg::OnMsgShowLine(WPARAM wParam, LPARAM lParam)
{
	CString* pStrName = (CString*)wParam;
	m_DlgShow.showLine(*pStrName);
	return 0;  
}
// lcz new 01/31
int CINScanLiteDlg::WriteToLog(char* str)
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

