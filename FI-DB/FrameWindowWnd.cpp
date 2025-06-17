#include "stdafx.h"
#include "FrameWindowWnd.h"
#include "PublicInterface.h"
#include "TeeChart2011\tchart.h"
#include "TeeChart2011\series.h"
#include "FI-DBDlg.h"

CFrameWindowWnd::CFrameWindowWnd()
{
}


CFrameWindowWnd::~CFrameWindowWnd()
{
}

void CFrameWindowWnd::Notify(TNotifyUI& msg) {
	if (msg.sType == _T("click")) {
		if (msg.pSender->GetName() == _T("closebtn")) {
			Close();
		}
		else if (msg.pSender->GetName() == _T("selectFiles"))
		{
			// 选择添加数据文件
			selectFile();
		}
	}
}

void CFrameWindowWnd::selectFile()
{
	CFileDialog dlg(TRUE, NULL, NULL, OFN_ALLOWMULTISELECT,
		"TxtFile(*.txt)|*.txt|Csv(*.csv)|*.csv||");

	// Create buffer for file names.
	const DWORD numberOfFileNames = 1;
	const DWORD fileNameMaxLength = MAX_PATH + 1;
	const DWORD bufferSize = (numberOfFileNames * fileNameMaxLength) + 1;
	TCHAR* filenamesBuffer = new TCHAR[bufferSize];

	// Initialize beginning and end of buffer.
	filenamesBuffer[0] = NULL;//必à?须?的ì?
	filenamesBuffer[bufferSize - 1] = NULL;

	// Attach buffer to OPENFILENAME member.
	dlg.m_ofn.lpstrFile = filenamesBuffer;
	dlg.m_ofn.nMaxFile = bufferSize;

	if (dlg.DoModal() == IDOK)
	{
		// Retrieve file name(s).
		POSITION fileNamesPosition = dlg.GetStartPosition();

		if (fileNamesPosition != NULL)
		{
			CString fileName = dlg.GetNextPathName(fileNamesPosition);
			BOOL bSt = FALSE;
			int n = fileName.ReverseFind('.');
			if (n == -1)
			{
				return;
			}
			bSt = ImportFullLine(fileName);
		}
	}
}

BOOL CFrameWindowWnd::ImportFullLine(LPCTSTR txtFileName)
{
	CStdioFile file;
	CString txtFilename = txtFileName;
	int i;
	CString str, pattern = txtFilename.Right(3) == _T("csv") ? "," : "\t";

	if (!file.Open(txtFilename, CFile::modeRead | CFile::typeText))
	{
		return FALSE;
	}

	bool isTxt = false;
	// 如果是txt格式的，则需要兼容\t  , 和空格这三类分隔符
	if (txtFilename.Right(3) == _T("txt"))
		isTxt = true;

	CStringArray sa;
	long saCount = 0, nIndexCount = 0, Is3D = 0;
	CString strFileName = file.GetFileName().Left(file.GetFileName().GetLength() - 4);
	// 创建图表
	CFIDBDlg *fidbDlg = (CFIDBDlg*)AfxGetMainWnd();
	fidbDlg->m_schart.m_tchart.AddSeries(0);
	fidbDlg->m_schart.m_tchart.Series(0).SetActive(TRUE);
	while (file.ReadString(str))
	{
		int pos;
		CString tmp, tmpstr = str;
		tmpstr.TrimLeft();
		pos = tmpstr.Find(pattern);
		if (pos<1)
		{
			// 如果是txt, 需要添加判断pattern是否为 , 和 空格
			if (isTxt)
			{
				pattern = TEXT(",");
				pos = tmpstr.Find(pattern);
				if (pos<1)
				{
					pattern = TEXT(" ");
					pos = tmpstr.Find(pattern);
					if (pos<1)
						continue;
				}

			}
			//continue;
		}
		tmp = tmpstr.Left(pos);
		//  判断是否添加了表头，如果有表头，则跳过该行
		if (atof(tmp) == 0)
			continue;

		tmpstr = tmpstr.Mid(pos + 1);
		if (PublicInterface::IsNumber(tmp) && PublicInterface::IsNumber(tmpstr))
		{
			if (!tmp.IsEmpty() && PublicInterface::StrNumber(tmpstr) == 0)
			{
				saCount = 2;

				break;
			}
			return FALSE;
		}
	}
	if (saCount == 0) { return FALSE; }
	file.Seek(-str.GetLength() - 2, CFile::current);
	if (Is3D == 0)
	{
		while (file.ReadString(str))
		{
			PublicInterface::Split(str, pattern, sa);
			CString strtmpX, strtmpY;
			for (i = 0; i<saCount / 2; i++)
			{
				strtmpX = sa.GetAt(2 * i);
				strtmpY = sa.GetAt(2 * i + 1);
				if (!strtmpX.IsEmpty())
				{
					// 添加到数据表
					fidbDlg->m_schart.m_tchart.Series(0).AddXY(atof(strtmpX), atof(strtmpY), NULL, RGB(255,0,0));
					::OutputDebugStringA("添加数据当中");
				}
			}
			sa.RemoveAll();
		}
	}

	fidbDlg->m_schart.m_tchart.SetAutoRepaint(TRUE);
	file.Close();

	return TRUE;
}