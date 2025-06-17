// RoundButton.cpp : 实现文件
//

#include "stdafx.h"
#include "RoundButton.h"
#include "INScan-LiteDlg.h"
#include "SetSampleName.h"

// CRoundButton

IMPLEMENT_DYNAMIC(CRoundButton, CButton)

CRoundButton::CRoundButton()
{
	m_Pen.CreatePen(PS_SOLID, 1, RGB(201, 201, 233));  
	m_normalBrush.CreateSolidBrush(RGB(255, 255, 255));//正常状态下的按钮颜色  
	m_activeBrush.CreateSolidBrush(RGB(255, 255, 0));//按钮按下时当前的按钮颜色 
	m_OverBrush.CreateSolidBrush(RGB(0,191,255));//测试已完成时的按钮颜色 
	m_GroupBrush.CreateSolidBrush(RGB(192,192,192));//组按钮的按钮颜色 
	m_bCurrent = false;
	m_bScanOver = false;
	m_bGroup = false;
	m_bDef = true;
}

CRoundButton::~CRoundButton()
{
	m_Pen.DeleteObject();  
	m_normalBrush.DeleteObject();  
	m_activeBrush.DeleteObject(); 
}


BEGIN_MESSAGE_MAP(CRoundButton, CButton)
	ON_WM_RBUTTONUP()
	ON_COMMAND(ID_32787, &CRoundButton::On32787)
	ON_COMMAND(ID_32788, &CRoundButton::On32788)
	ON_WM_TIMER()
	ON_WM_LBUTTONDBLCLK()
END_MESSAGE_MAP()



// CRoundButton 消息处理程序


void CRoundButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	CRect rect;
	GetClientRect(rect);
	CDC dc;
	dc.Attach(lpDrawItemStruct->hDC);

	UINT state = lpDrawItemStruct->itemState;
	CRect focusRect(rect);
	if ((state & ODS_SELECTED)/* || (state & ODS_FOCUS)*/) {
		if(m_bCurrent)
			dc.SelectObject(&m_activeBrush);
	} else {
		if (m_bScanOver)
			dc.SelectObject(&m_OverBrush);
		else
		{
			if(m_bGroup)
				dc.SelectObject(&m_GroupBrush);
			else
				dc.SelectObject(&m_normalBrush);
		}
	}
	if(m_bCurrent)
		dc.SelectObject(&m_activeBrush);
	

	dc.DrawFocusRect(focusRect);
	dc.Ellipse(&focusRect);

	CString strText;
	GetWindowText(strText);
	if(m_bDef)
	{
		m_bDef = false;
		m_strNewName = m_strDefName = strText;
	}
	if(m_strNewName.GetLength() > 0 && m_strNewName.CompareNoCase(strText) != 0)
	{
		strText = m_strNewName;
	}
	dc.SetBkMode(TRANSPARENT);
	dc.SetTextColor(RGB(0, 0, 0));
	if (state & ODS_DISABLED) {
		dc.SetTextColor(RGB(172, 168, 153));
	}
	dc.DrawText(strText, rect, DT_CENTER | DT_SINGLELINE | DT_VCENTER);
	dc.Detach();
}

void CRoundButton::PreSubclassWindow()
{
	// TODO: 在此添加专用代码和/或调用基类
	ModifyStyle(0, BS_OWNERDRAW);//改为自绘风格  

	// 绘制按钮可用区域，切掉四个边角  
	CRgn rgn;  
	CRect rct;  
	GetClientRect(&rct);  
	rgn.CreateEllipticRgnIndirect(&rct);//在按钮矩形内创建椭圆区域  
	::SetWindowRgn(GetSafeHwnd(), (HRGN)rgn, true);//将椭圆区域应用到按钮上  

	CButton::PreSubclassWindow();
}

void CRoundButton::SetCurrentActive(bool val, int iStatus) 
{
	if (m_bCurrent != val)
	{
		m_bCurrent = val;
	}
	if(iStatus == 1)
	{
		m_bScanOver = true;
	}
	if(iStatus == -1)
	{
		m_bCurrent= false;
		m_bScanOver = false;
		m_bGroup = false;
	}
	Invalidate();

}


// void CRoundButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
// {
// 
// 	// TODO:  添加您的代码以绘制指定项
// 	// TODO:  添加您的代码以绘制指定项  
// 	CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);  
// 	int nSaveDC = pDC->SaveDC();//存储当前设备环境，以便绘图结束时恢复原来状态</span>  
// 	pDC->SelectObject(&m_normalBrush);//选择按钮正常状态（默认状态）下的画刷  
// 	pDC->SelectObject(&m_Pen);//选择画笔  
// 	CRect rct = lpDrawItemStruct->rcItem;//获取按钮矩形区域  
// 	if (lpDrawItemStruct->itemState&ODS_SELECTED)//绘制按钮按下时的颜色  
// 	{  
// 		pDC->SelectObject(&m_activeBrush);  
// 	}  
// 	//pDC->Ellipse(&rct);//画椭圆按钮，这一步用了之前选择的画笔和画刷  
// 	pDC->Rectangle(&rct);
// 
// 	//重绘字体  
// 	pDC->SetBkMode(TRANSPARENT);//重绘文本时不擦除背景即透明模式，如果选择OPAQUE（不透明），在文本四周有白色矩形边框，十分之难看  
// 	CString strText;//c++11版本以下不支持此方法  
// 	GetWindowText(strText);//获取按钮文本  
// 	pDC->DrawText(strText, rct, DT_CENTER | DT_VCENTER | DT_SINGLELINE);//重绘按钮文本  
// 
// 	//恢复设备环境  
// 	pDC->RestoreDC(nSaveDC);  
// }





void CRoundButton::OnRButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CMenu menu;
	menu.LoadMenuA(IDR_POPUP_SAMPLE);//加载菜单资源
    CMenu *pPopup=menu.GetSubMenu(0);
    ClientToScreen(&point);//将客户区坐标转换为屏幕坐标

    //显示右键菜单，由视类窗口拥有。
    pPopup->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON,point.x,point.y,this);
    //TrackPopupMenu的最后一个参数，可以为GetParent让父窗口既框架窗口拥有右键菜单，这样
    //框架类窗口获得了对右键菜单中菜单项的命令响应，

	CButton::OnRButtonUp(nFlags, point);
}


void CRoundButton::On32787()//样品谱图
{
	// TODO: 在此添加命令处理程序代码
	//this->GetWindowTextA(m_strCurName);
	m_strCurName = m_strNewName;
	CINScanLiteDlg * pDlg=(CINScanLiteDlg*)AfxGetApp()->GetMainWnd();
	if(!m_bScanOver)
		MessageBox("未扫描此样品，无法显示谱图！");
	else
		::PostMessageW(pDlg->m_hWnd, WM_SHOWLINE,(WPARAM)&m_strCurName, NULL);
}


void CRoundButton::On32788()//样品名称编辑
{
	// TODO: 在此添加命令处理程序代码
	//this->GetWindowTextA(m_strCurName);
	m_strCurName = m_strNewName;
	SetSampleName dlg;
	if(dlg.DoModal() == IDOK)
	{
		m_strNewName = dlg.GetNewName();
		this->SetWindowTextA(m_strNewName);
	}
	//else
	//	m_strNewName = m_strCurName;

	CINScanLiteDlg * pDlg=(CINScanLiteDlg*)AfxGetApp()->GetMainWnd();
	::PostMessageW(pDlg->m_hWnd, WM_UPDATENAME,(WPARAM)&m_strCurName, (LPARAM)&m_strNewName);
}

void CRoundButton::ResetBtn()
{
	m_strNewName = m_strDefName;
}


BOOL CRoundButton::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	/*if (pMsg->message == WM_LBUTTONDOWN)
    {
		m_iTimer = 0;
		this->SetTimer(3,1000,NULL);
    }
    else if (pMsg->message == WM_LBUTTONUP)
    {
        KillTimer(3);
    }*/

	return CButton::PreTranslateMessage(pMsg);
}


void CRoundButton::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	switch(nIDEvent)
	{
	case 3:
		{
			if(m_iTimer++ >= 2)
			{
				KillTimer(3);
				CMenu menu;
				menu.LoadMenuA(IDR_POPUP_SAMPLE);//加载菜单资源
				CMenu *pPopup=menu.GetSubMenu(0);
				CPoint   point;
				//GetCursorPos(&point);
				ClientToScreen(&point);//将客户区坐标转换为屏幕坐标

				//显示右键菜单，由视类窗口拥有。
				pPopup->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON,point.x,point.y,this);
			}
		}
		break;
	}
	CButton::OnTimer(nIDEvent);
}


void CRoundButton::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CMenu menu;
	menu.LoadMenuA(IDR_POPUP_SAMPLE);//加载菜单资源
    CMenu *pPopup=menu.GetSubMenu(0);
    ClientToScreen(&point);//将客户区坐标转换为屏幕坐标

    //显示右键菜单，由视类窗口拥有。
    pPopup->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON,point.x,point.y,this);
	CButton::OnLButtonDblClk(nFlags, point);
}
