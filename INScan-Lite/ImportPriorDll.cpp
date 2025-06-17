#include "StdAfx.h"
#include "ImportPriorDll.h"
//#include <Windows.h>


CImportPriorDll::CImportPriorDll(void)
{
	m_iError = 0;
	m_fCurX = 0;
	m_fCurY = 0;
	m_bConnect = false;
	long port = 15;
	HRESULT hRes;

	double x,y;
	long busy;

	::CoInitialize(NULL);
	{
		hRes = pScan.CreateInstance(__uuidof(Scan));
		if(FAILED(hRes))
		{
			printf("IScanPtr::CreateInstance failed w/err 0x%08lx\n", hRes);
		}
// 		else
// 		{
// 			long result = pScan->Connect(&port);
// 
// 			if (result)
// 			{
// 				printf("Failed to connect to controller %d\n", result);   
// 			}
// 			else
// 			{
// 				printf("%d\n", pScan->GetControllerFirmwareVersion());
// 
// 				hRes = pStage.CreateInstance(__uuidof(Stage));
// 				if(FAILED(hRes))
// 				{
// 					printf("IStagePtr::CreateInstance failed w/err 0x%08lx\n", hRes);
// 				}
// 				else
// 				{
// 					pStage->MoveToAbsolute(-4000,2000);
// 
// 					do
// 					{
// 						pScan->IsMoving(&busy);
// 					} while (busy & 0x03);
// 
// 					pStage->GetPosition(&x,&y);
// 
// 					pStage->MoveRelative(50,50);
// 
// 					do
// 					{
// 						pScan->IsMoving(&busy);
// 					} while (busy & 0x03);
// 
// 					pStage->GetPosition(&x,&y);
// 
// 					pScan->DisConnect();
// 				}
// 			}
// 		}    
	}



}


CImportPriorDll::~CImportPriorDll(void)
{
	::CoUninitialize();
}


bool	CImportPriorDll::connect_prior(long iPort)
{
	m_iError = 0;
	long result = pScan->Connect(&iPort);
	if(result != 0)
		m_iError = 1000;
	else
	{
		HRESULT hRes = pStage.CreateInstance(__uuidof(Stage));
		if(FAILED(hRes))
			m_iError = 1001;
		else
			m_bConnect = true;
	}
		
	return (m_iError==0);
}
bool	CImportPriorDll::disconnect_prior()
{
	m_iError=0;
	long result = pScan->DisConnect();
	if(result != PRIOR_OK)
		m_iError = 1002;
	else
		m_bConnect = false;

	return (m_iError==0);
}
bool	CImportPriorDll::get_is_connect()
{
	return m_bConnect;
}
int	CImportPriorDll::get_error()
{
	return m_iError;
}
bool	CImportPriorDll::move_left(int istep)
{
	m_iError = 0;
	if(m_bConnect)
	{
		pStage->GetPosition(&m_fCurX,&m_fCurY);
		pStage->MoveToAbsolute(m_fCurX+istep*1000,m_fCurY);
		long busy;
		do
		{
			pScan->IsMoving(&busy);
		} while (busy & 0x03);
	}
	else
		m_iError = 1000;
	return (m_iError==0);
}
bool	CImportPriorDll::move_right(int istep)
{
	m_iError = 0;
	if(m_bConnect)
	{
		pStage->GetPosition(&m_fCurX,&m_fCurY);
		pStage->MoveToAbsolute(m_fCurX-istep*1000,m_fCurY);
		long busy;
		do
		{
			pScan->IsMoving(&busy);
		} while (busy & 0x03);
	}
	else
		m_iError = 1000;
	return (m_iError==0);
}
bool	CImportPriorDll::move_forward(int istep)
{
	m_iError = 0;
	if(m_bConnect)
	{
		pStage->GetPosition(&m_fCurX,&m_fCurY);
		pStage->MoveToAbsolute(m_fCurX,m_fCurY+istep*1000);
		long busy;
		do
		{
			pScan->IsMoving(&busy);
		} while (busy & 0x03);
	}
	else
		m_iError = 1000;
	return (m_iError==0);
}
bool	CImportPriorDll::move_back(int istep)
{
	m_iError = 0;
	if(m_bConnect)
	{
		pStage->GetPosition(&m_fCurX,&m_fCurY);
		pStage->MoveToAbsolute(m_fCurX,m_fCurY-istep*1000);
		long busy;
		do
		{
			pScan->IsMoving(&busy);
		} while (busy & 0x03);
	}
	else
		m_iError = 1000;
	return (m_iError==0);
}
bool	CImportPriorDll::move_left_forward(int istep)
{
	m_iError = 0;
	if(m_bConnect)
	{
		pStage->GetPosition(&m_fCurX,&m_fCurY);
		pStage->MoveToAbsolute(m_fCurX+istep*1000,m_fCurY+istep*1000);
		long busy;
		do
		{
			pScan->IsMoving(&busy);
		} while (busy & 0x03);
	}
	else
		m_iError = 1000;
	return (m_iError==0);
}
bool	CImportPriorDll::move_left_back(int istep)
{
	m_iError = 0;
	if(m_bConnect)
	{
		pStage->GetPosition(&m_fCurX,&m_fCurY);
		pStage->MoveToAbsolute(m_fCurX+istep*1000,m_fCurY-istep*1000);
		long busy;
		do
		{
			pScan->IsMoving(&busy);
		} while (busy & 0x03);
	}
	else
		m_iError = 1000;
	return (m_iError==0);
}
bool	CImportPriorDll::move_right_forward(int istep)
{
	m_iError = 0;
	if(m_bConnect)
	{
		pStage->GetPosition(&m_fCurX,&m_fCurY);
		pStage->MoveToAbsolute(m_fCurX-istep*1000,m_fCurY+istep*1000);
		long busy;
		do
		{
			pScan->IsMoving(&busy);
		} while (busy & 0x03);
	}
	else
		m_iError = 1000;
	return (m_iError==0);
}
bool	CImportPriorDll::move_right_back(int istep)
{
	m_iError = 0;
	if(m_bConnect)
	{
		pStage->GetPosition(&m_fCurX,&m_fCurY);
		pStage->MoveToAbsolute(m_fCurX-istep*1000,m_fCurY-istep*1000);
		long busy;
		do
		{
			pScan->IsMoving(&busy);
		} while (busy & 0x03);
	}
	else
		m_iError = 1000;
	return (m_iError==0);
}
bool	CImportPriorDll::move_up(int istep)
{
	m_iError = 0;
	if(m_bConnect)
	{
		pStage->GetPosition(&m_fCurX,&m_fCurY);
		pScan->MoveXYZRelative(m_fCurX, m_fCurY, istep*10);
		long busy;
		do
		{
			pScan->IsMoving(&busy);
		} while (busy & 0x03);
	}
	else
		m_iError = 1000;
	return (m_iError==0);
}
bool	CImportPriorDll::move_down(int istep)
{
	m_iError = 0;
	if(m_bConnect)
	{
		pStage->GetPosition(&m_fCurX,&m_fCurY);
		pScan->MoveXYZRelative(m_fCurX, m_fCurY, -istep*10);
		long busy;
		do
		{
			pScan->IsMoving(&busy);
		} while (busy & 0x03);
	}
	else
		m_iError = 1000;
	return (m_iError==0);
}
bool	CImportPriorDll::move_position(double X, double Y)
{
	m_iError = 0;
	if(m_bConnect)
	{
		pStage->GetPosition(&m_fCurX,&m_fCurY);
		pStage->MoveToAbsolute(X*1000,Y*1000);
		long busy;
		do
		{
			pScan->IsMoving(&busy);
		} while (busy & 0x03);
	}
	else
		m_iError = 1000;
	return (m_iError==0);
}
bool	CImportPriorDll::get_position(double &X, double &Y)
{
	m_iError = 0;
	if(m_bConnect)
	{
		pStage->GetPosition(&m_fCurX,&m_fCurY);
		X = m_fCurX/1000;
		Y = m_fCurY/1000;
	}
	else
		m_iError = 1000;
	return (m_iError==0);
}
bool	CImportPriorDll::move_zero_xy()
{
	m_iError = 0;
	if(m_bConnect)
	{
		pStage->GetPosition(&m_fCurX,&m_fCurY);
		pStage->MoveToAbsolute(0,0);
		long busy;
		do
		{
			pScan->IsMoving(&busy);
		} while (busy & 0x03);
	}
	else
		m_iError = 1000;
	return (m_iError==0);
}

bool	CImportPriorDll::move_zero_z()
{
	m_iError = 0;
	if(m_bConnect)
	{
		pStage->GetPosition(&m_fCurX,&m_fCurY);
		pScan->MoveXYZRelative(m_fCurX, m_fCurY, 0);
		long busy;
		do
		{
			pScan->IsMoving(&busy);
		} while (busy & 0x03);
	}
	else
		m_iError = 1000;
	return (m_iError==0);
}
bool	CImportPriorDll::stop()
{
	m_iError = 0;
	if(m_bConnect)
	{
		pScan->KillAllMotion();
		long busy;
		do
		{
			pScan->IsMoving(&busy);
		} while (busy & 0x03);
	}
	else
		m_iError = 1000;
	return (m_iError==0);
}