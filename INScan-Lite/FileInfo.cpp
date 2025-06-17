// FileInfo.cpp : 实现文件
//

#include "stdafx.h"
#include "INScan-Lite.h"
#include "INScan-LiteDlg.h"
#include "FileInfo.h"
#include "afxdialogex.h"


// CFileInfo 对话框

IMPLEMENT_DYNAMIC(CFileInfo, CDialog)

CFileInfo::CFileInfo(CWnd* pParent /*=NULL*/)
	: CDialog(CFileInfo::IDD, pParent)
	, m_bCheckSmooth(FALSE)
	, m_bCheckFit(FALSE)
	, m_bCheckCutOut(FALSE)
	, m_dNumStart(0)
	, m_dNumEnd(0)
	, m_nSmoothNum(0)
	, m_nFitNum(0)
	, m_bCheckHeader(true)
{

}

CFileInfo::~CFileInfo()
{
}

void CFileInfo::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_PATH, m_BtnPath);
	DDX_Control(pDX, IDC_BUTTON_SET, m_BtnSet);
	DDX_Control(pDX, IDC_BUTTON_NFO_SAVE, M_ButtonNfoSave);
	DDX_Check(pDX, IDC_CHECK_SMOOTH, m_bCheckSmooth);
	DDX_Check(pDX, IDC_CHECK_FIT, m_bCheckFit);
	DDX_Check(pDX, IDC_CHECK_CUT_OUT, m_bCheckCutOut);
	DDX_Control(pDX, IDC_EDIT_NUM_START, m_ctrlNumStart);
	DDX_Control(pDX, IDC_EDIT_NUM_END, m_ctrlNumEnd);
	DDX_Text(pDX, IDC_EDIT_NUM_START, m_dNumStart);
	DDX_Text(pDX, IDC_EDIT_NUM_END, m_dNumEnd);
	DDX_Control(pDX, IDC_COMBO_FILE_TYPE, m_ctrlComboFileType);
	DDX_Text(pDX, IDC_EDIT_NUM_SMOOTH, m_nSmoothNum);
	DDX_Text(pDX, IDC_EDIT_NUM_FIT, m_nFitNum);
	DDX_Control(pDX, IDC_EDIT_NUM_SMOOTH, m_ctrlEditSoomthNum);
	DDX_Control(pDX, IDC_EDIT_NUM_FIT, m_ctrlFitEditNum);
}


BEGIN_MESSAGE_MAP(CFileInfo, CDialog)
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BUTTON_SET, &CFileInfo::OnBnClickedButtonSet)
	ON_BN_CLICKED(IDC_BUTTON_PATH, &CFileInfo::OnBnClickedButtonPath)
	ON_EN_CHANGE(IDC_EDIT_HEAD, &CFileInfo::OnEnChangeEditHead)
	ON_BN_CLICKED(IDC_BUTTON_NFO_SAVE, &CFileInfo::OnBnClickedButtonNfoSave)
	ON_BN_CLICKED(IDC_CHECK_CUT_OUT, &CFileInfo::OnBnClickedCheckCutOut)
	ON_BN_CLICKED(IDC_CHECK_SMOOTH, &CFileInfo::OnBnClickedCheckSmooth)
	ON_BN_CLICKED(IDC_CHECK_FIT, &CFileInfo::OnBnClickedCheckFit)
	ON_BN_CLICKED(IDC_CHECK_HEADER, &CFileInfo::OnBnClickedCheckHeader)
END_MESSAGE_MAP()


// CFileInfo 消息处理程序


void CFileInfo::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	CRect rect;
	GetClientRect(rect);
	dc.FillSolidRect(rect,clr);
	CDialog::OnPaint();
}


HBRUSH CFileInfo::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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


BOOL CFileInfo::OnInitDialog()
{
	CDialog::OnInitDialog();

	

	if (sINInfo.strName==_T("")||sINInfo.strName==_T("0"))
	{
		sINInfo.strName=_T("new");
	}
	if (sINInfo.strPath==_T("")||sINInfo.strPath==_T("0"))
	{
		sINInfo.strPath=sInfo.Path+_T("\\DataTest");
	}

	GetDlgItem(IDC_EDIT_HEAD)->SetWindowText(sINInfo.strName);
	GetDlgItem(IDC_EDIT_PATH)->SetWindowText(sINInfo.strPath);
	m_bCheckSmooth = sINInfo.bCheakSmooth;
	m_bCheckFit = sINInfo.bCheakFit;
	m_bCheckCutOut = sINInfo.bCheakCut;

	m_bCheckHeader = sTInfo.AddHeader;
	((CButton*)GetDlgItem(IDC_CHECK_HEADER))->SetCheck(m_bCheckHeader);

	m_dNumStart = sINInfo.dEditStart;
	m_dNumEnd = sINInfo.dEditEnd;
	if (m_bCheckCutOut)
	{
		m_ctrlNumStart.EnableWindow(TRUE);
		m_ctrlNumEnd.EnableWindow(TRUE);
		
	}else{
		m_ctrlNumStart.EnableWindow(FALSE);
		m_ctrlNumEnd.EnableWindow(FALSE);
	}
	m_nSmoothNum = sINInfo.nSmooth;
	if (m_bCheckSmooth)
	{
		m_ctrlEditSoomthNum.EnableWindow(TRUE);
	} 
	else
	{
		m_ctrlEditSoomthNum.EnableWindow(FALSE);
	}
	m_nFitNum = sINInfo.nFit;
	if (m_bCheckFit)
	{
		m_ctrlFitEditNum.EnableWindow(TRUE);
	
	} 
	else
	{
		m_ctrlFitEditNum.EnableWindow(FALSE);
	}
	m_ctrlComboFileType.InsertString(0,_T("*.txt"));
	m_ctrlComboFileType.InsertString(1,_T("*.csv"));
	m_ctrlComboFileType.SetCurSel(sINInfo.nFileType==0?0:1);
	SetButtonState(&m_BtnPath);
	SetButtonState(&m_BtnSet);
	SetButtonState(&M_ButtonNfoSave);
	UpdateData(FALSE);
	UpdateLanguage();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CFileInfo::OnBnClickedButtonSet()
{
	SetButtonState(&m_BtnSet);
	GetDlgItem(IDC_EDIT_HEAD)->GetWindowText(sINInfo.strName);
	if(sINInfo.strName.IsEmpty())
	{
		sINInfo.strName=_T("new");
	}
}


void CFileInfo::OnBnClickedButtonPath()
{
	CString m_FileDir;
	BROWSEINFO bi;
	ZeroMemory(&bi, sizeof(BROWSEINFO));
	bi.hwndOwner = m_hWnd;
	bi.ulFlags = BIF_RETURNONLYFSDIRS;
	LPITEMIDLIST pidl = SHBrowseForFolder(&bi);
	BOOL bRet = FALSE;
	TCHAR szFolder[MAX_PATH*2];
	szFolder[0] = _T('\0');
	if (pidl)
	{
		if (SHGetPathFromIDList(pidl, szFolder))
			bRet = TRUE;
		IMalloc *pMalloc = NULL;
		if (SUCCEEDED(SHGetMalloc(&pMalloc))
			&& pMalloc)
		{
			pMalloc->Free(pidl);
			pMalloc->Release();
		}
		m_FileDir = szFolder;//选择的文件夹路径
		m_FileDir.Replace("\\","/");

		GetDlgItem(IDC_EDIT_PATH)->SetWindowText(m_FileDir);
		sINInfo.strPath=m_FileDir;
	}
}


void CFileInfo::OnEnChangeEditHead()
{
	SetCButtonState(&m_BtnSet);
}
void WriteIniInfo(CString strT,CString strInfo,CString strPath)
{
	WritePrivateProfileString(strT,strInfo,strPath,sInfo.strPath);
}
void CFileInfo::UpdateLanguage()
{
	CINScanLiteDlg * pDlg=(CINScanLiteDlg*)AfxGetApp()->GetMainWnd();
	pDlg->m_ZLanguage.SetCreateLanguageState(FALSE);
	pDlg->m_ZLanguage.SetDialogStrings((long)(CDialog*)this,IDD);

	//SetControlFont();
}

//  保存参数
void CFileInfo::OnBnClickedButtonNfoSave()
{
	// TODO: 在此添加控件通知处理程序代码
	OnBnClickedButtonSet();// lcz new 04/24

	UpdateData(TRUE);
	CString strTemp;
	sINInfo.bCheakSmooth = m_bCheckSmooth;
	sINInfo.bCheakFit = m_bCheckFit;
	sINInfo.bCheakCut = m_bCheckCutOut;
	strTemp.Format(_T("%d"),m_bCheckCutOut?1:0);
	WriteIniInfo(_T("INTIME"),_T("bCheakCut"),strTemp);
	strTemp.Format(_T("%d"),m_bCheckSmooth?1:0);
	WriteIniInfo(_T("INTIME"),_T("bCheakSmooth"),strTemp);
	strTemp.Format(_T("%d"),m_bCheckFit?1:0);
	WriteIniInfo(_T("INTIME"),_T("bCheakFit"),strTemp);
	if (m_bCheckCutOut)
	{
		sINInfo.dEditStart = m_dNumStart;
		sINInfo.dEditEnd = m_dNumEnd;
		strTemp.Format(_T("%f"),m_dNumStart);
		WriteIniInfo(_T("INTIME"),_T("dEditStart"),strTemp);
		strTemp.Format(_T("%f"),m_dNumEnd);
		WriteIniInfo(_T("INTIME"),_T("dEditEnd"),strTemp);
	} 
	if (m_bCheckSmooth)
	{
		sINInfo.nSmooth = m_nSmoothNum;
		
		strTemp.Format(_T("%d"),m_nSmoothNum);
		WriteIniInfo(_T("INTIME"),_T("nSmooth"),strTemp);
	} 
	
	if (m_bCheckFit)
	{
		sINInfo.nFit = m_nFitNum;
		strTemp.Format(_T("%d"),m_nFitNum);
		WriteIniInfo(_T("INTIME"),_T("nFit"),strTemp);
	} 
	sINInfo.nFileType = m_ctrlComboFileType.GetCurSel();
	strTemp.Format(_T("%d"),sINInfo.nFileType);
	WriteIniInfo(_T("INTIME"),_T("nFileType"),strTemp);

}


void CFileInfo::OnBnClickedCheckCutOut()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (m_bCheckCutOut)
	{
		m_ctrlNumStart.EnableWindow(TRUE);
		m_ctrlNumEnd.EnableWindow(TRUE);
	}else{
		m_ctrlNumStart.EnableWindow(FALSE);
		m_ctrlNumEnd.EnableWindow(FALSE);
	}

}


void CFileInfo::OnBnClickedCheckSmooth()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (m_bCheckSmooth)
	{
		m_ctrlEditSoomthNum.EnableWindow(TRUE);
	} 
	else
	{
		m_ctrlEditSoomthNum.EnableWindow(FALSE);
	}
}


void CFileInfo::OnBnClickedCheckFit()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (m_bCheckFit)
	{
		m_ctrlFitEditNum.EnableWindow(TRUE);
	} 
	else
	{
		m_ctrlFitEditNum.EnableWindow(FALSE);
	}
}


void CFileInfo::OnBnClickedCheckHeader()
{
	// TODO: 在此添加控件通知处理程序代码
	sTInfo.AddHeader = ((CButton*)GetDlgItem(IDC_CHECK_HEADER))->GetCheck();
}
