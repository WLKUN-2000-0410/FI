#pragma once


// SetSampleName 对话框

class SetSampleName : public CDialogEx
{
	DECLARE_DYNAMIC(SetSampleName)

public:
	SetSampleName(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~SetSampleName();

// 对话框数据
	enum { IDD = IDD_DIALOG3 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	CString GetNewName(){ return m_strName;}

private:
	CString m_strName;

};
