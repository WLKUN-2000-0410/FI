
// INScanLiteConfigDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "INScanLiteConfig.h"
#include "INScanLiteConfigDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define INI_PATH "./INScan-Lite.ini"


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CINScanLiteConfigDlg 对话框



CINScanLiteConfigDlg::CINScanLiteConfigDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_INSCANLITECONFIG_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CINScanLiteConfigDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_SOFT_ATTR, m_softAttr);
	DDX_Control(pDX, IDC_COMBO_SHOW_METHOD, m_showMethod);
	DDX_Control(pDX, IDC_COMBO_IS_SHOW_CAMERA, m_isShowCamera);
	DDX_Control(pDX, IDC_EDIT_LEFT, m_left);
	DDX_Control(pDX, IDC_EDIT_TOP, m_top);
	DDX_Control(pDX, IDC_EDIT_RIGHT, m_right);
	DDX_Control(pDX, IDC_EDIT_BOTTOM, m_bottom);
	DDX_Control(pDX, IDC_EDIT_HOFFSET, m_hoffset);
	DDX_Control(pDX, IDC_EDIT_VOFFSET, m_voffset);
	DDX_Control(pDX, IDC_EDIT_W, m_w);
	DDX_Control(pDX, IDC_EDIT_H, m_h);
}

BEGIN_MESSAGE_MAP(CINScanLiteConfigDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_SAVE, &CINScanLiteConfigDlg::OnBnClickedButtonSave)
	ON_CBN_SELCHANGE(IDC_COMBO_SHOW_METHOD, &CINScanLiteConfigDlg::OnCbnSelchangeComboShowMethod)
END_MESSAGE_MAP()


// CINScanLiteConfigDlg 消息处理程序

BOOL CINScanLiteConfigDlg::OnInitDialog()
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

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	CString tmp;
	// 软件属性
	int selfUse = GetPrivateProfileInt("CAMERA", "self_use", 0, INI_PATH);
	// 0 客户版
	m_softAttr.AddString("客户版");
	// 1 自用版
	m_softAttr.AddString("自用版");
	m_softAttr.SetCurSel(selfUse);

	// 相机显示方式，目前两种相机
	// method=opencv
	// 1. 200E 只有opencv默认方式
	// 2. 500E 相机，有opencv默认方式和官方方式
	char method[1024]{0};
	GetPrivateProfileString("CAMERA", "method", "opencv", method, 1024, INI_PATH);
	m_showMethod.InsertString(0, "默认方式");
	m_showMethod.InsertString(1, "500E专用");
	m_showMethod.InsertString(2, "鑫图相机专用");
	if (strcmp(method, "opencv") == 0 || strcmp(method, "xt") == 0)
	{
		if (strcmp(method, "xt") == 0)
		{
			m_showMethod.SetCurSel(2);
		}
		else
		{
			m_showMethod.SetCurSel(0);
		}
		
		// 禁用500E ROI编辑框
		m_hoffset.EnableWindow(FALSE);
		m_voffset.EnableWindow(FALSE);
		m_w.EnableWindow(FALSE);
		m_h.EnableWindow(FALSE);

		m_left.EnableWindow(TRUE);
		m_top.EnableWindow(TRUE);
		m_left.EnableWindow(TRUE);
		m_top.EnableWindow(TRUE);
	}
	else
	{
		m_showMethod.SetCurSel(1);
		// 500E ROI编辑框
		m_hoffset.EnableWindow(TRUE);
		m_voffset.EnableWindow(TRUE);
		m_w.EnableWindow(TRUE);
		m_h.EnableWindow(TRUE);

		m_left.EnableWindow(FALSE);
		m_top.EnableWindow(FALSE);
		m_left.EnableWindow(FALSE);
		m_top.EnableWindow(FALSE);
	}

	// 是否显示摄像头
	int show = GetPrivateProfileInt("CAMERA", "show", 1, INI_PATH);
	m_isShowCamera.InsertString(0, "不显示");
	m_isShowCamera.InsertString(1, "显示");
	m_isShowCamera.SetCurSel(show);

	// 默认方式opencv截取方式参数
	int left = GetPrivateProfileInt("CAMERA", "left", 0, INI_PATH);
	int top = GetPrivateProfileInt("CAMERA", "top", 0, INI_PATH);
	int right = GetPrivateProfileInt("CAMERA", "right", 0, INI_PATH);
	int bottom = GetPrivateProfileInt("CAMERA", "bottom", 0, INI_PATH);
	tmp.Format("%d", left);
	m_left.SetWindowTextA(tmp);
	tmp.Format("%d", top);
	m_top.SetWindowTextA(tmp);
	tmp.Format("%d", right);
	m_right.SetWindowTextA(tmp);
	tmp.Format("%d", bottom);
	m_bottom.SetWindowTextA(tmp);

	// 500E 专用
	int hoffset = GetPrivateProfileInt("CAMERA", "hoffset", 0, INI_PATH);
	int voffset = GetPrivateProfileInt("CAMERA", "voffset", 0, INI_PATH);
	int hwidth = GetPrivateProfileInt("CAMERA", "hwidth", 0, INI_PATH);
	int vheight = GetPrivateProfileInt("CAMERA", "vheight", 0, INI_PATH);
	tmp.Format("%d", hoffset);
	m_hoffset.SetWindowTextA(tmp);
	tmp.Format("%d", voffset);
	m_voffset.SetWindowTextA(tmp);
	tmp.Format("%d", hwidth);
	m_w.SetWindowTextA(tmp);
	tmp.Format("%d", vheight);
	m_h.SetWindowTextA(tmp);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CINScanLiteConfigDlg::OnSysCommand(UINT nID, LPARAM lParam)
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
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CINScanLiteConfigDlg::OnPaint()
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
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CINScanLiteConfigDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CINScanLiteConfigDlg::OnBnClickedButtonSave()
{
	CString tmp;
	// 保存配置
	// 软件属性
	int attr = m_softAttr.GetCurSel();
	// 自用版
	tmp.Format("%d", attr);
	WritePrivateProfileString("CAMERA", "self_use", tmp, INI_PATH);

	// 相机显示方式
	if (m_showMethod.GetCurSel() == 0)
	{
		WritePrivateProfileStringA("CAMERA", "method", "opencv", INI_PATH);
	}
	else if (m_showMethod.GetCurSel() == 1)
	{
		WritePrivateProfileStringA("CAMERA", "method", "500E", INI_PATH);
	}
	else 
	{
		WritePrivateProfileStringA("CAMERA", "method", "xt", INI_PATH);
	}

	// 是否显示摄像头
	tmp.Format("%d", m_isShowCamera.GetCurSel());
	WritePrivateProfileString("CAMERA", "show", tmp, INI_PATH);

	// 默认相机截取参数
	m_left.GetWindowTextA(tmp);
	WritePrivateProfileString("CAMERA", "left", tmp, INI_PATH);
	m_top.GetWindowTextA(tmp);
	WritePrivateProfileString("CAMERA", "top", tmp, INI_PATH);
	m_right.GetWindowTextA(tmp);
	WritePrivateProfileString("CAMERA", "right", tmp, INI_PATH);
	m_bottom.GetWindowTextA(tmp);
	WritePrivateProfileString("CAMERA", "bottom", tmp, INI_PATH);

	// 500截取参数
	m_hoffset.GetWindowTextA(tmp);
	WritePrivateProfileString("CAMERA", "hoffset", tmp, INI_PATH);
	m_voffset.GetWindowTextA(tmp);
	WritePrivateProfileString("CAMERA", "voffset", tmp, INI_PATH);
	m_w.GetWindowTextA(tmp);
	WritePrivateProfileString("CAMERA", "hwidth", tmp, INI_PATH);
	m_h.GetWindowTextA(tmp);
	WritePrivateProfileString("CAMERA", "vheight", tmp, INI_PATH);

	AfxMessageBox("保存成功");
}



void CINScanLiteConfigDlg::OnCbnSelchangeComboShowMethod()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_showMethod.GetCurSel() == 0 || m_showMethod.GetCurSel() == 2)
	{

		// 禁用500E ROI编辑框
		m_hoffset.EnableWindow(FALSE);
		m_voffset.EnableWindow(FALSE);
		m_w.EnableWindow(FALSE);
		m_h.EnableWindow(FALSE);

		m_left.EnableWindow(TRUE);
		m_top.EnableWindow(TRUE);
		m_right.EnableWindow(TRUE);
		m_bottom.EnableWindow(TRUE);
	}
	else
	{
		// 500E ROI编辑框
		m_hoffset.EnableWindow(TRUE);
		m_voffset.EnableWindow(TRUE);
		m_w.EnableWindow(TRUE);
		m_h.EnableWindow(TRUE);

		m_left.EnableWindow(FALSE);
		m_top.EnableWindow(FALSE);
		m_right.EnableWindow(FALSE);
		m_bottom.EnableWindow(FALSE);
	}
}
