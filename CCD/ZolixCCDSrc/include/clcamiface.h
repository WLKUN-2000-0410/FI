/**
 *  \file clcamiface.h
 *
 *  \author  Critical Link LLC
 *
 *  \date    4/16/2009
 *
 *  \remarks
 *
 *  \verbatim

      o  0
      | /       Copyright (c) 2009
     (CL)---o   Critical Link, LLC
       \
        O

    \endverbatim
 **/

#ifndef CLCAMIFACE_H
#define CLCAMIFACE_H

#if defined(__GNUC__)
#include <stddef.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h> 
#endif

#include "clcamdefines.h"

#if (defined(_MSC_VER) || defined(WIN32))
#include <winsock2.h>

/// Make this header file usable by both the DLL and the user of the DLL
#ifndef DECLSPEC
#ifdef BUILDING_THE_DLL
   #define DECLSPEC    __declspec(dllexport)
#else
   #define DECLSPEC    __declspec(dllimport)
#endif
#define DLL_NAME    TEXT("clcamiface3.dll")
#endif

#else
   #define DECLSPEC
   #define __cdecl   
#endif


/**
 *   This enumeration is used when configuring the GPIO pins 
 *   on the CCDsp camera.  The GPIOs can be configured as 
 *   inputs (including using them for external triggers) and
 *   may also be configured as outputs.  For outputs, you
 *   may control the IO lines via software (LOW or HIGH) or
 *   tie them to a logic timing signal, including the 
 *   shutter strobe.
 */  
typedef enum
{  
    eeINPUT      = 0,   //!< set the GPIO as in INPUT
    eeOUTLOW     = 1,   //!< drive GPIO low
    eeOUTHIGH    = 2,   //!< drive GPIO high
    eeSHUTTER    = 3,   //!< drive GPIO with shutter signal
    eeOUTTIMGEN0 = 4,   //!< drive GPIO with custom timing strobe 0
    eeOUTTIMGEN1 = 5,   //!< drive GPIO with custom timing strobe 1
    eeOUTTIMGEN2 = 6    //!< drive GPIO with custom timing strobe 2
} tePinState;

/**
 *   This enumeration is used to describe how the external
 *   trigger event (rising edge on the trigger pin) is used
 *   for a capture request (CLReadCCDArea or CLReadCCDBinned).
 *   This mode is only significant for capture calls using a 
 *   NumImages parameter greater than 1.
 */
typedef enum
{
	eeOnePerImage = 0,  //!< requires a rising edge event for each image in a capture request
	eeOnePerGroup = 1   //!< requires a trigger event for the first image in a capture request
} teExtTrigMode;

/**
 *  If this field is "or'd" into the NumClears field of a ReadCCDArea or ReadCCDBinned
 *  call, the CCD will be cleared at the beginning of each frame for an acquisition 
 *  set (e.g., the case where NumImages is non-zero)
 */
#define CLEAR_EACH_FRAME_MASK  128

/**
 * @mainpage 
 *
 * @section intro_sec Introduction
 *
 * The CLCamera interface library provides a C dynamic link library interface 
 * to any number of CCDsp cameras powered by the MightyDSP framework.
 * The CLCamera interface supports communication to CCDsp cameras 
 * using a USB 2.0 port (up 480 MBps) and also over Ethernet TCP/IP protocol (10/100 MBit).
 *
 * All CCDsp Cameras handle commands in a pipeline fashion.  The CLCamera 
 * interface allows CLClearCCD, CLReadCCDArea, and CLReadCCDBinned calls to be 
 * made in a non-blocking fasion and "queue" in the camera.  This allows for 
 * tight timeline execution between Clearing, Exposing, and Reading the CCD in order
 * to minimize Dark Current buildup between a clear and an exposure.  Additionally, 
 * Read calls may be configured to use callback routines in order to allow tasks
 * to continue which camera image data is transferred to the PC Host.
 *
 * @section example_sec Example
 * 
 * The following code snippet provides a simple example of interfacing to a 
 * CCDsp camera over a USB serial port interface.  The camera is opened.  
 * An exposure time is configured.  Then, the camera is cleared, exposed, 
 * read (full image) capturing 5 images, and finally closed.
 *
 * @code
 * {
 *     int CameraHandle;
 *     unsigned short* lpData = new unsigned short[1024*256];
 *
 *
 *     // Open the camera
 *     CameraHandle = CLOpenHSUSBCamera(0);
 *
 *     // Set the camera for a 5 ms exposure time
 *     CLSetExposure(CameraHandle, 5);
 *     
 *     // Loop On Image count
 *     for (int i = 0; i < 5; i++)
 *     {
 *        // Clear the camera once, then immediately read
 *        // (the exposure time is configured above)
 *        CLClearCCD(CameraHandle, 1, 0);
 *        CLReadCCDArea(CameraHandle, lpData);
 *
 *        // Wait for the Read to Complete
 *        CLWaitCompletion(CameraHandle, -1);
 *        
 *        // Process the image data (application code)
 *        ProcessImage(lpData)
 *     }
 *
 *     CLCloseCamera(CameraHandle);
 *     ...
 * } @endcode
 *
 */

#pragma pack(push,1)
/**
 * Camera Hardware Revision Info
 * This structure contains the CCDsp Camera
 * hardware revision information.
 */
typedef struct TCLCameraHardware
{
	char            ModelNumber[16];   //!< The Camera Model Number
	unsigned short  SerialNumber;      //!< The Camera Serial Number
	char            Manufacturer[24];  //!< The Manufacturer
	unsigned short  ImageRows;         //!< The number of rows on the CCD (copied from TimingFileData)
	unsigned short  Reserved;          //!< This field is reserved for future use
	unsigned short  ActiveCols;        //!< The number of Columns on the CCD (copied from TimingFileData)
	unsigned short	FPGAVersion;       //!< The FPGA version 0xXXYY  (XX Major/YY Minor)  
	unsigned short	DSPVersion;        //!< The DSP Version  0xXXYY  (XX Major/YY Minor)
    unsigned short  Reserved0;         //!< Not Used
    unsigned short  ThermistorBeta;    //!< beta term for CCD thermistor
    unsigned short  TECMaxCurrent;     //!< TE Cooler Maximum Current in mA
    unsigned short  Reserved1;         //!< reserved
	unsigned short  BottomMaskedRows;  //!< number of rows at bottom of array that are masked (copied from TimingFileData)
	unsigned short  TopMaskedRows;     //!< number of rows at top of array that are masked (copied from TimingFileData)
	int             PixelWidthNm;      //!< pixel width in NanoMeters
	int             PixelHeightNm;     //!< pixel height in NanoMeters
    int             reserved[7];       //!< 10 for 4 byte alignment
} CAMERA_HARDWARE;
#pragma pack(pop)

#pragma pack(push,1)
/**
 * Ethernet Configuration Information
 */
typedef struct TCLNetworkConfig
{
    unsigned short  ServerPort;      //!< TCP Server Port
    unsigned char   MACAddr[6];      //!< Camera MAC Address
    unsigned char   NETAddr[4];      //!< Camera Network Address (network order)
    unsigned char   GWAddr[4];       //!< Camera Gateway Address (network order)
    unsigned char   NETMask[4];      //!< Camera Network Mask    (network order)
    unsigned char   UseDHCP;         //!< if non-zero, will force camera to use DHCP for network config
    unsigned char   Spare;           //!< reserved
} CAMERA_NET_CONFIG;
#pragma pack(pop)

#pragma pack(push,1)
/**
 * Camera Heart Beat Data
 * The CCDsp camera periodically reports
 * heartbeat data to the API.  This structure defines
 * the contents of the heartbeat message.
 */
typedef struct tsHeartBeatData
{
   unsigned char errorByte;    //!< Camera Error Status, 0 is no error.
                               //!< - Bit 0 - when 1 camera is not configured (factory settings are bad)
   unsigned char cameraFlags;  //!<  Bit Mask
	                           //!< - Bit 0 - Cooling Enabled (see CLSetCCDCooling)
	                           //!< - Bit 1 - Zero Mean Biasing Enabled (see CLSetCCDOffset)
                               //!< - Bit 2 - Camera Is Acquiring
   short boardTemp;            //!< Camera Pre-Amp Board temperature in tenths of degrees C
   short CCDTemp;              //!< CCD Interface temperature in tenths of degrees C
   unsigned short offsetSetting;    //!< CCD bias setting, 1 LSB is TBD volts
   unsigned short substrateSetting; //!< CCD substrate setting, 1 LSB is TBD volts
   unsigned short computedMean;     //!< Mean Value subtracted in DSP if Zero Mean Biasing is Enabled
   float    coolingUsed;            //!< range 0-1, amount of available TE cooling current applied       
   unsigned int   ExpTimeMSec;      //!< Exposure Time Count Down, in seconds
} HEARTBEAT_DATA;
#pragma pack(pop)

/**
 *  @name CLCamera Return Codes
 *  This area defines return codes used by the interface API.
 */
//@{
#define CL_CAMERA_READY    0  //!< Indicates camera is ready and idle
#define CL_CAMERA_OK       0  //!< Indicates camera successfully processed a command
#define CL_OPS_PENDING    -1  //!< Indicates camera is pending on operations (not idle, busy)
#define CL_TIMEOUT        -2  //!< Indicates camera operations were timed out (a problem has occurred)
#define CL_INVALID_HANDLE -3  //!< Indicates invalid camera handle was passed to the API
#define CL_BUFF_TOO_SMALL -4  //!< Indicates buffer passed to function is too small for data requested
#define CL_BAD_PARAM      -5  //!< Indicates bad parameter passed to function
//@}

#define CL_DEFAULT_TIMEOUT 20000  //!< default camera timeout period

// Function prototypes

#ifdef __cplusplus
extern "C" {    // only need to export C interface if
                // used by C++ source code
#endif



//=====================================================================
//
//     CAMERA SETUP COMMANDS
//
//=====================================================================

/**
 * Get the current version level of the CLCamera Interface Dynamic Link Library.
 *
 * \return integer representation of the DLL version for the specified opened device.
 *                 the version number is 0xXXXXYYYY where XXXX is the major and YYYY
 *                 is the minor version numbers.
 * 获取dll版本
 */
DECLSPEC int __cdecl CLGetDllVersion(void);

/**
 *  Get the number of bytes required to hold a CCDsp Camera Timing file.
 *	得到所需要的字节数举行CCDsp相机定时文件。
 *  \return number of bytes needed to hold a CCDsp Camera Timing File Data structure.
 *  返回的字节数需要CCDsp相机定时文件数据结构
 */
DECLSPEC int __cdecl CLGetTimingDataSize(void);

/**
 * This functions checks to see if camera is ready, returning immediately.
 *这个函数检查摄像头是否准备好了,立即返回。
 * \param[in] CameraHandle  Handle to a tcCLCamera that was opened.
 * 参数[在]CameraHandle tcCLCamera是打开的句柄
 * \return  CL_CAMERA_READY all operations have completed successfully
 * 返回CL_CAMERA_READY所有操作已经成功完成
 *          CL_OPS_PENDING some operation is still pending
 * CL_OPS_PENDING一些操作仍悬而未决
 */
DECLSPEC int __cdecl CLisCameraReady(int CameraHandle);

/**
 * Open a CLCamera using an ethernet interface.
 * 打开一个CLCamera使用以太网接口。
 * \param[in] IPAddress string containing IP address (Ex: "192.168.0.1")
 * \param[in] IPPort    Internet Port Port to connect to
 * \return a valid CLCamera Handle or -1 on failure
 */
DECLSPEC int __cdecl CLOpenENetCamera(char* IPAddress, unsigned short IPPort);

/**
 * Open a CLCamera using an RS-232 Serial Port Interface.  This DLL supports
 * 打开一个CLCamera使用rs - 232串口接口。
 * opening of multiple CLCamera devices if necessary.
 *这个DLL支持开放多个CLCamera设备如果必要的话。
 * \note the serial interface is deprecated.  Users should use the CLOpenHSUSBCamera or
 *       the ethernet interface to the camera. The Baud rate for the serial
 *       interface is 115200.
 * 注意,串行接口是弃用。用户应该使用CLOpenHSUSBCamera或相机的以太网接口。串行接口的波特率为115200
 * \param[in] ComNumber the windows COM Port number
 *参数[在]ComNumber windows COM端口号
 * \return a valid CLCamera Handle or -1 on failure
 * 返回一个有效CLCamera处理或1失败
 */
DECLSPEC int __cdecl CLOpenSerialCamera(int ComNumber);

/**
 * Open a CLCamera using an USB Serial Port Interface employing
 * a CCDsp camera model utilizing a serial bridge chip interface. 
 * 打开一个CLCamera使用一个USB串口接口采用CCDsp相机模型利用串行桥芯片接口。
 * This DLL supports opening of multiple CLCamera devices if necessary.
 *这个DLL支持开放多个CLCamera设备如果必要的话。
 * The baud rate for this method is 921600.
 *这个方法的波特率为921600。
 * \note this function is not generally used, as newer CCDsp cameras
 * use a USB 2.0 compliant interface.  See CLOpenHSUSBCamera and
 * CLLocateHSUSBCameras.
 *注意，这个功能一般不使用，因为新CCDsp相机
	使用USB2.0兼容接口。见CLOpenHSUSBCamera和CLLocateHSUSBCameras。
 * \param[in] ComNumber the windows COM Port number (1 through N)
 * \return a valid CLCamera Handle or -1 on failure
 */
DECLSPEC int __cdecl CLOpenUSBSerialCamera(int ComNumber);

/**
 * Open a CLCamera using a High Speed USB Serial Port Interface.  
 * 打开使用高速USB串行端口接口一个摄像头。
 *  This DLL supports opening of multiple CLCamera devices if necessary.
 *	这个DLL支持多个摄像设备，如果有必要的开放。
 * \param[in] DeviceIndex the USB device index (set to 0 for first camera, -1 to read from INI file)
 * \return a valid CLCamera Handle or -1 on failure
 * 参数[IN]通讯设备指数USB设备指数（设置为0的第一台相机，从-1 INI文件读取）返回一个有效的CC相机手柄或-1失败
 */
DECLSPEC int __cdecl CLOpenHSUSBCamera(int DeviceIndex);

/**
 * Try to find the camera who's serial number is given.
 *	试图找到谁的序列号被赋予相机。
 * This routine will check ethernet, HSUSB, and USB CDC serial cameras.
 *这个程序将检查网卡，HUB，和USB CDC串行摄像头。
 * \note Each camera found will be opened to check the serial number.
 *注意发现将被打开检查序列号，每个摄像机。
 *  If the serial number is not the desired one, it will be closed again.
 *如果序列号不期望的一个，它将被再次关闭。
 * \param[in] anSerialNumber The serial number of the desired camera
 * \return A valid CLCamera Handle or -1 on failure. The failure could
 * be either the camera was not found \e or the camera was found, but
 * there was an error opening it.
 */
DECLSPEC int __cdecl CLOpenCameraBySerialNumber(int anSerialNumber);

/**
 * Close/Clean up resouces associated with a CLCamera Handle
 * 用相机句柄相关联关闭/清理资源
 * \param[in] CameraHandle A valid CLCamera Handle
 *相机手柄的有效CC相机手柄
 * \return non-zero on error
 */
DECLSPEC int __cdecl CLCloseCamera(int CameraHandle);

/**
 *   Retrieve Camera Hardware information from the attached CLCamera.
 *	 检索来自连接的摄像头摄像头硬件信息。
 *   \note this routine will overwrite the BufferLen parameter with the amount
 *         of memory required to fill in the Hardware structure.
 *   注意，这个例行程序将覆盖用的存储器，以填补在硬件结构所需的量的缓冲区长度参数。
 *   \param[in]     CameraHandle A valid CLCamera Handle
 *   \param[out]    Hardware structure which will be filled with camera information 这将在填充有照相机信息的硬件结构
 *   \param[in,out] BufferLen length of the buffer passed in, amount of data filled returned
 *   \return non-zero on error
 */
DECLSPEC int __cdecl CLGetCameraHardwareInfo(int CameraHandle, TCLCameraHardware* Hardware, int* BufferLen);

/**
 *   Set the Camera Hardware information in the attached CLCamera.
 *  坐落在连接的摄像头摄像头硬件信息。
 *   \note this routine will overwrite the BufferLen parameter with the amount
 *         of memory required to fill in the Hardware structure.
 *
 *   \param[in]     CameraHandle A valid CLCamera Handle
 *   \param[out]    Hardware structure which will be filled with camera information
 *   \param[in,out] BuffLen length of the buffer passed in, amount of data filled returned
 *   \return non-zero on error
 */
DECLSPEC int __cdecl CLSetCameraHardwareInfo(int CameraHandle, TCLCameraHardware* Hardware, int *BuffLen);

/**
 * Get Camera Network Configuration for the attached CLCamera.  
 * 获取摄像机网络配置的连接的摄像头。
 * \param[in] CameraHandle A valid CLCamera Handle
 * \param[out] Config structure which will contain the information retrieved.
 * \return non-zero on error.
 */
DECLSPEC int __cdecl CLGetCameraNetworkConfig(int CameraHandle, TCLNetworkConfig* Config);

/**
 * Set Camera Network Configuration for the attached CLCamera.  
 * 设置摄像机网络配置的连接的摄像头。
 * This command will program into non-volatile memory (FLASH) the desired network configuration. 
 *此命令将编程到非易失性存储器（FLASH）所需的网络配置。
 * In order for these settings to become valid, the camera must be power cycled.
 * 为了使这些设置生效，相机必须重新加电。
 * \param[in] CameraHandle A valid CLCamera Handle
 * \param[in] Config structure which contains the information to set.
 * \return non-zero on error.
 */
DECLSPEC int __cdecl CLSetCameraNetworkConfig(int CameraHandle, TCLNetworkConfig* Config);

/**
 * Try to find the camera who's serial number is given. 
 * 试图找到谁的序列号被赋予相机。
 * This routine will check only Critical Link USB CDC cameras 
	note Each camera found will be opened to check the serial number.
 *	该程序将只检查关键环节USB CDC相机注意发现每个摄像机将被打开检查序列号。
 * If the serial number is not the desired one, it will be closed again.
 * 如果序列号不期望的一个，它将被再次关闭。
 * \param[in] anSerialNumber The serial number of the desired camera
 * \return A valid CLCamera Handle or -1 on failure. The failure could
 * be either the camera was not found \e or the camera was found, but
 * there was an error opening it.
 */
DECLSPEC int __cdecl CLFindUSBSerialCamera(int anSerialNumber);

/**
 *  This structure defines the image data passed to the application from a IMAGEDATACALLBACK
 *  function callback.
 *	本结构定义传递到从的ImageData回调函数回调应用程序的图像数据。
 */
typedef struct
{
	/** Describes the image type received.
	 *
	 */
	enum teImageType
	{
		eeArea,             //!< area data read, mpData points to unsigned shorts (16 bits)
		eeBinned,           //!< Vertical Binned data read, mpData points to signed integers (32 bits)
		eeMaskHorizontal,   //!< Mask Binning mode, horizontal alignment, mpData points to signed integers (32 bits)
		eeMaskVertical,     //!< Mask Binning mode, vertical alignment, mpData points to signed integers (32 bits)
		eeCustom,           //!< Customer Customized image format
        eeArea8             //!< area data read, mpData points to unsigned characters (8 bits)
	} meImageType;
	int   mnNumRows;        //!< number of rows captured
	int   mnNumCols;        //!< number of columns captured
	int   mnStartRow;       //!< first row of captured data
	int   mnStartCol;       //!< first column of captured data
    int   mnVertBinSet;     //!< Vertical Binning Index
    int   mnHorBinSet;      //!< Horizontal Binning Index
	int   mnActiveCols;		//!< Total number of columns (sensor dimensions)
	int   mnActiveRows;     //!< Total number of rows (sensor dimensions)

	/** Image number for current multiframe 
      * capture (1 to N, where N is number of frames requested)
	  */
	unsigned int mnFrameSeqNo;
	unsigned int mnImageNumber;  //!< incrementing total frame count since connection
/**
 *  Data store for originating call.
 * 
 *  For area reads performed using CLSetCCDArea and CLReadCCDArea commands, the data will be populated 
 *  according to the area read ASSUMING a data block consistent with a NRxNC image size, where
 *  NR is the number of rows in the camera, and NC is the number of columns in the camera.
 *  E.G., if the user requests the first 20 columns from all 256 rows of a 256 row x 1024 column
 *  camera, pixels will be written to short (2 byte) offset positions 0-19, 1024-1043, ... 261120-261139.  
 *  It is strongly recommended that users always allocate a full NRxNC short array buffer 
 *  when working with the camera and area reads.
 *
 *  AreaByte reads are similar to area reads on the data is in the format of unsigned character data.
 *
 *  For binned reads performed using CLReadCCDBinned command, the data will be populated accordign
 *  to the binned rows read ASSUMING a data block consistent with NBRxNC image size, where NBR is the
 *  number of output binned rows (rows corresponding to the BIN_SEND opcode) and NC is the number of 
 *  active columns in the camera.  E.G., if the user creates a binning pattern that generates 2 binned
 *  output rows that have been horizontally binned two-to-1 of a 256 row x 1024 column camera, pixels
 *  will be written to integer (4 byte, signed) offset positions 0-512 and 1024-1536, of the buffer.
 */
	void* mpData;
/**
 *  Maximum pixel value prior to digital binning for the image.  Can be used to 
 *  detect saturation conditions while digitally binning data.
 */
	unsigned short mnMaxPixel;
} tsImageData;

/**
 *  This is the required function prototype that an application can use to 
 *  register a callback for any image request results.
 *
 *  \param[out] CameraHandle the handle to the camera initiating the callback
 *  \param[out] tsImageData structure containing the image capture parameters and pointer to data
 * 
 */
typedef void (*IMAGEDATACALLBACK)(int CameraHandle, tsImageData* data);

/**
 * This is the required function prototype that an application must use to register
 * a callback for the CCDsp Camera HeartBeat Data.
 *
 * \param[out] CameraHandle the handle of the camera initiating the callback
 * \param[out] data A pointer to the DLL allocated data structure.  
 *
 * \attention  This structure is not
 *             valid after the callback function returns.  The Application should copy
 *             any data from this structure prior to returning from the callback function.
 *
 * \warning    This callback should not attempt to invoke the same thread as that which may
 *             be performing a CLCamera blocking call (e.g., CLWaitCompletion).
 */
typedef void (*HEARTBEATCALLBACK)(int CameraHandle, HEARTBEAT_DATA* data);

/**
 * This is the required function prototype that an application must use to register 
 * a callback for the CCDsp Camera Error Message Data.
 * 
 * \param[out] severity: 
 *                        - 0: debug
 *                        - 1: status
 *                        - 2: warning
 *                        - 3: error
 *                        - 4: unknown 
 * \note debug level severity is not reported in production camera builds.
 *
 * \param[out] CameraHandle the camera handle generating the error 
 * \param[out] code     error code - currently not used (0)
 * \param[out] string   null terminated string containing error description
 */
typedef void (*ERRORMESSAGECALLBACK)(int CameraHandle, unsigned char severity, unsigned char code, char* ErrorString);

/**
 * Sets a user defined callback function in order to receive camera error messages.
   为了收到相机错误信息设置一个用户定义的回调函数。
 * The pointer in the callback function may not be valid following a return from the callback function. 
   在回调函数指针以下从回调函数的返回可能是无效的。
 * Users should make local copies of the data if it is necessary to preserve the 
 * information following the callback processing.
 * 用户应使数据的本地副本，如果有必要保留以下回调处理的信息。
 * \param[in] CameraHandle A valid CLCamera Handle
 * \param[in,out] ProcessErrorMsg User Supplied Callback Function
 * \return non-zero on error
 */
DECLSPEC int __cdecl CLSetErrorCallback(int CameraHandle,
											ERRORMESSAGECALLBACK ProcessErrorMsg);

/**
 * Sets a user defined callback function in order to receive camera heartbeat information.
 * 为了接收相机心跳信息设置一个用户定义的回调函数。
 * The pointer in the callback function may not be valid following a return from the callback
 * function.
 * 在回调函数指针以下从回调函数的返回可能是无效的。
 * Users should make local copies of the data if it is necessary to preserve the 
 * information following the callback processing.
 *用户应使数据的本地副本，如果有必要保留以下回调处理的信息。
 * \param[in] CameraHandle A valid CLCamera Handle
 * \param[in,out] ProcessHeartBeatData User Supplied Callback Function
 * \return non-zero on error
 */
DECLSPEC int __cdecl CLSetHeartBeatCallback(int CameraHandle,
											HEARTBEATCALLBACK ProcessHeartBeatData);

/**
 * Sets the CLCamera Image Data Callback function that will be called following a 
 * reception of any form of CCD Image Data.  
 * 将摄像头的图像数据的回调函数，将下面的接待任何形式的CCD图像数据的调用。
 * The ImageData Callback function takes the place of the CLSetReadMaskBinningCallback, 
 * the CLSetReadBinningCallback,and the CLSetReadAreaCallback functions, 
 * which were left in the API for backwards compatibility.
 * 为imageData回调函数采用CLSetReadMaskBinningCallback的CLSetReadBinningCallback和CLSetReadAreaCallback功能，
 * 这是为了向下兼容的API中的位置。
 * \param[in] CameraHandle A valid CLCamera Handle
 * \param[in,out] ProcessImageData callback function for processing the data when it arrives
 * \return non-zero on error
 */
DECLSPEC int __cdecl CLSetImageDataCallback(int CameraHandle, IMAGEDATACALLBACK ProcessImageData);

/**
 * Sets exposure time for timed exposures.  When set to 0, exposure timing
 * is disabled.
 * 设置曝光时间定时曝光。如果设置为0，曝光时间被禁用。
 *  If exposure is set, the following sequence occurs when a CLReadXXX() is called:
 * 如果曝光设置，按以下顺序时出现CLReadXXX（）被调用：                 
 *  The exposure time is set to the set value.
 *  曝光时间被设定为设定值。
 *  Once the exposure is complete, the camera is read out according to
 *  desired CLReadXXX() function
 *  一旦曝光完成后，照相机根据所需CLReadXXX读出（）函数
 *  The ReadXXX() call should normally be preceded by a CLClearCCD() call to
 *  clear out the imager.
 *  ReadXXX()调用一般应于一个CLClearCCD（）调用前面到清除出成像器。
 * \param[in] CameraHandle A valid CLCamera Handle
 * \param[in] Milliseconds exposure time in milliseconds
 * \return non-zero on error.
 */
DECLSPEC int __cdecl CLSetExposure (int CameraHandle, long int Milliseconds);

/**
 * Sets exposure time for timed exposures.
 *设置曝光时间定时曝光。
 * NOTE: This function only works for a subset of cameras. Contact Critical
 *       Link if you are unsure if your camera can use this function.
 *	注：此功能仅适用于摄像头的子集。如果您不确定如果您的相机可以使用这个功能联系关键环节。
 *  If exposure is set, the following sequence occurs when a CLReadXXX() is called:
 *	如果曝光设置，按以下顺序时出现CLReadXXX（）被调用：
 *  The exposure time is set to the set value.
 *  曝光时间被设定为设定值。
 *  Once the exposure is complete, the camera is read out according to
 *      desired CLReadXXX() function
 * 一旦曝光完成后，照相机根据所需CLReadXXX读出（）函数
 * \param[in] CameraHandle A valid CLCamera Handle
 * \param[in] Microseconds exposure time in Microseconds
 * \return non-zero on error.
 */
DECLSPEC int __cdecl CLSetExposureEx (int CameraHandle, long int Microseconds);

/**
 * Sets shutter delay time parameters.
 * 设定快门延迟时间参数。
 * The camera shutter opening delay, closing delay, and output pulse logic 
 * true levels can be configured with this call.  During an exposure:
 * 相机的快门延迟开启，关闭延迟，以及输出脉冲逻辑真水平可以用这个电话进行配置。在曝光：
 * The camera will open the shutter (TTL pulse '1' when ActiveHigh is non-zero, 
 * 相机将打开快门（TTL脉冲'1'时ActiveHigh非零，'0'，否则）
 * '0' otherwise).  The camera will delay for OpenMs milliseconds.  
 * 相机将推迟OpenNMS的毫秒。
 * The camera will then expose for the exposure time from CLSetExposure().  
 * 然后相机将公开从衣柜里曝光的曝光时间（）。
 * The camera will close the shutter (TTL pulse '0' when ActiveHigh is non-zero, '1' otherwise).
 * 相机将关闭快门（TTL脉冲“0”时ActiveHigh非零，'1'，否则）。
 * The camera will delay CloseMs milliseconds.  Then the CCD pixel data will be read/shifted from the CCD array.
 * 相机会耽误撤防毫秒。然后，CCD像素数据将是红色的从CCD阵列。
 * \param[in] CameraHandle A valid CLCamera Handle
 * \param[in] OpenMs Milliseconds to delay while waiting for shutter to open
 * \param[in] CloseMs Milliseconds to delay while waiting for shutter to close
 * \param[in] ActiveHigh when non-zero, camera asserts '1' to open shutter.  When
 *            zero, camera asserts TTL '0' to open shutter.
 * \return non-zero on error.
 */
DECLSPEC int __cdecl CLSetShutterParams(int CameraHandle, long int OpenMs, 
										long int CloseMs, int ActiveHigh);


/**
 * Read Back shutter delay time parameters.  See CLSetShutterParams for details.
 * 读回的快门延迟时间参数。详情请参见CLSetShutterParams。
 * \param[in] CameraHandle A valid CLCamera Handle
 * \param[in] apOpenMs pointer to store shutter open time (in ms)
 * \param[in] apCloseMs pointer to store shutter close time (in ms)
 * \param[in] apActiveHigh pointer to store shutter active high or low
 * \return non-zero on error.
 */
DECLSPEC int __cdecl CLGetShutterParams(int CameraHandle, long int* apOpenMs,
                                        long int* apCloseMs, int* apActiveHigh);

/**
 *  Sets the desired interval between the start of exposure from one image 
 *  capture cycle to the next.  This interval typically includes the transfer
 * 设定曝光从一个图像捕捉周期到下一个开始之间所需的间隔。
 *  time between the camera and the host end point (typically a PC). 
 * 该间隔通常包括相机和主机端点（通常是个人计算机）之间的传输时间。
 *  The  desired interval time may not be what is actually achievable, and users
 *  should verify the cycle time using the GetImageInterval call.
 * 所需的间隔时间可能不会有什么实际上是可以实现的，和用户应使用的getImage间隔调用验证周期。
 *  \note Setting the DesiredInvervalMs to 0 will request the camera to operate
 *  as rapidly as possible. 
 * 注意DesiredInvervalMs设置为0将请求相机尽可能迅速地进行操作。
 *  This may result in jitter in cycle times as the interface mechanism 
 * (e.g., TCP transmit times using ethernet)
 *  may not be reliably consistent at "fast as possible rates".  
 * 这可能会导致在循环时间作为接口机制抖动在“快尽可能率”（例如，TCP发送使用以太网次）可能无法可靠地一致。
 * Setting this value to non-zero will cause the camera to use the best settings possible
 * while gauranteeing a consistent update cycle.
 * 将此值设置为非零会导致相机同时保证一致的更新周期使用的最佳设置成为可能。
 *  \param[in] CameraHandle A valid CLCamera Handle
 *  \param[in] DesiredIntervalMs desired image capture cycle time in milliseconds.
 *  \return non-zero on error (illegal camerahandle).
 */
DECLSPEC int __cdecl CLSetImageInterval(int CameraHandle, unsigned long int DesiredIntervalMs);

/**
 *  Returns the actual image capture intervals that the CCDsp camera based on the 
 *  current image transfer settings (binning parameters, area size, etc.) as well 
 *  as the connection interface and the last set CLSetImageInterval().
 *  返回的CCDsp相机基于当前图像传送设置的实际图像捕获间隔（分级参数，区域尺寸等）
 *  以及连接接口和最后一组CLSetImageInterval（）。
 *  If the values for interval times are 0, then the camera will start (if configured)
 *  a new exposure immediately after all data is sent, and the interval times are 
 *  "as fast as possible". 
 *  如果间隔时间的数值是0，
 *  则该相机将立即开始（如果配置）一个新曝光的所有数据被发送之后，和间隔时间为“尽可能快地”。
 *  In this mode, the image updates are not necessarily consistent from capture to capture.
 *	在这种模式下，图像的更新不一定一致从捕捉到捕捉。
 *  Different values are provided given the various binning, area, or area masked binned
 *  configurations.  
 *  提供不同的值赋予不同的偏置，区域或区域屏蔽分级配置。
 *  Input pointer arguements may be NULL (and not used) if they are not needed by the application.
 * 	如果由应用中不需要它们输入指针参数可以是NULL（和不使用）。
 *  \param[in] CameraHandle A valid CLCamera Handle
 *  \param[out] AreaReadMs pointer to location to store expected Area Read image interval
 *  \param[out] BinnedReadMs pointer to location to store expected Binned Read image interval
 *  \param[out] AreaMaskedReadMs pointer to location to store expected Area Masked Read image interval
 *  \param[out] CustomReadMs not currently used.
 *  \return non-zero on error.
 */
DECLSPEC int __cdecl CLGetImageIntervals(int CameraHandle, 
                                        unsigned long int* AreaReadMs,
                                        unsigned long int* BinnedReadMs,
                                        unsigned long int* AreaMaskedReadMs,
                                        unsigned long int* CustomReadMs);
/**
 *  Set Camera / Image shearing parameters
 *	将相机/图像剪切参数
 *  This function configures the camera to apply peak/noise shearing to a captured image.
 *  该函数配置摄像头应用高峰/噪音剪切到拍摄的图像。 
 *  Given a window of WinRowsxWinCols centered on a pixel of interest, the average
 *  (excluding the pixel of interest) is computed.  
 *  定集中在感兴趣的像素WinRowsxWinCols的一个窗口，平均（不包括感兴趣的像素）被计算。
 *  If the pixel of interest exceeds the average by Threshold counts, 
 *	the pixel of interest is replaced by the computed average.
 *  如果所关心的象素超过由阈值计数的平均，感兴趣的像素是通过将计算出的平均取代。
 *  This routine is useful for removing pixel noise due to cosmic rays or other forms of 
 *  radiation noise which may present during prolonged camera exposures.
 *  这个程序是由于宇宙射线或其他形式的辐射噪声可能在长时间曝光的相机取出呈现像素噪声非常有用。
 *  \note shearing is performed before binning in the DSP (but after any vertical binning
 *        in the CCD readout register).
 *  注剪切在DSP分档之前执行（但之后的CCD读出寄存器任何垂直分级）。
 *  \param[in] CameraHandle A valid CLCamera Handle
 *  \param[in] Enable set to non-zero to enable image shearing (disabled by default)
 *  \param[in] WinRows the averaging region height (in rows) must be ODD
 *  \param[in] WinCols the averaging region width (in columns) must be ODD
 *  \param[in] Threshold the pixel threshold that must be exceeded for shearing to occur
 *
 *  \return non-zero on error (even WinRows or WinCols, bad camera handle).  0 on success
 */
DECLSPEC int __cdecl CLSetShearingParams(int CameraHandle, int Enable, unsigned int WinRows,
										 unsigned int WinCols, unsigned int Threshold);

/**
 * Sets the parameters for subsequent ReadCCDArea() Commands.
 * 
 * \param[in] CameraHandle A valid CLCamera Handle
 * \param[in] RowStart     starting row pixel for readout (zero based)
 * \param[in] ColStart     starting column pixel for readout (zero based)
 * \param[in] NRows        number of rows to read out starting from RowStart
 * \param[in] NCols        number of cols to read out starting from ColStart
 * \return non-zero on error.
 */
DECLSPEC int __cdecl CLSetCCDArea(int CameraHandle, int RowStart, int ColStart,
                                  int NRows, int NCols);



/**
 *  Configure the vertical binning pattern of subsequent CLReadCCDBinned calls.
 *  配置后续CLReadCCDBinned呼叫的垂直分级图案。
 *  Example:
 *  @code
 *   BinCode[] = { BINCODE_BIN, BINCODE_BIN, BINCODE_BIN, BINCODE_DISCARD,
 *                 BINCODE_BIN, BINCODE_BIN, BINCODE_BIN, BINCODE_DISCARD,
 *
 *                 BINCODE_BIN, BINCODE_SUM, BINCODE_BIN, BINCODE_SUM,
 *                 BINCODE_BIN, BINCODE_SUM, BINCODE_BIN, BINCODE_SEND,
 *
 *                 BINCODE_BIN, BINCODE_SUM, BINCODE_BIN, BINCODE_SUM,
 *                 BINCODE_BIN, BINCODE_SUM, BINCODE_BIN, BINCODE_SEND,
 *
 *                 BINCODE_BIN, BINCODE_SUM, BINCODE_BIN, BINCODE_SUM,
 *                 BINCODE_BIN, BINCODE_SUM, BINCODE_BIN, BINCODE_SEND,
 *
 *                 BINCODE_BIN, BINCODE_SUM, BINCODE_BIN, BINCODE_SUM,
 *                 BINCODE_BIN, BINCODE_SUM, BINCODE_BIN, BINCODE_SEND,
 *
 *                 BINCODE_BIN, BINCODE_BIN, BINCODE_BIN, BINCODE_DISCARD,
 *                 BINCODE_BIN, BINCODE_BIN, BINCODE_BIN, BINCODE_DISCARD,
 *
 *                 ...
 *
 *                 BINCODE_BIN, BINCODE_BIN, BINCODE_BIN, BINCODE_DISCARD,
 *                 BINCODE_BIN, BINCODE_BIN, BINCODE_BIN, BINCODE_DISCARD }
 *   @endcode
 *
 *
 *   This will first discard 8 lines by binning 4 lines at a time and
 *   discarding the data (2 times total).  
 *   这将首先通过像素合并4行的时间和丢弃数据（2倍数量）丢弃8行。
 *   Then two lines will be binned in the CCD and read out. 
 *   然后，两行将会在CCD被禁止并读出。
 *   Another two lines will be binned and read out. 
 *   另外两条线将被分级，并读出。
 *	 The second read out line will be added to the first.
 *   第二读出的线将被添加到第一个。
 *   This will continue until eight read out lines have been summed. 
 *   这将继续下去，直到8个读出线路已求和。
 *   This summed line data will then be sent out by the camera as 1024 long ints.
 *   这个求和线数据将被由相机1024长整数送出。
 *   Next another two lines will be read out and binned, 
 *   下一个另一两条线将被读出并分级，
 *   and this is done summing a total of eight times to generate the second 1024 points of data sent to the camera.
 *   并且这样做求和总共八次，以产生发送到所述相机数据的第二1024点。
 *   This readout process will repeat two more times, resulting in a total
 *   of four sets of 1024 long ints.  
 *   此读出处理将重复两次以上，造成总共四个套1024长整数。
 *   The remaining rows of the CCD will then be read out (four at a time) and discarded.
 *   CCD的其余行随后将被读出（4，一次），并丢弃。
 *   This function sets up the parameter for this readout, but doesn't
 *   initiate a readout.  That is done by CLReadCCDBinned.
 *	 这个函数设置为这个读出的参数，但不启动读数。是受CLReadCCDBinned完成。
 *   This routine is idendical to CLSetBinParam, but allows the user to specify
 *   a binning index number (from 0 to (CL_MAX_BINSETS-1)), which will allow storing different 
 *   binning sets on the camera.
 *   这个程序是相同的CLSetBinParam，但是允许用户指定一个分级指数（从0到（CL_MAX_BINSETS-1）），
 *   这将允许在相机上存储不同分级集。
 * \param[in] CameraHandle A valid CLCamera Handle
 * \param[in] BinCodes     BinCodes command (one per CCD row) sequence. Valid commands are:
 *        - BINCODE_BIN = keep row in readout register
 *        - BINCODE_DISCARD = read out and discard row
 *        - BINCODE_SUM = read out row and sum in FPGA
 *        - BINCODE_SEND = read out row, sum in FPGA and send results to PC
 * \param[in] BinNumber  Binning set to store, range 0 to (CL_MAX_BINSETS-1)
 * \return non-zero on error.
 */
DECLSPEC int __cdecl CLSetBinParam(int CameraHandle, unsigned char* BinCodes, int BinNumber);

/**
 *  Configure the horizontal binning pattern of subsequent CLReadCCDBinned calls.
 *
 *  Example:
 *  @code
 *   BinCode[] = { BINCODE_DISCARD, BINCODE_DISCARD, BINCODE_DISCARD, BINCODE_DISCARD,
 *                 BINCODE_DISCARD, BINCODE_DISCARD, BINCODE_DISCARD, BINCODE_DISCARD,
 *
 *                 BINCODE_SUM, BINCODE_SUM, BINCODE_SUM, BINCODE_SUM,
 *                 BINCODE_SUM, BINCODE_SUM, BINCODE_SUM, BINCODE_SEND,
 *
 *                 BINCODE_SUM, BINCODE_SUM, BINCODE_SUM, BINCODE_SUM,
 *                 BINCODE_SUM, BINCODE_SUM, BINCODE_SUM, BINCODE_SEND,
 *
 *                 BINCODE_SUM, BINCODE_SUM, BINCODE_SUM, BINCODE_SUM,
 *                 BINCODE_SUM, BINCODE_SUM, BINCODE_SUM, BINCODE_SEND,
 *
 *                 BINCODE_SUM, BINCODE_SUM, BINCODE_SUM, BINCODE_SUM,
 *                 BINCODE_SUM, BINCODE_SUM, BINCODE_SUM, BINCODE_SEND,
 *
 *                 BINCODE_DISCARD, BINCODE_DISCARD, BINCODE_DISCARD, BINCODE_DISCARD,
 *                 BINCODE_DISCARD, BINCODE_DISCARD, BINCODE_DISCARD, BINCODE_DISCARD,
 *
 *                 ...
 *
 *                 BINCODE_DISCARD, BINCODE_DISCARD, BINCODE_DISCARD, BINCODE_DISCARD,
 *                 BINCODE_DISCARD, BINCODE_DISCARD, BINCODE_DISCARD, BINCODE_DISCARD }
 *   @endcode
 *
 *
 *   This will first discard 8 Columns of data. Then the next 8 columns will be added
 *   and combined into a single pixel for a given row and sent to the host.  This 
 *   process repeats for 3 more times.  The remaining pixels are all discarded in the row.
 *   The final output sent to the host will be 4 pixels per row.  
 *
 *   This function sets up the horizontal binning parameters for a readout, but doesn't
 *   initiate a readout.  That is done by CLReadCCDBinned or CLReadCCDBinnedEXT.
 *
 * \note The horizontal and vertical binning set by CLSetBinParam and CLSetHorBinParam 
 *       are both applied when a binned image is captured by CLReadCCDBinned.  By default,
 *       horizontal binning is set to BINCODE_SEND for all columns.
 * 
 * \param[in] CameraHandle A valid CLCamera Handle
 * \param[in] BinCodes    Bincode command (one per CCD column) sequence. Valid commands are:
 *        - BINCODE_DISCARD = discard column
 *        - BINCODE_SUM = SUM column with next column in row.
 *        - BINCODE_SEND = output pixel with current accumulated column value in a given row.
 * \param[in] BinNumber  Horizontal Binning Set, range 0 through 7.
 * \return non-zero on error.
 */
DECLSPEC int __cdecl CLSetHorBinParam(int CameraHandle, unsigned char* BinCodes, int BinNumber);

/**
 * @name Constants used in configuring direction to bin in.
 * 
 */
//@{
	#define BIN_VERTICALLY 1      // combines pixels along the short array axis
	#define BIN_HORIZONTALLY 2  // combines pixels along the long array axis
//@}

/**
 *  
 *	 Configure the binning pattern of subsequent CLReadCCDMaskBinned calls.
 *
 *  Example:
 *	 @code
 *   rc = CLSetBinMaskParam(hCam, BinCode, BIN_VERTICALLY);
 *
 *   BinMaskCode[256*1024]  = {
 *   1, 1, 1, 1, 1, 1, 1, 1, ? 1, 1,
 *   1, 1, 1, 1, 1, 0, 0, 0,?, 0, 0,
 *   0, 0, 0, 0, 0, 0, 0, 0,?, 2, 2,
 *   0, 0, 0, 2, 2, 2, 2, 2,?, 2, 2,
 *   2, 2, 2, 2, 2, 2, 2, 2,?, 2, 2,
 *    ?
 *   8, 8, 8, 8, 8, 8, 8, 8,?, 8, 8,
 *   };
 *	
 *	 @endcode
 *
 *   This example will result in 8 rows of 1024 bins.  Any pixel marked with a 0 will be 
 *   discarded.   Pixels marked with a 1 will be binned into spectrum 1. Pixels marked with 
 *   2 will be binned into spectrum 2.  etc.
 *
 *   This function sets up the parameter for this readout, but doesn抰
 *   initiate a readout.  That is done by CLReadMaskBinned().
 *
 *   Note that BinMaskCode is a pointer to a 1D array of consecutive rows of the bin mask code.
 *
 * \param[in] CameraHandle A valid CLCamera Handle
 * \param[in] BinMaskCode code indicating the spectral array to bin this pixel into
 * \param[in] HorizVertical direction to bin in
 */
DECLSPEC int __cdecl CLSetBinMaskParam(int CameraHandle, unsigned char * BinMaskCode, int HorizVertical);

/**
 * Reads the Exposure Time currently configured in the CLCamera
 *读取目前在摄像头配置的曝光时间
 * \param[in] CameraHandle A valid CLCamera Handle
 * \param[out] Milliseconds location to place exposure time in milliseconds
 * \return non-zero on error.  This call will block until the value is acuired
 */
DECLSPEC int __cdecl CLGetExposureTime(int CameraHandle, long int * Milliseconds);

/**
 * Reads the Exposure Time currently configured in the CLCamera
 *读取目前在摄像头配置的曝光时间
 * NOTE: This function only works for a subset of cameras. Contact Critical
 *       Link if you are unsure if your camera can use this function.
 *
 * \param[in] CameraHandle A valid CLCamera Handle
 * \param[in] Microseconds location to place exposure time in Microseconds
 * \return non-zero on error.
 */
DECLSPEC int __cdecl CLGetExposureEx (int CameraHandle, long int * Microseconds);

/**
 * Reads the CCD Read Area parameters currently configured in the CLCamera
 *读取目前在摄像头配置的CCD读取区参数
 * \param[in]  CameraHandle A valid CLCamera Handle
 * \param[out] RowStart  location to write the starting row location
 * \param[out] ColStart  location to write the starting column location
 * \param[out] NRows     location to write the number of rows to read
 * \param[out] NCols     location to write the number of columns to read
 * \return non-zero on error.  This function is a blocking function.
 */
DECLSPEC int __cdecl CLGetCCDArea(int CameraHandle, int* RowStart, int* ColStart,
                                         int* NRows, int* NCols);

/**
 * Reads the Binning Parameters currently configured in the CLCamera
 * 读取分级的参数目前在摄像头配置
 * \param[in]  CameraHandle A valid CLCamera Handle
 * \param[in]  Index binning index number (from 0 to (CL_MAX_BINSETS-1))
 * \param[out] BinCode location to write the binning parameters (array of characters)
 * \param[in]  BinCodeSize size (in bytes) of BinCode array
 * \return non-zero on error.  This is a blocking function call.
 */
DECLSPEC int __cdecl CLGetBinParam(int CameraHandle, int Index, unsigned char* BinCode,
                                   int* BinCodeSize);

/**
 * Reads the Binning Parameters currently configured in the CLCamera
 * 读取分级的参数目前在摄像头配置
 * \param[in]  CameraHandle A valid CLCamera Handle
 * \param[in]  Index binning index number (from 0 to (CL_MAX_BINSETS-1))
 * \param[out] BinCode location to write the binning parameters (array of characters)
 * \param[in]  BinCodeSize size (in bytes) of BinCode array
 * \return non-zero on error.  This is a blocking function call.
 */
DECLSPEC int __cdecl CLGetHorBinParam(int CameraHandle, int Index, unsigned char* BinCode,
                                      int* BinCodeSize);

/**
 * Reads the Binning Mask Parameters currently configured in the CLCamera
 * 读取目前在摄像头配置的分档覆盖参数
 * \param[in]  CameraHandle A valid CLCamera Handle
 * \param[out] BinMask  location to write the binning masks (array of characters)
 * \param[in]  BinMaskSize size (in bytes of the BinMask Array
 * \param[out] HorizBinning location to write the horizontal binning parameter
 * \return non-zero on error.  This is a blocking function call.
 */
DECLSPEC int __cdecl CLGetBinMaskParam(int CameraHandle, unsigned char *BinMask,
									   int *BinMaskSize, int *HorizBinning);

/**
 * Reads the temperature of the CCD reference thermistor on the CLCamera.
 * 读取CCD参考热敏电阻的温度在相机上
 * \note The temperature reported is the same as that in the HEARTBEAT_DATA structure.
 *
 * \param[in]  CameraHandle A valid CLCamera Handle
 * \param[out] temp        location to write the temperature (signed short, LSB = 0.1 degrees C)
 * \return  non-zero on error. This is a blocking function call.
 */
DECLSPEC int __cdecl CLGetCCDTemp(int CameraHandle, short* temp);

/**
 * Reads the current I/O Board temperature on the CLCamera.
 * 读取上摄像头的电流I / O板的温度。
 * \note The temperature reported is the same as that in the HEARTBEAT_DATA structure.
 *
 * \param[in] CameraHandle A valid CLCamera Handle
 * \param[out] temp        location to write the temperature (signed short, LSB = 0.1 degrees C)
 * \return  non-zero on error. This is a blocking function call.
 */
DECLSPEC int __cdecl CLGetBoardTemp(int CameraHandle, short* temp);

/** 
 *  Aborts a multi-image read.  Camera stops after completion of next remaining image set.
 * 放弃一个多图像读取。接下来剩下的图像集结束后，摄像机停止。
 *  \return non-zero on error.
 */
DECLSPEC int __cdecl CLAbortRead(int CameraHandle);

/**
 * Reads the CCD Area Data.  This function is non-blocking.  If no callback is installed,
 * the user must check that the operation has completed (using CLWaitCompletion) prior to accessing the Data.
 * 读取CCD面积的数据。这个功能是非阻塞的。如果没有安装回调，
   用户必须检查操作已完成（使用CLWaitCompletion）访问数据之前。 
 * Otherwise the callback will be invoked following a completed read.
 * 否则，回调将以下完成的读取调用。
 * \warning If a subarea read is performed using CLSetCCDArea command, the data will be populated 
 *          according to the area read ASSUMING a data block consistent with a NRxNC image size, where
 *          NR is the number of rows in the camera, and NC is the number of columns in the camera.
 *          E.G., if the user requests the first 20 columns from all 256 rows of a 256 row x 1024 column
 *          camera, pixels will be written to short offset positions 0-19, 1024-1043, ... 261120-261139.  
 *          It is strongly recommended that users always allocate a full NRxNC short array buffer 
 *          when working with the camera.
 * 如果使用CLSetCCDArea命令进行的分区的读取，数据将根据该区域填充读基于假设用NRxNC图像尺寸，
 * 其中NR是行中的摄像机的数量，和NC是列中的数字相一致的数据块 相机。
 * 例如如果用户从所有256行的256行×1024列照相机的请求第一20列，
 * 像素将被写入短偏移位置0-19，1024至1043，...261120-261139。
 * 强烈建议用相机工作时，用户始终分配一个完整的NRxNC短数组缓冲区。
 * \param[in] CameraHandle A valid CLCamera Handle
 * \param[out] Data 16-bit image data, which should be sized appropriately.  If NULL, an internal
 *             buffer will be used and provided in the callback.  Internal buffers are only
 *             valid until the callback function returns.  Copies should be made if needed outside
 *             of the callback context.
 * \param[in]  NumImages number of images to read.  A value of 0 will read continuously until aborted.
 * \param[in]  NumClears Number of times to clear prior to each image capture.
 * \return non-zero on error.
 */
DECLSPEC int __cdecl CLReadCCDArea(int CameraHandle, 
                                   unsigned short * Data, 
                                   unsigned int NumImages,
                                   unsigned int NumClears);

/**
 * Reads the CCD using the Binning parameters.  This function is non-blocking.  If no callback
 * is installed, the user must check that the operation has completed prior to accessing the Data.
 * 
 * Otherwise the callback will be invoked following a completed read.
 *
 * \warning For binned reads performed using CLReadCCDBinned command, the data will be populated according
 *  to the binned rows read ASSUMING a data block consistent with NBRxNC image size, where NBR is the
 *  number of output binned rows (rows corresponding to the BIN_SEND opcode) and NC is the number of 
 *  active columns in the camera.  E.G., if the user creates a binning pattern that generates 2 binned
 *  output rows that have been horizontally binned two-to-1 of a 256 row x 1024 column camera, pixels
 *  will be written to integer (4 byte, signed) offset positions 0-512 and 1024-1536, of the buffer.  Users
 *  must allocate enough memory assuming no horizontal binning is applied and stride the resulting
 *  data stream appropriately.
 *
 * \param[in] CameraHandle A valid CLCamera Handle
 * \param[out] Data        array of pixel values to be filled.  If null, an internal buffer
 *                         will be used and returned to the callback function.
 * \param[out] MaxPixel    location to write the maximum value of the read.  If NULL, not used.
 * \param[in]  NumImages   Number of images.  A value of 0 will read continuously until aborted.
 * \param[in]  NumClears   Number of clears.
 * \return 
 */
DECLSPEC int __cdecl CLReadCCDBinned(int CameraHandle, 
                                     long int* Data,
                                     unsigned short* MaxPixel,
                                     int NumImages,
                                     int NumClears);

/**
 * Reads the CCD using the Masked Binning parameters.  This function is non-blocking.  If no callback
 * is installed, the user must check that the operation has completed prior to accessing the Data.
 * 读取使用屏蔽分级参数的CCD。这个功能是非阻塞的。如果没有安装回调，用户必须检查操作之前访问数据完成。
 * Otherwise the callback will be invoked following a completed read.
 * 否则，回调将以下完成的读取调用。
 * \warning The Data pointer in this method assumes a 2 dimension array to populate that is NRows x NCols
 *          pixels in size.  Horizontally binned data (collapsed along rows) wlll yeild data points separated
 *          by NCols integers from one row to the next.
 *
 * \param[in]  CameraHandle A valid CLCamera Handle
 * \param[out] data         array of pixel values to be filled, this is sized as NRowsx1024 pixels in nature.
 * \param[out] MaxPixel     location to store maximum pixel seen in CCD (may be NULL)
 * \param[in]  NumImages    Number of images.  A value of 0 will read continuously until aborted.
 * \param[in]  NumClears    Number of clears.
 * \return non-zero on error
 */
DECLSPEC int __cdecl CLReadCCDMaskBinned(int CameraHandle, long int* data, unsigned short *MaxPixel, int NumImages, int NumClears);

/**
 * Enables or disables the cooling in the camera.
 *启用或禁用相机中的冷却。
 * \param[in] CameraHandle A valid CLCamera Handle
 * \param[in] CoolingEnabled 
 *            - 0: No Cooling
 *            - 1: Cooling
 * \return non-zero on error.
 */
DECLSPEC int __cdecl CLSetCooling(int CameraHandle, int CoolingEnabled);

/**
 * Reads back the currently configured CLCamera Cooling parameters.
 * 读回当前配置的摄像机冷却参数。
 * \param[in]  CameraHandle A valid CLCamera Handle
 * \param[out] CoolingEnabled location to write the cooling value (0, or 1)
 * \return non-zero on error.
 */
DECLSPEC int __cdecl CLGetCooling(int CameraHandle, int * CoolingEnabled);

/**
 * Command the CCD to clear.  This is a non-blocking call.  To force the application to 
 * wait until this operation completes, the user should use the CLWaitCompletion function.
 * 命令CCD清除。这是一个非阻挡呼叫。要强制应用程序
 * 等到该操作完成后，用户应使用CLWaitCompletion功能。
 * \param[in] CameraHandle A valid CLCamera Handle
 * \param[in] NTimes       number of times to clear camera 
 * \param[in] Binning      number of rows to bin while clearing (not used, set to 0)
 * \return non-zero on error.
 */
DECLSPEC int __cdecl CLClearCCD (int CameraHandle, int NTimes, int Binning);

/**
 * Wait for all currently queued camera applications to complete before continuing with forced timeout.
 * 等待所有当前排队相机应用强迫超时完成后再继续。
 * This method is useful for applications that must know when a read or clear operation
 * has been completed in order to syncronize optical equipment alignment (shutters, lenses, etc.).
 * 这种方法对于必须知道应用程序是非常有用的，当读取或清除操作已经以同步光设备对准（百叶窗，透镜等）完成。
 * The user can adjust the timeout period if significantly long exposures are being used (> 10 seconds).
 * 
 * \note The timeout time should be > 9 seconds plus the exposure time.
 *
 * \param[in] CameraHandle A valid CLCamera Handle
 * \param[in] TimeoutTime_Ms - Time in milliseconds that camera should wait before declaring a Timeout condition.
 *                             set this value to CL_DEFAULT_TIMEOUT to use the default timeout settings in the camera.
 * \return CL_CAMERA_READY all operations completed successfully
 *         -1 if no camera exists with this handle
 */
DECLSPEC int __cdecl CLWaitCompletion(int CameraHandle, int TimeoutTime_Ms);


/**
 * This function resets the communications link between the camera and PC
 * and stops any camera operation that is pending.
 *  此功能将相机与计算机之间的通信链路，并停止该挂起相机操作。
 * \param[in] CameraHandle A valid CLCamera Handle
 * \return non-zero on error.
 */
DECLSPEC int __cdecl CLCameraReset (int CameraHandle);

/**
 * Utility command used to pass engineering commands to the DLL software.
 *实用命令用于工程的命令传递给DLL软件。
 * \attention This function is not supported and is intended for internal
 *            development use only.
 * 此功能不支持仅供内部开发使用。
 * \param[in] CameraHandle  a valid CLCamera Handle
 * \param[in] command       integer command
 * \param[in] pData         data block for optional command parameters
 * \return non-zero on error.  This method is a blocking call.
 */
DECLSPEC int __cdecl CLEngineeringCommand(int CameraHandle, int command, void* pData);

/**
 * Commands the CLCamera uDSP to configure the DAC such that the average bias of the CCD pre-amp circuit,
 * aligns with the Target value specified.  This is a blocking call.
 * 命令摄像机向上配置DAC使得在CCD前置放大器电路的平均偏差，对准使用指定的目标值。这是一个阻塞调用。
 * \note The CCD substrate voltage should be set prior to making this call.
 * \note Typical values for targets are 400 counts.
 *
 * \param[in] CameraHandle A valid CLCamera Handle
 * \param[in] Target Target value (in CCD counts) to attempt to achieve
 * \param[in] ApplyZeroMean if true, the uDSP will subtract the mean on all Binned Reads 
 * \return non-zero on error.
 */
DECLSPEC int __cdecl CLSetCCDOffset(int CameraHandle, unsigned short Target, bool ApplyZeroMean);

/**
 * Non blocking call which provides percent complete status of ongoing read area or read binned operation.
 * 非阻塞调用提供持续的读取区或读装箱操作完成百分比状态。
 * Applications may use this command to provide image read status to the operator on CLCameras with slow
 * physical interfaces.
 * 应用程序可以使用这个命令提供的图像读取的状态对相机操作有慢物理接口。
 * \param[in]  CameraHandle 
 * \param[out] ImageNum Location to store. Current ImageNumber status corresponds to.  Starts at 1.  If NULL not used.
 * \return    Percent Complete of current Image/Frame.  Value will be 100 if no read area or read binned operation is active.
 */
DECLSPEC int __cdecl CLGetReadProgress(int CameraHandle, unsigned int* ImageNum);

/**
 * Blocking call to camera which extracts readout noise from the pre-amp conversion circuit without 
 * extracting the charge from the CCD camera.  This method is provided solely for characterization of
 * the CCDsp camera circuitry between the CCD output and the ADC output.
 * 阻塞调用相机从前置放大器的转换电路提取读出噪声，而不提取的CCD相机充电。
 * 纯粹为CCD输出和ADC输出之间的CCDsp相机电路的特性提供了这种方法。
 * \param[in] CameraHandle A valid CLCamera Handle
 * \param[in] Mean the mean value of the readout noise (should be the same as from setCCD offset
 * \param[out] std the standard of deviation of the readout noise 
 * \return non-zero on error.
 *
 */
DECLSPEC int __cdecl CLGetReadoutNoise(int CameraHandle, float* Mean, float* std);

/**
 *  This is the required function prototype that an application must use to 
 *  register a new message callback.
 *  这是一个应用程序必须使用注册一个新的消息回调所需的函数原型。
 *  \param[in] CameraHandle the handle to the camera initiating the callback
 *  \param[in] msg the pointer to the user data area passed by the application to the CLReadCCDArea Call
 *  \param[in] length the number of points contained in the message
 * 
 */
typedef void (*USERDEFINEDCALLBACK)(int CameraHandle, void * msg, int length);

/**
 * Sets the CLCamera User Defined Message Callback function that will be called after
 * receiving a user defined message.
 * 将摄像头的用户定义的消息回调函数，将接收到用户定义的消息后调用。
 * \param[in] CameraHandle  a valid CLCamera Handle
 * \param[in,out] ProcessUserDefined callback function for processing the message when it arrives
 * \return non-zero on error
 */
DECLSPEC int __cdecl CLSetUserDefinedCallback (int CameraHandle,
                 USERDEFINEDCALLBACK ProcessUserDefined);

/**
 * Sends a user defined message to the CCDSP.  This message should only be used if 
 * the CCDSP camera has been customized for a specific operation.  Please contact
 * the Critical Link LLC engineering team for more details.
 * 发送用户定义的消息CCDSP。该消息只应如果使用的话
 * 该CCDSP相机已定制了特定的操作。请联系的关键纽带LLC的工程团队的更多细节。
 * \param[in]  CameraHandle A valid CLCamera Handle
 * \param[in]  msg   the user defined message to send
 * \return     0 on success, -1 on timeout
 */
DECLSPEC int __cdecl CLSendUserDefined(int CameraHandle, void * msg);

/**
 *  This structure is used to contain the results provided by the CLLocateEnetCameras command.
 *
 */
typedef struct
{
    struct sockaddr_in msAddr;        //!< internet (IPV4) address of found camera
	unsigned short     mnServerPort;  //!< internet port number for camera socket
	int                mnSerialNo;    //!< serial number of found camera
	int                meCameraType;  //!< always 12
} tsEnetData;

/**
 *  This function will poll the current network (using a UDP broadcast protocol) 
 *  for any connected CCDsp Cameras and populate the user provided apData structure
 *  with the results.
 * 此功能将轮询任何连接CCD摄像机当前网络（使用UDP广播协议），并用结果填充用户提供的数据的结构。
 *  \param[out] apData pointer to user allocated structure to hold the results
 * 
 *  \param[in] anNumBuffers number of buffers allocated structure may hold
 *
 *  \return -1 on error or number of cameras located (zero being a valid output)
 */
DECLSPEC int __cdecl CLLocateEnetCameras(tsEnetData* apData, int anNumBuffers);

#define CCDSP_PRODUCT_ID 0x1005 //!< the nominal CCDSP USB Product ID
#define CCDSP_VENDOR_ID  0x1EF8 //!< the nominal CCDSP USB Vendor  ID

/** 
 *  This structure is used by the CLLocateHSUSBCameras call.
 */
typedef struct
{
    int      mnDeviceIndex;     //!< device index to use for CLOpenHSUSBCamera()
    int      mnVendorID;        //!< USB enumerated product ID, normally CCDSP_VENDOR_ID
    int      mnProductID;       //!< USB enumerated product ID, normally CCDSP_PRODUCT_ID
    char     maDescription[64]; //!< textual description of enumerated device
    unsigned short  mnSerialNo; //!< serial number of found camera
    int      mbAvailable;       //!< Non-zero indicates camera is powered on/available.
} tsUSBData;

/** 
 *   This function will located any potential CCDsp cameras connected via a USB 2.0 compliant
 *   interface and return the number of devices found.  Users should check the
 *   Vendor and Product IDs to match the CDDSP_VENDOR_ID and CCDSP_PRODUCT_ID
 *   codes to ensure that they are connected to a CCDSP device.  
 *   该功能将位于通过USB2.0兼容接口连接任何潜在CCDsp相机和返回找到设备的数量。
 *   用户应检查供应商和产品标识相匹配的CDDSP_VENDOR_ID和CCDSP_PRODUCT_ID守则，
 *   以确保它们被连接到一个CCDSP设备。
 *   \param[out] apData pointer to user allocated structure to hold the results
 * 
 *   \param[in]  anNumBuffers number of buffers allocated structure may hold
 *
 *   \return -1 on error or number of cameras located (zero being a valid output)
 *
 */
DECLSPEC int __cdecl CLLocateHSUSBCameras(tsUSBData* apData, int anNumBuffers);

/**
 *  This function will enable or disable an attached FAN on the camera.  
 *  By default the FAN will be enabled on power up.
 *  该功能将启用或在camera.By禁用附加风扇默认风扇将上电时启用。
 *  \note this feature will only work with cameras with controllable FAN module.
 *
 *  \param[in] CameraHandle A valid CLCamera Handle
 *  \param[in] anEnable when non-zero, FAN will be enabled.
 *
 *  \return -1 on error
 */
DECLSPEC int __cdecl CLEnableFan(int CameraHandle, int anEnable);

/**
 *  This function configures the desired CCD temperature set point that should
 *  be tracked by the camera while cooled.  This function is inteded for 
 *  factory calibration, the setpoint is stored in non-volatile memory on the
 *  CCD and does not require adjustment by user applications.
 *  该函数配置应该由相机而冷却被跟踪所需的CCD的温度设定点。此功能是inteded工厂校准，
 *  设定点存储在非易失性存储器上的CCD，并且不需要由用户应用程序的调整。
 *  \warning User Applications should *not* call this function.
 *
 *  \param[in] CameraHandle A valid CLCamera Handle
 *  \param[in] anSetPointDegC Desired CCD temperature setpoint.
 *
 *  \return -1 on error.
 */
DECLSPEC int __cdecl CLSetCCDTargetTemp(int CameraHandle, float anSetPointDegC);

/**
 *  This function reads back the CCD temperature setpoint set by CLSetCCDTemp.
 *  该函数读取回CCD的温度设定值由CLtCC温度设定。
 *  \param[in] CameraHandle A valid CLCamera Handle
 *  \param[in] apSetPointDegC Location to store CCD temperature setpoint.
 *
 *  \return -1 on error.
 */
DECLSPEC int __cdecl CLGetCCDTargetTemp(int CameraHandle, float* apSetPointDegC);

/**
 *   This function sets a binning mode sequence for CLReadCCDBinned calls.  The 
 *   CLSetBinParam and CLSetHorBinParam methods provide a way to configure 
 *   up to 8 vertical binning and up to 8 horizontal binning schemes for use 
 *   during binned reads.
 *   该函数为CLReadCCDBinned呼叫的分级模式序列。
 *   该CLSetBinParam和CLSetHorBinParam方法提供了一种配置多达8个
 *   垂直分级和多达8个水平像素合并方案在使用过程中的离散化的读取。
 *   This routine allows configuring the CCDsp to cycle through various combinations
 *   of the vertical and horizontal binning patterns for a multi-image 
 *   acquisition (e.g., calls to CLReadCCDBinned with NumImages > 1).
 *   这个例程允许通过用于多图像采集的垂直和水平像素合并模式的各种组合配置CCDsp循环
    （例如，调用与NumImages>1至CLReadCCDBinned）。
 *   \param[in] CameraHandle A valid CLCamera Handle
 *   \param[in] apVerticalSeq pointer to 1xN array of vertical binning set indices (0 to 7)
 *   \param[in] apHorizSeq pointer to 1xN array of horizontal binning set indices (0 to 7)
 *   \param[in] anN length of repeating sequence (maximum 16 sequences).
 *   
 *   \return non-zero on error.
 */
DECLSPEC int __cdecl CLSetBinningSequence(int CameraHandle, unsigned char* apVerticalSeq,
                                          unsigned char* apHorizSeq, unsigned char anN);

/**
 *   This function gets the binning mode sequence for CLReadCCDBinned calls.  The 
 *   CLSetBinParam and CLSetHorBinParam methods provide a way to configure 
 *   up to 8 vertical binning and up to 8 horizontal binning schemes for use 
 *   during binned reads.
 *   这个函数获取了CLReadCCDBinned呼叫的分级模式序列。
	 该CLSetBinParam和CLSetHorBinParam方法提供了一种配置多达8个
	 垂直分级和多达8个水平像素合并方案在使用过程中的离散化的读取。
 *   This routine allows configuring the CCDsp to cycle through various combinations
 *   of the vertical and horizontal binning patterns for a multi-image 
 *   acquisition (e.g., calls to CLReadCCDBinned with NumImages > 1).
 *
 *   \param[in] CameraHandle A valid CLCamera Handle
 *   \param[in] apVerticalSeq pointer to 1x16 array of vertical binning set indices (0 to 7)
 *   \param[in] apHorizSeq pointer to 1x16 array of horizontal binning set indices (0 to 7)
 *   \param[out] apN pointer to char the length of repeating sequence (maximum 16 sequences).
 *   
 *   \return non-zero on error.
 */
DECLSPEC int __cdecl CLGetBinningSequence(int CameraHandle, unsigned char* apVerticalSeq,
                                          unsigned char* apHorizSeq, unsigned char* apN);

/**
 *   This function is used to configure image mirroring on the attached CCD camera.  It 
 *   is useful for presenting images to an operator in reverse readout order so that 
 *   an image may be "flipped" along the horizontal or vertical axis.  This allows 
 *   readouts to be more intuitive to a user (e.g., spectrum data aligned from low frequency
 *   to high rather than high to low based on optics setup, etc.).
 *   此功能用于配置图像镜像所附的CCD相机。它是用于使沿水平或垂直方向的图像可以被“翻转”
     以相反的读出顺序将图像呈现给操作者是有用的。这使得读出更直观的用户
	（例如，从低频率对准以高，而不是高到低基于光学设置，等等光谱数据）。
 *   Image data is *always* mirrored along the specified axis following any specified 
 *   binning operations (CCD or DSP).  Binning must be configured relative to true 
 *   CCD orientation.  Mirroring is intended for convenience of readout display orientation, 
 *   not configuration of binning.
 *   图像数据是*始终*沿着以下任意指定的装箱操作（CCD或DSP）指定轴镜像。
     分级必须相对于真正的CCD方向的配置。镜像是用于读取显示方向，分级的不是配置的便利性。
 *   For area reads the start row and column specified in CLSetCCDArea commands are
 *   with respect to the full area mirrored image.  The intent is to provide sub-area readouts
 *   that are consistent with full area readouts in pixel image buffers when mirrored.  
 *   In area readout, image data is *always* mirrored along the centerline of the CCD axis.
 *   对于区域读取CLSetCCDArea命令指定的开始行和列
	 相对于整个区域的镜像图像。的目的是提供子区域读出时mirrored.
	 In区读出的图象数据是*总是*沿CCD轴的中心线镜像是与像素图像缓冲器充满区域的读数一致。
 *   For binned readouts the resulting binned image is mirrored along the center of the 
 *   resulting image.  For example, if a binning configuration results in 5 rows of 
 *   512 columns and both axis are configured for mirroring, then the result returned will be 
 *   5 rows (mirrored along row 3) and 512 columns (mirrored along columns 128/129).   The
 *   binned image will still be packed into the bottom 5 rows and "left most" 512 columns in the
 *   received image buffer.
 *
 *   \param[in] CameraHandle A valid CLCamera Handle
 *   \param[in] anMirrorHorizontal when non-zero mirroring should be applied along horizontal dimension (CCD columns)
 *   \param[in] anMirrorVertical when non-zero mirroring should be applied along vertical dimension (CCD rows)
 *
 *   \return non-zero when an error condition is detected
 */
DECLSPEC int __cdecl CLSetCCDImageMirroring(int CameraHandle, 
                                            unsigned int anMirrorHorizontal,
                                            unsigned int anMirrorVertical);

/**
 *   Gets the current mirroring state of the camera.  See CLSetCCDImageMirroring for more detail.
 *   获取相机的当前镜像状态。见CLSetCCDImageMirroring了解更多详情。
 *   \param[in]  CameraHandle A valid CLCamera Handle
 *   \param[out] apMirrorHorizontal pointer to location to store horizontal configuration
 *   \param[out] apMirrorVertical   pointer to location to store vertical configuration
 *
 *   \return non-zdero when an error condition is detected
 */
DECLSPEC int __cdecl CLGetCCDImageMirroring(int CameraHandle,
                                            unsigned int* apMirrorHorizontal,
                                            unsigned int* apMirrorVertical);

/**
 *  Forces the camera shutter to remain open or closed until disabled.
 *  \note you must call this command with Override = 0 to enable normal
 *        shutter operation.
 *  强制相机快门保持打开或关闭，直到disabled.note则必须调用此命令覆盖=0，使正常的快门操作。
 *  \param[in] CameraHandle A valid CLCamera Handle
 *  \param[in] Override  When non-zero override normal shutter operation
 *  \param[in] ForceOpen Force shutter open when non-zero, force shutter closed when 0
 *
 *  \return non-zero when an error condition is detected
 */
DECLSPEC int __cdecl CLSetShutterOverride(int CameraHandle, int Override, int ForceOpen);


/**
 *   Override the image timeout value when issuing CCDSP image requests.  
 *   发行CCDSP图像请求时覆盖图像超时值。
 *   This function is only required if you are working with a custom CCDSP build that integrates images
 *   in the camera and does not generate the same number of images as you request (e.g.,
 *   if you're camera has a co-adding capability built in the MityDSP). 
 *   如果您正在使用一个自定义CCDSP构建，在摄像机的图像集成，并为您请求（例如不会产生相同数量的图像，
	 如果你的相机工作该功能时，才需要有一个建于MityDSP共同加入功能）。
 *   Normally, the interface will time out if it hasn't received an image within 20 seconds following
 *   the configured exposure time.  If you are co-adding many images in the camera, then this time may 
 *   not be sufficient.  This method allows you to override it based on your application's needs.
 *   正常情况下，该接口将超时，如果它尚未以下配置的曝光时间在20秒内接收到的图像。
 *	 如果您在相机共增加了许多图像，那么这一次可能是不够的。此方法允许您根据应用程序的需求来覆盖它。
 *   \param[in] CameraHandle A valid Camera Handle
 *   \param[in] Timeout_Ms Time to wait in milliseconds, when 0 used default value.  When < 0, wait indefinately.
 *      
 *   \return non-zero when an error condition is detected
 */
DECLSPEC int __cdecl CLOverrideImageTimout(int CameraHandle, int Timeout_Ms);

/**
 *   This routine loads a new CCD timing file generated by the timing generator
 *   application to the attached CCDsp camera.  
 *   这个例程加载由定时发生器应用到连接的CCD摄像机产生一个新的CCD定时文件。
 *   \warning this routine is intended for field level support of CCD timing
 *            performance.  Failure to use the correct timing files for a given
 *            camera can permanently damage a CCDsp camera system.
 *
 *   \param[in] CameraHandle a valid Camera Handle
 *   \param[in] TimingData Pointer to a structure containing the timing information
 *   \param[in] TimingDataSize Size of the data structure
 *
 *   \return non-zero when an error condition is detected
 */
DECLSPEC int __cdecl CLSetTimingData(int CameraHandle, void* TimingData, int TimingDataSize);

/**
 *   This routine extracts information regarding the timing of the CCD stored
 *   on the attached CCDsp camera.
 *   关于存储在连接的CCD照相机CCD上的定时该程序中提取信息。
 *   \param[in] CameraHandle a valid camera handle
 *   \param[in] TimingData Pointer to a structure to store the information
 *   \param[in,out] TimingDataSize
 *
 *   \return non-zero when an error condition is detected
 */
DECLSPEC int __cdecl CLGetTimingData(int CameraHandle, void* TimingData, int* TimingDataSize);

/**
 *   This routine sets the state of the 4 GPIO pins
 *   这个程序设置了4个GPIO引脚的状态
 *   
 *   \param[in] CameraHandle a valid camera handle
 *   \param[in] PinNumber number of the GPIO pin to be set.  Valid PinNumbers are 0 through 3.
 *   \param[in] PinState enum describing state of pin
 * 
 *   Valid states include:
 *      eeINPUT, eeOUTLOW, eeOUTHIGH, eeOUTTIMGEN0, eeOUTTIMGEN1, eeOUTTIMGEN2
 *
 *   \return non-zero when an error condition is detected
 */
DECLSPEC int __cdecl CLSetGPIO(int CameraHandle, int PinNumber, tePinState PinState);

/**
 *   This routine sets GPIO pin to use as exposure trigger
 *   该程序集GPIO引脚要曝光触发使用
 *   \param[in] CameraHandle a valid camera handle
 *   \param[in] PinNumber number of the GPIO pin to be set;  Range [0-3], -1 = no trigger 
 * 
 *   \return non-zero when an error condition is detected
 */
DECLSPEC int __cdecl CLSetTrigger(int CameraHandle, int PinNumber);

/**
 *   This routine sets GPIO pin to use as exposure trigger
 *   该程序集GPIO引脚要曝光触发使用
 *   \param[in]  CameraHandle a valid camera handle
 *   \param[out] PinNumber pointer to number of the GPIO that is set;  -1 = no trigger
 * 
 *   \return non-zero when an error condition is detected
 */
DECLSPEC int __cdecl CLGetTrigger(int CameraHandle, int* PinNumber);

/**
 *   This routine configures the external trigger mode.
 *   这个程序配置外部触发模式。
 *   \param[in] CameraHandle a valid camera handle
 *   \param[in] TriggerMode, may be eeOnePerImage(default) or eeOnePerGroup 
 *   \param[in] DelayMs number of milliseconds to delay from rising edge
 *              of trigger to start of frame capture cycle (default is zero)
 *   \param[in] FallingEdge, when 1 trigger declared on falling edge (default),
 *              when 0 trigger declared on rising edge
 *
 *   \return non-zero when an error condition is detected
 */
DECLSPEC int __cdecl CLSetTriggerMode(int CameraHandle, teExtTrigMode TriggerMode, int DelayMs, int FallingEdge);

/**
 *   This routine returns the current triggermode configuration (see CLSetTriggerMode
 *   for details.
 *   这个程序返回当前triggermode配置（见CLSetTriggerMode了解详细信息。
 *   \param[in] CameraHandler a valid camera handle
 *   \param[in] TriggerMode pointer to location to store current trigger mode
 *   \param[in] DelayMs pointer to location to store DelayMs
 *   \param[in] FallingEdge, when 1 trigger on falling edge, when 0 on rising edge
 *
 *   \return non-zero when an error condition is detected
 */
DECLSPEC int __cdecl CLGetTriggerMode(int CameraHandle, teExtTrigMode* TriggerMode, int* DelayMs, int* FallingEdge);

/**
 *    This routine will enable auto re-arming of the camera trigger.  This feature
 *    is intended for use with continuous captures of eeOnePerGroup external 
 *    triggered data.  When this feature is set, the camera will capture the requested
 *    number of images from CLReadCCDBinned or CLReadCCDArea upon detection of
 *    the trigger condition specified by the CLSetTrigger and CLSetTriggerMode
 *    API calls.  Immediately after the capture, the camera will re-execute the
 *    the last requested CLReadCCDBinned or CLReadCCDArea command, effectively 
 *    re-arming the trigger.
 *    这个程序将允许相机触发自动重新布防。此功能是为了用于eeOnePerGroup外部的连续捕捉触发的数据。
	  当此功能设置，相机会在检测到由CLSetTrigger和CLSetTriggerMode API
	  调用指定的触发条件捕获CLReadCCDBinned或CLReadCCDArea图像的请求数量。
	  捕获后，立即相机会重新执行最后一个请求CLReadCCDBinned或CLReadCCDArea命令，有效地重新武装触发。
 *    When this feature is enabled, the camera imaging cycle runs continouosly 
 *    until a CLAbortRead is called.
 *    如果启用此功能，直到CLAbortRead被称为相机的成像周期连续运行。
 *    \param[in] CameraHandle a valid camera handle
 *    \param[in] ReArm when non-zero, automatic re-arming will be performed.
 *    
 *    \return non-zero when an error condition is detected.
 */
DECLSPEC int __cdecl CLSetAutoTriggerReArm(int CameraHandle, int ReArm);

/**
 *   This routine returns the current AutoTriggerReArm configuration (see 
 *   CLSetAutoTriggerReArm for details).
 *   这个程序返回当前AutoTriggerReArm配置（见CLSetAutoTriggerReArm详情）。
 *   \param[in] CameraHandle a valid camera handle
 *   \param[in] ReArm pointer to location to store current re-arm mode
 *
 *   \return non-zero when an error condition is detected
 */
DECLSPEC int __cdecl CLGetAutoTriggerReArm(int CameraHandle, int* ReArm);

/**
 *   This routine gets the current state of the 4 GPIO pins
 *   该例程是4 GPIO引脚的当前状态
 *   \param[in] CameraHandle a valid camera handle
 *
 *   \return value reflecting the 4 GPIO pins in low 4 bits
 */
DECLSPEC int __cdecl CLGetGPIO(int CameraHandle);

/** \defgroup Test Pattern Types 
 *
 * \{
 */
#define PATTERNTYPE_NONE         SIMTYPE_NONE        //!< use real CCD data
#define PATTERNTYPE_DC           SIMTYPE_DC          //!< each read pixel from camera is replaced with DC value in mnSimParams[0]
#define PATTERNTYPE_RAMP         SIMTYPE_RAMP        //!< pixels are incremented by one starting at zero
#define PATTERNTYPE_GRADIENT_X   SIMTYPE_GRADIENT_X  //!< pixels are scaled from PatternParams[0] to PatternParams[1] by PatternParams[2] in X dimension
#define PATTERNTYPE_GRADIENT_Y   SIMTYPE_GRADIENT_Y  //!< pixels are scaled from PatternParams[0] to PatternParams[1] by PatternParams[2] in Y dimension
#define PATTERNTYPE_CROSSHAIR    SIMTYPE_CROSSHAIR   //!< two values, cross hairs DC value in PatternParams[0], value 2 in PatternParams[1], every [2] in X, [3] in Y
#define PATTERNTYPE_RANDOM       SIMTYPE_RANDOM      //!< pixesl mean is PatternParams[0] with std of 0.1 * PatternParams[1]
/** \} */

/**
 *  This routine configures the attached CCDsp camera to override data clocked out 
 *  by the CCD with test pattern data.
 *  这个程序配置连接的CCD摄像头覆盖的数据与测试图形数据CCD同步输出。
 *  \param[in] CameraHandle a valid camera handle
 *  \param[in] pattern the test pattern to generate:
 *             - PATTERNTYPE_NONE
 *             - PATTERNTYPE_DC
 *             - PATTERNTYPE_RAMP
 *             - PATTERNTYPE_GRADIENT_X
 *             - PATTERNTYPE_GRADIENT_Y
 *             - PATTERNTYPE_CROSSHAIR
 *             - PATTERNTYPE_RANDOM
 *  \param[in] PatternParams parameters needed for the specified test pattern
 *  \param[in] ParamSize number of valid parameters in the PatternParams Array
 *
 *  \return non-zero on error.
 */
DECLSPEC int __cdecl CLSetTestPattern(int CameraHandle, int pattern, unsigned short* PatternParams, int ParamSize);

/**
 *  Get the gain configuration of the camera.
 *  获得摄像机的增益配置。
 *  \param[in]  CameraHandle a valid camera handle
 *  \param[out] NumSettings pointer to integer to hold number of possible settings
 *  \param[out] Gains Array of length MAX_GAIN_SETTINGS of gain values (electrons per count)
 *  \param[out] CurrentSetting point to integer to hold current selected gain index setting
 *
 *  \return non-zero on error.
 */
DECLSPEC int __cdecl CLGetGainConfig(int CameraHandle, int* NumSettings, float* Gains, int* CurrentSetting);

/**
 *  Set the gain configuration of the camera.
 *  设定照相机的增益配置。
 *  \param[in]  CameraHandle a valid camera handle
 *  \param[out] Setting to use, range of 0 to (NumSettings-1), where NumSettings is obtained by CLGetGainConfig
 *
 *  \return non-zero on error.
 */
DECLSPEC int __cdecl CLSetGain(int CameraHandle, int Setting);

typedef enum
{
    eeRollingShutter = 0,
    eeGlobalShutter  = 1
} teCISShutterMode;

/**
 *  Set CIS camera shutter mode.
 *  设置CIS相机快门模式。
 *  \param[in] CameraHandle a valid camera handle.
 *  \param[in] Mode Valid shutter mode, either eeRollingShutter or eeGlobalShutter
 *
 *  \return non-zero on error.
 */
DECLSPEC int __cdecl CLCISSetShutterMode(int CameraHandle, teCISShutterMode Mode);

/** 
 *  Request CIS camera Gain and Offset Calibration.
 *  请求CIS相机增益和偏移校准。 
 *  This routine should be called while the camera is under a dark environment.
 *  Results of the calibration are stored in non-volatile memory and will
 *  be used on subsequent operations.  Users should not typically need to 
 *  call this routine as the units are calibrated at factory.
 *
 *  \return non-zero on error.
 */
DECLSPEC int __cdecl CLCISCalibrate(int CameraHandle);

/** 
 *  Request CIS Camera (re)load calibration data from FLASH.
 *  请求CIS相机（从FLASH重装校准数据。
 *  Normally, it only makes sense to call this function on a CIS camera that
 *  has had it's calibration cleared with CLCISClearCalibration().
 *  通常情况下，它才有意义调用一个CIS摄像头，已经有（）标定与CLCISClearCalibration清除此功能。
 *  \return non-zero on error.
 */
DECLSPEC int __cdecl CLCISLoadCalibration(int CameraHandle);

/** 
 *  Remove active calibration data from CIS processing.
 *  独联体处理删除活跃校准数据。
 *  Call this do un-apply gain and bias calibration settings from the 
 *  CIS camera in order to receive RAW sensor data.  Call CLCISLoadCalibration
 *  to 
 *  调用该做的，以获得原始传感器数据从CIS相机未适用的增益和偏移校准设置。调用类加载器校准
 *  \return non-zero on error.
 */
DECLSPEC int __cdecl CLCISClearCalibration(int CameraHandle);

#define CIS_CALSTATE_UNKNOWN    0
#define CIS_CALSTATE_LOADED     1
#define CIS_CALSTATE_CLEARED    2

/**
 *  Request current calibration state of a CIS camera.
 *  要求CIS相机的当前校准状态。 
 *  Call this to determine current calibration state of the camera.
 *  调用此决定照相机的当前校准状态。
 *  \param State calibration state of the camera, one of CIS_CALSTATE_UNKNOWN,
 *         CIS_CALSTATED_CLEARED, CIS_CALSTATE_LOADED
 * 
 *  \return non-zero on error.
 */
DECLSPEC int __cdecl CLCISCalibrationState(int CameraHandle, int* State);

typedef enum
{
    eeCombined = 0,
    eeLowGain  = 1,
    eeHighGain = 2
} teCISChannelConfig;

/**
 *  Configure CIS camera channel selection for subsequent imaging.
 *  配置为后续成像相机CISCO通道选择。
 *  \param[in] CameraHandle a valid camera handle.
 *  \param[in] Config must be eeCombined, eeLowGain, or eeHighGain
 *
 *  \return non-zero on error.
 */
DECLSPEC int __cdecl CLCISSelectChannel(int CameraHandle, teCISChannelConfig Config);

typedef struct
{
    unsigned int mnEnable;      //!< set to non-zero to enable recording
    enum {
        eeTIFF = 0,
        eePNG  = 1,
        eeJPG  = 2,
        eeRAW  = 3
    } meFormat;                 //!< image format on file
    unsigned int mnDecimation;  //!< number of frames to skip between recordings
    char         maRecDir[256]; //!< path to recording directory (remotely on camera)
} tsRecordParams;

/**
 *   Configure camera image recording.
 *   配置摄像头图像记录。
 *   This will initiate remote image recording for cameras that support remote storage
 *   such as the CIS camera.
 *   这将启动远程图像记录为支持远程存储设备，如独联体相机摄像机。
 *   \param[in] CameraHandle a valid camera handle.
 *   \param[in] params a valid tsRecordParams structure
 *   \param[in] paramsize number of bytes in the tsRecordParams structure (e.g., sizeof(tsRecordParams))
 *
 *   \return non-zero on error.
 */
DECLSPEC int __cdecl CLSetRecording(int CameraHandle, tsRecordParams* params, int paramsize);

/**
 *   Obtain the currently configured multiple-ROI state. This can also be used
 *   to determine the maximum number of windows the camera supports. When 
 *   numROI returns 1, the camera's ROI should be fetched with CLGetCCDArea.
 *   获得当前配置的多ROI状态。这也可以用于确定相机支持窗口的最大数量。
 *	 当numROI返回1，摄像头的投资回报率应该是获取与CLGetCCDArea。
 *   This is a blocking call.
 *   这是一个阻塞调用。
 *   \param[in] CameraHandle a valid camera handle.
 *   \param[out] maxROI pointer to store the maximum number of ROIs the camera supports
 *   \param[out] numROI pointer to store the number of ROIs the camera is configured for
 *   \param[out] RowStart pointer to array of size MAX_NUM_MULTIPLE_ROI for each ROI start row
 *   \param[out] ColStart pointer to array of size MAX_NUM_MULTIPLE_ROI for each ROI start column
 *   \param[out] NRows pointer to array of size MAX_NUM_MULTIPLE_ROI for each ROI height
 *   \param[out] NCols pointer to array of size MAX_NUM_MULTIPLE_ROI for each ROI width
 *
 *   \return non-zero on error.
 */
DECLSPEC int __cdecl CLGetAreaEx(int CameraHandle, int *maxROI, int *numROI, int *RowStart, int *ColStart, int *NRows, int *NCols);

/**
 *   Set a multiple-ROI configuration. To move into Single ROI mode, numROI 
 *   should be set to 1. When in single ROI mode, use CLSetCCDArea to change
 *   the ROI. When setting numROI to greater than 1, the camera enters multiple
 *   ROI mode. Some cameras may have specific requirements dictating limits on
 *   the window sizes.
 *   设置多的ROI配置。移动到单ROI模式，numROI应设置为1。
 *	 当在单一的ROI模式，使用CLSetCCDArea改变投资回报率。
 *	 当设置numROI为大于1时，摄像机进入多个ROI模式。一些照相机可以具有特定的要求口述的窗口大小的限制。
 *   This is a non-blocking call.
 *   这是一个非阻挡调用。
 *   \param[in] CameraHandle a valid camera handle.
 *   \param[in] maxROI pointer to store the maximum number of ROIs the camera supports
 *   \param[in] numROI pointer to store the number of ROIs the camera is configured for
 *   \param[in] RowStart pointer to array of size MAX_NUM_MULTIPLE_ROI for each ROI start row
 *   \param[in] ColStart pointer to array of size MAX_NUM_MULTIPLE_ROI for each ROI start column
 *   \param[in] NRows pointer to array of size MAX_NUM_MULTIPLE_ROI for each ROI height
 *
 *   \return non-zero on error.
 */
DECLSPEC int __cdecl CLSetAreaEx(int CameraHandle, int numROI, int *RowStart, int *ColStart, int *NRows, int *NCols);

/**
 *   Configure a vision camera for uniform binning.  This method supports configuring standard
 *   uniform binning patterns (e.g., 1x1, 2x2, 4x4, 1x4, etc.) on MityCAM vision cameras.  Check
 *   the User's Guide / Data Sheet for your specific camera to see of this command is supported.
 *   Most CCD cameras require the use of CLSetBinParam() instead of this call.
 *   配置视摄像头进行统一分级。此方法支持配置上MityCAM视觉相机标准统一的分级模式（例如，1×1，2×2，4×4，×4等）。
 *   检查用户手册/数据表为特定的相机，看看这个命令是supported.Most CCD摄像机需要使用CLSetBinParam的（）而不是此电话。 
 *   \param[in] CameraHandle a valid camera handle.
 *   \param[in] VertBinFactor Number of rows to bin in vertical direction.
 *   \param[in] HorBinFactor Number of columns to bin in horizontal direction.
 *   \param[in] VertScale set to 1.0
 *   \param[in] HorScale set to 1.0
 *
 *   \return non-zero on error.
 */
DECLSPEC int __cdecl CLSetSimpleBinning(int CameraHandle, int VertBinFactor, int HorBinFactor,
	                                    float VertScale, float HorScale);

/**
 *   Configure a vision camera for uniform binning.  This method supports configuring standard
 *   uniform binning patterns (e.g., 1x1, 2x2, 4x4, 1x4, etc.) on MityCAM vision cameras.  Check
 *   the User's Guide / Data Sheet for your specific camera to see of this command is supported.
 *   Most CCD cameras require the use of CLSetBinParam() instead of this call.
 *	 配置视摄像头进行统一分级。此方法支持配置上MityCAM视觉相机标准统一的分级模式（例如，1×1，2×2，4×4，×4等）。
 *   检查用户手册/数据表为特定的相机，看看这个命令的支持。大多数CCD相机需要，而不是这个调用使用CLSetBinParam（的）。
 *   \param[in] CameraHandle a valid camera handle.
 *   \param[in] VertBinFactor Pointer to location to store number of rows to bin in vertical direction.
 *   \param[in] HorBinFactor Pointer to location to store number of columns to bin in horizontal direction.
 *   \param[in] VertScale Pointer to location to store vertical scale parameter
 *   \param[in] HorScale Pointer to location to store horizontal scale parameter
 *
 *   \return non-zero on error.
 */
DECLSPEC int __cdecl CLGetSimpleBinning(int CameraHandle, int *VertBinFactor, int *HorBinFactor,
	                                    float *VertScale, float *HorScale);


/**
 *   Retreive a camera specific parameter.
 *   检索相机特定参数。
 *   \param[in] CameraHandle a valid camera handle.
 *   \param[in] A valid parameter ID.  See camera specifications.
 *   \param[in] ParamBuf buffer to store the parameter.
 *   \param[in] ParamBufSize of the buffer.
 *
 *   \return non-zero on error.
 */
DECLSPEC int __cdecl CLGetParameter(int CameraHandle, unsigned int ParamID, void* ParamBuf, int ParamBufSize);

/**
 *   Set a camera specific parameter.
 *   设置一个摄像头特定参数。
 *   \param[in] CameraHandle a valid camera handle.
 *   \param[in] A valid parameter ID.  See camera specifications.
 *   \param[in] ParamBuf buffer to store the parameter.
 *   \param[in] ParamBufSize of the buffer.
 *
 *   \return non-zero on error.  Note: if the parameter ID was illegal or the wrong size, this routine will 
 *   still return a "success".  However, the camera will generate an error message that can be seen on the
 *   camera error callback mechanism.
 */
DECLSPEC int __cdecl CLSetParameter(int CameraHandle, unsigned int ParamID, void* ParamBuf, int ParamBufSize);

#ifdef __cplusplus
}
#endif


#endif

