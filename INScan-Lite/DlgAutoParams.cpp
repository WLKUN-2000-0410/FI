// DlgAutoParams.cpp : 实现文件
//

#include "stdafx.h"
#include "INScan-Lite.h"
#include "DlgAutoParams.h"
#include "afxdialogex.h"
#include "INScan-LiteDlg.h"

// CDlgAutoParams 对话框

IMPLEMENT_DYNAMIC(CDlgAutoParams, CDialogEx)

CDlgAutoParams::CDlgAutoParams(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgAutoParams::IDD, pParent)
{
	m_bSelectStart = false;
	m_bSelectEnd = false;
	m_iStartBtnID = m_iEndBtnID =0;

}

CDlgAutoParams::~CDlgAutoParams()
{
	/*m_FontEnglish->DeleteObject();
	m_FontChinese->DeleteObject();*/
}

void CDlgAutoParams::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_START, m_btn_start);
	DDX_Control(pDX, IDC_BUTTON_END, m_btn_end);
	DDX_Control(pDX, IDC_BUTTON_STARTSCAN, m_btn_startscan);
	DDX_Control(pDX, IDC_BUTTON_GOOUT, m_btn_goout);

	DDX_Control(pDX, IDC_BUTTON1 , m_btn1 );
	DDX_Control(pDX, IDC_BUTTON2 , m_btn2 );
	DDX_Control(pDX, IDC_BUTTON3 , m_btn3 );
	DDX_Control(pDX, IDC_BUTTON4 , m_btn4 );
	DDX_Control(pDX, IDC_BUTTON5 , m_btn5 );
	DDX_Control(pDX, IDC_BUTTON6 , m_btn6 );
	DDX_Control(pDX, IDC_BUTTON7 , m_btn7 );
	DDX_Control(pDX, IDC_BUTTON8 , m_btn8 );
	DDX_Control(pDX, IDC_BUTTON9 , m_btn9 );
	DDX_Control(pDX, IDC_BUTTON10, m_btn10);
	DDX_Control(pDX, IDC_BUTTON11, m_btn11);
	DDX_Control(pDX, IDC_BUTTON12, m_btn12);
	DDX_Control(pDX, IDC_BUTTON13, m_btn13);
	DDX_Control(pDX, IDC_BUTTON14, m_btn14);
	DDX_Control(pDX, IDC_BUTTON15, m_btn15);
	DDX_Control(pDX, IDC_BUTTON16, m_btn16);
	DDX_Control(pDX, IDC_BUTTON17, m_btn17);
	DDX_Control(pDX, IDC_BUTTON18, m_btn18);
	DDX_Control(pDX, IDC_BUTTON19, m_btn19);
	DDX_Control(pDX, IDC_BUTTON20, m_btn20);
	DDX_Control(pDX, IDC_BUTTON21, m_btn21);
	DDX_Control(pDX, IDC_BUTTON22, m_btn22);
	DDX_Control(pDX, IDC_BUTTON23, m_btn23);
	DDX_Control(pDX, IDC_BUTTON24, m_btn24);
	DDX_Control(pDX, IDC_BUTTON25, m_btn25);
	DDX_Control(pDX, IDC_BUTTON26, m_btn26);
	DDX_Control(pDX, IDC_BUTTON27, m_btn27);
	DDX_Control(pDX, IDC_BUTTON28, m_btn28);
	DDX_Control(pDX, IDC_BUTTON29, m_btn29);
	DDX_Control(pDX, IDC_BUTTON30, m_btn30);
	DDX_Control(pDX, IDC_BUTTON31, m_btn31);
	DDX_Control(pDX, IDC_BUTTON32, m_btn32);
	DDX_Control(pDX, IDC_BUTTON33, m_btn33);
	DDX_Control(pDX, IDC_BUTTON34, m_btn34);
	DDX_Control(pDX, IDC_BUTTON35, m_btn35);
	DDX_Control(pDX, IDC_BUTTON36, m_btn36);
	DDX_Control(pDX, IDC_BUTTON37, m_btn37);
	DDX_Control(pDX, IDC_BUTTON38, m_btn38);
	DDX_Control(pDX, IDC_BUTTON39, m_btn39);
	DDX_Control(pDX, IDC_BUTTON40, m_btn40);
	DDX_Control(pDX, IDC_BUTTON41, m_btn41);
	DDX_Control(pDX, IDC_BUTTON42, m_btn42);
	DDX_Control(pDX, IDC_BUTTON43, m_btn43);
	DDX_Control(pDX, IDC_BUTTON44, m_btn44);
	DDX_Control(pDX, IDC_BUTTON45, m_btn45);
	DDX_Control(pDX, IDC_BUTTON46, m_btn46);
	DDX_Control(pDX, IDC_BUTTON47, m_btn47);
	DDX_Control(pDX, IDC_BUTTON48, m_btn48);
	DDX_Control(pDX, IDC_BUTTON49, m_btn49);
	DDX_Control(pDX, IDC_BUTTON50, m_btn50);
	DDX_Control(pDX, IDC_BUTTON51, m_btn51);
	DDX_Control(pDX, IDC_BUTTON52, m_btn52);
	DDX_Control(pDX, IDC_BUTTON53, m_btn53);
	DDX_Control(pDX, IDC_BUTTON54, m_btn54);
	DDX_Control(pDX, IDC_BUTTON55, m_btn55);
	DDX_Control(pDX, IDC_BUTTON56, m_btn56);
	DDX_Control(pDX, IDC_BUTTON57, m_btn57);
	DDX_Control(pDX, IDC_BUTTON58, m_btn58);
	DDX_Control(pDX, IDC_BUTTON59, m_btn59);
	DDX_Control(pDX, IDC_BUTTON60, m_btn60);
	DDX_Control(pDX, IDC_BUTTON61, m_btn61);
	DDX_Control(pDX, IDC_BUTTON62, m_btn62);
	DDX_Control(pDX, IDC_BUTTON63, m_btn63);
	DDX_Control(pDX, IDC_BUTTON64, m_btn64);
	DDX_Control(pDX, IDC_BUTTON65, m_btn65);
	DDX_Control(pDX, IDC_BUTTON66, m_btn66);
	DDX_Control(pDX, IDC_BUTTON67, m_btn67);
	DDX_Control(pDX, IDC_BUTTON68, m_btn68);
	DDX_Control(pDX, IDC_BUTTON69, m_btn69);
	DDX_Control(pDX, IDC_BUTTON70, m_btn70);
	DDX_Control(pDX, IDC_BUTTON71, m_btn71);
	DDX_Control(pDX, IDC_BUTTON72, m_btn72);
	DDX_Control(pDX, IDC_BUTTON73, m_btn73);
	DDX_Control(pDX, IDC_BUTTON74, m_btn74);
	DDX_Control(pDX, IDC_BUTTON75, m_btn75);
	DDX_Control(pDX, IDC_BUTTON76, m_btn76);
	DDX_Control(pDX, IDC_BUTTON77, m_btn77);
	DDX_Control(pDX, IDC_BUTTON78, m_btn78);
	DDX_Control(pDX, IDC_BUTTON79, m_btn79);
	DDX_Control(pDX, IDC_BUTTON80, m_btn80);
	DDX_Control(pDX, IDC_BUTTON81, m_btn81);
	DDX_Control(pDX, IDC_BUTTON82, m_btn82);
	DDX_Control(pDX, IDC_BUTTON83, m_btn83);
	DDX_Control(pDX, IDC_BUTTON84, m_btn84);
	DDX_Control(pDX, IDC_BUTTON85, m_btn85);
	DDX_Control(pDX, IDC_BUTTON86, m_btn86);
	DDX_Control(pDX, IDC_BUTTON87, m_btn87);
	DDX_Control(pDX, IDC_BUTTON88, m_btn88);
	DDX_Control(pDX, IDC_BUTTON89, m_btn89);
	DDX_Control(pDX, IDC_BUTTON90, m_btn90);
	DDX_Control(pDX, IDC_BUTTON91, m_btn91);
	DDX_Control(pDX, IDC_BUTTON92, m_btn92);
	DDX_Control(pDX, IDC_BUTTON93, m_btn93);
	DDX_Control(pDX, IDC_BUTTON94, m_btn94);
	DDX_Control(pDX, IDC_BUTTON95, m_btn95);
	DDX_Control(pDX, IDC_BUTTON96, m_btn96);
}


void CDlgAutoParams::ResetCurBtn()
{
	map<int, S_DATAINFO>::iterator iter_datainfo;
	map<int, CRoundButton*>::iterator iter = m_mapBtn.begin();
	while(iter != m_mapBtn.end())
	{
		iter_datainfo = g_mapDataInfo.find(iter->first);
		if(iter_datainfo!= g_mapDataInfo.end())
		{
			if(iter->first == g_iCurBtnID)
				iter->second->SetCurrentActive(true,iter_datainfo->second.iStatus);
			else
				iter->second->SetCurrentActive(false,iter_datainfo->second.iStatus);
		}
		else
		{
			iter->second->SetCurrentActive(false,-1);
		}
		iter++;
	}
}

BEGIN_MESSAGE_MAP(CDlgAutoParams, CDialogEx)
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
	ON_COMMAND_RANGE(IDC_BUTTON1,IDC_BUTTON1+95,&CDlgAutoParams::OnBnClickedButtonX)
	ON_BN_CLICKED(IDC_BUTTON_START, &CDlgAutoParams::OnBnClickedButtonStart)
	ON_BN_CLICKED(IDC_BUTTON_END, &CDlgAutoParams::OnBnClickedButtonEnd)
	ON_BN_CLICKED(IDC_BUTTON_STARTSCAN, &CDlgAutoParams::OnBnClickedButtonStartscan)
	ON_BN_CLICKED(IDC_BUTTON_GOOUT, &CDlgAutoParams::OnBnClickedButtonGoout)
END_MESSAGE_MAP()


// CDlgAutoParams 消息处理程序


BOOL CDlgAutoParams::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	CButton* radio=(CButton*)GetDlgItem(IDC_RADIO1);
	radio->SetCheck(1);

	SetButtonState(&m_btn_start);
	SetButtonState(&m_btn_end);
	SetButtonState(&m_btn_startscan);
	SetButtonState(&m_btn_goout);
	CINScanLiteDlg * pDlg=(CINScanLiteDlg*)GetParent();
	pDlg->m_ZLanguage.SetCreateLanguageState(FALSE);
	pDlg->m_ZLanguage.SetDialogStrings((long)(CDialog*)this,IDD);

	SetControlFont();

	m_btn_end.EnableWindow(FALSE);
	m_mapBtn.insert(pair<int, CRoundButton*>(IDC_BUTTON1, &m_btn1));
	m_mapBtn.insert(pair<int, CRoundButton*>(IDC_BUTTON2 , &m_btn2 ));
	m_mapBtn.insert(pair<int, CRoundButton*>(IDC_BUTTON3 , &m_btn3 ));
	m_mapBtn.insert(pair<int, CRoundButton*>(IDC_BUTTON4 , &m_btn4 ));
	m_mapBtn.insert(pair<int, CRoundButton*>(IDC_BUTTON5 , &m_btn5 ));
	m_mapBtn.insert(pair<int, CRoundButton*>(IDC_BUTTON6 , &m_btn6 ));
	m_mapBtn.insert(pair<int, CRoundButton*>(IDC_BUTTON7 , &m_btn7 ));
	m_mapBtn.insert(pair<int, CRoundButton*>(IDC_BUTTON8 , &m_btn8 ));
	m_mapBtn.insert(pair<int, CRoundButton*>(IDC_BUTTON9 , &m_btn9 ));
	m_mapBtn.insert(pair<int, CRoundButton*>(IDC_BUTTON10, &m_btn10));
	m_mapBtn.insert(pair<int, CRoundButton*>(IDC_BUTTON11, &m_btn11));
	m_mapBtn.insert(pair<int, CRoundButton*>(IDC_BUTTON12, &m_btn12));
	m_mapBtn.insert(pair<int, CRoundButton*>(IDC_BUTTON13, &m_btn13));
	m_mapBtn.insert(pair<int, CRoundButton*>(IDC_BUTTON14, &m_btn14));
	m_mapBtn.insert(pair<int, CRoundButton*>(IDC_BUTTON15, &m_btn15));
	m_mapBtn.insert(pair<int, CRoundButton*>(IDC_BUTTON16, &m_btn16));
	m_mapBtn.insert(pair<int, CRoundButton*>(IDC_BUTTON17, &m_btn17));
	m_mapBtn.insert(pair<int, CRoundButton*>(IDC_BUTTON18, &m_btn18));
	m_mapBtn.insert(pair<int, CRoundButton*>(IDC_BUTTON19, &m_btn19));
	m_mapBtn.insert(pair<int, CRoundButton*>(IDC_BUTTON20, &m_btn20));
	m_mapBtn.insert(pair<int, CRoundButton*>(IDC_BUTTON21, &m_btn21));
	m_mapBtn.insert(pair<int, CRoundButton*>(IDC_BUTTON22, &m_btn22));
	m_mapBtn.insert(pair<int, CRoundButton*>(IDC_BUTTON23, &m_btn23));
	m_mapBtn.insert(pair<int, CRoundButton*>(IDC_BUTTON24, &m_btn24));
	m_mapBtn.insert(pair<int, CRoundButton*>(IDC_BUTTON25, &m_btn25));
	m_mapBtn.insert(pair<int, CRoundButton*>(IDC_BUTTON26, &m_btn26));
	m_mapBtn.insert(pair<int, CRoundButton*>(IDC_BUTTON27, &m_btn27));
	m_mapBtn.insert(pair<int, CRoundButton*>(IDC_BUTTON28, &m_btn28));
	m_mapBtn.insert(pair<int, CRoundButton*>(IDC_BUTTON29, &m_btn29));
	m_mapBtn.insert(pair<int, CRoundButton*>(IDC_BUTTON30, &m_btn30));
	m_mapBtn.insert(pair<int, CRoundButton*>(IDC_BUTTON31, &m_btn31));
	m_mapBtn.insert(pair<int, CRoundButton*>(IDC_BUTTON32, &m_btn32));
	m_mapBtn.insert(pair<int, CRoundButton*>(IDC_BUTTON33, &m_btn33));
	m_mapBtn.insert(pair<int, CRoundButton*>(IDC_BUTTON34, &m_btn34));
	m_mapBtn.insert(pair<int, CRoundButton*>(IDC_BUTTON35, &m_btn35));
	m_mapBtn.insert(pair<int, CRoundButton*>(IDC_BUTTON36, &m_btn36));
	m_mapBtn.insert(pair<int, CRoundButton*>(IDC_BUTTON37, &m_btn37));
	m_mapBtn.insert(pair<int, CRoundButton*>(IDC_BUTTON38, &m_btn38));
	m_mapBtn.insert(pair<int, CRoundButton*>(IDC_BUTTON39, &m_btn39));
	m_mapBtn.insert(pair<int, CRoundButton*>(IDC_BUTTON40, &m_btn40));
	m_mapBtn.insert(pair<int, CRoundButton*>(IDC_BUTTON41, &m_btn41));
	m_mapBtn.insert(pair<int, CRoundButton*>(IDC_BUTTON42, &m_btn42));
	m_mapBtn.insert(pair<int, CRoundButton*>(IDC_BUTTON43, &m_btn43));
	m_mapBtn.insert(pair<int, CRoundButton*>(IDC_BUTTON44, &m_btn44));
	m_mapBtn.insert(pair<int, CRoundButton*>(IDC_BUTTON45, &m_btn45));
	m_mapBtn.insert(pair<int, CRoundButton*>(IDC_BUTTON46, &m_btn46));
	m_mapBtn.insert(pair<int, CRoundButton*>(IDC_BUTTON47, &m_btn47));
	m_mapBtn.insert(pair<int, CRoundButton*>(IDC_BUTTON48, &m_btn48));
	m_mapBtn.insert(pair<int, CRoundButton*>(IDC_BUTTON49, &m_btn49));
	m_mapBtn.insert(pair<int, CRoundButton*>(IDC_BUTTON50, &m_btn50));
	m_mapBtn.insert(pair<int, CRoundButton*>(IDC_BUTTON51, &m_btn51));
	m_mapBtn.insert(pair<int, CRoundButton*>(IDC_BUTTON52, &m_btn52));
	m_mapBtn.insert(pair<int, CRoundButton*>(IDC_BUTTON53, &m_btn53));
	m_mapBtn.insert(pair<int, CRoundButton*>(IDC_BUTTON54, &m_btn54));
	m_mapBtn.insert(pair<int, CRoundButton*>(IDC_BUTTON55, &m_btn55));
	m_mapBtn.insert(pair<int, CRoundButton*>(IDC_BUTTON56, &m_btn56));
	m_mapBtn.insert(pair<int, CRoundButton*>(IDC_BUTTON57, &m_btn57));
	m_mapBtn.insert(pair<int, CRoundButton*>(IDC_BUTTON58, &m_btn58));
	m_mapBtn.insert(pair<int, CRoundButton*>(IDC_BUTTON59, &m_btn59));
	m_mapBtn.insert(pair<int, CRoundButton*>(IDC_BUTTON60, &m_btn60));
	m_mapBtn.insert(pair<int, CRoundButton*>(IDC_BUTTON61, &m_btn61));
	m_mapBtn.insert(pair<int, CRoundButton*>(IDC_BUTTON62, &m_btn62));
	m_mapBtn.insert(pair<int, CRoundButton*>(IDC_BUTTON63, &m_btn63));
	m_mapBtn.insert(pair<int, CRoundButton*>(IDC_BUTTON64, &m_btn64));
	m_mapBtn.insert(pair<int, CRoundButton*>(IDC_BUTTON65, &m_btn65));
	m_mapBtn.insert(pair<int, CRoundButton*>(IDC_BUTTON66, &m_btn66));
	m_mapBtn.insert(pair<int, CRoundButton*>(IDC_BUTTON67, &m_btn67));
	m_mapBtn.insert(pair<int, CRoundButton*>(IDC_BUTTON68, &m_btn68));
	m_mapBtn.insert(pair<int, CRoundButton*>(IDC_BUTTON69, &m_btn69));
	m_mapBtn.insert(pair<int, CRoundButton*>(IDC_BUTTON70, &m_btn70));
	m_mapBtn.insert(pair<int, CRoundButton*>(IDC_BUTTON71, &m_btn71));
	m_mapBtn.insert(pair<int, CRoundButton*>(IDC_BUTTON72, &m_btn72));
	m_mapBtn.insert(pair<int, CRoundButton*>(IDC_BUTTON73, &m_btn73));
	m_mapBtn.insert(pair<int, CRoundButton*>(IDC_BUTTON74, &m_btn74));
	m_mapBtn.insert(pair<int, CRoundButton*>(IDC_BUTTON75, &m_btn75));
	m_mapBtn.insert(pair<int, CRoundButton*>(IDC_BUTTON76, &m_btn76));
	m_mapBtn.insert(pair<int, CRoundButton*>(IDC_BUTTON77, &m_btn77));
	m_mapBtn.insert(pair<int, CRoundButton*>(IDC_BUTTON78, &m_btn78));
	m_mapBtn.insert(pair<int, CRoundButton*>(IDC_BUTTON79, &m_btn79));
	m_mapBtn.insert(pair<int, CRoundButton*>(IDC_BUTTON80, &m_btn80));
	m_mapBtn.insert(pair<int, CRoundButton*>(IDC_BUTTON81, &m_btn81));
	m_mapBtn.insert(pair<int, CRoundButton*>(IDC_BUTTON82, &m_btn82));
	m_mapBtn.insert(pair<int, CRoundButton*>(IDC_BUTTON83, &m_btn83));
	m_mapBtn.insert(pair<int, CRoundButton*>(IDC_BUTTON84, &m_btn84));
	m_mapBtn.insert(pair<int, CRoundButton*>(IDC_BUTTON85, &m_btn85));
	m_mapBtn.insert(pair<int, CRoundButton*>(IDC_BUTTON86, &m_btn86));
	m_mapBtn.insert(pair<int, CRoundButton*>(IDC_BUTTON87, &m_btn87));
	m_mapBtn.insert(pair<int, CRoundButton*>(IDC_BUTTON88, &m_btn88));
	m_mapBtn.insert(pair<int, CRoundButton*>(IDC_BUTTON89, &m_btn89));
	m_mapBtn.insert(pair<int, CRoundButton*>(IDC_BUTTON90, &m_btn90));
	m_mapBtn.insert(pair<int, CRoundButton*>(IDC_BUTTON91, &m_btn91));
	m_mapBtn.insert(pair<int, CRoundButton*>(IDC_BUTTON92, &m_btn92));
	m_mapBtn.insert(pair<int, CRoundButton*>(IDC_BUTTON93, &m_btn93));
	m_mapBtn.insert(pair<int, CRoundButton*>(IDC_BUTTON94, &m_btn94));
	m_mapBtn.insert(pair<int, CRoundButton*>(IDC_BUTTON95, &m_btn95));
	m_mapBtn.insert(pair<int, CRoundButton*>(IDC_BUTTON96, &m_btn96));
	RefreshButton();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

BOOL CDlgAutoParams::PreTranslateMessage(MSG* pMsg)
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

HBRUSH CDlgAutoParams::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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

void CDlgAutoParams::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	CRect rect;
	GetClientRect(rect);
	dc.FillSolidRect(rect,clr);
	CDialog::OnPaint();
}

void CDlgAutoParams::SetControlFont()
{
	CFont *m_Font;

	if (sInfo.nLanguage==1)
		m_Font=m_FontEnglish;
	else
		m_Font=m_FontChinese;

	GetDlgItem(IDC_BUTTON_START)->SetFont(m_Font);
	GetDlgItem(IDC_BUTTON_END)->SetFont(m_Font);
	GetDlgItem(IDC_BUTTON_STARTSCAN)->SetFont(m_Font);
	GetDlgItem(IDC_BUTTON_GOOUT)->SetFont(m_Font);
}


void CDlgAutoParams::OnBnClickedButtonStart()
{
	// TODO: 在此添加控件通知处理程序代码
	map<int, CRoundButton*>::iterator iter = m_mapGroupBtn.begin();
	for (; iter != m_mapGroupBtn.end(); iter++)
	{
		iter->second->SetGroup( false);
	}
	m_mapGroupBtn.clear();
	m_btn_start.EnableWindow(FALSE);
	m_btn_end.EnableWindow(TRUE);
	m_bSelectStart = true;
}


void CDlgAutoParams::OnBnClickedButtonEnd()
{
	// TODO: 在此添加控件通知处理程序代码
	m_btn_end.EnableWindow(FALSE);
	m_bSelectEnd = true;
}

void CDlgAutoParams::OnBnClickedButtonX(UINT uID)
{
	g_iCurBtnID = uID;
	ResetCurBtn();
	map<int, CRoundButton*>::iterator iter = m_mapBtn.begin();
	while(iter != m_mapBtn.end())
	{
		if(uID == iter->first)
		{
			if(m_bSelectStart)
			{
				m_iStartBtnID = uID;
				iter->second->SetGroup(true);
				m_bSelectStart = false;
			}
			if(m_bSelectEnd)
			{
				m_iEndBtnID = uID;
				iter->second->SetGroup(true);
				m_bSelectEnd = m_bSelectStart = false;
				m_btn_start.EnableWindow(TRUE);
				m_btn_end.EnableWindow(TRUE);
				int iTemp = m_iStartBtnID;
				if(m_iStartBtnID > m_iEndBtnID )
				{
					m_iEndBtnID = m_iStartBtnID;
					m_iStartBtnID = iTemp;
				}
				int iCount = abs(m_iEndBtnID-m_iStartBtnID)+1;
				while(iCount--)
				{
					m_mapGroupBtn.insert(pair<int, CRoundButton*>(iTemp, m_mapBtn[iTemp]));
					m_mapBtn[iTemp]->SetGroup(true);
					iTemp++;
				}
			}
				//iter->second->SetCurrentActive(true, 0);
			break;
		}
		iter++;
	}

}


void CDlgAutoParams::OnBnClickedButtonStartscan()
{
	// TODO: 在此添加控件通知处理程序代码
	CINScanLiteDlg * pDlg = (CINScanLiteDlg *)AfxGetApp()->GetMainWnd();
	int Temp=0;
	m_ZolixCCD.GetTemper(&Temp);
	if(Temp > -5)
	{
		if(MessageBox(pDlg->LoadStringMeau(_T("ID_START_FAIL")),pDlg->LoadStringMeau(_T("ID_WARNING")),MB_OKCANCEL|MB_ICONWARNING) == IDCANCEL)
			return;
	}

	if(m_iStartBtnID <= 0 || m_iEndBtnID <= 0) 
	{
		MessageBox(pDlg->LoadStringMeau(_T("ID_CHOOSE_SAMPLE")));
		return;
	}

	int iCount = abs(m_iEndBtnID-m_iStartBtnID)+1;
	int iIndexStart = (m_iStartBtnID - 1300)/12;
	int iIndexEnd = (m_iEndBtnID - 1300)/12;
	int iTempID = m_iStartBtnID;
	int iOrder = 1;
	int iOrder_R = 0;
	while(iCount--)
	{
		int iTemp = iTempID - 1300;

		if(g_iScanOrder == 1)
		{
			S_DATAINFO info;
			if(iIndexEnd == iIndexStart)
			{
				info.iPosX = iTemp%12*KJJ;
				info.iPosY = iTemp/12*KJJ;
				info.iStatus = 0;
				info.iOrder = iOrder++;
				m_mapBtn[iTempID]->GetWindowText(info.strLineName);
				g_mapDataInfo.insert(pair<int, S_DATAINFO>(iTempID, info));	
			}
			else
			{
				if(((iTemp/12)-iIndexStart)%2 == 0)//正向
				{
					info.iPosX = iTemp%12*KJJ;
					info.iPosY = iTemp/12*KJJ;
					info.iStatus = 0;
					info.iOrder = iOrder++;
					iOrder_R = 0;
				}
				else //反向
				{
					if(iTemp/12 == iIndexEnd)//已经是选中的最后一行
					{
						if(iOrder_R == 0)
						{
							iOrder = iOrder + (12 - ((iIndexEnd+1)*12 - (m_iEndBtnID - 1300)));
							iOrder_R = iOrder;
						}
						info.iPosX = iTemp%12*KJJ;
						info.iPosY = iTemp/12*KJJ;
						info.iStatus = 0;
						info.iOrder = iOrder_R--;
					}
					else if(iTemp/12 < iIndexEnd)//不是选中的最后一行
					{
						if(iOrder_R == 0)
						{
							iOrder_R = iOrder+11;
							iOrder = iOrder_R+1;
						}
						info.iPosX = iTemp%12*KJJ;
						info.iPosY = iTemp/12*KJJ;
						info.iStatus = 0;
						info.iOrder = iOrder_R--;
					}
				}
				m_mapBtn[iTempID]->GetWindowText(info.strLineName);
				g_mapDataInfo.insert(pair<int, S_DATAINFO>(iTempID, info));	
			}
		}
		else if(g_iScanOrder == 2)
		{
			S_DATAINFO info;
			info.iPosX = iTemp%12*KJJ;
			info.iPosY = iTemp/12*KJJ;
			info.iStatus = 0;
			m_mapBtn[iTempID]->GetWindowText(info.strLineName);
			g_mapDataInfo.insert(pair<int, S_DATAINFO>(iTempID, info));
		}
		iTempID++;
	}

	OnOK();
}


void CDlgAutoParams::OnBnClickedButtonGoout()
{
	// TODO: 在此添加控件通知处理程序代码
	g_mapDataInfo.clear();
	map<int, CRoundButton*>::iterator iter = m_mapBtn.begin();
	while(iter != m_mapBtn.end())
	{
		iter->second->ResetBtn();
		iter->second->SetCurrentActive(false,-1);
		iter++;
	}
	m_btn_start.EnableWindow(TRUE);
	m_btn_end.EnableWindow(FALSE);
	m_iStartBtnID = 0;
	m_iEndBtnID = 0;
	//OnCancel();
}

void CDlgAutoParams::RefreshButton()
{
	int iMin = 100;
	int iCurBtn = 0;
	if (g_mapDataInfo.size() > 0)
	{
		map<int, S_DATAINFO>::iterator iter = g_mapDataInfo.begin();
		while (iter != g_mapDataInfo.end())
		{
			//if(iter->second.iStatus == 0)
			//{
			//	int iCur = iter->second.iOrder;
			//	iMin = iMin < iCur ? iMin : iCur;
			//	if(iMin < iCur)
			//	{
			//		iMin = iCur;
			//		iCurBtn = iter->first;
			//	}
			//	//m_mapBtn[iter->first]->SetCurrentActive(true, 0);
			//	//break;
			//}
			//else 
			if (iter->second.iStatus == 1)
			{
				m_mapBtn[iter->first]->SetCurrentActive(false, 1);
			}
			iter++;
		}
		m_mapBtn[g_iCurBtnID]->SetCurrentActive(true, 0);
	}
	else
	{
		ResetCurBtn();
	}
}

int CDlgAutoParams::GetMinIndex()
{
	int iMin = 100;
	map<int, S_DATAINFO>::iterator iter = g_mapDataInfo.begin();
	while (iter != g_mapDataInfo.end())
	{
		if(iter->second.iStatus == 0)
		{
			int iCur = iter->second.iOrder;
			iMin = iMin < iCur ? iMin : iCur;
		}
		iter++;
	}
	return iMin;
}
