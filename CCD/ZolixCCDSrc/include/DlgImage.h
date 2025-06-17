#pragma once


// CDlgImage 对话框
#include "MFCResMacros.h"
#include "..\common\resource.h"
// CDlgAndorSet 对话框
NS_MFC_Res_BEGIN
class CDlgImage : public CDialog
{
	DECLARE_DYNAMIC(CDlgImage)

public:
	CDlgImage(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgImage();

// 对话框数据
	enum { IDD = ID_DLG_IMAGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
protected:
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
public:
	void SetMark(int bMark);
	void SetPicSize(int NCols,int NRows);
	void OnDisplay();
	void GetPoint(int *x,int *y);
	int SavePicBmp();
	BYTE * pBuffer;
private:
	void DrawBmp(BYTE * pBuffer/*,int NCols,int NRows*/);

	int nMark;
	int Checkx;
	int Checky;
	int gCols;
	int gRows;
	CPoint gPoint;
public:

};
NS_MFC_Res_END