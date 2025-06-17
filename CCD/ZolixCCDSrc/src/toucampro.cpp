#include "toucampro.h"
#include "stdio.h"
toucam_ccdproc::toucam_ccdproc(void)
{
	m_hmodule = NULL;
	m_pOpen= nullptr; 
	m_pOpenEx= nullptr; 
	m_pListDevices= nullptr; 
	m_pClose= nullptr;
	m_pRead= nullptr;
	m_pWrite= nullptr;
	m_pResetDevice= nullptr;
	m_pPurge= nullptr;
	m_pSetTimeouts= nullptr;
	m_pGetQueueStatus= nullptr;
	m_pGetDriverVersion= nullptr;
	m_pSetBaudRate=nullptr;
}


toucam_ccdproc::~toucam_ccdproc(void)
{

	m_pOpen= nullptr; 
	m_pOpenEx= nullptr; 
	m_pListDevices= nullptr; 
	m_pClose= nullptr;
	m_pRead= nullptr;
	m_pWrite= nullptr;
	m_pResetDevice= nullptr;
	m_pPurge= nullptr;
	m_pSetTimeouts= nullptr;
	m_pGetQueueStatus= nullptr;
	m_pGetDriverVersion= nullptr;
	m_pSetBaudRate=nullptr;
	m_pSetDataCharacteristics= nullptr;
}

bool toucam_ccdproc::LoadFunctionFromlibray(char* wcLibName)
{
	if (m_hmodule != nullptr)
	{
		FreeFunctionFromlibray();
	}
	m_hmodule = LoadLibraryA(wcLibName);

	printf("%s\n", wcLibName);
	//TCHAR szPath[MAX_PATH] = { 0 };
	//GetModuleFileName(NULL, szPath, MAX_PATH);
	//wprintf(szPath);


	if (m_hmodule == nullptr)
	{
		OutputDebugString("dll open fail\n");
		return false;
	}
	m_pWrite = (PtrToWrite)GetProcAddress(m_hmodule, "FT_Write");
	if (m_pWrite == NULL)
	{
	
		return false;
	}

	m_pRead = (PtrToRead)GetProcAddress(m_hmodule, "FT_Read");
	if (m_pRead == NULL)
	{
		return false;
	}

	m_pOpen = (PtrToOpen)GetProcAddress(m_hmodule, "FT_Open");
	if (m_pOpen == NULL)
	{
		return false;
	}

	m_pOpenEx = (PtrToOpenEx)GetProcAddress(m_hmodule, "FT_OpenEx");
	if (m_pOpenEx == NULL)
	{
		return false;
	}

	m_pListDevices = (PtrToListDevices)GetProcAddress(m_hmodule, "FT_ListDevices");
	if(m_pListDevices == NULL)
	{
		return false;
	}

	m_pClose = (PtrToClose)GetProcAddress(m_hmodule, "FT_Close");
	if (m_pClose == NULL)
	{
		return false;
	}

	m_pResetDevice = (PtrToResetDevice)GetProcAddress(m_hmodule, "FT_ResetDevice");
	if (m_pResetDevice == NULL)
	{
		return false;
	}

	m_pPurge = (PtrToPurge)GetProcAddress(m_hmodule, "FT_Purge");
	if (m_pPurge == NULL)
	{
		return false;
	}
	m_pSetBaudRate = (PtrToSetBaudRate)GetProcAddress(m_hmodule, "FT_SetBaudRate");
	if (m_pPurge == NULL)
	{
		return false;
	}
	m_pSetTimeouts = (PtrToSetTimeouts)GetProcAddress(m_hmodule, "FT_SetTimeouts");
	if (m_pSetTimeouts == NULL)
	{
		return false;
	}

	m_pGetQueueStatus = (PtrToGetQueueStatus)GetProcAddress(m_hmodule, "FT_GetQueueStatus");
	if (m_pGetQueueStatus == NULL)
	{
		return false;
	}
	m_pGetDriverVersion = (PtrToGetDriverVersion)GetProcAddress(m_hmodule, "FT_GetDriverVersion");
	if (m_pGetQueueStatus == NULL)
	{
		return false;
	}
	m_pSetDataCharacteristics= (PtrToSetDataCharacteristics)GetProcAddress(m_hmodule, "FT_SetDataCharacteristics");
	m_pSetFlowControl=(PtrToSetFlowControl)GetProcAddress(m_hmodule, "FT_SetFlowControl");
	return true;
}

bool toucam_ccdproc::FreeFunctionFromlibray()
{
	FreeLibrary(m_hmodule);
	m_hmodule = nullptr;
	 m_pOpen= nullptr; 
	 m_pOpenEx= nullptr; 
	 m_pListDevices= nullptr; 
	 m_pClose= nullptr;
	 m_pRead= nullptr;
	 m_pWrite= nullptr;
	 m_pResetDevice= nullptr;
	 m_pPurge= nullptr;
	 m_pSetTimeouts= nullptr;
	 m_pGetQueueStatus= nullptr;
	 m_pGetDriverVersion= nullptr;
	 m_pSetBaudRate=nullptr;
	 m_pSetDataCharacteristics= nullptr;

	return true;
}