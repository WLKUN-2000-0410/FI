// MiniCamera.cpp : 实现文件
//

#include "stdafx.h"
#include "MiniCamera.h"
#include "afxdialogex.h"
#include "INScan-LiteDlg.h"
#include <DICAMAPI.h>


const UINT WM_USERDEFMSG = ::RegisterWindowMessage(_T("UserDefMsg"));

CPoint x_start,x_end,y_start,y_end;
bool flag = false;

UINT AFX_CDECL Acquire_XT_Frame(LPVOID target)
{
	CMiniCamera *camera = (CMiniCamera*)target;
	while (camera->m_waitFrame)
	{
		if (TUCAMRET_SUCCESS == TUCAM_Buf_WaitForFrame(camera->opCam.hIdxTUCam, &camera->m_tf))
		{
			// 跳出循环
			if (!camera->m_waitFrame)
			{
				break;
			}
			TUCAM_DRAW pd;
			pd.pFrame = &camera->m_tf;

			pd.nDstX = 0;
			pd.nDstY = 0;

			CRect rc;
			camera->GetDlgItem(IDC_STATIC_VIDEO)->GetWindowRect(rc);
			pd.nDstWidth = rc.Width();
			pd.nDstHeight = rc.Height();

			pd.nSrcX = 0;
			pd.nSrcY = 0;
			pd.nSrcWidth = camera->m_tf.usWidth;
			pd.nSrcHeight = camera->m_tf.usHeight;

			TUCAM_Capa_SetValue(camera->opCam.hIdxTUCam, TUIDC_VERCORRECTION, 1);
			if (TUCAMRET_SUCCESS != TUCAM_Draw_Frame(camera->opCam.hIdxTUCam, &pd))
			{

			}
			else
			{

			}
		}
	}
	return 0;
}


int CALLBACK MY_DI_SNAP_PROC(BYTE *pImageBuffer, DI_DATA_TYPE TYPE, LPVOID lpContext)
{

	return 0;
}


IMPLEMENT_DYNAMIC(CMiniCamera, CDialogEx)

CMiniCamera::CMiniCamera(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMiniCamera::IDD, pParent)
	, m_Aeen(FALSE)
	, GammaValue(0)
{
	mousedown=FALSE;
	this->lookflag=true;
	stopline=false;
	usbCameraIndex = 0;
}

CMiniCamera::~CMiniCamera()
{
	if(m_hWnd != NULL) 
		DestroyWindow(); 
}

void CMiniCamera::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//DDX_Control(pDX, IDC_COMBO1, m_ListCtrl);
	//DDX_Control(pDX, IDC_SHOW_IMG, m_staticScreen);
	//DDX_Control(pDX, IDC_BUTTON_IMG , m_btn1 );
	DDX_Control(pDX, IDC_STATIC_VIDEO, m_Video);
	DDX_Control(pDX, IDC_SHOW_IMG, m_show);
	DDX_Control(pDX, IDC_SLIDER2, m_RGainCtrl);
	DDX_Control(pDX, IDC_SLIDER3, m_GGainCtrl);
	DDX_Control(pDX, IDC_SLIDER4, m_BGainCtrl);
	DDX_Control(pDX, IDC_SLIDER1, m_ExpTimeCtrl);
	DDX_Check(pDX, IDC_CHECK1, m_Aeen);
	DDX_Control(pDX, IDC_SLIDER5, m_Gamma);
	DDX_Control(pDX, IDC_SLIDER6, m_Contrast);
	DDX_Slider(pDX, IDC_SLIDER5, GammaValue);
	DDX_Control(pDX, IDC_SLIDER7, m_SaturationCtrl);
	DDX_Control(pDX, IDC_SLIDER8, m_ACtrl);
	DDX_Control(pDX, IDC_SLIDER9, m_SliderTest);
}


BEGIN_MESSAGE_MAP(CMiniCamera, CDialogEx)
//	ON_BN_CLICKED(IDC_OPENCAMERA, &CMiniCamera::OnBnClickedOpencamera)
	ON_WM_TIMER()
	//ON_BN_CLICKED(IDC_CLOSE, &CMiniCamera::OnBnClickedClose)
	ON_WM_CLOSE()
	ON_WM_QUERYDRAGICON()
	ON_WM_PAINT()
	//ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_LBUTTONDOWN()
	ON_STN_CLICKED(IDC_SHOW_IMG, &CMiniCamera::OnStnClickedShowImg)
	ON_BN_CLICKED(IDOK, &CMiniCamera::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CMiniCamera::OnBnClickedCancel)
	//ON_WM_VSCROLL()
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_WM_RBUTTONDOWN()
	ON_BN_CLICKED(IDC_BUTTON_PAUSE, &CMiniCamera::OnBnClickedButtonPause)
	ON_BN_CLICKED(IDC_BUTTON_RUN, &CMiniCamera::OnBnClickedButtonRun)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER2, &CMiniCamera::OnNMCustomdrawSlider2)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER3, &CMiniCamera::OnNMCustomdrawSlider3)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER4, &CMiniCamera::OnNMCustomdrawSlider4)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER1, &CMiniCamera::OnNMCustomdrawSlider1)
	ON_BN_CLICKED(IDC_BUTTON1, &CMiniCamera::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_CHECK1, &CMiniCamera::OnBnClickedCheck1)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER5, &CMiniCamera::OnNMCustomdrawSlider5)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER6, &CMiniCamera::OnNMCustomdrawSlider6)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER7, &CMiniCamera::OnNMCustomdrawSlider7)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER8, &CMiniCamera::OnNMCustomdrawSlider8)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER9, &CMiniCamera::OnNMCustomdrawSlider9)
END_MESSAGE_MAP()

// lcz new 01/31
int CMiniCamera::WriteToLog(char* str)
{
        FILE* pfile;
        fopen_s(&pfile, FILE_PATH, "a+");

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

// CMiniCamera 消息处理程序
//  一号定时器，间隔1ms，刷新红线标记
void CMiniCamera::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	/*CPen pen(1,1,RGB(255,0,0));
	CClientDC dc(this);
	CPen *pOldPen=dc.SelectObject(&pen);
	dc.MoveTo(x_start);
	dc.LineTo(x_end);
	//dc.SelectObject(pOldPen);
	dc.MoveTo(y_start);
	dc.LineTo(y_end);
	dc.SelectObject(pOldPen);
	*/

	/**************************************************/
	if (100 == nIDEvent)
	{
		if (TUCAMRET_SUCCESS == TUCAM_Buf_WaitForFrame(opCam.hIdxTUCam, &m_tf))
		{
			TUCAM_DRAW pd;
			pd.pFrame = &m_tf;

			pd.nDstX = 0;
			pd.nDstY = 0;

			CRect rc;
			GetDlgItem(IDC_STATIC_VIDEO)->GetWindowRect(rc);
			pd.nDstWidth = rc.Width();
			pd.nDstHeight = rc.Height();

			pd.nSrcX = 0;
			pd.nSrcY = 0;
			pd.nSrcWidth = m_tf.usWidth;
			pd.nSrcHeight = m_tf.usHeight;

			TUCAM_Capa_SetValue(opCam.hIdxTUCam, TUIDC_VERCORRECTION, 1);
			if (TUCAMRET_SUCCESS != TUCAM_Draw_Frame(opCam.hIdxTUCam, &pd))
			{
				
			}
			else
			{
				
			}
		}
	}
	else if (99 == nIDEvent)
	{
		
		DI_DATA_TYPE type = DATA_TYPE_RGB24;
		// DI_CAMERA_STATUS s = CameraCaptureToBuf(data, type, R640_480);
		DI_CAMERA_STATUS s = CameraCaptureFile("./1", FILE_JPG, 90, R640_480);
		if (s == STATUS_OK)
		{
			// CameraCaptureFile("./1", FILE_JPG, 90, R640_480);
			Mat m = imread("./1.jpg");
			
			/*
			Mat m(480, 640, CV_8UC3);
			BYTE *data = new BYTE[640 * 480 * 3];
			{
				const int kHeight = m.rows;
				const int kWidth = m.cols;
				double t1 = cv::getTickCount();

				for (int col = 0; col<kWidth; ++col) {
					for (int row = 0; row<kHeight; ++row) {
						auto& rgb = m.at<cv::Vec3b>(row, col);
						rgb[0] = data[row * 3 +  col * 3 + 2];
						rgb[1] = data[row * 3  + col * 3 + 1];
						rgb[2] = data[row * 3 + col * 3];
					}
				}
			}
			delete [] data;
			*/

			int width = m.cols;
			int height = m.rows;

			if (width != 0 || height != 0)
			{
				int halfW = width / 2;
				int halfH = height / 2;

				int left = (halfW / 10) * m_leftRoi;
				int top = (halfH / 10) * m_topRoi;
				int right = width - (halfW / 10) * m_rightRoi;
				int bottom = height - (halfH / 10) * m_bottomRoi;

				if (!m.empty())
				{
					imshow("picView2", m(Range(top, bottom), Range(left, right)));//显示当前帧图像
				}
			}
		}

	
		}
		

	/**************************************************/

	//WriteToLog("重新绘制了十字光标");// lcz new 01/31
	CDialogEx::OnTimer(nIDEvent);
}


//  绘制十字光标的线程
UINT LINE(PVOID Param)
{
	CMiniCamera* m_test=(CMiniCamera*)Param;
	m_test->drowline();
	return 0;
}



BOOL CMiniCamera::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	CINScanLiteDlg *pDlg = (CINScanLiteDlg*)AfxGetApp();

	if (sCInfo.Laser - 532 < 10)
	{

	}
	else
	{
		// 785相机
		// 785激光器补充流程点击“打开相机”判断激活功率是否为0 %
		// 判断是否“停止获取”，两者都为真，发送“关使能指令”关闭激光器
		if (m_bAutoRunning == FALSE)
		{
			// 激光器关闭使能
			pDlg->m_DlgSet.m_Serial.CloseLaserPower(sSInfo.nCOM);
		}
	}

	//  lcz new 02/06  将cv窗口初始化和设置
	CRect rc;
	GetDlgItem(IDC_STATIC_VIDEO)->GetWindowRect(rc);


	//cv::namedWindow("picView", cv::WINDOW_AUTOSIZE);// cv窗口名为"picView"
	cv::namedWindow("picView", cv::WINDOW_NORMAL);
	cv::resizeWindow("picView", rc.Width(), rc.Height());// cv窗口名为"picView" lcz new 03/21  调整窗口尺寸，避免采集的图像范围过大导致最后的视频窗口的图像不居中

	//cv::setWindowProperty("picView", 4, 0.0);
	//double ii = getWindowProperty("picView", 4);
	//char buffer[10];
	//sprintf(buffer, "%.9f", ii);	//9位小数
	//string str = buffer;
	//char *c = const_cast<char *>(str.c_str());
	//WriteToLog(c);// lcz new 02/06
	HWND hWnd = (HWND)cvGetWindowHandle("picView");//cv窗口句柄为hWnd
	HWND cvhWnd = hWnd;
	HWND hParent = ::GetParent(hWnd);//  cv窗口句柄hWnd的父句柄为hParent
	::SetParent(hWnd, GetDlgItem(IDC_STATIC_VIDEO)->m_hWnd);
	::ShowWindow(hParent, SW_HIDE);

	// TODO:  在此添加额外的初始化
	/*m_cap.EnumDevices (m_ListCtrl);
	m_ListCtrl.SetCurSel (0); */
	GetClientRect(&m_rect);  //获取对话框的大小
	old.x=m_rect.right-m_rect.left;  
	old.y=m_rect.bottom-m_rect.top;

	CString strResult;
	int dwSize=1024;
	int  sel;

	sel=sSInfo.ComboLed;
	usbCameraIndex = sel;
	//CINScanLiteDlg * pDlg=(CINScanLiteDlg*)AfxGetApp()->GetMainWnd();

	if (lookflag==true)
	{
		/*CWnd* pWnd = GetDlgItem(IDC_SHOW_IMG); 
		pWnd->ModifyStyle(1,WS_CLIPCHILDREN); */
		HWND hWnd = GetDlgItem(IDC_STATIC_VIDEO)->GetSafeHwnd();
		//HWND hWnd = GetDlgItem(IDC_BUTTON_IMG)->GetSafeHwnd();
		//HRESULT hr = m_cap.Init(sel,hWnd);//////////////0默认电脑摄像头 1设备摄像头//  lcz new 02/02
		lookflag=false;
		//  lcz new  原先代码里设置了，打开相机自动开激光，注释掉
		//  pDlg->m_DlgSet.m_Serial.OpenLaserPower(sSInfo.nCOM);
		// lcz new 05/27  如果打开的不是usb相机，则不移动电机

		//if(pDlg->m_DlgSet.iUSBCameraID == sSInfo.ComboLed)// lcz new 01/31
			pDlg->m_DlgSet.m_SerialLed.MoveLed(true);
		//OnBnClickedButtonPause();
		//XSleep(3000);

		//SetTimer(1,10,NULL);// lcz new 01/31
		int dwSize=1024;
		CString strResult;
	//	GetPrivateProfileString("crosspoint","xpoint","0",strResult.GetBuffer(dwSize),dwSize,pThreadApp->PathIni);
		crospoint.x = 233;// _ttoi(strResult);
	////	GetPrivateProfileString("crosspoint","ypoint","0",strResult.GetBuffer(dwSize),dwSize,pThreadApp->PathIni);
		crospoint.y = 196;//_ttoi(strResult);
		
		CRect Prect1;          //定义图片的矩形
		//GetDlgItem(IDC_SHOW_IMG)->GetWindowRect(&Prect1);    //得到图片的矩//形大小   
		GetDlgItem(IDC_STATIC_VIDEO)->GetWindowRect(&Prect1);    //得到图片的矩//形大小
		ScreenToClient(&Prect1);   //将图片框的绝对矩形大小
		// lcz new 05/23 记录中心十字光标的位置
		crospoint.x = Prect1.right / 2;
		crospoint.y = Prect1.bottom / 2;
		if(sTInfo.CrossY > Prect1.bottom || sTInfo.CrossX > Prect1.right)
		{
			x_start.x=Prect1.left;
			x_start.y=crospoint.y;
			x_end.x =Prect1.right;
			x_end.y =crospoint.y;

			y_start.x = crospoint.x;
			y_start.y = Prect1.top;
			y_end.x = crospoint.x;
			y_end.y = Prect1.bottom;
		}
		else
		{
			x_start.x=Prect1.left;
			x_start.y=sTInfo.CrossY;
			x_end.x =Prect1.right;
			x_end.y =sTInfo.CrossY;

			y_start.x = sTInfo.CrossX;
			y_start.y = Prect1.top;
			y_end.x = sTInfo.CrossX;
			y_end.y = Prect1.bottom;
		}

		// lcz new 02/03  直接让光标居中
		x_start.x = Prect1.left;
		x_start.y = Prect1.bottom / 2;
		x_end.x = Prect1.right;
		x_end.y = Prect1.bottom / 2;

		y_start.x = Prect1.right / 2;
		y_start.y = Prect1.top;
		y_end.x = Prect1.right / 2;
		y_end.y = Prect1.bottom;
		// lcz new 05/23 记录当前点的位置
		sTInfo.CrossX = y_start.x;
		sTInfo.CrossY = x_start.y;// lcz new 02/03

		AfxBeginThread(LINE,this);
	}
	Sleep(100);
	//if(sInfo.LConnect==1)
	//{
	//	//pDlg->m_DlgShow.SetLawerPower(sTInfo.JPower,TRUE);
	//	pDlg->m_DlgSet.m_Serial.OpenCameraLigth(sSInfo.nCOM,"ON");
	//}


	char method[1024];
	GetPrivateProfileStringA("CAMERA", "method", "opencv", method, 1024, sInfo.strPath);

	m_leftRoi = GetPrivateProfileIntA("CAMERA", "left", 0, sInfo.strPath);
	m_topRoi = GetPrivateProfileIntA("CAMERA", "top", 0, sInfo.strPath);
	m_rightRoi = GetPrivateProfileIntA("CAMERA", "right", 0, sInfo.strPath);
	m_bottomRoi = GetPrivateProfileIntA("CAMERA", "bottom", 0, sInfo.strPath);


	hoffset = GetPrivateProfileIntA("CAMERA", "hoffset", 0, sInfo.strPath);
	voffset = GetPrivateProfileIntA("CAMERA", "voffset", 0, sInfo.strPath);
	hwidth = GetPrivateProfileIntA("CAMERA", "hwidth", 0, sInfo.strPath);
	vheight = GetPrivateProfileIntA("CAMERA", "vheight", 0, sInfo.strPath);
	int show = GetPrivateProfileInt("CAMERA", "show", 1, sInfo.strPath);
	if (show == 1)
	{
		if (strcmp(method, "xt") == 0)
		{
			m_RGainCtrl.SetRange(0, 510);			//红色
			m_GGainCtrl.SetRange(0, 510);			//绿色
			m_BGainCtrl.SetRange(0, 510);			//蓝色
			m_ACtrl.SetRange(0, 15);//亮度
			m_SliderTest.SetRange(0, 10);
			
			TUCAM_INIT itApi; // 初始化 SDK 环境参数
			itApi.pstrConfigPath = NULL;
			itApi.uiCamCount = 0;
			if (TUCAMRET_SUCCESS != TUCAM_Api_Init(&itApi))
			{
				AfxMessageBox("相机打开失败");
			}
			if (0 == itApi.uiCamCount)
			{
				AfxMessageBox("相机打开失败");
			}
			opCam.hIdxTUCam = 0;
			opCam.uiIdxOpen = 0;
			if (TUCAMRET_SUCCESS != TUCAM_Dev_Open(&opCam))
			{
				AfxMessageBox("相机打开失败");
			}
			else
			{

				// 设置相机ROI, 注意：默认分辨率是1920*1080
				// 目前选择的是这个分辨率，后续要根据实际像素截取，调用相机API
				// 宽度高度, 后续更改为动态获取
				int width = 1920, height = 1080;

				int halfW = width / 2;
				int halfH = height / 2;
				TUCAM_ROI_ATTR attr;
				attr.bEnable = TRUE;
				attr.nHOffset = (halfW / 10) * m_leftRoi;
				attr.nVOffset = (halfH / 10) * m_topRoi;
				attr.nWidth = width - attr.nHOffset - (halfH / 10) * m_rightRoi;
				attr.nHeight = height - attr.nVOffset -(halfH / 10) * m_rightRoi;

				
				attr.nHOffset = attr.nHOffset - attr.nHOffset % 4;
				attr.nVOffset = attr.nVOffset - attr.nVOffset % 4;
				attr.nWidth = attr.nWidth - attr.nWidth % 4;
				attr.nHeight = attr.nHeight - attr.nHeight % 4;

				
				if (TUCAMRET_SUCCESS != TUCAM_Cap_SetROI(opCam.hIdxTUCam, attr))
				{
					::OutputDebugStringA("相机截取失败！");
				}
				//初始化
				TUCAM_PROP_ATTR attrProp;
				attrProp.nIdxChn = 0; // 当前通道 8. 
				attrProp.idProp = TUIDP_EXPOSURETM;
				TUCAM_Capa_SetValue(opCam.hIdxTUCam, TUIDC_ATWBALANCE, 2);
				TUCAM_Prop_GetAttr(opCam.hIdxTUCam, &attrProp);  // 曝光时间范围 13.
				attrProp.dbValMin; // 最小曝光时间 14. 
				attrProp.dbValMax; // 最大曝光时间 15. 
				m_ExpTimeCtrl.SetRange(attrProp.dbValMin, attrProp.dbValMax);//设置曝光
				m_Gamma.SetRange(0, 63);
				m_Contrast.SetRange(0, 15);
				m_SaturationCtrl.SetRange(0, 15);
				InitXTCameraData(0);
				// 分配相机内存
				TUCAM_DRAW_INIT init;
				init.hWnd = cvhWnd;
				init.nHeight = rc.Height();
				init.nWidth = rc.Width();
				init.nMode = TUDRAW_DFT;
				init.ucChannels = 3;

				m_tf.pBuffer = NULL;
				m_tf.ucFormat = TUFRM_FMT_RGB888;
				m_tf.uiRsdSize = 1;
				if (TUCAMRET_SUCCESS != TUCAM_Buf_Alloc(opCam.hIdxTUCam, &m_tf))
				{
					AfxMessageBox("相机内存分配失败");
				}
				else
				{
					if (TUCAMRET_SUCCESS != TUCAM_Cap_Start(opCam.hIdxTUCam, TUCCM_SEQUENCE))
					{
						AfxMessageBox("相机播放失败");	
					}
					
					// 绘制初始失败
					if (TUCAMRET_SUCCESS != TUCAM_Draw_Init(opCam.hIdxTUCam, init))
					{
						AfxMessageBox("相机播放失败");
					}
					else
					{
						m_waitFrame = TRUE;
						m_xtAcquireThreadHandle = AfxBeginThread(Acquire_XT_Frame, this);
						Sleep(1000);
						
					}
				}
			}
		}
		else if (strcmp(method, "opencv") == 0)
		{
			// 继续使用opencv方式进行播放截取
			// 如果是自用，则要弹出配置界面, 自用要添加一个宏

			// 1为自用, 自用的显示调节窗口
			int selfUse = GetPrivateProfileIntA("CAMERA", "self_use", 0, sInfo.strPath);
			if (selfUse == 1)
			{
				if (!m_opencvCameraConfig)
				{
					m_opencvCameraConfig = new OpencvCameraConfig;
					m_opencvCameraConfig->Create(IDD_DIALOG6, this);
				}
				m_opencvCameraConfig->ShowWindow(SW_SHOW);
			}
		}
		else
		{
			// 隐藏底层视频窗口
			::ShowWindow(hWnd, SW_HIDE);//  不显示hParent

			// 显示截取窗口
			// m_externDlg.Create(IDD_ExternDlg, 0);
			// m_externDlg.ShowWindow(SW_SHOW);
			cv::namedWindow("picView2", cv::WINDOW_NORMAL);
			cv::resizeWindow("picView2", rc.Width(), rc.Height());// cv窗口名为"picView" lcz new 03/21  调整窗口尺寸，避免采集的图像范围过大导致最后的视频窗口的图像不居中
			HWND hWnd2 = (HWND)cvGetWindowHandle("picView2");//cv窗口句柄为hWnd
			HWND hParent = ::GetParent(hWnd2);//  cv窗口句柄hWnd的父句柄为hParent
			::SetParent(hWnd2, GetDlgItem(IDC_STATIC_VIDEO)->m_hWnd);
			::ShowWindow(hParent, SW_HIDE);//  不显示hParent


			// 按照DEMO设置死的
			// CameraSetROI(640, 480, 1280, 1024);
			CameraSetROI(hoffset, voffset, hwidth, vheight);
			// 尝试加载500E
			DI_CAMERA_STATUS status = CameraInit(MY_DI_SNAP_PROC, R640_480, hWnd, 1, 0);
			if (status == STATUS_OK)
			{
				{
					INT width = 0;
					INT hight = 0;
					CameraGetImageSize(&width, &hight);
					CameraSetWBWindow(width >> 2, hight >> 2, width >> 1, hight >> 1);
				}
				CameraSetMessage(this->m_hWnd, WM_USERDEFMSG);
				m_500EInitStatus = TRUE;
				status = CameraPlay();
				if (status == STATUS_OK)
				{
					SetTimer(99, 100, 0);
					m_500EPlayStatus = TRUE;
				}
			}
		}
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CMiniCamera::OnClose()
{
	WriteToLog("LOGLED01--Onclose");
	CDialogEx::OnClose();
	
	int nself=GetPrivateProfileIntA("CAMERA", "self_use", 0, sInfo.strPath);
	CString method;
	GetPrivateProfileStringA("CAMERA", "method", "opencv", method.GetBuffer(1024), 1024, sInfo.strPath);
	if (nself==0&& method=="xt")
	{
		InitXTCameraData(1);
	}

	// 停止xt相机图像
	m_waitFrame = FALSE;
	if (m_xtAcquireThreadHandle)
	{
		m_xtAcquireThreadHandle->SuspendThread();
		delete m_xtAcquireThreadHandle;
	}
	CINScanLiteDlg * pDlg = (CINScanLiteDlg*)AfxGetApp()->GetMainWnd();

	// 点击关闭相机时，发送“开始能指令”开启激光器使能。
	if (sCInfo.Laser - 532 < 10)
	{
	}
	else
	{
		// 785相机
		pDlg->m_DlgSet.m_Serial.OpenLaserPower(sSInfo.nCOM);
	}

	// 关闭窗口的时候就关闭相机
	if (m_500EPlayStatus)
	{
		CameraStop();
		m_500EPlayStatus = FALSE;
	}

	if (m_500EInitStatus)
	{
		CameraUnInit();
		m_500EInitStatus = TRUE;
	}

	if (m_opencvCameraConfig)
	{
		m_opencvCameraConfig->DestroyWindow();
		delete m_opencvCameraConfig;
		m_opencvCameraConfig = NULL;
	}

	// 尝试关闭XT相机
	{
		// 绘制反初始
		if (TUCAM_Draw_Uninit(opCam.hIdxTUCam) == TUCAMRET_SUCCESS)
		{
			// 播放视频
			TUCAM_Cap_Stop(opCam.hIdxTUCam);
			// 回收内存
			TUCAM_Buf_Release(opCam.hIdxTUCam);
			// 关闭相机
			TUCAM_Dev_Close(opCam.hIdxTUCam);
			// 反初始化
			TUCAM_Api_Uninit();
		}
	}

	flag = false;//  点击关闭按钮，置flag为false以停止刷新视频
	Sleep(500);

	//pDlg->m_DlgSet.m_Serial.OpenCameraLigth(sSInfo.nCOM,"OFF");
	pDlg->m_DlgSet.m_SerialLed.MoveLed(false);

	// lcz new 07/21 
	Sleep(100);
	if (m_bAutoRunningStatue==FALSE)
	{
	   pDlg->m_DlgSet.m_Serial.CloseLaserPower(sSInfo.nCOM);

	}
	stopline=true;
	//m_cap.stoprun();
	
//	OnBnClickedClose();
	DestroyWindow();
	m_hWnd = NULL;

	//Sleep(10000);// 等待电机复位
	// XSleep(3000);
	Sleep(3000);

	g_bOpenCamera = false;
	g_bCameraOpen2Close = true;// 表示相机从打开到关闭，这时候需要清空次数据
	// 如果不是正在连续测试，则将采谱按钮变为可用
	if (m_bAutoRunning == FALSE)
	{
		pDlg->m_DlgTest.m_DlgTestCheckJ.GetDlgItem(IDC_BUTTON_D)->EnableWindow(TRUE);
		pDlg->m_DlgTest.m_DlgTestCheckJ.GetDlgItem(IDC_BUTTON_L)->EnableWindow(TRUE);
	}
	// lcz new 22/10/11 连续测试中打开相机，然后关闭相机，间隔采样按钮不可用---修复该bug
	pDlg->m_DlgTest.m_DlgTestCheckK.GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);
}


HCURSOR CMiniCamera::OnQueryDragIcon()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	return static_cast<HCURSOR>(m_hIcon);
}


void CMiniCamera::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	 //TODO: 在此处添加消息处理程序代码
	 //不为绘图消息调用 CDialogEx::OnPaint()
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
		CDialogEx::OnPaint();
	}
	// 非500E动态库方式则使用opencv进行播放
	if (!m_500EInitStatus)
	{
		// 如果找到并成功打开摄像头才进行播放，没找到播放的话会卡巴斯基
		if (g_bFindCamera)
		{
			OnBnClickedButtonPause();
		}
	}
}

//  鼠标移动-----左键按住且移动
void CMiniCamera::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (mousedown==TRUE)
	{
		CRect Prect1;          //定义图片的矩形
		GetDlgItem(IDC_SHOW_IMG)->GetWindowRect(&Prect1);    //得到图片的矩//形大小
		ScreenToClient(&Prect1);   //将图片框的绝对矩形大小
		if (point.x<Prect1.left||point.x>Prect1.right||point.y<Prect1.top||point.y>Prect1.bottom)
		{
			//MessageBox("chujie");
			return;
		}

		x_start.x=Prect1.left;
		x_start.y=point.y;
		x_end.x =Prect1.right;
		x_end.y =point.y;

		y_start.x = point.x;
		y_start.y = Prect1.top;
		y_end.x = point.x;
		y_end.y = Prect1.bottom;
	}
	
	CDialogEx::OnMouseMove(nFlags, point);
}

//  画横竖两条红线（十字光标）
void CMiniCamera::drowline()
{
	//  lcz 待检验是否会影响相机卡顿时十字光标的绘制问题
	while(1)
	{
		CPen pen(1,1,RGB(255,0,0));
		CClientDC dc(this);
		CPen *pOldPen=dc.SelectObject(&pen);
		dc.MoveTo(x_start);
		dc.LineTo(x_end);
		//dc.SelectObject(pOldPen);
		dc.MoveTo(y_start);
		dc.LineTo(y_end);
		dc.SelectObject(pOldPen);
		Sleep(2);
		if (stopline==true)
		{
			break;
		}
	}

}

//  左键抬起，更新x_start、x_end、y_start、y_end的值
void CMiniCamera::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	//mousedown=FALSE;
	//CRect Prect1;          //定义图片的矩形
	//GetDlgItem(IDC_SHOW_IMG)->GetWindowRect(&Prect1);    //得到图片的矩//形大小
	//ScreenToClient(&Prect1);   //将图片框的绝对矩形大小
	//if (point.x<Prect1.left||point.x>Prect1.right||point.y<Prect1.top||point.y>Prect1.bottom)
	//{
	//	return;
	//}
	//x_start.x=Prect1.left;
	//x_start.y=point.y;
	//x_end.x =Prect1.right;
	//x_end.y =point.y;

	//y_start.x = point.x;
	//y_start.y = Prect1.top;
	//y_end.x = point.x;
	//y_end.y = Prect1.bottom;
	//
	//// lcz new 05/23 记录当前点的位置
	//sTInfo.CrossX = point.x;
	//sTInfo.CrossY = point.y;

	CDialogEx::OnLButtonUp(nFlags, point);
}

//  双击鼠标左键
void CMiniCamera::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	//mousedown=TRUE;
	CDialogEx::OnLButtonDblClk(nFlags, point);
}

//  左键按下，停止重绘十字光标
void CMiniCamera::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	//mousedown=TRUE;
	//stopline=true;
	//CRect Prect1;          //定义图片的矩形
	//GetDlgItem(IDC_SHOW_IMG)->GetWindowRect(&Prect1);    //得到图片的矩//形大小
	//ScreenToClient(&Prect1);   //将图片框的绝对矩形大小
	//if (point.x<Prect1.left||point.x>Prect1.right||point.y<Prect1.top||point.y>Prect1.bottom)
	//{
	//	return;
	//}
	//x_start.x=Prect1.left;
	//x_start.y=point.y;
	//x_end.x =Prect1.right;
	//x_end.y =point.y;
	//
	//y_start.x = point.x;
	//y_start.y = Prect1.top;
	//y_end.x = point.x;
	//y_end.y = Prect1.bottom;
	//WriteToLog("*******鼠标左键按下******");// lcz new 01/31
	CDialogEx::OnLButtonDown(nFlags, point);
}

// 截获消息的。可以通过重载它来处理键盘和鼠标消息
BOOL CMiniCamera::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	//HWND h1 = GetDlgItem(IDC_SHOW_IMG)->GetSafeHwnd();
	//HWND h2 = GetDlgItem(IDC_STATIC_VIDEO)->GetSafeHwnd();
	//HWND h3 = GetDlgItem(IDD_MINI_CAMERA)->GetSafeHwnd();
	//hWnd1;
	//hParent2;
	if (GetDlgItem(IDC_SHOW_IMG)->GetSafeHwnd() == pMsg->hwnd &&
		pMsg->message == WM_LBUTTONDOWN)
	{
	//if(pMsg->message == WM_LBUTTONDOWN)
	//{
		OnLButtonDown(MK_LBUTTON, pMsg->pt);
		return TRUE;
	}
	if (GetDlgItem(IDC_SHOW_IMG)->GetSafeHwnd() == pMsg->hwnd &&
		pMsg->message == WM_MOUSEMOVE)
	{
		OnMouseMove(MK_MBUTTON, pMsg->pt);
		return TRUE;
	}
	if (GetDlgItem(IDC_SHOW_IMG)->GetSafeHwnd() == pMsg->hwnd &&
		pMsg->message == WM_LBUTTONUP)
	{
		OnLButtonUp(MK_LBUTTON, pMsg->pt);
		return TRUE;
	}
	if (GetDlgItem(IDC_SHOW_IMG)->GetSafeHwnd() == pMsg->hwnd &&
		pMsg->message == WM_RBUTTONDOWN)
	{
		OnRButtonDown(MK_RBUTTON, pMsg->pt);
		return TRUE;
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}


void CMiniCamera::OnStnClickedShowImg()
{
	// TODO: 在此添加控件通知处理程序代码
	

}


void CMiniCamera::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	//CDialogEx::OnOK();
}


void CMiniCamera::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	//CDialogEx::OnCancel();
}


BOOL CMiniCamera::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	RECT   rect; 
	CWnd* pWnd = GetDlgItem(IDC_SHOW_IMG);
	pWnd->GetWindowRect(&rect); 
	ScreenToClient(&rect); 
	pDC-> ExcludeClipRect(&rect); 
	//return   CDialog::OnEraseBkgnd(pDC);
	return CDialogEx::OnEraseBkgnd(pDC);
}

void CMiniCamera::ReSize(void)  
{  
	float fsp[2];  
	POINT Newp; //获取现在对话框的大小  
	CRect recta;      
	GetClientRect(&recta);     //取客户区大小    
	Newp.x=recta.right-recta.left;  
	Newp.y=recta.bottom-recta.top;  
	fsp[0]=(float)Newp.x/old.x;  
	fsp[1]=(float)Newp.y/old.y;  
	CRect Rect;  
	int woc;  
	CPoint OldTLPoint,TLPoint; //左上角  
	CPoint OldBRPoint,BRPoint; //右下角  
	HWND  hwndChild=::GetWindow(m_hWnd,GW_CHILD);  //列出所有控件    
	while(hwndChild){      
		woc=::GetDlgCtrlID(hwndChild);//取得ID  
		GetDlgItem(woc)->GetWindowRect(Rect);    
		ScreenToClient(Rect);    
		OldTLPoint = Rect.TopLeft();    
		TLPoint.x = 0;    
		TLPoint.y = 0;    
		OldBRPoint = Rect.BottomRight();    
		BRPoint.x = Newp.x;    
		BRPoint.y = Newp.y;    
		Rect.SetRect(TLPoint,BRPoint);    
		GetDlgItem(woc)->MoveWindow(Rect,TRUE);  
		//ChangeSize(woc,Rect.top,Rect.left);
		m_cap.ResizeVideoWindow();


		x_start.x=0;
		x_start.y=int(1.0*x_start.y*Newp.y/old.y+0.45);
		x_end.x =Newp.x;
		x_end.y =int(1.0*x_end.y*Newp.y/old.y+0.45);

		y_start.x = int(1.0*y_start.x*Newp.x/old.x+0.45);
		y_start.y =0;
		y_end.x = int(1.0*y_end.x*Newp.x/old.x+0.45);
		y_end.y = Newp.y;

		hwndChild=::GetWindow(hwndChild, GW_HWNDNEXT);      
	}
	old=Newp;  
}  

void CMiniCamera::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);
	if (nType!=SIZE_MINIMIZED )
	{
		if (!m_500EInitStatus)
		{
			ReSize();
		}	
	}
}


void CMiniCamera::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	stopline=true;
	CRect Prect1;          //定义图片的矩形
	GetDlgItem(IDC_SHOW_IMG)->GetWindowRect(&Prect1);    //得到图片的矩//形大小
	ScreenToClient(&Prect1);   //将图片框的绝对矩形大小

	x_start.x = Prect1.left;
	x_start.y = Prect1.bottom / 2;
	x_end.x  = Prect1.right;
	x_end.y  = Prect1.bottom / 2;

	y_start.x = Prect1.right / 2;
	y_start.y = Prect1.top;
	y_end.x = Prect1.right / 2;
	y_end.y = Prect1.bottom;
	// lcz new 05/23 记录当前点的位置
	sTInfo.CrossX = y_start.x;
	sTInfo.CrossY = x_start.y;

	CDialogEx::OnRButtonDown(nFlags, point);
}

// lcz new 02/02 开始采集影像
void CMiniCamera::OnBnClickedButtonPause()
{
	// TODO: 在此添加控件通知处理程序代码
	CINScanLiteDlg * pDlgT = (CINScanLiteDlg*)AfxGetApp()->GetMainWnd();//  lcz new 02/06

	flag = true;
	while (flag)
	{
		pDlgT->cap >> cam1;//获取当前帧图像
		//WriteToLog("采集了一帧");// lcz new 02/06

		// 进行截取操作
		int width = cam1.cols;
		int height = cam1.rows;

		if (width == 0 || height == 0)
		{
			continue;
		}

		int halfW = width / 2;
		int halfH = height / 2;

		int left = (halfW / 10) * m_leftRoi;
		int top = (halfH / 10) * m_topRoi;
		int right = width - (halfW / 10) * m_rightRoi;
		int bottom = height - (halfH / 10) * m_bottomRoi;

		if (cam1.empty())
		{
			continue;
		}

		imshow("picView", cam1(Range(top, bottom), Range(left, right)) );//显示当前帧图像
		waitKey(30);//延时30ms
		if (!g_bFindCamera)
		{
			break;
		}
	}
}

//  lcz new 02/02 暂停影像
void CMiniCamera::OnBnClickedButtonRun()
{
	// TODO: 在此添加控件通知处理程序代码
//	FILTER_STATE temp;
////typedef enum _FilterState {
////  State_Stopped = 0,
////  State_Paused,
////  State_Running
////} FILTER_STATE;
//	m_cap. m_pBF->GetState(1000,  &temp);
//	//WriteToLog("*******鼠标左键按下******");// lcz new 01/31
//	CString t1; 
//	t1.Format(_T("%d"), temp);
//	AfxMessageBox(t1);
//
//	m_cap. m_pMC->Run();

	//if (flag)
	//{
	//	flag = false;
	//	GetDlgItem(IDC_BUTTON_RUN)->SetWindowTextA(TEXT("继续影像"));
	//}
	//else
	//{
	//	GetDlgItem(IDC_BUTTON_RUN)->SetWindowTextA(TEXT("暂停影像"));
	//	flag = true;
	//}

	flag = false;
}


void CMiniCamera::OnNMCustomdrawSlider2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	UpdateColorGain();
	*pResult = 0;
}
// 更新颜色 
void CMiniCamera::UpdateColorGain()
{
	TUCAM_Capa_SetValue(opCam.hIdxTUCam, TUIDC_ATWBALANCE, 0);
	USHORT RGain = m_RGainCtrl.GetPos();/* 红色*/
	USHORT GGain = m_GGainCtrl.GetPos();/* 绿色*/
	USHORT BGain = m_BGainCtrl.GetPos();/* 蓝色*/

	CString sTemp;
	sTemp.Format(_T("%5.2f"), GGain / 2.0f);
	GetDlgItem(IDC_STATIC_G)->SetWindowText(sTemp);

	sTemp.Format(_T("%5.2f"), RGain / 2.0f);
	GetDlgItem(IDC_STATIC_R)->SetWindowText(sTemp);

	sTemp.Format(_T("%5.2f"), BGain / 2.0f);
	GetDlgItem(IDC_STATIC_B)->SetWindowText(sTemp);



	TUCAM_Prop_SetValue(opCam.hIdxTUCam, TUIDP_CHNLGAIN, (m_RGainCtrl.GetPos() * 1.0f), 1);
	TUCAM_Prop_SetValue(opCam.hIdxTUCam, TUIDP_CHNLGAIN, (m_GGainCtrl.GetPos() * 1.0f), 2);
	TUCAM_Prop_SetValue(opCam.hIdxTUCam, TUIDP_CHNLGAIN, (m_BGainCtrl.GetPos() * 1.0f), 3);

}

void CMiniCamera::OnNMCustomdrawSlider3(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	UpdateColorGain();
	*pResult = 0;
}


void CMiniCamera::OnNMCustomdrawSlider4(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	UpdateColorGain();
	*pResult = 0;
}

void CMiniCamera::UpdateExposurteTimeText()
{
	double RawTime;
	CString sTemp;

	//CameraGetRowTime(&RawTime);
	TUCAM_Prop_GetValue(opCam.hIdxTUCam, TUIDP_EXPOSURETM, &RawTime);

	sTemp.Format(_T("%.3f ms"), (float)RawTime);//
												//	sTemp.Format("%d",m_ExpTime);//
	GetDlgItem(IDC_STATIC_TIME)->SetWindowText(sTemp);
}


void CMiniCamera::OnNMCustomdrawSlider1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	int pos;
	pos = m_ExpTimeCtrl.GetPos();
	TUCAM_Prop_SetValue(opCam.hIdxTUCam, TUIDP_EXPOSURETM, pos);

	UpdateExposurteTimeText();
	UpdateData(TRUE);
	*pResult = 0;
}


void CMiniCamera::OnBnClickedButton1()
{
	TUCAM_Capa_SetValue(opCam.hIdxTUCam, TUIDC_ATWBALANCE, 2);
	double RGain, GGain, BGain;
	Sleep(1000);
	TUCAM_Prop_GetValue(opCam.hIdxTUCam, TUIDP_CHNLGAIN, &RGain, 1);
	TUCAM_Prop_GetValue(opCam.hIdxTUCam, TUIDP_CHNLGAIN, &GGain, 2);
	TUCAM_Prop_GetValue(opCam.hIdxTUCam, TUIDP_CHNLGAIN, &BGain, 3);

	CString sTemp;
	sTemp.Format(_T("%5.2f"), GGain / 2.0f);
	GetDlgItem(IDC_STATIC_G)->SetWindowText(sTemp);

	sTemp.Format(_T("%5.2f"), RGain / 2.0f);
	GetDlgItem(IDC_STATIC_R)->SetWindowText(sTemp);

	sTemp.Format(_T("%5.2f"), BGain / 2.0f);
	GetDlgItem(IDC_STATIC_B)->SetWindowText(sTemp);

	m_RGainCtrl.SetPos((int)(RGain));
	m_GGainCtrl.SetPos((int)(GGain));
	m_BGainCtrl.SetPos((int)(BGain));
}


void CMiniCamera::OnBnClickedCheck1()
{

	UpdateData(TRUE);
	
	TUCAM_Capa_SetValue(opCam.hIdxTUCam, TUIDC_ATEXPOSURE, m_Aeen);


	GetDlgItem(IDC_SLIDER1)->EnableWindow(!m_Aeen);			//时间
	GetDlgItem(IDC_STATIC_TIME)->EnableWindow(!m_Aeen);		//时间标签
	GetDlgItem(IDC_SLIDER8)->EnableWindow(m_Aeen);			//时间
	GetDlgItem(IDC_STATIC_TIME2)->EnableWindow(m_Aeen);		//时间标签

	if (!m_Aeen) {
		CString sTemp;
		double iTemp;
		//CameraGetExposureTime(&iTemp);

		TUCAM_Prop_GetValue(opCam.hIdxTUCam, TUIDP_EXPOSURETM, &iTemp);
		UpdateExposurteTimeText();

		m_ExpTimeCtrl.SetPos((int)(iTemp));
		USHORT uiAnalogGain;

	}

	////

}


void CMiniCamera::OnNMCustomdrawSlider5(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	UpdateData(TRUE);
	int nGamma= m_Gamma.GetPos();;
	TUCAM_Prop_SetValue(opCam.hIdxTUCam, TUIDP_GAMMA, nGamma);
	CString sTemp;
	sTemp.Format(_T("%4.2f"), nGamma / 100.0f);
	GetDlgItem(IDC_STATIC_R2)->SetWindowText(sTemp);
	*pResult = 0;
}


void CMiniCamera::OnNMCustomdrawSlider6(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	int ContrastValue = m_Contrast.GetPos();
	TUCAM_Prop_SetValue(opCam.hIdxTUCam, TUIDP_CONTRAST, ContrastValue);
	CString sTemp;
	sTemp.Format(_T("%d"), ContrastValue);
	GetDlgItem(IDC_STATIC_G2)->SetWindowText(sTemp);
}


void CMiniCamera::OnNMCustomdrawSlider7(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	int SaturationValue = m_SaturationCtrl.GetPos(); {
		//m_pCommUsb->SetSaturation(SaturationValue);
		TUCAM_Prop_SetValue(opCam.hIdxTUCam, TUIDP_SATURATION, SaturationValue);
		CString sTemp;
		sTemp.Format(_T("%d"), SaturationValue);
		GetDlgItem(IDC_STATIC_B2)->SetWindowText(sTemp);
	}
	*pResult = 0;
}

void CMiniCamera::InitXTCameraData(int nMode)//0初始化 1写入
{
	if(nMode==0)
	{
		
		CString str; float fData; int nData;
		GetPrivateProfileStringA("TX", "time", "9", str.GetBuffer(1024), 1024, sInfo.strPath);
		TUCAM_Prop_SetValue(opCam.hIdxTUCam, TUIDP_EXPOSURETM, atoi(str));
		m_ExpTimeCtrl.SetPos(atoi(str));
		GetDlgItem(IDC_STATIC_TIME)->SetWindowText(str);

		GetPrivateProfileStringA("TX", "a", "9", str.GetBuffer(1024), 1024, sInfo.strPath);
		TUCAM_Prop_SetValue(opCam.hIdxTUCam, TUIDP_BRIGHTNESS, atoi(str));
		m_ACtrl.SetPos(atoi(str));
		GetDlgItem(IDC_STATIC_TIME2)->SetWindowText(str);

	
		GetPrivateProfileStringA("TX", "r", "0", str.GetBuffer(1024), 1024, sInfo.strPath);
		//TUCAM_Prop_SetValue(opCam.hIdxTUCam, TUIDP_CHNLGAIN, (atof(str) * 1.0f), 1);
		m_RGainCtrl.SetPos(atof(str) *2);
		GetDlgItem(IDC_STATIC_R)->SetWindowText(str);


		GetPrivateProfileStringA("TX", "g", "0", str.GetBuffer(1024), 1024, sInfo.strPath);
		//TUCAM_Prop_SetValue(opCam.hIdxTUCam, TUIDP_CHNLGAIN, (atof(str) * 1.0f), 2);
		m_GGainCtrl.SetPos(atof(str) * 2);
		GetDlgItem(IDC_STATIC_G)->SetWindowText(str);


		GetPrivateProfileStringA("TX", "b", "0", str.GetBuffer(1024), 1024, sInfo.strPath);
		//TUCAM_Prop_SetValue(opCam.hIdxTUCam, TUIDP_CHNLGAIN, (atof(str) * 1.0f), 3);
		m_BGainCtrl.SetPos(atof(str) * 2);
		GetDlgItem(IDC_STATIC_B)->SetWindowText(str);


	
		GetPrivateProfileStringA("TX", "gamma", "0.1", str.GetBuffer(1024), 1024, sInfo.strPath);
		TUCAM_Prop_SetValue(opCam.hIdxTUCam, TUIDP_GAMMA, atof(str));

		m_Gamma.SetPos(atof(str)*100);
		CString sTemp;
		sTemp.Format(_T("%4.2f"), atof(str));
		GetDlgItem(IDC_STATIC_R2)->SetWindowText(sTemp);

		GetPrivateProfileStringA("TX", "contrast", "8", str.GetBuffer(1024), 1024, sInfo.strPath);
		TUCAM_Prop_SetValue(opCam.hIdxTUCam, TUIDP_CONTRAST, atof(str));
		m_Contrast.SetPos(atof(str));
		GetDlgItem(IDC_STATIC_G2)->SetWindowText(str);

		GetPrivateProfileStringA("TX", "saturation", "5", str.GetBuffer(1024), 1024, sInfo.strPath);
		TUCAM_Prop_SetValue(opCam.hIdxTUCam, TUIDP_SATURATION, atof(str));
		m_SaturationCtrl.SetPos(atof(str)); 
		GetDlgItem(IDC_STATIC_B2)->SetWindowText(str);

		m_SliderTest.SetPos(5);
		

	}
	else
	{
		CString strResult;
		GetDlgItem(IDC_STATIC_TIME)->GetWindowText(strResult);
		WritePrivateProfileString("TX","time",  strResult, sInfo.strPath);
		GetDlgItem(IDC_STATIC_TIME2)->GetWindowText(strResult);
		WritePrivateProfileString("TX", "a", strResult, sInfo.strPath);
		GetDlgItem(IDC_STATIC_R)->GetWindowText(strResult);
		WritePrivateProfileString("TX","r",  strResult, sInfo.strPath);
		GetDlgItem(IDC_STATIC_G)->GetWindowText(strResult);
		WritePrivateProfileString("TX","g",  strResult, sInfo.strPath);
		GetDlgItem(IDC_STATIC_B)->GetWindowText(strResult);
		WritePrivateProfileString( "TX","b", strResult, sInfo.strPath);
		GetDlgItem(IDC_STATIC_R2)->GetWindowText(strResult);
		WritePrivateProfileString("TX","gamma",  strResult, sInfo.strPath);
		GetDlgItem(IDC_STATIC_G2)->GetWindowText(strResult);
		WritePrivateProfileString("TX","contrast",  strResult, sInfo.strPath);
		GetDlgItem(IDC_STATIC_B2)->GetWindowText(strResult);
		WritePrivateProfileString( "TX","saturation", strResult, sInfo.strPath);
	}

}

void CMiniCamera::OnNMCustomdrawSlider8(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	int pos;
	pos = m_ACtrl.GetPos();
	TUCAM_Prop_SetValue(opCam.hIdxTUCam, TUIDP_BRIGHTNESS, pos);
	/*UpdateExposurteTimeText();
	UpdateData(TRUE);*/

	double RawTime;
	CString sTemp;

	//CameraGetRowTime(&RawTime);
	TUCAM_Prop_GetValue(opCam.hIdxTUCam, TUIDP_BRIGHTNESS, &RawTime);

	sTemp.Format(_T("%.1f"), RawTime);//
												//	sTemp.Format("%d",m_ExpTime);//
	GetDlgItem(IDC_STATIC_TIME2)->SetWindowText(sTemp);
	*pResult = 0;
}


void CMiniCamera::OnNMCustomdrawSlider9(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	TUCAM_Capa_SetValue(opCam.hIdxTUCam, TUIDC_ATWBALANCE, 2);
	*pResult = 0;
}
