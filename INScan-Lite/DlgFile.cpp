// DlgFile.cpp : 实现文件
//

#include "stdafx.h"
#include "INScan-Lite.h"
#include "DlgFile.h"
#include "afxdialogex.h"
#include "INScan-LiteDlg.h"
#include "DlgPrint.h"
#include "SaveFilesDlg.h"
#include "..\file\TeeChart2011\valuelist.h"
#include "..\file\TeeChart2011\series.h"
#include "..\file\TeeChart2011\axis.h"
#include "..\file\TeeChart2011\axes.h"
#include "..\file\TeeChart2011\zoom.h"
#include "..\file\TeeChart2011\lineseries.h"
#include "..\file\TeeChart2011\printer.h"
#include "..\file\TeeChart2011\pen.h"
#include "..\file\TeeChart2011\marks.h"
#include "..\file\TeeChart2011\markscallout.h"
#include "..\file\TeeChart2011\teefont.h"
#include "..\file\TeeChart2011\legend.h"
// CDlgFile 对话框

IMPLEMENT_DYNAMIC(CDlgFile, CDialog)

CDlgFile::CDlgFile(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgFile::IDD, pParent)
{
	m_bSelectAll = false;
}

CDlgFile::~CDlgFile()
{
}

void CDlgFile::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_OPEN, m_BtnOpen);
	DDX_Control(pDX, IDC_BUTTON_SAVE, m_BtnSave);
	DDX_Control(pDX, IDC_BUTTON_PRINT, m_BtnPrint);
	DDX_Control(pDX, IDC_BUTTON_SELECT, m_BtnSelect);
}


BEGIN_MESSAGE_MAP(CDlgFile, CDialog)
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BUTTON_OPEN, &CDlgFile::OnBnClickedButtonOpen)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, &CDlgFile::OnBnClickedButtonSave)
	ON_BN_CLICKED(IDC_BUTTON_PRINT, &CDlgFile::OnBnClickedButtonPrint)
	ON_BN_CLICKED(IDC_BUTTON_SELECT, &CDlgFile::OnBnClickedButtonSelect)
END_MESSAGE_MAP()


// CDlgFile 消息处理程序


void CDlgFile::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	CRect rect;
	GetClientRect(rect);
	dc.FillSolidRect(rect,clr);
	CDialog::OnPaint();
}


HBRUSH CDlgFile::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性

	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}


BOOL CDlgFile::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetButtonState(&m_BtnSave);
	SetButtonState(&m_BtnOpen);
	SetButtonState(&m_BtnPrint);
	SetButtonState(&m_BtnSelect);

	m_FontChinese.CreatePointFont(nFontSize,strFontChinese);
	m_FontEnglish.CreatePointFont(nFontSize,strFontEngish);


	InitDialog();
	SetControlFont();
	if (sInfo.nLanguage==1)
		m_BtnSelect.SetWindowTextA("Select All");
	else
		m_BtnSelect.SetWindowTextA("全选");

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDlgFile::InitDialog()
{
	m_DlgDataCom.Create(IDD_DLG_DATACOM,this);
	m_DlgDataPro.Create(IDD_DLG_DATAFILE,this);
	CRect rect;
	GetDlgItem(IDC_STATIC_FILE)->GetWindowRect(rect);
	GetDlgItem(IDC_STATIC_FILE)->ShowWindow(SW_HIDE);	
	ScreenToClient(&rect);
	::SetWindowPos(m_DlgDataCom.m_hWnd,HWND_TOP,rect.left,rect.top,rect.Width(),rect.Height()*3,SWP_SHOWWINDOW|SWP_FRAMECHANGED);
	::SetWindowPos(m_DlgDataPro.m_hWnd,HWND_TOP,rect.left,rect.top,rect.Width(),rect.Height()*3,SWP_SHOWWINDOW|SWP_FRAMECHANGED);

	m_DlgDataCom.ModifyStyle(0,WS_CLIPSIBLINGS);
	m_DlgDataCom.SetParent(this);
	m_DlgDataPro.ModifyStyle(0,WS_CLIPSIBLINGS);
	m_DlgDataPro.SetParent(this);
}
void CDlgFile::InitDialogShow(int nID)
{
	m_DlgDataCom.ShowWindow(SW_HIDE);
	m_DlgDataPro.ShowWindow(SW_HIDE);

	switch(nID)
	{
	case 1:
		m_DlgDataCom.ShowWindow(SW_SHOW);
		break;
	case 0:
		m_DlgDataPro.ShowWindow(SW_SHOW);
		break;
	}
}
void CDlgFile::SetControlFont()
{
	CFont *m_Font;

	if (sInfo.nLanguage==1)
		m_Font=&m_FontEnglish;
	else
		m_Font=&m_FontChinese;

	GetDlgItem(IDC_BUTTON_OPEN)->SetFont(m_Font);
	GetDlgItem(IDC_BUTTON_SAVE)->SetFont(m_Font);
	GetDlgItem(IDC_BUTTON_PRINT)->SetFont(m_Font);
	GetDlgItem(IDC_BUTTON_SELECT)->SetFont(m_Font);
}
void CDlgFile::UpdateLanguage()
{
	CINScanLiteDlg * pDlg=(CINScanLiteDlg*)GetParent();
	pDlg->m_ZLanguage.SetCreateLanguageState(FALSE);
	pDlg->m_ZLanguage.SetDialogStrings((long)(CDialog*)this,IDD);

	m_DlgDataCom.UpdateLanguage();
	m_DlgDataPro.UpdateLanguage();
	if (sInfo.nLanguage==1)
	{
		if(!m_bSelectAll)
			m_BtnSelect.SetWindowTextA("Select All");
		else
			m_BtnSelect.SetWindowTextA("Select None");
	}
	else
	{
		if(!m_bSelectAll)
			m_BtnSelect.SetWindowTextA("全选");
		else
			m_BtnSelect.SetWindowTextA("全不选");
	}

	SetControlFont();
}
BOOL CDlgFile::PreTranslateMessage(MSG* pMsg)
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


void CDlgFile::OnBnClickedButtonOpen()
{
	CINScanLiteDlg * pDlg = (CINScanLiteDlg *)AfxGetApp()->GetMainWnd();

	CFileDialog dlg(TRUE,NULL,NULL,OFN_ALLOWMULTISELECT,
		"TxtFile(*.txt)|*.txt|Csv(*.csv)|*.csv||");

	// Create buffer for file names.
	const DWORD numberOfFileNames = 96;
	const DWORD fileNameMaxLength = MAX_PATH + 1;
	const DWORD bufferSize = (numberOfFileNames * fileNameMaxLength) + 1;
	TCHAR* filenamesBuffer = new TCHAR[bufferSize];

	// Initialize beginning and end of buffer.
	filenamesBuffer[0] = NULL;//必à?须?的ì?
	filenamesBuffer[bufferSize-1] = NULL;

	// Attach buffer to OPENFILENAME member.
	dlg.m_ofn.lpstrFile = filenamesBuffer;
	dlg.m_ofn.nMaxFile = bufferSize;

	// Create array for file names.
	CString fileNameArray[numberOfFileNames];

	if(dlg.DoModal() == IDOK)
	{
		// Retrieve file name(s).
		POSITION fileNamesPosition = dlg.GetStartPosition();
		int iCtr = 0;

		int currentLineNum = 0;
		for(int i = 0; i < pDlg->m_DlgShow.m_Tchart.GetSeriesCount(); i++)
		{
			if(pDlg->m_DlgShow.m_Tchart.Series(i).GetShowInLegend() == true)
				currentLineNum++;
		}
		//  如果已有50条，则给出提示
		if(currentLineNum >= maxLineNum)
		{
			MessageBox(pDlg->LoadStringMeau(_T("ID_OUTOF_LINE_RANGE")));
			return;
		}
		while(fileNamesPosition != NULL)
		{
			fileNameArray[iCtr] = dlg.GetNextPathName(fileNamesPosition);
			iCtr++;
			////  最多只能打开96条
			//if (iCtr > maxLineNum)break;
			// 如果打开的加上已有的，大于50条，则给出提示，并只打开部分
			if(currentLineNum + iCtr > maxLineNum)
			{
				MessageBox(pDlg->LoadStringMeau(_T("ID_OPEN_TOO_MUCH")));
				iCtr--;
				break;
			}
		} 

		while(iCtr>0)
		{
			CString strFathName =fileNameArray[iCtr-1];
			BOOL bSt=FALSE;
			int n=strFathName.ReverseFind('.');
			if (n==-1)
			{
				return ;
			}
			if (pDlg->m_DlgShow.m_Tchart.GetSeriesCount()>96)
			{
				return;
			}
			bSt= pDlg->m_DlgShow.ImportFullLine(strFathName);

			if (bSt==FALSE)
			{
				MessageBox(pDlg->LoadStringMeau(_T("ID_FILE_FAIL")));	
			}
			else
			{
				pDlg->SetWindowText(strSoftName+_T(" |")+strFathName);
				sTInfo.KLMode=2;
			}

			iCtr--;
		}

	}
	//Release file names buffer.
	delete[] filenamesBuffer;


	/*CINScanLiteDlg * pDlg = (CINScanLiteDlg *)AfxGetApp()->GetMainWnd();
	CFileDialog dlg(TRUE,NULL,NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,
		"TxtFile(*.txt)|*.txt|Csv(*.csv)|*.csv||");

	BOOL bSt=FALSE;
	if (dlg.DoModal()!=IDOK)
	{
		return ;
	}

	CString strFathName =dlg.GetPathName();

	int n=strFathName.ReverseFind('.');
	if (n==-1)
	{
		return ;
	}
	CString fileStyle=strFathName.Right(strFathName.GetLength()-n-1);
	if(fileStyle==_T("txt"))
	{
		bSt= pDlg->m_DlgShow.ImportFullLine(strFathName);
	}
	if (fileStyle==_T("csv"))
	{
		bSt= pDlg->m_DlgShow.ImportFullLine(strFathName);
	}
	if (bSt==FALSE)
	{
		MessageBox(pDlg->LoadStringMeau(_T("ID_FILE_FAIL")));	
	}
	else
	{
		pDlg->SetWindowText(strSoftName+_T(" |")+strFathName);
		sTInfo.KLMode=2;
	}*/
}


void CDlgFile::OnBnClickedButtonSave()
{
	CINScanLiteDlg * pDlg = (CINScanLiteDlg *)AfxGetApp()->GetMainWnd();
	if (pDlg->m_DlgShow.m_Tchart.GetSeriesCount()>0)
	{
		CSaveFilesDlg dlg;
		dlg.DoModal();
	}
	
}


void CDlgFile::OnBnClickedButtonPrint()
{
	CDlgPrint dlg;
	dlg.DoModal();
}


void CDlgFile::OnBnClickedButtonSelect()
{
	// TODO: 在此添加控件通知处理程序代码
	CINScanLiteDlg * pDlg = (CINScanLiteDlg *)AfxGetApp()->GetMainWnd();
	if(m_bSelectAll)
	{
		m_bSelectAll = false;
		if (sInfo.nLanguage==1)
			m_BtnSelect.SetWindowTextA("Select All");
		else
			m_BtnSelect.SetWindowTextA("全选");
	}
	else
	{
		m_bSelectAll = true;
		if (sInfo.nLanguage==1)
			m_BtnSelect.SetWindowTextA("Select None");
		else
			m_BtnSelect.SetWindowTextA("全不选");
	}
	int iIndex = pDlg->m_DlgShow.m_Tchart.GetSeriesCount();
	while(iIndex > 0)
	{
		pDlg->m_DlgShow.m_Tchart.Series(iIndex-1).SetActive(m_bSelectAll);
		iIndex--;
	}
}
