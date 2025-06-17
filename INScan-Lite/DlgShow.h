#pragma once
#include "dbado\DataCompareLast.h"
#include "afxdialogex.h"
//#include "INScan-LiteDlg.h"
//#include "lines\BaselineCorrect.h"
//#include "lines\LineMath.h"
//  lcz  new
#include "lines\zolix_algorithm.h"
#include "..\file\TeeChart2011\tchart.h"
#include "..\file\TeeChart2011\teepoint2d.h"

 #define WM_SHOWDATA      WM_USER+9001


// CDlgShow 对话框

class CDlgShow : public CDialog
{
	DECLARE_DYNAMIC(CDlgShow)

public:
	CDlgShow(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgShow();

// 对话框数据
	enum { IDD = IDD_DLG_SHOW };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	afx_msg LRESULT OnShowData(WPARAM wParam,LPARAM lParam);

	DECLARE_MESSAGE_MAP()

private:
	int m_iMouseX;
	int m_iMouseY;
	bool m_bEnterSeries;
	int m_iEnterSeriesIndex;
public:
	int m_iSeriesIndex;

	BOOL chickline;
	BOOL is_HandMark;
	BOOL is_DelMark;
	int nCurChartLine;
	int nSeries;
	//  更改某条曲线的名字
	void updateName(CString strUpdateName,int nSer);
	//  更改某条曲线的名字
	void updateName(CString strUpdateName,CString strOldName);
	//  根据曲线名称来打开曲线（从下拉框里选择曲线）
	void showLine(CString strName);
	//  设置第nNum条曲线的颜色
	void setColor(int R,int G,int B,int nNum);
	//  删除第nSer条曲线（nSer从0开始）
	void del(int nSer);
	//  删除所有曲线
	void delAll();
	virtual BOOL OnInitDialog();
	//  预处理消息
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//  设置激光功率
	void SetLawerPower(int nPow,BOOL bS);//bs 代表 开关激光器状态
	//  根据百分比值，得到传入串口的min~max之间的值（转换了的）
	int SendLaser(int nlaser);
	double ReturnMaxData(double *buffer,int nSize);
	double ReturnK(double dData);
	//  自动缩放
	void OnAutomatic();
	//  自动重绘
	void OnSetAutoRepaint(BOOL bSet);
	void GetBackData(double *optData,double *BkData);
	int GetTeeCount();
    BOOL ExportTxtFile(CString strFilename);
	int ExportTxtFiles(CString fileFolderPath,CString fileExt, bool bForce = false);
	BOOL ImportFullLine(LPCTSTR txtFilename);
	CString FileNameFormat(CString strLineName);
	void AutoSaveFile(CString strLineName);
	void OnSmooth();
	void JohnSmooth(SHORT LineSize, DOUBLE* YArray);
	void SaveLineValue();
	void PolynomSubChart(int width,int degree);
	void Nihe(int cvalue);
	void Recover();
	void subline();
	void clearline();
	int  GetCount();
	void ReMoveSeries(int nR);
	void MarksValue(int in_width,int in_maxvalue );
	void DeleteShx();
	void DeleteChMarks(double xvalue);
	void AddPointMarks(long SeriesIndex,long x,double y);
	void SetSize();
	void OriginalData(double * intensity);
	void OriginalDataX(double * intensity);
	BOOL SelectSql();
	void AddSqlSer(int nChoose);
	double CaulNum(double lValueX);
	double ScanOffset(double x,double y);
	BOOL bReturn;
	CTChart m_Tchart;
	CDataCompareLast mDataCompare;
	std::vector<double> XVector,YVector,niheVector;

	DECLARE_EVENTSINK_MAP()
	void OnDblClickTchart1();
	void OnClickSeriesTchart1(long SeriesIndex, long ValueIndex, long Button, long Shift, long X, long Y);
	void OnMouseMoveTchart1(long Shift, long X, long Y);
	void OnClickBackgroundTchart1(long Button, long Shift, long X, long Y);

	void OnClickLegendTchartMainShow(long Button, long Shift, long X, long Y);

	afx_msg void OnUpdateName();

	afx_msg void OnTimer(UINT_PTR nIDEvent);
	void OnMouseDownTchartMainShow(long Button, long Shift, long X, long Y);
	void OnMouseUpTchartMainShow(long Button, long Shift, long X, long Y);
	void OnMouseMoveTchartMainShow(long Shift, long X, long Y);
	void OnMouseEnterSeriesTchartMainShow2017(long SeriesIndex);
	void OnMouseLeaveSeriesTchartMainShow2017(long SeriesIndex);

	static UINT TestThread(LPVOID pParam);
	static UINT MoniThread(LPVOID pParam);

	void Test();
	
	void OnUndoZoomTchartMainShow();
	//  lcz new  只根据index显示一条曲线，单选
public:
	void showSingleLine(int index);
	//  放大
	void OnZoomTchartMainShow();
	int m_iZoomCount;

	HANDLE m_hEvent;

	void show_dfield_get_data(char id, unsigned short* buff, int buff_size, int cutf_size);
};
