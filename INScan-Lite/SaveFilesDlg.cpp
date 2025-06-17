// SaveFilesDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "INScan-Lite.h"
#include "INScan-LiteDlg.h"
#include "SaveFilesDlg.h"
#include "afxdialogex.h"
#include "SetNameDlg.h"
char g_pPath[MAX_PATH]={0};

// CSaveFilesDlg 对话框

IMPLEMENT_DYNAMIC(CSaveFilesDlg, CDialogEx)

CSaveFilesDlg::CSaveFilesDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSaveFilesDlg::IDD, pParent)
	, m_strFilePath(_T(""))
{

}

CSaveFilesDlg::~CSaveFilesDlg()
{
}

void CSaveFilesDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CB_FILE_TYPE, m_ctrlCBFileType);
	DDX_Text(pDX, IDC_ET_FILE_PATH, m_strFilePath);
	DDX_Control(pDX, IDC_BT_CHOOSE_FILE_PATH, m_ctrlBtChooseFilePath);
	DDX_Control(pDX, IDC_BT_SAVE_FILES, m_ctrlBtSaveFiles);
}


BEGIN_MESSAGE_MAP(CSaveFilesDlg, CDialogEx)
	
	ON_BN_CLICKED(IDC_BT_CHOOSE_FILE_PATH, &CSaveFilesDlg::OnBnClickedBtChooseFilePath)
	ON_BN_CLICKED(IDC_BT_SAVE_FILES, &CSaveFilesDlg::OnBnClickedBtSaveFiles)
	ON_WM_CTLCOLOR()
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CSaveFilesDlg 消息处理程序


// 此回调函数为全局函数或静态函数;
int CALLBACK BrowseCallbackProc( HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData )
{
    switch(uMsg)
    {
    case BFFM_INITIALIZED:
        {
            ::SendMessage(hwnd, BFFM_SETSELECTION, TRUE, (LPARAM)g_pPath);
        }
        break;
	case BFFM_SELCHANGED:    //选择路径变化，
   {
		TCHAR curr[MAX_PATH];  
		SHGetPathFromIDList((LPCITEMIDLIST)lParam,curr);  
		::SendMessage(hwnd,BFFM_SETSTATUSTEXT,0,(LPARAM)curr);  
   }
   break;
    default:
        break;
    }
    return 0;
}


void CSaveFilesDlg::OnBnClickedBtChooseFilePath()
{
	// TODO: 在此添加控件通知处理程序代码
	TCHAR szDefaultDir[MAX_PATH] = {0};
	if(g_strDefSaveFilePath.GetLength() > 0)
	{
		CString strTemp = g_strDefSaveFilePath;
		strTemp.Replace("/","\\");
		USES_CONVERSION;
		ZeroMemory(g_pPath, sizeof(g_pPath));
		strcpy(g_pPath, T2A(strTemp.GetBuffer(0)));

	}
	CString m_FileDir;
	BROWSEINFO bi;
	ZeroMemory(&bi, sizeof(BROWSEINFO));
	bi.hwndOwner = m_hWnd;
	bi.ulFlags = BIF_RETURNONLYFSDIRS| BIF_USENEWUI | BIF_UAHINT | BIF_STATUSTEXT;
	bi.lpfn        = (BrowseCallbackProc);

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

		m_strFilePath = m_FileDir;
		g_strDefSaveFilePath = m_strFilePath;
		UpdateData(FALSE);
	}
}

void CSaveFilesDlg::UpdateLanguage()
{
	CINScanLiteDlg * pDlg=(CINScanLiteDlg*)GetParent();
	pDlg->m_ZLanguage.SetCreateLanguageState(FALSE);
	pDlg->m_ZLanguage.SetDialogStrings((long)(CDialog*)this,IDD);
}
void CSaveFilesDlg::OnBnClickedBtSaveFiles()
{
	// TODO: 在此添加控件通知处理程序代码
	m_strFilePath=_T("");
	OnBnClickedBtChooseFilePath();
	UpdateData(TRUE);
	CINScanLiteDlg * pDlg = (CINScanLiteDlg *)AfxGetApp()->GetMainWnd();
	int iRet=1;
	CString fileExt = m_ctrlCBFileType.GetCurSel()==0?"txt":"csv";

	if (m_strFilePath==_T(""))
	{
		iRet=0;
	}
	else
	{
		iRet=pDlg->m_DlgShow.ExportTxtFiles(m_strFilePath,fileExt);
	}

	if(iRet == 0)
	{
		MessageBox(pDlg->LoadStringMeau(_T("ID_FILE_SAVE_FAIL")));
	}
	else if(iRet == 1)
	{
		MessageBox(pDlg->LoadStringMeau(_T("ID_FILE_SAVE_SUCCESS")));	
	}
	
	//while(1)
	//{
	//	if (iRet == -1)
	//	{
	//		if(MessageBox(_T("是否重命名"), NULL, MB_YESNO) == IDYES)
	//		{
	//			CSetNameDlg dlg;
	//			dlg.DoModal();
	//			iRet=pDlg->m_DlgShow.ExportTxtFiles(m_strFilePath,fileExt);
	//		}
	//		else
	//		{
	//			iRet=pDlg->m_DlgShow.ExportTxtFiles(m_strFilePath,fileExt, true);
	//		}
	//	}
	//	else if(iRet == 0)
	//	{
	//		MessageBox(pDlg->LoadStringMeau(_T("ID_FILE_SAVE_FAIL")));
	//		break;
	//	}
	//	else if(iRet == 1)
	//	{
	//		MessageBox(pDlg->LoadStringMeau(_T("ID_FILE_SAVE_SUCCESS")));	
	//		break;
	//	}
	//}
	OnOK();
}


BOOL CSaveFilesDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	SetButtonState(&m_ctrlBtChooseFilePath);
	SetButtonState(&m_ctrlBtSaveFiles);
	m_ctrlCBFileType.InsertString(0,_T("*.(txt)"));
	m_ctrlCBFileType.InsertString(1,_T("*.(csv)"));
	m_ctrlCBFileType.SetCurSel(0);
	UpdateLanguage();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


HBRUSH CSaveFilesDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性
	if (nCtlColor==CTLCOLOR_STATIC)
	{
		pDC->SetBkColor(clr);
		HBRUSH b =CreateSolidBrush(clr);
		return b;
	}
	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}


void CSaveFilesDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialogEx::OnPaint()
	CRect rect;
	GetClientRect(rect);
	dc.FillSolidRect(rect,clr);
	CDialog::OnPaint();
}
