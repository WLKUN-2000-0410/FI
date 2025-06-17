#include "stdafx.h"
#include "DBAdo.h"

CString	CDBAdo::m_strConnect="";		//连接字符串，使用静态变量免得多次读取ini配置文件

CDBAdo::CDBAdo(void)
{
	Init();
}

CDBAdo::~CDBAdo(void)
{
	UnInit();
}


bool CDBAdo::Init()
{

	m_strErrorMsg = "";		//错误信息

	//设置连接字符串
	if (!SetConnectionString())
	{
		return false;
	}

	//初始化 COM
	CoInitialize(NULL);

	//创建对象
	return CreateInstance();
}

void CDBAdo::UnInit()
{
	//关闭连接
	CloseConnection();

	//释放对象
	if(NULL != m_ptrCommand)
	{
		m_ptrCommand.Release();
		m_ptrCommand = NULL;
	}

	if(NULL != m_ptrRecordset)
	{
		m_ptrRecordset.Release();
		m_ptrRecordset = NULL;
	}

	if(NULL != m_ptrConnection)
	{
		m_ptrConnection.Release();
		m_ptrConnection = NULL;
	}

	//释放COM
	CoUninitialize();
}


//初始化 COM CoInitialize(NULL);
void CDBAdo::ThreadComInit()
{
	 CoInitialize(NULL);
}

//回收COM CoUninitialize();
void CDBAdo::ThreadComUninit()
{
	CoUninitialize();
}

/**********************************************************
   功能：创建对象 
   参数：void
   返回：全部成功返回true 有一个失败返回false
  主要思路：CreateInstance
***********************************************************/ 
bool CDBAdo::CreateInstance()
{
	//释放对象
	if(NULL != m_ptrCommand)
	{
		m_ptrCommand.Release();
		m_ptrCommand = NULL;
	}

	if(NULL != m_ptrRecordset)
	{
		m_ptrRecordset.Release();
		m_ptrRecordset = NULL;
	}

	if(NULL != m_ptrConnection)
	{
		m_ptrConnection.Release();
		m_ptrConnection = NULL;
	}

	//建立新对象
	m_ptrCommand.CreateInstance(__uuidof(Command));
	m_ptrRecordset.CreateInstance(__uuidof(Recordset));
	m_ptrConnection.CreateInstance(__uuidof(Connection));

	if(m_ptrCommand==NULL)
	{
		m_strErrorMsg	= _T("数据库命令对象创建失败！");
		return false;
	}

	if(m_ptrRecordset==NULL)
	{
		m_strErrorMsg	= _T("数据库记录集对象创建失败！");
		return false;
	}

	if(m_ptrConnection==NULL)
	{
		m_strErrorMsg	= _T("数据库连接对象创建失败！");
		return false;
	}

	//设置变量
	m_ptrCommand->CommandType	= adCmdStoredProc;
	return	true;
}


void CDBAdo::DetectResult(HRESULT hResult)
{
	if(FAILED(hResult))
		_com_issue_error(hResult);
}


void CDBAdo::RecordErrorMsg(_com_error comError)
{
	_bstr_t	bstrDescribe(comError.Description());

	m_strErrorMsg.Format(TEXT("ADO 错误：0x%8x，%s"), comError.Error(), (LPCTSTR)bstrDescribe);
}


/**********************************************************
   功能：数据库信息以参数形式赋值，拼接数据库连接字符串 
***********************************************************/ 
bool CDBAdo::SetConnectionString(const string& strDBType, const string& strDBHost, const string& strDBName
								 , const string& strDBPort, const string& strDBUser, const string& strDBPassWord)
{
	m_strConnect = "";
	if (strDBType=="MySql")
	{
		m_strConnect.Format(_T("Driver={MySQL ODBC 5.1 Driver};Server=%s; Port=%s;Option=4;Database=%s;User ID=%s;Password=%s")
			, (CString)(strDBHost.c_str())
			, (CString)(strDBPort.c_str())
			, (CString)(strDBName.c_str())
			, (CString)(strDBUser.c_str())
			, (CString)(strDBPassWord.c_str()));
	}
	else if (strDBType=="Oracle")
	{
		m_strConnect.Format(_T("Driver={Microsoft ODBC for Oracle};Server=%s;Uid=%s;Pwd=%s;"),strDBName.c_str(),strDBUser.c_str(),strDBPassWord.c_str());

	}
	else if (strDBType=="MsSql")
	{

	}
	else
	{
		m_strErrorMsg.Format("Could not find DBType : %s", strDBType.c_str());
		return false;
	}

	if ("" == m_strConnect)
	{
		m_strErrorMsg = "Set Connect String Failt";
		return false;
	}

	return true;
}

/**********************************************************
   功能：获取配置文件的数据库信息，拼接数据库连接字符串 
   参数：void
   返回：成功返回true 失败返回false
  主要思路：读取config.ini配置文件,判断数据库类型，组成不同的连接字符串
***********************************************************/ 
bool CDBAdo::SetConnectionString()
{
	if (m_strConnect.IsEmpty())
	{
		char buffer[512] = "";
		GetModuleFileNameA(NULL, buffer, _countof( buffer ) );
		CStringA sPath = buffer;
		int iPostion = sPath.ReverseFind( '\\');
		sPath = sPath.Left( iPostion );
		sPath += "\\config.ini";  

		char DBType[50]={0},DBName[50]={0},DBHost[50]={0}, DBPort[50]={0}, DBUser[50]={0},DBPassword[50]={0};
		GetPrivateProfileStringA ("DataBase", "DBType", "", DBType, 50, sPath);   
		GetPrivateProfileStringA ("DataBase", "DBName", "", DBName, 50, sPath); 
		GetPrivateProfileStringA ("DataBase", "DBHost", "", DBHost, 50, sPath); 
		GetPrivateProfileStringA ("DataBase", "DBPort", "", DBPort, 50, sPath);   
		GetPrivateProfileStringA ("DataBase", "DBUser", "", DBUser, 50, sPath); 
		GetPrivateProfileStringA ("DataBase", "DBPassword", "", DBPassword, 50, sPath); 

		if (strcmp(DBType,"MySql")==0)
		{
			m_strConnect.Format(_T("Driver={MySQL ODBC 5.1 Driver};Server=%s; Port=%s;Option=4;Database=%s;User ID=%s;Password=%s;allowMultiQueries=true")
				,(CString)DBHost,(CString)DBPort,(CString)DBName,(CString)DBUser,(CString)DBPassword);
		}

		if (strcmp(DBType,"MsSql")==0)
		{
			m_strConnect.Format(_T("Driver={SQL Server};Server=%s;Uid=%s;Pwd=%s;Database=%s"),(CString)DBHost,(CString)DBPort,(CString)DBName,(CString)DBUser,(CString)DBPassword);
		}

		if (strcmp(DBType,"Oracle")==0)
		{
			m_strConnect.Format(_T("Driver={Microsoft ODBC for Oracle};Server=%s;Uid=%s;Pwd=%s;"),(CString)DBName,(CString)DBUser,(CString)DBPassword);

		}

	}

	if (m_strConnect.IsEmpty())
	{
		AfxMessageBox(_T("数据库连接字符串生成失败,请检查config.ini文件中DBType(MySql、MsSql、Oracle中的一项)是否已经配置正确！"));
		return false;
	}
	else
	{
		return true;

	}
}


/**********************************************************
   功能：连接数据库
   参数：void
   返回：成功返回true 失败返回false
  主要思路：内置对象Open
***********************************************************/ 
bool CDBAdo::OpenConnection()
{
	if (m_strConnect.IsEmpty())
	{
		m_strErrorMsg = "Set Connect String Failt";
		return false;
	}

	try
	{
		//关闭连接
		CloseConnection();

		//连接数据库
		DetectResult(m_ptrConnection->Open(_bstr_t(m_strConnect), "", "", adModeUnknown));
		m_ptrConnection->CursorLocation	= adUseClient;
		m_ptrCommand->ActiveConnection	= m_ptrConnection;

		return true;
	}
	catch(_com_error& comError) 
	{
		RecordErrorMsg(comError);
	}

	return false;
}

/**********************************************************
   功能：关闭数据库连接
   参数：void
   返回：成功返回true 失败返回false
  主要思路：
***********************************************************/ 
bool CDBAdo::CloseConnection()
{
	try
	{
		CloseRecordset();
		if((m_ptrConnection!=NULL)&&(m_ptrConnection->GetState()!=adStateClosed))
			DetectResult(m_ptrConnection->Close());

		return true;
	}
	catch(_com_error& comError)
	{
		RecordErrorMsg(comError);
	}

	return false;
}

/**********************************************************
   功能：获取数据库连接状态
   参数：void
   返回：连接状态返回true 断开状态返回false
  主要思路：
***********************************************************/ 
bool CDBAdo::IsConnecting()
{
	try 
	{
		//状态判断
		if(m_ptrConnection==NULL)
			return	false;
		if(m_ptrConnection->GetState()==adStateClosed)
			return	false;

		//参数判断
		long	lErrorCount	= m_ptrConnection->Errors->Count;
		if(lErrorCount>0L)
		{
			ErrorPtr	pError	 = NULL;
			for(long i=0; i<lErrorCount; i++)
			{
				pError	= m_ptrConnection->Errors->GetItem(i);
				if(pError->Number==0x80004005)
					return	false;
			}
		}

		return true;
	}
	catch(_com_error& comError)
	{
		RecordErrorMsg(comError);
	}

	return false;
}

/**********************************************************
   功能：根据格式化的sql语句打开填充数据集
   参数：in 格式化后的sql语句
   返回：成功返回true 失败返回false
  主要思路：
***********************************************************/ 
bool CDBAdo::OpenRecordset(LPCTSTR lpcsrSPName)
{
	try
	{
		if(!IsConnecting())
		{
          if (!OpenConnection())
          {
			  return false;
          }
		}
		//关闭记录集
		CloseRecordset();

		m_ptrRecordset->Open(lpcsrSPName, m_ptrConnection.GetInterfacePtr(), adOpenDynamic, adLockOptimistic, adCmdText);

		return true;
		
	}
	catch(_com_error& comError)
	{
		RecordErrorMsg(comError);
	}

	return	false;
}

bool CDBAdo::CloseRecordset()
{
	try
	{
		if(IsRecordsetOpened())
			DetectResult(m_ptrRecordset->Close());

		//*************
		//m_ptrRecordset.Release();
		//*************

		return true;
	}
	catch(_com_error& comError)
	{
		RecordErrorMsg(comError);
	}

	return false;
}

bool CDBAdo::IsRecordsetOpened()
{
	if(m_ptrRecordset==NULL)
		return	false;
	if(m_ptrRecordset->GetState()==adStateClosed)
		return	false;

	return true;
}

bool CDBAdo::IsEndRecordset()
{
	try 
	{
		return (m_ptrRecordset->EndOfFile==VARIANT_TRUE);
	}
	catch(_com_error& comError)
	{
		RecordErrorMsg(comError);
	}

	return true;
}


/**********************************************************
   功能：游标下移
   参数：void
   返回：void
  主要思路：
***********************************************************/ 
void CDBAdo::MoveToNext()
{
	try 
	{ 
		m_ptrRecordset->MoveNext(); 
	}
	catch(_com_error& comError)
	{
		RecordErrorMsg(comError);
	}
}

/**********************************************************
   功能：游标移到第一行记录
   参数：void
   返回：void
  主要思路：
***********************************************************/ 
void CDBAdo::MoveToFirst()
{
	try 
	{ 
		m_ptrRecordset->MoveFirst(); 
	}
	catch(_com_error& comError)
	{
		RecordErrorMsg(comError);
	}
}

/**********************************************************
   功能：游标移到最后一行记录
   参数：void
   返回：void
  主要思路：
***********************************************************/ 
void CDBAdo::MoveToLast()
{
	try 
	{ 
		m_ptrRecordset->MoveLast(); 
	}
	catch(_com_error& comError)
	{
		RecordErrorMsg(comError);
	}
}

/**********************************************************
   功能：获取数据集中的总行数
   参数：void
   返回：long型数据集中记录行数
  主要思路：
***********************************************************/ 
long CDBAdo::GetRecordCount()
{
	try
	{
		if(m_ptrRecordset==NULL)
			return	0;
		return	m_ptrRecordset->GetRecordCount();
	}
	catch(_com_error& comError)
	{
		RecordErrorMsg(comError);
	}

	return 0;
}

/**********************************************************
   功能：清空参数
   参数：void
   返回：void
  主要思路：
***********************************************************/ 
void CDBAdo::ClearAllParameters()
{
	try 
	{
		long	lParamCount	= m_ptrCommand->Parameters->Count;
		if(lParamCount>0L)
		{
			for(long i=lParamCount; i>0; i--)
			{
				_variant_t	vtIndex;

				vtIndex.intVal	= i-1;
				m_ptrCommand->Parameters->Delete(vtIndex);
			}
		}
	}
	catch(_com_error& comError)
	{
		RecordErrorMsg(comError);
	}
}

/**********************************************************
   功能：添加参数，ExecuteCommand(bool bIsRecordset)前使用
   参数：lpcsrName 参数名称，Direction 输入还是输出，Type数据库类型 ，lSize长度 ，vtValue参数值
   返回：viod
   主要思路：
***********************************************************/ 
void CDBAdo::AddParamter(LPCTSTR lpcsrName, ADOWE::ParameterDirectionEnum Direction, ADOWE::DataTypeEnum Type, long lSize, _variant_t & vtValue)
{
	ASSERT(lpcsrName!=NULL);
	try 
	{
		_ParameterPtr	Parameter	= m_ptrCommand->CreateParameter(lpcsrName, Type, Direction, lSize,vtValue);
		m_ptrCommand->Parameters->Append(Parameter);
	}
	catch(_com_error& comError)
	{
		RecordErrorMsg(comError);
	}
}


void CDBAdo::AddParamter(LPCTSTR lpcsrName, ADOWE::ParameterDirectionEnum Direction, ADOWE::DataTypeEnum Type, long lSize)
{
	ASSERT(lpcsrName!=NULL);
	try 
	{
		_ParameterPtr	Parameter	= m_ptrCommand->CreateParameter(lpcsrName, Type, Direction, lSize);
		m_ptrCommand->Parameters->Append(Parameter);
	}
	catch(_com_error& comError)
	{
		RecordErrorMsg(comError);
	}
}
/**********************************************************
   功能：设置m_ptrCommand需要执行的sql语句，ExecuteCommand(bool bIsRecordset)前使用
   参数：in 格式化的sql语句
   返回：viod
   主要思路：
***********************************************************/ 
void CDBAdo::SetSPName(LPCTSTR lpcsrSPName)
{
	try 
	{ 
		m_ptrCommand->CommandText	= lpcsrSPName; 
	}
	catch(_com_error& comError)
	{
		RecordErrorMsg(comError);
	}
}
/**********************************************************
   功能：参数化执行sql语句
   参数：in lpcsrSPName：格式化后的sql语句、 bIsRecordset：true返回数据集 false不返回数据集
   返回：成功返回true 失败返回false
  主要思路：
***********************************************************/ 
bool CDBAdo::ExecuteCommand(LPCTSTR lpcsrSPName,bool bIsRecordset)
{
	try 
	{
		if(!IsConnecting())
		{
			if (!OpenConnection())
			{
				return false;
			}
		}
		//关闭记录集
		CloseRecordset();
		//执行命令
		if(bIsRecordset)
		{
			m_ptrCommand->CommandText	= lpcsrSPName; 
			m_ptrRecordset->PutRefSource(m_ptrCommand);
			m_ptrRecordset->CursorLocation	= adUseClient;
			DetectResult(m_ptrRecordset->Open((IDispatch*)m_ptrCommand, vtMissing, adOpenForwardOnly, adLockReadOnly, adOptionUnspecified));
		}else 
		{
			m_ptrCommand->CommandText	= lpcsrSPName; 
			m_ptrConnection->CursorLocation	= adUseClient;
			_variant_t vtEmpty (DISP_E_PARAMNOTFOUND, VT_ERROR);
			_variant_t vNULL;
			DetectResult(m_ptrCommand->Execute(&vNULL,&vtEmpty,adCmdText));
		}
		return	true;
	}
	catch(_com_error& comError)
	{
		RecordErrorMsg(comError);
	}

	return	false;
}


/**********************************************************
   功能：执行格式化的sql语句
   参数：in 格式化后的sql语句
   返回：成功返回true 失败返回false
  主要思路：
***********************************************************/ 
bool CDBAdo::Execute(LPCTSTR lpcsrCommand)
{
	try
	{
		if(!IsConnecting())
		{
			if (!OpenConnection())
			{
				return false;
			}
		}
		m_ptrConnection->CursorLocation	= adUseClient;
		m_ptrConnection->Execute(lpcsrCommand, NULL, adExecuteNoRecords);
		return	true;
	}
	catch(_com_error& comError)
	{
		RecordErrorMsg(comError);
	}

	return	false;
}

long CDBAdo::GetReturnValue()
{
	try 
	{
		_ParameterPtr	ptrParam;
		long			lParameterCount	= m_ptrCommand->Parameters->Count;
		for(long i=0; i<lParameterCount; i++)
		{
			ptrParam	= m_ptrCommand->Parameters->Item[i];
			if(ptrParam->Direction==adParamReturnValue)
				return	ptrParam->Value.lVal;
		}
	}
	catch(_com_error& comError)
	{
		RecordErrorMsg(comError);
	}

	return	0;
}


/**********************************************************
   功能：char数组转VARIANT,用于图片保存数据库
   参数：in  a_pData： char数组指针变量，len：char数组长度
   返回：VARIANT
  主要思路：
***********************************************************/ 
VARIANT CDBAdo::CharToVARIANT(char *a_pData ,int& len)
{
	SAFEARRAY *psa;
	SAFEARRAYBOUND rgsabound[1];
	rgsabound[0].lLbound = 0;  
	rgsabound[0].cElements =len;  
	psa = SafeArrayCreate(VT_UI1, 1, rgsabound);                 ///创建SAFEARRAY对象 
	unsigned char* pSafePtr;
	if (NOERROR ==SafeArrayAccessData(psa, (void**)&pSafePtr))
	{
		memcpy(pSafePtr,a_pData, len);
	}
	SafeArrayUnaccessData(psa);

	VARIANT  varBLOB;
	varBLOB.vt = VT_ARRAY | VT_UI1;               ///将varBLOB的类型设置为BYTE类型的数组   
	varBLOB.parray = psa;   
	return varBLOB;
}

/**********************************************************
   功能：重载函数，根据字段名，数据类型获取数据集中的数据
   参数：in  sql语句中的字段名
         out 返回数据集中的值
   返回：成功返回true 失败返回false
  主要思路：
***********************************************************/ 
bool CDBAdo::GetFieldValue(LPCTSTR lpcsrFieldName, WORD& wValue)
{
	wValue	= 0L;
	try
	{
		_variant_t	vtFld	= m_ptrRecordset->Fields->GetItem(lpcsrFieldName)->Value;
		if((vtFld.vt!=VT_NULL)&&(vtFld.vt!=VT_EMPTY))
			wValue	= (WORD)vtFld.uiVal;

		return true;
	}
	catch(_com_error& comError)
	{
		RecordErrorMsg(comError);
	}

	return false;
}

/**********************************************************
   功能：重载函数，根据字段名，数据类型获取数据集中的数据
   参数：in  sql语句中的字段名
         out 返回数据集中的值
   返回：成功返回true 失败返回false
  主要思路：
***********************************************************/ 
bool CDBAdo::GetFieldValue(LPCTSTR lpcsrFieldName, CString& strValue)
{
	try
	{
		_variant_t vtFld	= m_ptrRecordset->Fields->GetItem(lpcsrFieldName)->Value;
		if(vtFld.vt==VT_BSTR)
		{
			strValue	= (char*)_bstr_t(vtFld);
			strValue.TrimLeft();
			return	true;
		}
		return	false;
	}
	catch(_com_error& comError)
	{
		RecordErrorMsg(comError);
	}

	return	false;
}

/**********************************************************
   功能：重载函数，根据字段名，数据类型获取数据集中的数据
   参数：in  sql语句中的字段名
         out 返回数据集中的值
   返回：成功返回true 失败返回false
  主要思路：
***********************************************************/ 
bool CDBAdo::GetFieldValue(LPCTSTR lpcsrFieldName, string& strValue)
{
	try
	{
		_variant_t vtFld	= m_ptrRecordset->Fields->GetItem(lpcsrFieldName)->Value;
		switch(vtFld.vt)
		{
		case VT_BSTR:
			{
				strValue= (const char*)_bstr_t(vtFld);
				//...这里应去掉首尾空格
				break;
			}		
		case VT_DATE:
			{
				_bstr_t val(vtFld);
				strValue=(const char*)_bstr_t(vtFld);
				break;
			}
		case 8209:
			{
				//strValue=(const char*)_bstr_t(vtFld);
				strValue=(const char*)(vtFld.parray->pvData);
				break;
			}
		default: strValue	="";

		}
		return true;
	}
	catch(_com_error& comError)
	{
		RecordErrorMsg(comError);
	}

	return	false;
}

/**********************************************************
   功能：重载函数，根据字段名，数据类型获取数据集中的数据
   参数：in  sql语句中的字段名
         out 返回数据集中的值
   返回：成功返回true 失败返回false
  主要思路：
***********************************************************/ 
bool CDBAdo::GetFieldValue(LPCTSTR lpcsrFieldName, INT& nValue)
{
	nValue	= 0;
	try
	{
		_variant_t vtFld = m_ptrRecordset->Fields->GetItem(lpcsrFieldName)->Value;
		switch(vtFld.vt)
		{
		case VT_BOOL:
			{
				nValue	= vtFld.boolVal;
				break;
			}
		case VT_I2:
		case VT_UI1:
			{
				nValue	= vtFld.iVal;
				break;
			}
		case VT_NULL:
		case VT_EMPTY:
			{
				nValue	= 0;
				break;
			}
		default: nValue	= vtFld.iVal;
		}	
		return true;
	}
	catch(_com_error& comError)
	{
		RecordErrorMsg(comError);
	}

	return false;
}

/**********************************************************
   功能：重载函数，根据字段名，数据类型获取数据集中的数据
   参数：in  sql语句中的字段名
         out 返回数据集中的值
   返回：成功返回true 失败返回false
  主要思路：
***********************************************************/ 
bool CDBAdo::GetFieldValue(LPCTSTR lpcsrFieldName, BYTE& bValue)
{
	bValue	= 0;
	try
	{
		_variant_t vtFld	= m_ptrRecordset->Fields->GetItem(lpcsrFieldName)->Value;
		switch(vtFld.vt)
		{
		case VT_BOOL:
			{
				bValue	= (vtFld.boolVal!=0)?1:0;
				break;
			}
		case VT_I2:
		case VT_UI1:
			{
				bValue	= (vtFld.iVal>0)?1:0;
				break;
			}
		case VT_NULL:
		case VT_EMPTY:
			{
				bValue	= 0;
				break;
			}
		default: bValue	= (BYTE)vtFld.iVal;
		}	
		return true;
	}
	catch(_com_error& comError)
	{
		RecordErrorMsg(comError);
	}

	return	false;
}
/**********************************************************
   功能：重载函数，根据字段名，数据类型获取数据集中的数据
   参数：in  sql语句中的字段名
         out 返回数据集中的值
   返回：成功返回true 失败返回false
  主要思路：
***********************************************************/ 
bool CDBAdo::GetFieldValue(LPCTSTR lpcsrFieldName, UINT& ulValue)
{
	ulValue	= 0L;
	try
	{
		_variant_t	vtFld	= m_ptrRecordset->Fields->GetItem(lpcsrFieldName)->Value;
		if((vtFld.vt!=VT_NULL)&&(vtFld.vt!=VT_EMPTY))
			ulValue	= vtFld.lVal;
		return	true;
	}
	catch(_com_error& comError)
	{
		RecordErrorMsg(comError);
	}

	return	false;
}
/**********************************************************
   功能：重载函数，根据字段名，数据类型获取数据集中的数据
   参数：in  sql语句中的字段名
         out 返回数据集中的值
   返回：成功返回true 失败返回false
  主要思路：
***********************************************************/ 
bool CDBAdo::GetFieldValue(LPCTSTR lpcsrFieldName, DOUBLE& dbValue)
{
	dbValue=0.0L;
	try
	{
		_variant_t	vtFld	= m_ptrRecordset->Fields->GetItem(lpcsrFieldName)->Value;
		switch(vtFld.vt)
		{
		case VT_R4:dbValue	= vtFld.fltVal;break;
		case VT_R8:dbValue	= vtFld.dblVal;break;
		case VT_DECIMAL:
			{
				dbValue	 = vtFld.decVal.Lo32;
				dbValue	*= (vtFld.decVal.sign==128)?-1:1;
				dbValue	/= pow((float)10,vtFld.decVal.scale);
			}
			break;
		case VT_UI1:dbValue	= vtFld.iVal;break;
		case VT_I2:
		case VT_I4:dbValue	= vtFld.lVal;break;
		case VT_NULL:
		case VT_EMPTY:dbValue	= 0.0L;break;
		default:dbValue	= vtFld.dblVal;
		}
		return true;
	}
	catch(_com_error& comError)
	{
		RecordErrorMsg(comError);
	}

	return	false;
}

/**********************************************************
   功能：重载函数，根据字段名，数据类型获取数据集中的数据
   参数：in  sql语句中的字段名
         out 返回数据集中的值
   返回：成功返回true 失败返回false
  主要思路：
***********************************************************/ 
bool CDBAdo::GetFieldValue(LPCTSTR lpcsrFieldName, LONG& lValue)
{
	lValue	= 0L;
	try
	{
		_variant_t	vtFld	= m_ptrRecordset->Fields->GetItem(lpcsrFieldName)->Value;
		if((vtFld.vt!=VT_NULL)&&(vtFld.vt!=VT_EMPTY))
			lValue	= vtFld.lVal;
		return	true;
	}
	catch(_com_error& comError)
	{
		RecordErrorMsg(comError);
	}

	return	false;
}

/**********************************************************
   功能：重载函数，根据字段名，数据类型获取数据集中的数据
   参数：in  sql语句中的字段名
         out 返回数据集中的值
   返回：成功返回true 失败返回false
  主要思路：
***********************************************************/ 
bool CDBAdo::GetFieldValue(LPCTSTR lpcsrFieldName, DWORD& dwValue)
{
	dwValue	= 0L;
	try
	{
		_variant_t	vtFld	= m_ptrRecordset->Fields->GetItem(lpcsrFieldName)->Value;
		if((vtFld.vt!=VT_NULL)&&(vtFld.vt!=VT_EMPTY))
			dwValue	= vtFld.ulVal;
		return	true;
	}
	catch(_com_error& comError)
	{
		RecordErrorMsg(comError);
	}

	return	false;
}

/**********************************************************
   功能：重载函数，根据字段名，数据类型获取数据集中的数据
   参数：in  sql语句中的字段名
         out 返回数据集中的值
   返回：成功返回true 失败返回false
  主要思路：
***********************************************************/ 
bool CDBAdo::GetFieldValue(LPCTSTR lpcsrFieldName, __int64& llValue)
{
	llValue	= 0L;
	try
	{
		_variant_t	vtFld	= m_ptrRecordset->Fields->GetItem(lpcsrFieldName)->Value;
		if((vtFld.vt!=VT_NULL)&&(vtFld.vt!=VT_EMPTY))
			llValue=vtFld.lVal;

		return	true;
	}
	catch(_com_error& comError)
	{
		RecordErrorMsg(comError);
	}

	return	false;
}

/**********************************************************
   功能：重载函数，根据字段名，数据类型获取数据集中的数据
   参数：in  sql语句中的字段名
         out 返回数据集中的值
   返回：成功返回true 失败返回false
   主要思路：
***********************************************************/ 
bool CDBAdo::GetFieldValue(LPCTSTR lpcsrFieldName, COleDateTime& Time)
{
	try
	{
		_variant_t	vtFld	= m_ptrRecordset->Fields->GetItem(lpcsrFieldName)->Value;
		switch(vtFld.vt) 
		{
		case VT_DATE:
		case 8209:
			{
				COleDateTime	TempTime(vtFld);
				Time	= TempTime;
			}break;
		case VT_EMPTY:
		case VT_NULL:Time.SetStatus(COleDateTime::null);break;
		default: return false;
		}
		return	true;
	}
	catch(_com_error& comError)
	{
		RecordErrorMsg(comError);
	}

	return	false;
}


/**********************************************************
   功能：重载函数，根据字段名，数据类型获取数据集中的数据
   参数：in  sql语句中的字段名
         out 返回数据集中的值
   返回：成功返回true 失败返回false
   主要思路：
***********************************************************/ 
bool CDBAdo::GetFieldValue(LPCTSTR lpcsrFieldName, bool& bValue)
{
	try
	{
		_variant_t	vtFld	= m_ptrRecordset->Fields->GetItem(lpcsrFieldName)->Value;
		switch(vtFld.vt) 
		{
		case VT_BOOL:bValue=(vtFld.boolVal==0)?false:true;break;
		case VT_EMPTY:
		case VT_NULL:bValue = false;break;
		default:return false;
		}

		return	true;
	}
	catch(_com_error& comError)
	{
		RecordErrorMsg(comError);
	}

	return	false;
}


/**********************************************************
   功能：重载函数，根据字段名，数据类型获取数据集中的数据
   参数：in  sql语句中的字段名
         out 返回数据集中的值
   返回：成功返回true 失败返回false
   主要思路：
***********************************************************/ 
bool CDBAdo::GetFieldValue(LPCTSTR lpcsrFieldName, _variant_t& varBLOB,int& len)
{
	try
	{
		unsigned char* pSafePtr= NULL;
		len=m_ptrRecordset->Fields->GetItem(lpcsrFieldName)->ActualSize;
		varBLOB=m_ptrRecordset->Fields->GetItem(lpcsrFieldName)->GetChunk(len);
		return	true;
	}
	catch(_com_error& comError)
	{
		RecordErrorMsg(comError);
	}

	return	false;
}

