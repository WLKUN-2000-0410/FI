#pragma once
#ifndef  TOUCAM_CCD_H
#define  TOUCAM_CCD_H

#include <windows.h>
#include "ftd2xx.h"
class toucam_ccdproc
{


public:
	toucam_ccdproc();
	~toucam_ccdproc();

private:
	typedef FT_STATUS (WINAPI *PtrToOpen)(PVOID, FT_HANDLE *); 
	typedef FT_STATUS (WINAPI *PtrToOpenEx)(PVOID, DWORD, FT_HANDLE *); 
	typedef FT_STATUS (WINAPI *PtrToListDevices)(PVOID, PVOID, DWORD);
	typedef FT_STATUS (WINAPI *PtrToClose)(FT_HANDLE);
	typedef FT_STATUS (WINAPI *PtrToRead)(FT_HANDLE, LPVOID, DWORD, LPDWORD);
	typedef FT_STATUS (WINAPI *PtrToWrite)(FT_HANDLE, LPVOID, DWORD, LPDWORD);
	typedef FT_STATUS (WINAPI *PtrToResetDevice)(FT_HANDLE);
	typedef FT_STATUS (WINAPI *PtrToPurge)(FT_HANDLE, ULONG);
	typedef FT_STATUS (WINAPI *PtrToSetTimeouts)(FT_HANDLE, ULONG, ULONG);
	typedef FT_STATUS (WINAPI *PtrToGetQueueStatus)(FT_HANDLE, LPDWORD);
	typedef FT_STATUS (WINAPI *PtrToGetDriverVersion)(FT_HANDLE, LPDWORD);
	typedef FT_STATUS (WINAPI *PtrToSetBaudRate)(FT_HANDLE, ULONG);
	typedef FT_STATUS (WINAPI *PtrToSetDataCharacteristics)(FT_HANDLE, ULONG, ULONG, ULONG);
	typedef FT_STATUS (WINAPI *PtrToSetFlowControl)(FT_HANDLE, ULONG, ULONG, ULONG);

	
private:
	HINSTANCE			m_hmodule;
public:

	PtrToOpen m_pOpen; 
	PtrToOpenEx m_pOpenEx; 
	PtrToListDevices m_pListDevices; 
	PtrToClose m_pClose;
	PtrToRead m_pRead;
	PtrToWrite m_pWrite;
	PtrToResetDevice m_pResetDevice;
	PtrToPurge m_pPurge;
	PtrToSetTimeouts m_pSetTimeouts;
	PtrToGetQueueStatus m_pGetQueueStatus;
	PtrToGetDriverVersion m_pGetDriverVersion;
	PtrToSetBaudRate m_pSetBaudRate;
	PtrToSetDataCharacteristics m_pSetDataCharacteristics;
	PtrToSetFlowControl m_pSetFlowControl;

	bool LoadFunctionFromlibray(char* wcLibName);
	bool FreeFunctionFromlibray();
	////



};

#endif