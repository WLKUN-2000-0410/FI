#pragma once


class CFrameWindowWnd : public WindowImplBase
{
public:
	explicit CFrameWindowWnd();
	~CFrameWindowWnd();
	LPCTSTR GetWindowClassName() const { return _T("UIMainFrame"); };




	virtual CDuiString GetSkinFile() //  主界面 XML
	{
		return _T("./ui/main.xml");
	}

	virtual CDuiString GetSkinFolder()
	{
		return _T("");
	}
	
	void OnFinalMessage(HWND /*hWnd*/) { 
		// delete this; 
	};

	// duilib消息通知
	void Notify(TNotifyUI& msg);

	void Close(UINT nRet = IDOK)
	{
		CWindowWnd::Close(nRet);
	}

	// 选择添加文件
	void selectFile();


	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		if (uMsg == WM_CREATE) {
			/*
			*/
			// 加载当前界面
			m_pm.Init(m_hWnd);
			CDialogBuilder builder;
			CControlUI* pRoot = builder.Create(_T("./ui/main.xml"), (UINT)0, NULL, &m_pm);
			ASSERT(pRoot && "Failed to parse XML");
			m_pm.AttachDialog(pRoot);
			m_pm.AddNotifier(this);
			return 0;
		}
		//屏蔽系统标题栏
		else if (uMsg == WM_NCACTIVATE)
		{
			if (!IsIconic(m_hWnd))
			{
				return (wParam == 0) ? TRUE : FALSE;
			}
		}
		else if (uMsg == WM_DESTROY) {
			::PostQuitMessage(0);
		}
		LRESULT lRes = 0;
		if (m_pm.MessageHandler(uMsg, wParam, lParam, lRes)) return lRes;
		return CWindowWnd::HandleMessage(uMsg, wParam, lParam);
	}

	// 导入选择要入库的文件
	BOOL ImportFullLine(LPCTSTR txtFileName);
public:
	CPaintManagerUI m_pm;

};

