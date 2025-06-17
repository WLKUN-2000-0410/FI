#pragma once

// 计算机生成了由 Microsoft Visual C++ 创建的 IDispatch 包装类

// 注意: 不要修改此文件的内容。如果此类由
//  Microsoft Visual C++ 重新生成，您的修改将被覆盖。

/////////////////////////////////////////////////////////////////////////////
// CZlanguagectrl1 包装类

class CZlanguagectrl1 : public CWnd
{
protected:
	DECLARE_DYNCREATE(CZlanguagectrl1)
public:
	CLSID const& GetClsid()
	{
		static CLSID const clsid
			= { 0x1FA8F9F4, 0xBDD6, 0x4BE7, { 0x97, 0x6C, 0xDD, 0x8D, 0x3C, 0xBA, 0xC3, 0xF5 } };
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

// _DZLanguage

// Functions
//

	BOOL Init(LPCTSTR SettingIniPath)
	{
		BOOL result;
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x2, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, SettingIniPath);
		return result;
	}
	short GetLanguageCount()
	{
		short result;
		InvokeHelper(0x3, DISPATCH_METHOD, VT_I2, (void*)&result, NULL);
		return result;
	}
	CString GetLanguageName(short index)
	{
		CString result;
		static BYTE parms[] = VTS_I2 ;
		InvokeHelper(0x4, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms, index);
		return result;
	}
	BOOL LoadLanguage(LPCTSTR LanguageName)
	{
		BOOL result;
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x5, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, LanguageName);
		return result;
	}
	CString GetCurrentLanguageFilename()
	{
		CString result;
		InvokeHelper(0x6, DISPATCH_METHOD, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	BOOL SetDialogStrings(long DialogPoint, long DialogID)
	{
		BOOL result;
		static BYTE parms[] = VTS_I4 VTS_I4 ;
		InvokeHelper(0x7, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, DialogPoint, DialogID);
		return result;
	}
	CString LoadString(LPCTSTR strKeyID, LPCTSTR strDefault)
	{
		CString result;
		static BYTE parms[] = VTS_BSTR VTS_BSTR ;
		InvokeHelper(0x8, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms, strKeyID, strDefault);
		return result;
	}
	long LoadSettingInt(LPCTSTR szSection, LPCTSTR szKeyID, long intDefault)
	{
		long result;
		static BYTE parms[] = VTS_BSTR VTS_BSTR VTS_I4 ;
		InvokeHelper(0x9, DISPATCH_METHOD, VT_I4, (void*)&result, parms, szSection, szKeyID, intDefault);
		return result;
	}
	BOOL SaveSettingInt(LPCTSTR szSection, LPCTSTR szKeyID, long Intvalue)
	{
		BOOL result;
		static BYTE parms[] = VTS_BSTR VTS_BSTR VTS_I4 ;
		InvokeHelper(0xa, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, szSection, szKeyID, Intvalue);
		return result;
	}
	CString LoadSettingString(LPCTSTR szSection, LPCTSTR szKeyID, LPCTSTR szDefault)
	{
		CString result;
		static BYTE parms[] = VTS_BSTR VTS_BSTR VTS_BSTR ;
		InvokeHelper(0xb, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms, szSection, szKeyID, szDefault);
		return result;
	}
	BOOL SaveSettingString(LPCTSTR szSection, LPCTSTR szKeyID, LPCTSTR szValue)
	{
		BOOL result;
		static BYTE parms[] = VTS_BSTR VTS_BSTR VTS_BSTR ;
		InvokeHelper(0xc, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, szSection, szKeyID, szValue);
		return result;
	}
	double LoadSettingDouble(LPCTSTR szSection, LPCTSTR szKeyID, double dblDefault)
	{
		double result;
		static BYTE parms[] = VTS_BSTR VTS_BSTR VTS_R8 ;
		InvokeHelper(0xd, DISPATCH_METHOD, VT_R8, (void*)&result, parms, szSection, szKeyID, dblDefault);
		return result;
	}
	BOOL SaveSettingDouble(LPCTSTR szSection, LPCTSTR szKeyID, double dblValue)
	{
		BOOL result;
		static BYTE parms[] = VTS_BSTR VTS_BSTR VTS_R8 ;
		InvokeHelper(0xe, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, szSection, szKeyID, dblValue);
		return result;
	}
	BOOL LoadLanguageByIndex(short languageIndex)
	{
		BOOL result;
		static BYTE parms[] = VTS_I2 ;
		InvokeHelper(0xf, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, languageIndex);
		return result;
	}
	void AboutBox()
	{
		InvokeHelper(DISPID_ABOUTBOX, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}

// Properties
//

long GetReadyState()
{
	long result;
	GetProperty(DISPID_READYSTATE, VT_I4, (void*)&result);
	return result;
}
void SetReadyState(long propVal)
{
	SetProperty(DISPID_READYSTATE, VT_I4, propVal);
}
BOOL GetCreateLanguageState()
{
	BOOL result;
	GetProperty(0x1, VT_BOOL, (void*)&result);
	return result;
}
void SetCreateLanguageState(BOOL propVal)
{
	SetProperty(0x1, VT_BOOL, propVal);
}


};
