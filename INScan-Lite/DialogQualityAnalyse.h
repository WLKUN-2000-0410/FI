#pragma once
#include "..\sqlite\sqlite3.h"
#include "afxcmn.h"
#include "afxwin.h"
// CDialogQualityAnalyse 对话框

class CDialogQualityAnalyse : public CDialogEx
{
	DECLARE_DYNAMIC(CDialogQualityAnalyse)

public:
	CDialogQualityAnalyse(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDialogQualityAnalyse();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_QUALITY_ANALYSE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	sqlite3* m_db;          // 数据库指针
	char* m_errMsg;         
	int m_nRow;             // sqlite3_get_table 查询返回的行数
	int m_nColumn;          // sqlite3_get_table 查询返回的列数

	CString m_strModelName;  

	CListCtrl m_list_pure;
	CListCtrl m_list_mix;
	CListCtrl m_list_ratio;
	CListCtrl m_list_models;
	CListCtrl m_list_result;

	bool m_bCurrentModelIsPLS; // true 表示当前选择的是 PLS 模型，false 表示 LSRP 模型
	bool m_bCurrentImportIspure; // ture 表示当前是pure导入,false表示当前是mix导入
	bool needSetResultCol;

	afx_msg void OnBnClickedButtonAddPureData();
	virtual BOOL OnInitDialog();                     
	BOOL InitDB(); 
	BOOL ImportPureFromFile(const CString& filePath);
	BOOL InsertPureDataToDB(const CString& pureName, const CString& strX, const CString& strY, const CString& strProcessedY);
	
	afx_msg void OnBnClickedButtonAddMixData();
	BOOL ImportMixFromFile(const CString& filePath);
	BOOL InsertMixDataToDB(const CString& pureName, const CString& strX, const CString& strY, const CString& strProcessedY);
	afx_msg void OnBnClickedButtonImportRatio();

	BOOL ImportRatioFromFile(const CString& ratioFilePath, int pureCount, int mixCount);
	vector<double> m_vecRatio;
	afx_msg void OnBnClickedButtonBuildModel();
	BOOL CDialogQualityAnalyse::SplitStringToDoubleArray(const CString& sData,std::vector<double>& outArr,LPCTSTR delim);
	

	BOOL CDialogQualityAnalyse::LoadPureTableToList();
	CString Utf8ToCString(const char* utf8Str);
	std::string AnsiToUtf8(const char* ansiStr, UINT codePage = CP_ACP);

	afx_msg void OnBnClickedBtnSelPure();
	BOOL LoadMixTableToList();
	afx_msg void OnBnClickedBtnSelMix();
	BOOL SetRatioListColumns(const std::vector<CString>& columnNames);

	BOOL BuildPureMatrix(double* pureMatrix, int pureCount);
	BOOL BuildMixMatrix(double* mixMatrix, int mixCount);
	BOOL BuildRatioMatrix(double* ratioMatrix, int pureCount, int mixCount);
	BOOL InsertPlsModelToDB(const CString& modelName, const CString& strPlsTheta, int pureNum, int mixNum, int thetaDim, const CString& pureName);



	afx_msg void OnBnClickedBtnBuildNetmodel();
	afx_msg void OnBnClickedRadioPls();
	void LoadModelsToList(bool bPLS);
	afx_msg void OnBnClickedRadioLpls();
	afx_msg void OnBnClickedButtonCalculate();
	afx_msg void OnBnClickedBtnSelModel();
	BOOL SetResultListColumns(const CString& modelName, bool bIsPLS);
	BOOL ProcessFile(const CString& filePath, vector<double>& vecX, vector<double>& vecY,CString & fileName);
	afx_msg void OnBnClickedButtonOutputPdf();

private:
	std::vector<double> m_vecXTest; // 用于保存测试数据的X
	std::vector<double> m_vecYTest; // 用于保存测试数据的Y
	CString m_strDateTime;
	CString m_testName;
	CString m_pureName;
	CString m_modelName;
	
public:
	afx_msg void OnBnClickedBtnSelPred();
	afx_msg void OnBnClickedBtnPureDel();
	afx_msg void OnBnClickedButtonMixDel();
	afx_msg void OnBnClickedBtnDelModel();
};
