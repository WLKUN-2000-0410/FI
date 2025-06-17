#include <string>
#include "math.h"

//#import "msado15.dll"	rename_namespace("ADOWE") rename("EOF","EndOfFile")
//#import "F:\\zddj\\FI-PC\\INScan-Lite\\dbado\\msado15.dll"	rename_namespace("ADOWE") rename("EOF","EndOfFile")
#import "D:\\01_WLKUN_PROJECT\\FI-PC\\INScan-Lite\\dbado\\msado15.dll"	rename_namespace("ADOWE") rename("EOF","EndOfFile")

using namespace ADOWE;

using namespace std;

class CDBAdo
{
//private:
public:
	_CommandPtr		m_ptrCommand;		//命令对象
	_RecordsetPtr	m_ptrRecordset;		//记录集对象
	_ConnectionPtr	m_ptrConnection;	//数据库对象

	static CString	m_strConnect;		//连接字符串
	CString			m_strErrorMsg;		//错误信息

	_RecordsetPtr& GetRecordSet(_bstr_t bstrSQL);

public:

	CDBAdo(void);
	~CDBAdo(void);

	bool	Init();
	void	UnInit();

	//初始化 COM
	void	ThreadComInit();//CoInitialize(NULL);

	//回收COM
	void	ThreadComUninit();//CoUninitialize();

	void	DetectResult(HRESULT hResult);
	void	RecordErrorMsg(_com_error comError);
	CString	GetLastError(){return m_strErrorMsg;}

	bool	CreateInstance();
	bool	SetConnectionString();
	bool	SetConnectionString(const string& strDBType, const string& strDBHost, const string& strDBName
				, const string& strDBPort, const string& strDBUser, const string& strDBPassWord);

	/**********************************************************
	功能：连接数据库
	参数：void
	返回：成功返回true 失败返回false
	主要思路：内置对象Open
	***********************************************************/ 
	bool	OpenConnection();

	/**********************************************************
	功能：关闭数据库连接
	参数：void
	返回：成功返回true 失败返回false
	主要思路：
	***********************************************************/ 
	bool	CloseConnection();

	//获取连接状态
	bool	IsConnecting();

	//清空参数
	void	ClearAllParameters();

	//增加参数
	void	AddParamter(LPCTSTR lpcsrName, ADOWE::ParameterDirectionEnum Direction, ADOWE::DataTypeEnum Type, long lSize, _variant_t & vtValue);
	void	AddParamter(LPCTSTR lpcsrName, ADOWE::ParameterDirectionEnum Direction, ADOWE::DataTypeEnum Type, long lSize);


	//设置sql语句
	void	SetSPName(LPCTSTR lpcsrSPName);

	/**********************************************************
   功能：参数化执行sql语句
   参数：in lpcsrSPName：格式化后的sql语句、 bIsRecordset：true返回数据集 false不返回数据集
   返回：成功返回true 失败返回false
  主要思路：
   ***********************************************************/ 
	bool	ExecuteCommand(LPCTSTR lpcsrSPName,bool bIsRecordset);


	/**********************************************************
	功能：执行格式化的sql语句
	参数：in 格式化后的sql语句
	返回：成功返回true 失败返回false
	主要思路：
	***********************************************************/ 
	bool	Execute(LPCTSTR lpcsrCommand);

	long	GetReturnValue();

	/**********************************************************
	功能：根据格式化的sql语句打开填充数据集
	参数：in 格式化后的sql语句
	返回：成功返回true 失败返回false
	主要思路：
	***********************************************************/ 
	bool	OpenRecordset(LPCTSTR lpcsrSPName);
	bool	CloseRecordset();
	bool	IsRecordsetOpened();
	bool	IsEndRecordset();

	/**********************************************************
	功能：游标下移
	参数：void
	返回：void
	主要思路：
	***********************************************************/ 
	void	MoveToNext();

	void	MoveToFirst();
	void	MoveToLast();

	/**********************************************************
	功能：获取数据集中的总行数
	参数：void
	返回：long型数据集中记录行数
	主要思路：
	***********************************************************/ 
	long	GetRecordCount();


/**********************************************************
   功能：char数组转VARIANT,用于图片保存数据库
   参数：in  a_pData： char数组指针变量，len：char数组长度
   返回：VARIANT
  主要思路：
***********************************************************/ 
	VARIANT CharToVARIANT(char *strUtf8 ,int& len); 

	/**********************************************************
	功能：重载函数，根据字段名，数据类型获取数据集中的数据
	参数：in  sql语句中的字段名
	out 返回数据集中的值
	返回：成功返回true 失败返回false
	主要思路：
	***********************************************************/ 
	bool	GetFieldValue(LPCTSTR lpcsrFieldName, WORD& wValue);
	bool	GetFieldValue(LPCTSTR lpcsrFieldName, CString& strValue);
	bool	GetFieldValue(LPCTSTR lpcsrFieldName, string& strValue);
	bool	GetFieldValue(LPCTSTR lpcsrFieldName, INT& nValue);
	bool	GetFieldValue(LPCTSTR lpcsrFieldName, BYTE& bValue);
	bool	GetFieldValue(LPCTSTR lpcsrFieldName, LONG& lValue);
	bool	GetFieldValue(LPCTSTR lpcsrFieldName, DWORD& dwValue);
	bool	GetFieldValue(LPCTSTR lpcsrFieldName, UINT& ulValue);
	bool	GetFieldValue(LPCTSTR lpcsrFieldName, DOUBLE& dbValue);
	bool	GetFieldValue(LPCTSTR lpcsrFieldName, __int64& llValue);
	bool	GetFieldValue(LPCTSTR lpcsrFieldName, COleDateTime& Time);
	bool	GetFieldValue(LPCTSTR lpcsrFieldName, bool& bValue);
	bool	GetFieldValue(LPCTSTR lpcsrFieldName, _variant_t& varBLOB,int& len);//图片读取

};
