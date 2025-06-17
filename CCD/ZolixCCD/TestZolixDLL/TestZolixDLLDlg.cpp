
// TestZolixDLLDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "TestZolixDLL.h"
#include "TestZolixDLLDlg.h"
#include "afxdialogex.h"
#include "IniFile.h"

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
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CTestZolixDLLDlg 对话框




CTestZolixDLLDlg::CTestZolixDLLDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTestZolixDLLDlg::IDD, pParent)
	, strEdit(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTestZolixDLLDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, strEdit);
	DDX_Control(pDX, IDC_TCHART2, m_Tchar);
}

BEGIN_MESSAGE_MAP(CTestZolixDLLDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CTestZolixDLLDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON1, &CTestZolixDLLDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CTestZolixDLLDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CTestZolixDLLDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CTestZolixDLLDlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &CTestZolixDLLDlg::OnBnClickedButton5)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON6, &CTestZolixDLLDlg::OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON7, &CTestZolixDLLDlg::OnBnClickedButton7)
	ON_BN_CLICKED(IDC_BUTTON8, &CTestZolixDLLDlg::OnBnClickedButton8)
	ON_BN_CLICKED(IDC_BUTTON9, &CTestZolixDLLDlg::OnBnClickedButton9)
	ON_BN_CLICKED(IDC_BUTTON10, &CTestZolixDLLDlg::OnBnClickedButton10)
	ON_BN_CLICKED(IDC_BUTTON11, &CTestZolixDLLDlg::OnBnClickedButton11)
	ON_BN_CLICKED(IDC_BUTTON12, &CTestZolixDLLDlg::OnBnClickedButton12)
	ON_BN_CLICKED(IDC_BUTTON13, &CTestZolixDLLDlg::OnBnClickedButton13)
END_MESSAGE_MAP()


// CTestZolixDLLDlg 消息处理程序

BOOL CTestZolixDLLDlg::OnInitDialog()
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

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CTestZolixDLLDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CTestZolixDLLDlg::OnPaint()
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
HCURSOR CTestZolixDLLDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CTestZolixDLLDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	//delete testBase1;
	CDialogEx::OnOK();
}


void CTestZolixDLLDlg::OnBnClickedButton1()
{
	UpdateData(TRUE);
	testBase1.InitType(atoi(strEdit));
}


void CTestZolixDLLDlg::OnBnClickedButton2()
{
	int nRet;
	nRet=testBase1.Connect();
	strEdit.Format("%d",nRet);
	UpdateData(FALSE);
	
	//testBase1.InitType(10);
	//nRet=testBase1.Connect();
	
	//testBase2.InitType(15);
	//nRet=testBase2.Connect();

	//testBase3.InitType(20);
	//nRet=testBase3.Connect();

}


void CTestZolixDLLDlg::OnBnClickedButton3()
{
	testBase1.SetCooler(1);
}


void CTestZolixDLLDlg::OnBnClickedButton4()
{
	testBase1.SetCooler(0);
}


void CTestZolixDLLDlg::OnBnClickedButton5()
{
	UpdateData(TRUE);
	testBase1.SetExpTime(atof(strEdit));
}


void CTestZolixDLLDlg::OnTimer(UINT_PTR nIDEvent)
{
	if(nIDEvent==1)
	{
	    int Temp;
		testBase1.GetTemper(&Temp);
		CString str;
		str.Format("%d",Temp);
		GetDlgItem(IDC_STATIC_T)->SetWindowText(str);
	}

	CDialogEx::OnTimer(nIDEvent);
}


void CTestZolixDLLDlg::OnBnClickedButton6()
{
	SetTimer(1,1000,NULL);
}


void CTestZolixDLLDlg::OnBnClickedButton7()
{
	KillTimer(1);
}


void CTestZolixDLLDlg::OnBnClickedButton8()
{
	
	strEdit=testBase1.GetDevName();
	UpdateData(FALSE);
}


void CTestZolixDLLDlg::OnBnClickedButton9()
{
	testBase1.DisConnect();
}


void CTestZolixDLLDlg::OnBnClickedButton10()
{
	//fl.FindFile(_T("F:\\test.ini"));
	testBase1.ShowCameraSetDlg();
}


void CTestZolixDLLDlg::OnBnClickedButton11()
{
	
	UpdateData(TRUE);
	testBase1.SetLanguage(atoi(strEdit));

	
}


void CTestZolixDLLDlg::OnBnClickedButton12()
{
	UpdateData(TRUE);
	int x,y;
	testBase1.GetDevSize(&x,&y);
	double pd[3000]={0};
	double pdX[3000]={0};
	//testBase1.SetExpTime(atof(strEdit));
	double aa[6]={0.123,0.1,0.000125,0.0,0.0,0.0};
	testBase1.DataAcquisition(pd,x);
	testBase1.XAxisData(pdX,aa,6,x);

	int  a=0;
	m_Tchar.Series(0).Clear();
	for(int i=0;i<x;i++)
	{
		m_Tchar.Series(0).AddXY(pdX[i],pd[i],"",NULL);
	}

}

void CTestZolixDLLDlg::OnBnClickedButton13()
{
	int  NUm;
	CString str[10];
	int  itye[10];
	testBase1.CameraType(str,itye,&NUm);
	CString strX,strTemp;
	for (int i=0;i<NUm;i++)
	{
		strTemp.Format("-%d ",itye[i]);
		strX+=str[i]+strTemp;
	}
	strEdit=strX;
	UpdateData(FALSE);
}
