#include "stdafx.h"      // 如果使用预编译头文件，则包含它；否则可去掉
#include "DlgA1Set.h"
#include "afxdialogex.h" // 对话框扩展类头文件
#include "A1Camera.h"
#include "ZolixCCD.h"
#define WM_A1_THREAD_IMAGE (WM_APP + 100)

NS_MFC_Res_BEGIN

IMPLEMENT_DYNAMIC(CDlgA1Set, CDialog)
CDlgA1Set *a1_pDlg = NULL;
CDlgA1Set::CDlgA1Set(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgA1Set::IDD, pParent)
{
	// TODO: 在此添加构造代码
	m_nGhostStart = 0;
	m_nGhostEnd = 0;
}

CDlgA1Set::~CDlgA1Set()
{
}

void CDlgA1Set::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	
	DDX_Text(pDX, IDC_EDIT1_GAIN, stuA1CCD.iGain);
	DDX_Text(pDX, IDC_EDIT_A1_SETTIME, fTime);
	DDX_Text(pDX, IDC_EDIT_BADPOINT, stuA1CCD.iCbpNum);
	DDX_Text(pDX, IDC_EDIT_BADPOINT2, stuA1CCD.strCbp);
	DDX_Text(pDX, IDC_EDIT_GHOST, stuA1CCD.ghostImageStrRelative);
}

BEGIN_MESSAGE_MAP(CDlgA1Set, CDialog)
	// TODO: 在此添加消息映射
	ON_BN_CLICKED(IDC_BTN_SETGAIN, &CDlgA1Set::OnSetGainButtonClicked)
	ON_BN_CLICKED(IDC_BTN_SETSPEED, &CDlgA1Set::OnSetSpeedButtonClicked)
	ON_BN_CLICKED(IDC_A1_SETCOOL, &CDlgA1Set::OnSetCoolButtonClicked)

	ON_BN_CLICKED(IDC_BUTTON_A1IMAGE, &CDlgA1Set::OnA1ImageStart)
	ON_MESSAGE(WM_A1_THREAD_IMAGE, &CDlgA1Set::OnA1ThreadImage) //绘制
	ON_BN_CLICKED(IDC_A1_STOPIMAGE, &CDlgA1Set::OnBnClickedStopImage)
	ON_BN_CLICKED(IDC_A1_MIRR, &CDlgA1Set::OnBnClickedMIRR)
	ON_BN_CLICKED(IDC_A1_SAVEIAMGE, &CDlgA1Set::OnBnClickedSaveImage)
	ON_BN_CLICKED(IDC_CHECK_CURSOR, &CDlgA1Set::OnBnClickedCheckCursor)
	ON_BN_CLICKED(IDC_BTN_BADCHANGE, &CDlgA1Set::OnBnClickedBadChange)
	ON_BN_CLICKED(IDC_BTN_REMOVE_GHOST, &CDlgA1Set::OnBnClickedBtnRemoveGhost)
	ON_CBN_SELCHANGE(IDC_COMBO_A1MIR, &CDlgA1Set::OnCbnSelchangeComboMirror)

	ON_WM_TIMER()
		
END_MESSAGE_MAP()

BOOL CDlgA1Set::OnInitDialog()
{
	CDialog::OnInitDialog();

	InitComboGain();  //初始化增益
	CComboBox* pComboSpeed = (CComboBox*)GetDlgItem(IDC_COMBO_A1_SPEED);
	InitTemp();
	if (pComboSpeed)
	{
		pComboSpeed->AddString(_T("250KHz"));
		pComboSpeed->AddString(_T("125KHz"));
		pComboSpeed->AddString(_T("62.5KHz"));
		pComboSpeed->AddString(_T("31.25KHz"));

	}
	CComboBox* pComboMirror = (CComboBox*)GetDlgItem(IDC_COMBO_A1MIR);
	if (pComboMirror)
	{
	
		 pComboMirror->AddString(_T("反向镜像"));
		 pComboMirror->AddString(_T("正向镜像"));

		 if (stuA1CCD.isMirror == 0) {
			 pComboMirror->SetCurSel(0);
		 }
		 if (stuA1CCD.isMirror == 1) {
			 pComboMirror->SetCurSel(1);
		 }
		 
	}
	CRect rect;
	GetDlgItem(IDC_STATIC_BMP)->GetWindowRect(&rect);
	GetDlgItem(IDC_STATIC_BMP)->ModifyStyle(0, WS_CLIPSIBLINGS | WS_CLIPCHILDREN);
	ScreenToClient(&rect);
	dlgImage.Create(ID_DLG_IMAGE, this);

	::SetWindowPos(dlgImage.m_hWnd, HWND_TOP, rect.left, rect.top, rect.Width(), rect.Height(), SWP_SHOWWINDOW | SWP_FRAMECHANGED);
	dlgImage.ModifyStyle(0, WS_CLIPSIBLINGS);
	dlgImage.SetParent(this);
	
	GetDlgItem(IDC_A1_STOPIMAGE)->EnableWindow(FALSE);
	GetDlgItem(IDC_A1_MIRR)->EnableWindow(FALSE);
	
	SetString(0, 0);
	SetTimer(1, 100, NULL);
	fTime = 0.1f;
	UpdateData(FALSE);
	
	return TRUE;  // 返回 TRUE 除非将焦点设置到控件上
}
void CDlgA1Set::InitComboGain() {
	short fGain = 0;
	pA1Camera->GetGain(0, &fGain);
	stuA1CCD.iGain = fGain;
	UpdateData(FALSE);
}


void CDlgA1Set::OnSetSpeedButtonClicked() {

	CComboBox* pComboSpeed = (CComboBox*)GetDlgItem(IDC_COMBO_A1_SPEED);
	CString selText;
	pComboSpeed->GetWindowText(selText);

	if (selText == _T("250KHz"))
	{
		if(pA1Camera->SetSpeed(speed_0))
		AfxMessageBox(_T("速率已设置为 250KHz"));
	}
	else if (selText == _T("125KHz"))
	{
		if (pA1Camera->SetSpeed(speed_1))	
			AfxMessageBox(_T("速率已设置为 125KHz"));
	}
	else if (selText == _T("62.5KHz"))
	{
		if (pA1Camera->SetSpeed(speed_2))	
			AfxMessageBox(_T("速率已设置为 62.5KHz"));
	}
	else if (selText == _T("31.25KHz"))
	{
		if (pA1Camera->SetSpeed(speed_3))
			AfxMessageBox(_T("速率已设置为 31.25KHz"));
	}
	else
	{
		// 如果下拉框中未选或选了其他文本，则提示用户
		CString msg;
		msg.Format(_T("无法识别此速率选项: %s"), selText);
		AfxMessageBox(msg);
	}
	/*CString tPath, str1, str2, str3, szText;
	GetModuleFileName(NULL, tPath.GetBufferSetLength(MAX_PATH + 1), MAX_PATH);
	tPath.ReleaseBuffer();
	int nPos = tPath.ReverseFind('\\');
	str1 = tPath.Left(nPos);
	str3 = str1 + _T("\\") + strIniName;

	CString str;

	WritePrivateProfileString(sA1, _T("ReadoutSpeed"), selText, str3);*/
}

void CDlgA1Set::OnSetCoolButtonClicked() {

	
	CString str;
	GetDlgItem(IDC_EDIT_A1_COOL)->GetWindowText(str);
	pA1Camera->SetTemper(atoi(str));
	pA1Camera->SetCooler(1);
	
}
void CDlgA1Set::InitTemp() {

	CString str;
	str.Format(_T("%d"), stuA1CCD.nTemp);
	GetDlgItem(IDC_EDIT_A1_COOL)->SetWindowText(str);

}


void CDlgA1Set::OnBnClickedMIRR() {

	stuAndorCCD.HMirror = !stuAndorCCD.HMirror;

}
UINT CCDImageA1(LPVOID lParam)
{

	auto pDlg = reinterpret_cast<CDlgA1Set*>(lParam);
	if (!pDlg)
	{
		return 0;
	}
	pA1Camera->SetReadoutMode(mode_image);  

	int width = 0, height = 0;
	pA1Camera->GetDevSize(&width,&height);  //此时width是2048,height为64

	short binh = 0, binv = 0;
	pA1Camera->GetBin(&binh, &binv);

	if (binh <= 0) binh = 1;
	if (binv <= 0) binv = 1;

	width = width / binh;
	height = height / binv;

	
	unsigned short* imageBuffer = new unsigned short[width*height];
	memset(imageBuffer, 0, width*height * sizeof(unsigned short));
	

	BYTE *byteBuffer = new BYTE[width*height* 3];
	memset(byteBuffer, 0, width*height* 3);
	
	//设置积分时间
	CString strExpTime;
	pDlg->GetDlgItem(IDC_EDIT_A1_SETTIME)->GetWindowText(strExpTime);
	float expTime = static_cast<float>(_ttof(strExpTime));
	if (expTime <= 0.0f)
	{
		expTime = 1.0f; // 默认1秒
	}
	pA1Camera->SetExpTime(expTime);

	while (pDlg->m_CameraState==TRUE) {
		pA1Camera->ImageDataAcquisition(imageBuffer, byteBuffer, width, height);
	
		auto pThreadData = new CDlgA1Set::A1_THREAD_DATA;
		pThreadData->width = width;
		pThreadData->height = height;
		pThreadData->imageBuffer.resize(width * height);
		pThreadData->byteBuffer.resize(width * height * 3);

		memcpy(pThreadData->imageBuffer.data(), imageBuffer, width * height * sizeof(unsigned short));
		memcpy(pThreadData->byteBuffer.data(), byteBuffer, width * height * 3);


		pDlg->PostMessage(WM_A1_THREAD_IMAGE, 0, reinterpret_cast<LPARAM>(pThreadData));
		Sleep(50);
	}

	pA1Camera->SetReadoutMode(mode_fvb);



	delete[] imageBuffer;
	delete[] byteBuffer;
	
	return 0;

}
void CDlgA1Set::OnA1ImageStart() {
	//这里要写一个按钮状态
	GetDlgItem(IDC_BUTTON_A1IMAGE)->EnableWindow(FALSE);
	GetDlgItem(IDC_A1_STOPIMAGE)->EnableWindow(true);
	GetDlgItem(IDC_A1_MIRR)->EnableWindow(true);
	GetDlgItem(IDC_A1_SAVEIAMGE)->EnableWindow(FALSE);
	m_CameraState = TRUE;
	AfxBeginThread(CCDImageA1, (LPVOID)this);

}
void CDlgA1Set::OnBnClickedStopImage() {
	//这里要写一个按钮状态
	//...

	m_CameraState = FALSE;
	GetDlgItem(IDC_A1_STOPIMAGE)->EnableWindow(FALSE);
	GetDlgItem(IDC_A1_MIRR)->EnableWindow(FALSE);
	GetDlgItem(IDC_A1_SAVEIAMGE)->EnableWindow(true);
	GetDlgItem(IDC_BUTTON_A1IMAGE)->EnableWindow(true);

}
void CDlgA1Set::OnBnClickedSaveImage() {

	if (m_CameraState == FALSE) {
		dlgImage.SavePicBmp();
	}

}
LRESULT CDlgA1Set::OnA1ThreadImage(WPARAM wParam, LPARAM lParam)
{
	A1_THREAD_DATA* pData = reinterpret_cast<A1_THREAD_DATA*>(lParam);
	if (!pData) return 0;

	// （1）先用 pData->width / pData->height 来设置显示尺寸
	m_nImageWidth = pData->width;
	m_nImageHeight = pData->height;

	dlgImage.SetPicSize(pData->width, pData->height);
	memcpy(dlgImage.pBuffer, pData->byteBuffer.data(), pData->width * pData->height * 3);
	dlgImage.OnDisplay();


	delete pData;
	pData = nullptr;

	return 0;
}
BOOL CDlgA1Set::PreTranslateMessage(MSG * pMsg) {

	CRect rect;
	dlgImage.GetWindowRect(&rect);
	if (rect.PtInRect(pMsg->pt))
	{
		int x, y;
		dlgImage.GetPoint(&x, &y);
		SetString(x, y);
	}
	if (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE)
		return TRUE;
	else
		return CDialog::PreTranslateMessage(pMsg);
}
void CDlgA1Set::SetString(int x, int y)
{
	CString str;
	str.Format(_T("X:%4d"), x);
	m_StaticX = str;
	str.Format(_T("Y:%4d"), y);
	m_StaticY = str;

}
void CDlgA1Set::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == 1)
	{
		GetDlgItem(IDC_STATIC_X)->SetWindowText(m_StaticX);
		GetDlgItem(IDC_STATIC_Y)->SetWindowText(m_StaticY);
	}

	CDialog::OnTimer(nIDEvent);
}
void CDlgA1Set::OnBnClickedCheckCursor()
{
	int state = ((CButton *)GetDlgItem(IDC_CHECK_CURSOR))->GetCheck();
	dlgImage.SetMark(state);
}
void CDlgA1Set::OnBnClickedBadChange(){
	CString str;

	GetDlgItem(IDC_EDIT_BADPOINT)->GetWindowText(str);
	stuA1CCD.iCbpNum = atoi(str);

	GetDlgItem(IDC_EDIT_BADPOINT2)->GetWindowText(str);
	stuA1CCD.strCbp = str;
	ManageZolixIni->WriteIni();

}
void CDlgA1Set::OnSetGainButtonClicked() {

	UpdateData(TRUE);

	pA1Camera->SetGainA1(stuA1CCD.iGain);
}


void CDlgA1Set::OnBnClickedBtnRemoveGhost() {

	//1.从界面读用户输入的鬼像区域到ghostImageStrRelative
	UpdateData(true);
	if (stuA1CCD.ghostImageStrRelative.Trim().IsEmpty())
	{
		AfxMessageBox(_T("已修正"));
		stuA1CCD.ghostImageStrAbsolute = "";
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
	GetPrivateProfileString(_T("A1"), _T("dataOffset"), _T("0"), szOffset, 256, iniPath);
	int offset = _ttoi(szOffset);  // 转换为 int
	
    //2.更新绝对范围
	CString result;
	CStringArray ranges;

	CString& ghostStr = stuA1CCD.ghostImageStrRelative;
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
	stuA1CCD.ghostImageStrAbsolute = result;

	//3.写回配置文件范围字段(绝对范围)
	ManageZolixIni->WriteIni();
    AfxMessageBox("已修正");
}


void CDlgA1Set::OnCbnSelchangeComboMirror()
{
	stuA1CCD.isMirror = ((CComboBox *)GetDlgItem(IDC_COMBO_A1MIR))->GetCurSel();
	ManageZolixIni->WriteIni();
}
NS_MFC_Res_END
