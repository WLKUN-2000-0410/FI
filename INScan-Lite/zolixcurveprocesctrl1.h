#pragma once

// 计算机生成了由 Microsoft Visual C++ 创建的 IDispatch 包装类

// 注意: 不要修改此文件的内容。如果此类由
//  Microsoft Visual C++ 重新生成，您的修改将被覆盖。

/////////////////////////////////////////////////////////////////////////////
// CZolixcurveprocesctrl1 包装类

class CZolixcurveprocesctrl1 : public CWnd
{
protected:
	DECLARE_DYNCREATE(CZolixcurveprocesctrl1)
public:
	CLSID const& GetClsid()
	{
		static CLSID const clsid
			= { 0xF2057CEA, 0xD05, 0x48E4, { 0x97, 0x47, 0x6, 0xDC, 0xC, 0x3B, 0x66, 0xDF } };
		return clsid;
	}
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle,
						const RECT& rect, CWnd* pParentWnd, UINT nID, 
						CCreateContext* pContext = NULL)
	{ 
		return CreateControl(GetClsid(), lpszWindowName, dwStyle, rect, pParentWnd, nID); 
	}

    BOOL Create(LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, 
				UINT nID, CFile* pPersist = NULL, BOOL bStorage = FALSE,
				BSTR bstrLicKey = NULL)
	{ 
		return CreateControl(GetClsid(), lpszWindowName, dwStyle, rect, pParentWnd, nID,
		pPersist, bStorage, bstrLicKey); 
	}

// 特性
public:


// 操作
public:

// _DZolixCurveProcessing

// Functions
//

	void AboutBox()
	{
		InvokeHelper(DISPID_ABOUTBOX, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void ZolixProcessing(short Arraylength, double * XArray, double * YArray, short Degree)
	{
		static BYTE parms[] = VTS_I2 VTS_PR8 VTS_PR8 VTS_I2 ;
		InvokeHelper(0x1, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Arraylength, XArray, YArray, Degree);
	}

// Properties
//



};
