//#include "D:\FI\INScan-Lite\stdafx.h"
//#ifdef NDEBUG
#include "StdAfx.h"
//#endif
#include "../stdafx.h"
#include "rt_nonfinite.h"
#include "MixtureFeatureExtract.h"
#include "PureFeatureExtract.h"
#include "analyseMixture.h"
#include "analysePure.h"
#include "autoestibl.h"
#include "baseline_correct.h"
#include "cal_noiselevel.h"
#include "cal_snr.h"
#include "calkafang.h"
#include "cwt1.h"
#include "deri2.h"
#include "fastsmooth.h"
#include "geto_new.h"
#include "offsetCorrect.h"
#include "resolution.h"
#include "resolution_factor.h"
#include "selectitem_m.h"
#include "selectitem_p.h"
#include "zl_fullspectrumcontrast.h"
#include "zl_lsqnonneg1.h"
#include "analyseMixture_terminate.h"
#include "analyseMixture_emxAPI.h"
#include "analyseMixture_initialize.h"
#include "DataCompareLast.h"


#include "..\dbado\base_64.h"


CString sql_id=_T("id");
CString sql_datax=_T("dataX");
CString sql_datay=_T("dataY");
CString sql_data=_T("data");
CString sql_name_ch=_T("filename");
CString sql_name_eh=_T("filename_eh");
CString sql_code=_T("filename_code");
CString sql_datalen=_T("datalen");
CString sql_len=_T("len");
CString sql_table=_T("DataT");

CDataCompareLast::CDataCompareLast(void)
{
	m_IsOpen=false;
	len=0;
	VectorInit();
	


	sql_id=_T("id");
	sql_datax=_T("sample_data_x");
	sql_datay=_T("sample_data_y");
	sql_name_ch=_T("sample_name_zh");
	sql_name_eh=_T("sample_name");
	sql_code=_T("sample_code");
	sql_data=_T("dataY1650");
	//sql_datalen=_T("datalen");
	//sql_len=_T("len");
	//sql_self=_T("sample_self");
	//sql_class=_T("sample_class_data");
	if (sSInfo.nSqlMode==0)
	{
		sql_table=_T("DataT");
	}
	else
	{
		sql_table=_T("DataK");
	}
	sql_dataP=_T("dataP");
	sql_data_len=_T("sample_data_len");	
	InitSQL();
}

CDataCompareLast::~CDataCompareLast(void)
{
	adoinfo.ClearAllParameters();
	adoinfo.CloseConnection();
	adoinfo.CloseRecordset();
}
void CDataCompareLast::VectorInit()
{
	vDataInfo.clear();
	vDataInfo.shrink_to_fit();
	vAllRes.clear();
	vAllRes.shrink_to_fit();
	DataInfo strDataInfor;
	for (int i=0;i<MaxTenData;i++)
	{   
		strDataInfor.fSimilarNum=0;
		vDataInfo.push_back(strDataInfor);
	}
}

bool CDataCompareLast::InitSQL()
{
	if (sSInfo.nSqlMode==0)
	{
		sql_table=_T("DataT");
	}
	else
	{
		sql_table=_T("DataK");
	}
	CString str=_T("select id from ")+sql_table+_T(" where id=1");
	m_IsOpen=adoinfo.OpenRecordset(str); 
	return m_IsOpen;
}
bool CDataCompareLast::IsOpen()
{
	return m_IsOpen;
}
CString decode(CString strData)
{
	CString str;
	str=base64_decode(strData.GetBuffer()).c_str();		
	return base64_decode(str.GetBuffer()).c_str();		
}
BOOL CDataCompareLast::DataCompare(double *pDataX,double *pDataY,int pDataLen)
{
	if (m_IsOpen==false)
	{
		return FALSE;
	}
	long lCount=adoinfo.GetRecordCount();
	CString strDataInforP,strSQL,strData1800;
	strSQL="select "+ sql_id +"," +sql_data+","+sql_dataP+" from "+sql_table;
    int ID;
	int len=0;//特征值长度
	m_IsOpen=adoinfo.OpenRecordset(strSQL);
	double dOutput[2000];
	bool bRet=false;
	bRet=AddData2SQL(pDataX,pDataY,pDataLen,cDataTest,dOutput,&pDataLen);

	if (bRet ==false)
	{
		return TRUE;
	}

	for (int i=0;i<adoinfo.GetRecordCount();i++)
	{

		adoinfo.GetFieldValue(sql_id,ID);
		adoinfo.GetFieldValue(sql_data,strData1800);
		adoinfo.GetFieldValue(sql_dataP,strDataInforP);

		double dSmple[120];
		ZeroMemory(dSmple,120);
		Split(strDataInforP,"\t",dSmple);
		double res[2];
		analysePure(dSmple,dOutput,res);
		AllRes strDataInfor;
		strDataInfor.id=ID;
		strDataInfor.res[0]=res[0];
		strDataInfor.res[1]=res[1];
		vAllRes.push_back(strDataInfor);
		adoinfo.MoveToNext();
	}
	

	double *dRet=new double[vAllRes.size()];
	int dRsize[3];
	double *dReslist=NULL;
	dReslist=new double[vAllRes.size()*3];
	for (int j=0;j<vAllRes.size();j++)
	{
		dReslist[j]=j;
		dReslist[vAllRes.size()+j]=vAllRes[j].res[0];
		dReslist[vAllRes.size()*2+j]=vAllRes[j].res[1];
	}
	emxArray_real_T *aa;

	aa = emxCreateWrapper_real_T(dReslist,vAllRes.size(),3);	
	
	double dFlag;

	selectitem_p(aa,dRet,dRsize,&dFlag);
	emxDestroyArray_real_T(aa);
	delete [] dReslist;


	double *d1800=new double[1800];
	ZeroMemory(d1800,1800);
	
   
	double  danalyse;
	#pragma ompparallel for
	for (int w=0;w<dRsize[0];w++)
	{
		CString str11,strSQL11="select "+sql_data+" from "+sql_table+" where "+sql_id+" =%d";
		str11.Format(strSQL11,(int)vAllRes[dRet[w]].id);
		bool oprec=adoinfo.OpenRecordset(str11); 
		if (oprec)
		{
			adoinfo.GetFieldValue(sql_data,strData1800);		
			Split(strData1800,"\t",d1800);
			danalyse=zl_fullspectrumcontrast(d1800, cDataTest,vAllRes[dRet[w]].res[1]);
			Sort(danalyse,vAllRes[dRet[w]].id);
		}		
	}

	adoinfo.ClearAllParameters();
	delete [] dRet;
	dRet=NULL;
	delete [] d1800;
	d1800=NULL;
	return TRUE;
}

//数据预处理
bool CDataCompareLast::DataPreprocessing(double*x,double*y,int nSize,double*dRet)
{	
		double * new_data = new double [nSize*2];
		memcpy(new_data,x,nSize*sizeof(double));
		memcpy(new_data+nSize,y,nSize*sizeof(double));

		emxArray_real_T *aa;
		//double y[1650];

		// Initialize function 'resolution' input arguments.
		// Initialize function input argument 'aa'.
		aa = emxCreateWrapper_real_T(new_data,nSize,2);

		double dFlag;
		int nRSize[3];
		// Call the entry-point 'resolution'.
		resolution(aa, dRet,nRSize,&dFlag);
		emxDestroyArray_real_T(aa);

		delete [] new_data;

		if (dFlag>0.5)
		{
			return false;
		}
		return true;
	//}
}
//提取特征值
bool CDataCompareLast::dPureFeatureExtract(double *y,double *dRet)
{
	emxArray_real_T *aa;

	aa = emxCreateWrapper_real_T(NULL,0,0);

	// Call the entry-point 'resolution'.

	double dFlag;
	PureFeatureExtract(y, aa,&dFlag);
	for (int i=0;i<14;i++)
	{
		dRet[i]=aa->data[i];
	}	
	emxDestroyArray_real_T(aa);

	if (dFlag>0.5)
	{
		return false;
	}
	return true;
}
//提取特征值

//判断信噪比
bool CDataCompareLast::IsValue(double*x,double*y,int Len)
{
	double *RealData = new double[Len] {0.0};
	int RealLen=0;
	for (int i =0;i<Len;i++)
	{
		if ((x[i]>=150)&&(x[i]<=2000))
		{
			RealData[RealLen]=y[i];
			RealLen++;
		}
	}

	emxArray_real_T *aa;

	aa = emxCreateWrapper_real_T(RealData,Len,1);

	double dReet=cal_snr(aa);
	// Call the entry-point 'resolution'.


	emxDestroyArray_real_T(aa);

	delete [] RealData;

	if(dReet>=5)
	{
		return true;
	}
	else
	{
		return false;
	}

}

bool CDataCompareLast::AddData2SQL(double *x,double *y,int Len,double *CData,double *PData,int *nRetSize)
{
	int nSize=0;
	if (IsValue(x,y,Len))
	{
		if (DataPreprocessing(x,y,Len,CData))
		{
			return dPureFeatureExtract(CData,PData);
		}
	}
	
	return false;
}

CString CDataCompareLast::encode(CString str2)
{
	CString strTemp;
	strTemp=base64_encode((unsigned char*)str2.GetBuffer(str2.GetLength()), str2.GetLength()).c_str(); 
	strTemp=base64_encode((unsigned char*)strTemp.GetBuffer(strTemp.GetLength()),strTemp.GetLength()).c_str();
	return strTemp;
}

int CDataCompareLast::Split(CString content, CString pattern, double *strlistX)
{
	
	int iPos = content.Find(pattern), iStart = 0;
	int x=0; 
	while (iPos != -1)
	{

		strlistX[x]=(double)atof(content.Mid(iStart,iPos - iStart));
		iStart = iPos + pattern.GetLength();
		iPos = content.Find(pattern, iStart);
		x++;
	}

	if (iStart <= content.GetLength())
		strlistX[x]=(double)atof(content.Mid(iStart));

	return x;
}



void CDataCompareLast::Sort(float nSimilar,int num)
{
	float iTemp=0.0;
	CString strTemp=_T("");
	int inum=0;
	if(nSimilar>=vDataInfo[MaxTenData-1].fSimilarNum)
	{
		vDataInfo[MaxTenData-1].fSimilarNum=nSimilar;
		vDataInfo[MaxTenData-1].num=num;

		for (int i=MaxTenData-1;i>0;i--)
		{
			if (vDataInfo[i].fSimilarNum>vDataInfo[i-1].fSimilarNum)
			{
				iTemp=vDataInfo[i-1].fSimilarNum;
				vDataInfo[i-1].fSimilarNum=vDataInfo[i].fSimilarNum;
				vDataInfo[i].fSimilarNum=iTemp;

				inum=vDataInfo[i-1].num;
				vDataInfo[i-1].num=vDataInfo[i].num;
				vDataInfo[i].num=inum;
			}
		}
	}
}

void  CDataCompareLast::ShowData(std::vector<DataInfo> *m_DataInfo)
{
	DataInfo dInfo;
	m_DataInfo->clear();
	m_DataInfo->shrink_to_fit();
	for (int i=0;i<MaxTenData;i++)
	{
		if (vDataInfo[i].fSimilarNum>0)
		{
			int j=0;
			CString str,strSQL="select "+sql_data_len +","+sql_name_ch+","+sql_name_eh+","+sql_datax+","+sql_datay+" from "+sql_table+" where "+sql_id+" =%d";
        	str.Format(strSQL,vDataInfo[i].num);
			bool oprec=adoinfo.OpenRecordset(str); 
			if (oprec)
			{
				string cc;
				dInfo.num=vDataInfo[i].num;
				adoinfo.GetFieldValue(sql_data_len,dInfo.datalen);
				adoinfo.GetFieldValue(sql_name_ch,dInfo.name_ch);
				adoinfo.GetFieldValue(sql_name_eh,dInfo.name_eh);
				adoinfo.GetFieldValue(sql_datax,dInfo.strXData);
				adoinfo.GetFieldValue(sql_datay,dInfo.strYData);
				dInfo.strXData=decode(dInfo.strXData);
				dInfo.strYData=decode(dInfo.strYData);
				if (sql_name_ch=="null")
				{
					dInfo.name_ch=dInfo.name_eh;
				}
				dInfo.fSimilarNum=vDataInfo[i].fSimilarNum;
				m_DataInfo->push_back(dInfo);
			}

		}
	}
}

