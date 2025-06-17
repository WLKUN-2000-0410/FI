// DlgAdvAndor.cpp : 实现文件
//

#include "stdafx.h"
#include "ZolixCCD.h"
#include "DlgAdvAndor.h"
#include "afxdialogex.h"
#include "IniFile.h"
#include "DlgAdvAndor.h"
#include "Language.h"
#include "CameraDll.h"
NS_MFC_Res_BEGIN
// CDlgAdvAndor 对话框

IMPLEMENT_DYNAMIC(CDlgAdvAndor, CDialog)

CDlgAdvAndor::CDlgAdvAndor(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgAdvAndor::IDD, pParent)
{

}

CDlgAdvAndor::~CDlgAdvAndor()
{
}

void CDlgAdvAndor::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_ADV_GHOST, stuAndorCCD.ghostImageStrRelative);
}


BEGIN_MESSAGE_MAP(CDlgAdvAndor, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_CCD, &CDlgAdvAndor::OnBnClickedButtonCcd)
	ON_CBN_SELCHANGE(IDC_COMBO_MIRROR, &CDlgAdvAndor::OnCbnSelchangeComboMirror)
	ON_BN_CLICKED(IDC_BUTTON_ADV_GHOST, &CDlgAdvAndor::OnBnClickedButtonGhost)
END_MESSAGE_MAP()

BOOL CDlgAdvAndor::OnInitDialog()
{
	CDialog::OnInitDialog();
	CString str;
	CLanguage mLag;
	CDialog *pDlg;
	pDlg=(CDialog *)this;
	mLag.UpdateLanguage(pDlg,ID_DLG_ADV,mLag.szDig[nLanguage],g_strPath);
	((CComboBox *)GetDlgItem(IDC_COMBO_MIRROR))->ResetContent();
	 str=mLag.ReadID(_T("ID_MIRROR_Z"),mLag.szDig[nLanguage]);
	((CComboBox *)GetDlgItem(IDC_COMBO_MIRROR))->AddString(str);
	 str=mLag.ReadID(_T("ID_MIRROR_F"),mLag.szDig[nLanguage]);
	((CComboBox *)GetDlgItem(IDC_COMBO_MIRROR))->AddString(str);
	((CComboBox *)GetDlgItem(IDC_COMBO_MIRROR))->SetCurSel(stuAndorCCD.isMirror);
	str.Format("%d",stuAndorCCD.iCbpNum);
	GetDlgItem(IDC_EDIT_CCDNUM)->SetWindowText(str);
	GetDlgItem(IDC_EDIT_CCD)->SetWindowText(stuAndorCCD.strCbp);

	ManageZolixIni->ReadIni();

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}
void CDlgAdvAndor::OnBnClickedButtonCcd()
{
	CString str;
	GetDlgItem(IDC_EDIT_CCD)->GetWindowText(str);
	stuAndorCCD.strCbp=str;
	GetDlgItem(IDC_EDIT_CCDNUM)->GetWindowText(str);
	stuAndorCCD.iCbpNum=atoi(str);
	ManageZolixIni->WriteIni();
}
void CDlgAdvAndor::OnCbnSelchangeComboMirror()
{
	stuAndorCCD.isMirror=((CComboBox *)GetDlgItem(IDC_COMBO_MIRROR))->GetCurSel();
	ManageZolixIni->WriteIni();
}
BOOL CDlgAdvAndor::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg->wParam==VK_RETURN || pMsg->wParam==VK_ESCAPE ) 
		return TRUE;
	else    
		return CDialog::PreTranslateMessage(pMsg);
}
void CDlgAdvAndor::OnBnClickedButtonGhost() {


	GetDlgItemText(IDC_EDIT_ADV_GHOST, stuAndorCCD.ghostImageStrRelative);
	
	if (stuAndorCCD.ghostImageStrRelative.Trim().IsEmpty())
	{
		AfxMessageBox(_T("已修正"));
		stuAndorCCD.ghostImageStrAbsolute = "";
		ManageZolixIni->WriteIni();
		return;
	}

	//1.从配置文件中读offset
	TCHAR szPath[MAX_PATH] = { 0 };
	GetModuleFileName(NULL, szPath, MAX_PATH);
	CString strExePath = szPath;
	strExePath = strExePath.Left(strExePath.ReverseFind(_T('\\')));  // 截取目录部分
	CString iniPath = strExePath + _T("\\ZolixCCD.ini");
	TCHAR szOffset[256] = { 0 };
	GetPrivateProfileString(_T("A"), _T("dataOffset"), _T("0"), szOffset, 256, iniPath);
	int offset = _ttoi(szOffset);  // 转换为 int

	//2.更新绝对范围
	CString result;
	CStringArray ranges;

	CString& ghostStr = stuAndorCCD.ghostImageStrRelative;
	int pos = 0;
	CString token = ghostStr.Tokenize(_T(";"), pos);
	while (!token.IsEmpty())
	{
		int commaPos = token.Find(_T(","));
		if (commaPos > 0)
		{
			CString left = token.Left(commaPos).Trim();
			CString right = token.Mid(commaPos + 1).Trim();
			int start = _ttoi(left) + offset;
			int end = _ttoi(right) + offset;

			CString newRange;
			newRange.Format(_T("%d,%d"), start, end);

			if (!result.IsEmpty())
				result += _T(";");
			result += newRange;
		}
		token = ghostStr.Tokenize(_T(";"), pos);
	}
	stuAndorCCD.ghostImageStrAbsolute = result;
	
	//3.写回配置文件范围字段(绝对范围)
	ManageZolixIni->WriteIni();
	AfxMessageBox("已修正");
}


// CDlgAdvAndor 消息处理程序
NS_MFC_Res_END