// SimpleLog.h : SimpleLog DLL 的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号


#ifndef _SIMPLELOG_H_
#define _SIMPLELOG_H_

#ifdef DLL_SIMPLELOG_EXPORT
#define DLL_SIMPLELOG_API __declspec(dllexport)
#else
#define DLL_SIMPLELOG_API __declspec(dllimport)
#endif

#ifdef __cplusplus
extern "C"{
#endif


//日志信息类型
enum LOG_LEVEL
{
	LL_NONE, //屏蔽所有日志信息
	LL_INFO, //普通
	LL_ERROR,//错误
	LL_WARN, //警告
	LL_DEBUG,//调试信息
	LL_ALL	 //所有信息
};

class DLL_SIMPLELOG_API SimpleLog
{
public:
	SimpleLog();
	/*
	logPath:log生成的路径
	logName:文件名称
	level：日志级别，默认LL_ALL
	isMutiFile：是否切分文件，默认true
	fileSize_MB：切分的文件大小，单位MB
	*/
	SimpleLog(const char * logPath, const char * logName,UINT level = LL_ALL,bool isMutiFile=true,DWORD fileSize_MB=10);
	SimpleLog(const wchar_t * logPath, const wchar_t * logName,UINT level = LL_ALL,bool isMutiFile=true,DWORD fileSize_MB=10);
	~SimpleLog();

	/*
	logPath:log生成的路径
	logName:文件名称
	level：日志级别，默认LL_ALL
	isMutiFile：是否切分文件，默认true
	fileSize_MB：切分的文件大小，单位MB
	*/
	bool SetLog(const char * logPath, const char * logName,UINT level = LL_ALL,bool isMutiFile=true,DWORD fileSize_MB=10);
	bool SetLog(const wchar_t * logPath, const wchar_t * logName,UINT level = LL_ALL,bool isMutiFile=true,DWORD fileSize_MB=10);
	bool WriteLog(UINT,const char    * pszFormat, ...);
	bool WriteLog(UINT,const wchar_t * pszFormat, ...);	

	bool WriteInfoLog(const char    * pszFormat, ...);
	bool WriteInfoLog(const wchar_t    * pszFormat, ...);
	bool WriteWarnLog(const char    * pszFormat, ...);
	bool WriteWarnLog(const wchar_t    * pszFormat, ...);
	bool WriteErrorLog(const char    * pszFormat, ...);
	bool WriteErrorLog(const wchar_t    * pszFormat, ...);
	bool WriteDebugLog(const char    * pszFormat, ...);
	bool WriteDebugLog(const wchar_t    * pszFormat, ...);
private:
	void init();
	bool log(UINT level,const char * info);
	bool WriteLog(UINT level,const char * pszFormat, va_list args);
	bool WriteLog(UINT level,const wchar_t * pszFormat, va_list args);
	bool CreateDir(const char * strDir);
	DWORD GetFileSize_Byte(char *);


	static LPTSTR GetExceptionString( DWORD dwCode );
	static BOOL GetLogicalAddress(PVOID addr, PTSTR szModule, DWORD len, DWORD& section, DWORD& offset );
	static SimpleLog * s_pSimpleLog;
	static LPTOP_LEVEL_EXCEPTION_FILTER s_PreviousFilter;
	static LONG WINAPI MSJUnhandledExceptionFilter(PEXCEPTION_POINTERS pExceptionInfo);
private:
	bool				m_IsMutiFile;
	DWORD				m_FileSize_MB;
	int					m_FileIndex;
	char				m_LogPath[MAX_PATH];
	char				m_LogName[MAX_PATH];
	UINT				m_LogLevel;
	CRITICAL_SECTION	m_cs;
};







#ifdef __cplusplus
}
#endif

#endif