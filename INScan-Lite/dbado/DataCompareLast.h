#pragma once
#define DataNum  2094
#define SQLDataNum 100
#define nRemoveLen 9
#define MaxTenData 5
#include "..\stdafx.h"
//#include "stdafx.h"
#include "..\dbado\DBAdo.h"

#include <vector>
//struct DataInfo
//{
//	long  num;
//	float    fSimilarNum;
//	CString  name_ch;
//	CString  name_eh;
//	CString  code;
//	CString  strXData;
//	CString  strYData;
//	int      datalen;
//};


class CDataCompareLast
{

public:

	CDataCompareLast(void);
	~CDataCompareLast(void);
	bool IsOpen();
	BOOL DataCompare(double *pDataX,double *pDataY,int pDataLen);
	float DataProcess(CString str1800,CString strDataInfor,int pSqlLen,int ID,double *dInputData,int Inputlen);


private:
	
	
	int  Split(CString content, CString pattern, double *strlistX);
	void Sort(float nSimilar,int num);
	CDBAdo adoinfo;  
	bool m_IsOpen;
	int len;
	
	CString sql_id;
	CString sql_datax;
	CString sql_datay;
	CString sql_data_len;
	CString sql_data;
	CString sql_name;
	CString sql_name_eh;
	CString sql_code;
	CString sql_datalen;
	CString sql_len;
	CString sql_table;
	CString sql_self;
	CString sql_class;
	CString sql_dataP;

	double cData[1800];
	double cDataTest[1800];

public:
	std::vector<DataInfo>vDataInfo;
	std::vector<AllRes>vAllRes;
	void VectorInit();
	void ShowData(std::vector<DataInfo> *m_DataInfo);
	CString encode(CString str2);
public:
	bool InitSQL();//连接动态库
	bool AddData2SQL(double *x,double *y,int Len,double *CData,double *PData,int *nRetSize);
	double *ID;
	double *analyseData;
	double  size;
	bool DataPreprocessing(double*x,double*y,int nSize,double*dRet);//数据预处理
	bool dPureFeatureExtract(double *y,double *dRet);
	bool IsValue(double*x,double*y,int Len);
};