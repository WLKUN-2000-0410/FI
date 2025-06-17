// OpencvCameraConfig.cpp : 实现文件
//

#include "stdafx.h"
#include "INScan-Lite.h"
#include "OpencvCameraConfig.h"
#include "afxdialogex.h"
#include "MiniCamera.h"

// OpencvCameraConfig 对话框

IMPLEMENT_DYNAMIC(OpencvCameraConfig, CDialogEx)

OpencvCameraConfig::OpencvCameraConfig(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG6, pParent)
{

}

OpencvCameraConfig::~OpencvCameraConfig()
{
}

void OpencvCameraConfig::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SLIDER_LEFT, m_left);
	DDX_Control(pDX, IDC_SLIDER_TOP, m_top);
	DDX_Control(pDX, IDC_SLIDER_RIGHT, m_right);
	DDX_Control(pDX, IDC_SLIDER_BOTTOM, m_bottom);
}


BEGIN_MESSAGE_MAP(OpencvCameraConfig, CDialogEx)
	// ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_LEFT, &OpencvCameraConfig::OnNMCustomdrawSliderLeft)
	ON_BN_CLICKED(IDC_BUTTON1, &OpencvCameraConfig::OnBnClickedButton1)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_LEFT, &OpencvCameraConfig::OnNMReleasedcaptureSliderP)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_TOP, &OpencvCameraConfig::OnNMReleasedcaptureSliderP)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_RIGHT, &OpencvCameraConfig::OnNMReleasedcaptureSliderP)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_BOTTOM, &OpencvCameraConfig::OnNMReleasedcaptureSliderP)
END_MESSAGE_MAP()

BOOL OpencvCameraConfig::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置范围
	m_left.SetRange(0, 9);
	m_top.SetRange(0, 9);
	m_right.SetRange(0, 9);
	m_bottom.SetRange(0, 9);

	// 刷新ini
	refreshParamFromIni();

	return TRUE;
}

// OpencvCameraConfig 消息处理程序


void OpencvCameraConfig::OnNMCustomdrawSliderLeft(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	UpdateData(TRUE);
	CString log;
	CMiniCamera *camera = (CMiniCamera*)GetParent();
	if (pNMHDR->hwndFrom == m_left.m_hWnd)
	{
		int pos = m_left.GetPos();
		log.Format("滑动至 %", pos);
		camera->setLeftRoi(pos);
	}
	else if (pNMHDR->hwndFrom == m_top.m_hWnd)
	{
		int pos = m_top.GetPos();
		log.Format("滑动至 %", pos);
		camera->setTopRoi(pos);
	}
	else if (pNMHDR->hwndFrom == m_right.m_hWnd)
	{
		int pos = m_right.GetPos();
		log.Format("滑动至 %", pos);
		camera->setRightRoi(pos);
	}
	else if (pNMHDR->hwndFrom == m_bottom.m_hWnd)
	{
		int pos = m_bottom.GetPos();
		log.Format("滑动至 %", pos);
		camera->setBottomRoi(pos);
	}
	::OutputDebugString(log);
	UpdateData(FALSE);
}


void OpencvCameraConfig::OnBnClickedButton1()
{
	CMiniCamera *camera = (CMiniCamera*)GetParent();
	// TODO: 在此添加控件通知处理程序代码
	int pos = m_left.GetPos();
	CString tmp;
	camera->setLeftRoi(pos);
	tmp.Format("%d", pos);
	WritePrivateProfileStringA("CAMERA", "left", tmp, sInfo.strPath);
	
	pos = m_top.GetPos();
	camera->setTopRoi(pos);
	tmp.Format("%d", pos);
	WritePrivateProfileStringA("CAMERA", "top", tmp, sInfo.strPath);

	pos = m_right.GetPos();
	camera->setRightRoi(pos);
	tmp.Format("%d", pos);
	WritePrivateProfileStringA("CAMERA", "right", tmp, sInfo.strPath);

	pos = m_bottom.GetPos();
	camera->setBottomRoi(pos);
	tmp.Format("%d", pos);
	WritePrivateProfileStringA("CAMERA", "bottom", tmp, sInfo.strPath);
}

void OpencvCameraConfig::refreshParamFromIni()
{
	int left = GetPrivateProfileInt("CAMERA", "left", 0, sInfo.strPath);
	int top = GetPrivateProfileInt("CAMERA", "top", 0, sInfo.strPath);
	int right = GetPrivateProfileInt("CAMERA", "right", 0, sInfo.strPath);
	int bottom = GetPrivateProfileInt("CAMERA", "bottom", 0, sInfo.strPath);

	m_left.SetPos(left);
	m_top.SetPos(top);
	m_right.SetPos(right);
	m_bottom.SetPos(bottom);

	// 刷新到静态文本
	CString tmp;
	tmp.Format("%d", m_left.GetPos());
	GetDlgItem(IDC_STATIC_LEFT)->SetWindowTextA(tmp);
	tmp.Format("%d", m_top.GetPos());
	GetDlgItem(IDC_STATIC_TOP)->SetWindowTextA(tmp);
	tmp.Format("%d", m_right.GetPos());
	GetDlgItem(IDC_STATIC_RIGHT)->SetWindowTextA(tmp);
	tmp.Format("%d", m_bottom.GetPos());
	GetDlgItem(IDC_STATIC_BOTTOM)->SetWindowTextA(tmp);
}

void OpencvCameraConfig::OnNMReleasedcaptureSliderP(NMHDR *pNMHDR, LRESULT *pResult)
{
	// 控件的值刷新到变量
	// hwndFrom
	UpdateData(TRUE);
	CString tmp;
	if (pNMHDR->hwndFrom == m_left.m_hWnd)
	{
		tmp.Format("%d", m_left.GetPos());
		GetDlgItem(IDC_STATIC_LEFT)->SetWindowTextA(tmp);
	}
	else if (pNMHDR->hwndFrom == m_top.m_hWnd)
	{
		tmp.Format("%d", m_top.GetPos());
		GetDlgItem(IDC_STATIC_TOP)->SetWindowTextA(tmp);
	}
	else if (pNMHDR->hwndFrom == m_right.m_hWnd)
	{
		tmp.Format("%d", m_right.GetPos());
		GetDlgItem(IDC_STATIC_RIGHT)->SetWindowTextA(tmp);
	}
	else if (pNMHDR->hwndFrom == m_bottom.m_hWnd)
	{
		tmp.Format("%d", m_bottom.GetPos());
		GetDlgItem(IDC_STATIC_BOTTOM)->SetWindowTextA(tmp);
	}

	// 变量的值刷新到控件
	UpdateData(FALSE);
	*pResult = 0;
}