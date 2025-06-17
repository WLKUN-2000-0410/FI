// DlgTest.cpp : 实现文件
//

#include "stdafx.h"
#include "INScan-Lite.h"
#include "DlgTest.h"
#include "afxdialogex.h"
#include "INScan-LiteDlg.h"
#include ".\Prior\PriorCtrl.h"

// CDlgTest 对话框

IMPLEMENT_DYNAMIC(CDlgTest, CDialog)

CDlgTest::CDlgTest(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgTest::IDD, pParent)
{
	m_iCheck96 = 0;
	InterlockedExchange(&m_lMoveFinsh, 0);
	m_iScanModel = -1;
	m_bStop = true;
	m_iOrder = 1;
}

CDlgTest::~CDlgTest()
{
	m_FontEnglish.DeleteObject();
	m_FontChinese.DeleteObject();
}

void CDlgTest::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_CHECK_K, m_BtnCkeckK);
	DDX_Control(pDX, IDC_BUTTON_CHECK_J, m_BtnCkeckJ);
	DDX_Control(pDX, IDC_BUTTON_ALONE, m_btn_alone);
	DDX_Control(pDX, IDC_BUTTON_AUTO, m_btn_auto);
	DDX_Control(pDX, IDC_BUTTON_PRIOR, m_btn_priorctrl);

	//DDX_Control(pDX, IDC_CHECK_96, m_check_96);
}


BEGIN_MESSAGE_MAP(CDlgTest, CDialog)
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BUTTON_CHECK_K, &CDlgTest::OnBnClickedButtonCheckK)
	ON_BN_CLICKED(IDC_BUTTON_CHECK_J, &CDlgTest::OnBnClickedButtonCheckJ)
	ON_BN_CLICKED(IDC_CHECK_96, &CDlgTest::OnBnClickedCheck96)
	ON_BN_CLICKED(IDC_BUTTON_ALONE, &CDlgTest::OnBnClickedButtonAlone)
	ON_BN_CLICKED(IDC_BUTTON_AUTO, &CDlgTest::OnBnClickedButtonAuto)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_PRIOR, &CDlgTest::OnBnClickedButtonPrior)
END_MESSAGE_MAP()


// CDlgTest 消息处理程序


void CDlgTest::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	CRect rect;
	GetClientRect(rect);
	dc.FillSolidRect(rect,clr);
	CDialog::OnPaint();
}


BOOL CDlgTest::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetButtonState(&m_BtnCkeckJ);
	SetButtonState(&m_BtnCkeckK);
	SetButtonState(&m_btn_alone);
	SetButtonState(&m_btn_auto);
	SetButtonState(&m_btn_priorctrl);

	//SetButtonState(&m_check_96);

	m_FontChinese.CreatePointFont(nFontSize,strFontChinese);
	m_FontEnglish.CreatePointFont(nFontSize,strFontEngish);
	SetControlFont();

	InitDialog();
	InitDialogShow(1);
	GetDlgItem(IDC_BUTTON_ALONE)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_AUTO)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_PRIOR)->EnableWindow(FALSE);

	m_dlgAlone = new CDlgAloneParams;
	m_dlgAuto = new CDlgAutoParams;
	m_dlgAlone->SetFontLan(&m_FontChinese, &m_FontEnglish);
	m_dlgAuto->SetFontLan(&m_FontChinese, &m_FontEnglish);
	m_dlgPriorCtrl.SetFontLan(&m_FontChinese, &m_FontEnglish);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDlgTest::InitDialog()
{
	m_DlgTestCheckJ.Create(IDD_DLG_TEST_J,this);
	m_DlgTestCheckK.Create(IDD_DLG_TEST_IN,this);
	CRect rect;
	GetDlgItem(IDC_STATIC_TEST)->GetWindowRect(rect);	
	ScreenToClient(&rect);
	::SetWindowPos(m_DlgTestCheckJ.m_hWnd,HWND_TOP,rect.left,rect.top+8,rect.Width(),rect.Height()-150,SWP_SHOWWINDOW|SWP_FRAMECHANGED);
	::SetWindowPos(m_DlgTestCheckK.m_hWnd,HWND_TOP,rect.left,rect.top+8,rect.Width(),rect.Height()-150,SWP_SHOWWINDOW|SWP_FRAMECHANGED);

	m_DlgTestCheckJ.ModifyStyle(0,WS_CLIPSIBLINGS);
	m_DlgTestCheckJ.SetParent(this);
	m_DlgTestCheckK.ModifyStyle(0,WS_CLIPSIBLINGS);
	m_DlgTestCheckK.SetParent(this);
	SetTimer(1001,10,NULL);
	//AfxBeginThread(&CtrlPriorThread, this);

}
void CDlgTest::InitDialogShow(int nID)
{
	m_DlgTestCheckJ.ShowWindow(SW_HIDE);
	m_DlgTestCheckK.ShowWindow(SW_HIDE);

	switch(nID)
	{
	case 0:
		m_DlgTestCheckK.ShowWindow(SW_SHOW);
		sTInfo.KLMode=0;
		break;
	case 1:
		m_DlgTestCheckJ.ShowWindow(SW_SHOW);
		sTInfo.KLMode=1;
		break;
	}
}
void CDlgTest::SetControlFont()
{
	CFont *m_Font;

	if (sInfo.nLanguage==1)
		m_Font=&m_FontEnglish;
	else
		m_Font=&m_FontChinese;

	GetDlgItem(IDC_BUTTON_CHECK_J)->SetFont(m_Font);
	GetDlgItem(IDC_BUTTON_CHECK_K)->SetFont(m_Font);
	GetDlgItem(IDC_BUTTON_ALONE)->SetFont(m_Font);
	GetDlgItem(IDC_BUTTON_AUTO)->SetFont(m_Font);
	GetDlgItem(IDC_BUTTON_PRIOR)->SetFont(m_Font);

	GetDlgItem(IDC_CHECK_96)->SetFont(m_Font);

}
HBRUSH CDlgTest::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	if (nCtlColor==CTLCOLOR_STATIC)
	{
		//pDC->SetBkMode(TRANSPARENT);
		pDC->SetBkColor(clr);
		HBRUSH b =CreateSolidBrush(clr);
		return b;
	}
	
	return hbr;
}


void CDlgTest::OnBnClickedButtonCheckK()
{
	InitDialogShow(0);
	sTInfo.KLMode=0;
}


void CDlgTest::OnBnClickedButtonCheckJ()
{
	InitDialogShow(1);
	sTInfo.KLMode=1;
}
void CDlgTest::UpdateLanguage()
{
	CINScanLiteDlg * pDlg=(CINScanLiteDlg*)GetParent();
	pDlg->m_ZLanguage.SetCreateLanguageState(FALSE);
	pDlg->m_ZLanguage.SetDialogStrings((long)(CDialog*)this,IDD);

	m_DlgTestCheckJ.UpdateLanguage();
	m_DlgTestCheckK.UpdateLanguage();

	SetControlFont();

}
void CDlgTest::EnableBtn(BOOL bShow)
{
	GetDlgItem(IDC_BUTTON_CHECK_K)->EnableWindow(bShow);
	GetDlgItem(IDC_BUTTON_CHECK_J)->EnableWindow(bShow);
	GetDlgItem(IDC_CHECK_96)->EnableWindow(bShow);
	bool bCheck = m_iCheck96 > 0 ? true :false;
	GetDlgItem(IDC_BUTTON_ALONE)->EnableWindow(bCheck&&bShow);
	GetDlgItem(IDC_BUTTON_AUTO)->EnableWindow(bCheck&&bShow);
	//GetDlgItem(IDC_BUTTON_PRIOR)->EnableWindow(bCheck&&bShow);
	CINScanLiteDlg * pDlg=(CINScanLiteDlg*)GetParent();
	pDlg->SetWindowText(strSoftName);
}
BOOL CDlgTest::PreTranslateMessage(MSG* pMsg)
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

	return CDialog::PreTranslateMessage(pMsg);
}


void CDlgTest::OnBnClickedCheck96()
{
	// TODO: 在此添加控件通知处理程序代码
	CINScanLiteDlg * pDlg = (CINScanLiteDlg *)AfxGetApp()->GetMainWnd();
	m_iCheck96=((CButton*)GetDlgItem(IDC_CHECK_96))->GetCheck();
	if (m_iCheck96 > 0)
	{
		if(!get_is_connect_prior())
		{
			sInfo.PriorConnect=1;
			if(!connect_prior(sSInfo.nComPrior))
			{
				sInfo.PriorConnect=0;
				MessageBox(pDlg->LoadStringMeau(_T("ID_PRIOR_CONNECT")));
				((CButton*)GetDlgItem(IDC_CHECK_96))->SetCheck(0);
				m_iCheck96 = 0;
			}
			else
			{
				//move_position_XY_prior(0,0);
				GetDlgItem(IDC_BUTTON_ALONE)->EnableWindow(TRUE);
				GetDlgItem(IDC_BUTTON_AUTO)->EnableWindow(TRUE);
				GetDlgItem(IDC_BUTTON_PRIOR)->EnableWindow(TRUE);
			}
		}
		else
		{
			//move_position_XY_prior(0,0);
			GetDlgItem(IDC_BUTTON_ALONE)->EnableWindow(TRUE);
			GetDlgItem(IDC_BUTTON_AUTO)->EnableWindow(TRUE);
			GetDlgItem(IDC_BUTTON_PRIOR)->EnableWindow(TRUE);
		}
	}
	else
	{
		GetDlgItem(IDC_BUTTON_ALONE)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_AUTO)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_PRIOR)->EnableWindow(FALSE);
	}
	
}


void CDlgTest::OnBnClickedButtonAlone()
{
	// TODO: 在此添加控件通知处理程序代码
	m_bStop = true;
	if( m_iScanModel != Alone)
	{
		g_mapDataInfo.clear();
		m_iScanModel = Alone;
	}
	/*if(m_dlgAlone->DoModal() ==IDOK)
	{
	m_bStop = false;
	}*/
	//m_testshow.DoModal();
	CDlgAloneParams dlgAlone(GetParent());
	dlgAlone.SetFontLan(&m_FontChinese, &m_FontEnglish);
	
	if(dlgAlone.DoModal() ==IDOK)
	{
		GetDlgItem(IDC_BUTTON_ALONE)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_AUTO)->EnableWindow(FALSE);
		m_bStop = false;
	}
}


void CDlgTest::OnBnClickedButtonAuto()
{
	// TODO: 在此添加控件通知处理程序代码
	m_bStop = true;
	//m_iOrder = 1;
	if( m_iScanModel != Auto)
	{
		g_mapDataInfo.clear();
		m_iScanModel = Auto;
	}
	//m_dlgAuto.RefreshButton();
	/*if(m_dlgAuto->DoModal() ==IDOK)
	{
	m_iOrder = m_dlgAuto->GetMinIndex();
	m_bStop = false;
	}*/

	CDlgAutoParams	dlgAuto(GetParent());
	dlgAuto.SetFontLan(&m_FontChinese, &m_FontEnglish);
	CINScanLiteDlg * pDlg=(CINScanLiteDlg*)GetParent();
	
	if(dlgAuto.DoModal() ==IDOK)
	{
		GetDlgItem(IDC_BUTTON_ALONE)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_AUTO)->EnableWindow(FALSE);
		m_iOrder = m_dlgAuto->GetMinIndex();
		m_bStop = false;
	}
}


void CDlgTest::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	switch(nIDEvent)
	{
	case 1001:
		{
			if (InterlockedCompareExchange(&m_lMoveFinsh, 0, 0) == 1)
			{
				KillTimer(1001);
				if(sTInfo.KLMode==0)//间隔
				{
					m_DlgTestCheckK.StartScan();
					SetTimer(1002, 10, NULL);
				}
				else if(sTInfo.KLMode==1)//精检
				{
					m_DlgTestCheckJ.StartScan();
					SetTimer(1002, 10, NULL);
				}
				
			}
		}
		break;
	case 1002:
		{
			bool bAutoScanOver = true;
			if(m_DlgTestCheckK.GetStopStatus() || m_DlgTestCheckJ.GetStopStatus())//间隔
			{
				m_iOrder++;
				g_strCurLineName = "NULL";
				m_bStop = true;
				KillTimer(1002);
				SetTimer(1001, 10, NULL);
				InterlockedExchange(&m_lMoveFinsh, 0);
				if(sTInfo.KLMode==0)//间隔
				{
					m_DlgTestCheckK.SetAutoScanBtnStatus(true);
				}
				else if(sTInfo.KLMode==1)//精检
				{
					m_DlgTestCheckJ.SetAutoScanBtnStatus(true);
				}
				break;
			}
			else
			{
				if (m_bAutoRunning==FALSE&&m_bAutoRunningStatue==FALSE)
				{
					S_DATAINFO info;
					int iCurBtn = -1;
					if(m_iScanModel == Auto && g_iScanOrder != 2)//顺序扫描时，不是按照从左到右顺序
					{
						map<int, S_DATAINFO>::iterator iter = g_mapDataInfo.begin();
						for(;iter != g_mapDataInfo.end(); ++iter)
						{
							if(iter->second.iOrder == m_iOrder)
							{
								iCurBtn = iter->first;
								info = iter->second;
								break;
							}
						}
					}
					else
					{
						map<int, S_DATAINFO>::iterator iter = g_mapDataInfo.begin();
						for(;iter != g_mapDataInfo.end(); iter++)
						{
							if (iter->second.iStatus == 0)
							{
								iCurBtn = iter->first;
								info = iter->second;
								break;
							}
						}
					}


				
					m_iOrder++;
					g_strCurLineName = "NULL";
					info.iStatus = 1;
					info.iOrder = 0;
					g_mapDataInfo[iCurBtn] = info;
					InterlockedExchange(&m_lMoveFinsh, 0);
					KillTimer(1002);
					SetTimer(1001, 10, NULL);

					map<int, S_DATAINFO>::iterator iter = g_mapDataInfo.begin();
					for(;iter != g_mapDataInfo.end(); ++iter)
					{
						if(iter->second.iStatus == 0)
						{
							bAutoScanOver = false;
							break;
						}
					}
					if(bAutoScanOver)
					{
						if(sTInfo.KLMode==0)//间隔
						{
							m_DlgTestCheckK.SetAutoScanBtnStatus(true);
						}
						else if(sTInfo.KLMode==1)//精检
						{
							m_DlgTestCheckJ.SetAutoScanBtnStatus(true);
						}
					}
				}
			}
		}
		break;
	}
	CDialog::OnTimer(nIDEvent);
}

UINT CDlgTest::CtrlPriorThread(LPVOID pParam)
{
	CDlgTest* pthis = (CDlgTest*)pParam;
	if(pthis)
		pthis->CtrlPrior();
	return 0;
}

void CDlgTest::CtrlPrior()
{
	CINScanLiteDlg * pDlg = (CINScanLiteDlg *)AfxGetApp()->GetMainWnd();
	int iCurBtn = -1;
	S_DATAINFO info;
	while(1)
	{
		if(m_bStop) 
		{
			Sleep(10);
			continue;
		}

		bool bOver = false;
		
		if(m_iScanModel == Auto && g_iScanOrder != 2)//顺序扫描时，不是按照从左到右顺序
		{
			while(!bOver)
			{
				bOver = true;
				map<int, S_DATAINFO>::iterator iter = g_mapDataInfo.begin();
				for(;iter != g_mapDataInfo.end(); ++iter)
				{
					if(iter->second.iStatus == 0)
						bOver = false;
					if(m_bStop) break;
					while (1)
					{
						if(m_bStop) break;;
						if(InterlockedCompareExchange(&m_lMoveFinsh, 0, 0) == 0)
						{
							if (get_is_connect_prior())
							{
								if(iter->second.iStatus == 0 && (iter->second.iOrder == m_iOrder) )
								{
									g_iCurBtnID = iCurBtn = iter->first;
									info = iter->second;
									g_strCurLineName = info.strLineName;
									set_steplen_x_prior(g_iPriorStep_X);
									set_steplen_y_prior(g_iPriorStep_Y);
									if(!move_position_XY_prior(info.iPosX, -info.iPosY))
									{
										MessageBox(pDlg->LoadStringMeau(_T("ID_PRIOR_CONNECT")));	
									}
									else
									{
										InterlockedExchange(&m_lMoveFinsh, 1);
										break;
									}
								}
								else if(iter->second.iOrder < m_iOrder && (iter->second.iStatus == 1))
								{
									//m_iOrder++;
									break;
								}
								else if(iter->second.iOrder > m_iOrder)
									break;
							}
							else
								break;
						}
						else
							Sleep(1);
					}
				}
			}
			if(bOver) m_bStop = true;
		}
		else
		{
			map<int, S_DATAINFO>::iterator iter = g_mapDataInfo.begin();
			for(;iter != g_mapDataInfo.end(); ++iter)
			{
				if(m_bStop) break;
				while (1)
				{
					if(m_bStop) break;;
					if(InterlockedCompareExchange(&m_lMoveFinsh, 0, 0) == 0)
					{
						if (iter->second.iStatus == 0 && get_is_connect_prior())
						{
							g_iCurBtnID = iCurBtn = iter->first;
							info = iter->second;
							g_strCurLineName = info.strLineName;
							set_steplen_x_prior(g_iPriorStep_X);
							set_steplen_y_prior(g_iPriorStep_Y);
							if(!move_position_XY_prior(info.iPosX, -info.iPosY))
							{
								MessageBox(pDlg->LoadStringMeau(_T("ID_PRIOR_CONNECT")));	
							}
							else
							{
								InterlockedExchange(&m_lMoveFinsh, 1);
								break;
							}
						}
						else
							break;
					}
					else
						Sleep(1);
				}
			}
		}
		
	}
	
}

void CDlgTest::ReleaseResource()
{
	release_prior();
}


void CDlgTest::OnBnClickedButtonPrior()
{
	// TODO: 在此添加控件通知处理程序代码
	CDlgPriorCtrl dlgPriorCtrl(GetParent());;
	dlgPriorCtrl.SetFontLan(&m_FontChinese, &m_FontEnglish);
	dlgPriorCtrl.DoModal();
}
