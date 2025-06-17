#include "stdafx.h"
#include "INScan-Lite.h"
#include "DialogQualityAnalyse.h"
#include "afxdialogex.h"

#include ".\lines\zolix_quantiAnalyse.h"
#include <iostream>
#include <string>
#include <Windows.h>
#include "pdflib.h"
#include <fstream>

// CDialogQualityAnalyse 对话框

IMPLEMENT_DYNAMIC(CDialogQualityAnalyse, CDialogEx)

CDialogQualityAnalyse::CDialogQualityAnalyse(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_QUALITY_ANALYSE, pParent)
{
	m_db = nullptr;
	m_errMsg = nullptr;
	m_nRow = 0;
	m_nColumn = 0;
	needSetResultCol = true;
}

CDialogQualityAnalyse::~CDialogQualityAnalyse()
{
	if (m_db != nullptr)
	{
		sqlite3_close(m_db);
		m_db = nullptr;
	}
}

void CDialogQualityAnalyse::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_PURE, m_list_pure);
	DDX_Control(pDX, IDC_LIST_MIX, m_list_mix);
	DDX_Control(pDX, IDC_LIST_RATIO, m_list_ratio);
	DDX_Text(pDX, IDC_EDIT_MODEL_NAME, m_strModelName);
	DDX_Control(pDX, IDC_LIST_LSRP_MODEL, m_list_models);
	DDX_Control(pDX, IDC_LIST_RESULT, m_list_result);

}


BEGIN_MESSAGE_MAP(CDialogQualityAnalyse, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_ADD_PURE_DATA, &CDialogQualityAnalyse::OnBnClickedButtonAddPureData)
	ON_BN_CLICKED(IDC_BUTTON_ADD_MIX_DATA, &CDialogQualityAnalyse::OnBnClickedButtonAddMixData)
	ON_BN_CLICKED(IDC_BUTTON_IMPORT_RATIO, &CDialogQualityAnalyse::OnBnClickedButtonImportRatio)
	ON_BN_CLICKED(IDC_BUTTON_BUILD_MODEL, &CDialogQualityAnalyse::OnBnClickedButtonBuildModel)
	ON_BN_CLICKED(IDC_BTN_SEL_PURE, &CDialogQualityAnalyse::OnBnClickedBtnSelPure)
	ON_BN_CLICKED(IDC_BTN_SEL_MIX, &CDialogQualityAnalyse::OnBnClickedBtnSelMix)
	ON_BN_CLICKED(IDC_BTN_BUILD_NETMODEL, &CDialogQualityAnalyse::OnBnClickedBtnBuildNetmodel)
	ON_BN_CLICKED(IDC_RADIO_PLS, &CDialogQualityAnalyse::OnBnClickedRadioPls)
	ON_BN_CLICKED(IDC_RADIO_LPLS, &CDialogQualityAnalyse::OnBnClickedRadioLpls)
	ON_BN_CLICKED(IDC_BUTTON_CALCULATE, &CDialogQualityAnalyse::OnBnClickedButtonCalculate)
	ON_BN_CLICKED(IDC_BTN_SEL_MODEL, &CDialogQualityAnalyse::OnBnClickedBtnSelModel)
	ON_BN_CLICKED(IDC_BUTTON_OUTPUT_PDF, &CDialogQualityAnalyse::OnBnClickedButtonOutputPdf)
	ON_BN_CLICKED(IDC_BTN_SEL_PRED, &CDialogQualityAnalyse::OnBnClickedBtnSelPred)
	ON_BN_CLICKED(IDC_BTN_PURE_DEL, &CDialogQualityAnalyse::OnBnClickedBtnPureDel)
	ON_BN_CLICKED(IDC_BUTTON_MIX_DEL, &CDialogQualityAnalyse::OnBnClickedButtonMixDel)
	ON_BN_CLICKED(IDC_BTN_DEL_MODEL, &CDialogQualityAnalyse::OnBnClickedBtnDelModel)
END_MESSAGE_MAP()

BOOL CDialogQualityAnalyse::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	m_list_pure.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES); 
	m_list_pure.InsertColumn(0, _T("序号"), LVCFMT_CENTER, 0);
	m_list_pure.InsertColumn(1, _T("名称"), LVCFMT_CENTER, 0);

	CRect rcClient;
	m_list_pure.GetClientRect(&rcClient);
	int totalWidth = rcClient.Width();


	int colWidth0 = static_cast<int>(totalWidth * 0.30);
	int colWidth1 = totalWidth - colWidth0;  
								 
	m_list_pure.SetColumnWidth(0, colWidth0);
	m_list_pure.SetColumnWidth(1, colWidth1);

	m_list_mix.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_list_mix.InsertColumn(0, _T("序号"), LVCFMT_CENTER, 0);
	m_list_mix.InsertColumn(1, _T("名称"), LVCFMT_CENTER, 0);

	m_list_mix.GetClientRect(&rcClient);
	totalWidth = rcClient.Width();
	colWidth0 = static_cast<int>(totalWidth * 0.30);
	colWidth1 = totalWidth - colWidth0;
	m_list_mix.SetColumnWidth(0, colWidth0);
	m_list_mix.SetColumnWidth(1, colWidth1);

	m_list_ratio.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	m_list_models.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_list_models.InsertColumn(0, _T("序号"), LVCFMT_CENTER, 0);
	m_list_models.InsertColumn(1, _T("名称"), LVCFMT_CENTER, 0);
	m_list_models.GetClientRect(&rcClient);
	totalWidth = rcClient.Width();
	colWidth0 = static_cast<int>(totalWidth * 0.30);
	colWidth1 = totalWidth - colWidth0;
	m_list_models.SetColumnWidth(0, colWidth0);
	m_list_models.SetColumnWidth(1, colWidth1);



	//这里调用初始化DB的逻辑
	if (!InitDB())
	{
		AfxMessageBox(_T("数据库初始化失败！"));
	}

	if (!LoadPureTableToList())
	{
		AfxMessageBox(_T("加载 pureTable 数据失败！"));
	}
	if (!LoadMixTableToList())
	{
		AfxMessageBox(_T("加载 pureTable 数据失败！"));
	}

	return TRUE;  
}


//初始化数据库,添加纯净物,混合物到列表中
BOOL CDialogQualityAnalyse::InitDB()
{
	//创建纯净物表
	CString dbPath = _T("QuantiAnalyse429.db");
	int result = sqlite3_open(CT2A(dbPath), &m_db);
	if (result != SQLITE_OK) {
		AfxMessageBox(_T("无法打开数据库！"));
		return FALSE;
	}
	const char* createPureTable =
		"CREATE TABLE IF NOT EXISTS pureTable ("
		"id INTEGER PRIMARY KEY AUTOINCREMENT,"
		"name TEXT NOT NULL,"
		"original_data_x TEXT,"
		"original_data_y TEXT,"
		"processed_data_y TEXT);";

	result = sqlite3_exec(m_db, createPureTable, NULL, NULL, NULL);
	if (result != SQLITE_OK) {
		AfxMessageBox(_T("创建 pure 表失败！"));
		return FALSE;
	}

	//创建混合物表
	if (result != SQLITE_OK) {
		AfxMessageBox(_T("无法打开数据库！"));
		return FALSE;
	}
	const char* createMixTable =
		"CREATE TABLE IF NOT EXISTS mixTable ("
		"id INTEGER PRIMARY KEY AUTOINCREMENT,"
		"name TEXT NOT NULL,"
		"original_data_x TEXT,"
		"original_data_y TEXT,"
		"processed_data_y TEXT);";
	result = sqlite3_exec(m_db, createMixTable, NULL, NULL, NULL);
	if (result != SQLITE_OK) {
		AfxMessageBox(_T("创建 MIX 表失败！"));
		return FALSE;
	}

	//创建PLS模型表
	const char* createPlsModel =
		"CREATE TABLE IF NOT EXISTS pls_model ("
		"id INTEGER PRIMARY KEY AUTOINCREMENT,"
		"name TEXT UNIQUE NOT NULL,"
		"pls_theta TEXT,"
		"pureNum INT,"
		"mixtureNum INT,"
		"thetaDim INT,"
		"pureName TEXT);"; // 添加 pureName 字段

	result = sqlite3_exec(m_db, createPlsModel, NULL, NULL, NULL);
	if (result != SQLITE_OK) {
		AfxMessageBox(_T("创建 PLS 表失败！"));
		return FALSE;
	}

	// 创建 lsrp_model 表
	const char* createLsrpModel =
		"CREATE TABLE IF NOT EXISTS lsrp_model ("
		"id INTEGER PRIMARY KEY AUTOINCREMENT,"
		"name TEXT UNIQUE NOT NULL,"
		"lsrp_theta TEXT,"
		"pureNum INT,"
		"mixtureNum INT,"
		"thetaDim INT,"
		"pureName TEXT);";  // 添加 pureName 字段

	result = sqlite3_exec(m_db, createLsrpModel, NULL, NULL, NULL);
	if (result != SQLITE_OK) {
		AfxMessageBox(_T("创建 lsrp_model 表失败！"));
		return FALSE;
	}


	return TRUE;
}
BOOL CDialogQualityAnalyse::LoadMixTableToList() {
	m_list_mix.DeleteAllItems();

	// 查询 pureTable
	CString sql = _T("SELECT id,name FROM mixTable");
	char ** dbResult = nullptr;
	int nRows = 0, nCols = 0;
	int rc = sqlite3_get_table(m_db, CT2A(sql), &dbResult, &nRows, &nCols, &m_errMsg);
	if (rc != SQLITE_OK) {
		CString errorMsg;
		errorMsg.Format(_T("查询 pureTable 数据失败：%hs"), m_errMsg);
		AfxMessageBox(errorMsg);
		sqlite3_free_table(dbResult);
		return FALSE;
	}

	// 注意 dbResult[0], dbResult[1] 是列名行，这里从 i=1 开始取数据
	for (int i = 1; i <= nRows; ++i)
	{
		// dbResult[i*nCols + 0] 是 id
		// dbResult[i*nCols + 1] 是 name
		CString id = Utf8ToCString(dbResult[i * nCols + 0]);
		CString name = Utf8ToCString(dbResult[i * nCols + 1]);

		// 列表 0 列要显示基于行号的序号
		// 比如 i-1 或者直接用 i
		// 这里用 i 做显示(1-based)
		CString displayIndex;
		displayIndex.Format(_T("%d"), i);

		// 插入一行
		int rowIndex = m_list_mix.InsertItem(i - 1, displayIndex);
		// 第 1 列显示名称
		m_list_mix.SetItemText(rowIndex, 1, name);

		// 用 itemData 记住数据库 id，方便删除/编辑
		m_list_mix.SetItemData(rowIndex, _ttoi(id));
	}

	sqlite3_free_table(dbResult);
	return TRUE;
}
BOOL CDialogQualityAnalyse::LoadPureTableToList()
{
	m_list_pure.DeleteAllItems();

	// 查询 pureTable
	CString sql = _T("SELECT id,name FROM pureTable");
	char ** dbResult = nullptr;
	int nRows = 0, nCols = 0;
	int rc = sqlite3_get_table(m_db, CT2A(sql), &dbResult, &nRows, &nCols, &m_errMsg);
	if (rc != SQLITE_OK) {
		CString errorMsg;
		errorMsg.Format(_T("查询 pureTable 数据失败：%hs"), m_errMsg);
		AfxMessageBox(errorMsg);
		sqlite3_free_table(dbResult);
		return FALSE;
	}

	// 注意 dbResult[0], dbResult[1] 是列名行，这里从 i=1 开始取数据
	for (int i = 1; i <= nRows; ++i)
	{
		// dbResult[i*nCols + 0] 是 id
		// dbResult[i*nCols + 1] 是 name
		CString id = Utf8ToCString(dbResult[i * nCols + 0]);
		CString name = Utf8ToCString(dbResult[i * nCols + 1]);

		// 列表 0 列要显示基于行号的序号
		// 比如 i-1 或者直接用 i
		// 这里用 i 做显示(1-based)
		CString displayIndex;
		displayIndex.Format(_T("%d"), i);

		// 插入一行
		int rowIndex = m_list_pure.InsertItem(i - 1, displayIndex);
		// 第 1 列显示名称
		m_list_pure.SetItemText(rowIndex, 1, name);

		// 用 itemData 记住数据库 id，方便删除/编辑
		m_list_pure.SetItemData(rowIndex, _ttoi(id));
	}

	sqlite3_free_table(dbResult);
	return TRUE;
}


//编码转换
CString CDialogQualityAnalyse::Utf8ToCString(const char* utf8Str)
{
	if (!utf8Str) return _T("");

	// UTF-8 转 WideChar
	int wideLen = MultiByteToWideChar(CP_UTF8, 0, utf8Str, -1, NULL, 0);
	if (wideLen <= 0) return _T("");

	std::wstring wideStr(wideLen, L'\0');
	MultiByteToWideChar(CP_UTF8, 0, utf8Str, -1, &wideStr[0], wideLen);

	// WideChar 转 CString
#ifdef UNICODE
	return CString(wideStr.c_str());
#else
	int ansiLen = WideCharToMultiByte(CP_ACP, 0, wideStr.c_str(), -1, NULL, 0, NULL, NULL);
	if (ansiLen <= 0) return _T("");

	std::string ansiStr(ansiLen, '\0');
	WideCharToMultiByte(CP_ACP, 0, wideStr.c_str(), -1, &ansiStr[0], ansiLen, NULL, NULL);

	return CString(ansiStr.c_str());
#endif
}
std::string CDialogQualityAnalyse::AnsiToUtf8(const char* ansiStr, UINT codePage )
{
	// 第一步：先把多字节(ANSI) 转成宽字符
	int wideLen = MultiByteToWideChar(codePage, 0, ansiStr, -1, NULL, 0);
	if (wideLen <= 0) {
		return std::string();
	}
	std::wstring wideBuf(wideLen, L'\0');
	MultiByteToWideChar(codePage, 0, ansiStr, -1, &wideBuf[0], wideLen);

	// 第二步：把宽字符转成 UTF-8
	int utf8Len = WideCharToMultiByte(CP_UTF8, 0, wideBuf.c_str(), -1, NULL, 0, NULL, NULL);
	if (utf8Len <= 0) {
		return std::string();
	}
	std::string utf8Buf(utf8Len, '\0');
	WideCharToMultiByte(CP_UTF8, 0, wideBuf.c_str(), -1, &utf8Buf[0], utf8Len, NULL, NULL);

	return utf8Buf;
}


//点击添加纯净物按钮,从文件中读取纯净物数据,添加到纯净物数据库,选择列表保留项
void CDialogQualityAnalyse::OnBnClickedButtonAddPureData()
{

	// TODO: 在此添加控件通知处理程序代码
	CFileDialog fileDlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		_T("CSV Files (*.csv)|*.csv|TXT Files (*.txt)|*.txt||"));
	if (fileDlg.DoModal() == IDOK) {
		CString filePath = fileDlg.GetPathName(); // 获取文件路径
		if (ImportPureFromFile(filePath)) {
			//AfxMessageBox(_T("数据导入成功！"));
		}
		else {
			AfxMessageBox(_T("数据导入失败，请检查文件内容！"));
		}

	}
	if (!LoadPureTableToList())
	{
		AfxMessageBox(_T("展示到列表失败！"));
	}

}
BOOL CDialogQualityAnalyse::ImportPureFromFile(const CString& filePath)
{

	vector<double> vecX, vecY;
	CString fileName;
	if (!ProcessFile(filePath, vecX, vecY, fileName))
	{
		AfxMessageBox(_T("processFile执行失败！"));
		return false;
	}

	CString strX, strY;
	for (size_t i = 0; i < vecX.size(); i++)
	{
		CString tempX, tempY;
		tempX.Format(_T("%.6f"), vecX[i]); // 保留小数位
		tempY.Format(_T("%.6f"), vecY[i]);

		strX += tempX;
		strY += tempY;
		if (i != vecX.size() - 1)
		{
			strX += _T(",");
			strY += _T(",");
		}
	}

	int pointCount = (int)vecX.size();
	if (pointCount <= 0)
	{
		AfxMessageBox(_T("该文件没有读取到有效光谱数据！"));
		return FALSE;
	}

	double* inData = new double[2 * pointCount];
	for (int i = 0; i < pointCount; i++)
	{
		inData[i] = vecX[i];                     // 前半部分放X
		inData[i + pointCount] = vecY[i];        // 后半部分放Y
	}

	// 3) 准备输出数组 y_test[1650] 和 flag
	double outData[1650] = { 0.0 };  // 函数要求固定 1650
	double flag = 0.0;               // 用于返回成功/失败

									 // 4) 调用 dll_Resolution
									 //    numDimensions = 2 * pointCount
	dll_Resolution(inData, 2 * pointCount, outData, &flag);

	// 5) 检查 flag
	if (flag != 0.0)
	{
		AfxMessageBox(_T("dll_Resolution 预处理失败，请检查数据！"));
		delete[] inData;
		return FALSE;
	}


	// 6) 拼接 outData => strProcessedY (以逗号隔开)
	//    由于 outData 长度固定 1650，这里将全部 1650 点拼起来
	CString strProcessedY;
	for (int i = 0; i < 1650; i++)
	{
		CString tmp;
		tmp.Format(_T("%.6f"), outData[i]);
		strProcessedY += tmp;
		if (i != 1649)
		{
			strProcessedY += _T(",");
		}
	}

	if (!InsertPureDataToDB(fileName, strX, strY, strProcessedY))
	{
		return FALSE;
	}

	//// 提示或刷新 UI
	//CString msg;
	//msg.Format(_T("成功将文件 [%s] 导入到 pureTable！\r\n名称:%s\r\n行数:%u"),
	//	justName, pureName, (UINT)vecX.size());
	//AfxMessageBox(msg);

	return TRUE;

}
BOOL CDialogQualityAnalyse::InsertPureDataToDB(const CString& pureName,const CString& strX,const CString& strY,const CString& strProcessedY)
{
	// 将每个 CString 转成 UTF-8
	std::string utf8Name = AnsiToUtf8(CT2A(pureName, CP_ACP));
	std::string utf8X = AnsiToUtf8(CT2A(strX, CP_ACP));
	std::string utf8Y = AnsiToUtf8(CT2A(strY, CP_ACP));
	std::string utf8Proc = AnsiToUtf8(CT2A(strProcessedY, CP_ACP));

	// 拼接 SQL 语句
	CStringA sqlUTF8;
	sqlUTF8.Format(
		"INSERT INTO pureTable (name, original_data_x, original_data_y, processed_data_y) "
		"VALUES ('%s','%s','%s','%s')",
		utf8Name.c_str(),
		utf8X.c_str(),
		utf8Y.c_str(),
		utf8Proc.c_str()
	);

	// 执行 SQL 语句
	int rc = sqlite3_exec(m_db, sqlUTF8, NULL, NULL, &m_errMsg);
	if (rc != SQLITE_OK)
	{
		CString msg;
		msg.Format(_T("插入数据到 pureTable 出错: %hs"), m_errMsg);
		AfxMessageBox(msg);
		sqlite3_free(m_errMsg);
		m_errMsg = nullptr;
		return FALSE;
	}

	return TRUE;
}
void CDialogQualityAnalyse::OnBnClickedBtnSelPure()
{
	// 1.获取选中项
	std::vector<int> selectedIndices;
	POSITION pos = m_list_pure.GetFirstSelectedItemPosition(); // 获取第一个选中项的位置
	while (pos) {
		int index = m_list_pure.GetNextSelectedItem(pos); // 获取选中项的索引
		selectedIndices.push_back(index);
	}
	if (selectedIndices.empty()) {
		AfxMessageBox(_T("请先选择要保留的项！"));
		return;
	}
	// 2.临时保存选中项的数据
	std::vector<std::pair<CString, CString>> selectedData;
	for (int i : selectedIndices) {
		CString id = m_list_pure.GetItemText(i, 0); // 获取第0列的值
		CString name = m_list_pure.GetItemText(i, 1); // 获取第1列的值
		selectedData.emplace_back(id, name); // 保存为键值对
	}

	m_list_pure.DeleteAllItems();

	// 3.将选中项重新插入到列表中
	for (size_t i = 0; i < selectedData.size(); ++i) {
		int rowIndex = m_list_pure.InsertItem(i, selectedData[i].first); // 插入序号
		m_list_pure.SetItemText(rowIndex, 1, selectedData[i].second);   // 设置名称
	}
}
BOOL CDialogQualityAnalyse::ProcessFile(const CString& filePath, vector<double>& vecX, vector<double>& vecY,CString &fileName)
{
	vecX.clear();
	vecY.clear();

	//1.获取文件名(不含拓展名)
	CString fullName = filePath;
	int pos = fullName.ReverseFind(_T('\\'));
	CString justName = fullName.Mid(pos + 1);  // "乙腈-200-2.txt"
	int dotPos = justName.ReverseFind(_T('.'));
	fileName = (dotPos > 0) ? justName.Left(dotPos) : justName;  // "乙腈-200-2"

	// 2.判断文件类型
	 dotPos = filePath.ReverseFind(_T('.'));
	if (dotPos < 0)
	{
		AfxMessageBox(_T("文件类型无法识别！"));
		return FALSE;
	}

	CString fileExtension = filePath.Mid(dotPos + 1).MakeLower();

	//3.处理csv文件
	if (fileExtension == _T("csv"))  //处理csv文件
	{
		
		CStdioFile file;
		if (!file.Open(filePath, CFile::modeRead | CFile::typeText))
		{
			AfxMessageBox(_T("无法打开 CSV 文件！"));
			return FALSE;
		}

		CString strLine;
		while (file.ReadString(strLine))
		{
			strLine.Trim();
			if (strLine.IsEmpty())
				continue;

			// 假设 CSV 使用逗号分隔
			int commaPos = strLine.Find(_T(','));
			if (commaPos < 0)
				continue;

			CString xPart = strLine.Left(commaPos).Trim();
			CString yPart = strLine.Mid(commaPos + 1).Trim();

			vecX.push_back(_tstof(xPart));
			vecY.push_back(_tstof(yPart));
		}

		file.Close();
	}

	//4.处理txt文件
	else if (fileExtension == _T("txt"))  //处理txt文件
	{
		CStdioFile file;
		if (!file.Open(filePath, CFile::modeRead | CFile::typeText))
		{
			AfxMessageBox(_T("无法打开 TXT 文件！"));
			return FALSE;
		}

		CString strLine;
		while (file.ReadString(strLine))
		{
			strLine.Trim();
			if (strLine.IsEmpty())
				continue; // 跳过空行

			// 查找第一个空格或制表符的位置
			int delimiterPos = strLine.FindOneOf(_T(" \t"));
			if (delimiterPos < 0)
				continue; // 没有找到分隔符，跳过行

			CString xPart = strLine.Left(delimiterPos).Trim();
			CString yPart = strLine.Mid(delimiterPos + 1).Trim();

			// 处理可能存在的多个空格或制表符
			yPart.TrimLeft(_T(" \t"));

			vecX.push_back(_tstof(xPart));
			vecY.push_back(_tstof(yPart));
		}

		file.Close();
	}
}
void CDialogQualityAnalyse::OnBnClickedBtnPureDel()
{
	// 1. 获取选中项
	std::vector<int> selectedDbIDs;
	POSITION pos = m_list_pure.GetFirstSelectedItemPosition();
	while (pos)
	{
		int listIndex = m_list_pure.GetNextSelectedItem(pos);
		// 从 ItemData 中取出数据库 id
		int dbID = (int)m_list_pure.GetItemData(listIndex);
		selectedDbIDs.push_back(dbID);
	}

	if (selectedDbIDs.empty())
	{
		AfxMessageBox(_T("请先选择要删除的项！"));
		return;
	}

	// 2. 从数据库中删除选中项
	for (int dbID : selectedDbIDs)
	{
		CString sql;
		sql.Format(_T("DELETE FROM pureTable WHERE id = %d"), dbID);
		int rc = sqlite3_exec(m_db, CT2A(sql), NULL, NULL, &m_errMsg);
		if (rc != SQLITE_OK)
		{
			CString msg;
			msg.Format(_T("删除 pureTable 数据出错：%hs"), m_errMsg);
			AfxMessageBox(msg);
			sqlite3_free(m_errMsg);
			m_errMsg = nullptr;
			return;
		}
	}

	// 3. 从列表中移除选中项 (再次获取选中项, 也可简单地重新加载列表)
	pos = m_list_pure.GetFirstSelectedItemPosition();
	while (pos)
	{
		int listIndex = m_list_pure.GetNextSelectedItem(pos);
		m_list_pure.DeleteItem(listIndex);
		pos = m_list_pure.GetFirstSelectedItemPosition();
	}

	AfxMessageBox(_T("选中项已从数据库删除！"));
}


//点击添加混合物按钮,从文件读取混合物数据,添加到混合物到数据库
void CDialogQualityAnalyse::OnBnClickedButtonAddMixData()
{
	CFileDialog fileDlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		_T("CSV Files (*.csv)|*.csv|TXT Files (*.txt)|*.txt||"));
	if (fileDlg.DoModal() == IDOK) {
		CString filePath = fileDlg.GetPathName(); // 获取文件路径
		if (ImportMixFromFile(filePath)) {
			//AfxMessageBox(_T("数据导入成功！"));
		}
		else {
			AfxMessageBox(_T("数据导入失败，请检查文件内容！"));
		}
	}
	if (!LoadMixTableToList())
	{
		AfxMessageBox(_T("展示到混合物列表失败！"));
	}
}
BOOL CDialogQualityAnalyse::ImportMixFromFile(const CString& filePath) {

	vector<double> vecX, vecY;
	CString fileName;
	if (!ProcessFile(filePath, vecX, vecY, fileName))
	{
		AfxMessageBox(_T("processFile执行失败！"));
		return false;
	}

	// 4) 拼接 original_data_x, original_data_y 成字符串，用逗号隔开
	//    也可以用其它分隔符
	CString strX, strY;
	for (size_t i = 0; i < vecX.size(); i++)
	{
		CString tempX, tempY;
		tempX.Format(_T("%.6f"), vecX[i]); // 保留小数位
		tempY.Format(_T("%.6f"), vecY[i]);

		strX += tempX;
		strY += tempY;
		if (i != vecX.size() - 1)
		{
			strX += _T(",");
			strY += _T(",");
		}
	}

	int pointCount = (int)vecX.size();
	if (pointCount <= 0)
	{
		AfxMessageBox(_T("该文件没有读取到有效光谱数据！"));
		return FALSE;
	}

	// 2) 组装传给 dll_Resolution 的输入 data：先X后Y
	//    numDimensions = 2 * pointCount
	double* inData = new double[2 * pointCount];
	for (int i = 0; i < pointCount; i++)
	{
		inData[i] = vecX[i];                     // 前半部分放X
		inData[i + pointCount] = vecY[i];        // 后半部分放Y
	}

	// 3) 准备输出数组 y_test[1650] 和 flag
	double outData[1650] = { 0.0 };  // 函数要求固定 1650
	double flag = 0.0;               // 用于返回成功/失败

									 // 4) 调用 dll_Resolution
									 //    numDimensions = 2 * pointCount
	dll_Resolution(inData, 2 * pointCount, outData, &flag);

	// 5) 检查 flag
	if (flag != 0.0)
	{
		AfxMessageBox(_T("dll_Resolution 预处理失败，请检查数据！"));
		delete[] inData;
		return FALSE;
	}




	// 6) 拼接 outData => strProcessedY (以逗号隔开)
	//    由于 outData 长度固定 1650，这里将全部 1650 点拼起来
	CString strProcessedY;
	for (int i = 0; i < 1650; i++)
	{
		CString tmp;
		tmp.Format(_T("%.6f"), outData[i]);
		strProcessedY += tmp;
		if (i != 1649)
		{
			strProcessedY += _T(",");
		}
	}


	//CString debugInfo;
	//debugInfo.Format(
	//	_T("即将插入的数据：\n")
	//	
	//	_T("processed_data_y: %s\n"),
	//
	//	strProcessedY // 如果有
	//);

	//// 弹窗看看
	//AfxMessageBox(debugInfo);


	if (!InsertMixDataToDB(fileName, strX, strY, strProcessedY))
	{
		return FALSE;
	}

	//// 提示或刷新 UI
	//CString msg;
	//msg.Format(_T("成功将文件 [%s] 导入到 MixTable！\r\n名称:%s\r\n行数:%u"),
	//	fileName, fileName, (UINT)vecX.size());
	//AfxMessageBox(msg);

	return TRUE;
}
BOOL CDialogQualityAnalyse::InsertMixDataToDB(const CString& pureName, const CString& strX, const CString& strY, const CString& strProcessedY)
{
	// 将每个 CString 转成 UTF-8
	std::string utf8Name = AnsiToUtf8(CT2A(pureName, CP_ACP));
	std::string utf8X = AnsiToUtf8(CT2A(strX, CP_ACP));
	std::string utf8Y = AnsiToUtf8(CT2A(strY, CP_ACP));
	std::string utf8Proc = AnsiToUtf8(CT2A(strProcessedY, CP_ACP));

	// 拼接 SQL 语句
	CStringA sqlUTF8;
	sqlUTF8.Format(
		"INSERT INTO mixTable (name, original_data_x, original_data_y, processed_data_y) "
		"VALUES ('%s','%s','%s','%s')",
		utf8Name.c_str(),
		utf8X.c_str(),
		utf8Y.c_str(),
		utf8Proc.c_str()
	);

	// 执行 SQL 语句
	int rc = sqlite3_exec(m_db, sqlUTF8, NULL, NULL, &m_errMsg);
	if (rc != SQLITE_OK)
	{
		CString msg;
		msg.Format(_T("插入数据到 mixTable 出错: %hs"), m_errMsg);
		AfxMessageBox(msg);
		sqlite3_free(m_errMsg);
		m_errMsg = nullptr;
		return FALSE;
	}

	return TRUE;
}
void CDialogQualityAnalyse::OnBnClickedBtnSelMix()
{
	// 获取选中项
	std::vector<int> selectedIndices;
	POSITION pos = m_list_mix.GetFirstSelectedItemPosition(); // 获取第一个选中项的位置
	while (pos) {
		int index = m_list_mix.GetNextSelectedItem(pos); // 获取选中项的索引
		selectedIndices.push_back(index);
	}
	// 如果没有选择项，提示用户
	if (selectedIndices.empty()) {
		AfxMessageBox(_T("请先选择要保留的项！"));
		return;
	}
	// 临时保存选中项的数据
	std::vector<std::pair<CString, CString>> selectedData;
	for (int i : selectedIndices) {
		CString id = m_list_mix.GetItemText(i, 0); // 获取第0列的值
		CString name = m_list_mix.GetItemText(i, 1); // 获取第1列的值
		selectedData.emplace_back(id, name); // 保存为键值对
	}

	m_list_mix.DeleteAllItems();
	// 将选中项重新插入到列表中
	for (size_t i = 0; i < selectedData.size(); ++i) {
		int rowIndex = m_list_mix.InsertItem(i, selectedData[i].first); // 插入序号
		m_list_mix.SetItemText(rowIndex, 1, selectedData[i].second);   // 设置名称
	}

}
void CDialogQualityAnalyse::OnBnClickedButtonMixDel()
{// 1. 获取选中项
	std::vector<int> selectedDbIDs;
	POSITION pos = m_list_mix.GetFirstSelectedItemPosition();
	while (pos)
	{
		int listIndex = m_list_mix.GetNextSelectedItem(pos);
		// 从 ItemData 中取出数据库 id
		int dbID = (int)m_list_mix.GetItemData(listIndex);
		selectedDbIDs.push_back(dbID);
	}

	if (selectedDbIDs.empty())
	{
		AfxMessageBox(_T("请先选择要删除的项！"));
		return;
	}

	// 2. 从数据库中删除选中项
	for (int dbID : selectedDbIDs)
	{
		CString sql;
		sql.Format(_T("DELETE FROM mixTable WHERE id = %d"), dbID);
		int rc = sqlite3_exec(m_db, CT2A(sql), NULL, NULL, &m_errMsg);
		if (rc != SQLITE_OK)
		{
			CString msg;
			msg.Format(_T("删除 pureTable 数据出错：%hs"), m_errMsg);
			AfxMessageBox(msg);
			sqlite3_free(m_errMsg);
			m_errMsg = nullptr;
			return;
		}
	}

	// 3. 从列表中移除选中项 (再次获取选中项, 也可简单地重新加载列表)
	pos = m_list_mix.GetFirstSelectedItemPosition();
	while (pos)
	{
		int listIndex = m_list_mix.GetNextSelectedItem(pos);
		m_list_mix.DeleteItem(listIndex);
		pos = m_list_mix.GetFirstSelectedItemPosition();
	}

	AfxMessageBox(_T("选中的数据已成功从数据库删除！"));

}


//导入混合物比例
void CDialogQualityAnalyse::OnBnClickedButtonImportRatio()
{
	// 弹文件对话框让用户选择 ratio.txt 或 CSV
	CFileDialog fileDlg(TRUE, NULL, NULL,
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		_T("TXT Files (*.txt)|*.txt|CSV Files (*.csv)|*.csv||"));
	if (fileDlg.DoModal() == IDOK)
	{
		CString filePath = fileDlg.GetPathName();

		int pureCount = m_list_pure.GetItemCount();
		int mixCount = m_list_mix.GetItemCount();
		
		if (ImportRatioFromFile(filePath, pureCount, mixCount))
		{
			//AfxMessageBox(_T("比例数据导入成功！"));
		}
		else
		{
			AfxMessageBox(_T("导入比例数据失败，请检查文件格式！"));
		}
	}
}
BOOL CDialogQualityAnalyse::SetRatioListColumns(const std::vector<CString>& columnNames) {
	// 清除现有列
	while (m_list_ratio.GetHeaderCtrl()->GetItemCount() > 0)
	{
		m_list_ratio.DeleteColumn(0);
	}

	// 获取列表控件的宽度
	CRect rect;
	m_list_ratio.GetClientRect(&rect); // 使用 GetClientRect 填充 rect
	int totalWidth = rect.Width();

	// 动态计算列宽
	int columnWidth = totalWidth / columnNames.size();

	// 添加列
	for (size_t i = 0; i < columnNames.size(); ++i)
	{
		m_list_ratio.InsertColumn((int)i, columnNames[i], LVCFMT_CENTER, columnWidth);
	}

	return TRUE;
}
BOOL CDialogQualityAnalyse::ImportRatioFromFile(const CString& ratioFilePath, int pureCount, int mixCount)
{
	// 打开文件
	CStdioFile file;
	if (!file.Open(ratioFilePath, CFile::modeRead | CFile::typeText))
	{
		AfxMessageBox(_T("无法打开所选的比例文件！"));
		return FALSE;
	}

	m_vecRatio.clear();
	std::vector<std::vector<double>> tempRatioData; // 临时存储比例数据
	
	// 3) 逐行读取 CSV
	CString strLine;
	int currentRow = 0;
	while (file.ReadString(strLine))
	{
		strLine.Trim();
		if (strLine.IsEmpty())
			continue; // 跳过空行

	
		std::vector<CString> columns;

		int curPos = 0;
		CString token = strLine.Tokenize(_T(","), curPos);
		while (!token.IsEmpty())
		{
			token.Trim();
			columns.push_back(token);
			token = strLine.Tokenize(_T(","), curPos);
		}


		// 检查列数是否与纯净物数量一致
		if ((int)columns.size() != pureCount)
		{
			CString msg;
			msg.Format(_T("请确保比例文件列数与纯净物数量(%d)保持一致"),  pureCount);
			AfxMessageBox(msg);
			file.Close();
			return FALSE;
		}

		// 获取纯净物列表的列名
		std::vector<CString> pureNames;
		for (int i = 0; i < pureCount; ++i)
		{
			CString name = m_list_pure.GetItemText(i, 1); // 获取第1列名称
			pureNames.push_back(name);
		}

		// 动态设置比例列表的列名
		if (!SetRatioListColumns(pureNames))
		{
			AfxMessageBox(_T("设置比例列表列名失败！"));
			return false;
		}

		// 转换为 double 并存储
		std::vector<double> ratioRow;
		for (int i = 0; i < pureCount; ++i)
		{
			double val = _tstof(columns[i]);
			ratioRow.push_back(val);
		}
		tempRatioData.push_back(ratioRow);
		currentRow++;
	}

	file.Close();

	// 检查行数是否与混合物数量一致
	if (currentRow != mixCount)
	{
		CString msg;
		msg.Format(_T("请确保比例文件的行数(%d)与混合物数量(%d)一致"), currentRow, mixCount);
		AfxMessageBox(msg);
		return FALSE;
	}

	// 将数据存入 m_vecRatio（扁平化存储）
	for (const auto& row : tempRatioData)
	{
		for (double val : row)
		{
			m_vecRatio.push_back(val);
		}
	}

	// 将数据展示到 m_list_ratio
	m_list_ratio.DeleteAllItems();
	for (int i = 0; i < mixCount; ++i)
	{
		CString ratioStr;
		for (int j = 0; j < pureCount; ++j)
		{
			CString temp;
			temp.Format(_T("%.2f"), tempRatioData[i][j]);
			ratioStr += temp;
			if (j != pureCount - 1)
				ratioStr += _T(", ");
		}

		// 插入行
		int rowIndex = m_list_ratio.InsertItem(i, _T("")); // 插入一个空行
		for (int j = 0; j < pureCount; ++j)
		{
			CString temp;
			temp.Format(_T("%.2f"), tempRatioData[i][j]);
			m_list_ratio.SetItemText(rowIndex, j, temp);
		}
	}

	//// 显示 m_vecRatio 的内容
	//CString resultMsg;
	//resultMsg.Format(_T("成功导入 %d 行，m_vecRatio 内容如下：\n"), (int)m_vecRatio.size() / 3);
	//for (size_t i = 0; i < m_vecRatio.size(); i++)
	//{
	//	CString temp;
	//	temp.Format(_T("%.3f"), m_vecRatio[i]);
	//	resultMsg += temp + _T(", ");
	//	if ((i + 1) % 3 == 0) // 每行 3 个值
	//		resultMsg += _T("\n");
	//}
	//AfxMessageBox(resultMsg);

	return TRUE;
	
}


//PLS模型建立
BOOL CDialogQualityAnalyse::SplitStringToDoubleArray(const CString& sData, std::vector<double>& outArr, LPCTSTR delim)
{
	outArr.clear();
	int curPos = 0;
	CString token = sData.Tokenize(delim, curPos);
	while (!token.IsEmpty())
	{
		token.Trim();
		double val = _tstof(token);
		outArr.push_back(val);
		token = sData.Tokenize(delim, curPos);
	}
	return TRUE; // 简化
}
BOOL CDialogQualityAnalyse::BuildPureMatrix(double* pureMatrix, int pureCount)
{
	for (int i = 0; i < pureCount; i++)
	{
		// 构建 SQL 查询
		CString sql;
		sql.Format(_T("SELECT processed_data_y FROM pureTable LIMIT 1 OFFSET %d"), i);

		char** dbResult = nullptr;
		int row = 0, col = 0;
		int rc = sqlite3_get_table(m_db, CT2A(sql), &dbResult, &row, &col, &m_errMsg);
		if (rc != SQLITE_OK || row < 1)
		{
			AfxMessageBox(_T("无法获取 pureTable 中的记录，或记录不足！"));
			if (dbResult) sqlite3_free_table(dbResult);
			return FALSE;
		}

		// 获取 processed_data_y
		//CStringA sDataUTF8 = dbResult[col];
		//sqlite3_free_table(dbResult);

		// 转换为 CString 并拆分为 1650 个 double
		CString sDataW = Utf8ToCString(dbResult[col]);
		sqlite3_free_table(dbResult);
		std::vector<double> vecTemp;
		if (!SplitStringToDoubleArray(sDataW, vecTemp, _T(",")))
		{
			AfxMessageBox(_T("拆分 pureTable 的 processed_data_y 失败！"));
			return FALSE;
		}
		if ((int)vecTemp.size() < 1650)
		{
			AfxMessageBox(_T("pureTable 的数据点数不足 1650！"));
			return FALSE;
		}

		// 填充 pureMatrix
		for (int rowIndex = 0; rowIndex < 1650; rowIndex++)
		{
			pureMatrix[i * 1650 + rowIndex] = vecTemp[rowIndex];
		}
	}
	return TRUE;
}
BOOL CDialogQualityAnalyse::BuildMixMatrix(double* mixMatrix, int mixCount)
{
	for (int j = 0; j < mixCount; j++)
	{
		// 构建 SQL 查询
		CString sql;
		sql.Format(_T("SELECT processed_data_y FROM mixTable LIMIT 1 OFFSET %d"), j);

		char** dbResult = nullptr;
		int row = 0, col = 0;
		int rc = sqlite3_get_table(m_db, CT2A(sql), &dbResult, &row, &col, &m_errMsg);
		if (rc != SQLITE_OK || row < 1)
		{
			AfxMessageBox(_T("无法获取 mixTable 中的记录，或记录不足！"));
			if (dbResult) sqlite3_free_table(dbResult);
			return FALSE;
		}

		// 获取 processed_data_y
		//CStringA sDataUTF8 = dbResult[col];
		//sqlite3_free_table(dbResult);

		// 转换为 CString 并拆分为 1650 个 double
		CString sDataW = Utf8ToCString(dbResult[col]);
		sqlite3_free_table(dbResult);
		std::vector<double> vecTemp;
		if (!SplitStringToDoubleArray(sDataW, vecTemp, _T(",")))
		{
			AfxMessageBox(_T("拆分 mixTable 的 processed_data_y 失败！"));
			return FALSE;
		}
		if ((int)vecTemp.size() < 1650)
		{
			AfxMessageBox(_T("mixTable 的数据点数不足 1650！"));
			return FALSE;
		}

		// 填充 mixMatrix
		for (int rowIndex = 0; rowIndex < 1650; rowIndex++)
		{
			mixMatrix[j * 1650 + rowIndex] = vecTemp[rowIndex];
		}
	}
	return TRUE;
}
BOOL CDialogQualityAnalyse::BuildRatioMatrix(double* ratioMatrix, int pureCount, int mixCount)
{
	if ((int)m_vecRatio.size() < pureCount * mixCount)
	{
		AfxMessageBox(_T("m_vecRatio 数据行数不足(需要 ") +
			CString(std::to_wstring(pureCount * mixCount).c_str()) + _T(" 个数)。请先导入 ratio！"));
		return FALSE;
	}

	for (int r = 0; r < mixCount; r++)
	{
		for (int c = 0; c < pureCount; c++)
		{
			ratioMatrix[r * pureCount + c] = m_vecRatio[r * pureCount + c];
		}
	}

	return TRUE;
}
void CDialogQualityAnalyse::OnBnClickedButtonBuildModel() {
		UpdateData(TRUE); 
		CString modelName = m_strModelName.Trim(); 
		int pureCount = m_list_pure.GetItemCount();
		int mixCount = m_list_mix.GetItemCount();  // 去除首尾空格
		if (modelName.IsEmpty())
		{
			AfxMessageBox(_T("请输入模型名称！"));
			return;
		}

		// 动态分配矩阵内存
		double* pureMatrix = new (std::nothrow) double[1650 * pureCount];
		double* mixMatrix = new (std::nothrow) double[1650 * mixCount];
		double* ratioMatrix = new (std::nothrow) double[mixCount * pureCount];

		if (!BuildPureMatrix(pureMatrix, pureCount))
		{
			delete[] pureMatrix;
			delete[] mixMatrix;
			delete[] ratioMatrix;
			return;
		}

		// 构建 mixMatrix
		if (!BuildMixMatrix(mixMatrix, mixCount))
		{
			delete[] pureMatrix;
			delete[] mixMatrix;
			delete[] ratioMatrix;
			return;
		}

		// 构建 ratioMatrix
		if (!BuildRatioMatrix(ratioMatrix, pureCount, mixCount))
		{
			delete[] pureMatrix;
			delete[] mixMatrix;
			delete[] ratioMatrix;
			return;
		}

		// 调用 PLS 模型构建函数
		double outFlag = 0.0;
		double outThetaDim = 0.0;
		double* pls_model_theta = pls_theta(
			pureMatrix,      // 1650×pureCount
			pureCount,       // pureNum
			mixMatrix,       // 1650×mixCount
			mixCount,        // mixtureNum
			ratioMatrix,     // mixCount×pureCount
			&outThetaDim,    // theta维度
			&outFlag         // 成功/失败标记
		);


		// 清理分配的内存--矩阵
		delete[] pureMatrix;
		delete[] mixMatrix;
		delete[] ratioMatrix;

		// 拼接 theta 成字符串
		CString thetaStr;
		for (int i = 0; i < (int)outThetaDim; i++)
		{
			CString tmp;
			tmp.Format(_T("%.6f"), pls_model_theta[i]);
			thetaStr += tmp;
			if (i != (int)outThetaDim - 1)
			{
				thetaStr += _T(",");
			}
		}
		// 获取 pureName 列表，使用双逗号分隔
		CString pureNamesCombined;
		for (int i = 0; i < pureCount; ++i)
		{
			CString pureName = m_list_pure.GetItemText(i, 1); // 获取名称列
			pureNamesCombined += pureName;
			if (i != pureCount - 1)
			{
				pureNamesCombined += _T(",,"); // 双逗号分隔
			}
		}

		// 插入到 pls_model 表
		if (InsertPlsModelToDB(modelName, thetaStr, pureCount, mixCount, (int)outThetaDim, pureNamesCombined))
		{
			AfxMessageBox(_T("PLS模型建立成功！"));
		}

		// 清理 pls_model_theta 内存
		if (pls_model_theta)
			delete[] pls_model_theta;


}
void CDialogQualityAnalyse::OnBnClickedBtnBuildNetmodel()
{
	UpdateData(TRUE);
	CString modelName = m_strModelName.Trim();

	int pureCount = m_list_pure.GetItemCount();
	int mixCount = m_list_mix.GetItemCount();

	// 验证输入
	if (modelName.IsEmpty())
	{
		AfxMessageBox(_T("请输入模型名称！"));
		return;
	}

	// 动态分配矩阵内存
	double* pureMatrix = new (std::nothrow) double[1650 * pureCount];
	double* mixMatrix = new (std::nothrow) double[1650 * mixCount];
	double* ratioMatrix = new (std::nothrow) double[mixCount * pureCount];

	if (!pureMatrix || !mixMatrix || !ratioMatrix)
	{
		AfxMessageBox(_T("内存分配失败！"));
		delete[] pureMatrix;
		delete[] mixMatrix;
		delete[] ratioMatrix;
		return;
	}

	// 构建 pureMatrix
	if (!BuildPureMatrix(pureMatrix, pureCount))
	{
		delete[] pureMatrix;
		delete[] mixMatrix;
		delete[] ratioMatrix;
		return;
	}

	// 构建 mixMatrix
	if (!BuildMixMatrix(mixMatrix, mixCount))
	{
		delete[] pureMatrix;
		delete[] mixMatrix;
		delete[] ratioMatrix;
		return;
	}

	// 构建 ratioMatrix
	if (!BuildRatioMatrix(ratioMatrix, pureCount, mixCount))
	{
		delete[] pureMatrix;
		delete[] mixMatrix;
		delete[] ratioMatrix;
		return;
	}

	// 调用神经网络模型构建函数 lsrp_theta
	double outFlag = 0.0;
	double* lsrp_model_theta = lsrp_theta(
		pureMatrix,      // 1650 × pureCount
		pureCount,       // pureNum
		mixMatrix,       // 1650 × mixCount
		mixCount,        // mixtureNum
		ratioMatrix,     // actual_ratio (mixCount × pureCount)
		&outFlag         // flag
	);

	// 清理矩阵内存
	delete[] pureMatrix;
	delete[] mixMatrix;
	delete[] ratioMatrix;

	if (outFlag != 0.0)
	{
		AfxMessageBox(_T("lsrp_theta 建模失败，请检查数据或算法！"));
		if (lsrp_model_theta) delete[] lsrp_model_theta;
		return;
	}

	// 拼接 theta 成字符串
	CString thetaStr;
	
	int thetaDim = 100; // 根据实际情况调整
	for (int i = 0; i < thetaDim; i++)
	{
		CString tmp;
		tmp.Format(_T("%.6f"), lsrp_model_theta[i]);
		thetaStr += tmp;
		if (i != thetaDim - 1)
		{
			thetaStr += _T(",");
		}
	}

	// 获取 pureName 列表，使用双逗号分隔
	CString pureNamesCombined;
	for (int i = 0; i < pureCount; ++i)
	{
		CString pureName = m_list_pure.GetItemText(i, 1); // 获取名称列
		pureNamesCombined += pureName;
		if (i != pureCount - 1)
		{
			pureNamesCombined += _T(",,"); // 双逗号分隔
		}
	}

	// 插入到 lsrp_model 表
	{
		// 将 CString 转成 UTF-8
		std::string utf8Name = AnsiToUtf8(CT2A(modelName, CP_ACP));
		std::string utf8Theta = AnsiToUtf8(CT2A(thetaStr, CP_ACP));
		std::string utf8PureName = AnsiToUtf8(CT2A(pureNamesCombined, CP_ACP));

		// 拼接 SQL 语句
		CStringA sqlUTF8;
		sqlUTF8.Format(
			"INSERT INTO lsrp_model (name, lsrp_theta, pureNum, mixtureNum, thetaDim, pureName) "
			"VALUES ('%s','%s',%d,%d,%d,'%s')",
			utf8Name.c_str(),
			utf8Theta.c_str(),
			pureCount,    // pureNum
			mixCount,     // mixtureNum
			thetaDim,
			utf8PureName.c_str()
		);

		// 执行 SQL 语句
		int rc = sqlite3_exec(m_db, sqlUTF8, NULL, NULL, &m_errMsg);
		if (rc != SQLITE_OK)
		{
			CString msg;
			msg.Format(_T("插入 lsrp_model 失败: %hs"), m_errMsg);
			AfxMessageBox(msg);
			sqlite3_free(m_errMsg);
			m_errMsg = nullptr;
		}
		else
		{
			AfxMessageBox(_T("LPLS模型建立成功！"));
		}
	}

	// 清理神经网络模型参数内存
	if (lsrp_model_theta)
		delete[] lsrp_model_theta;

}
BOOL CDialogQualityAnalyse::InsertPlsModelToDB(const CString& modelName, const CString& strPlsTheta, int pureNum, int mixNum, int thetaDim, const CString& pureName)
{
	// 将 CString 转成 UTF-8
	std::string utf8Name = AnsiToUtf8(CT2A(modelName, CP_ACP));
	std::string utf8Theta = AnsiToUtf8(CT2A(strPlsTheta, CP_ACP));
	std::string utf8PureName = AnsiToUtf8(CT2A(pureName, CP_ACP));

	// 拼接 SQL 语句
	CStringA sqlUTF8;
	sqlUTF8.Format(
		"INSERT INTO pls_model (name, pls_theta, pureNum, mixtureNum, thetaDim, pureName) "
		"VALUES ('%s','%s',%d,%d,%d,'%s')",
		utf8Name.c_str(),
		utf8Theta.c_str(),
		pureNum,
		mixNum,
		thetaDim,
		utf8PureName.c_str()
	);

	// 执行 SQL 语句
	int rc = sqlite3_exec(m_db, sqlUTF8, NULL, NULL, &m_errMsg);
	if (rc != SQLITE_OK)
	{
		CString msg;
		msg.Format(_T("插入 pls_model 失败: %hs"), m_errMsg);
		AfxMessageBox(msg);
		sqlite3_free(m_errMsg);
		m_errMsg = nullptr;
		return FALSE;
	}

	return TRUE;
}


//选择模型
void CDialogQualityAnalyse::OnBnClickedRadioPls()
{
	// 当选择PLS时，加载PLS模型
	m_bCurrentModelIsPLS = true;
	LoadModelsToList(true);
}
void CDialogQualityAnalyse::OnBnClickedRadioLpls()
{
	//当选择LPLS时，加载LPLS模型
	m_bCurrentModelIsPLS = false;
	LoadModelsToList(false);
}
void CDialogQualityAnalyse::LoadModelsToList(bool bPLS)
{
	m_list_models.DeleteAllItems();

	// 确定要查询的表
	CString tableName = bPLS ? _T("pls_model") : _T("lsrp_model");

	// 构建SQL查询语句
	CString sql;
	sql.Format(_T("SELECT id, name FROM %s"), tableName);

	char** dbResult = nullptr;
	int nRows = 0, nCols = 0;
	int rc = sqlite3_get_table(m_db, CT2A(sql), &dbResult, &nRows, &nCols, &m_errMsg);
	if (rc != SQLITE_OK) {
		CString errorMsg;
		errorMsg.Format(_T("查询 %s 表数据失败：%hs"), tableName, m_errMsg);
		AfxMessageBox(errorMsg);
		sqlite3_free(m_errMsg);
		sqlite3_free_table(dbResult);
		return;
	}

	// 遍历结果集，从第1行开始，第0行是列名
	for (int i = 1; i <= nRows; ++i) {
		CString id = Utf8ToCString(dbResult[i * nCols]);
		CString name = Utf8ToCString(dbResult[i * nCols + 1]);

		CString displayIndex;
		displayIndex.Format(_T("%d"), i);

		// 插入列表行
		int rowIndex = m_list_models.InsertItem(i - 1, displayIndex);
		m_list_models.SetItemText(rowIndex, 1, name);       // 设置模型名称
	}

	// 释放内存
	sqlite3_free_table(dbResult);
}
void CDialogQualityAnalyse::OnBnClickedBtnSelModel()
{
	// 获取选中项
	std::vector<int> selectedIndices;
	POSITION pos = m_list_models.GetFirstSelectedItemPosition(); // 获取第一个选中项的位置
	while (pos) {
		int index = m_list_models.GetNextSelectedItem(pos); // 获取选中项的索引
		selectedIndices.push_back(index);
	}
	// 如果没有选择项，提示用户
	if (selectedIndices.empty()) {
		AfxMessageBox(_T("请先选择要保留的一项！"));
		return;
	}
	// 临时保存选中项的数据
	std::vector<std::pair<CString, CString>> selectedData;
	for (int i : selectedIndices) {
		CString id = m_list_models.GetItemText(i, 0); // 获取第0列的值
		CString name = m_list_models.GetItemText(i, 1); // 获取第1列的值
		selectedData.emplace_back(id, name); // 保存为键值对
	}

	m_list_models.DeleteAllItems();

	// 将选中项重新插入到列表中
	for (size_t i = 0; i < selectedData.size(); ++i) {
		int rowIndex = m_list_models.InsertItem(i, selectedData[i].first); // 插入序号
		m_list_models.SetItemText(rowIndex, 1, selectedData[i].second);   // 设置名称
	}


}
void CDialogQualityAnalyse::OnBnClickedBtnDelModel()
{
	std::vector<CString> selectedNames;
	POSITION pos = m_list_models.GetFirstSelectedItemPosition();
	while (pos)
	{
		int listIndex = m_list_models.GetNextSelectedItem(pos);
		CString modelName = m_list_models.GetItemText(listIndex, 1); 
		selectedNames.push_back(modelName);
	}

	if (selectedNames.empty())
	{
		AfxMessageBox(_T("请先选择要删除的模型！"));
		return;
	}

	CString tableName = m_bCurrentModelIsPLS ? _T("pls_model") : _T("lsrp_model");

	for (const auto& cstrName : selectedNames)
	{
		std::string utf8Name = AnsiToUtf8(CT2A(cstrName, CP_ACP));

		CStringA sqlUTF8;
		sqlUTF8.Format("DELETE FROM %s WHERE name = ?", CT2A(tableName, CP_ACP));

		sqlite3_stmt* stmt = nullptr;
		int rc = sqlite3_prepare_v2(m_db, sqlUTF8, -1, &stmt, nullptr);
		if (rc != SQLITE_OK)
		{
			CString msg;
			msg.Format(_T("准备 SQL 语句失败：%hs"), sqlite3_errmsg(m_db));
			AfxMessageBox(msg);
			return;
		}

		rc = sqlite3_bind_text(stmt, 1, utf8Name.c_str(), -1, SQLITE_TRANSIENT);
		if (rc != SQLITE_OK)
		{
			CString msg;
			msg.Format(_T("绑定模型名称失败：%hs"), sqlite3_errmsg(m_db));
			AfxMessageBox(msg);
			sqlite3_finalize(stmt);
			return;
		}

		rc = sqlite3_step(stmt);
		if (rc != SQLITE_DONE)
		{
			CString msg;
			msg.Format(_T("删除模型（%s）失败：%hs"), cstrName, sqlite3_errmsg(m_db));
			AfxMessageBox(msg);
			sqlite3_finalize(stmt);
			return;
		}

		sqlite3_finalize(stmt);
	}

	LoadModelsToList(m_bCurrentModelIsPLS);

	
	AfxMessageBox(_T("选中的模型已成功从数据库删除！"));
}



//选择数据进行测试
void CDialogQualityAnalyse::OnBnClickedButtonCalculate()
{
	CTime curTime = CTime::GetCurrentTime();
	m_strDateTime.Format(_T("%04d-%02d-%02d %02d:%02d:%02d"),
		curTime.GetYear(),
		curTime.GetMonth(),
		curTime.GetDay(),
		curTime.GetHour(),
		curTime.GetMinute(),
		curTime.GetSecond());

	m_list_result.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	// 1) 获取模型名称
	int modelCount = m_list_models.GetItemCount();
	if (modelCount == 0)
	{
		AfxMessageBox(_T("模型列表中没有任何模型，请先建立或导入模型！"));
		return;
	}

	CString modelName;
	int modelID = -1;
	if (modelCount == 1)
	{
		// 只有一个模型，自动使用该模型
		CString strId = m_list_models.GetItemText(0, 0); // 0列是ID
		modelName = m_list_models.GetItemText(0, 1); // 第1列是名称
		modelID = _ttoi(strId); // 转成 int
		m_modelName = modelName;
	}
	else
	{
		// 多个模型时，让用户在 m_list_models 中选一个
		POSITION pos = m_list_models.GetFirstSelectedItemPosition();
		if (!pos)
		{
			AfxMessageBox(_T("模型列表中有多个模型，请选择一个模型进行预测！"));
			return;
		}
		int selectedIndex = m_list_models.GetNextSelectedItem(pos);

		CString strId = m_list_models.GetItemText(selectedIndex, 0);
		modelName = m_list_models.GetItemText(selectedIndex, 1);
		modelID = _ttoi(strId);
	}

	// 2) 初始化测试结果列表
	if (needSetResultCol == true) {
		needSetResultCol = false;
		if (!SetResultListColumns(modelName, m_bCurrentModelIsPLS))
		{
			AfxMessageBox(_T("设置结果列表列名失败！"));
			return;
		}
	}

	// 3) 从数据库中获取模型参数
	CString sql;
	


	if (m_bCurrentModelIsPLS)
	{
		std::string utf8ModelName = AnsiToUtf8(CT2A(modelName, CP_ACP));
		sql.Format(_T("SELECT pls_theta, pureNum, thetaDim, pureName FROM pls_model WHERE name = '%s'  LIMIT 1"), utf8ModelName.c_str());
	}
	else
	{
		std::string utf8ModelName = AnsiToUtf8(CT2A(modelName, CP_ACP));
		sql.Format(_T("SELECT lsrp_theta, pureNum, thetaDim, pureName FROM lsrp_model WHERE name = '%s' LIMIT 1"), utf8ModelName.c_str());
	}

	char** dbResult = nullptr;
	int nRows = 0, nCols = 0;
	int rc = sqlite3_get_table(m_db, CT2A(sql), &dbResult, &nRows, &nCols, &m_errMsg);
	if (rc != SQLITE_OK || nRows < 1)
	{
		CString errorMsg;
		if (m_bCurrentModelIsPLS)
			errorMsg.Format(_T("无法获取 PLS 模型 [name=%s] 的参数：%hs"),
				 modelName, m_errMsg);
		else
			errorMsg.Format(_T("无法获取 LSRP 模型 [name=%s] 的参数：%hs"),
				 modelName, m_errMsg);
		AfxMessageBox(errorMsg);
		sqlite3_free(m_errMsg);
		sqlite3_free_table(dbResult);
		return;
	}

	// 提取模型参数
	CStringA sThetaUTF8 = dbResult[nCols * 1 + 0]; // 第一行数据，第0列
	CStringA sPureNum = dbResult[nCols * 1 + 1];   // 第一行数据，第1列
	CStringA sThetaDim = dbResult[nCols * 1 + 2];  // 第一行数据，第2列
	CStringA sPureNameUTF8 = dbResult[nCols * 1 + 3]; // 第一行数据，第3列

	sqlite3_free_table(dbResult);

	int pureNum = atoi(sPureNum);
	int thetaDim = atoi(sThetaDim);

	// 转换 theta 和 pureName
	CString sThetaW = Utf8ToCString(sThetaUTF8);
	CString sPureNameW = Utf8ToCString(sPureNameUTF8);

	// 解析 theta
	std::vector<double> vecTheta;
	SplitStringToDoubleArray(sThetaW, vecTheta, _T(","));
	if ((int)vecTheta.size() != thetaDim)
	{
		CString msg;
		if (m_bCurrentModelIsPLS)
			msg.Format(_T("PLS 模型 [%s] 中 theta 的维数 (%d) 与存储的 theta 数量 (%d) 不符！"), modelName, thetaDim, vecTheta.size());
		else
			msg.Format(_T("LSRP 模型 [%s] 中 theta 的维数 (%d) 与存储的 theta 数量 (%d) 不符！"), modelName, thetaDim, vecTheta.size());
		AfxMessageBox(msg);
		return;
	}

	// 解析 pureName，获取纯净物名称列表
	std::vector<CString> vecPureNames;
	int curPos = 0;
	CString token = sPureNameW.Tokenize(_T(",,"), curPos);
	while (!token.IsEmpty())
	{
		vecPureNames.push_back(token);
		token = sPureNameW.Tokenize(_T(",,"), curPos);
	}

	if ((int)vecPureNames.size() != pureNum)
	{
		CString msg;
		if (m_bCurrentModelIsPLS)
			msg.Format(_T("PLS 模型 [%s] 中 pureNum (%d) 与 pureName 数量 (%d) 不符！"), modelName, pureNum, vecPureNames.size());
		else
			msg.Format(_T("LSRP 模型 [%s] 中 pureNum (%d) 与 pureName 数量 (%d) 不符！"), modelName, pureNum, vecPureNames.size());
		AfxMessageBox(msg);
		return;
	}

	// 4)选择测试数据文件
	CFileDialog fileDlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		_T("CSV Files (*.csv)|*.csv|TXT Files (*.txt)|*.txt||"));
	if (fileDlg.DoModal() != IDOK)
	{
		return; 
	}

	//解析用于测试的文件,提取两列数据到m_vecX和m_vecY
	m_vecXTest.clear();
	m_vecYTest.clear();


	CString filePath = fileDlg.GetPathName();
	//vector<double> m_vecX, m_vecY;

	if (!ProcessFile(filePath, m_vecXTest, m_vecYTest, m_testName))
	{
		AfxMessageBox(_T("processFile执行失败！"));
		return ;
	}

	int pointCount = (int)m_vecXTest.size();
	if (pointCount < 1)
	{
		AfxMessageBox(_T("测试数据文件无有效数据！"));
		return;
	}

	// 5. 调用 dll_Resolution 预处理数据,存到y_test
	double* inData = new (std::nothrow) double[2 * pointCount];
	if (!inData)
	{
		AfxMessageBox(_T("内存分配失败！"));
		return;
	}
	for (int i = 0; i < pointCount; i++)
	{
		inData[i] = m_vecXTest[i];
		inData[i + pointCount] = m_vecYTest[i];
	}

	double y_test[1650] = { 0 };
	double flag = 0.0;
	dll_Resolution(inData, 2 * pointCount, y_test, &flag);
	delete[] inData;

	if (flag != 0.0)
	{
		AfxMessageBox(_T("dll_Resolution 预处理失败，无法进行预测！"));
		return;
	}

	// 6)进行预测
	std::vector<double> y_pred(pureNum, 0.0);

	if (m_bCurrentModelIsPLS)
	{
		// 使用 PLS 模型预测
		pls_res(&vecTheta[0],    // theta_pls
			pureNum,
			thetaDim,
			y_test,          // y_test[1650]
			&y_pred[0]);     // y_pred
	}
	else
	{
		// 使用 LSRP 模型预测
		// 使用现有的 BuildPureMatrix 函数构建 pureMatrix
		double* pureMatrix = new (std::nothrow) double[1650 * pureNum];
		if (!pureMatrix)
		{
			AfxMessageBox(_T("内存分配失败！"));
			return;
		}

		if (!BuildPureMatrix(pureMatrix, pureNum))
		{
			AfxMessageBox(_T("构建 pureMatrix 失败！"));
			delete[] pureMatrix;
			return;
		}

		// 调用 LSRP 预测函数
		lsrp_res(pureMatrix, &vecTheta[0], pureNum, y_test, &y_pred[0]);

		// 释放 pureMatrix 内存
		delete[] pureMatrix;
	}

	// 7) 将预测结果显示在结果列表中

	// 获取当前列表中的行数，作为插入行的索引
	int rowIndex = m_list_result.GetItemCount();

	// 向列表插入一行。InsertItem只会给第 0 列写文本，其它列需用 SetItemText
	rowIndex = m_list_result.InsertItem(rowIndex, _T(""));

	// 填写序号（第 0 列）
	CString strIndex;
	strIndex.Format(_T("%d"), rowIndex + 1);  // 序号从 1 开始
	m_list_result.SetItemText(rowIndex, 0, strIndex);

	// 填写预测结果（从第 1 列开始）
	for (int i = 0; i < pureNum; ++i)
	{
		CString temp;
		temp.Format(_T("%.5f"), y_pred[i]);
		// 第 (i+1) 列
		m_list_result.SetItemText(rowIndex, i + 1, temp);

		

	}

	// 提示用户
	AfxMessageBox(_T("预测完成，结果已显示在结果列表中！"));
}
BOOL CDialogQualityAnalyse::SetResultListColumns(const CString& modelName, bool bIsPLS) {


	CString tableName = bIsPLS ? _T("pls_model") : _T("lsrp_model");
	
	CString sql;
	std::string utf8ModelName = AnsiToUtf8(CT2A(modelName, CP_ACP));
	sql.Format(_T("SELECT pureName FROM %s WHERE name='%s' LIMIT 1"), tableName, utf8ModelName.c_str());

	char** dbResult = nullptr;
	int nRows = 0, nCols = 0;
	int rc = sqlite3_get_table(m_db, CT2A(sql), &dbResult, &nRows, &nCols, &m_errMsg);
	if (rc != SQLITE_OK || nRows < 1)
	{
	
		CString errMsg;
		errMsg.Format(_T("在表 [%s] 里根据模型名 [%s] 查询 pureName 失败：%hs"), tableName, modelName, m_errMsg);
		AfxMessageBox(errMsg);
		sqlite3_free(m_errMsg);
		sqlite3_free_table(dbResult);
		return FALSE;
	}

	CStringA pureNameUTF8 = dbResult[nCols * 1 + 0]; // nCols 是列数，这里只有 1 列：pureName
	sqlite3_free_table(dbResult);

	// UTF-8 转成 Unicode
	CString pureNameW = Utf8ToCString(pureNameUTF8);

	// 拆分 pureNameW，例如以 ",," 作为分隔符
	std::vector<CString> pureNameList;
	int curPos = 0;
	CString token = pureNameW.Tokenize(_T(",,"), curPos);
	while (!token.IsEmpty())
	{
		token.Trim();
		pureNameList.push_back(token);
		token = pureNameW.Tokenize(_T(",,"), curPos);
	}

	//清除现有列
	while (m_list_result.GetHeaderCtrl()->GetItemCount() > 0)
	{
		m_list_result.DeleteColumn(0);
	}

	// 计算列宽并插入列
	CRect rect;
	m_list_result.GetClientRect(&rect);
	int totalWidth = rect.Width();

	// 列总数 = 1（序号列）+ pureNameList.size()
	int columnCount = 1 + (int)pureNameList.size();
	int columnWidth = columnCount > 0 ? totalWidth / columnCount : totalWidth;

	// 第 0 列：序号
	m_list_result.InsertColumn(0, _T("序号"), LVCFMT_CENTER, columnWidth);

	// 后续列：纯净物名称
	for (size_t i = 0; i < pureNameList.size(); ++i)
	{
		m_list_result.InsertColumn((int)(i + 1), pureNameList[i], LVCFMT_CENTER, columnWidth);
		m_pureName += pureNameList[i];
		if (i < pureNameList.size() - 1)
		{
			m_pureName += _T(" : ");
		}
	}
	

	//AfxMessageBox(m_pureName);
	return TRUE;
}
void CDialogQualityAnalyse::OnBnClickedBtnSelPred()
{
	// 获取选中项的索引
	std::vector<int> selectedIndices;
	POSITION pos = m_list_result.GetFirstSelectedItemPosition(); // 获取第一个选中项的位置
	while (pos) {
		int index = m_list_result.GetNextSelectedItem(pos); // 获取选中项的索引
		selectedIndices.push_back(index);
	}

	// 如果没有选择项，提示用户
	if (selectedIndices.empty()) {
		AfxMessageBox(_T("请先选择要保留的一项！"));
		return;
	}

	// 临时保存选中项的数据
	std::vector<std::vector<CString>> selectedData;
	for (int i : selectedIndices) {
		std::vector<CString> rowData;
		for (int col = 0; col < m_list_result.GetHeaderCtrl()->GetItemCount(); ++col) {
			rowData.push_back(m_list_result.GetItemText(i, col));
		}
		selectedData.push_back(rowData);
	}

	// 清空列表
	m_list_result.DeleteAllItems();

	// 将选中项重新插入到列表中
	for (size_t i = 0; i < selectedData.size(); ++i) {
		int rowIndex = m_list_result.InsertItem(i, selectedData[i][0]); // 插入第0列（序号）
		for (size_t col = 1; col < selectedData[i].size(); ++col) {
			m_list_result.SetItemText(rowIndex, col, selectedData[i][col]); // 设置其他列数据
		}
	}
}


//pdf导出
using namespace std;

std::string UnicodeToUtf8(const std::wstring& strUnicode)
{
	int len = WideCharToMultiByte(CP_UTF8, 0, strUnicode.c_str(), -1, NULL, 0, NULL, NULL);
	if (len == 0)
	{
		return "";
	}

	char* pRes = new char[len];
	if (pRes == NULL)
	{
		return "";
	}

	WideCharToMultiByte(CP_UTF8, 0, strUnicode.c_str(), -1, pRes, len, NULL, NULL);
	pRes[len - 1] = '\0';
	std::string result = pRes;
	delete[] pRes;

	return result;
}
std::wstring StringToWString(const std::string& str)
{
	int len = MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, NULL, 0);
	if (len == 0)
	{
		return L"";
	}

	wchar_t* pRes = new wchar_t[len];
	if (pRes == NULL)
	{
		return L"";
	}

	MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, pRes, len);
	pRes[len - 1] = L'\0';
	std::wstring result = pRes;
	delete[] pRes;

	return result;
}
string toUtf8(const char* str) {
	string s = UnicodeToUtf8(StringToWString(str));
	return s;
}
bool LoadAndUseSimHeiFont(PDF* p, double fontSize)
{

	char exePath[MAX_PATH];
	GetModuleFileNameA(NULL, exePath, MAX_PATH);

	char* pLastSlash = strrchr(exePath, '\\');
	if (pLastSlash)
	{
		*pLastSlash = '\0';
	}

	strcat(exePath, "\\font\\SimHei:1");

	std::string utf8Path = toUtf8(exePath);
	char        bom[4] = { (char)0xEF, (char)0xBB, (char)0xBF, 0 };
	std::string fontPathWithBom = bom;
	fontPathWithBom += utf8Path;

	int font_hei = PDF_load_font(p, fontPathWithBom.c_str(), 0, "unicode", "");
	if (font_hei == -1)
	{
		AfxMessageBox(_T("加载字体失败，未找到 SimHei.ttf 或 SimHei:1 字体文件。"));
		return false;  
	}

	PDF_setfont(p, font_hei, fontSize);
	PDF_setcolor(p, "fill", "rgb", 0, 0, 0, 0);

	return true; 
}
void CDialogQualityAnalyse::OnBnClickedButtonOutputPdf() {
	//1)检查数据的有效性
	if (m_vecXTest.empty() || m_vecYTest.empty())
	{
		AfxMessageBox(_T("没有可绘制的数据！"));
		return;
	}
	int itemCount = m_list_result.GetItemCount();

	if (itemCount > 1) {
		AfxMessageBox(_T("结果列表中存在多条数据，请只保留一条数据后再导出！"));
		return;
	}

	//弹出对话框选择保存路径
	CString pdfFilePath;
	while (true) {
		CFileDialog dlg(FALSE, _T("pdf"), _T("TestReport.pdf"),
			OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
			_T("PDF Files (*.pdf)|*.pdf||"), this);

		if (dlg.DoModal() != IDOK) {
			return;
		}

		pdfFilePath = dlg.GetPathName();			 // 用户选择的文件路径

													
		CString fileExtension = pdfFilePath.Right(4).MakeLower(); 
		if (fileExtension != _T(".pdf")) {
			AfxMessageBox(_T("文件扩展名必须为 .pdf，请重新输入文件名！"));
			continue;							
		}
		break;										
	}

	std::ofstream testFile((LPCTSTR)pdfFilePath, std::ios::out | std::ios::app);//检查文件是否已经打开
	if (!testFile.is_open()) {
		AfxMessageBox(_T("PDF文件已被打开，请关闭后再试！"));
		return;
	}
	testFile.close();


	//2)初始化pdf
	PDF* p = PDF_new();

	std::string utf8Str = toUtf8(pdfFilePath);
	char bom[4] = { (char)0xEF, (char)0xBB, (char)0xBF, 0 };// 加 BOM
	std::string filenameBom = bom;
	filenameBom += utf8Str;

	if (PDF_begin_document(p, filenameBom.c_str(), 0, "") == -1) {
		CString errorMsg;
		errorMsg.Format(_T("05PDF_begin_document failed. Path: %s"), pdfFilePath);
		AfxMessageBox(errorMsg);
		PDF_delete(p);
		return;
	}

	PDF_set_option(p, "charref=true");              //启用字符引用模式。字符引用模式允许在文本中使用字符的 Unicode 码点作为引用，这对于处理非 ASCII 字符（如中文、日文、韩文等）非常有用。
	PDF_set_option(p, "textformat=utf8");           //指定输入文本的编码格式为 UTF-8。UTF-8 是一种广泛使用的字符编码，能够表示几乎所有书写系统的字符，因此在处理多语言文本时非常有用。
	PDF_set_info(p, "Creator", "PDF Creator");      //创建者
	PDF_set_info(p, "Title", "Convert to PDF");     //标题
	PDF_begin_page_ext(p, a4_width, a4_height, "");  


	//3)绘制外层大矩形
	PDF_setlinewidth(p, 1.0);                      //线宽
	PDF_setcolor(p, "stroke", "rgb", 0, 0, 0, 0);  //颜色

	double outerMargin = 40;
	double outerRectX = outerMargin;
	double outerRectY = outerMargin;
	double outerRectW = 595 - 2 * outerMargin;  
	double outerRectH = 842 - 2 * outerMargin; 

	PDF_setcolor(p, "fill", "rgb", 0.95, 0.95, 0.95, 0);           // 1) 设置填充色: 浅灰
	PDF_rect(p, outerRectX, outerRectY, outerRectW, outerRectH);   // 2) 绘制矩形路径
	PDF_fill(p);                                                   // 3) 填充（不画边框）

	PDF_setcolor(p, "stroke", "rgb", 0, 0, 0, 0);                  
	PDF_rect(p, outerRectX, outerRectY, outerRectW, outerRectH);
	PDF_stroke(p);

	//4)table-Examining Report 
	{

		double outerTopY = outerRectY + outerRectH;

		double lineY = outerTopY - 30.0;

		PDF_setcolor(p, "stroke", "rgb", 0, 0, 0, 0);
		PDF_setlinewidth(p, 1.0);

		PDF_moveto(p, outerRectX, lineY);
		PDF_lineto(p, outerRectX + outerRectW, lineY);
		PDF_stroke(p);

		int fontTitle = PDF_load_font(p, "Helvetica-Bold", 0, "winansi", "");
		PDF_setfont(p, fontTitle, 12.0);
		PDF_setcolor(p, "fill", "rgb", 0, 0, 0, 0);

		double textX = outerRectX + outerRectW / 2.0;
		double textY = lineY + 10;
		PDF_fit_textline(p, "Test Report", 0, textX, textY, "position={center bottom}");
	}


	//5)table-spectrum
	{
		double outerTopY = outerRectY + outerRectH;
		double lineTop = outerTopY - 110.0;
		double lineBottom = outerTopY - 140.0;

		PDF_setcolor(p, "stroke", "rgb", 0, 0, 0, 0);
		PDF_setlinewidth(p, 1.0);

		PDF_moveto(p, outerRectX, lineTop);
		PDF_lineto(p, outerRectX + outerRectW, lineTop);
		PDF_stroke(p);

		PDF_moveto(p, outerRectX, lineBottom);
		PDF_lineto(p, outerRectX + outerRectW, lineBottom);
		PDF_stroke(p);

		int fontTitle = PDF_load_font(p, "Helvetica-Bold", 0, "winansi", "");
		PDF_setfont(p, fontTitle, 12.0);
		PDF_setcolor(p, "fill", "rgb", 0, 0, 0, 0);

		double textX = outerRectX + outerRectW / 2.0;
		double textY = lineBottom + 10;
		PDF_fit_textline(p, "Spectrum", 0, textX, textY, "position={center bottom}");
	}




	//6)curve of spectrum
	{
		double pageW = 595, pageH = 842;

		double rectInnerPad = 40;
		double marginLeft = outerRectX + rectInnerPad;
		double marginRight = pageW - (outerRectX + outerRectW) + rectInnerPad;
		double marginBottom = 400;
		double marginTop = 200;

		double chartRectX = marginLeft;
		double chartRectY = marginBottom;
		double chartRectW = (pageW - marginRight) - marginLeft;
		double chartRectH = (pageH - marginTop) - marginBottom;

		PDF_setcolor(p, "fill", "rgb", 1.0, 1.0, 1.0, 0);                     //白色填充
		PDF_rect(p, chartRectX, chartRectY, chartRectW, chartRectH);
		PDF_fill(p);														 
	
	
		double minX = *std::min_element(m_vecXTest.begin(), m_vecXTest.end());//根据测试数据来确定x轴和y轴范围
		double maxX = *std::max_element(m_vecXTest.begin(), m_vecXTest.end());
		double minY = *std::min_element(m_vecYTest.begin(), m_vecYTest.end());
		double maxY = *std::max_element(m_vecYTest.begin(), m_vecYTest.end());

		
		if (maxX == minX) maxX = minX + 1;									// 避免 max=min
		if (maxY == minY) maxY = minY + 1;


		auto scaleX = [&](double x) {
			double w = (pageW - marginLeft - marginRight);
			return marginLeft + (x - minX) / (maxX - minX)* w;
		};
		auto scaleY = [&](double y) {
			double h = (pageH - marginBottom - marginTop);
			return marginBottom + (y - minY) / (maxY - minY)* h;
		};

		
		PDF_setcolor(p, "stroke", "rgb", 0, 0, 0, 0);// 画坐标轴  (p为矩形框线条宽度)
		PDF_setlinewidth(p, 1.0);

		
		PDF_moveto(p, marginLeft, marginBottom);// X 轴
		PDF_lineto(p, pageW - marginRight, marginBottom);
		PDF_stroke(p);

	
		PDF_moveto(p, marginLeft, marginBottom);// X 轴
		PDF_lineto(p, marginLeft, pageH - marginTop);
		PDF_stroke(p);
		
		PDF_moveto(p, marginLeft, pageH - marginTop);				 // 上边界线条// 从左上角开始
		PDF_lineto(p, pageW - marginRight, pageH - marginTop);		 // 到右上角
		PDF_stroke(p);

		PDF_moveto(p, pageW - marginRight, marginBottom);			 // 右边界线条// 从右下角开始
		PDF_lineto(p, pageW - marginRight, pageH - marginTop);		 // 到右上角
		PDF_stroke(p);

		
		PDF_setcolor(p, "stroke", "gray", 0.8, 0, 0, 0);			 // 设置浅灰色网格
		PDF_setlinewidth(p, 0.5);

		for (double xv = ceil(minX / 500) * 500; xv <= maxX; xv += 500)	          // X 方向网格 步长500 
		{
			double px = scaleX(xv);
			PDF_moveto(p, px, marginBottom);// 竖线
			PDF_lineto(p, px, pageH - marginTop);
			PDF_stroke(p);
		}

		
		for (double yv = ceil(minY / 2500) * 2500; yv <= maxY; yv += 2500)        // Y 方向网格 步长2500
		{
			double py = scaleY(yv);
			PDF_moveto(p, marginLeft, py);// 横线
			PDF_lineto(p, pageW - marginRight, py);
			PDF_stroke(p);
		}

		//绘制刻度和刻度文字
		int font = PDF_load_font(p, "Helvetica", 0, "winansi", "");
		PDF_setfont(p, font, 8.0);
		PDF_setcolor(p, "fill", "rgb", 0, 0, 0, 0); // 黑色文字
		//X刻度
		double majorTickStep_x = 500.0;  // 主刻度间距
		double minorTickStep_x = 50.0;   // 副刻度间距

		double startX = floor(minX / minorTickStep_x) * minorTickStep_x;
		if (startX > minX) startX -= minorTickStep_x;


		for (double xv = startX; xv <= maxX; xv += minorTickStep_x)
		{
			double px = scaleX(xv);

			bool isMajorTick = (fmod(xv, majorTickStep_x) == 0);			// 判断是否主刻度

		
			double tickLen = isMajorTick ? 3.0 : 2.0;	// 如果是主刻度，线更长，并显示文字

			PDF_moveto(p, px, marginBottom);           // 画刻度线
			PDF_lineto(p, px, marginBottom - tickLen);
			PDF_stroke(p);

			if (isMajorTick)                           // 如果是主刻度，就显示数字
			{
				
				double pyText = marginBottom - 10.0;   // 显示数值文本在刻度线下方
				CString label;
				label.Format(_T("%.0f"), xv);

				PDF_fit_textline(p, CT2A(label), 0, px, pyText, "position={center bottom}");// 让文字底对齐坐标并居中

			}
		}

		//Y刻度
		double majorTickStepY = 5000.0;   // 主刻度间隔
		double minorTickStepY = 500.0;    // 副刻度间隔
							  
		double startY = floor(minY / minorTickStepY) * minorTickStepY; // 找一个不大于 minY 的整500开始
		if (startY > minY) {
			startY -= minorTickStepY;
		}

	
		for (double yv = startY; yv <= maxY; yv += minorTickStepY)   // 从 startY 到 maxY，每隔 500 就画一次刻度
		{
			double py = scaleY(yv);
			bool isMajor = (fmod(yv, majorTickStepY) == 0);			// 判断是                                                                                                                              否是主刻度(可用取余或 fmod)
			double tickLen = isMajor ? 5.0 : 3.0;			        // 如果是主刻度，线更长并显示文字；			
			PDF_moveto(p, marginLeft, py);			                // 画刻度：从 (marginLeft, py) 往左画 tickLen

			PDF_lineto(p, marginLeft - tickLen, py);
			PDF_stroke(p);

			if (isMajor)// 若是主刻度，显示数值标签
			{
				double pxText = marginLeft - 8.0;				    // 在刻度左边稍微留点距离
				CString label;
				label.Format(_T("%.0f"), yv);

				PDF_fit_textline(p, CT2A(label), 0, pxText, py, "position={right center}");	// 将文字居中对齐(垂直)该坐标	
																							// position={right center} => 右对齐
			}
		}

		//绘制折线图
		PDF_setcolor(p, "stroke", "rgb", 1, 0, 0, 0);			    // 线条为红色
		PDF_setlinewidth(p, 0.1);

		PDF_moveto(p, scaleX(m_vecXTest[0]), scaleY(m_vecYTest[0]));
		for (size_t i = 1; i < m_vecXTest.size(); i++)
		{
			PDF_lineto(p, scaleX(m_vecXTest[i]), scaleY(m_vecYTest[i]));
		}
		PDF_stroke(p);
	}


	//7) Exaimine Result 
	{

	double lineTop = outerRectY + 180.0;
	double lineBottom = outerRectY + 150.0;

	PDF_setcolor(p, "stroke", "rgb", 0, 0, 0, 0);
	PDF_setlinewidth(p, 1.0);

	PDF_moveto(p, outerRectX, lineTop);
	PDF_lineto(p, outerRectX + outerRectW, lineTop);
	PDF_stroke(p);

	PDF_moveto(p, outerRectX, lineBottom);
	PDF_lineto(p, outerRectX + outerRectW, lineBottom);
	PDF_stroke(p);

	int fontTitle = PDF_load_font(p, "Helvetica-Bold", 0, "winansi", "");
	PDF_setfont(p, fontTitle, 12.0);
	PDF_setcolor(p, "fill", "rgb", 0, 0, 0, 0);

	double textX = outerRectX + outerRectW / 2.0;
	double textY = lineBottom + 10;
	PDF_fit_textline(p, "Test Result", 0, textX, textY, "position={center bottom}");
   }
	
		
	//8)date
      {
			
		double lineTop = outerRectY + 150.0;
		double lineBottom = outerRectY + 120.0;
	
		PDF_setcolor(p, "stroke", "rgb", 0, 0, 0, 0);
		PDF_setlinewidth(p, 1.0);
	
		PDF_moveto(p, outerRectX, lineTop);
		PDF_lineto(p, outerRectX + outerRectW, lineTop);
		PDF_stroke(p);
	
		PDF_moveto(p, outerRectX, lineBottom);
		PDF_lineto(p, outerRectX + outerRectW, lineBottom);
		PDF_stroke(p);
	
		// 在矩形中央画一条竖线，把它分为左右两半
		double centerX = outerRectX + outerRectW / 3.0;
		PDF_moveto(p, centerX, lineTop);
		PDF_lineto(p, centerX, lineBottom);
		PDF_stroke(p);
	

		LoadAndUseSimHeiFont(p, 12);
		// 左半部分文字位置
		double textY = lineBottom + 10.0; 
		double textX = outerRectX + 20.0;
	
		PDF_fit_textline(p, "Date", 0, textX, textY, "position={left bottom}");
	
		// 右半部分文字位置
		double rightCenterX = outerRectX + (outerRectW * 3.0 / 4.0);
		PDF_fit_textline(p, CT2A(m_strDateTime), 0, centerX+20, textY, "position={left bottom}");
	}

	 // 8.5)model name
	  {
		  double lineTop = outerRectY + 120.0;
		  double lineBottom = outerRectY + 90.0;

		  PDF_setcolor(p, "stroke", "rgb", 0, 0, 0, 0);
		  PDF_setlinewidth(p, 1.0);


		  PDF_moveto(p, outerRectX, lineTop);
		  PDF_lineto(p, outerRectX + outerRectW, lineTop);
		  PDF_stroke(p);


		  PDF_moveto(p, outerRectX, lineBottom);
		  PDF_lineto(p, outerRectX + outerRectW, lineBottom);
		  PDF_stroke(p);


		  double centerX = outerRectX + outerRectW / 3.0;
		  PDF_moveto(p, centerX, lineTop);
		  PDF_lineto(p, centerX, lineBottom);
		  PDF_stroke(p);

		  
		  LoadAndUseSimHeiFont(p, 12);

		   //左半部分文字位置
		  double leftCenterX = outerRectX + (outerRectW / 4.0);
		  double textY = lineBottom + 10.0;
		  double textX = outerRectX + 20.0;

		  PDF_fit_textline(p, "Model Name", 0, textX, textY, "position={left bottom}");
		  //右边文字部分
		 
		 
		  int nHeight = a4_height;
		  nHeight -= 50;
		  PDF_set_text_pos(p, 50, nHeight);
		  PDF_setcolor(p, "fill", "rgb", 0, 0, 0, 0);
		  PDF_setlinewidth(p, 2.0);

		  std::string std(m_modelName.GetBuffer());

		  static const char* DFTitle[] = { nullptr };
		  DFTitle[0] = std.c_str();

		  PDF_set_text_pos(p, 232, nHeight - 653);//字体坐标
		  char a[100];

		  strcpy_s(a, toUtf8(DFTitle[0]).c_str());
		  strcpy_s(a, DFTitle[0]);

		  PDF_show(p, toUtf8(DFTitle[0]).c_str());

	  }

	  //9) Mixture Name
	  	{
	 
	  		double lineTop = outerRectY + 90.0;
	  		double lineBottom = outerRectY + 60.0;
	  
	  		PDF_setcolor(p, "stroke", "rgb", 0, 0, 0, 0);
	  		PDF_setlinewidth(p, 1.0);
	  
	  
	  		PDF_moveto(p, outerRectX, lineTop);
	  		PDF_lineto(p, outerRectX + outerRectW, lineTop);
	  		PDF_stroke(p);
	  
	  
	  		PDF_moveto(p, outerRectX, lineBottom);
	  		PDF_lineto(p, outerRectX + outerRectW, lineBottom);
	  		PDF_stroke(p);
	  
	  
	  		double centerX = outerRectX + outerRectW / 3.0;
	  		PDF_moveto(p, centerX, lineTop);
	  		PDF_lineto(p, centerX, lineBottom);
	  		PDF_stroke(p);
	  
	  		// 3) 左右两半分别写文字
	  		//    比如左半边写 "Date"，右半边写某个数值
			LoadAndUseSimHeiFont(p, 12);
	  
	  		// 左半部分文字位置
	  		double leftCenterX = outerRectX + (outerRectW / 4.0);
	  		double textY = lineBottom + 10.0;  
			double textX = outerRectX + 20.0;
	  
	  		PDF_fit_textline(p, "Mixture Name", 0, textX, textY, "position={left bottom}");
	  
	  		// 右半部分文字位置
	  		double rightCenterX = outerRectX + (outerRectW * 3.0 / 4.0);
	  		PDF_fit_textline(p, CT2A(m_testName), 0, centerX+20, textY, "position={left bottom}");
	  	}

		
		//10)Component 
		{
				double lineTop = outerRectY + 60.0;
				double lineBottom = outerRectY + 30.0;
		
				PDF_setcolor(p, "stroke", "rgb", 0, 0, 0, 0);
				PDF_setlinewidth(p, 0.7);
		
				PDF_moveto(p, outerRectX, lineTop);
				PDF_lineto(p, outerRectX + outerRectW, lineTop);
				PDF_stroke(p);
		
				PDF_moveto(p, outerRectX, lineBottom);
				PDF_lineto(p, outerRectX + outerRectW, lineBottom);
				PDF_stroke(p);
		
				double centerX = outerRectX + outerRectW / 3.0;
				PDF_moveto(p, centerX, lineTop);
				PDF_lineto(p, centerX, lineBottom);
				PDF_stroke(p);
		
				LoadAndUseSimHeiFont(p, 12);
				
				// 左半部分
				double textY = lineBottom + 10.0;
				double textX = outerRectX + 20.0;
		
				PDF_fit_textline(p, "Component", 0, textX, textY, "position={left bottom}");
		
	
			    //右半部分
				int nHeight = a4_height;
				//int font_song = PDF_load_font(p, "C:\\Windows\\Fonts\\simsun:1", 0, "unicode", "");
				//int font_hei = PDF_load_font(p, "simhei:1", 0, "unicode", "");
				//int font_hei= PDF_load_font(p, "C:\\Windows\\Fonts\\simhei:1", 0, "unicode","");
				  
				nHeight -= 50;
				PDF_set_text_pos(p, 50, nHeight);
				PDF_setcolor(p, "fill", "rgb", 0, 0, 0, 0);
				PDF_setlinewidth(p, 2.0);
			
				std::string std(m_pureName.GetBuffer());

				static const char* DFTitle[] = { nullptr };
				DFTitle[0] = std.c_str();

				PDF_set_text_pos(p, 232, nHeight - 713);//字体坐标
				char a[100];

				strcpy_s(a, toUtf8(DFTitle[0]).c_str());
				strcpy_s(a, DFTitle[0]);

				PDF_show(p, toUtf8(DFTitle[0]).c_str());
			}

		//10)composition
		{
		
		double lineTop = outerRectY + 30.0;
		double lineBottom = outerRectY + 0;
		
		PDF_setcolor(p, "stroke", "rgb", 0, 0, 0, 0);
		PDF_setlinewidth(p, 1.0);
		
		PDF_moveto(p, outerRectX, lineTop);
		PDF_lineto(p, outerRectX + outerRectW, lineTop);
		PDF_stroke(p);
		
		PDF_moveto(p, outerRectX, lineBottom);
		PDF_lineto(p, outerRectX + outerRectW, lineBottom);
		PDF_stroke(p);
		
		double centerX = outerRectX + outerRectW / 3.0;
		PDF_moveto(p, centerX, lineTop);
		PDF_lineto(p, centerX, lineBottom);
		PDF_stroke(p);
		
		LoadAndUseSimHeiFont(p, 12);
		
		// 左半部分文字位置
		//double leftCenterX = outerRectX + (outerRectW / 4.0);
		double textY = lineBottom + 10.0;  
		double textX = outerRectX + 20.0;
		
		PDF_fit_textline(p, "Proportion", 0, textX, textY, "position={left bottom}");
		
		// 右半部分文字位置
		double rightCenterX = outerRectX + (outerRectW * 3.0 / 4.0);

		//获取结果列表第一行的值
		int colCount = m_list_result.GetHeaderCtrl()->GetItemCount();
		CString strJoined;
		for (int col = 1; col < colCount; ++col)
		{
			CString itemText = m_list_result.GetItemText(0, col); 
			if (col > 1) 
			{
				strJoined += _T(" : ");
			}
			strJoined += itemText;
		}
		std::string pred = strJoined.GetString();
		PDF_fit_textline(p, pred.c_str(), 0, centerX+20, textY, "position={left bottom}");
		}
	    //结束
    	PDF_end_page_ext(p, "");
	    PDF_end_document(p, "");
	    PDF_delete(p);
	
	    AfxMessageBox(_T("导出pdf成功!"));
}




