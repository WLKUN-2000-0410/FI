/////////////////////////////////////////////////////////////////////
#if !defined(AFX_CAPTUREVIDEO_H__F5345AA4_A39F_4B07_B843_3D87C4287AA0__INCLUDED_)
#define AFX_CAPTUREVIDEO_H__F5345AA4_A39F_4B07_B843_3D87C4287AA0__INCLUDED_
/////////////////////////////////////////////////////////////////////
// CaptureVideo.h : header file
/////////////////////////////////////////////////////////////////////
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <atlbase.h>
//#include <windows.h>
#include <dshow.h>
#define __IDxtCompositor_INTERFACE_DEFINED__  
#define __IDxtAlphaSetter_INTERFACE_DEFINED__  
#define __IDxtJpeg_INTERFACE_DEFINED__  
#define __IDxtKey_INTERFACE_DEFINED__ 


//#include <Qedit.h>
#include "qedit.h"
#ifndef srelease
#define srelease(x) \
	if ( NULL != x ) \
{ \
	x->Release( ); \
	x = NULL; \
}
#endif


class CSampleGrabberCB;
class CCaptureVideo:public CWnd 
{
friend class CSampleGrabberCB;
public:
    void GrabOneFrame(BOOL bGrab);//调用GrabOneFrame(true)就可以捕获当前的静态图像并保存到硬盘上
    HRESULT Init(int iDeviceID,HWND hWnd);//iDeviceID是视频捕捉设备序号，hWnd是视频捕捉窗口的句柄
    int EnumDevices(HWND hList);//hList是下拉列表框的句柄，本函数用于枚举当前系统安装的所有视频捕捉设备
	void stoprun();
    CCaptureVideo();
    virtual ~CCaptureVideo();
private:// lcz new 01/31 由private改为public
    HWND m_hWnd;
    IGraphBuilder *m_pGB;//filter最大容器
    ICaptureGraphBuilder2* m_pCapture;
    IBaseFilter* m_pBF;
    IMediaControl* m_pMC;//采集媒体使用接口
    IVideoWindow* m_pVW;//采集视屏使用接口
public:
    //CComPtr <ISampleGrabber> m_pGrabber;
	 ISampleGrabber* m_pGrabber;
public:
void FreeMediaType(AM_MEDIA_TYPE& mt);
bool BindFilter(int deviceId, IBaseFilter **pFilter);
void ResizeVideoWindow();
HRESULT SetupVideoWindow();

HRESULT InitCaptureGraphBuilder();

int WriteToLog(char* str);// lcz new 01/31
};

#endif // !defined(AFX_CAPTUREVIDEO_H__F5345AA4_A39F_4B07_B843_3D87C4287AA0__INCLUDED_)
