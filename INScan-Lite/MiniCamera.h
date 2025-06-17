
//#if !defined(AFX_TESTDLG_H__C91FF804_3B3F_4C37_AC28_CEE88E549887__INCLUDED_)
//#define AFX_TESTDLG_H__C91FF804_3B3F_4C37_AC28_CEE88E549887__INCLUDED_
#include "resource.h"
#pragma once
#include "CaptureVideo.h"
#include "afxwin.h"
#include "SerialClass.h"

#include "opencv2/opencv.hpp"

#include "cv.h"
#include "cxcore.h"
#include "highgui.h"
#include "opencv2/highgui/highgui.hpp"  
#include "opencv2/imgproc/imgproc.hpp"

#include "OpencvCameraConfig.h"
#include "ExternDlg.h"
#include <TUCamApi.h>
#include "afxcmn.h"
using namespace cv;

#define FILE_PATH "D:\\FILog.txt" //信息输出文件 lcz new 01/31

class CMiniCamera : public CDialogEx
{
	DECLARE_DYNAMIC(CMiniCamera)

public:
	CMiniCamera(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CMiniCamera();
	CCaptureVideo m_cap;
// 对话框数据
	enum { IDD = IDD_MINI_CAMERA };
	CSerialClass m_Serial;
protected:
	HICON m_hIcon;
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	//afx_msg void OnBnClickedOpencamera();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	//afx_msg void OnBnClickedClose();
	//void cvText(IplImage* img, const char* text, int x, int y);
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
	//void drowline();
	BOOL mousedown;
	bool lookflag;
	CPoint crospoint;
	bool stopline;
	void drowline();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnPaint();
	//afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	//CComboBox m_ListCtrl;
	//CStatic m_staticScreen;
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnStnClickedShowImg();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	//afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	CRect m_rect;
	void ReSize(void);  
	POINT old; 
	//CRoundButton m_btn1 ;
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);// 点击鼠标右键触发事件

	int WriteToLog(char* str);// lcz new 01/31
	afx_msg void OnBnClickedButtonPause();
	afx_msg void OnBnClickedButtonRun();
	CStatic m_Video;
	int usbCameraIndex;
	CStatic m_show;
	// 外部窗口
	// ExternDlg m_externDlg;

	//HWND hWnd1;
	//HWND hParent2;
	
	Mat cam1;// lcz new 02/06

	BOOL m_500EInitStatus = FALSE, m_500EPlayStatus = FALSE;

	// 在四个方向截取
	OpencvCameraConfig *m_opencvCameraConfig = NULL;
	int m_leftRoi = 0, m_topRoi = 0, m_rightRoi = 0, m_bottomRoi = 0;
	int hoffset = 0, voffset = 0, hwidth = 0, vheight = 0;

	void setLeftRoi(const int& val)
	{
		m_leftRoi = val;
	}

	void setTopRoi(const int& val)
	{
		m_topRoi = val;
	}

	void setRightRoi(const int& val)
	{
		m_rightRoi = val;
	}

	void setBottomRoi(const int& val)
	{
		m_bottomRoi = val;
	}

	// 获取相机的每一帧
	TUCAM_FRAME m_tf;
	// 打开的相机句柄和编号
	TUCAM_OPEN opCam;
	// 相机获取图像线程句柄
	CWinThread *m_xtAcquireThreadHandle = NULL;
	// 是否等待图像
	BOOL m_waitFrame = FALSE;

	void UpdateColorGain();
	afx_msg void OnNMCustomdrawSlider2(NMHDR *pNMHDR, LRESULT *pResult);
	CSliderCtrl m_RGainCtrl;
	CSliderCtrl m_GGainCtrl;
	CSliderCtrl m_BGainCtrl;
	afx_msg void OnNMCustomdrawSlider3(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawSlider4(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawSlider1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButton1();
	CSliderCtrl m_ExpTimeCtrl;
	void UpdateExposurteTimeText();
	afx_msg void OnBnClickedCheck1();
	
	BOOL m_Aeen;
	CSliderCtrl m_Gamma;
	CSliderCtrl m_Contrast;
	int GammaValue;
	afx_msg void OnNMCustomdrawSlider5(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawSlider6(NMHDR *pNMHDR, LRESULT *pResult);
	CSliderCtrl m_SaturationCtrl;
	afx_msg void OnNMCustomdrawSlider7(NMHDR *pNMHDR, LRESULT *pResult);

	void InitXTCameraData(int nMode);
	afx_msg void OnNMCustomdrawSlider8(NMHDR *pNMHDR, LRESULT *pResult);
	CSliderCtrl m_ACtrl;
	afx_msg void OnNMCustomdrawSlider9(NMHDR *pNMHDR, LRESULT *pResult);
	CSliderCtrl m_SliderTest;
};
//#endif