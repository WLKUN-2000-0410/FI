#include "stdafx.h"
#include "PublicInterface.h"


PublicInterface::PublicInterface()
{
}


PublicInterface::~PublicInterface()
{
}

BOOL PublicInterface::IsNumber(CString &str)
{
	int len;
	str.TrimLeft();
	str.TrimRight();
	len = str.GetLength();
	if (len == 0)
	{
		return TRUE;
	}

	TCHAR ch = str.GetAt(0);

	if ((ch != '-') && (ch != '+') && (ch != '.'))
	{
		if (ch<'0' || ch>'9')
		{
			return FALSE;
		}
	}
	for (int i = 0; i<len; i++)
	{
		ch = str.GetAt(i);
		if (ch<'0' || ch>'9')
		{
			switch (ch) {
			case 'E':
			case 'e':
			case '+':
			case '-':
			case '.':
			case '\t':
				continue;
			default:
				return FALSE;
			}
		}
	}
	return TRUE;
}

int PublicInterface::StrNumber(CString &str)
{
	int len;
	int nCount = 0;
	TCHAR ch = str.GetAt(0);
	len = str.GetLength();
	for (int i = 0; i<len; i++)
	{
		ch = str.GetAt(i);
		if (ch == '\t')
		{
			nCount++;
		}
	}
	return nCount;
}

BOOL PublicInterface::Split(CString content, CString pattern, CStringArray& strlist)
{
	int iPos = content.Find(pattern), iStart = 0;
	while (iPos != -1)
	{
		strlist.Add(content.Mid(iStart, iPos - iStart));
		iStart = iPos + pattern.GetLength();
		iPos = content.Find(pattern, iStart);
	}

	if (iStart <= content.GetLength())
		strlist.Add(content.Mid(iStart));
	return TRUE;
}
