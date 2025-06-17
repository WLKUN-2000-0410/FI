//-------------------------------------------------------------------
// CCaptureVideo视频捕捉类实现文件CaptureVideo.cpp
//-------------------------------------------------------------------
// CaptureVideo.cpp: implementation of the CCaptureVideo class.
//
/////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "CaptureVideo.h"
#include "INScan-LiteDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define FILE_PATH "D:\\FILog.txt" //信息输出文件 lcz new 01/31

BOOL bOneShot=FALSE;//全局变量 
CString szFileName;/// 位图文件名称 
class CSampleGrabberCB : public ISampleGrabberCB 
{
public:
	long lWidth;
	long lHeight;
	CString m_szFileName;// 位图文件名称
	CSampleGrabberCB( )
	{
	
	} 
	STDMETHODIMP_(ULONG) AddRef() { return 2; }
	STDMETHODIMP_(ULONG) Release() { return 1; }
	STDMETHODIMP QueryInterface(REFIID riid, void ** ppv)
	{
		if( riid == IID_ISampleGrabberCB || riid == IID_IUnknown )
		{ 
			*ppv = (void *) static_cast<ISampleGrabberCB*> ( this );
			return NOERROR;
		} 
		return E_NOINTERFACE;
	}
	STDMETHODIMP SampleCB( double SampleTime, IMediaSample * pSample )
	{
		return 0;
	}
	STDMETHODIMP BufferCB( double dblSampleTime, BYTE * pBuffer, long lBufferSize )
	{
		if( !bOneShot )return 0;
		if (!pBuffer)return E_POINTER;
		//strcpy(m_szFileName, szFileName);
		m_szFileName = szFileName;
		SaveBitmap(pBuffer, lBufferSize);
		bOneShot = FALSE;
		return 0;
	}
	//创建位图文件
	BOOL SaveBitmap(BYTE * pBuffer, long lBufferSize )
	{
		HANDLE hf = CreateFile(
		m_szFileName, GENERIC_WRITE, FILE_SHARE_READ, NULL,
		CREATE_ALWAYS, NULL, NULL );
		if( hf == INVALID_HANDLE_VALUE )return 0;
		// 写文件头 
		BITMAPFILEHEADER bfh;
		memset( &bfh, 0, sizeof( bfh ) );
		bfh.bfType ='MB';
		bfh.bfSize = sizeof( bfh ) + lBufferSize + sizeof( BITMAPINFOHEADER );
		bfh.bfOffBits = sizeof( BITMAPINFOHEADER ) + sizeof( BITMAPFILEHEADER );
		DWORD dwWritten = 0;
		WriteFile( hf, &bfh, sizeof( bfh ), &dwWritten, NULL );
		// 写位图格式
		BITMAPINFOHEADER bih;
		memset( &bih, 0, sizeof( bih ) );
		bih.biSize = sizeof( bih );
		bih.biWidth = lWidth;
		bih.biHeight = lHeight;
		bih.biPlanes = 1;
		bih.biBitCount = 24;
		WriteFile( hf, &bih, sizeof( bih ), &dwWritten, NULL );
		// 写位图数据
		WriteFile( hf, pBuffer, lBufferSize, &dwWritten, NULL );
		CloseHandle( hf );
		return 0;
	}
};
CSampleGrabberCB mCB;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CCaptureVideo::CCaptureVideo()
{
	//COM Library Intialization
	if(FAILED(CoInitialize(NULL))) /*, COINIT_APARTMENTTHREADED)))*/
	{
		AfxMessageBox(_T("CoInitialize Failed!\r\n")); 
		return;
	}
	m_hWnd = NULL;
	m_pVW = NULL;
	m_pMC = NULL;
	m_pGB = NULL;
	m_pCapture = NULL; 
	m_pGrabber= NULL;
	m_pBF = NULL;
}
CCaptureVideo::~CCaptureVideo()
{
	// Stop media playback
	if (m_pMC!=NULL)
	{
		if(m_pMC)m_pMC->Stop();
	}	
	if(m_pVW)
	{
		m_pVW->put_Visible(OAFALSE);
		m_pVW->put_Owner(NULL);
	}
	if(m_pGrabber){srelease(m_pGrabber);}
	if(m_pCapture)srelease(m_pCapture);
	if(m_pMC)srelease(m_pMC);
	if(m_pGB)srelease(m_pGB);
	if(m_pBF)srelease(m_pBF);
	
	CoUninitialize( );
}

// 得到视频捕捉设备的信息，存入hList中
int CCaptureVideo::EnumDevices(HWND hList)
{
	if (!hList)	return -1;
	int id = 0;
	//枚举视频扑捉设备
	ICreateDevEnum *pCreateDevEnum;
	HRESULT hr = CoCreateInstance(CLSID_SystemDeviceEnum, NULL, CLSCTX_INPROC_SERVER,IID_ICreateDevEnum, (void**)&pCreateDevEnum);
	if (hr != NOERROR)return -1;
	CComPtr<IEnumMoniker> pEm;
	hr = pCreateDevEnum->CreateClassEnumerator(CLSID_VideoInputDeviceCategory,&pEm, 0);
	if (hr != NOERROR)return -1;
	pEm->Reset();
	ULONG cFetched;
	IMoniker *pM;
	CINScanLiteDlg * pDlg = (CINScanLiteDlg *)AfxGetApp()->GetMainWnd();// lcz new 05/27
	int index = 0;// lcz new 05/27
	while(hr = pEm->Next(1, &pM, &cFetched), hr==S_OK)
	{
		IPropertyBag *pBag;
		hr = pM->BindToStorage(0, 0, IID_IPropertyBag, (void **)&pBag);
		if(SUCCEEDED(hr)) 
		{
			VARIANT var;
			var.vt = VT_BSTR;
			hr = pBag->Read(L"FriendlyName", &var, NULL);
			if (hr == NOERROR) 
			{
				//LPARAM str;
				TCHAR str[2048]; 
				id++;
				WideCharToMultiByte(CP_ACP,0,var.bstrVal, -1, (LPSTR)str, 2048, NULL, NULL);
				::SendMessage(hList, CB_ADDSTRING, 0,(LPARAM)str);

				CString tempCString;
				tempCString.Format(TEXT("%s"), str);
				pDlg->m_DlgSet.m_vCameraInfo.insert(make_pair(index, tempCString));
				index++;

				SysFreeString(var.bstrVal);
			}
			pBag->Release();
		}
		pM->Release();
	}
	return id;
}

HRESULT CCaptureVideo::Init(int iDeviceID, HWND hWnd)
{
	// lcz new : 为了更加容易建立视频捕捉应用程序，DirectShow提供了一个叫做Capture Graph Builder的对象，
	//Capture Graph Builder提供IcaptureGraphBuilder2接口，该接口可以建立和控制Capture Graph
	HRESULT hr;
	hr = InitCaptureGraphBuilder();//  初始化Capture Graph Builder对象
	if (FAILED(hr))
	{
		AfxMessageBox(_T("Failed to get video interfaces!"));
		return hr;
	}
	//WriteToLog("=======1====.成功初始化Capture Graph Builder对象。");// lcz new 01/31
	// Bind Device Filter. We know the device because the id was passed in
	if(!BindFilter(iDeviceID, &m_pBF))
		return S_FALSE;
	//WriteToLog("=======2====.成功Bind Device Filter Bind Device Filter 绑定设备筛选器");// lcz new 01/31
	//  lcz new : 建立视频捕捉程序，必须首先获取并初始化IcaptureGraphBuilder2接口，然后选择一个适当的视频捕捉设备。
	//  选择好设备后，为该设备创建Capture filter，然后调用AddFilter把Capture filter添加到Filter Graph
	hr = m_pGB->AddFilter(m_pBF, L"Capture Filter");
	//if(SUCCEEDED(hr))  WriteToLog("=======3====.成功AddFilter  m_pBF");// lcz new 01/31
	// hr = m_pCapture->RenderStream(&PIN_CATEGORY_PREVIEW, &MEDIATYPE_Video, 
	// m_pBF, NULL, NULL);
	// create a sample grabber
	//2017hr = m_pGrabber.CoCreateInstance( CLSID_SampleGrabber );
	//  lcz new 01/31  创建一个OutputFilter
	hr = CoCreateInstance( CLSID_SampleGrabber, NULL, CLSCTX_INPROC_SERVER, IID_ISampleGrabber, (void**)&m_pGrabber );// lcz new 01/31 和示例不同需注意
	//if(SUCCEEDED(hr))  WriteToLog("=======4====.成功创建一个OutputFilter");// lcz new 01/31
	if( !m_pGrabber )
	{
		AfxMessageBox(_T("Fail to create SampleGrabber, maybe qedit.dll is not registered?"));
		return hr;
	}
	CComQIPtr< IBaseFilter, &IID_IBaseFilter > pGrabBase( m_pGrabber );
	//设置SampleGrabber的视频格式
	AM_MEDIA_TYPE mt; 
	ZeroMemory(&mt, sizeof(AM_MEDIA_TYPE));
	mt.majortype = MEDIATYPE_Video;
	mt.subtype = MEDIASUBTYPE_RGB24;// lcz new 01/31 这里只是设置输出的图像数据类型（24bit，32bit，...），directshow会对自动的对采集到的图像数据做转换
	hr = m_pGrabber->SetMediaType(&mt);
	//if(SUCCEEDED(hr))  WriteToLog("=======5====.成功设置SampleGrabber的视频格式");// lcz new 01/31
	if( FAILED( hr ) )
	{
		AfxMessageBox(_T("Fail to set media type!"));
		return hr;
	}
	hr = m_pGB->AddFilter( pGrabBase, L"Grabber" );  // lcz new 01/31 将2个Filter添加到GraphBuilder，有一个在上面
	//if(SUCCEEDED(hr))  WriteToLog("=======6====.成功AddFilter  pGrabBase");// lcz new 01/31
	if( FAILED( hr ) )
	{
		AfxMessageBox(_T("Fail to put sample grabber in graph"));
		return hr;
	}

	// try to render preview/capture pin
	// lcz new 01/31  尝试去RenderStream，失败则表示该设备无法使用（比如被其他应用程序所占用）
	hr = m_pCapture->RenderStream(&PIN_CATEGORY_PREVIEW, &MEDIATYPE_Video,m_pBF,pGrabBase,NULL);
	if( FAILED( hr ) )
		hr = m_pCapture->RenderStream(&PIN_CATEGORY_CAPTURE, &MEDIATYPE_Video,m_pBF,pGrabBase,NULL);

	if( FAILED( hr ) )
	{
		AfxMessageBox(_T("Can’t build the graph"));
		return hr;
	}
	//if(SUCCEEDED(hr))  WriteToLog("=======7====.成功RenderStream  递交数据流");// lcz new 01/31
	hr = m_pGrabber->GetConnectedMediaType( &mt );
	if ( FAILED( hr) )
	{
		AfxMessageBox(_T("Failt to read the connected media type"));
		return hr;
	}

	VIDEOINFOHEADER * vih = (VIDEOINFOHEADER*) mt.pbFormat;
	//  将相机读取视频流的分辨率降为一半//  2022/4/22  还原分辨率   可能是硬件线路的原因导致相机卡顿
	mCB.lWidth = vih->bmiHeader.biWidth; //  lcz modify
	mCB.lHeight = vih->bmiHeader.biHeight;
	//  lcz modify

	FreeMediaType(mt);
	// lcz new 01/31 配置SampleGrabber
	hr = m_pGrabber->SetBufferSamples( FALSE );//如果设置true，SampleGrabber会创建一个内部的buffer，可以通过GetCurrentBuffer获得当前帧数据
	//if(SUCCEEDED(hr))  WriteToLog("=======8====.成功SetBufferSamples");// lcz new 01/31
	hr = m_pGrabber->SetOneShot( FALSE );//如果设置成true，表示只会进入callback函数一次
	//if(SUCCEEDED(hr))  WriteToLog("=======9====.成功SetOneShot");// lcz new 01/31
	hr = m_pGrabber->SetCallback( &mCB, 1 );//设置callback，第2个参数如果设置成0，则第一个参数必须是IMediaSample指针
	//if(SUCCEEDED(hr))  WriteToLog("=======10====.成功SetCallback");// lcz new 01/31

	//设置视频捕捉窗口
	m_hWnd = hWnd ; 
	SetupVideoWindow();
	hr =m_pVW->put_MessageDrain((OAHWND)m_hWnd);////高瑀含添加
	//if(SUCCEEDED(hr))  WriteToLog("=======12====.成功m_pVW->put_MessageDrain");// lcz new 01/31
	//m_pVW->put_WindowStyleEx()
	//if(bOneShot)
	//	WriteToLog("=======bOneShot为true");// lcz new 01/31
	//else
	//	WriteToLog("=======bOneShot为false");// lcz new 01/31
	hr = m_pMC->Run();//开始视频捕捉
	if(FAILED(hr))
	{
		AfxMessageBox(_T("Couldn’t run the graph!"));
		return hr;
	}
	//if(SUCCEEDED(hr))  WriteToLog("=======13====.成功捕捉视频");// lcz new 01/31
	//hr = m_pMC->Run();//开始视频捕捉 lcz new 01/31
	return S_OK;
}
bool CCaptureVideo::BindFilter(int deviceId, IBaseFilter **pFilter)
{
	if (deviceId < 0) return false;

	// enumerate all video capture devices
	CComPtr<ICreateDevEnum> pCreateDevEnum;
	HRESULT hr = CoCreateInstance(CLSID_SystemDeviceEnum, NULL, CLSCTX_INPROC_SERVER,
	IID_ICreateDevEnum, (void**)&pCreateDevEnum);
	if (hr != NOERROR) return false;
	CComPtr<IEnumMoniker> pEm;
	hr = pCreateDevEnum->CreateClassEnumerator(CLSID_VideoInputDeviceCategory,&pEm, 0);
	if (hr != NOERROR) return false;
	pEm->Reset();
	ULONG cFetched;
	IMoniker *pM;
	int index = 0;
	while(hr = pEm->Next(1, &pM, &cFetched), hr==S_OK, index <= deviceId)
	{
		IPropertyBag *pBag;
		hr = pM->BindToStorage(0, 0, IID_IPropertyBag, (void **)&pBag);
		if(SUCCEEDED(hr)) 
		{
			VARIANT var;
			var.vt = VT_BSTR;
			hr = pBag->Read(L"FriendlyName", &var, NULL);
			if (hr == NOERROR) 
			{
				if (index == deviceId)
				{
					pM->BindToObject(0, 0, IID_IBaseFilter, (void**)pFilter);
				}
				SysFreeString(var.bstrVal);
			}			
			pBag->Release();
		}
		pM->Release();
		index++;
	}
	return true;
}

HRESULT CCaptureVideo::InitCaptureGraphBuilder()
{
	HRESULT hr;

	// 创建IGraphBuilder接口
	hr=CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC_SERVER, IID_IGraphBuilder, (void **)&m_pGB);
	// 创建ICaptureGraphBuilder2接口
	hr = CoCreateInstance (CLSID_CaptureGraphBuilder2 , NULL, CLSCTX_INPROC,
	IID_ICaptureGraphBuilder2, (void **) &m_pCapture);
	if (FAILED(hr))return hr;
	m_pCapture->SetFiltergraph(m_pGB);
	hr = m_pGB->QueryInterface(IID_IMediaControl, (void **)&m_pMC);
	if (FAILED(hr))return hr;
	hr = m_pGB->QueryInterface(IID_IVideoWindow, (LPVOID *) &m_pVW);
	if (FAILED(hr))return hr;
	return hr;
}
HRESULT CCaptureVideo::SetupVideoWindow()
{
	HRESULT hr;
	hr = m_pVW->put_Owner((OAHWND)m_hWnd);
	if (FAILED(hr))return hr;
	hr = m_pVW->put_WindowStyle(WS_CHILD | WS_CLIPCHILDREN);
	if (FAILED(hr))return hr;
	ResizeVideoWindow();
	hr = m_pVW->put_Visible(OATRUE);
	hr =m_pVW->put_MessageDrain((OAHWND)m_hWnd);////高瑀含添加//lcz new :此方法使应用程序能够响应视频窗口中生成的鼠标和键盘事件。
	//if(SUCCEEDED(hr))  WriteToLog("=======11====.成功SetupVideoWindow");// lcz new 01/31
	return hr;
}
void CCaptureVideo::ResizeVideoWindow()
{
	if (m_pVW)
	{
		//让图像充满整个窗口
		CRect rc;
		::GetClientRect(m_hWnd,&rc);
		m_pVW->SetWindowPosition(0, 0, rc.right, rc.bottom);
	} 
}

void CCaptureVideo::GrabOneFrame(BOOL bGrab)
{
	//CString sFileName; 
	
	//static char BASED_CODE szFilter[] = "Bitmap Files (*.bmp) |*.bmp| All Files (*.*) |*.*||"; 
	
//	CFileDialog ofdFileDialog(FALSE,_T("bmp"),NULL,OFN_LONGNAMES,szFilter, this); 
	
	//if (ofdFileDialog.DoModal() != IDOK) 
	//	return; 
	
	//sFileName = ofdFileDialog.m_ofn.lpstrFile; 
	
	//strcpy(szFileName,sFileName); 
	
 // bOneShot = bGrab;
}
void CCaptureVideo::FreeMediaType(AM_MEDIA_TYPE& mt)
{
	if (mt.cbFormat != 0) 
	{
		CoTaskMemFree((PVOID)mt.pbFormat);
		// Strictly unnecessary but tidier
		mt.cbFormat = 0;
		mt.pbFormat = NULL;
	}
	if (mt.pUnk != NULL) 
	{
		mt.pUnk->Release();
		mt.pUnk = NULL;
	}
} 

void CCaptureVideo::stoprun()
{
	m_pMC->Stop();//开始视频捕捉
}

// lcz new 01/31
int CCaptureVideo::WriteToLog(char* str)
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