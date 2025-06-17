// DlgDataPro.cpp : 实现文件
//

#include "stdafx.h"
#include "INScan-Lite.h"
#include "DlgDataPro.h"
#include "afxdialogex.h"
#include "INScan-LiteDlg.h"
#include "..\file\TeeChart2011\valuelist.h"
#include "..\file\TeeChart2011\series.h"
#include "..\file\TeeChart2011\axis.h"
#include "..\file\TeeChart2011\axes.h"
// CDlgDataPro 对话框

IMPLEMENT_DYNAMIC(CDlgDataPro, CDialog)

CDlgDataPro::CDlgDataPro(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgDataPro::IDD, pParent)
	, m_Edit(0)
{

}

CDlgDataPro::~CDlgDataPro()
{
}

void CDlgDataPro::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_COM, m_BtnCom);
	DDX_Control(pDX, IDC_BUTTON_SET, m_BtnSet);
	DDX_Control(pDX, IDC_LIST_DATA, m_ListCtrl);
	DDX_Text(pDX, IDC_EDIT_SET, m_Edit);
	DDX_Control(pDX, IDC_COMBO1, m_ComboName);
	DDX_Control(pDX, IDC_TCHART2, m_Tchart2);
}


BEGIN_MESSAGE_MAP(CDlgDataPro, CDialog)
	ON_WM_CTLCOLOR()
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BUTTON_SET, &CDlgDataPro::OnBnClickedButtonSet)
	ON_BN_CLICKED(IDC_BUTTON_COM, &CDlgDataPro::OnBnClickedButtonCom)
	ON_EN_CHANGE(IDC_EDIT_SET, &CDlgDataPro::OnEnChangeEditSet)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_DATA, &CDlgDataPro::OnNMDblclkListData)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CDlgDataPro::OnCbnSelchangeCombo1)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CDlgDataPro 消息处理程序


HBRUSH CDlgDataPro::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	if (nCtlColor==CTLCOLOR_STATIC)
	{
		pDC->SetBkColor(clr);
		HBRUSH b =CreateSolidBrush(clr);
		return b;
	}
	return hbr;
}


void CDlgDataPro::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	CRect rect;
	GetClientRect(rect);
	dc.FillSolidRect(rect,clr);
	CDialog::OnPaint();
}

//  初始化界面时
BOOL CDlgDataPro::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_FontChinese.CreatePointFont(nFontSize,strFontChinese);
	m_FontEnglish.CreatePointFont(nFontSize,strFontEngish);

	CINScanLiteDlg * pDlg=(CINScanLiteDlg*)AfxGetApp()->GetMainWnd();

	int nSeriesCount = pDlg->m_DlgShow.m_Tchart.GetSeriesCount();
	if (nSeriesCount==0) return TRUE;
	for (int i = 0 ;i<nSeriesCount;i++)
	{
		CString strSeriesName = pDlg->m_DlgShow.m_Tchart.Series(i).GetTitle();
		m_ComboName.InsertString(i,strSeriesName);
	}
	// 选择最后一条线
	m_ComboName.SetCurSel(nSeriesCount - 1);
	
	SetButtonState(&m_BtnCom);
	SetButtonState(&m_BtnSet);

	m_Edit=sSInfo.DataPer;
	SetControlFont();
	UpdateLanguage();
	sSInfo.SeriesNum=0;
	ShowData(0);
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

//  根据索引显示曲线
void CDlgDataPro::ShowData(int nSerr)
{
	CINScanLiteDlg *pDlg = (CINScanLiteDlg*)AfxGetApp()->GetMainWnd();

	if (m_bAutoRunning==FALSE&&m_bAutoRunningStatue==FALSE)
	{
		if(pDlg->m_DlgShow.GetTeeCount()>0)
		{
			m_Tchart2.RemoveAllSeries();
			m_Tchart2.AddSeries(0);
			CValueList yvaluelist;
			//int serr=0;
			int ax =pDlg->m_DlgShow.m_Tchart.GetSeriesCount();
			yvaluelist = pDlg->m_DlgShow.m_Tchart.Series(nSerr).GetYValues();
			int count=pDlg->m_DlgShow.m_Tchart.Series(nSerr).GetCount();
			double yxis=0;
			CValueList xvaluelist;
			xvaluelist=pDlg->m_DlgShow.m_Tchart.Series(nSerr).GetXValues();

			for(int i=0;i<count;i++)
			{
				m_Tchart2.Series(0).AddXY(xvaluelist.GetValue(i),yvaluelist.GetValue(i),NULL,clrSet);
			}
		}

	}

	m_Tchart2.GetAxis().GetLeft().SetAutomatic(TRUE);
	m_Tchart2.GetAxis().GetBottom().SetAutomatic(TRUE);

}

void CDlgDataPro::SetControlFont()
{
	CFont *m_Font;

	if (sInfo.nLanguage==1)
		m_Font=&m_FontEnglish;
	else
		m_Font=&m_FontChinese;

	GetDlgItem(IDC_BUTTON_SET)->SetFont(m_Font);
	GetDlgItem(IDC_BUTTON_COM)->SetFont(m_Font);

	GetDlgItem(IDC_LIST_DATA)->SetFont(m_Font);
	GetDlgItem(IDC_STATIC_SET)->SetFont(m_Font);
	GetDlgItem(IDC_STATIC_B)->SetFont(m_Font);
	GetDlgItem(IDC_STATIC_NAME)->SetFont(m_Font);
	GetDlgItem(IDC_EDIT_SET)->SetFont(m_Font);

	CINScanLiteDlg * pDlg=(CINScanLiteDlg*)AfxGetApp()->GetMainWnd();

	m_ListCtrl.SetExtendedStyle(LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);
	while(m_ListCtrl.DeleteColumn(0));
	m_ListCtrl.InsertColumn(0,pDlg->LoadStringMeau(_T("ID_SQL_NAME")),LVCFMT_CENTER,130,0);
	m_ListCtrl.InsertColumn(1,pDlg->LoadStringMeau(_T("ID_SQL_NUMBER")),LVCFMT_CENTER,115,1);
	m_ListCtrl.SetFont(m_Font);

}
void CDlgDataPro::UpdateLanguage()
{
	CINScanLiteDlg * pDlg=(CINScanLiteDlg*)AfxGetApp()->GetMainWnd();
	pDlg->m_ZLanguage.SetCreateLanguageState(FALSE);
	pDlg->m_ZLanguage.SetDialogStrings((long)(CDialog*)this,IDD);

	SetControlFont();
	ShowList();
}

BOOL CDlgDataPro::PreTranslateMessage(MSG* pMsg)
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

//  设置相似度阈值
void CDlgDataPro::OnBnClickedButtonSet()
{
    UpdateData(TRUE);
	if (m_Edit>100)
	{
		m_Edit=100;
	}
	if (m_Edit<0)
	{
		m_Edit=0;
	}
	sSInfo.DataPer=m_Edit;
	UpdateData(FALSE);
	SetButtonState(&m_BtnSet);
	ShowList();
}


void CDlgDataPro::OnBnClickedButtonCom()
{
	CINScanLiteDlg * pDlg=(CINScanLiteDlg*)AfxGetApp()->GetMainWnd();
	GetDlgItem(IDC_BUTTON_COM)->EnableWindow(FALSE);
	CRect rect;
	pDlg->GetWindowRect(&rect);
    pDlg->pDlgWait->MoveWindow(rect.left,rect.top,rect.Width(),rect.Height());
	//pDlg->pDlgWait->SetWindowPos(m_hWnd,SWP_TOPMOST,rect.left,rect.top,rect.Width(),rect.Height());
	//pDlg->pDlgWait->ShowWindow(SW_SHOW);
	BOOL bRet=FALSE;
	ClearList();
	ShowData(m_ComboName.GetCurSel());	
	//pDlg->m_DlgShow.ReMoveSeries(1);
	sSInfo.bSQL=false;
	SetTimer(1,500,NULL);
	bRet=pDlg->m_DlgShow.SelectSql();
	if (bRet==FALSE)
	{
		KillTimer(1);
		GetDlgItem(IDC_BUTTON_COM)->EnableWindow(TRUE);
	}
}
void CDlgDataPro::ClearList()
{
	m_ListCtrl.DeleteAllItems();
	sDInfo.clear();
	sDInfo.shrink_to_fit();
}
void CDlgDataPro::ShowList()
{
	GetDlgItem(IDC_BUTTON_COM)->EnableWindow(TRUE);
	m_ListCtrl.DeleteAllItems();
	int j=0;
	if (sDInfo.size()<=0)
	{
		return;
	}
	for(int i=0; i < MaxTenData; i++)
	{
		CString strSimilarNum,strName;
		if (j<sDInfo.size()&&sDInfo[j].fSimilarNum*100>=sSInfo.DataPer)
		{
			strSimilarNum.Format("%.1f%%",sDInfo[j].fSimilarNum*100);
			if (sInfo.nLanguage==0)
			{
				strName=sDInfo[j].name_ch;
			}
			else
			{
				strName=sDInfo[j].name_eh;
			}
			m_ListCtrl.InsertItem(j,strName,j);
			m_ListCtrl.SetItemText(j,1,strSimilarNum);
			j++;
		}
		
	}
	sSInfo.DataPerNum=j;
	
}


void CDlgDataPro::OnEnChangeEditSet()
{
	SetCButtonState(&m_BtnSet);

}

void CDlgDataPro::AddSqlSer(int nChoose)
{
	m_Tchart2.GetAxis().GetLeft().SetAutomatic(TRUE);
	m_Tchart2.GetAxis().GetBottom().SetAutomatic(TRUE);
	m_Tchart2.GetAxis().GetRight().SetAutomatic(TRUE);
	m_Tchart2.GetAxis().GetTop().SetAutomatic(TRUE);
	DWORD len=0;
	COleSafeArray strx;
	COleSafeArray stry;
	strx.Clear();
	stry.Clear();
	len=sDInfo[nChoose].datalen;
	strx.Create(VT_R8, 1, &len); 
	stry.Create(VT_R8, 1, &len); 
	Split(sDInfo[nChoose].strXData,"\t",strx);
	Split(sDInfo[nChoose].strYData,"\t",stry);
	m_Tchart2.AddSeries(0);
	int count=m_Tchart2.GetSeriesCount();
	m_Tchart2.Series(count-1).AddArray(len,stry,strx);
}
void CDlgDataPro::OnNMDblclkListData(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

	int Choose=pNMItemActivate->iItem;
	if (Choose>=0)
	{
		AddSqlSer(Choose);

	}
	*pResult = 0;
}


void CDlgDataPro::OnCbnSelchangeCombo1()
{
	sSInfo.SeriesNum=m_ComboName.GetCurSel();
	ShowData(m_ComboName.GetCurSel());
	ClearList();
}


void CDlgDataPro::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent==1)
	{
		if (sSInfo.bSQL==TRUE)
		{
			KillTimer(1);
			ShowList();
			sSInfo.bSQL=false;
		}
	}

	CDialog::OnTimer(nIDEvent);
}
BEGIN_EVENTSINK_MAP(CDlgDataPro, CDialog)
	ON_EVENT(CDlgDataPro, IDC_TCHART2, 26, CDlgDataPro::OnUndoZoomTchart2, VTS_NONE)
END_EVENTSINK_MAP()


void CDlgDataPro::OnUndoZoomTchart2()
{
	// TODO: 在此处添加消息处理程序代码
	m_Tchart2.GetAxis().GetLeft().SetAutomatic(TRUE);
	m_Tchart2.GetAxis().GetBottom().SetAutomatic(TRUE);
}
