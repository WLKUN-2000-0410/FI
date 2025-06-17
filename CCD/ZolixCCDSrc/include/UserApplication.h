
#ifndef __USERAPPLICATION__H
#define __USERAPPLICATION__H

#ifdef DLL_LIBRARY_EXPORTS
#include "DeviceLibrary.h"
#include "DSPLibrary.h"
#include "MathFuncs.h"

#else
#define DLL_API __declspec(dllimport)
typedef enum
{
	//Internal Definition
	API_INT_START = 0,
	API_SUCCESS = API_INT_START,
	API_INT_BUFFER_INVALID,
	API_INT_FEATURE_UNSUPPORTED,
	API_INT_PROTOCOL_ERROR,
	API_INT_CALIBRATION_ERROR,
	API_INT_MEMORY_ERROR,
	API_INT_ARGUMENT_ERROR,
	API_INT_HANDLE_INVALID,
	API_INT_TIMEOUT,
	//API_INT_DDI_BUFFER_ERROR,
	API_INT_DATA_NOT_READY,
	API_INT_DATA_TIME_OUT,
	API_INT_FILE_IO_ERROR, //20161012 Kevin
	API_INT_FILE_Exist_ERROR, //20170913 Kevin
	API_INT_END,

	//Vendor Reservation Code
	API_EXT_START = 0x80000000
} ERRORCODE;
#endif



#ifdef __cplusplus
extern "C" 
{
#endif
#define UAI_LIBRARY_NAME "UserAppilcation"
#define UAI_LIBRARY_VERSION (LIBRARY_VERSION)
#define UAI_LIBRARY_BUILD_NUMBER (128)
//***************************************************************************************//
//Function Name: 	UAI_LibraryGetNameLength
//Input Arguments:	
//		Type: unsigned int*, Name: library name length buffer pointer
//Return Value: 		API Error Code
//***************************************************************************************//
DLL_API ERRORCODE UAI_LibraryGetNameLength(unsigned int* length);

//***************************************************************************************//
//Function Name: 	UAI_LibraryGetName
//Input Arguments:	
//		Type: unsigned int*, Name: library name buffer pointer
//Return Value: 		API Error Code
//***************************************************************************************//
DLL_API ERRORCODE UAI_LibraryGetName(unsigned char* name);

//***************************************************************************************//
//Function Name: 	UAI_LibraryGetVersion
//Input Arguments:	
//		Type: unsigned short*, Name: library version buffer pointer
//Return Value: 		API Error Code
//***************************************************************************************//
DLL_API ERRORCODE UAI_LibraryGetVersion(unsigned short* version);

//***************************************************************************************//
//Function Name: 	UAI_LibraryGetBuildNumber
//Input Arguments:	
//		Type: unsigned int*, Name: build number buffer pointer
//Return Value: 		API Error Code
//***************************************************************************************//
DLL_API ERRORCODE UAI_LibraryGetBuildNumber(unsigned int* build);

//***************************************************************************************//
//Function Name: 	UAI_FirmwareGetVersion
//Input Arguments:	
//		Type: void*, Name: device handle pointer
//		Type: unsigned short*, Name: firmware version buffer pointer
//Return Value: 		API Error Code
//***************************************************************************************//
DLL_API ERRORCODE UAI_FirmwareGetVersion(void* api_handle, unsigned int* version);

//***************************************************************************************//
//Function Name: 	UAI_FirmwareGetBuildNumber
//Input Arguments:	
//		Type: void*, Name: device handle pointer
//		Type: unsigned int*, Name: build number buffer pointer
//Return Value: 		API Error Code
//***************************************************************************************//
DLL_API ERRORCODE UAI_FirmwareGetBuildNumber(void* api_handle, unsigned int* build);

//***************************************************************************************//
//Function Name: 	UAI_SpectrometerGetRomVersion
//Input Arguments:	
//		Type: void*, Name: device handle pointer
//		Type: unsigned short*, Name: rom version buffer pointer
//Return Value: 		API Error Code
//***************************************************************************************//
DLL_API ERRORCODE UAI_SpectrometerGetRomVersion(void* api_handle, unsigned short* version);

//***************************************************************************************//
//Function Name: 	UAI_SpectrometerGetModelName
//Input Arguments:	
//		Type: void*, Name: device handle pointer
//		Type: unsigned char*, Name: model name buffer pointer
//Return Value: 		API Error Code
//***************************************************************************************//
DLL_API ERRORCODE UAI_SpectrometerGetModelName(void* api_handle, unsigned char *model);

//***************************************************************************************//
//Function Name: 	UAI_SpectrometerGetSerialNumber
//Input Arguments:	
//		Type: void*, Name: device handle pointer
//		Type: unsigned char*, Name: serial number buffer pointer
//Return Value: 		API Error Code
//***************************************************************************************//
DLL_API ERRORCODE UAI_SpectrometerGetSerialNumber(void* api_handle, unsigned char *serial);

//***************************************************************************************//
//Function Name: 	UAI_SpectrometerGetManufacturingDate
//Input Arguments:	
//		Type: void*, Name: device handle pointer
//		Type: unsigned short*, Name: manufacturing date buffer pointer
//Return Value: 		API Error Code
//***************************************************************************************//
DLL_API ERRORCODE UAI_SpectrometerGetManufacturingDate(void* api_handle, unsigned char *date);

//***************************************************************************************//
//Function Name: 	UAI_SpectrometerGetActivatingDate
//Input Arguments:	
//		Type: void*, Name: device handle pointer
//		Type: unsigned short*, Name: activating date buffer pointer
//Return Value: 		API Error Code
//***************************************************************************************//
DLL_API ERRORCODE UAI_SpectrometerGetActivatingDate(void* api_handle, unsigned char *date);

//***************************************************************************************//
//***************************************************************************************//
//Function Name: 	UAI_SpectrometerGetSocType
//Input Arguments:	
//		Type: void*, Name: device handle pointer
//		Type: unsigned char*, Name: soc type buffer pointer
//Return Value: 		API Error Code
//***************************************************************************************//
DLL_API ERRORCODE UAI_SpectrometerGetSocType(void* api_handle, unsigned char *type);

//**************************************************************************************
//Function Name: 	UAI_SpectrometerGetRomType
//Input Arguments:	
//		Type: void*, Name: device handle pointer
//		Type: unsigned char*, Name: rom type buffer pointer
//Return Value: 		API Error Code
//***************************************************************************************//
DLL_API ERRORCODE UAI_SpectrometerGetRomType(void* api_handle, unsigned char *type);

//***************************************************************************************//
//Function Name: 	UAI_SpectrometerGetRomSerialNumber
//Input Arguments:	
//		Type: void*, Name: device handle pointer
//		Type: unsigned char*, Name: rom serial number buffer pointer
//Return Value: 		API Error Code
//***************************************************************************************//
DLL_API ERRORCODE UAI_SpectrometerGetRomSerialNumber(void* api_handle, unsigned char *serial);

//***************************************************************************************//
//Function Name: 	UAI_SpectrometerGetDeviceAmount
//Input Arguments:	
//		Type: unsigned int, Name: vendor id code
//		Type: unsigned int, Name: product id code
//		Type: unsigned int*, Name: device amount buffer pointer
//Return Value: 		API Error Code
//***************************************************************************************//
DLL_API ERRORCODE UAI_SpectrometerGetDeviceAmount(unsigned int vid, unsigned int pid, unsigned int* dev);

//***************************************************************************************//
//Function Name: 	UAI_SpectrometerGetDeviceList
//Input Arguments:	
//		Type: unsigned int*, Name: number of the list size
//		Type: unsigned int*, Name: the buffer pointer of the list
//Return Value: 		API Error Code
//***************************************************************************************//
DLL_API ERRORCODE UAI_SpectrometerGetDeviceList(unsigned int* number, unsigned int* list);

//***************************************************************************************//
//Function Name: 	UAI_SpectrometerOpen
//Input Arguments:	
//		Type: unsigned int, Name: device number
//		Type: void**, Name: reference of device handle pointer
//		Type: unsigned int, Name: vendor id code
//		Type: unsigned int, Name: product id code
//Return Value: 		API Error Code
//***************************************************************************************//
DLL_API ERRORCODE UAI_SpectrometerOpen(unsigned int dev, void** handle, unsigned int vid, unsigned int pid);
//***************************************************************************************//
//Function Name: 	UAI_SpectrometerOpen_NetIP
//Input Arguments:	
//		Type: void**, Name: reference of IP address
//		Type: void**, Name: reference of device handle pointer
//		Type: unsigned int, type of device //silcon lab:0 , z5:1
//Return Value: 		API Error Code
//***************************************************************************************//
DLL_API ERRORCODE UAI_SpectrometerOpen_NetIP( char* , void**  ,unsigned int );
//***************************************************************************************//
//Function Name: 	UAI_SpectrometerOpen_NetSocket
//Input Arguments:	
//		Type: void**, Name: reference of socket handle pointer
//		Type: void**, Name: reference of device handle pointer
//		Type: unsigned int, type of device //silcon lab:0 , z5:1
//Return Value: 		API Error Code
//***************************************************************************************//
DLL_API ERRORCODE UAI_SpectrometerOpen_NetSocket(void** , void** ,unsigned int );

//***************************************************************************************//
//Function Name: 	UAI_SpectrometerClose
//Input Arguments:	
//		Type: void*, Name: device handle pointer
//Return Value: 		API Error Code
//***************************************************************************************//
DLL_API ERRORCODE UAI_SpectrometerClose(void* handle);
//***************************************************************************************//
//Function Name: 	UAI_SpectrometerClose_NetIP
//Input Arguments:	
//		Type: void*, Name: device handle pointer
//Return Value: 		API Error Code
//***************************************************************************************//
DLL_API ERRORCODE UAI_SpectrometerClose_NetIP(void* handle);

//***************************************************************************************//
//Function Name: 	UAI_SpectrometerGetExternalPort
//Input Arguments:	
//		Type: void*, Name: device handle pointer
//		Type: unsigned char*, Name: trigger port buffer pointer
//Return Value: 		API Error Code
//***************************************************************************************//
DLL_API ERRORCODE UAI_SpectrometerGetExternalPort(void* api_handle, unsigned int *port);

//***************************************************************************************//
//Function Name: 	UAI_SpectrometerSetExternalPort
//Input Arguments:	
//		Type: void*, Name: device handle pointer
//		Type: unsigned char*, Name: trigger port data
//Return Value: 		API Error Code
//***************************************************************************************//
DLL_API ERRORCODE UAI_SpectrometerSetExternalPort(void* api_handle, unsigned int port);

//***************************************************************************************//
//Function Name: 	UAI_SpectrometerGetTriggerGroupIntegrationTime
//Input Arguments:	
//		Type: void*, Name: device handle pointer
//		Type: unsigned int*, Name: pointer to counts of group trigger 
//		Type: unsigned int*, Name: pointer to integration times for group trigger
//Return Value: 		API Error Code
//***************************************************************************************//
DLL_API ERRORCODE UAI_SpectrometerGetTriggerGroupIntegrationTime(void*, unsigned int*, unsigned int*);

//***************************************************************************************//
//Function Name: 	UAI_SpectrometerSetTriggerGroupIntegrationTime
//Input Arguments:	
//		Type: void*, Name: device handle pointer
//		Type: unsigned int, Name: counts of group trigger 
//		Type: unsigned int*, Name: pointer to integration times for group trigger
//Return Value: 		API Error Code
//***************************************************************************************//
DLL_API ERRORCODE UAI_SpectrometerSetTriggerGroupIntegrationTime(void*, unsigned int, unsigned int*);

//***************************************************************************************//
//Function Name: 	UAI_SpectrometerCheckTriggerDone
//Input Arguments:	
//		Type: void*, Name: device handle pointer
//		Type: unsigned int*, Name: pointer to read the counts of DONE triggers
//Return Value: 		API Error Code
//***************************************************************************************//
DLL_API ERRORCODE UAI_SpectrometerCheckTriggerDone(void*, unsigned int*);


//***************************************************************************************//
//Function Name: 	UAI_SpectrometerGetTriggerData
//Input Arguments:	
//		Type: void*, Name: device handle pointer
//		Type: unsigned int, Name: frame size of the spectrum
//		Type: unsigned int, Name: index of the spectrum
//		Type: float*, Name: pointer to read spectrum
//Return Value: 		API Error Code
//***************************************************************************************//
DLL_API ERRORCODE UAI_SpectrometerGetTriggerData(void*, unsigned int, unsigned int, float*);

//***************************************************************************************//
//Function Name: 	UAI_SpectrometerGetTriggerDataAll
//Input Arguments:	
//		Type: void*, Name: device handle pointer
//		Type: unsigned int, Name: frame size of the spectrum
//		Type: unsigned int, Name: number of the spectrum
//		Type: float*, Name: pointer to read spectrum
//Return Value: 		API Error Code
//***************************************************************************************//
DLL_API ERRORCODE UAI_SpectrometerGetTriggerDataAll(void* ,  unsigned int , unsigned int  , float *);

//***************************************************************************************//
//Function Name: 	UAI_SpectrometerGetTriggerDataBlock
//Input Arguments:	
//		Type: void*, Name: device handle pointer
//		Type: unsigned int, Name: frame size of the spectrum
//		Type: unsigned int, Name: start index of the spectrum
//		Type: unsigned int, Name: number of the spectrum
//		Type: float*, Name: pointer to read spectrum
//Return Value: 		API Error Code
//***************************************************************************************//
DLL_API ERRORCODE UAI_SpectrometerGetTriggerDataBlock(void* ,  unsigned int , unsigned int ,unsigned int  , float *);

//***************************************************************************************//
//Function Name: 	DLI_SpectrometerCheckDoneAndGetTriggerData
//Input Arguments:	
//		Type: void*, Name: device handle pointer
//		Type: unsigned int, Name: frame size of the spectrum
//		Type: unsigned int, Name: index of the spectrum
//		Type: float*, Name: pointer to read spectrum
//Return Value: 		API Error Code, API_INT_DATA_NOT_READY means the trigger data
//                      does not ready
//***************************************************************************************//
DLL_API ERRORCODE UAI_SpectrometerCheckDoneAndGetTriggerData(void*, unsigned int, unsigned int, float*);

//***************************************************************************************//
//Function Name: 	DLI_SpectrometerCheckDoneAndGetTriggerDataRAW
//Input Arguments:	
//		Type: void*, Name: device handle pointer
//		Type: unsigned int, Name: frame size of the spectrum
//		Type: unsigned int, Name: index of the spectrum
//		Type: float*, Name: pointer to read spectrum
//Return Value: 		API Error Code, API_INT_DATA_NOT_READY means the trigger data
//                      does not ready
//***************************************************************************************//
DLL_API ERRORCODE UAI_SpectrometerCheckDoneAndGetTriggerDataRAW(void*, unsigned int, unsigned int, float*);

//***************************************************************************************//
//Function Name: 	UAI_SpectrometerGetTriggerIO
//Input Arguments:	
//		Type: void*, Name: device handle pointer
//		Type: unsigned int*, Name: trigger io enable pointer
//		Type: unsigned int*, Name: trigger io timeout pointer
//      Type: unsigned int*, Name: trigger io level pointer
//Return Value: 		API Error Code
//***************************************************************************************//
DLL_API ERRORCODE UAI_SpectrometerGetTriggerIO(void*, unsigned int*, unsigned int* , unsigned int*);

//***************************************************************************************//
//Function Name: 	UAI_SpectrometerSetTriggerIO
//Input Arguments:	
//		Type: void*, Name: device handle pointer
//		Type: unsigned int, Name: trigger io enable (color&0xFFFFFFF0, enable&0x0000000F)
//		Type: unsigned int, Name: trigger io timeout q04
//		Type: unsigned int, Name: trigger io level
//Return Value: 		API Error Code
//***************************************************************************************//
DLL_API ERRORCODE UAI_SpectrometerSetTriggerIO(void*, unsigned int,unsigned int, unsigned int);

//***************************************************************************************//
//Function Name: 	DLI_SpectrometerSetPWM
//Input Arguments:	
//		Type: void*, Name: device handle pointer
//		Type: unsigned int, Name: channel number of PWM
//		Type: unsigned int, Name: pointer of returned PWM frequency
//		Type: unsigned int, Name: PWM duty
//Return Value: 		API Error Code
//***************************************************************************************//
DLL_API ERRORCODE UAI_SpectrometerSetPWM(void* api_handle, unsigned int channel,unsigned int frequency, unsigned int duty);

//***************************************************************************************//
//Function Name: 	UAI_SpectrometerGetPWM
//Input Arguments:	
//		Type: void*, Name: device handle pointer
//		Type: unsigned int, Name: channel number of PWM
//		Type: unsigned int*, Name: pointer of returned PWM frequency
//		Type: unsigned int*, Name: pointer of returned PWM duty
//Return Value: 		API Error Code
//***************************************************************************************//
DLL_API ERRORCODE UAI_SpectrometerGetPWM(void* api_handle, unsigned int channel,unsigned int *frequency, unsigned int *duty);

//***************************************************************************************//
//Function Name: 	UAI_SpectrometerStopPHOTODIODE
//Input Arguments:	
//		Type: void*, Name: device handle pointer
//		Type: unsigned int, Name: channel of photo diod
//		Type: unsigned int, Name: pointer of returned photo diod value
//Return Value: 		API Error Code
//***************************************************************************************//
DLL_API ERRORCODE UAI_SpectrometerStopPHOTODIODE(void* api_handle, unsigned int channel);

//***************************************************************************************//
//Function Name: 	DLI_SpectrometerGetPHOTODIODE
//Input Arguments:	
//		Type: void*, Name: device handle pointer
//		Type: unsigned int, Name: channel of photo diod
//		Type: unsigned int, Name: pointer of returned photo diod value
//Return Value: 		API Error Code
//***************************************************************************************//
DLL_API ERRORCODE UAI_SpectrometerGetPHOTODIODE(void* api_handle, unsigned int channel, unsigned int *value);

//***************************************************************************************//
//Function Name: 	UAI_SpectrometerWavelengthAcquire
//Input Arguments:	
//		Type: void*, Name: device handle pointer
//		Type: unsigned float*, Name: wavelength buffer
//Return Value: 		API Error Code
//***************************************************************************************//
DLL_API ERRORCODE UAI_SpectrometerGetIntegrationTime(void* api_handle, unsigned int *it_us);
//***************************************************************************************//
//Function Name: 	DLI_SpectrometerGetIntegrationTime
//Input Arguments:	
//		Type: void*, Name: device handle pointer
//		Type: unsigned int, Name: value of integration time
//Return Value: 		API Error Code
//***************************************************************************************//
DLL_API ERRORCODE UAI_SpectrometerSetIntegrationTime(void* api_handle, unsigned int it_us);

//***************************************************************************************//
//Function Name: 	DLI_SpectrometerSetIntegrationTime
//Input Arguments:	
//		Type: void, Name: device handle pointer
//		Type: unsigned int, Name: value of integration time
//Return Value: 		API Error Code
//***************************************************************************************//
DLL_API ERRORCODE UAI_SpectrometerWavelengthAcquire(void* api_handle, float *buffer);

//***************************************************************************************//
//Function Name: 	UAI_SpectrometerDataAcquire
//Input Arguments:	
//		Type: void*, Name: device handle pointer
//		Type: unsigned int, Name: integration time
//		Type: unsigned float*, Name: spectrum intensity data buffer pointer
//		Type: unsigned int, Name: continuous average times
//Return Value: 		API Error Code
//***************************************************************************************//

DLL_API ERRORCODE UAI_SpectrometerWavelengthAcquireRaw(void* api_handle, float *buffer);

//***************************************************************************************//
//Function Name: 	UAI_SpectrometerWavelengthAcquireRaw
//Input Arguments:	
//		Type: void*, Name: device handle pointer
//		Type: unsigned int, Name: integration time
//		Type: unsigned float*, Name: spectrum intensity data buffer pointer
//		Type: unsigned int, Name: continuous average times
//Return Value: 		API Error Code
//***************************************************************************************//

DLL_API ERRORCODE UAI_SpectrometerDataAcquire(void* api_handle, unsigned int integration_time_us, float *buffer, unsigned int average);


//DLL_API ERRORCODE UAI_SpectrometerDataAcquireHWAvg(HANDLE api_handle, UINT32 integration_time_us, UINT32 frame_size, float *buffer, UINT32 average)
//***************************************************************************************//
//Function Name: 	UAI_SpectrometerDataAcquireHWAvg
//Input Arguments:	
//		Type: void*, Name: device handle pointer
//		Type: unsigned int, Name: integration time
//		Type: unsigned int, Name: frame size
//		Type: unsigned float*, Name: spectrum intensity data buffer pointer
//		Type: unsigned int, Name: continuous average times
//Return Value: 		API Error Code
//***************************************************************************************//
DLL_API ERRORCODE UAI_SpectrometerDataAcquireHWAvg(void* api_handle, unsigned int integration_time_us, unsigned int frame_size, float *buffer, unsigned int average);

//***************************************************************************************//
//Function Name: 	UAI_SpectrometerDataOneshot
//Input Arguments:	
//		Type: void*, Name: device handle pointer
//		Type: unsigned int, Name: integration time
//		Type: unsigned float*, Name: spectrum intensity data buffer pointer
//		Type: unsigned int, Name: continuous average times
//Return Value: 		API Error Code
//***************************************************************************************//
DLL_API ERRORCODE UAI_SpectrometerDataOneshot(void* api_handle, unsigned int integration_time_us, float *buffer, unsigned int average);

//***************************************************************************************//
//Function Name: 	UAI_SpectrometerDataOneshots
//Input Arguments:	
//		Type: void*, Name: device handle pointer
//		Type: unsigned int, Name: integration time
//		Type: unsigned float*, Name: spectrum intensity data buffer pointer
//		Type: unsigned int, Name: continuous average times
//Return Value: 		API Error Code
//***************************************************************************************//
DLL_API ERRORCODE UAI_SpectrometerDataOneshots(void* api_handle, unsigned int integration_time_us, float *buffer, unsigned int average);
//***************************************************************************************//
//Function Name: 	UAI_SpectrometerDataOneshotRaw
//Input Arguments:	
//		Type: void*, Name: device handle pointer
//		Type: unsigned int, Name: integration time
//		Type: unsigned float*, Name: spectrum intensity data buffer pointer
//		Type: unsigned int, Name: continuous average times
//Return Value: 		API Error Code
//***************************************************************************************//
DLL_API ERRORCODE UAI_SpectrometerDataOneshotRaw(void* api_handle, unsigned int integration_time_us, float *buffer, unsigned int average);

//***************************************************************************************//
//Function Name: 	UAI_SpectrometerDataOneshotWithTime
//Input Arguments:	
//		Type: void*, Name: device handle pointer
//		Type: unsigned int, Name: spectrum frame size
//		Type: unsigned float*, Name: spectrum intensity data buffer pointer
//		Type: unsigned int, Name: continuous average times
//Return Value: 		API Error Code
//***************************************************************************************//
DLL_API ERRORCODE UAI_SpectrometerDataOneshotWithTime(void* api_handle, unsigned int integration_time_us, float *buffer, unsigned int average);

//***************************************************************************************//
//Function Name: 	UAI_SpectrometerDataOneshotWithTimeRaw
//Input Arguments:	
//		Type: void*, Name: device handle pointer
//		Type: unsigned int, Name: spectrum frame size
//		Type: unsigned float*, Name: spectrum intensity data buffer pointer
//		Type: unsigned int, Name: continuous average times
//Return Value: 		API Error Code
//***************************************************************************************//
DLL_API ERRORCODE UAI_SpectrometerDataOneshotWithTimeRaw(void* api_handle, unsigned int integration_time_us, float *buffer, unsigned int average);


//***************************************************************************************//
//Function Name: 	UAI_SpectrometerLEDMeasure
//Input Arguments:	
//		Type: void*, Name: device handle pointer
//		Type: unsigned int, Name: integration time
//		Type: unsigned float*, Name: spectrum lambda data buffer pointer
//		Type: unsigned float*, Name: spectrum intensity data buffer pointer
//		Type: unsigned int, Name: continuous average times
//		Type: unsigned int, Name: Select 0: No intensity calibration / 1:AbsoluteIntensityCalibration / 2:ContrastIntensityCalibration
//		Type: unsigned int, Name: Select Index of Intensity table
//Return Value: 		API Error Code
//***************************************************************************************//
DLL_API ERRORCODE UAI_SpectrometerLEDMeasure(void* api_handle, unsigned int integration_time_us, unsigned int framesize, float *lambda, float *intensity, unsigned int average, unsigned int IntensityType, unsigned int IntensityTableIndex);


//***************************************************************************************//
//Function Name: 	UAI_SpectrometerTriggerDataAcquire
//Input Arguments:	
//		Type: void*, Name: device handle pointer
//		Type: unsigned float*, Name: spectrum intensity data buffer pointer
//Return Value: 		API Error Code
//***************************************************************************************//
DLL_API ERRORCODE UAI_SpectrometerTriggerDataAcquire(void*, float *);

//***************************************************************************************//
//Function Name: 	UAI_SpectromoduleGetRomVersion
//Input Arguments:	
//		Type: void*, Name: device handle pointer
//		Type: unsigned short*, Name: rom version buffer pointer
//Return Value: 		API Error Code
//***************************************************************************************//
DLL_API ERRORCODE UAI_SpectromoduleGetRomVersion(void* api_handle, unsigned short *version);

//***************************************************************************************//
//Function Name: 	UAI_SpectromoduleGetModelName
//Input Arguments:	
//		Type: void*, Name: device handle pointer
//		Type: unsigned char*, Name: model name buffer pointer
//Return Value: 		API Error Code
//***************************************************************************************//
DLL_API ERRORCODE UAI_SpectromoduleGetModelName(void* api_handle, unsigned char *model);

//***************************************************************************************//
//Function Name: 	UAI_SpectromoduleGetSerialNumber
//Input Arguments:	
//		Type: void*, Name: device handle pointer
//		Type: unsigned char*, Name: serial number buffer pointer
//Return Value: 		API Error Code
//***************************************************************************************//
DLL_API ERRORCODE UAI_SpectromoduleGetSerialNumber(void* api_handle, unsigned char *serial);

//***************************************************************************************//
//Function Name: 	UAI_SpectromoduleGetManufacturingDate
//Input Arguments:	
//		Type: void*, Name: device handle pointer
//		Type: unsigned short*, Name: manufacturing date buffer pointer
//Return Value: 		API Error Code
//***************************************************************************************//
DLL_API ERRORCODE UAI_SpectromoduleGetManufacturingDate(void* api_handle, unsigned char *date);

//***************************************************************************************//
//Function Name: 	UAI_SpectromoduleGetSlitType
//Input Arguments:	
//		Type: void*, Name: device handle pointer
//		Type: unsigned char*, Name: type buffer pointer
//Return Value: 		API Error Code
//***************************************************************************************//
DLL_API ERRORCODE UAI_SpectromoduleGetSlitType(void* api_handle, unsigned char *type);

//***************************************************************************************//
//Function Name: 	UAI_SpectromoduleGetSlitSerialNumber
//Input Arguments:	
//		Type: void*, Name: device handle pointer
//		Type: unsigned char*, Name: serial number buffer pointer
//Return Value: 		API Error Code
//***************************************************************************************//
DLL_API ERRORCODE UAI_SpectromoduleGetSlitSerialNumber(void* api_handle, unsigned char *serial);

//***************************************************************************************//
//Function Name: 	UAI_SpectromoduleGetRomSerialNumber
//Input Arguments:	
//		Type: void*, Name: device handle pointer
//		Type: unsigned char*, Name: serial number buffer pointer
//Return Value: 		API Error Code
//***************************************************************************************//
DLL_API ERRORCODE UAI_SpectromoduleGetRomSerialNumber(void* api_handle, unsigned char *serial);

//***************************************************************************************//
//Function Name: 	UAI_SpectromoduleGetSensorType
//Input Arguments:	
//		Type: void*, Name: device handle pointer
//		Type: unsigned char*, Name: type buffer pointer
//Return Value: 		API Error Code
//***************************************************************************************//
DLL_API ERRORCODE UAI_SpectromoduleGetSensorType(void* api_handle, unsigned char *type);

//***************************************************************************************//
//Function Name: 	UAI_SpectromoduleGetSensorSerialNumber
//Input Arguments:	
//		Type: void*, Name: device handle pointer
//		Type: unsigned char*, Name: serial number buffer pointer
//Return Value: 		API Error Code
//***************************************************************************************//
DLL_API ERRORCODE UAI_SpectromoduleGetSensorSerialNumber(void* api_handle, unsigned char *serial);

//***************************************************************************************//
//Function Name: 	UAI_SpectromoduleGetFrameSize
//Input Arguments:	
//		Type: void*, Name: device handle pointer
//		Type: unsigned short*, Name: frame size buffer pointer
//Return Value: 		API Error Code
//***************************************************************************************//
DLL_API ERRORCODE UAI_SpectromoduleGetFrameSize(void* api_handle, unsigned short *size);

//***************************************************************************************//
//Function Name: 	UAI_SpectromoduleGetFrameSizeRaw
//Input Arguments:	
//		Type: void*, Name: device handle pointer
//		Type: unsigned short*, Name: frame size buffer pointer
//Return Value: 		API Error Code
//***************************************************************************************//
DLL_API ERRORCODE UAI_SpectromoduleGetFrameSizeRaw(void* api_handle, unsigned short *size);


//***************************************************************************************//
//Function Name: 	UAI_SpectromoduleGetWavelengthCalibrationCoefficients
//Input Arguments:	
//		Type: void*, Name: device handle pointer
//		Type: double*, Name: coefficients pointer
//Return Value: 		API Error Code
//***************************************************************************************//
DLL_API ERRORCODE UAI_SpectromoduleGetWavelengthCalibrationCoefficients(void* api_handle, double *coefficients);

//20160912 Kevin
//***************************************************************************************//
//Function Name: 	UAI_SpectromoduleSetWavelengthCalibrationCoefficients
//Input Arguments:	
//		Type: void*, Name: device handle pointer
//		Type: double*, Name: coefficients pointer
//Return Value: 		API Error Code
//***************************************************************************************//
DLL_API ERRORCODE UAI_SpectromoduleSetWavelengthCalibrationCoefficients(void* api_handle, double *coefficients);

//***************************************************************************************//
//Function Name: 	UAI_SpectromoduleGetWavelengthStart
//Input Arguments:	
//		Type: void*, Name: device handle pointer
//		Type: float*, Name: start lumda buffer pointer
//Return Value: 		API Error Code
//***************************************************************************************//
DLL_API ERRORCODE UAI_SpectromoduleGetWavelengthStart(void* api_handle, float* lumda);

//***************************************************************************************//
//Function Name: 	UAI_SpectromoduleGetWavelengthEnd
//Input Arguments:	
//		Type: void*, Name: device handle pointer
//		Type: float, Name: end lumda buffer pointer
//Return Value: 		API Error Code
//***************************************************************************************//
DLL_API ERRORCODE UAI_SpectromoduleGetWavelengthEnd(void* api_handle, float* lumda);

//***************************************************************************************//
//Function Name: 	UAI_SpectromoduleGetMinimumIntegrationTime
//Input Arguments:	
//		Type: void*, Name: device handle pointer
//		Type: unsigned int, Name: rom address
//		Type: unsigned int, Name: data size
//		Type: unsigned char*, Name: data buffer pointer
//Return Value: 		API Error Code
//***************************************************************************************//
DLL_API ERRORCODE UAI_SpectromoduleGetMinimumIntegrationTime(void* api_handle, unsigned short* time);

//***************************************************************************************//
//Function Name: 	UAI_SpectromoduleGetMaximumIntegrationTime
//Input Arguments:	
//		Type: void*, Name: device handle pointer
//		Type: unsigned int, Name: rom address
//		Type: unsigned int, Name: data size
//		Type: unsigned char*, Name: data buffer pointer
//Return Value: 		API Error Code
//***************************************************************************************//
DLL_API ERRORCODE UAI_SpectromoduleGetMaximumIntegrationTime(void* api_handle, unsigned short* time);

//***************************************************************************************//
//Function Name: 	UAI_SpectromoduleSetIntensityCalibration
//Input Arguments:	
//		Type: void*, Name: device handle pointer
//		Type: double*, Name: intensity gain buffer 
//		Type: unsigned short, Name: date code
//		Type: unsigned int, Name: integration time in us
//		Type: double, Name: NormalizationFactor
//Return Value: 		API Error Code
//***************************************************************************************//
DLL_API ERRORCODE UAI_SpectromoduleSetIntensityCalibration(void* api_handle, double *gain, unsigned short date, unsigned int integration_time,double NormalizationFactor);

//***************************************************************************************//
//Function Name: 	UAI_SpectromoduleUserSetIntensityCalibration
//Input Arguments:	
//		Type: void*, Name: device handle pointer
//		Type: double*, Name: intensity gain buffer 
//		Type: unsigned short, Name: date code
//		Type: unsigned int, Name: integration time in us
//		Type: double, Name: NormalizationFactor
//		Type: unsigned int, Name: index of tables
//Return Value: 		API Error Code
//***************************************************************************************//
DLL_API ERRORCODE UAI_SpectromoduleUserSetIntensityCalibration(void* api_handle, double *gain, unsigned short date, unsigned int integration_time,double NormalizationFactor, unsigned int index);


//***************************************************************************************//
//Function Name: 	UAI_SpectromoduleGetIntensityCalibration
//Input Arguments:	
//		Type: void*, Name: device handle pointer
//		Type: double*, Name: intensity gain buffer 
//		Type: unsigned short*, Name: buffer of date code
//		Type: unsigned int*, Name: buffer of integration time in us
//		Type: double*, Name: NormalizationFactor
//Return Value: 		API Error Code
//***************************************************************************************//
DLL_API ERRORCODE UAI_SpectromoduleGetIntensityCalibration(void* api_handle, double *gain, unsigned short *date, unsigned int *integration_time, double *NormalizationFactor);

//***************************************************************************************//
//Function Name: 	UAI_SpectromoduleUserGetIntensityCalibration
//Input Arguments:	
//		Type: void*, Name: device handle pointer
//		Type: double*, Name: intensity gain buffer 
//		Type: unsigned short*, Name: buffer of date code
//		Type: unsigned int*, Name: buffer of integration time in us
//		Type: double*, Name: NormalizationFactor
//		Type: unsigned int, Name: index of intensity table
//Return Value: 		API Error Code
//***************************************************************************************//
DLL_API ERRORCODE UAI_SpectromoduleUserGetIntensityCalibration(void* api_handle, double *gain, unsigned short *date, unsigned int *integration_time, double *NormalizationFactor, unsigned int index);


//***************************************************************************************//
//Function Name: 	UAI_SpectromoduleGetLinearityCalibrationCoefficients
//Input Arguments:	
//		Type: void*, Name: device handle pointer
//		Type: double*, Name: coefficients buffer 
//Return Value: 		API Error Code
//***************************************************************************************//
DLL_API ERRORCODE UAI_SpectromoduleGetLinearityCalibrationCoefficients(void* api_handle, double *coefficients);

//***************************************************************************************//
//Function Name: 	UAI_SpectrometerInitUserRom
//Input Arguments:	
//		Type: void*, Name: device handle pointer
//Return Value: 		API Error Code
//***************************************************************************************//
DLL_API ERRORCODE UAI_SpectrometerInitUserRom(void* api_handle);

//***************************************************************************************//
//Function Name: 	UAI_SpectrometerSetUserRom
//Input Arguments:	
//		Type: void*, Name: device handle pointer
//		Type: unsigned char*, Name: buffer of data
//		Type: unsigned int, Name: length of data
//		Type: unsigned int, Name: offset in the UserROM
//Return Value: 		API Error Code
//***************************************************************************************//
DLL_API ERRORCODE UAI_SpectrometerSetUserRom(void* api_handle, unsigned char *buffer, unsigned int length, unsigned int offset);

//***************************************************************************************//
//Function Name: 	UAI_SpectrometerGetUserRom
//Input Arguments:	
//		Type: void*, Name: device handle pointer
//		Type: unsigned char*, Name: buffer of data
//		Type: unsigned int, Name: length of data
//		Type: unsigned int, Name: offset in the UserROM
//Return Value: 		API Error Code
//***************************************************************************************//
DLL_API ERRORCODE UAI_SpectrometerGetUserRom(void* api_handle, unsigned char *buffer, unsigned int length, unsigned int offset);

//===================================================================//

//***************************************************************************************//
//Function Name: 	UAI_StrayLightCorrection
//Input Arguments:	
//		Type: void*, Name: device handle pointer
//		Type: float*, Name: source buffer pointer
//Return Value: 		API Error Code
//***************************************************************************************//
DLL_API ERRORCODE UAI_StrayLightCorrection(void* api_handle, float* source);

//***************************************************************************************//
//Function Name: 	UAI_BackgroundRemove
//Input Arguments:	
//		Type: void*, Name: device handle pointer
//		Type: unsigned int, Name: integration time in us
//		Type: float*, Name: source buffer pointer which the background level will be substracted from the source data
//Return Value: 		API Error Code
//***************************************************************************************//
DLL_API ERRORCODE UAI_BackgroundRemove(void* api_handle, unsigned int integration_time, float* source);

//***************************************************************************************//
//Function Name: 	UAI_BackgroundRemoveWithAVG
//Input Arguments:	
//		Type: void*, Name: device handle pointer
//		Type: unsigned int, Name: integration time in us
//		Type: float*, Name: source buffer pointer which the background level will be substracted from the source data
//Return Value: 		API Error Code
//***************************************************************************************//
DLL_API ERRORCODE UAI_BackgroundRemoveWithAVG(void* api_handle, unsigned int integration_time, float* source);

//***************************************************************************************//
//Function Name: 	UAI_ContrastIntensityCorrection
//Input Arguments:	
//		Type: void*, Name: device handle pointer
//		Type: float*, Name: source buffer pointer
//Return Value: 		API Error Code
//***************************************************************************************//
DLL_API ERRORCODE UAI_ContrastIntensityCorrection(void* api_handle, float* source);

//***************************************************************************************//
//Function Name: 	UAI_ContrastIntensityCorrection
//Input Arguments:	
//		Type: void*, Name: device handle pointer
//		Type: float*, Name: source buffer pointer
//		Type: unsigned int, Name: select index of calibration table
//Return Value: 		API Error Code
//***************************************************************************************//
DLL_API ERRORCODE UAI_ContrastUserIntensityCorrection(void* api_handle, float* source, unsigned int  index);

//***************************************************************************************//
//Function Name: 	UAI_AbsoluteIntensityCorrection
//Input Arguments:	
//		Type: void*, Name: device handle pointer
//		Type: float*, Name: source buffer pointer
//		Type: unsigned int, Name: integration time in us
//Return Value: 		API Error Code
//***************************************************************************************//
DLL_API ERRORCODE UAI_AbsoluteIntensityCorrection(void* api_handle, float* source, unsigned int integration_time);

//***************************************************************************************//
//Function Name: 	UAI_AbsoluteIntensityCorrection
//Input Arguments:	
//		Type: void*, Name: device handle pointer
//		Type: float*, Name: source buffer pointer
//		Type: unsigned int, Name: integration time in us
//		Type: unsigned int, Name: select index of calibration table
//Return Value: 		API Error Code
//***************************************************************************************//
DLL_API ERRORCODE UAI_AbsoluteUserIntensityCorrection(void* api_handle, float* source, unsigned int integration_time , unsigned int  index);

//***************************************************************************************//
//Function Name: 	UAI_LinearityCorrection
//Input Arguments:	
//		Type: void*, Name: device handle pointer
//		Type: unsigned int, Name: frame size
//		Type: float*, Name: data buffer
//Return Value: 		API Error Code
//***************************************************************************************//
DLL_API ERRORCODE UAI_LinearityCorrection(void* api_handle, unsigned int frame_size, float* data);

//***************************************************************************************//
//Function Name: 	UAI_DoIntensityCalibration
//Input Arguments:	
//		Type: void*, Name: device handle pointer
//		Type: unsigned int, Name: standard spectrum data number
//		Type: float*, Name: standard spectrum wavelength data buffer pointer
//		Type: float*, Name: standard spectrum intensity data buffer pointer
//		Type: float*, Name: meanure spectrum intensity data buffer pointer
//		Type: unsigned int, Name: integration time in microsecond
//		Type: unsigned short, Name: calibration date code
//Return Value: 		API Error Code
//***************************************************************************************//
DLL_API ERRORCODE UAI_DoIntensityCalibration(void* api_handle, unsigned int std_size, float *std_lambda, float *std_intensity, float *m_intensity, unsigned int integration_time, unsigned short date);

//***************************************************************************************//
//Function Name: 	UAI_DoIntensityCalibration
//Input Arguments:	
//		Type: void*, Name: device handle pointer
//		Type: unsigned int, Name: standard spectrum data number
//		Type: float*, Name: standard spectrum wavelength data buffer pointer
//		Type: float*, Name: standard spectrum intensity data buffer pointer
//		Type: float*, Name: meanure spectrum intensity data buffer pointer
//		Type: unsigned int, Name: integration time in microsecond
//		Type: unsigned short, Name: calibration date code
//		Type: unsigned int, Name: Name: index of table 
//Return Value: 		API Error Code
//***************************************************************************************//
DLL_API ERRORCODE UAI_DoUserIntensityCalibration(void* api_handle, unsigned int std_size, float *std_lambda, float *std_intensity, float *m_intensity, unsigned int integration_time, unsigned short date,unsigned int index);

//***************************************************************************************//
//Function Name: 	UAI_ColorGetXYZRef
//Input Arguments:	
//		Type: void*, Name: spectrum information handler
//		Type: double *, Name: XYZ
//Return Value: 		API Error Code
//***************************************************************************************//
DLL_API ERRORCODE UAI_ColorGetXYZRef(void* color, double* XYZ);

//***************************************************************************************//
//Function Name: 	UAI_ColorGetxyzRef
//Input Arguments:	
//		Type: void*, Name: color information handler
//		Type: double *, Name: xyz
//Return Value: 		API Error Code
//***************************************************************************************//
DLL_API ERRORCODE UAI_ColorGetxyzRef(void* color, double* xyz);


//***************************************************************************************//
//Function Name: 	UAI_ColorGetRadiantPower
//Input Arguments:	
//		Type: void*, Name: color information handler
//		Type: double *, Name: radiantpower
//Return Value: 		API Error Code
//***************************************************************************************//
DLL_API ERRORCODE UAI_ColorGetRadiantPower(void* color, double* radiantpower);

//***************************************************************************************//
//Function Name: 	UAI_ColorGetPPFD
//Input Arguments:	
//		Type: void*, Name: color information handler
//		Type: double *, Name: PPFD
//Return Value: 		API Error Code
//***************************************************************************************//
DLL_API ERRORCODE UAI_ColorGetPPFD(void* color, double* PPFD);

//***************************************************************************************//
//Function Name: 	UAI_ColorGetXYZ
//Input Arguments:	
//		Type: void*, Name: spectrum information handler
//		Type: double *, Name: XYZ
//Return Value: 		API Error Code
//***************************************************************************************//
DLL_API ERRORCODE UAI_ColorGetXYZ(void* color, double* XYZ);

//20180108 Kevin
//DLL_API ERRORCODE UAI_ColorGetPhotonFlow(float *lambda,float *intensity_watt , unsigned int framesize)
//***************************************************************************************//
//Function Name: 	UAI_ColorGetPhotonFlow
//Input Arguments:	
//		Type: float*, Name: Point of wavelength array
//		Type: float*, Name: Point of intensity array
//		Type: unsigned int, Name: size of intensity and wavelength array 
//Return Value: 		API Error Code
//***************************************************************************************//
DLL_API ERRORCODE UAI_ColorGetPhotonFlow(float *lambda,float *intensity_watt , unsigned int framesize);

//***************************************************************************************//
//Function Name: 	UAI_ColorSetXYZValue
//Input Arguments:	
//		Type: void*, Name: spectrum information handler
//		Type: double , Name: X
//		Type: double , Name: Y
//		Type: double , Name: Z
//Return Value: 		API Error Code
//***************************************************************************************//
DLL_API ERRORCODE UAI_ColorSetXYZValue(void* color, double X ,double Y,double Z);

//***************************************************************************************//
//Function Name: 	UAI_ColorGetxyz
//Input Arguments:	
//		Type: void*, Name: color information handler
//		Type: double *, Name: xyz
//Return Value: 		API Error Code
//***************************************************************************************//
DLL_API ERRORCODE UAI_ColorGetxyz(void* color, double* xyz);

//***************************************************************************************//
//Function Name: 	UAI_ColorGetLab
//Input Arguments:	
//		Type: void*, Name: color information handler
//		Type: double *, Name: xyz
//Return Value: 		API Error Code
//***************************************************************************************//
DLL_API ERRORCODE UAI_ColorGetLab(void* color, double* Lab);

//***************************************************************************************//
//Function Name: 	UAI_ColorGetHunterLab
//Input Arguments:	
//		Type: void*, Name: color information handler
//		Type: double *, Name: Lab
//Return Value: 		API Error Code
//***************************************************************************************//
DLL_API ERRORCODE UAI_ColorGetHunterLab(void* color, double* HLab);

//***************************************************************************************//
//Function Name: 	UAI_ColorGetLuv
//Input Arguments:	
//		Type: void*, Name: color information handler
//		Type: double *, Name: HLab
//Return Value: 		API Error Code
//***************************************************************************************//
DLL_API ERRORCODE UAI_ColorGetLuv(void* color, double* Luv);

//***************************************************************************************//
//Function Name: 	UAI_ColorGetUVW
//Input Arguments:	
//		Type: void*, Name: color information handler
//		Type: double *, Name: Luv
//Return Value: 		API Error Code
//***************************************************************************************//
DLL_API ERRORCODE UAI_ColorGetUVW(void* color, double*UVW);

//***************************************************************************************//
//Function Name: 	UAI_ColorGetuvw
//Input Arguments:	
//		Type: void*, Name: color information handler
//		Type: double *, Name: UVW
//Return Value: 		API Error Code
//***************************************************************************************//
DLL_API ERRORCODE UAI_ColorGetuvw(void* color, double* uvw);

//***************************************************************************************//
//Function Name: 	UAI_ColorGet1960UCS
//Input Arguments:	
//		Type: void*, Name: color information handler
//		Type: double *, Name: uvw
//Return Value: 		API Error Code
//***************************************************************************************//
DLL_API ERRORCODE UAI_ColorGet1960UCS(void* color, double* UVW);

//***************************************************************************************//
//Function Name: 	UAI_ColorGet1960ucs
//Input Arguments:	
//		Type: void*, Name: color information handler
//		Type: double *, Name: UVW
//Return Value: 		API Error Code
//***************************************************************************************//
DLL_API ERRORCODE UAI_ColorGet1960ucs(void* color, double* uvw);

//***************************************************************************************//
//Function Name: 	UAI_ColorGet1976UCS
//Input Arguments:	
//		Type: void*, Name: color information handler
//		Type: double *, Name: uvw
//Return Value: 		API Error Code
//***************************************************************************************//
DLL_API ERRORCODE UAI_ColorGet1976UCS(void* color, double* UVW);

//***************************************************************************************//
//Function Name: 	UAI_ColorGet1976ucs
//Input Arguments:	
//		Type: void*, Name: color information handler
//		Type: double *, Name: UVW
//Return Value: 		API Error Code
//***************************************************************************************//
DLL_API ERRORCODE UAI_ColorGet1976ucs(void* color, double* uvw);

//***************************************************************************************//
//Function Name: 	UAI_ColorGetCCT
//Input Arguments:	
//		Type: void*, Name: color information handler
//		Type: double *, Name: uvw
//Return Value: 		API Error Code
//***************************************************************************************//
DLL_API ERRORCODE UAI_ColorGetCCT(void* color, double* cct);

//***************************************************************************************//
//Function Name: 	UAI_ColorGetDominantWavelength
//Input Arguments:	
//		Type: void*, Name: color information handler
//		Type: double *, Name: correlated color temerature
//Return Value: 		API Error Code
//***************************************************************************************//
DLL_API ERRORCODE UAI_ColorGetDominantWavelength(void* color, double* lumbda_d);

//DLL_API ERRORCODE UAI_ColorGetDominantWavelengthByxy(void* buffer , double x , double y , double x0 , double y0, double* lumbda_d , double* purity)

DLL_API ERRORCODE UAI_ColorGetDominantWavelengthByxy(void* buffer , double x , double y , double x0 , double y0, double* lumbda_d , double* purity);

//***************************************************************************************//
//Function Name: 	UAI_ColorGetPurity
//Input Arguments:	
//		Type: void*, Name: color information handler
//		Type: double *, Name: excitation purity
//Return Value: 		API Error Code
//***************************************************************************************//
DLL_API ERRORCODE UAI_ColorGetPurity(void* color, double* purity_e);

//***************************************************************************************//
//Function Name: 	UAI_ColorGetCIEWhiteness
//Input Arguments:	
//		Type: void*, Name: color information handler
//		Type: double *, Name: CIE Whiteness
//Return Value: 		API Error Code
//***************************************************************************************//
DLL_API ERRORCODE UAI_ColorGetCIEWhiteness(void* color, double* Wcie);

//***************************************************************************************//
//Function Name: 	UAI_ColorGetCIETint
//Input Arguments:	
//		Type: void*, Name: color information handler
//		Type: double *, Name: CIE Tint
//Return Value: 		API Error Code
//***************************************************************************************//
DLL_API ERRORCODE UAI_ColorGetCIETint(void* color, double* Tcie);

//***************************************************************************************//
//Function Name: 	UAI_ColorGetColorRenderingIndex
//Input Arguments:	
//		Type: void*, Name: device handle pointer
//		Type: double*, Name: data buffer pointer
//		Type: double, Name: specified CCT value
//Return Value: 		API Error Code
//***************************************************************************************//
DLL_API ERRORCODE UAI_ColorGetColorRenderingIndex(void* color, double* cri,double cct);

//***************************************************************************************//
//Function Name: 	UAI_ColorGetColorQualityScale
//Input Arguments:	
//		Type: void*, Name: device handle pointer
//		Type: unsigned char*, Name: data buffer pointer
//Return Value: 		API Error Code
//***************************************************************************************//
DLL_API ERRORCODE UAI_ColorGetColorQualityScale(void* color, double* cqs);

//***************************************************************************************//
//Function Name: 	UAI_ColorInformationSet
//Input Arguments:	
//		Type: void**, Name: color information handler's address, it will be returned by the fucntion
//		Type: float*, Name: wavelength data buffer point
//		Type: float*, Name: reference intensity data buffer point
//		Type: float*, Name: measurement intensity data buffer point
//Return Value: 		API Error Code
//***************************************************************************************//
DLL_API ERRORCODE UAI_ColorInformationSet(void *color, float* lumbda, float* intensity_r, float* intensity_m);

//***************************************************************************************//
//Function Name: 	UAI_ColorInformationGet
//Input Arguments:	
//		Type: void**, Name: color information handler's address, it will be returned by the fucntion
//		Type: float*, Name: wavelength data buffer point
//		Type: float*, Name: reference intensity data buffer point
//		Type: float*, Name: measurement intensity data buffer point
//Return Value: 		API Error Code
//***************************************************************************************//
DLL_API ERRORCODE UAI_ColorInformationGet(void *color, float** lumbda, float** intensity_r, float** intensity_m, float** intensity_transmission);

//***************************************************************************************//
//Function Name: 	UAI_ColorInformationAllocation
//Input Arguments:	
//		Type: void*, Name: device handler
//		Type: void**, Name: color information handler's address, it will be returned by the fucntion
//		Type: unsigned int, Name: type of the colormeasurement, 0=relative emission, 1=relaive reflection, 2=absp;ute emission
//		Type: unsigned int, Name: the cie standard illuminant
//		Type: float*, Name: wavelength data buffer point, if NULL, fucntion will allocate one
//		Type: float*, Name: reference intensity data buffer point, if NULL, fucntion will allocate one
//		Type: float*, Name: measurement intensity data buffer point, if NULL, fucntion will allocate one
//		Type: unsigned int, Name: the size of the wavelength buffer
//Return Value: 		API Error Code
//***************************************************************************************//
DLL_API ERRORCODE UAI_ColorInformationAllocation(void*handle,void **color, unsigned int type, unsigned int observer, unsigned int illuminant, float* lumbda, float* intensity_r, float* intensity_m, unsigned int size);

//***************************************************************************************//
//Function Name: 	UAI_ColorInformationFree
//Input Arguments:	
//		Type: void*, Name: color information handler
//Return Value: 		API Error Code
//***************************************************************************************//
DLL_API ERRORCODE UAI_ColorInformationFree(void* color);

//***************************************************************************************//
//Function Name: 	UAI_ColorOperation
//Input Arguments:	
//		Type: void*, Name: color information handler
//Return Value: 		API Error Code
//***************************************************************************************//
DLL_API ERRORCODE UAI_ColorOperation(void* color);

//***************************************************************************************//
//Function Name: 	UAI_MATHGetCurveInfo
//Input Arguments:	
//		Type: float*, pointer of wavelength array
//		Type: float*, pointer of Intensity array
//		Type: Unsigned int , size of Wavelength and Intensity array
//		Type: float, Start wavelength for analyzing peak
//		Type: float, End wavelength for analyzing peak
//		Type: float* , pointer of wavelength of peak
//		Type: float* , pointer of wavelength of CenterWavelength
//		Type: float* , pointer of FWHM of Peak
//Return Value: 		API Error Code
//***************************************************************************************//
DLL_API ERRORCODE UAI_MATHGetCurveInfo(float* Lambda , float* Intensity, unsigned int size ,float Wavelength_start , float Wavelength_end, float* LambdaP, float* CenterWavelength ,  float* FWHM);

//***************************************************************************************//
//Function Name: 	UAI_SMOOTH_Boxcar
//Input Arguments:	
//		Type: float*, pointer of Intensity array
//		Type: Unsigned int , size of Intensity array
//		Type: int , level of boxcar smoothing
//Return Value: 		API Error Code
//***************************************************************************************//
DLL_API ERRORCODE UAI_SMOOTH_Boxcar(float* source,unsigned int size , int width);//20140613 kevin

//***************************************************************************************//
//Function Name: 	UAI_Color_CalculateCorrelatedColorTemperature
//Input Arguments:	
//		Type: double , specified chromaticity coordinates of CIE x
//		Type: double , specified chromaticity coordinates of CIE y
//		Type: double* , pointer of CorrelatedColorTemperature
//Return Value: 		API Error Code
//***************************************************************************************//
DLL_API ERRORCODE UAI_Color_CalculateCorrelatedColorTemperature(double x, double y, double *cct);//20140616 kevin

//***************************************************************************************//
//Function Name: 	UAI_ColorSetColorRenderingIndex
//Input Arguments:	
//		Type: void*, Name: color information handler
//		Type: double , specified CorrelatedColorTemperature
//Return Value: 		API Error Code
//***************************************************************************************//
DLL_API ERRORCODE UAI_ColorSetColorRenderingIndex(void *buffer , double SpecifiedCCT);//20140616 kevin

//***************************************************************************************//
//Function Name: 	UAI_SpectrometerExportDeviceInfo
//Input Arguments:	
//		Type: void*, Name: color information handler
//		Type: char* ,exported file path
//Return Value: 		API Error Code
//***************************************************************************************//
DLL_API ERRORCODE UAI_SpectrometerExportDeviceInfo(void* api_handle,char* Filename);//20151109 kevin

//***************************************************************************************//
//Function Name: 	UAI_ColorGetDuv
//Input Arguments:	
//		Type: double, Name: Input CIE x
//		Type: double, Name: Input CIE y
//		Type: double*, Name: Output duv
//Return Value: 		API Error Code
//***************************************************************************************//
DLL_API ERRORCODE UAI_ColorGetDuv(double x , double y, double* duv);

//***************************************************************************************//
//Function Name: 	UAI_SpectrometerSetPCUSBUnlock
//Input Arguments:	
//		Type: Handle, Name: device handle pointer
//Return Value: 		API Error Code
//***************************************************************************************//
DLL_API ERRORCODE UAI_SpectrometerSetPCUSBUnlock(void* api_handle);

//***************************************************************************************//
//Function Name: 	DLI_SpectrometerGetTriggerDelay
//Input Arguments:	
//		Type: void*, Name: device handle pointer
//		Type: unsigned int, Name: value of Trigger Delay
//Return Value: 		API Error Code
//***************************************************************************************//
DLL_API ERRORCODE UAI_SpectrometerGetTriggerDelay(void* api_handle,  unsigned int* it_us);

//***************************************************************************************//
//Function Name: 	DLI_SpectrometerSetTriggerDelay
//Input Arguments:	
//		Type: void, Name: device handle pointer
//		Type: unsigned int, Name: value of Trigger Delay
//Return Value: 		API Error Code
//***************************************************************************************//
DLL_API ERRORCODE UAI_SpectrometerSetTriggerDelay(void* api_handle,  unsigned int it_us);

DLL_API ERRORCODE UAI_BackgroundRemove_RAW(void* api_handle, unsigned int integration_time, float* source,unsigned int frame_begin,unsigned int framesize,bool IsRunningAVG);
DLL_API ERRORCODE UAI_StraylightRemove_RAW(void* api_handle, unsigned int integration_time, float* source,unsigned int frame_begin,unsigned int framesize);

//***************************************************************************************//
//Function Name: 	UAI_SpectrometerSetBatchMode
//Input Arguments:	
//		Type: void, Name: device handle pointer
//		Type: unsigned int, Name: value of frame numbers
//		Type: unsigned int, Name: mode , 0: software acquire, 1: falling edge trigger, 2: rising edge trigger
//Return Value: 		API Error Code
//***************************************************************************************//
DLL_API ERRORCODE UAI_SpectrometerSetBatchMode(void* api_handle, unsigned int count, unsigned int mode);


//20161220 kevin
//***************************************************************************************//
//Function Name: 	UAI_SpectrometerSetStraylightCalibrationInformationF
//Input Arguments:	
//		Type: void, Name: device handle pointer
//		Type: char*, Name: full file name of straylight table
//Return Value: 		API Error Code
//***************************************************************************************//
DLL_API ERRORCODE UAI_SpectrometerSetStraylightCalibrationInformationF(void* api_handle,  char* fullfilename);

//20161220 kevin
//***************************************************************************************//
//Function Name: 	UAI_SpectrometerSetStraylightCalibrationInformationF
//Input Arguments:	
//		Type: void, Name: device handle pointer
//		Type: char*, Name: full file name of straylight table
//		Type: unsigned int, Name: interval of sampling
//Return Value: 		API Error Code
//***************************************************************************************//
DLL_API ERRORCODE UAI_SpectrometerSetStraylightCalibrationInformationSamplingF(void* api_handl, char* fullfilename, unsigned int sampling);


//20161220 kevin
//***************************************************************************************//
//Function Name: 	UAI_SpectrometerStraylightCalibration
//Input Arguments:	
//		Type: void, Name: device handle pointer
//		Type: unsigned int, Name: spectrum frame size
//		Type: unsigned float*, Name: spectrum intensity data buffer pointer
//Return Value: 		API Error Code
//***************************************************************************************//
DLL_API ERRORCODE UAI_SpectrometerStraylightCalibration(void* api_handle,  unsigned int frame_size , float* source);

//20180201 Kevin add function to sampling pixel in order to speed up calculating sc correction
//sampling 0 : calculate all pixel
//sampling X : calculate 1/X pixel
//***************************************************************************************//
//Function Name: 	UAI_SpectrometerStraylightCalibrationSampling
//Input Arguments:	
//		Type: Handle, Name: device handle pointer
//		Type: unsigned int, Name: framesize
//		Type: void*, Name: Lambda buffer pointer
//		Type: void*, Name: Intensity buffer pointer
//Return Value: 		API Error Code
//***************************************************************************************//
DLL_API ERRORCODE UAI_SpectrometerStraylightCalibrationSampling(void* api_handle, unsigned int frame_size, float* lambda, float* intensity);


//20170316 kevin
//***************************************************************************************//
//Function Name: 	UAI_SpectrometerSetXenonPulseDelay
//Input Arguments:	
//		Type: void*, Name: device handle pointer
//		Type: unsigned int, Name: value of pulse position delay (us)
//Return Value: 		API Error Code
//***************************************************************************************//
DLL_API ERRORCODE UAI_SpectrometerSetXenonPulseDelay(void* api_handle, unsigned int time_us);

//***************************************************************************************//
//Function Name: 	UAI_SpectrometerGetXenonPulseDelay
//Input Arguments:	
//		Type: void*, Name: device handle pointer
//		Type: unsigned int*, Name: value of pulse position delay returned
//Return Value: 		API Error Code
//***************************************************************************************//
DLL_API ERRORCODE UAI_SpectrometerGetXenonPulseDelay(void* api_handle, unsigned int *time_us);

//***************************************************************************************//
//Function Name: 	UAI_SpectrometerSetXenonPulseWidth
//Input Arguments:	
//		Type: void*, Name: device handle pointer
//		Type: unsigned int, Name: value of pulse width (us)
//Return Value: 		API Error Code
//***************************************************************************************//
DLL_API ERRORCODE UAI_SpectrometerSetXenonPulseWidth(void* api_handle, unsigned int time_us);

//***************************************************************************************//
//Function Name: 	UAI_SpectrometerGetXenonPulseWidth
//Input Arguments:	
//		Type: void*, Name: device handle pointer
//		Type: unsigned int*, Name: value of pulse width returned(us)
//Return Value: 		API Error Code
//***************************************************************************************//
DLL_API ERRORCODE UAI_SpectrometerGetXenonPulseWidth(void* api_handle, unsigned int *time_us);

//***************************************************************************************//
//Function Name: 	UAI_SpectrometerGetTemperature
//Input Arguments:	
//		Type: void*, Name: device handle pointer
//		Type: unsigned int, Name: channel number of Temperature Sensor
//		Type: float*, Name: pointer of returned Temperature degC
//Return Value: 		API Error Code
//***************************************************************************************//
DLL_API ERRORCODE UAI_SpectrometerGetTemperature(void* api_handle, unsigned int channel, float *degC);

//***************************************************************************************//
//Function Name: 	UAI_ColorGetTM3015
//Input Arguments:	
//		Type: void*, Name:  color information pointer
//		Type: double*, Name: pointer of returned Rf
//		Type: double*, Name: pointer of returned Rg
//      Type: double*, Name: pointer of returned a,b array of tested light  , size = 32
//      Type: double*, Name: pointer of returned a,b array of reference light , size = 32
//Return Value: 		API Error Code
//***************************************************************************************//
DLL_API ERRORCODE UAI_ColorGetTM3015(void* color, double *Rf,double *Rg , double *ab ,double *ab_ref);

//***************************************************************************************//
//Function Name: 	UAI_SpectrometerSetXenonPulseNumber
//Input Arguments:	
//		Type: void*, Name: device handle pointer
//		Type: unsigned int, Name: value of Pulse Number
//Return Value: 		API Error Code
//***************************************************************************************//
DLL_API ERRORCODE UAI_SpectrometerSetXenonPulseNumber(void* api_handle, unsigned int value);

//***************************************************************************************//
//Function Name: 	UAI_SpectrometerGetXenonPulseNumber
//Input Arguments:	
//		Type: void*, Name: device handle pointer
//		Type: unsigned int, Name: pointer of Pulse Number
//Return Value: 		API Error Code
//***************************************************************************************//
DLL_API ERRORCODE UAI_SpectrometerGetXenonPulseNumber(void* api_handle, unsigned int *value);

//***************************************************************************************//
//Function Name: 	UAI_SpectrometerSetXenonPulseInterval
//Input Arguments:	
//		Type: void*, Name: device handle pointer
//		Type: unsigned int, Name: value of XenonPulse Interval(us)
//Return Value: 		API Error Code
//***************************************************************************************//
DLL_API ERRORCODE UAI_SpectrometerSetXenonPulseInterval(void* api_handle, unsigned int time_us);

//***************************************************************************************//
//Function Name: 	UAI_SpectrometerGetXenonPulseInterval
//Input Arguments:	
//		Type: void*, Name: device handle pointer
//		Type: unsigned int, Name: pointer of XenonPulse Interval(us)
//Return Value: 		API Error Code
//***************************************************************************************//
DLL_API ERRORCODE UAI_SpectrometerGetXenonPulseInterval(void* api_handle, unsigned int *time_us);

//***************************************************************************************//
//Function Name: 	UAI_SpectrometerSetXenonMode
//Input Arguments:	
//		Type: void*, Name: device handle pointer
//		Type: unsigned int, Name: value of Xenon Mode
//Return Value: 		API Error Code
//***************************************************************************************//
DLL_API ERRORCODE UAI_SpectrometerSetXenonMode(void* api_handle, unsigned int onoff);

//***************************************************************************************//
//Function Name: 	UAI_SpectrometerGetXenonMode
//Input Arguments:	
//		Type: void*, Name: device handle pointer
//		Type: unsigned int, Name: pointer of Xenon Mode
//Return Value: 		API Error Code
//***************************************************************************************//
DLL_API ERRORCODE UAI_SpectrometerGetXenonMode(void* api_handle, unsigned int *onoff);

//***************************************************************************************//
//Function Name: 	UAI_SpectrometerSetWDTimerEnable
//Input Arguments:	
//		Type: void*, Name: device handle pointer
//		Type: unsigned int, Name: value of WDTimer
//Return Value: 		API Error Code
//***************************************************************************************//
DLL_API ERRORCODE UAI_SpectrometerSetWDTimerEnable(void* api_handle, unsigned int time_ms);

//***************************************************************************************//
//Function Name: 	UAI_SpectrometerSetWDTimerLoad
//Input Arguments:	
//		Type: void*, Name: device handle pointer
//Return Value: 		API Error Code
//***************************************************************************************//
DLL_API ERRORCODE UAI_SpectrometerSetWDTimerLoad(void* api_handle);

//***************************************************************************************//
//Function Name: 	UAI_SpectrometerSetTECOnOff
//Input Arguments:	
//		Type: void*, Name: device handle pointer
//		Type: unsigned int, Name: value of TECOnOff
//Return Value: 		API Error Code
//***************************************************************************************//
DLL_API ERRORCODE UAI_SpectrometerSetTECOnOff(void* api_handle, unsigned int onoff);

//***************************************************************************************//
//Function Name: 	UAI_SpectrometerGetTECOnOff
//Input Arguments:	
//		Type: void*, Name: device handle pointer
//		Type: unsigned int, Name: pointer of TECOnOff
//Return Value: 		API Error Code
//***************************************************************************************//
DLL_API ERRORCODE UAI_SpectrometerGetTECOnOff(void* api_handle, unsigned int *onoff);

//***************************************************************************************//
//Function Name: 	UAI_SpectrometerSetTECFansOnOff
//Input Arguments:	
//		Type: void*, Name: device handle pointer
//		Type: unsigned int, Name: value of TECFansOnOff
//Return Value: 		API Error Code
//***************************************************************************************//
DLL_API ERRORCODE UAI_SpectrometerSetTECFansOnOff(void* api_handle, unsigned int onoff);

//***************************************************************************************//
//Function Name: 	UAI_SpectrometerGetTECFansOnOff
//Input Arguments:	
//		Type: void*, Name: device handle pointer
//		Type: unsigned int, Name: pointer of TECFansOnOff
//Return Value: 		API Error Code
//***************************************************************************************//
DLL_API ERRORCODE UAI_SpectrometerGetTECFansOnOff(void* api_handle, unsigned int *onoff);

//***************************************************************************************//
//Function Name: 	UAI_SpectrometerSetTECDAC
//Input Arguments:	
//		Type: void*, Name: device handle pointer
//		Type: unsigned int, Name: value of TECFansOnOff
//Return Value: 		API Error Code
//***************************************************************************************//
DLL_API ERRORCODE UAI_SpectrometerSetTECDAC(void* api_handle, unsigned int value);

//***************************************************************************************//
//Function Name: 	UAI_SpectrometerGetTECDAC
//Input Arguments:	
//		Type: void*, Name: device handle pointer
//		Type: unsigned int, Name: pointer of TECDAC
//Return Value: 		API Error Code
//***************************************************************************************//
DLL_API ERRORCODE UAI_SpectrometerGetTECDAC(void* api_handle, unsigned int *value);

//***************************************************************************************//
//Function Name: 	UAI_SpectrometerGetTECTemperature
//Input Arguments:	
//		Type: void*, Name: device handle pointer
//		Type: unsigned int, Name: pointer of TEC Temperature
//Return Value: 		API Error Code
//***************************************************************************************//
DLL_API ERRORCODE UAI_SpectrometerGetTECTemperature(void* api_handle, float *degC);


//***************************************************************************************//
//Function Name: 	UAI_MATHGetCurveInfo
//Input Arguments:	
//		Type: float*, pointer of wavelength array
//		Type: float*, pointer of Intensity array
//		Type: Unsigned int , size of Wavelength and Intensity array
//		Type: float, Start wavelength for analyzing
//		Type: float, End wavelength for analyzing
//		Type: float* , pointer of centroid of spectrum

//Return Value: 		API Error Code
//***************************************************************************************//
DLL_API ERRORCODE UAI_MATHGetCentroid(float* Lambda , float* Intensity, unsigned int size ,float Wavelength_start , float Wavelength_end,  float* centroid);


//***************************************************************************************//
//Function Name: 	UAI_SpectrometerBlockModeStartStop
//Input Arguments:	
//		Type: void*, Name: device handle pointer
//		Type: unsigned int, Name: Block Mode Access Start/Stop (0: stop; 1: start)
//Return Value: 		API Error Code
//***************************************************************************************//
DLL_API ERRORCODE UAI_SpectrometerBlockModeStartStop(void* api_handle, unsigned int value);

//***************************************************************************************//
//Function Name: 	UAI_SpectrometerBlockDataAcquire
//Input Arguments:	
//		Type: void*, Name: device handle pointer
//		Type: unsigned int, Name: integration time
//		Type: unsigned int, Name: spectrum frame size
//		Type: unsigned int, Name: spectrum frame number(Block Length value)
//		Type: unsigned float*, Name: spectrum intensity data buffer pointer
//Return Value: 		API Error Code
//***************************************************************************************//
DLL_API ERRORCODE UAI_SpectrometerBlockDataAcquire(void* api_handle, unsigned int integration_time_us, unsigned int frame_size ,unsigned int framenumber, float *buffer);

//***************************************************************************************//
//Function Name: 	UAI_SpectrometerBlockLengthSet
//Input Arguments:	
//		Type: void*, Name: device handle pointer
//		Type: unsigned int, Name: Block Length value 
//Return Value: 		API Error Code
//***************************************************************************************//
DLL_API ERRORCODE UAI_SpectrometerBlockLengthSet(void* api_handle, unsigned int value);

//***************************************************************************************//
//Function Name: 	UAI_SpectrometerSetResetTimerEnable
//Input Arguments:	
//		Type: void*, Name: device handle pointer
//		Type: unsigned int, Name: value of Timer count down time in (ms); 0 is disable;
//Return Value: 		API Error Code
//***************************************************************************************//
DLL_API ERRORCODE UAI_SpectrometerSetResetTimerEnable(void* api_handle, unsigned int time_ms);

//***************************************************************************************//
//Function Name: 	UAI_SpectrometerSetResetTimerLoad
//Input Arguments:	
//		Type: void*, Name: device handle pointer
//Return Value: 		API Error Code
//***************************************************************************************//
DLL_API ERRORCODE UAI_SpectrometerSetResetTimerLoad(void* api_handle);

//***************************************************************************************//
//Function Name: 	UAI_SetExtIntTimeMode
//Input Arguments:	
//		Type: void*, Name: device handle pointer
//		Type: unsigned int, Name: ext. trigger Mode Access Start/Stop (0: stop; 1: start)
//Return Value: 		API Error Code
//***************************************************************************************//
DLL_API ERRORCODE UAI_SetExtIntTimeMode(void* api_handle, unsigned int value);

//20170713
//***************************************************************************************//
//Function Name: 	UAI_ColorGetTLCI
//Input Arguments:	
//		Type: void*, Name: color information handler
//		Type: double, Name: pointer of double value of TLCI Qa
//Return Value: 		API Error Code
//***************************************************************************************//
DLL_API ERRORCODE UAI_ColorGetTLCI(void* color, double* Qa);

//20170728
//***************************************************************************************//
//Function Name: 	UAI_SpectrometerSetExtIODirection
//Input Arguments:	
//		Type: void*, Name: device handle pointer
//		Type: unsigned int, Name: iodir
//Return Value: 		API Error Code
//***************************************************************************************//
DLL_API ERRORCODE UAI_SpectrometerSetExtIODirection(void* api_handle, unsigned int iodir);


//***************************************************************************************//
//Function Name: 	UAI_SpectrometerGetExtIODirection
//Input Arguments:	
//		Type: void*, Name: device handle pointer
//		Type: unsigned int* , Name: iodir
//Return Value: 		API Error Code
//***************************************************************************************//
DLL_API ERRORCODE UAI_SpectrometerGetExtIODirection(void* api_handle, unsigned int *iodir);


//***************************************************************************************//
//Function Name: 	UAI_SpectrometerSetExtIO
//Input Arguments:	
//		Type: void*, Name: device handle pointer
//		Type: unsigned int , Name: iovalue
//Return Value: 		API Error Code
//***************************************************************************************//
DLL_API ERRORCODE UAI_SpectrometerSetExtIO(void* api_handle, unsigned int iovalue);

//***************************************************************************************//
//Function Name: 	UAI_SpectrometerGetExtIO
//Input Arguments:	
//		Type: void*, Name: device handle pointer
//		Type: unsigned int* , Name: iovalue
//Return Value: 		API Error Code
//***************************************************************************************//
DLL_API ERRORCODE UAI_SpectrometerGetExtIO(void* api_handle, unsigned int *iovalue);

//***************************************************************************************//
//Function Name: 	UAI_SpectrometerSetExtIOEnable
//Input Arguments:	
//		Type: void*, Name: device handle pointer
//		Type: unsigned int, Name: enable (0: disable; 1: enable)
//Return Value: 		API Error Code
//***************************************************************************************//
DLL_API ERRORCODE UAI_SpectrometerSetExtIOEnable(void* api_handle, unsigned int enable);

//***************************************************************************************//
//Function Name: 	UAI_SpectrometerSetLEDSystemCalibration
//Input Arguments:	
//		Type: void*, Name: color handle pointer
//		Type: unsigned int, Name: fullfilename  , fill path of calibration file
//Return Value: 		API Error Code
//***************************************************************************************//
DLL_API ERRORCODE UAI_SpectrometerSetLEDSystemCalibration(void *buffer, char* fullfilename);

//***************************************************************************************//
//Function Name: 	UAI_DWTdenoise
//Input Arguments:	
//		Type: void*, Name: device handle pointer
//		Type: float*, Name: data buffer
//		Type: unsigned int, Name: frame size
//Return Value: 		API Error Code
//***************************************************************************************//
DLL_API ERRORCODE UAI_DWTdenoise(void* api_handle, float * buffer, unsigned int frame_size);


//***************************************************************************************//
//Function Name: 	UAI_SpectrometerSetAdcGain
//Input Arguments:	
//		Type: void*, Name: device handle pointer
//		Type: unsigned int*, Name: ADC Gain
//Return Value: 		API Error Code
//***************************************************************************************//
DLL_API ERRORCODE UAI_SpectrometerGetAdcGain(void* api_handle, unsigned int *gain);


//***************************************************************************************//
//Function Name: 	UAI_SpectrometerSetAdcGain
//Input Arguments:	
//		Type: void*, Name: device handle pointer
//		Type: unsigned int, Name: ADC Gain
//Return Value: 		API Error Code
//***************************************************************************************//
DLL_API ERRORCODE UAI_SpectrometerSetAdcGain(void* api_handle, unsigned int gain);


//***************************************************************************************//
//Function Name: 	UAI_SpectrometerGetAdcOffsetVolt
//Input Arguments:	
//		Type: void*, Name: device handle pointer
//		Type: unsigned int*, Name: ADC Offset
//Return Value: 		API Error Code
//***************************************************************************************//
DLL_API ERRORCODE UAI_SpectrometerGetAdcOffsetVolt(void* api_handle, int *offset);



//***************************************************************************************//
//Function Name: 	UAI_SpectrometerSetAdcOffsetVolt
//Input Arguments:	
//		Type: void*, Name: device handle pointer
//		Type: unsigned int, Name: ADC Offset
//Return Value: 		API Error Code
//***************************************************************************************//
DLL_API ERRORCODE UAI_SpectrometerSetAdcOffsetVolt(void* api_handle, int offset);



//***************************************************************************************//
//Function Name: 	UAI_SpectrometerSetAdcOffsetVolt
//Input Arguments:	
//		Type: void*, Name: device handle pointer
//		Type: unsigned int*, Name:Adc Maximum Volt
//Return Value: 		API Error Code
//***************************************************************************************//
DLL_API ERRORCODE UAI_SpectrometerGetAdcMaximumVolt(void* api_handle, unsigned int *volt);


//20180430 Kevin Add function to calculate PPFD with specific wavelength range
//***************************************************************************************//
//Function Name: 	UAI_ColorSetPPFDByRange
//Input Arguments:	
//		Type: void*, Name: color information handler
//		Type: double, Name:Start wavelength
//		Type: double, Name:End wavelength
//Return Value: 		API Error Code
//***************************************************************************************//
DLL_API ERRORCODE UAI_ColorSetPPFDByRange(void* color, double StartWavelength, double EndWavelength);

//***************************************************************************************//
//Function Name: 	UAI_SpectrometerSetGPIODbgmode
//Input Arguments:	
//		Type: void*, Name: device handle pointer
//		Type: unsigned int , Name: iovalue
//Return Value: 		API Error Code
//***************************************************************************************//
DLL_API ERRORCODE UAI_SpectrometerSetGPIODbgmode(void* api_handle, unsigned int onoff);

//***************************************************************************************//
//Function Name: 	UAI_SpectrometerGetGPIODbgmode
//Input Arguments:	
//		Type: void*, Name: device handle pointer
//		Type: unsigned int* , Name: iovalue
//Return Value: 		API Error Code
//***************************************************************************************//
DLL_API ERRORCODE UAI_SpectrometerGetGPIODbgmode(void* api_handle, unsigned int *onoff);

//***************************************************************************************//
//Function Name: 	UAI_SpectrometerGetShutterSwitch
//Input Arguments:	
//		Type: void*, Name: device handle pointer
//		Type: unsigned int* , Name: iovalue
//Return Value: 		API Error Code
//***************************************************************************************//
DLL_API ERRORCODE UAI_SpectrometerGetShutterSwitch(void* api_handle, unsigned int *onoff);


//***************************************************************************************//
//Function Name: 	UAI_SpectrometerSetShutterSwitch
//Input Arguments:	
//		Type: void*, Name: device handle pointer
//		Type: unsigned int* , Name: iovalue
//Return Value: 		API Error Code
//***************************************************************************************//
DLL_API ERRORCODE UAI_SpectrometerSetShutterSwitch(void* api_handle, unsigned int onoff);

//***************************************************************************************//
//Function Name: 	UAI_SpectrometerGetTECTargetTemperature
//Input Arguments:	
//		Type: void*, Name: device handle pointer
//		Type: float* , Name: degC
//Return Value: 		API Error Code
//***************************************************************************************//
DLL_API ERRORCODE UAI_SpectrometerGetTECTargetTemperature(HANDLE api_handle, float *degC);

//***************************************************************************************//
//Function Name: 	UAI_SpectrometerSetTECTargetTemperature
//Input Arguments:	
//		Type: void*, Name: device handle pointer
//		Type: float , Name: degC
//Return Value: 		API Error Code
//***************************************************************************************//
DLL_API ERRORCODE UAI_SpectrometerSetTECTargetTemperature(HANDLE api_handle, float degC);

//***************************************************************************************//
//Function Name: 	UAI_SpectrometerGetAmbientLightSensorMode
//Input Arguments:	
//		Type: void*, Name: device handle pointer
//		Type: UINT32, Name: mode of ALSnr operating - 0: ENABLE; 1: GAIN; 2:INTTIME;
//		Type: UINT32*, Name: value of each mode readback
//Return Value: 		API Error Code
//***************************************************************************************//
DLL_API ERRORCODE UAI_SpectrometerGetAmbientLightSensorMode(HANDLE api_handle, UINT32 mode, UINT32 *value);

//***************************************************************************************//
//Function Name: 	UAI_SpectrometerSetAmbientLightSensorMode
//Input Arguments:	
//		Type: void*, Name: device handle pointer
//		Type: UINT32, Name: mode of ALSnr operating - 0: ENABLE; 1: GAIN; 2:INTTIME;
//		Type: UINT32, Name: value of each mode would like to set.
//Return Value: 		API Error Code
//***************************************************************************************//
DLL_API ERRORCODE UAI_SpectrometerSetAmbientLightSensorMode(HANDLE api_handle, UINT32 mode, UINT32 value);

//***************************************************************************************//
//Function Name: 	UAI_SpectrometerGetAmbientLightSensorValue
//Input Arguments:	
//		Type: void*, Name: device handle pointer
//		Type: UINT32, Name: channel of ALSnr - 0: ADCH0; 1: ADCH1; 2: BOTH;
//		Type: UINT32*, Name: value of channel readback, each channel valid value are 2 bytes (0-65535).
//Return Value: 		API Error Code
//***************************************************************************************//
DLL_API ERRORCODE UAI_SpectrometerGetAmbientLightSensorValue(HANDLE api_handle, UINT32 channel, UINT32 *value);

//***************************************************************************************//
//Function Name: 	UAI_SpectrometerGetLightSourceDACValue
//Input Arguments:	
//		Type: void*, Name: device handle pointer
//		Type: UINT32*, Name: value of last setting dac value readback (0-4095)
//Return Value: 		API Error Code
//***************************************************************************************//
DLL_API ERRORCODE UAI_SpectrometerGetLightSourceDACValue(HANDLE api_handle, UINT32 *value);

//***************************************************************************************//
//Function Name: 	UAI_SpectrometerSetLightSourceDACValue
//Input Arguments:	
//		Type: void*, Name: device handle pointer
//		Type: UINT32, Name: value would like to set to DAC. (0-4095)
//Return Value: 		API Error Code
//***************************************************************************************//
DLL_API ERRORCODE UAI_SpectrometerSetLightSourceDACValue(HANDLE api_handle, UINT32 value);

//20181030 Kevin add product serial number
//***************************************************************************************//
//Function Name: 	UAI_SpectrometerGetProductSerialNumber
//Input Arguments:	
//		Type: void*, Name: device handle pointer
//		Type: unsigned char*, Name: serial number buffer pointer
//Return Value: 		API Error Code
//***************************************************************************************//
DLL_API ERRORCODE UAI_SpectrometerGetProductSerialNumber(void* api_handle, unsigned char *serial);

//***************************************************************************************//
//Function Name: 	UAI_SpectrometerSetProductSerialNumber
//Input Arguments:	
//		Type: void*, Name: device handle pointer
//		Type: unsigned char*, Name: serial number buffer pointer
//Return Value: 		API Error Code
//***************************************************************************************//
DLL_API ERRORCODE UAI_SpectrometerSetProductSerialNumber(void* api_handle, unsigned char *serial);

//20181030 Kevin add product serial number
//***************************************************************************************//
//Function Name: 	UAI_SpectrometerGetProductModelName
//Input Arguments:	
//		Type: void*, Name: device handle pointer
//		Type: unsigned char*, Name: model name buffer pointer
//Return Value: 		API Error Code
//***************************************************************************************//
DLL_API ERRORCODE UAI_SpectrometerGetProductModelName(void* api_handle, unsigned char *modelname);

//***************************************************************************************//
//Function Name: 	UAI_SpectrometerSetProductModelName
//Input Arguments:	
//		Type: void*, Name: device handle pointer
//		Type: unsigned char*, Name: model name buffer pointer
//Return Value: 		API Error Code
//***************************************************************************************//
DLL_API ERRORCODE UAI_SpectrometerSetProductModelName(void* api_handle, unsigned char *modelname);

//***************************************************************************************//
//Function Name: 	UAI_SpectrometerTriggerDataOneshotWithTime
//Input Arguments:	
//		Type: void*, Name: device handle pointer
//		Type: unsigned int, Name: integration time
//		Type: unsigned int, Name: buffer index to store (0~63)
//Return Value: 		API Error Code
//***************************************************************************************//
DLL_API ERRORCODE UAI_SpectrometerTriggerDataOneshotWithTime(void* api_handle, unsigned int integration_time_us, unsigned int index);

//Function Name: 	UAI_SpectrometerQueryDataOneshotWithTime
//Input Arguments:	
//		Type: void*, Name: device handle pointer
//		Type: unsigned int, Name: integration time
//		Type: unsigned int, Name: buffer index to load (0~63)
//		Type: unsigned int, Name: spectrum frame size
//		Type: unsigned float*, Name: spectrum intensity data buffer pointer
//Return Value: 		API Error Code
//***************************************************************************************//
DLL_API ERRORCODE UAI_SpectrometerQueryDataOneshotWithTime(void* api_handle, unsigned int integration_time_us, unsigned int index, unsigned int framesize, float * buffer);

//***************************************************************************************//
//Function Name: 	UAI_SpectrometerUpdateHandle
//Input Arguments:	
//		Type: unsigned int, Name: device number
//		Type: void**, Name: reference of device handle pointer
//		Type: unsigned int, Name: vendor id code
//		Type: unsigned int, Name: product id code
//Return Value: 		API Error Code
//***************************************************************************************//
DLL_API ERRORCODE UAI_SpectrometerUpdateHandle(unsigned int dev, void** handle, unsigned int vid, unsigned int pid);

//***************************************************************************************//
//Function Name: 	UAI_SpectrometerGetTemperaturePrecision
//Input Arguments:	
//		Type: void*, Name: device handle pointer
//		Type: float*, Name: pointer of returned Temperature degC
//Return Value: 		API Error Code
//***************************************************************************************//
DLL_API ERRORCODE UAI_SpectrometerGetTemperaturePrecision(HANDLE api_handle, float *degC);

#ifdef __cplusplus
}
#endif
#endif
