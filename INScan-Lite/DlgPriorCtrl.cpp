// DlgPriorCtrl.cpp : 实现文件
//

#include "stdafx.h"
#include "INScan-Lite.h"
#include "DlgPriorCtrl.h"
#include "afxdialogex.h"
#include ".\Prior\PriorCtrl.h"
#include "INScan-LiteDlg.h"

// CDlgPriorCtrl 对话框

IMPLEMENT_DYNAMIC(CDlgPriorCtrl, CDialogEx)

CDlgPriorCtrl::CDlgPriorCtrl(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgPriorCtrl::IDD, pParent)
	, m_fStep_XY(1000)
	, m_fStep_Z(10)
	, m_fMoveX(0)
	, m_fMoveY(0)
	, m_fMoveZ(0)
	,m_iPriorMove(NO_MOVE)
{
	m_dCurPos_X = 0;
	m_dCurPos_Y = 0;
	m_dCurPos_Z = 0;
	m_bMoveFinsh = true;
	m_bRefushBtn = false;
	m_bStop = false;
	m_bRefushEdit = false;
}

CDlgPriorCtrl::~CDlgPriorCtrl()
{
	//KillTimer(10);
}

void CDlgPriorCtrl::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_UP, m_btn_up);
	DDX_Control(pDX, IDC_BUTTON_DOWN, m_btn_down);
	DDX_Control(pDX, IDC_BUTTON_LEFT, m_btn_left);
	DDX_Control(pDX, IDC_BUTTON_RIGHT, m_btn_right);
	DDX_Control(pDX, IDC_BUTTON_ZERO, m_btn_zero);
	DDX_Control(pDX, IDC_BUTTON_IN, m_btn_in);
	DDX_Control(pDX, IDC_BUTTON_OUT, m_btn_out);
	DDX_Control(pDX, IDC_BUTTON_STOP, m_btn_stop);
	DDX_Control(pDX, IDC_BUTTON_MOVE, m_btn_move);

	DDX_Text(pDX, IDC_EDIT_STEP_STAGE, m_fStep_XY);
	DDX_Text(pDX, IDC_EDIT_STEP_FORCE, m_fStep_Z);
	//DDV_MinMaxDouble(pDX, m_fStep_XY, 1, 5000);
	DDV_MinMaxDouble(pDX, m_fStep_Z, 1, 1000);
	DDX_Text(pDX, IDC_EDIT_MOVE_X, m_fMoveX);
	DDX_Text(pDX, IDC_EDIT_MOVE_Y, m_fMoveY);
	DDX_Text(pDX, IDC_EDIT_MOVE_Z, m_fMoveZ);


	DDX_Control(pDX, IDC_EDIT_CUR_X, m_edit_curX);
	DDX_Control(pDX, IDC_EDIT_CUR_Y, m_edit_curY);
	DDX_Control(pDX, IDC_EDIT_CUR_Z, m_edit_curZ);
	DDX_Control(pDX, IDC_EDIT_MOVE_X, m_edit_MoveX);
	DDX_Control(pDX, IDC_EDIT_MOVE_Y, m_edit_MoveY);
	DDX_Control(pDX, IDC_EDIT_MOVE_Z, m_edit_MoveZ);
}


BEGIN_MESSAGE_MAP(CDlgPriorCtrl, CDialogEx)
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BUTTON_IN, &CDlgPriorCtrl::OnBnClickedButtonIn)
	ON_BN_CLICKED(IDC_BUTTON_OUT, &CDlgPriorCtrl::OnBnClickedButtonOut)
	ON_BN_CLICKED(IDC_BUTTON_LEFT, &CDlgPriorCtrl::OnBnClickedButtonLeft)
	ON_BN_CLICKED(IDC_BUTTON_RIGHT, &CDlgPriorCtrl::OnBnClickedButtonRight)
	ON_BN_CLICKED(IDC_BUTTON_ZERO, &CDlgPriorCtrl::OnBnClickedButtonZero)
	ON_BN_CLICKED(IDC_BUTTON_UP, &CDlgPriorCtrl::OnBnClickedButtonUp)
	ON_BN_CLICKED(IDC_BUTTON_DOWN, &CDlgPriorCtrl::OnBnClickedButtonDown)
	ON_BN_CLICKED(IDC_BUTTON_STOP, &CDlgPriorCtrl::OnBnClickedButtonStop)
	ON_BN_CLICKED(IDC_BUTTON_MOVE, &CDlgPriorCtrl::OnBnClickedButtonMove)
	ON_WM_TIMER()
	ON_WM_CLOSE()
END_MESSAGE_MAP()



BOOL CDlgPriorCtrl::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	SetButtonState(&m_btn_up);
	SetButtonState(&m_btn_down);
	SetButtonState(&m_btn_left);
	SetButtonState(&m_btn_right);
	SetButtonState(&m_btn_zero);
	SetButtonState(&m_btn_in);
	SetButtonState(&m_btn_out);
	SetButtonState(&m_btn_stop);
	SetButtonState(&m_btn_move);
	m_bExit = false;

	SetDlgItemInt(IDC_EDIT_STEP_STAGE, 100);
	SetDlgItemInt(IDC_EDIT_STEP_FORCE, 100);

	m_iPrvX = 0xFFFFFFFF;
	m_iPrvY = 0xFFFFFFFF;
	m_iPrvZ = 0xFFFFFFFF;


	AfxBeginThread(&PriorThread, this);
	SetTimer(10, 500, NULL);

	CINScanLiteDlg * pDlg=(CINScanLiteDlg*)GetParent();
	pDlg->m_ZLanguage.SetCreateLanguageState(FALSE);
	pDlg->m_ZLanguage.SetDialogStrings((long)(CDialog*)this,IDD);

	SetControlFont();
	ShowCurXYZ();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

UINT CDlgPriorCtrl::PriorThread(LPVOID pParam)
{
	CDlgPriorCtrl* pthis = (CDlgPriorCtrl*)pParam;
	if(pthis)
		pthis->CtrlPriorFun();
	return 0;
}

void CDlgPriorCtrl::CtrlPriorFun()
{
	CINScanLiteDlg * pDlg = (CINScanLiteDlg *)AfxGetApp()->GetMainWnd();
	while(!m_bExit)
	{
		if(m_iPriorMove == NO_MOVE)
		{
			Sleep(10);
			continue;
		}
		
		m_bRefushBtn = true;
		switch(m_iPriorMove)
		{
		case MOVE_IN:
			{
				if(get_is_connect_prior())
				{
					set_steplen_x_prior(m_fStep_XY);
					set_steplen_y_prior(m_fStep_XY);
					move_back_prior();
				}
			}
			break;
		case MOVE_OUT:
			{
				if(get_is_connect_prior())
				{
					set_steplen_x_prior(m_fStep_XY);
					set_steplen_y_prior(m_fStep_XY);
					move_forward_prior();
				}
			}
			break;
		case MOVE_LEFT:
			{
				if(get_is_connect_prior())
				{
					set_steplen_x_prior(m_fStep_XY);
					set_steplen_y_prior(m_fStep_XY);
					if((m_dCurPos_X - m_fStep_XY >= -100000/* && m_dCurPos_X - m_fStep_XY <= 2000 */))
						move_left_prior();
					else
						MessageBox(pDlg->LoadStringMeau(_T("ID_PRIOR_MOVE_FAIL")));
				}
			}
			break;
		case MOVE_RIGHT:
			{
				if(get_is_connect_prior())
				{
					set_steplen_x_prior(m_fStep_XY);
					set_steplen_y_prior(m_fStep_XY);
					move_right_prior();
				}
			}
			break;
		case MOVE_UP:
			{
				if(get_is_connect_prior())
				{
					set_steplen_z_prior(m_fStep_Z);
					move_up_prior();
				}
			}
			break;
		case MOVE_DOWN:
			{
				if(get_is_connect_prior())
				{
					set_steplen_z_prior(m_fStep_Z);
					move_down_prior();
				}
			}
			break;
		case MOVE_ALL:
			{
				if(get_is_connect_prior())
				{
					set_steplen_x_prior(1);
					set_steplen_y_prior(1);
					set_steplen_z_prior(1);
					if((m_fMoveX >= -100000 /*&& m_fMoveX <= 2000*/ ))
					{
						move_position_XY_prior(m_fMoveX*(-1), m_fMoveY);
						if(!m_bStop)
							move_position_Z_prior(m_fMoveZ);
					}
					else
						MessageBox(pDlg->LoadStringMeau(_T("ID_PRIOR_MOVE_FAIL")));
				}
			}
			break;
		case MOVE_ZERO_XY:
			{
				if(get_is_connect_prior())
				{
					move_zero_xy_prior();
				}
			}
			break;
		}
		//if(m_iPriorMove != NO_MOVE)
		{
			m_bMoveFinsh = true;
			m_iPriorMove = NO_MOVE;
			m_bRefushBtn = true;
		}
	}
}


void CDlgPriorCtrl::ShowCurXYZ()
{
	
	if(get_is_connect_prior())
	{
		get_position_prior(m_dCurPos_X, m_dCurPos_Y, m_dCurPos_Z);
	}
	if(m_iPrvX != (int)m_dCurPos_X)
	{
		CString strX;
		strX.Format("%d", (int)m_dCurPos_X);
		m_edit_curX.SetWindowTextA(strX);
		m_edit_MoveX.SetWindowTextA(strX);
		m_iPrvX = m_dCurPos_X;
	}
	if(m_iPrvY != (int)m_dCurPos_Y)
	{
		CString strY;
		strY.Format("%d", (int)m_dCurPos_Y);
		m_edit_curY.SetWindowTextA(strY);
		m_edit_MoveY.SetWindowTextA(strY);
		m_iPrvY = m_dCurPos_Y;
	}
	if(m_iPrvZ != (int)m_dCurPos_Z)
	{
		CString strZ;
		strZ.Format("%d", (int)m_dCurPos_Z);
		m_edit_curZ.SetWindowTextA(strZ);
		m_edit_MoveZ.SetWindowTextA(strZ);
		m_iPrvZ = m_dCurPos_Z;
	}

	


	/*SetDlgItemInt(IDC_EDIT_CUR_X, m_dCurPos_X);
	SetDlgItemInt(IDC_EDIT_CUR_Y, m_dCurPos_Y);
	SetDlgItemInt(IDC_EDIT_CUR_Z, m_dCurPos_Z);*/


}


// CDlgPriorCtrl 消息处理程序
void CDlgPriorCtrl::SetControlFont()
{
	CFont *m_Font;

	if (sInfo.nLanguage==1)
		m_Font=m_FontEnglish;
	else
		m_Font=m_FontChinese;

	GetDlgItem(IDC_BUTTON_UP)->SetFont(m_Font);
	GetDlgItem(IDC_BUTTON_STOP)->SetFont(m_Font);
	GetDlgItem(IDC_BUTTON_DOWN)->SetFont(m_Font);
	GetDlgItem(IDC_BUTTON_LEFT)->SetFont(m_Font);
	GetDlgItem(IDC_BUTTON_RIGHT)->SetFont(m_Font);
	GetDlgItem(IDC_BUTTON_ZERO)->SetFont(m_Font);
	GetDlgItem(IDC_BUTTON_IN)->SetFont(m_Font);
	GetDlgItem(IDC_BUTTON_OUT)->SetFont(m_Font);
	GetDlgItem(IDC_BUTTON_MOVE)->SetFont(m_Font);
	GetDlgItem(IDC_EDIT_MOVE_X)->SetFont(m_Font);
	GetDlgItem(IDC_EDIT_MOVE_Y)->SetFont(m_Font);
	GetDlgItem(IDC_EDIT_MOVE_Z)->SetFont(m_Font);
	GetDlgItem(IDC_EDIT_CUR_X)->SetFont(m_Font);
	GetDlgItem(IDC_EDIT_CUR_Y)->SetFont(m_Font);
	GetDlgItem(IDC_EDIT_CUR_Z)->SetFont(m_Font);
	GetDlgItem(IDC_EDIT_STEP_STAGE)->SetFont(m_Font);
	GetDlgItem(IDC_EDIT_STEP_FORCE)->SetFont(m_Font);

	GetDlgItem(IDC_STATIC_X)->SetFont(m_Font);
	GetDlgItem(IDC_STATIC_Y)->SetFont(m_Font);
	GetDlgItem(IDC_STATIC_Z)->SetFont(m_Font);
	GetDlgItem(IDC_STATIC_X2)->SetFont(m_Font);
	GetDlgItem(IDC_STATIC_Y2)->SetFont(m_Font);
	GetDlgItem(IDC_STATIC_Z2)->SetFont(m_Font);

	GetDlgItem(IDC_STATIC_FOCE)->SetFont(m_Font);
	GetDlgItem(IDC_STATIC_CURPOS)->SetFont(m_Font);
	GetDlgItem(IDC_STATIC_STAGE)->SetFont(m_Font);
	GetDlgItem(IDC_STATIC_MOVEPOS)->SetFont(m_Font);
	GetDlgItem(IDC_STATIC_STEP_XY)->SetFont(m_Font);
	GetDlgItem(IDC_STATIC_STEP)->SetFont(m_Font);
	
}

BOOL CDlgPriorCtrl::PreTranslateMessage(MSG* pMsg)
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

HBRUSH CDlgPriorCtrl::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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

void CDlgPriorCtrl::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	CRect rect;
	GetClientRect(rect);
	dc.FillSolidRect(rect,clr);
	CDialog::OnPaint();
}


void CDlgPriorCtrl::OnBnClickedButtonIn()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	m_iPriorMove = MOVE_IN;
	//m_bMoveFinsh = false;
	m_bStop = false;
}


void CDlgPriorCtrl::OnBnClickedButtonOut()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	m_iPriorMove = MOVE_OUT;
	//m_bMoveFinsh = false;
	m_bStop = false;
}


void CDlgPriorCtrl::OnBnClickedButtonLeft()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	m_iPriorMove = MOVE_LEFT;
	//m_bMoveFinsh = false;
	m_bStop = false;
}


void CDlgPriorCtrl::OnBnClickedButtonRight()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	m_iPriorMove = MOVE_RIGHT;
	//m_bMoveFinsh = false;
	m_bStop = false;
}


void CDlgPriorCtrl::OnBnClickedButtonZero()
{
	// TODO: 在此添加控件通知处理程序代码
	m_iPriorMove = MOVE_ZERO_XY;
	//m_bMoveFinsh = false;
	m_bStop = false;
}


void CDlgPriorCtrl::OnBnClickedButtonUp()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	m_iPriorMove = MOVE_UP;
	//m_bMoveFinsh = false;
	m_bStop = false;
}


void CDlgPriorCtrl::OnBnClickedButtonDown()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	m_iPriorMove = MOVE_DOWN;
	//m_bMoveFinsh = false;
	m_bStop = false;
}


void CDlgPriorCtrl::OnBnClickedButtonStop()
{
	// TODO: 在此添加控件通知处理程序代码
	if(get_is_connect_prior())
	{
		m_bStop = true;
		stop_prior();
	}
}


void CDlgPriorCtrl::OnBnClickedButtonMove()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	m_iPriorMove = MOVE_ALL;
	//m_bMoveFinsh = false;
	m_bStop = false;
}


void CDlgPriorCtrl::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	switch(nIDEvent)
	{
	case 10:
		{
			ShowCurXYZ();
			if (m_bRefushBtn)
			{
				SetBtnStatus();
				m_bRefushBtn = false;
			}
		}
		break;
	}
	CDialogEx::OnTimer(nIDEvent);
}

void CDlgPriorCtrl::SetBtnStatus()
{
	GetDlgItem(IDC_BUTTON_UP)->EnableWindow(m_bMoveFinsh);
	//GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(m_bMoveFinsh);
	GetDlgItem(IDC_BUTTON_DOWN)->EnableWindow(m_bMoveFinsh);
	GetDlgItem(IDC_BUTTON_LEFT)->EnableWindow(m_bMoveFinsh);
	GetDlgItem(IDC_BUTTON_RIGHT)->EnableWindow(m_bMoveFinsh);
	GetDlgItem(IDC_BUTTON_ZERO)->EnableWindow(m_bMoveFinsh);
	GetDlgItem(IDC_BUTTON_IN)->EnableWindow(m_bMoveFinsh);
	GetDlgItem(IDC_BUTTON_OUT)->EnableWindow(m_bMoveFinsh);
	GetDlgItem(IDC_BUTTON_MOVE)->EnableWindow(m_bMoveFinsh);
	GetDlgItem(IDC_EDIT_MOVE_X)->EnableWindow(m_bMoveFinsh);
	GetDlgItem(IDC_EDIT_MOVE_Y)->EnableWindow(m_bMoveFinsh);
	GetDlgItem(IDC_EDIT_MOVE_Z)->EnableWindow(m_bMoveFinsh);
	GetDlgItem(IDC_EDIT_STEP_STAGE)->EnableWindow(m_bMoveFinsh);
	GetDlgItem(IDC_EDIT_STEP_FORCE)->EnableWindow(m_bMoveFinsh);

}



void CDlgPriorCtrl::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	m_bExit = true;
	KillTimer(10);

	CDialogEx::OnClose();
}
