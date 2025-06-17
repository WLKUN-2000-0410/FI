// DlgDccdSet.cpp : 实现文件
//

#include "stdafx.h"
#include "INScan-Lite.h"
#include "DlgDccdSet.h"
#include "afxdialogex.h"
#include "INScan-LiteDlg.h"

// CDlgDccdSet 对话框

IMPLEMENT_DYNAMIC(CDlgDccdSet, CDialog)

CDlgDccdSet::CDlgDccdSet(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_DIALOG_DCCD, pParent)
{

}

CDlgDccdSet::~CDlgDccdSet()
{
}

void CDlgDccdSet::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgDccdSet, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, &CDlgDccdSet::OnBnClickedButton1)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CDlgDccdSet::OnCbnSelchangeCombo1)
	ON_CBN_SELCHANGE(IDC_COMBO2, &CDlgDccdSet::OnCbnSelchangeCombo2)
	ON_BN_CLICKED(IDC_BUTTON2, &CDlgDccdSet::OnBnClickedButton2)
	ON_CBN_SELCHANGE(IDC_COMBO3, &CDlgDccdSet::OnCbnSelchangeCombo3)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CDlgDccdSet 消息处理程序


BOOL CDlgDccdSet::OnInitDialog()
{
	CDialog::OnInitDialog();

	short nTmep = 0;
	CINScanLiteDlg * pDlg = (CINScanLiteDlg *)AfxGetApp()->GetMainWnd();
	CString strResult;
	//pDlg->m_b1064 = false;
	GetPrivateProfileString("Dfield", "gain", "0", strResult.GetBuffer(1024), 1024, sInfo.strPath);
	dfield_set_gain(pDlg->_handle, atoi(strResult)+1);
	((CComboBox *)GetDlgItem(IDC_COMBO1))->SetCurSel(atoi(strResult));
	
	GetPrivateProfileString("Dfield", "speed", "0", strResult.GetBuffer(1024), 1024, sInfo.strPath);
	((CComboBox *)GetDlgItem(IDC_COMBO2))->SetCurSel(atoi(strResult));
	OnCbnSelchangeCombo2();

	GetPrivateProfileString("Dfield", "Mirr", "0", strResult.GetBuffer(1024), 1024, sInfo.strPath);
	sTInfo.b1064ccdFlip = atoi(strResult);
	((CComboBox *)GetDlgItem(IDC_COMBO3))->SetCurSel(atoi(strResult));

	GetPrivateProfileString("SET", "CoolTemp", "0", strResult.GetBuffer(1024), 1024, sInfo.strPath);
	GetDlgItem(IDC_EDIT1)->SetWindowText(strResult);

	GetPrivateProfileString("Dfield", "ccdnum", "0", strResult.GetBuffer(1024), 1024, sInfo.strPath);
	GetDlgItem(IDC_EDIT2)->SetWindowText(strResult);

	GetPrivateProfileString("Dfield", "ccd", "0", strResult.GetBuffer(1024), 1024, sInfo.strPath);
	GetDlgItem(IDC_EDIT3)->SetWindowText(strResult);
	

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CDlgDccdSet::OnBnClickedButton1()
{
	CString str;
	CINScanLiteDlg * pDlg = (CINScanLiteDlg *)AfxGetApp()->GetMainWnd();
	GetDlgItem(IDC_EDIT1)->GetWindowText(str);
	dfield_set_temperature(pDlg->_handle,atoi(str));
	WritePrivateProfileString("SET", "CoolTemp", str, sInfo.strPath);
}


void CDlgDccdSet::OnCbnSelchangeCombo1()
{
	CString str;
	CINScanLiteDlg * pDlg = (CINScanLiteDlg *)AfxGetApp()->GetMainWnd();
	//GetDlgItem(IDC_COMBO1)->GetWindowText(str);
	int ntemp = ((CComboBox *)GetDlgItem(IDC_COMBO1))->GetCurSel()+1;	
	dfield_set_gain(pDlg->_handle, ntemp);
	str.Format("%d", ntemp-1);
	WritePrivateProfileString("Dfield", "gain", str, sInfo.strPath);
}


void CDlgDccdSet::OnCbnSelchangeCombo2()
{
	CString str;
	CINScanLiteDlg * pDlg = (CINScanLiteDlg *)AfxGetApp()->GetMainWnd();
	int ntemp = ((CComboBox *)GetDlgItem(IDC_COMBO2))->GetCurSel() + 1;
	int Index= ntemp;
	if (Index == 0) {
		dfield_set_readout_speed(pDlg->_handle, speed_0);
	}
	if (Index == 1) {
		dfield_set_readout_speed(pDlg->_handle, speed_1);
	}
	if (Index == 2) {
		dfield_set_readout_speed(pDlg->_handle, speed_2);
	}
	if (Index == 3) {
		dfield_set_readout_speed(pDlg->_handle, speed_3);
	}
}


void CDlgDccdSet::OnBnClickedButton2()
{
	CString str;
	GetDlgItem(IDC_EDIT2)->GetWindowText(str);
	WritePrivateProfileString("Dfield", "ccdnum", str, sInfo.strPath);
	GetDlgItem(IDC_EDIT3)->GetWindowText(str);
	WritePrivateProfileString("Dfield", "ccd", str, sInfo.strPath);
}


void CDlgDccdSet::OnCbnSelchangeCombo3()
{
	CString str;
	CINScanLiteDlg * pDlg = (CINScanLiteDlg *)AfxGetApp()->GetMainWnd();
	sTInfo.b1064ccdFlip =((CComboBox *)GetDlgItem(IDC_COMBO3))->GetCurSel();
	WritePrivateProfileString("Dfield", "Mirr", str, sInfo.strPath);
}


void CDlgDccdSet::OnClose()
{
	CINScanLiteDlg * pDlg = (CINScanLiteDlg *)AfxGetApp()->GetMainWnd();
	//pDlg->m_b1064 = true;

	CDialog::OnClose();
}
