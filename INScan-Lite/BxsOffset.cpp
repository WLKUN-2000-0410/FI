
#include "stdafx.h"
#include "BxsOffset.h"
#include "afxdialogex.h"
#include "INScan-LiteDlg.h"
#include "..\file\TeeChart2011\valuelist.h"
#include "..\file\TeeChart2011\series.h"
#include "..\file\TeeChart2011\axes.h"
#include "..\file\TeeChart2011\pen.h"
#include "..\file\TeeChart2011\axis.h"

#include "DlgLaCorr.h"
// CBxsOffset 对话框

IMPLEMENT_DYNAMIC(CBxsOffset, CDialogEx)

CBxsOffset::CBxsOffset(CWnd* pParent /*=NULL*/)
	: CDialogEx(CBxsOffset::IDD, pParent)	
	, a0(0)
	, a1(0)
	, a2(0)
	, a3(0)
	, a4(0)
	, a5(0)
	, a6(0)
	, a7(0)
	, a8(0)
	, a9(0)
	, a10(0)
	, a11(0)
	, a12(0)
	, a13(0)
	, a14(0)
	, b0(0)
	, b1(0)
	, b2(0)
	, b3(0)
	, b4(0)
	, b5(0)
	, b6(0)
	, b7(0)
	, b8(0)
	, b9(0)
	, b10(0)
	, b11(0)
	, b12(0)
	, b13(0)
	, b14(0)
	, m_bLenNum(true)
{

}

CBxsOffset::~CBxsOffset()
{
	if(m_hWnd != NULL) 
		DestroyWindow();
}

void CBxsOffset::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	//DDX_Control(pDX, IDC_MSFLEXGRID1, m_fexgrid);

	DDX_Control(pDX, IDC_TCHART1, m_tchart);
	DDX_Text(pDX, IDC_X0, a0);
	DDX_Text(pDX, IDC_X1, a1);
	DDX_Text(pDX, IDC_X2, a2);
	DDX_Text(pDX, IDC_X3, a3);
	DDX_Text(pDX, IDC_X4, a4);
	DDX_Text(pDX, IDC_X5, a5);
	DDX_Text(pDX, IDC_X6, a6);
	DDX_Text(pDX, IDC_X7, a7);
	DDX_Text(pDX, IDC_X8, a8);
	DDX_Text(pDX, IDC_X9, a9);
	DDX_Text(pDX, IDC_X10, a10);
	DDX_Text(pDX, IDC_X11, a11);
	DDX_Text(pDX, IDC_X12, a12);
	DDX_Text(pDX, IDC_X13, a13);
	DDX_Text(pDX, IDC_X14, a14);
	DDX_Text(pDX, IDC_Y0, b0);
	DDX_Text(pDX, IDC_Y1, b1);
	DDX_Text(pDX, IDC_Y2, b2);
	DDX_Text(pDX, IDC_Y3, b3);
	DDX_Text(pDX, IDC_Y4, b4);
	DDX_Text(pDX, IDC_Y5, b5);
	DDX_Text(pDX, IDC_Y6, b6);
	DDX_Text(pDX, IDC_Y7, b7);
	DDX_Text(pDX, IDC_Y8, b8);
	DDX_Text(pDX, IDC_Y9, b9);
	DDX_Text(pDX, IDC_Y10, b10);
	DDX_Text(pDX, IDC_Y11, b11);
	DDX_Text(pDX, IDC_Y12, b12);
	DDX_Text(pDX, IDC_Y13, b13);
	DDX_Text(pDX, IDC_Y14, b14);
	DDX_Control(pDX, IDC_JIAOZHENG, m_BtnCorr);
	DDX_Control(pDX, IDC_USETHISNUM, m_BtnUse);
	DDX_Control(pDX, IDC_OUTTEXT, m_BtnOut);
	DDX_Control(pDX, IDC_COMBO_NAME, m_ComboName);
	DDX_Control(pDX, IDC_BTN_LASER_CORR, m_BtnCenWaveLenCorr);
	DDX_Control(pDX, IDC_BTN_AUTOFILL, m_BtnAutoFill);
}


BEGIN_MESSAGE_MAP(CBxsOffset, CDialogEx)
	ON_BN_CLICKED(IDOK, &CBxsOffset::OnBnClickedOk)
	ON_BN_CLICKED(IDC_USETHISNUM, &CBxsOffset::OnBnClickedUsethisnum)
	ON_BN_CLICKED(IDC_OUTTEXT, &CBxsOffset::OnBnClickedOuttext)
	ON_BN_CLICKED(IDC_JIAOZHENG, &CBxsOffset::OnBnClickedJiaozheng)
	ON_EN_CHANGE(IDC_Y1, &CBxsOffset::OnEnChangeY1)
	ON_WM_CTLCOLOR()
	ON_WM_PAINT()
	ON_CBN_SELCHANGE(IDC_COMBO_NAME, &CBxsOffset::OnCbnSelchangeComboName)
	ON_EN_KILLFOCUS(IDC_Y14, &CBxsOffset::OnEnKillfocusY14)
	ON_EN_KILLFOCUS(IDC_Y13, &CBxsOffset::OnEnKillfocusY13)
	ON_BN_CLICKED(IDC_RADIO_WAVELEN, &CBxsOffset::OnBnClickedRadioWavelen)
	ON_BN_CLICKED(IDC_RADIO_WAVENUM, &CBxsOffset::OnBnClickedRadioWavenum)
	ON_BN_CLICKED(IDC_BTN_LASER_CORR, &CBxsOffset::OnBnClickedBtnLaserCorr)
	ON_BN_CLICKED(IDC_BTN_AUTOFILL, &CBxsOffset::OnBnClickedBtnAutofill)
END_MESSAGE_MAP()


// CBxsOffset 消息处理程序

void CBxsOffset::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	//CDialogEx::OnOK();
}

//  应用系数
void CBxsOffset::OnBnClickedUsethisnum()
{
	// TODO: 在此添加控件通知处理程序代码
	CINScanLiteDlg * pThreadApp=(CINScanLiteDlg*)AfxGetApp();
	UpdateData(TRUE);
	CString m_str;
	//  如果是波长校正
	if(m_bLenNum)
	{
		m_str.Format("%.30f",m_a0);
		WritePrivateProfileString("xs","a0",m_str,sInfo.strPath);
		m_str.Format("%.30f",m_a1);
		WritePrivateProfileString("xs","a1",m_str,sInfo.strPath);
		m_str.Format("%.30f",m_a2);
		WritePrivateProfileString("xs","a2",m_str,sInfo.strPath);
		m_str.Format("%.30f",m_a3);
		WritePrivateProfileString("xs","a3",m_str,sInfo.strPath);

		sSInfo.a[0]=m_a0;
		sSInfo.a[1]=m_a1;
		sSInfo.a[2]=m_a2;
		sSInfo.a[3]=m_a3;
		//sSInfo.a[4]=m_a4;
		//sSInfo.a[5]=m_a5;
		g_bCorrNumLen = false;
	}
	//  如果是波数校正
	else
	{
		m_str.Format("%.80f",m_a0);
		WritePrivateProfileString("xsWaveNum","b0",m_str,sInfo.strPath);
		m_str.Format("%.80f",m_a1);
		WritePrivateProfileString("xsWaveNum","b1",m_str,sInfo.strPath);
		m_str.Format("%.80f",m_a2);
		WritePrivateProfileString("xsWaveNum","b2",m_str,sInfo.strPath);
		m_str.Format("%.80f",m_a3);
		WritePrivateProfileString("xsWaveNum","b3",m_str,sInfo.strPath);

		sSInfo.b[0]=m_a0;
		sSInfo.b[1]=m_a1;
		sSInfo.b[2]=m_a2;
		sSInfo.b[3]=m_a3;
		g_bCorrNumLen = true;
	}


	CString temp;
	double aa=0;
	CString str;
	UpdateData(TRUE);
	for(int i=0;i<15;i++)
	{
		str.Format("%d",i);
		str = _T("a")+str;
		switch(i)
		{
		case 0:
			temp.Format("%f",b0);
			break;
		case 1:
			temp.Format("%f",b1);
			break;
		case 2:
			temp.Format("%f",b2);
			break;
		case 3:
			temp.Format("%f",b3);
			break;
		case 4:
			temp.Format("%f",b4);
			break;
		case 5:
			temp.Format("%f",b5);
			break;
		case 6:
			temp.Format("%f",b6);
			break;
		case 7:
			temp.Format("%f",b7);
			break;
		case 8:
			temp.Format("%f",b8);
			break;
		case 9:
			temp.Format("%f",b9);
			break;
		case 10:
			temp.Format("%f",b10);
			break;
		case 11:
			temp.Format("%f",b11);
			break;;
		case 12:
			temp.Format("%f",b12);
			break;;
		case 13:
			temp.Format("%f",b13);
			break;
		case 14:
			temp.Format("%f",b14);
			break;
		}
		if(m_bLenNum)
			WritePrivateProfileString("CS",str,temp,sInfo.strPath);
		else
			WritePrivateProfileString("CSWaveNum",str,temp,sInfo.strPath);
	}
}

BOOL CBxsOffset::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	CINScanLiteDlg * pThreadApp=(CINScanLiteDlg*)AfxGetApp()->GetMainWnd();

	int nSeriesCount = pThreadApp->m_DlgShow.m_Tchart.GetSeriesCount();
	if (nSeriesCount==0) return TRUE;
	for (int i = 0 ;i<nSeriesCount;i++)
	{
		CString strSeriesName = pThreadApp->m_DlgShow.m_Tchart.Series(i).GetTitle();
		m_ComboName.InsertString(i,strSeriesName);
	}
	//  lcz  new  设置精度  小数点位数
	
	SetButtonState(&m_BtnCorr);
	SetButtonState(&m_BtnUse);
	SetButtonState(&m_BtnOut);
	SetButtonState(&m_BtnCenWaveLenCorr);
	SetButtonState(&m_BtnAutoFill);
	//  默认勾选波数校正
	//OnBnClickedRadioWavenum();
	((CButton *)GetDlgItem(IDC_RADIO_WAVELEN))->SetCheck(TRUE);
	m_bLenNum = g_bWaveLenCorr;//  初始化时选择是波长还是波数校正

	GetDlgItem(IDC_BTN_LASER_CORR)->EnableWindow(true);
	//GetDlgItem(IDC_RADIO_WAVENUM)->EnableWindow(false);
	//  lcz new 08/17  改为显示最后一条曲线
	//  显示第一条曲线, 根据数据点数来显示数据，横坐标从1到num, 以表示像素值
	ShowData(nSeriesCount - 1);
	m_ComboName.SetCurSel(nSeriesCount - 1);
	//  根据是选择波长校正还是波数校正，从配置文件里读取数据存入向量
	importCorrPara();

	UpdateLanguage();

	if(m_bLenNum)
	{
		CString tempTEXT = pThreadApp->LoadStringMeau(_T("IDD150_12581"));
		SetDlgItemText(IDC_STATIC_BCZH, tempTEXT);
		GetDlgItem(IDC_BTN_LASER_CORR)->EnableWindow(TRUE);
	}
		//OnBnClickedRadioWavelen();
	else
	{
		//  将波长改为波数
		CString tempTEXT = pThreadApp->LoadStringMeau(_T("IDD150_12591"));
		SetDlgItemText(IDC_STATIC_BCZH, tempTEXT);
		GetDlgItem(IDC_BTN_LASER_CORR)->EnableWindow(FALSE);
	}
		//OnBnClickedRadioWavenum();

	
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CBxsOffset::UpdateLanguage()
{
	CINScanLiteDlg * pDlg=(CINScanLiteDlg*)GetParent();
	pDlg->m_ZLanguage.SetCreateLanguageState(FALSE);
	pDlg->m_ZLanguage.SetDialogStrings((long)(CDialog*)this,IDD);
}

//  导出系数
void CBxsOffset::OnBnClickedOuttext()
{
	CINScanLiteDlg * pDlg=(CINScanLiteDlg*)AfxGetApp()->GetMainWnd();

	UpdateData(TRUE);
	//
	CFileDialog dlg(FALSE,_T("ini"),_T("VALUE.ini"),OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,"value(*.ini)|*.ini||");
	//CFileDialog dlg(FALSE,_T("ini"),_T("VALUE.ini"),OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,"value(*.ini)|*.ini|value(*.xlsx)|*.xlsx||");
	CString str;
	dlg.m_ofn.lpstrTitle=str;
	DWORD osversion;
	osversion=::GetVersion();
	if(osversion<0x80000000)
		dlg.m_ofn.lStructSize=88;
	else dlg.m_ofn.lStructSize=76;
	
	UINT nResult=dlg.DoModal();
	
	if (nResult!=IDOK)	
	{
		return;
	}
	CString filename=dlg.GetPathName();
	
	CStdioFile file;
	CString tmp;
	if (!file.Open(filename,CFile::modeCreate|CFile::modeWrite)) 
	{
		str="Error:";
		AfxMessageBox(str,MB_ICONWARNING);
	}
	//2005-4-30
	
	//file.WriteString("\n");
	UpdateData(TRUE);
	
	CString strResult;
	DWORD	dwSize=1024;

	file.WriteString("[CCD]\n");
	CString eachLine;
	eachLine = TEXT("Model=") + sPInfo.Model + TEXT("\n") ;
	file.WriteString(eachLine);
	eachLine = TEXT("Number=") + sPInfo.Number + TEXT("\n") ;
	file.WriteString(eachLine);
	//file.WriteString("Number=\n");
	CString tempVal;

	tempVal.Format(TEXT("%d"), sInfo.OminiType);
	eachLine = TEXT("OminiType=") + tempVal + TEXT("\n") ;
	file.WriteString(eachLine);

	// lcz new 08/30  导出高低使能、电机步数、最后一个谱峰的值
	file.WriteString("[SET]\n");
	tempVal.Format(TEXT("%d"), sSInfo.HLMODE); 
	eachLine = TEXT("HLMODE=") + tempVal + TEXT("\n") ;
	file.WriteString(eachLine);
	//tempVal.Format(TEXT("%d"), sSInfo.LockSet); 
	//eachLine = TEXT("LockSet=") + tempVal + TEXT("\n") ;
	//file.WriteString(eachLine);
	tempVal.Format(TEXT("%.1f"), sSInfo.d532rdWaveNum); 
	eachLine = TEXT("FI532rdWaveNum=") + tempVal + TEXT("\n") ;
	file.WriteString(eachLine);
	tempVal.Format(TEXT("%.1f"), sSInfo.d785rdWaveNum); 
	eachLine = TEXT("FI785rdWaveNum=") + tempVal + TEXT("\n") ;
	file.WriteString(eachLine);

	file.WriteString("[sz]\n");
	
	tempVal.Format(TEXT("%f"), sCInfo.Laser);
	eachLine = TEXT("Laser=") + tempVal+ TEXT("\n") ;
	file.WriteString(eachLine);


	tempVal.Format(TEXT("%d"), (int)sSInfo.LaserPowerMin_XL);
	eachLine = TEXT("LaserPowerMin_XingLin=") + tempVal + TEXT("\n");
	file.WriteString(eachLine);

	tempVal.Format(TEXT("%d"), (int)sSInfo.LaserPowerMax_XL);
	eachLine = TEXT("LaserPowerMax_XingLin=") + tempVal + TEXT('\n');
	file.WriteString(eachLine);

	tempVal.Format(TEXT("%d"), (int)sSInfo.LaserPowerMin_XCY);
	eachLine = TEXT("LaserPowerMin_XinChanYe=") + tempVal + TEXT('\n');
	file.WriteString(eachLine);

	tempVal.Format(TEXT("%d"), (int)sSInfo.LaserPowerMax_XCY);
	eachLine = TEXT("LaserPowerMax_XinChanYe=") + tempVal + TEXT('\n');
	file.WriteString(eachLine);

	//file.WriteString("LASERPOWERMAX=\n");
	tempVal.Format(TEXT("%d"), sSInfo.LockSet);
	eachLine = TEXT("LockSet=") + tempVal + TEXT("\n") ;
	file.WriteString(eachLine);
	//file.WriteString("LockSet=\n");
	tempVal.Format(TEXT("%d"), sSInfo.nSqlMode);
	eachLine = TEXT("SqlMode=") + tempVal + TEXT("\n") ;
	file.WriteString(eachLine);
	//file.WriteString("SqlMode=\n");

	file.WriteString("[xs]\n");
	tempVal.Format(TEXT("%.20f"), sSInfo.a[0]);
	eachLine = TEXT("a0=") +tempVal + TEXT("\n") ;
	file.WriteString(eachLine);
	//file.WriteString("a0=\n");
	tempVal.Format(TEXT("%.20f"), sSInfo.a[1]);
	eachLine = TEXT("a1=") + tempVal + TEXT("\n") ;
	file.WriteString(eachLine);
	//file.WriteString("a1=\n");
	tempVal.Format(TEXT("%.20f"), sSInfo.a[2]);
	eachLine = TEXT("a2=") + tempVal + TEXT("\n") ;
	file.WriteString(eachLine);
	//file.WriteString("a2=\n");
	tempVal.Format(TEXT("%.20f"), sSInfo.a[3]);
	eachLine = TEXT("a3=") + tempVal + TEXT("\n") ;
	file.WriteString(eachLine);
	//file.WriteString("a3=\n");

	file.WriteString("[bxjzxs]\n");
	double temp = 0.0;
	temp = atof(pDlg->GetIniInfo(_T("bxjzxs"),_T("a0")));
	tempVal.Format(TEXT("%f"), temp);
	eachLine = TEXT("a0=") + tempVal + TEXT("\n") ;
	file.WriteString(eachLine);
	//file.WriteString("a0=\n");
	temp = atof(pDlg->GetIniInfo(_T("bxjzxs"),_T("a1")));
	tempVal.Format(TEXT("%f"), temp);
	eachLine = TEXT("a1=") + tempVal + TEXT("\n") ;
	file.WriteString(eachLine);
	//file.WriteString("a1=\n");
	temp = atof(pDlg->GetIniInfo(_T("bxjzxs"),_T("a2")));
	tempVal.Format(TEXT("%f"), temp);
	eachLine = TEXT("a2=") + tempVal + TEXT("\n") ;
	file.WriteString(eachLine);
	//file.WriteString("a2=\n");
	temp = atof(pDlg->GetIniInfo(_T("bxjzxs"),_T("a3")));
	tempVal.Format(TEXT("%f"), temp);
	eachLine = TEXT("a3=") + tempVal + TEXT("\n") ;
	file.WriteString(eachLine);
	//file.WriteString("a3=\n");
	temp = atof(pDlg->GetIniInfo(_T("bxjzxs"),_T("a4")));
	tempVal.Format(TEXT("%f"), temp);
	eachLine = TEXT("a4=") + tempVal + TEXT("\n") ;
	file.WriteString(eachLine);
	//file.WriteString("a4=\n");
	temp = atof(pDlg->GetIniInfo(_T("bxjzxs"),_T("a5")));
	tempVal.Format(TEXT("%f"), temp);
	eachLine = TEXT("a5=") + tempVal + TEXT("\n") ;
	file.WriteString(eachLine);
	//file.WriteString("a5=\n");
	temp = atof(pDlg->GetIniInfo(_T("bxjzxs"),_T("a6")));
	tempVal.Format(TEXT("%f"), temp);
	eachLine = TEXT("a6=") + tempVal + TEXT("\n") ;
	file.WriteString(eachLine);
	//file.WriteString("a6=\n");
	temp = atof(pDlg->GetIniInfo(_T("bxjzxs"),_T("a7")));
	tempVal.Format(TEXT("%f"), temp);
	eachLine = TEXT("a7=") + tempVal + TEXT("\n") ;
	file.WriteString(eachLine);
	//file.WriteString("a7=\n");
	temp = atof(pDlg->GetIniInfo(_T("bxjzxs"),_T("a8")));
	tempVal.Format(TEXT("%f"), temp);
	eachLine = TEXT("a8=") + tempVal + TEXT("\n") ;
	file.WriteString(eachLine);
	//file.WriteString("a8=\n");
	temp = atof(pDlg->GetIniInfo(_T("bxjzxs"),_T("a9")));
	tempVal.Format(TEXT("%f"), temp);
	eachLine = TEXT("a9=") + tempVal + TEXT("\n") ;
	file.WriteString(eachLine);
	//file.WriteString("a9=\n");
	temp = atof(pDlg->GetIniInfo(_T("bxjzxs"),_T("a10")));
	tempVal.Format(TEXT("%f"), temp);
	eachLine = TEXT("a10=") + tempVal + TEXT("\n") ;
	file.WriteString(eachLine);
	//file.WriteString("a10=\n");
	temp = atof(pDlg->GetIniInfo(_T("bxjzxs"),_T("a11")));
	tempVal.Format(TEXT("%f"), temp);
	eachLine = TEXT("a11=") + tempVal + TEXT("\n") ;
	file.WriteString(eachLine);
	//file.WriteString("a11=\n");
	temp = atof(pDlg->GetIniInfo(_T("bxjzxs"),_T("a12")));
	tempVal.Format(TEXT("%f"), temp);
	eachLine = TEXT("a12=") + tempVal + TEXT("\n") ;
	file.WriteString(eachLine);
	//file.WriteString("a12=\n");
	temp = atof(pDlg->GetIniInfo(_T("bxjzxs"),_T("a13")));
	tempVal.Format(TEXT("%f"), temp);
	eachLine = TEXT("a13=") + tempVal + TEXT("\n") ;
	file.WriteString(eachLine);
	//file.WriteString("a13=\n");
	temp = atof(pDlg->GetIniInfo(_T("bxjzxs"),_T("a14")));
	tempVal.Format(TEXT("%f"), temp);
	eachLine = TEXT("a14=") + tempVal + TEXT("\n") ;
	file.WriteString(eachLine);
	//file.WriteString("a14=\n");

	file.WriteString("[CS]\n");
	temp = atof(pDlg->GetIniInfo(_T("CS"),_T("a0")));
	tempVal.Format(TEXT("%f"), temp);
	eachLine = TEXT("a0=") + tempVal + TEXT("\n") ;
	file.WriteString(eachLine);
	temp = atof(pDlg->GetIniInfo(_T("CS"),_T("a1")));
	tempVal.Format(TEXT("%f"), temp);
	eachLine = TEXT("a1=") + tempVal + TEXT("\n") ;
	file.WriteString(eachLine);
	temp = atof(pDlg->GetIniInfo(_T("CS"),_T("a2")));
	tempVal.Format(TEXT("%f"), temp);
	eachLine = TEXT("a2=") + tempVal + TEXT("\n") ;
	file.WriteString(eachLine);
	temp = atof(pDlg->GetIniInfo(_T("CS"),_T("a3")));
	tempVal.Format(TEXT("%f"), temp);
	eachLine = TEXT("a3=") + tempVal + TEXT("\n") ;
	file.WriteString(eachLine);
	temp = atof(pDlg->GetIniInfo(_T("CS"),_T("a4")));
	tempVal.Format(TEXT("%f"), temp);
	eachLine = TEXT("a4=") + tempVal + TEXT("\n") ;
	file.WriteString(eachLine);
	temp = atof(pDlg->GetIniInfo(_T("CS"),_T("a5")));
	tempVal.Format(TEXT("%f"), temp);
	eachLine = TEXT("a5=") + tempVal + TEXT("\n") ;
	file.WriteString(eachLine);
	temp = atof(pDlg->GetIniInfo(_T("CS"),_T("a6")));
	tempVal.Format(TEXT("%f"), temp);
	eachLine = TEXT("a6=") + tempVal + TEXT("\n") ;
	file.WriteString(eachLine);
	temp = atof(pDlg->GetIniInfo(_T("CS"),_T("a7")));
	tempVal.Format(TEXT("%f"), temp);
	eachLine = TEXT("a7=") + tempVal + TEXT("\n") ;
	file.WriteString(eachLine);
	temp = atof(pDlg->GetIniInfo(_T("CS"),_T("a8")));
	tempVal.Format(TEXT("%f"), temp);
	eachLine = TEXT("a8=") + tempVal + TEXT("\n") ;
	file.WriteString(eachLine);
	temp = atof(pDlg->GetIniInfo(_T("CS"),_T("a9")));
	tempVal.Format(TEXT("%f"), temp);
	eachLine = TEXT("a9=") + tempVal + TEXT("\n") ;
	file.WriteString(eachLine);
	temp = atof(pDlg->GetIniInfo(_T("CS"),_T("a10")));
	tempVal.Format(TEXT("%f"), temp);
	eachLine = TEXT("a10=") + tempVal + TEXT("\n") ;
	file.WriteString(eachLine);
	temp = atof(pDlg->GetIniInfo(_T("CS"),_T("a11")));
	tempVal.Format(TEXT("%f"), temp);
	eachLine = TEXT("a11=") + tempVal + TEXT("\n") ;
	file.WriteString(eachLine);
	temp = atof(pDlg->GetIniInfo(_T("CS"),_T("a12")));
	tempVal.Format(TEXT("%f"), temp);
	eachLine = TEXT("a12=") + tempVal + TEXT("\n") ;
	file.WriteString(eachLine);
	temp = atof(pDlg->GetIniInfo(_T("CS"),_T("a13")));
	tempVal.Format(TEXT("%f"), temp);
	eachLine = TEXT("a13=") + tempVal + TEXT("\n") ;
	file.WriteString(eachLine);
	temp = atof(pDlg->GetIniInfo(_T("CS"),_T("a14")));
	tempVal.Format(TEXT("%f"), temp);
	eachLine = TEXT("a14=") + tempVal + TEXT("\n") ;
	file.WriteString(eachLine);
	//file.WriteString("a0=\n");
	//file.WriteString("a1=\n");
	//file.WriteString("a2=\n");
	//file.WriteString("a3=\n");
	//file.WriteString("a4=\n");
	//file.WriteString("a5=\n");
	//file.WriteString("a6=\n");
	//file.WriteString("a7=\n");
	//file.WriteString("a8=\n");
	//file.WriteString("a9=\n");
	//file.WriteString("a10=\n");
	//file.WriteString("a11=\n");
	//file.WriteString("a12=\n");
	//file.WriteString("a13=\n");
	//file.WriteString("a14=\n");

	//   导入波数校正的系数
	file.WriteString("[xsWaveNum]\n");
	tempVal.Format(TEXT("%.20f"), sSInfo.b[0]);
	eachLine = TEXT("b0=") +tempVal + TEXT("\n") ;
	file.WriteString(eachLine);
	//file.WriteString("a0=\n");
	tempVal.Format(TEXT("%.20f"), sSInfo.b[1]);
	eachLine = TEXT("b1=") + tempVal + TEXT("\n") ;
	file.WriteString(eachLine);
	//file.WriteString("a1=\n");
	tempVal.Format(TEXT("%.20f"), sSInfo.b[2]);
	eachLine = TEXT("b2=") + tempVal + TEXT("\n") ;
	file.WriteString(eachLine);
	//file.WriteString("a2=\n");
	tempVal.Format(TEXT("%.20f"), sSInfo.b[3]);
	eachLine = TEXT("b3=") + tempVal + TEXT("\n") ;
	file.WriteString(eachLine);
	//file.WriteString("a3=\n");

	file.WriteString("[waveNumber]\n");
	temp = atof(pDlg->GetIniInfo(_T("waveNumber"),_T("a0")));
	tempVal.Format(TEXT("%f"), temp);
	eachLine = TEXT("a0=") + tempVal + TEXT("\n") ;
	file.WriteString(eachLine);
	//file.WriteString("a0=\n");
	temp = atof(pDlg->GetIniInfo(_T("waveNumber"),_T("a1")));
	tempVal.Format(TEXT("%f"), temp);
	eachLine = TEXT("a1=") + tempVal + TEXT("\n") ;
	file.WriteString(eachLine);
	//file.WriteString("a1=\n");
	temp = atof(pDlg->GetIniInfo(_T("waveNumber"),_T("a2")));
	tempVal.Format(TEXT("%f"), temp);
	eachLine = TEXT("a2=") + tempVal + TEXT("\n") ;
	file.WriteString(eachLine);
	//file.WriteString("a2=\n");
	temp = atof(pDlg->GetIniInfo(_T("waveNumber"),_T("a3")));
	tempVal.Format(TEXT("%f"), temp);
	eachLine = TEXT("a3=") + tempVal + TEXT("\n") ;
	file.WriteString(eachLine);
	//file.WriteString("a3=\n");
	temp = atof(pDlg->GetIniInfo(_T("waveNumber"),_T("a4")));
	tempVal.Format(TEXT("%f"), temp);
	eachLine = TEXT("a4=") + tempVal + TEXT("\n") ;
	file.WriteString(eachLine);
	//file.WriteString("a4=\n");
	temp = atof(pDlg->GetIniInfo(_T("waveNumber"),_T("a5")));
	tempVal.Format(TEXT("%f"), temp);
	eachLine = TEXT("a5=") + tempVal + TEXT("\n") ;
	file.WriteString(eachLine);
	//file.WriteString("a5=\n");
	temp = atof(pDlg->GetIniInfo(_T("waveNumber"),_T("a6")));
	tempVal.Format(TEXT("%f"), temp);
	eachLine = TEXT("a6=") + tempVal + TEXT("\n") ;
	file.WriteString(eachLine);
	//file.WriteString("a6=\n");
	temp = atof(pDlg->GetIniInfo(_T("waveNumber"),_T("a7")));
	tempVal.Format(TEXT("%f"), temp);
	eachLine = TEXT("a7=") + tempVal + TEXT("\n") ;
	file.WriteString(eachLine);
	//file.WriteString("a7=\n");
	temp = atof(pDlg->GetIniInfo(_T("waveNumber"),_T("a8")));
	tempVal.Format(TEXT("%f"), temp);
	eachLine = TEXT("a8=") + tempVal + TEXT("\n") ;
	file.WriteString(eachLine);
	//file.WriteString("a8=\n");
	temp = atof(pDlg->GetIniInfo(_T("waveNumber"),_T("a9")));
	tempVal.Format(TEXT("%f"), temp);
	eachLine = TEXT("a9=") + tempVal + TEXT("\n") ;
	file.WriteString(eachLine);
	//file.WriteString("a9=\n");
	temp = atof(pDlg->GetIniInfo(_T("waveNumber"),_T("a10")));
	tempVal.Format(TEXT("%f"), temp);
	eachLine = TEXT("a10=") + tempVal + TEXT("\n") ;
	file.WriteString(eachLine);
	//file.WriteString("a10=\n");
	temp = atof(pDlg->GetIniInfo(_T("waveNumber"),_T("a11")));
	tempVal.Format(TEXT("%f"), temp);
	eachLine = TEXT("a11=") + tempVal + TEXT("\n") ;
	file.WriteString(eachLine);
	//file.WriteString("a11=\n");
	temp = atof(pDlg->GetIniInfo(_T("waveNumber"),_T("a12")));
	tempVal.Format(TEXT("%f"), temp);
	eachLine = TEXT("a12=") + tempVal + TEXT("\n") ;
	file.WriteString(eachLine);
	//file.WriteString("a12=\n");
	temp = atof(pDlg->GetIniInfo(_T("waveNumber"),_T("a13")));
	tempVal.Format(TEXT("%f"), temp);
	eachLine = TEXT("a13=") + tempVal + TEXT("\n") ;
	file.WriteString(eachLine);
	//file.WriteString("a13=\n");
	temp = atof(pDlg->GetIniInfo(_T("waveNumber"),_T("a14")));
	tempVal.Format(TEXT("%f"), temp);
	eachLine = TEXT("a14=") + tempVal + TEXT("\n") ;
	file.WriteString(eachLine);
	//file.WriteString("a14=\n");

	file.WriteString("[CSWaveNum]\n");
	temp = atof(pDlg->GetIniInfo(_T("CSWaveNum"),_T("a0")));
	tempVal.Format(TEXT("%f"), temp);
	eachLine = TEXT("a0=") + tempVal + TEXT("\n") ;
	file.WriteString(eachLine);
	temp = atof(pDlg->GetIniInfo(_T("CSWaveNum"),_T("a1")));
	tempVal.Format(TEXT("%f"), temp);
	eachLine = TEXT("a1=") + tempVal + TEXT("\n") ;
	file.WriteString(eachLine);
	temp = atof(pDlg->GetIniInfo(_T("CSWaveNum"),_T("a2")));
	tempVal.Format(TEXT("%f"), temp);
	eachLine = TEXT("a2=") + tempVal + TEXT("\n") ;
	file.WriteString(eachLine);
	temp = atof(pDlg->GetIniInfo(_T("CSWaveNum"),_T("a3")));
	tempVal.Format(TEXT("%f"), temp);
	eachLine = TEXT("a3=") + tempVal + TEXT("\n") ;
	file.WriteString(eachLine);
	temp = atof(pDlg->GetIniInfo(_T("CSWaveNum"),_T("a4")));
	tempVal.Format(TEXT("%f"), temp);
	eachLine = TEXT("a4=") + tempVal + TEXT("\n") ;
	file.WriteString(eachLine);
	temp = atof(pDlg->GetIniInfo(_T("CSWaveNum"),_T("a5")));
	tempVal.Format(TEXT("%f"), temp);
	eachLine = TEXT("a5=") + tempVal + TEXT("\n") ;
	file.WriteString(eachLine);
	temp = atof(pDlg->GetIniInfo(_T("CSWaveNum"),_T("a6")));
	tempVal.Format(TEXT("%f"), temp);
	eachLine = TEXT("a6=") + tempVal + TEXT("\n") ;
	file.WriteString(eachLine);
	temp = atof(pDlg->GetIniInfo(_T("CSWaveNum"),_T("a7")));
	tempVal.Format(TEXT("%f"), temp);
	eachLine = TEXT("a7=") + tempVal + TEXT("\n") ;
	file.WriteString(eachLine);
	temp = atof(pDlg->GetIniInfo(_T("CSWaveNum"),_T("a8")));
	tempVal.Format(TEXT("%f"), temp);
	eachLine = TEXT("a8=") + tempVal + TEXT("\n") ;
	file.WriteString(eachLine);
	temp = atof(pDlg->GetIniInfo(_T("CSWaveNum"),_T("a9")));
	tempVal.Format(TEXT("%f"), temp);
	eachLine = TEXT("a9=") + tempVal + TEXT("\n") ;
	file.WriteString(eachLine);
	temp = atof(pDlg->GetIniInfo(_T("CSWaveNum"),_T("a10")));
	tempVal.Format(TEXT("%f"), temp);
	eachLine = TEXT("a10=") + tempVal + TEXT("\n") ;
	file.WriteString(eachLine);
	temp = atof(pDlg->GetIniInfo(_T("CSWaveNum"),_T("a11")));
	tempVal.Format(TEXT("%f"), temp);
	eachLine = TEXT("a11=") + tempVal + TEXT("\n") ;
	file.WriteString(eachLine);
	temp = atof(pDlg->GetIniInfo(_T("CSWaveNum"),_T("a12")));
	tempVal.Format(TEXT("%f"), temp);
	eachLine = TEXT("a12=") + tempVal + TEXT("\n") ;
	file.WriteString(eachLine);
	temp = atof(pDlg->GetIniInfo(_T("CSWaveNum"),_T("a13")));
	tempVal.Format(TEXT("%f"), temp);
	eachLine = TEXT("a13=") + tempVal + TEXT("\n") ;
	file.WriteString(eachLine);
	temp = atof(pDlg->GetIniInfo(_T("CSWaveNum"),_T("a14")));
	tempVal.Format(TEXT("%f"), temp);
	eachLine = TEXT("a14=") + tempVal + TEXT("\n") ;
	file.WriteString(eachLine);

	file.WriteString("[CAMERA]\n");
	temp = atof(pDlg->GetIniInfo(_T("CAMERA"), _T("left")));
	tempVal.Format(TEXT("%d"), temp);
	eachLine = TEXT("left=") + tempVal + TEXT("\n");
	file.WriteString(eachLine);
	temp = atof(pDlg->GetIniInfo(_T("CAMERA"), _T("left")));
	tempVal.Format(TEXT("%d"), temp);
	eachLine = TEXT("top=") + tempVal + TEXT("\n");
	file.WriteString(eachLine);
	temp = atof(pDlg->GetIniInfo(_T("CAMERA"), _T("left")));
	tempVal.Format(TEXT("%d"), temp);
	eachLine = TEXT("right=") + tempVal + TEXT("\n");
	file.WriteString(eachLine);
	temp = atof(pDlg->GetIniInfo(_T("CAMERA"), _T("left")));
	tempVal.Format(TEXT("%d"), temp);
	eachLine = TEXT("bottom=") + tempVal + TEXT("\n");
	file.WriteString(eachLine);
	temp = atof(pDlg->GetIniInfo(_T("CAMERA"), _T("left")));
	tempVal.Format(TEXT("%d"), temp);
	eachLine = TEXT("hoffset=") + tempVal + TEXT("\n");
	file.WriteString(eachLine);
	temp = atof(pDlg->GetIniInfo(_T("CAMERA"), _T("left")));
	tempVal.Format(TEXT("%d"), temp);
	eachLine = TEXT("voffset=") + tempVal + TEXT("\n");
	file.WriteString(eachLine);
	file.Close();	
	Sleep(100);

	//理论上导出参数时候一定是仪器连接状态

	//GetPrivateProfileString(sInfo.sDInfo.CCDName.Left(1),"CCDNum","0",strResult.GetBuffer(dwSize),dwSize,sInfo.strPathCCD);
	//strResult.ReleaseBuffer();
	//WritePrivateProfileString("CCD","Num",strResult,filename);

	//GetPrivateProfileString(sInfo.sDInfo.CCDName.Left(1),"CCD","0",strResult.GetBuffer(dwSize),dwSize,sInfo.strPathCCD);
	//strResult.ReleaseBuffer();
	//WritePrivateProfileString("CCD","Pixel",strResult,filename);

 //   WritePrivateProfileString("CCD","Model",sPInfo.Model,filename);
	//WritePrivateProfileString("CCD","Number",sPInfo.Number,filename);

	//GetPrivateProfileString("CORRECT","Laser","785",strResult.GetBuffer(dwSize),dwSize,sInfo.strPath);
	//strResult.ReleaseBuffer();
	//WritePrivateProfileString("sz","bxsflas",strResult,filename);

	//GetPrivateProfileString("SET","LASERPOWERMIN","0",strResult.GetBuffer(dwSize),dwSize,sInfo.strPath);
	//strResult.ReleaseBuffer();
	//WritePrivateProfileString("sz","LASERPOWERMIN",strResult,filename);

	//GetPrivateProfileString("SET","LASERPOWERMAX","200",strResult.GetBuffer(dwSize),dwSize,sInfo.strPath);
	//strResult.ReleaseBuffer();
	//WritePrivateProfileString("sz","LASERPOWERMAX",strResult,filename);

	//GetPrivateProfileString("SET","LockSet","5000",strResult.GetBuffer(dwSize),dwSize,sInfo.strPath);
	//strResult.ReleaseBuffer();
	//WritePrivateProfileString("sz","LockSet",strResult,filename);

	//GetPrivateProfileString("SET","SqlMode","0",strResult.GetBuffer(dwSize),dwSize,sInfo.strPath);
	//strResult.ReleaseBuffer();
	//WritePrivateProfileString("sz","SqlMode",strResult,filename);

	//
	//GetPrivateProfileString("xs","a0","0",strResult.GetBuffer(dwSize),dwSize,sInfo.strPath);
	//strResult.ReleaseBuffer();
	//WritePrivateProfileString("xs","a0",strResult,filename);
	//
	//GetPrivateProfileString("xs","a1","0",strResult.GetBuffer(dwSize),dwSize,sInfo.strPath);
	//strResult.ReleaseBuffer();
	//WritePrivateProfileString("xs","a1",strResult,filename);
	//
	//GetPrivateProfileString("xs","a2","0",strResult.GetBuffer(dwSize),dwSize,sInfo.strPath);
	//strResult.ReleaseBuffer();
	//WritePrivateProfileString("xs","a2",strResult,filename);
	//
	//GetPrivateProfileString("xs","a3","0",strResult.GetBuffer(dwSize),dwSize,sInfo.strPath);
	//strResult.ReleaseBuffer();
	//WritePrivateProfileString("xs","a3",strResult,filename);
	//
	/////////////////////////////////////////////////////////////////////////////////////////////
	//
	/////////////////////////////////////////////////////////////////////////////////////////////
	//GetPrivateProfileString("bxjzxs","a0","0",strResult.GetBuffer(dwSize),dwSize,sInfo.strPath);
	//strResult.ReleaseBuffer();
	//WritePrivateProfileString("bxjzxs","a0",strResult,filename);
	//
	//GetPrivateProfileString("bxjzxs","a1","0",strResult.GetBuffer(dwSize),dwSize,sInfo.strPath);
	//strResult.ReleaseBuffer();
	//WritePrivateProfileString("bxjzxs","a1",strResult,filename);
	//
	//GetPrivateProfileString("bxjzxs","a2","0",strResult.GetBuffer(dwSize),dwSize,sInfo.strPath);
	//strResult.ReleaseBuffer();
	//WritePrivateProfileString("bxjzxs","a2",strResult,filename);
	//
	//GetPrivateProfileString("bxjzxs","a3","0",strResult.GetBuffer(dwSize),dwSize,sInfo.strPath);
	//strResult.ReleaseBuffer();
	//WritePrivateProfileString("bxjzxs","a3",strResult,filename);
	//
	//GetPrivateProfileString("bxjzxs","a4","0",strResult.GetBuffer(dwSize),dwSize,sInfo.strPath);
	//strResult.ReleaseBuffer();
	//WritePrivateProfileString("bxjzxs","a4",strResult,filename);
	//
	//GetPrivateProfileString("bxjzxs","a5","0",strResult.GetBuffer(dwSize),dwSize,sInfo.strPath);
	//strResult.ReleaseBuffer();
	//WritePrivateProfileString("bxjzxs","a5",strResult,filename);
	//
	//GetPrivateProfileString("bxjzxs","a6","0",strResult.GetBuffer(dwSize),dwSize,sInfo.strPath);
	//strResult.ReleaseBuffer();
	//WritePrivateProfileString("bxjzxs","a6",strResult,filename);
	//
	//GetPrivateProfileString("bxjzxs","a7","0",strResult.GetBuffer(dwSize),dwSize,sInfo.strPath);
	//strResult.ReleaseBuffer();
	//WritePrivateProfileString("bxjzxs","a7",strResult,filename);
	//
	//GetPrivateProfileString("bxjzxs","a8","0",strResult.GetBuffer(dwSize),dwSize,sInfo.strPath);
	//strResult.ReleaseBuffer();
	//WritePrivateProfileString("bxjzxs","a8",strResult,filename);
	//
	//GetPrivateProfileString("bxjzxs","a9","0",strResult.GetBuffer(dwSize),dwSize,sInfo.strPath);
	//strResult.ReleaseBuffer();
	//WritePrivateProfileString("bxjzxs","a9",strResult,filename);
	//
	//GetPrivateProfileString("bxjzxs","a10","0",strResult.GetBuffer(dwSize),dwSize,sInfo.strPath);
	//strResult.ReleaseBuffer();
	//WritePrivateProfileString("bxjzxs","a10",strResult,filename);
	//
	//GetPrivateProfileString("bxjzxs","a11","0",strResult.GetBuffer(dwSize),dwSize,sInfo.strPath);
	//strResult.ReleaseBuffer();
	//WritePrivateProfileString("bxjzxs","a11",strResult,filename);
	//
	//GetPrivateProfileString("bxjzxs","a12","0",strResult.GetBuffer(dwSize),dwSize,sInfo.strPath);
	//strResult.ReleaseBuffer();
	//WritePrivateProfileString("bxjzxs","a12",strResult,filename);
	//
	//GetPrivateProfileString("bxjzxs","a13","0",strResult.GetBuffer(dwSize),dwSize,sInfo.strPath);
	//strResult.ReleaseBuffer();
	//WritePrivateProfileString("bxjzxs","a13",strResult,filename);
	//
	//GetPrivateProfileString("bxjzxs","a14","0",strResult.GetBuffer(dwSize),dwSize,sInfo.strPath);
	//strResult.ReleaseBuffer();
	//WritePrivateProfileString("bxjzxs","a14",strResult,filename);
	////////////////////////////////////////////////////////////////////////////////////////////////
	//
	/////////////////////////////////////////////////////////////////////////////////////////////////
	//GetPrivateProfileString("CS","a0","0",strResult.GetBuffer(dwSize),dwSize,sInfo.strPath);
	//strResult.ReleaseBuffer();
	//WritePrivateProfileString("CS","a0",strResult,filename);
	//
	//GetPrivateProfileString("CS","a1","0",strResult.GetBuffer(dwSize),dwSize,sInfo.strPath);
	//strResult.ReleaseBuffer();
	//WritePrivateProfileString("CS","a1",strResult,filename);
	//
	//GetPrivateProfileString("CS","a2","0",strResult.GetBuffer(dwSize),dwSize,sInfo.strPath);
	//strResult.ReleaseBuffer();
	//WritePrivateProfileString("CS","a2",strResult,filename);
	//
	//GetPrivateProfileString("CS","a3","0",strResult.GetBuffer(dwSize),dwSize,sInfo.strPath);
	//strResult.ReleaseBuffer();
	//WritePrivateProfileString("CS","a3",strResult,filename);
	//
	//GetPrivateProfileString("CS","a4","0",strResult.GetBuffer(dwSize),dwSize,sInfo.strPath);
	//strResult.ReleaseBuffer();
	//WritePrivateProfileString("CS","a4",strResult,filename);
	//
	//GetPrivateProfileString("CS","a5","0",strResult.GetBuffer(dwSize),dwSize,sInfo.strPath);
	//strResult.ReleaseBuffer();
	//WritePrivateProfileString("CS","a5",strResult,filename);
	//
	//GetPrivateProfileString("CS","a6","0",strResult.GetBuffer(dwSize),dwSize,sInfo.strPath);
	//strResult.ReleaseBuffer();
	//WritePrivateProfileString("CS","a6",strResult,filename);
	//
	//GetPrivateProfileString("CS","a7","0",strResult.GetBuffer(dwSize),dwSize,sInfo.strPath);
	//strResult.ReleaseBuffer();
	//WritePrivateProfileString("CS","a7",strResult,filename);
	//
	//GetPrivateProfileString("CS","a8","0",strResult.GetBuffer(dwSize),dwSize,sInfo.strPath);
	//strResult.ReleaseBuffer();
	//WritePrivateProfileString("CS","a8",strResult,filename);
	//
	//GetPrivateProfileString("CS","a9","0",strResult.GetBuffer(dwSize),dwSize,sInfo.strPath);
	//strResult.ReleaseBuffer();
	//WritePrivateProfileString("CS","a9",strResult,filename);
	//
	//GetPrivateProfileString("CS","a10","0",strResult.GetBuffer(dwSize),dwSize,sInfo.strPath);
	//strResult.ReleaseBuffer();
	//WritePrivateProfileString("CS","a10",strResult,filename);
	//
	//GetPrivateProfileString("CS","a11","0",strResult.GetBuffer(dwSize),dwSize,sInfo.strPath);
	//strResult.ReleaseBuffer();
	//WritePrivateProfileString("CS","a11",strResult,filename);
	//
	//GetPrivateProfileString("CS","a12","0",strResult.GetBuffer(dwSize),dwSize,sInfo.strPath);
	//strResult.ReleaseBuffer();
	//WritePrivateProfileString("CS","a12",strResult,filename);
	//
	//GetPrivateProfileString("CS","a13","0",strResult.GetBuffer(dwSize),dwSize,sInfo.strPath);
	//strResult.ReleaseBuffer();
	//WritePrivateProfileString("CS","a13",strResult,filename);
	//
	//GetPrivateProfileString("CS","a14","0",strResult.GetBuffer(dwSize),dwSize,sInfo.strPath);
	//strResult.ReleaseBuffer();
	//WritePrivateProfileString("CS","a14",strResult,filename);
	////  波数校正 参数
	//GetPrivateProfileString("xsWaveNum","b0","0",strResult.GetBuffer(dwSize),dwSize,sInfo.strPath);
	//strResult.ReleaseBuffer();
	//WritePrivateProfileString("xsWaveNum","b0",strResult,filename);

	//GetPrivateProfileString("xsWaveNum","b1","0",strResult.GetBuffer(dwSize),dwSize,sInfo.strPath);
	//strResult.ReleaseBuffer();
	//WritePrivateProfileString("xsWaveNum","b1",strResult,filename);

	//GetPrivateProfileString("xsWaveNum","b2","0",strResult.GetBuffer(dwSize),dwSize,sInfo.strPath);
	//strResult.ReleaseBuffer();
	//WritePrivateProfileString("xsWaveNum","b2",strResult,filename);

	//GetPrivateProfileString("xsWaveNum","b3","0",strResult.GetBuffer(dwSize),dwSize,sInfo.strPath);
	//strResult.ReleaseBuffer();
	//WritePrivateProfileString("xsWaveNum","b3",strResult,filename);

	/////////////////////////////////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////////////////////////////////
	//GetPrivateProfileString("waveNumber","a0","0",strResult.GetBuffer(dwSize),dwSize,sInfo.strPath);
	//strResult.ReleaseBuffer();
	//WritePrivateProfileString("waveNumber","a0",strResult,filename);

	//GetPrivateProfileString("waveNumber","a1","0",strResult.GetBuffer(dwSize),dwSize,sInfo.strPath);
	//strResult.ReleaseBuffer();
	//WritePrivateProfileString("waveNumber","a1",strResult,filename);

	//GetPrivateProfileString("waveNumber","a2","0",strResult.GetBuffer(dwSize),dwSize,sInfo.strPath);
	//strResult.ReleaseBuffer();
	//WritePrivateProfileString("waveNumber","a2",strResult,filename);

	//GetPrivateProfileString("waveNumber","a3","0",strResult.GetBuffer(dwSize),dwSize,sInfo.strPath);
	//strResult.ReleaseBuffer();
	//WritePrivateProfileString("waveNumber","a3",strResult,filename);

	//GetPrivateProfileString("waveNumber","a4","0",strResult.GetBuffer(dwSize),dwSize,sInfo.strPath);
	//strResult.ReleaseBuffer();
	//WritePrivateProfileString("waveNumber","a4",strResult,filename);

	//GetPrivateProfileString("waveNumber","a5","0",strResult.GetBuffer(dwSize),dwSize,sInfo.strPath);
	//strResult.ReleaseBuffer();
	//WritePrivateProfileString("waveNumber","a5",strResult,filename);

	//GetPrivateProfileString("waveNumber","a6","0",strResult.GetBuffer(dwSize),dwSize,sInfo.strPath);
	//strResult.ReleaseBuffer();
	//WritePrivateProfileString("waveNumber","a6",strResult,filename);

	//GetPrivateProfileString("waveNumber","a7","0",strResult.GetBuffer(dwSize),dwSize,sInfo.strPath);
	//strResult.ReleaseBuffer();
	//WritePrivateProfileString("waveNumber","a7",strResult,filename);

	//GetPrivateProfileString("waveNumber","a8","0",strResult.GetBuffer(dwSize),dwSize,sInfo.strPath);
	//strResult.ReleaseBuffer();
	//WritePrivateProfileString("waveNumber","a8",strResult,filename);

	//GetPrivateProfileString("waveNumber","a9","0",strResult.GetBuffer(dwSize),dwSize,sInfo.strPath);
	//strResult.ReleaseBuffer();
	//WritePrivateProfileString("waveNumber","a9",strResult,filename);

	//GetPrivateProfileString("waveNumber","a10","0",strResult.GetBuffer(dwSize),dwSize,sInfo.strPath);
	//strResult.ReleaseBuffer();
	//WritePrivateProfileString("waveNumber","a10",strResult,filename);

	//GetPrivateProfileString("waveNumber","a11","0",strResult.GetBuffer(dwSize),dwSize,sInfo.strPath);
	//strResult.ReleaseBuffer();
	//WritePrivateProfileString("waveNumber","a11",strResult,filename);

	//GetPrivateProfileString("waveNumber","a12","0",strResult.GetBuffer(dwSize),dwSize,sInfo.strPath);
	//strResult.ReleaseBuffer();
	//WritePrivateProfileString("waveNumber","a12",strResult,filename);

	//GetPrivateProfileString("waveNumber","a13","0",strResult.GetBuffer(dwSize),dwSize,sInfo.strPath);
	//strResult.ReleaseBuffer();
	//WritePrivateProfileString("waveNumber","a13",strResult,filename);

	//GetPrivateProfileString("waveNumber","a14","0",strResult.GetBuffer(dwSize),dwSize,sInfo.strPath);
	//strResult.ReleaseBuffer();
	//WritePrivateProfileString("waveNumber","a14",strResult,filename);
	////////////////////////////////////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////////////////////////////////////
	//GetPrivateProfileString("CSWaveNum","a0","0",strResult.GetBuffer(dwSize),dwSize,sInfo.strPath);
	//strResult.ReleaseBuffer();
	//WritePrivateProfileString("CSWaveNum","a0",strResult,filename);

	//GetPrivateProfileString("CSWaveNum","a1","0",strResult.GetBuffer(dwSize),dwSize,sInfo.strPath);
	//strResult.ReleaseBuffer();
	//WritePrivateProfileString("CSWaveNum","a1",strResult,filename);

	//GetPrivateProfileString("CSWaveNum","a2","0",strResult.GetBuffer(dwSize),dwSize,sInfo.strPath);
	//strResult.ReleaseBuffer();
	//WritePrivateProfileString("CSWaveNum","a2",strResult,filename);

	//GetPrivateProfileString("CSWaveNum","a3","0",strResult.GetBuffer(dwSize),dwSize,sInfo.strPath);
	//strResult.ReleaseBuffer();
	//WritePrivateProfileString("CSWaveNum","a3",strResult,filename);

	//GetPrivateProfileString("CSWaveNum","a4","0",strResult.GetBuffer(dwSize),dwSize,sInfo.strPath);
	//strResult.ReleaseBuffer();
	//WritePrivateProfileString("CSWaveNum","a4",strResult,filename);

	//GetPrivateProfileString("CSWaveNum","a5","0",strResult.GetBuffer(dwSize),dwSize,sInfo.strPath);
	//strResult.ReleaseBuffer();
	//WritePrivateProfileString("CSWaveNum","a5",strResult,filename);

	//GetPrivateProfileString("CSWaveNum","a6","0",strResult.GetBuffer(dwSize),dwSize,sInfo.strPath);
	//strResult.ReleaseBuffer();
	//WritePrivateProfileString("CSWaveNum","a6",strResult,filename);

	//GetPrivateProfileString("CSWaveNum","a7","0",strResult.GetBuffer(dwSize),dwSize,sInfo.strPath);
	//strResult.ReleaseBuffer();
	//WritePrivateProfileString("CSWaveNum","a7",strResult,filename);

	//GetPrivateProfileString("CSWaveNum","a8","0",strResult.GetBuffer(dwSize),dwSize,sInfo.strPath);
	//strResult.ReleaseBuffer();
	//WritePrivateProfileString("CSWaveNum","a8",strResult,filename);

	//GetPrivateProfileString("CSWaveNum","a9","0",strResult.GetBuffer(dwSize),dwSize,sInfo.strPath);
	//strResult.ReleaseBuffer();
	//WritePrivateProfileString("CSWaveNum","a9",strResult,filename);

	//GetPrivateProfileString("CSWaveNum","a10","0",strResult.GetBuffer(dwSize),dwSize,sInfo.strPath);
	//strResult.ReleaseBuffer();
	//WritePrivateProfileString("CSWaveNum","a10",strResult,filename);

	//GetPrivateProfileString("CSWaveNum","a11","0",strResult.GetBuffer(dwSize),dwSize,sInfo.strPath);
	//strResult.ReleaseBuffer();
	//WritePrivateProfileString("CSWaveNum","a11",strResult,filename);

	//GetPrivateProfileString("CSWaveNum","a12","0",strResult.GetBuffer(dwSize),dwSize,sInfo.strPath);
	//strResult.ReleaseBuffer();
	//WritePrivateProfileString("CSWaveNum","a12",strResult,filename);

	//GetPrivateProfileString("CSWaveNum","a13","0",strResult.GetBuffer(dwSize),dwSize,sInfo.strPath);
	//strResult.ReleaseBuffer();
	//WritePrivateProfileString("CSWaveNum","a13",strResult,filename);

	//GetPrivateProfileString("CSWaveNum","a14","0",strResult.GetBuffer(dwSize),dwSize,sInfo.strPath);
	//strResult.ReleaseBuffer();
	//WritePrivateProfileString("CSWaveNum","a14",strResult,filename);
	
	 MessageBox(pDlg->LoadStringMeau(_T("ID_FINISH")));
}

// 点击校正   校正   校正
void CBxsOffset::OnBnClickedJiaozheng()
{
	// TODO: 在此添加控件通知处理程序代码
	double nm=0;
	UpdateData(TRUE);
	m_tchart.RemoveAllSeries();
	int i =  14;//x长度
	//double *x = new double[i+1];
	//double *y = new double[i+1];
	//double *a = new double[10];
	UpdateData(TRUE);
	CString temp;
	double aa ;
	RightData.clear();
	//  因为可能修改了像素值，所以要从控件里进行读取，存入RightData
	for(int i=0;i<15;i++)
	{
		//temp=m_fexgrid.GetTextMatrix(i,1);
		//aa = atof(temp);
		switch(i)
		{
		case 0:
			aa=b0;
			break;
		case 1:
			aa=b1;
			break;
		case 2:
			aa=b2;
			break;
		case 3:
			aa=b3;
			break;
		case 4:
			aa=b4;
			break;
		case 5:
			aa=b5;
			break;
		case 6:
			aa=b6;
			break;
		case 7:
			aa=b7;
			break;
		case 8:
			aa=b8;
			break;
		case 9:
			aa=b9;
			break;
		case 10:
			aa=b10;
			break;
		case 11:
			aa=b11;
			break;;
		case 12:
			aa=b12;
			break;;
		case 13:
			aa=b13;
			break;
		case 14:
			aa=b14;
			break;
		}
		RightData.push_back(aa);
	}
	m_tchart.AddSeries(0);
	//	m_tchart.GetAxis().GetBottom().GetTitle().SetCaption("Count");
	long color = m_tchart.Series(0).GetColor();
	int va=0;
	Data.clear();
	for (int j=0;j<i;j++)
	{
		va =RightData[j];
		if (va!=0)
		{
			//x[j] = va; 
			//y[j] = LeftData[j];
			Data.push_back(j);
			//m_tchart.Series(0).AddXY(x[j],y[j],NULL,color);
		}
	}
	int jj = Data.size();
	double *x = new double[jj];
	double *y = new double[jj];
	for(int i = 0; i < jj; i++)
	{
		x[i] = RightData[i];
		y[i] = LeftData[i];
		m_tchart.Series(0).AddXY(x[i],y[i],NULL,color);
	}
	double *para = dll_fit3( x, y, jj);
	//ZolixProcessing(jj,x,y,5,a);
	//m_a0=a[0];
	//m_a1=a[1];
	//m_a2=a[2];
	//m_a3=a[3];
	//m_a4=a[4];
	//m_a5=a[5];
	m_a0 = para[3];
	m_a1 = para[2];
	m_a2 = para[1];
	m_a3 = para[0];
	UpdateData(FALSE);
	int flag=m_tchart.AddSeries(0);
	int color1 = m_tchart.Series(flag).GetColor();
	double valuey,valuex;
	for (int j=0;j<jj;j++)
	{
		//valuey = y[j];
		valuex = x[j];
		valuey = m_a0 + m_a1 * pow(valuex, 1) + m_a2 * pow(valuex, 2) + m_a3 * pow(valuex, 3);
		m_tchart.Series(flag).AddXY(valuex,valuey,_T(""),color1);
	}
	delete[] x;
	delete[] y;
	//delete[] a;
	dll_deleteCorrPara();

	// lcz new 07/05 点击校正后，根据拟合结果让用户自己选择是否应用系数
	Sleep(500);
	CINScanLiteDlg *pDlg = (CINScanLiteDlg*)AfxGetApp()->GetMainWnd();
	if(MessageBox(pDlg->LoadStringMeau(_T("ID_APPLY_COEF")), NULL,MB_OKCANCEL|MB_ICONINFORMATION) == IDCANCEL)
		return;
	else
		OnBnClickedUsethisnum();
}

void CBxsOffset::OnEnChangeY1()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}

void CBxsOffset::Correcting(CString str,int nValue)
{
	CString strValue,strResult;
	int dwSize=1024;
	GetPrivateProfileString("CS",str,"0",strResult.GetBuffer(dwSize),dwSize,sInfo.strPath);
    strResult.ReleaseBuffer();
	strValue.Format("%d",atoi(strResult)+nValue);
	if ((atoi(strResult)+nValue)<0)
	{
		strValue.Format("%d",0);
	}

	WritePrivateProfileString("CS",str,strValue,sInfo.strPath);
}

HBRUSH CBxsOffset::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	if (nCtlColor==CTLCOLOR_STATIC)
	{
		pDC->SetBkColor(clr);
		HBRUSH b =CreateSolidBrush(clr);
		return b;
	}
	return hbr;
}

void CBxsOffset::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	CRect rect;
	GetClientRect(rect);
	dc.FillSolidRect(rect,clr);
	CDialog::OnPaint();
}

//  界面初始化时，ShowData(0)
void CBxsOffset::ShowData(int nSerr)
{
	CINScanLiteDlg *pDlg = (CINScanLiteDlg*)AfxGetApp()->GetMainWnd();
	//  如果不是正在检测运行，则先将小图框里的曲线都清空，添加第一条曲线，将大图框里的第一条曲线显示出来
	if (m_bAutoRunning==FALSE&&m_bAutoRunningStatue==FALSE)
	{
		if(pDlg->m_DlgShow.GetTeeCount()>0)
		{
			m_tchart.RemoveAllSeries();
			m_tchart.AddSeries(0);
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
				m_tchart.Series(0).AddXY(i,yvaluelist.GetValue(i),NULL,clrSet);
			}
		}
	}
	//  设置自动缩放
	m_tchart.GetAxis().GetLeft().SetAutomatic(TRUE);
	m_tchart.GetAxis().GetBottom().SetAutomatic(TRUE);
	m_tchart.GetAxis().GetRight().SetAutomatic(TRUE);
	m_tchart.GetAxis().GetTop().SetAutomatic(TRUE);
}

//  预翻译消息---如果键盘按下回车键或者返回键，函数返回true
BOOL CBxsOffset::PreTranslateMessage(MSG* pMsg)
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

//  进行计算校正系数
void CBxsOffset::ZolixProcessing(SHORT Arraylength, DOUBLE* XArray, DOUBLE* YArray, SHORT Degree,DOUBLE* Fvalue)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());// 获取当前资源模块句柄

	// TODO: 在此添加调度处理程序代码
	double fit_value[10]={0};
	double v[9];
	double *y_new;
	y_new=(double *)calloc(Arraylength,sizeof(double));
	Poly_Fitting(Arraylength,XArray,YArray,Degree,fit_value);
	//Fvalue = fit_value;
		for (int j=0;j<Arraylength;j++)
		{
			double new_y = 0;
			for (int d=0;d<9;d++)  // 算	y=a0+a1*x+a2*x^2+……+apoly_n*x^poly_n 的Y值
			{
				v[d] = fit_value[d]*pow(XArray[j],d);
				new_y+=v[d];
				Fvalue[d]=fit_value[d];
			}
			if (YArray[j]<new_y)
			{
				new_y = YArray[j];
			}
			y_new[j] = new_y;
		}
		for (int i=0;i<Arraylength;i++)
		{

			if (YArray[i]!=y_new[i])
			{
				YArray[i]=y_new[i];
			}
		}
	free(y_new);
}

/*==================polyfit(n,x,y,poly_n,a)===================*/
/*=======拟合y=a0+a1*x+a2*x^2+……+apoly_n*x^poly_n========*/
/*=====n是数据个数  xy是数据值  poly_n是多项式的项数======*/
/*===返回a0,a1,a2,……a[poly_n]，系数比项数多一（常数项）=====*/
//  多项式拟合
void CBxsOffset::Poly_Fitting(int n,double x[],double y[],int poly_n,double a[])
{
	int i,j;
	double *tempx,*tempy,*sumxx,*sumxy,*ata;
	tempx=(double *)calloc(n,sizeof(double));
	sumxx=(double *)calloc(poly_n*2+1,sizeof(double));
	tempy=(double *)calloc(n,sizeof(double));
	sumxy=(double *)calloc(poly_n+1,sizeof(double));
	ata=(double *)calloc((poly_n+1)*(poly_n+1),sizeof(double));
	for (i=0;i<n;i++)
	{
		tempx[i]=1;
		tempy[i]=y[i];
	}
	for (i=0;i<2*poly_n+1;i++)
		for (sumxx[i]=0,j=0;j<n;j++)
		{
			sumxx[i]+=tempx[j];
			tempx[j]*=x[j];
		}
		for (i=0;i<poly_n+1;i++)
			for (sumxy[i]=0,j=0;j<n;j++)
			{
				sumxy[i]+=tempy[j];
				tempy[j]*=x[j];
			}
			for (i=0;i<poly_n+1;i++)
				for (j=0;j<poly_n+1;j++)
					ata[i*(poly_n+1)+j]=sumxx[i+j];
			gauss_solve(poly_n+1,ata,a,sumxy);
			free(tempx);
			free(sumxx);
			free(tempy);
			free(sumxy);
			free(ata);
}

//  高斯拟合
void CBxsOffset::gauss_solve(int n,double A[],double x[],double b[])
{
	int i,j,k,r;
	double max;
	//把ata拥有最大斜对角值的行换到第一行，从大到小依次排列，把b按大小依次排列
	for (k=0;k<n-1;k++)
	{
		max=fabs(A[k*n+k]); //find maxmum，假设右斜线都是最大值
		r=k;
		for (i=k+1;i<n-1;i++)
			if (max<fabs(A[i*n+i]))
			{
				max=fabs(A[i*n+i]);	//假设此时max小于其右边斜对角的哪个值，将那个最大值赋予max，并记下行号
				r=i;
			}
			if (r!=k)				//假设斜角上的值在当前行不是最大值的话
				for (i=0;i<n;i++)        //*change array:A[k]&A[r]  
				{
					max=A[k*n+i];
					A[k*n+i]=A[r*n+i];	
					A[r*n+i]=max;		//把最大值那行的值与当前行互换
				}
				max=b[k];                    //change array:b[k]&b[r]  
				b[k]=b[r];		
				b[r]=max;				//ata的一行对应b的一个数，将变换后的ata与b的序号对应
				for (i=k+1;i<n;i++)
				{
					for (j=k+1;j<n;j++)
						A[i*n+j]-=A[i*n+k]*A[k*n+j]/A[k*n+k];
					b[i]-=A[i*n+k]*b[k]/A[k*n+k];
				}
	}  
	for (i=n-1;i>=0;x[i]/=A[i*n+i],i--)
		for (j=i+1,x[i]=b[i];j<n;j++)
			x[i]-=A[i*n+j]*x[j];
}

//  改变选择的曲线
void CBxsOffset::OnCbnSelchangeComboName()
{
	ShowData(m_ComboName.GetCurSel());
}

//  根据计算的中心波长，将波长转为波数
void CBxsOffset::OnEnKillfocusY13()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(true);
	//   lcz new  785仪器，并且选择波长校正，才会修改a13
	if(sCInfo.Laser > 750 && sCInfo.Laser < 800 &&  m_bLenNum )
	{
		double fTemp = 0.0;
		fTemp = pow(10.0,7) / (pow(10.0, 7) / sCInfo.Laser - sCInfo.CorrectHJW );
		//  lcz new  四舍五入，保留两位小数（只对正数有效）
		fTemp = roundDouble(fTemp, 1);
		a13 = fTemp;
		// 将新的系数写入配置文件，但没有更新校正所用的数组
		CString strResult;
		strResult.Format(_T("%.1f"), a13);
		WritePrivateProfileString("bxjzxs","a13",strResult,sInfo.strPath);

		strResult.Format(_T("%.1f"), b13);
		WritePrivateProfileString("CS","a13",strResult,sInfo.strPath);
		LeftData[LeftData.size() - 1] = a13;
	}
	UpdateData(false);
}

void CBxsOffset::OnEnKillfocusY14()
{
	// TODO: 在此添加控件通知处理程序代码
	
	
}

void CBxsOffset::OnBnClickedRadioWavelen()
{
	// TODO: 在此添加控件通知处理程序代码
	CINScanLiteDlg *pDlgF = (CINScanLiteDlg*)AfxGetApp()->GetMainWnd();
	m_bLenNum = true;
	importCorrPara();
	CString tempTEXT = pDlgF->LoadStringMeau(_T("IDD150_12581"));
	SetDlgItemText(IDC_STATIC_BCZH, tempTEXT);
	GetDlgItem(IDC_BTN_LASER_CORR)->EnableWindow(TRUE);
}

void CBxsOffset::OnBnClickedRadioWavenum()
{
	// TODO: 在此添加控件通知处理程序代码
	CINScanLiteDlg *pDlgF = (CINScanLiteDlg*)AfxGetApp()->GetMainWnd();
	m_bLenNum = false;
	importCorrPara();
	//  将波长改为波数
	CString tempTEXT = pDlgF->LoadStringMeau(_T("IDD150_12591"));
	SetDlgItemText(IDC_STATIC_BCZH, tempTEXT);
	GetDlgItem(IDC_BTN_LASER_CORR)->EnableWindow(FALSE);
}
//  将校正参数读取vector，并在界面显示
void CBxsOffset::importCorrPara()
{
	CString strResult1;
	CString strResult2;
	DWORD	dwSize=1024;
	double yv=0.0;
	double xv=0.0;
	double yv1=0.0;
	double xv1=0.0;
	CString str;
	//  从配置文件里读取校正所用的波长值和像素值，并显示在编辑框
	LeftData.clear();

	waveLen.clear(); // 存放一组波长值
	waveLenPixel.clear();// 存放一组波长对应的像素值
	waveNum.clear();// 存放一组波数值
	waveNumPixel.clear();// 存放一组波数对应的像素值

	for (int i=0;i<14;i++)
	{
		str.Format("%d",i);
		str = _T("a")+str;
		//  如果是波长校正
		//if (m_bLenNum)
		//{
		GetPrivateProfileString("bxjzxs", str, "100", strResult1.GetBuffer(dwSize), dwSize, sInfo.strPath);
		yv =atof(strResult1);
		//LeftData.push_back(yv);
		waveLen.push_back(yv);// 存入波长校正的波长参数
		GetPrivateProfileString("CS", str, "100", strResult2.GetBuffer(dwSize), dwSize, sInfo.strPath);
		xv = atof(strResult2);
		waveLenPixel.push_back(xv);//  存入波长校正的像素参数
	//}
	//else //  如果是波数校正
	//{
		GetPrivateProfileString("waveNumber", str, "100", strResult1.GetBuffer(dwSize), dwSize, sInfo.strPath);
		yv1 =atof(strResult1);
		//LeftData.push_back(yv);
		waveNum.push_back(yv1);// 存入波数校正的波数参数
		GetPrivateProfileString("CSWaveNum", str, "100", strResult2.GetBuffer(dwSize), dwSize, sInfo.strPath);
		xv1 = atof(strResult2);
		waveNumPixel.push_back(xv1);//  存入波数校正的像素参数
	//}
		if (!m_bLenNum)// 如果是波数校正
		{
			yv = yv1;
			xv = xv1;
			LeftData = waveNum;
		}
		else
			LeftData = waveLen;
		// lcz new  四舍五入，保留两位小数
		xv = roundDouble(xv, 1);
		yv = roundDouble(yv, 1);
		//yv =atof(strResult1);
		//LeftData.push_back(yv);
		//GetPrivateProfileString("CS", str, "100", strResult2.GetBuffer(dwSize), dwSize, sInfo.strPath);
		//xv = atof(strResult2);
		switch(i)
		{
		case 0:
			a0 = yv;
			b0 = xv;
			break;
		case 1:
			a1 = yv;
			b1 = xv;
			break;
		case 2:
			a2 = yv;
			b2 = xv;
			break;
		case 3:
			a3 = yv;
			b3 = xv;
			break;
		case 4:
			a4 = yv;
			b4 = xv;
			break;
		case 5:
			a5 = yv;
			b5 = xv;
			break;
		case 6:
			a6 = yv;
			b6 = xv;
			break;
		case 7:
			a7 = yv;
			b7 = xv;
			break;
		case 8:
			a8 = yv;
			b8 = xv;
			break;
		case 9:
			a9 = yv;
			b9 = xv;
			break;
		case 10:
			a10 = yv;
			b10 = xv;
			break;
		case 11:
			a11 = yv;
			b11 = xv;
			break;;
		case 12:
			a12 =yv;
			b12 = xv;
			break;;
		case 13:
			a13 = yv;
			b13 = xv;
			break;
		case 14:
			a14 = yv;
			b14 = xv;
			break;
		}
		UpdateData(FALSE);
	}
}

void CBxsOffset::OnBnClickedBtnLaserCorr()
{
	// TODO: 在此添加控件通知处理程序代码
	CDlgLaCorr dlg;
	dlg.DoModal();
}

BEGIN_EVENTSINK_MAP(CBxsOffset, CDialogEx)
	ON_EVENT(CBxsOffset, IDC_TCHART1, 26, CBxsOffset::OnUndoZoomTchart1, VTS_NONE)
END_EVENTSINK_MAP()


void CBxsOffset::OnUndoZoomTchart1()
{
	// TODO: 在此处添加消息处理程序代码
	m_tchart.GetAxis().GetLeft().SetAutomatic(TRUE);
	m_tchart.GetAxis().GetBottom().SetAutomatic(TRUE);
}

//  自动填值 ----- 谱仪必须是连接状态，不然全为0
void CBxsOffset::OnBnClickedBtnAutofill()
{
	// lcz new
	CINScanLiteDlg * pThreadApp = (CINScanLiteDlg*)AfxGetApp()->GetMainWnd();
	//pThreadApp->m_b1064 = true; sInfo.sDInfo.nGXPix = 2095;//-----------------临时添加，自动填值测试
	// TODO: 在此添加控件通知处理程序代码
	
	//  先清空像素值
	b0 = 0.0; b1 = 0.0; b2 = 0.0; b3 = 0.0; b4 = 0.0; b5 = 0.0; b6 = 0.0; b7 = 0.0; 
	b8 = 0.0; b9 = 0.0; b10 = 0.0; b11 = 0.0; b12 = 0.0; b13 = 0.0; b14 = 0.0; 
	UpdateData(FALSE);
	double *bufferX = new double[sInfo.sDInfo.nGXPix];
	memset( bufferX,0,sizeof(double)*(sInfo.sDInfo.nGXPix) );
	if(m_bLenNum)
	{
		//  波长校正方式
		if (sInfo.OminiType != 20)
			m_ZolixCCD.XAxisData(bufferX, sSInfo.a, 4, sInfo.sDInfo.nGXPix);//  bufferX是波长
		//  OTO的CCD调用该方法有误   
		else
			otoXAxisData(bufferX, sSInfo.a, 4, sInfo.sDInfo.nGXPix);
		//m_ZolixCCD.XAxisData(bufferX, sSInfo.a, 4, sInfo.sDInfo.nGXPix);
	}
	else
	{
		//  波数校正方式   如果是OTO或者1064探测器（调用不了ZolixCCD.dll里的接口）
		if( sInfo.OminiType != 20 && !pThreadApp->m_b1064)
			m_ZolixCCD.XAxisData(bufferX, sSInfo.b, 4, sInfo.sDInfo.nGXPix);
		//  OTO的CCD调用该方法有误
		else
			otoXAxisData(bufferX, sSInfo.b, 4, sInfo.sDInfo.nGXPix);
		//m_ZolixCCD.XAxisData(bufferX, sSInfo.b, 4, sInfo.sDInfo.nGXPix);
	}

	
	int nSeriesCount = pThreadApp->m_DlgShow.m_Tchart.GetSeriesCount();
	int index = m_ComboName.GetCurSel();
	vector<double> vecX,vecY;
	CValueList data_x = pThreadApp->m_DlgShow.m_Tchart.Series(index).GetXValues();
	CValueList data_y = pThreadApp->m_DlgShow.m_Tchart.Series(index).GetYValues();
	CString strSeriesName = pThreadApp->m_DlgShow.m_Tchart.Series(index).GetTitle();
	int color = pThreadApp->m_DlgShow.m_Tchart.Series(index).GetColor();
	//  vecX是横坐标值波数，vecY是纵坐标强度值
	for (int j=0;j<data_x.GetCount();j++)
	{
		vecX.push_back(data_x.GetValue(j));
		vecY.push_back(data_y.GetValue(j));
	}
	
	//for (int i=0;i<nSeriesCount;i++)
	//{
	//	//  根据选中了多条，只对第一条数据进行处理
	//	if (m_tchart.Series(i).GetActive())
	//	{
	//		CValueList data_x = m_tchart.Series(i).GetXValues();
	//		CValueList data_y = m_tchart.Series(i).GetYValues();
	//		CString strSeriesName = m_tchart.Series(i).GetTitle();
	//		int color = m_tchart.Series(i).GetColor();
	//		//  vecX是横坐标值波数，vecY是纵坐标强度值
	//		for (int j=0;j<data_x.GetCount();j++)
	//		{
	//			vecX.push_back(data_x.GetValue(j));
	//			vecY.push_back(data_y.GetValue(j));
	//		}
	//		break;
	//	}
	//}
	vector<int> vecIndex;  //    vecIndex记录一组 波数或者波长对应的像素值
	for(int i = 0; i < LeftData.size(); i ++)
	{
		if(LeftData[i] != 0)
		{
			double tempValue = LeftData[i];  //  读取谱峰波数或者波长值, 寻找对应的像素值
			for(int j = 0; j < sInfo.sDInfo.nGXPix - 1; j ++)
			{
				if( ( bufferX[j] <= tempValue ) && ( bufferX[j + 1] >= tempValue ))
				{
					if(abs(bufferX[j] - tempValue) < abs(bufferX[j+1] - tempValue))
						vecIndex.push_back(j);
					else
						vecIndex.push_back(j+1);
					break;//  
				}
			}
		}
	}
	//  找到最接近的波数/波长对应的像素值后，在其左右五个点找极值
	vector<double> seriesInstisy;//  seriesInstisy 记录该像素点附近的约11个点的强度值
	vector<int> vecPixNum;//  vecPixNum记录约11个点的最大强度值点对应的像素索引
	for(int i = 0; i < vecIndex.size(); i++)
	{
		seriesInstisy.clear();
		int index = vecIndex[i];
		int startIndex = 0;
		if(index < 5)
		{
			for(int j = 0; j <= index + 5; j++)
				seriesInstisy.push_back(vecY[j]);
		}
		else if(index >= vecY.size() - 5)
		{
			startIndex = index - 5;
			for(int j = index - 5; j < vecY.size(); j++)
				seriesInstisy.push_back(vecY[j]);
		}
		else
		{
			startIndex = index - 5;
			for(int j = index - 5; j <= index + 5; j++)
				seriesInstisy.push_back(vecY[j]);
		}
		int maxIndex = max_element(seriesInstisy.begin(), seriesInstisy.end()) - seriesInstisy.begin() + startIndex;
		//CString s_maxIndex;
		//s_maxIndex.Format("%d", maxIndex);
		//m_List_Correct.SetItemText(i, 1, s_maxIndex);
		vecPixNum.push_back(maxIndex);
	}
	int peakNum = vecIndex.size();
	//  lcz new 通过高斯拟合，计算准确的像素点
	vector<double> seriesInstisyOfMaxPixel;//  seriesInstisy 记录极值点对应的像素点附近的约5个点的强度值
	vector<double> seriesPixelOfMaxPixel;//  记录五个点的像素值
	vector<double> vecRealPixNum;//  vecRealPixNum记录约5个点的拟合得到的像素索引
	for(int i = 0; i < vecIndex.size(); i++)
	{
		seriesInstisyOfMaxPixel.clear();
		seriesPixelOfMaxPixel.clear();
		int index = vecPixNum[i];
		int startIndex = 0;
		if(index < 2)
		{
			for(int j = 0; j <= index + 2; j++)
			{
				seriesPixelOfMaxPixel.push_back(j);
				seriesInstisyOfMaxPixel.push_back(vecY[j]);
			}
		}
		else if(index >= vecY.size() - 2)
		{
			startIndex = index - 2;
			for(int j = index - 2; j < vecY.size(); j++)
			{
				seriesPixelOfMaxPixel.push_back(j);
				seriesInstisyOfMaxPixel.push_back(vecY[j]);
			}
		}
		else
		{
			startIndex = index - 2;
			for(int j = index - 2; j <= index + 2; j++)
			{
				seriesPixelOfMaxPixel.push_back(j);
				seriesInstisyOfMaxPixel.push_back(vecY[j]);
			}
		}
		double realPixel = 0;
		if(seriesInstisyOfMaxPixel.size() > 0)
		{
			double* aa = new double[seriesInstisyOfMaxPixel.size() * 2];
			for(int i = 0; i < seriesInstisyOfMaxPixel.size(); i++)
			{
				aa[i] = seriesPixelOfMaxPixel[i];
				aa[i + seriesInstisyOfMaxPixel.size()] = seriesInstisyOfMaxPixel[i];
			}
			realPixel = dll_realPeak(aa, seriesInstisyOfMaxPixel.size() * 2);
			//  lcz new  四舍五入，保留两位小数（只对正数有效）
			realPixel = roundDouble(realPixel, 1);
			if(aa)
			{
				delete[] aa;
				aa = NULL;
			}
		}
		else
			realPixel = 0.0;
		//int maxIndex = max_element(seriesInstisy.begin(), seriesInstisy.end()) - seriesInstisy.begin() + startIndex;
		//CString s_maxIndex;
		//s_maxIndex.Format("%d", maxIndex);
		//m_List_Correct.SetItemText(i, 1, s_maxIndex);
		vecRealPixNum.push_back(realPixel);
	}

	switch(peakNum)
	{
	case 15:
		b14 = vecRealPixNum[14];
	case 14:
		b13 = vecRealPixNum[13];
	case 13:
		b12 = vecRealPixNum[12];
	case 12:
		b11 = vecRealPixNum[11];
	case 11:
		b10 = vecRealPixNum[10];
	case 10:
		b9 = vecRealPixNum[9];
	case 9:
		b8 = vecRealPixNum[8];
	case 8:
		b7 = vecRealPixNum[7];
	case 7:
		b6 = vecRealPixNum[6];
	case 6:
		b5 = vecRealPixNum[5];
	case 5:
		b4 = vecRealPixNum[4];
	case 4:
		b3 = vecRealPixNum[3];
	case 3:
		b2 = vecRealPixNum[2];
	case 2:
		b1 = vecRealPixNum[1];
	case 1:
		b0 = vecRealPixNum[0];
	case 0:
		break;
	}
	UpdateData(FALSE);

	if(bufferX)
	{
		delete [] bufferX;
		bufferX = NULL;
	}
}
