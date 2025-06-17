// DlgImage.cpp : 实现文件
//
#pragma once
#include "stdafx.h"
#include "ZolixCCD.h"
#include "DlgImage.h"
#include "afxdialogex.h"
#include "Dib.h"
#define USB_MAX_SIZE  2048*1024*3;
// CDlgImage 对话框
NS_MFC_Res_BEGIN
IMPLEMENT_DYNAMIC(CDlgImage, CDialog)

CDlgImage::CDlgImage(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgImage::IDD, pParent)
{
	nMark=FALSE;
	pBuffer = (BYTE *)malloc(2048*1024*3);
	memset(pBuffer, 100, 2048*1024*3);
	gCols=2000;
	gRows=128;
	gPoint.x=0;
	gPoint.y=0;
	nMark=0;
}

CDlgImage::~CDlgImage()
{
	free(pBuffer);
}

void CDlgImage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgImage, CDialog)
	ON_WM_MOUSEMOVE()
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_LBUTTONDOWN()
	ON_WM_TIMER()
END_MESSAGE_MAP()
void CDlgImage::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialog::OnPaint()
	if (pBuffer!=NULL)
	{
		DrawBmp(pBuffer/*,int NCols,int NRows*/);
	}
	
}


void CDlgImage::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);
	//CRect rect;
	//GetClientRect(&rect);
	//ScreenToClient(&rect);
	//CWnd *pWnd;
	//pWnd = GetDlgItem(IDC_STATIC_IMAGE); //获取控件指针，IDC_BUTTON1为控件ID号
	//if (pWnd!=NULL)
	//{
	//	pWnd->SetWindowPos(&wndTop,0,0,rect.Width(),rect.Height(),SWP_SHOWWINDOW); //把按钮移到窗口的(50,80)处

	//}
}


void CDlgImage::OnLButtonDown(UINT nFlags, CPoint point)
{
	CRect rect;
	GetWindowRect(&rect);
	ScreenToClient(rect);
	if (rect.PtInRect(point))
	{		
		Checkx=((gCols-1)*1.0/(rect.Width()-1))*(point.x-rect.left);
		Checky=((gRows-1)*1.0/(rect.Height()-1))*(point.y-rect.top);
	}

	CDialog::OnLButtonDown(nFlags, point);
}
void CDlgImage::OnMouseMove(UINT nFlags, CPoint point)
{
	CRect rect;
	GetWindowRect(&rect);
	ScreenToClient(rect);
	int nheigth = 1.0*rect.Width()*gRows / gCols;
	int noffset = (rect.Height() - nheigth) / 2;
	if (rect.PtInRect(point))
	{		
		if ((point.y - rect.top)<noffset || (point.y - rect.top)>(nheigth+ noffset))
		{
			return;
		}
		gPoint.x=((gCols-1)*1.0/(rect.Width()-1))*(point.x-rect.left);
		//gPoint.y=((gRows-1)*1.0/(rect.Height()-1))*(point.y-rect.top);
		gPoint.y = ((gRows - 1)*1.0 / (nheigth))*(point.y - rect.top- noffset);
	}

	CDialog::OnMouseMove(nFlags, point);
}
void CDlgImage::DrawBmp(BYTE * pBuffer/*,int NCols,int NRows*/)
{

	BYTE * pImgBuf=(BYTE *)pBuffer;;

	/*if (SpecPort==1)
	{
	for(int i=0 ; i<NRows; i++ )  
	{  
	memcpy(m_pFzBuffer,pBuffer+NCols*3*i,NCols*3);
	for (int j=0;j<NCols;j++)
	{
	pImgBuf[(i*NCols+j)*3+0]=m_pFzBuffer[(NCols-j-1)*3+0];
	pImgBuf[(i*NCols+j)*3+1]=m_pFzBuffer[(NCols-j-1)*3+1];
	pImgBuf[(i*NCols+j)*3+2]=m_pFzBuffer[(NCols-j-1)*3+2];
	}		
	}  
	}*/
	CRect rcClient;
	//CPaintDC dc(this);
	//GetWindowRect(&rcClient);
	//CWnd *pWnd=GetDlgItem(IDC_STATIC_IMAGE);
	CWnd *pWnd=this;
	pWnd->GetWindowRect(&rcClient);//
	int nDIBWidth/*=rcClient.right-rcClient.left*/;
	int nDIBHeight/*=rcClient.bottom-rcClient.top*/;
	CDC* pDC=pWnd->GetDC();
	int nOffsetX,nOffsetY;


	//Render Image//////////////////////////////////////////////////////////////////////////
	CDC memDC;	
	memDC.CreateCompatibleDC(pDC); 

	CDIB bmp;
	bmp.CreateDIB(gCols,gRows,24);
	CBitmap* pOldBitmap=memDC.SelectObject(CBitmap::FromHandle(HBITMAP(bmp)));
	pDC->SetStretchBltMode(COLORONCOLOR);
	pDC->SetStretchBltMode(HALFTONE);

	//Allocate the memory for image data
	BITMAP bitmap;
	bmp.GetBitmap(&bitmap);

	LONG lSizeOne;
	lSizeOne = bitmap.bmWidthBytes * bitmap.bmHeight;		
	bmp.SetDIBBits(lSizeOne,pImgBuf);	
	CPen pen(PS_SOLID,1,RGB(255,0,0)); ////建立一个画笔类对象，构造时设置画笔属性
	memDC.SelectObject(&pen);

	if (nMark==1)
	{
		memDC.MoveTo(Checkx,0);
		memDC.LineTo(Checkx,gRows);

		memDC.MoveTo(0,Checky);
		memDC.LineTo(gCols,Checky);

	}
	//pDC->StretchBlt(0,0,rcClient.Width(),rcClient.Height(),&memDC,0,0,gCols,gRows,SRCCOPY);
	int nheigth = 1.0*rcClient.Width()*gRows / gCols;
	pDC->StretchBlt(0, (rcClient.Height() - nheigth) / 2, rcClient.Width(), nheigth, &memDC, 0, 0, gCols, gRows, SRCCOPY);

	memDC.SelectObject(pOldBitmap);
	bmp.DeleteObject();
	memDC.DeleteDC();
	ReleaseDC(pDC);
	//CDialog::OnPaint();
}
void CDlgImage::SetMark(int bMark)
{
	nMark=bMark;
}
void CDlgImage::SetPicSize(int NCols,int NRows)
{
	gCols=NCols;
	gRows=NRows;
}
void CDlgImage::GetPoint(int *x,int *y)
{
	*x=gPoint.x;
	*y=gPoint.y;

}
void CDlgImage::OnDisplay()
{
	Invalidate(FALSE);
}
int CDlgImage::SavePicBmp()
{
	CDIB bmp;
	bmp.CreateDIB(gCols,gRows,24);

	LONG lSizeOne;
	lSizeOne = gCols * gRows*3;		
	bmp.SetDIBBits(lSizeOne,pBuffer);	

	LPTSTR pExtTab[]=
	{
		_T(""),
		_T(".bmp"),	
	};

	CFileDialog oDlgFile(FALSE,NULL,_T("未命名"),0,_T("BMP FILES(*.bmp)|*.bmp|"),this);

	BOOL bRet=FALSE;
	if(IDOK==oDlgFile.DoModal())
	{
		int nFilterIndex=oDlgFile.m_ofn.nFilterIndex;
		CString csPath=oDlgFile.GetPathName();
		CString csExt=oDlgFile.GetFileExt();
		if(csExt==_T(""))
		{
			csPath=csPath+pExtTab[nFilterIndex];
		}

		if(nFilterIndex==1)
		{//BMP
			bmp.SaveBmp(csPath.GetBuffer());
			bRet= TRUE;
		}
		else
		{
			bRet= FALSE;
		}
	}
	bmp.DeleteObject();
	return bRet;
}

// CDlgImage 消息处理程序
NS_MFC_Res_END