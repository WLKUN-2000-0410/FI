#pragma once
class PublicInterface
{
public:
	PublicInterface();
	virtual ~PublicInterface();

	// 判断是否是数字
	static BOOL IsNumber(CString &str);

	// 将字符转化为整型
	static int StrNumber(CString &str);

	// 字符分割
	static BOOL Split(CString content, CString pattern, CStringArray& strlist);


};

