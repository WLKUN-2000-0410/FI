#include "toucam_zl.h"
#include "FTD2XX.H"
float m_fExpTime;
bool bRun=false;

zl_toucampro::zl_toucampro()
{

}
zl_toucampro::~zl_toucampro()
{

}
bool  zl_toucampro::SetDllPath(char* buf)
{
	if (!m_proc.LoadFunctionFromlibray(buf)){
		printf("3");
		return false;
	}
	return true;
}
bool zl_toucampro::Open(PVOID m_NameNmbr)
{
	FT_STATUS ftStatus;
	ftStatus = m_proc.m_pOpenEx((PVOID)(LPCTSTR)m_NameNmbr,FT_OPEN_BY_SERIAL_NUMBER,&m_ftHandle);

	ftStatus =m_proc.m_pSetBaudRate(m_ftHandle,115200);
	ftStatus =m_proc.m_pSetDataCharacteristics(m_ftHandle,FT_BITS_8,FT_STOP_BITS_1,FT_PARITY_NONE);
	ftStatus =m_proc.m_pSetFlowControl(m_ftHandle,FT_FLOW_NONE, 0x11, 0x13);
	if (ftStatus==FT_OK)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool zl_toucampro::ListDevices(PVOID pArg1, DWORD &pArg2)
{
	FT_STATUS ftStatus;
	DWORD numDevs;

	m_proc.m_pClose(m_ftHandle);//must be closed to perform the ListDevices() function

	ftStatus = m_proc.m_pListDevices(&numDevs, NULL, FT_LIST_NUMBER_ONLY);

	if(numDevs>0)
	{
		//AfxMessageBox("by serial"); 
		ftStatus = m_proc.m_pListDevices(&numDevs, NULL, FT_LIST_NUMBER_ONLY);
		if(ftStatus == FT_OK) 
		{

			ftStatus = m_proc.m_pListDevices(pArg1, &numDevs, FT_LIST_ALL|FT_OPEN_BY_SERIAL_NUMBER);
			
		}
	}
	if (ftStatus==FT_OK)
	{
		pArg2=numDevs;
		return true;
	}
	else
	{
		return false;
	}
}
bool zl_toucampro::Close()
{
	return m_proc.m_pClose(m_ftHandle);
}
bool zl_toucampro::Read(LPVOID lpvBuffer , DWORD dwBuffSize, LPDWORD lpdwBytesRead)
{
	FT_STATUS ftStatus;
	//m_proc.m_pPurge(m_ftHandle,FT_PURGE_RX || FT_PURGE_TX); //clear both TX and rx buffers before send and receive
	ftStatus=m_proc.m_pRead(m_ftHandle,lpvBuffer,dwBuffSize,lpdwBytesRead);
	//Sleep(10);
	if (ftStatus==FT_OK)
	{
		return true;
	}
	else
	{
		return false;
	}

}
bool zl_toucampro::Write(LPVOID lpvBuffer, DWORD dwBuffSize, LPDWORD lpdwBytesRead)
{
	FT_STATUS ftStatus;
	m_proc.m_pPurge(m_ftHandle,FT_PURGE_RX || FT_PURGE_TX); //clear both TX and rx buffers before send and receive
	ftStatus=m_proc.m_pWrite(m_ftHandle,lpvBuffer,dwBuffSize,lpdwBytesRead);
	//Sleep(10);
	if (ftStatus==FT_OK)
	{
		return true;
	}
	else
	{
		return false;
	}
}
bool zl_toucampro::ResetDevice()
{
	FT_STATUS ftStatus;

	ftStatus=m_proc.m_pResetDevice(m_ftHandle);
	if (ftStatus==FT_OK)
	{
		return true;
	}
	else
	{
		return false;
	}
}
bool zl_toucampro::Purge()
{
	FT_STATUS ftStatus;

	ftStatus=m_proc.m_pPurge(m_ftHandle,FT_PURGE_RX || FT_PURGE_TX);
	if (ftStatus==FT_OK)
	{
		return true;
	}
	else
	{
		return false;
	}
}
bool zl_toucampro::SetTimeouts(ULONG dwReadTimeout, ULONG dwWriteTimeout)
{
	FT_STATUS ftStatus;

	ftStatus=m_proc.m_pSetTimeouts(m_ftHandle,dwReadTimeout , dwWriteTimeout);
	if (ftStatus==FT_OK)
	{
		return true;
	}
	else
	{
		return false;
	}
}

BYTE zl_toucampro::WriteReg(BYTE*nbuf,int num)
{
	DWORD ret_bytes;
	unsigned char txbuf[25]={0}, rxbuf[25]={0};
	txbuf[0]=0x53;
	txbuf[1]=0xe0;
	if (num==1){
		txbuf[2]=0x01;}
	else{
		txbuf[2]=0x02;}

	for(int i=0;i<num;i++){

		txbuf[i+3]=nbuf[i];
	}
	txbuf[3+num]=0x50;
	txbuf[4+num]=chkSumInt(txbuf,num+4);
	Purge();
	Write(txbuf, num+5, &ret_bytes);
	SetTimeouts(20, 0);
	Read(rxbuf, 1,&ret_bytes);
	return rxbuf[0];
}
BYTE zl_toucampro::ReadReg()
{
	DWORD ret_bytes;
	unsigned char txbuf[25]={0}, rxbuf[25]={0};
	txbuf[0]=0x53;
	txbuf[1]=0xe1;
	txbuf[2]=0x01;
	txbuf[3]=0x50;
	txbuf[4]=chkSumInt(txbuf,4);
	Purge();
	Write(txbuf, 5, &ret_bytes);
	SetTimeouts(20, 0);
	Read(rxbuf, 1,&ret_bytes);
	return rxbuf[0];
}
BYTE zl_toucampro::chkSumInt(BYTE*nbuf,int num)
{
	BYTE txbuf=0;
	for(int i=0;i<num;i++)
	{
		txbuf=txbuf^nbuf[i];
	}
	return txbuf;
}
int zl_toucampro::GetXSize()
{
	unsigned char txbuf[25]={0xb4}, rxbuf[25]={0};
	WriteReg(txbuf,1);
	DWORD dData;
	dData=ReadReg()&0xffff;

	txbuf[0]=0xb5;
	WriteReg(txbuf,1);
	dData=(dData<<8)|(ReadReg()&0xfffff);

	txbuf[0]=0xa1;
	WriteReg(txbuf,1);
	rxbuf[0]=ReadReg();

	dData= dData / (rxbuf[0] + 1);
	return dData;
}
int zl_toucampro::GetYSize()
{
	unsigned char txbuf[25]={0xb8}, rxbuf[25]={0};
	WriteReg(txbuf,1);
	DWORD dData;
	dData=ReadReg()&0xffff;

	txbuf[0]=0xb9;
	WriteReg(txbuf,1);
	dData=(dData<<8)|(ReadReg()&0xfffff);

	txbuf[0]=0xa2;
	WriteReg(txbuf,1);
	rxbuf[0]=ReadReg();

	if((rxbuf[0] &1<<7)!=0)
		dData =1;
	else
		dData= dData / (rxbuf[0] + 1);
	return dData;

}
void zl_toucampro::GetInit(int nMode)
{
	unsigned char txbuf[25]={0}, rxbuf[25]={0},fbuf[5]={0};
	DWORD ret_bytes;



	txbuf[0]=0x4f;

	if (nMode==0)
	{
		txbuf[1]=0x53;
	}
	else
	{
		txbuf[1]=0x52;
	}
	txbuf[2]=0x50;
	txbuf[3]=txbuf[0]^txbuf[1]^txbuf[2];

	Write(txbuf,4,&ret_bytes);

	for (int i=0;i<2;i++)
	{
		Read(rxbuf+i,1,&ret_bytes);
	}
}
void zl_toucampro::GetMfData()
{
	unsigned char txbuf[25]={0}, rxbuf[25]={0},fbuf[5]={0};
	DWORD ret_bytes;



	txbuf[0]=0x53;
	txbuf[1]=0xae;
	txbuf[2]=0x05;
	txbuf[3]=0x01;
	txbuf[4]=0x00;
	txbuf[5]=0x00;
	txbuf[6]=0x02;
	txbuf[7]=0x00;
	txbuf[8]=0x50;
	txbuf[9]=txbuf[0]^txbuf[1]^txbuf[2]^txbuf[3]^txbuf[4]^txbuf[5]^txbuf[6]^txbuf[7]^txbuf[8];

	Write(txbuf,10,&ret_bytes);

	Read(rxbuf,2,&ret_bytes);


	memset(txbuf,0,25); memset(rxbuf,0,25);
	txbuf[0]=0x53;
	txbuf[1]=0xaf;
	txbuf[2]=0x12;
	txbuf[3]=0x50;
	txbuf[4]=txbuf[0]^txbuf[1]^txbuf[2]^txbuf[3];
	Write(txbuf,5,&ret_bytes);
	Read(rxbuf,18,&ret_bytes);


	nNo=rxbuf[1]&0xff;
	nNo=(nNo<<8)|(rxbuf[0]&0xfffff);

	nDate[0]=rxbuf[2];
	nDate[1]=rxbuf[3];
	nDate[2]=rxbuf[4];

	memcpy(fbuf,rxbuf+5,5);

	//strChar.Format("%s", fbuf);

	nADC[0]=rxbuf[11]&0xff;
	nADC[0]=(nADC[0]<<8)|(rxbuf[10]&0xfffff);

	nADC[1]=rxbuf[13]&0xff;
	nADC[1]=(nADC[1]<<8)|(rxbuf[12]&0xfffff);

	nDAC[0]=rxbuf[15]&0xff;
	nDAC[0]=(nDAC[0]<<8)|(rxbuf[14]&0xfffff);

	nDAC[1]=rxbuf[17]&0xff;
	nDAC[1]=(nDAC[1]<<8)|(rxbuf[16]&0xfffff);

	TEC_ADC_gradient = (0 - 40) / (nADC[0]*1.0 - nADC[1]*1.0);
	TEC_ADC_intercept = 40 - TEC_ADC_gradient * (nADC[1]*1.0);

	TEC_DAC_gradient = (0 - 40) / (nDAC[0]*1.0 - nDAC[1]*1.0);
	TEC_DAC_intercept = 40 - TEC_DAC_gradient * (nDAC[1]*1.0);
}

float zl_toucampro::GetExpTime()
{
	unsigned char txbuf[25]={0}, rxbuf[25]={0};
	BYTE fbuf[5]={0xED,0xEE,0xEF,0xF0,0xF1};
	DWORD ret_bytes;
	long long  uret=0;

	for(int i=0;i<5;i++){


		txbuf[0]=0x53;
		txbuf[1]=0xe0;
		txbuf[2]=0x01;
		txbuf[3]=fbuf[i];
		txbuf[4]=0x50;
		txbuf[5]=txbuf[0]^txbuf[1]^txbuf[2]^txbuf[3]^txbuf[4];

		Write(txbuf,6,&ret_bytes);
		Read(rxbuf,6,&ret_bytes);


		memset(txbuf,0,25); memset(rxbuf,0,25);
		txbuf[0]=0x53;
		txbuf[1]=0xe1;
		txbuf[2]=0x01;
		txbuf[3]=0x50;
		txbuf[4]=txbuf[0]^txbuf[1]^txbuf[2]^txbuf[3];
		Write(txbuf,5,&ret_bytes);
		Read(rxbuf,5,&ret_bytes);

		uret=(uret<<8)+rxbuf[0];
	}
	return uret/4e7*1.0;//µ¥Î»s
}
void zl_toucampro::SetExpTime(float fExpTime)
{
	
	unsigned char txbuf[25]={0}, rxbuf[25]={0};
	BYTE fbuf[5]={0xED,0xEE,0xEF,0xF0,0xF1};
	DWORD ret_bytes;
	long long  uret=fExpTime*4e7;

	for(int i=0;i<5;i++){


		txbuf[0]=0x53;
		txbuf[1]=0xe0;
		txbuf[2]=0x02;
		txbuf[3]=fbuf[i];
		txbuf[4]=uret>>8*(4-i);
		txbuf[5]=0x50;
		txbuf[6]=txbuf[0]^txbuf[1]^txbuf[2]^txbuf[3]^txbuf[4]^txbuf[5];

		Write(txbuf,7,&ret_bytes);
		Read(rxbuf,7,&ret_bytes);


		memset(txbuf,0,25); memset(rxbuf,0,25);

	}
	m_fExpTime=fExpTime;

}

void zl_toucampro::SetGain(int nIndex)
{
	BYTE nData=0;
	WriteReg(&nData,1);

	
	nData=ReadReg();
	
	BYTE BUF[5]={0};
	BUF[0]=0x00;
	if (nIndex==0)
	{
		
		BUF[1]=nData&0x7f;
		WriteReg(BUF,2);
	}
	else
	{
		BUF[1]=nData|0x80;
		WriteReg(BUF,2);
	}
}

float zl_toucampro::GetTemp()
{	
	unsigned char txbuf[25]={0x6e,0x00}, rxbuf[25]={0};
	WriteReg(txbuf,1);
	DWORD dData;
	dData=ReadReg()&0xffff;
	
	
	txbuf[0]=0x6f;
	txbuf[1]=0x00;
	WriteReg(txbuf,1);
	dData=(dData<<8)|(ReadReg()&0xfffff);


	float dTemp=(1.0*dData * TEC_ADC_gradient) +TEC_ADC_intercept;
	//float dTemp=(dData * TEC_DAC_gradient) +TEC_DAC_intercept;

	return dTemp;
}
void  zl_toucampro::SetTemp(int nTemp)
{
	int _tecSetCnt=(nTemp-TEC_DAC_intercept)*1.0/TEC_DAC_gradient;
	BYTE BUF[5]={0};
	BUF[0]=0x03;
	BUF[1]=_tecSetCnt>>8;
	WriteReg(BUF,2);
	BUF[0]=0x04;
	BUF[1]=_tecSetCnt<<0;
	WriteReg(BUF,2);
}


unsigned int calcMaxDelayPerFrameMs(unsigned int exposureMs, unsigned int addedDelayMs,unsigned int nYSize)
{
	// local vars 
	double _vClkRateMs, _hClkRateMs, _usbTransferRateMs;
	double _tExposure, _tCleanHreg2, _tShiftHreg, _tReadHreg, _tCleanVert, _tUsbTransfer;
	unsigned int _maxDelayPerFrameMs;
	int XSize=2048,YSize=nYSize,XBin=0,YBin=0;

	// calculate max approx timeout delay 
	_vClkRateMs = (double)1.0*32/ 1000000 * 1000;            //row transfer rate,/1000000 convert to s, x1000 convert to ms
	_hClkRateMs = (double)(1.0 /(500 * 1000) * 1000*1.0);         //pixel rate = 500kHz, x1000 convert to Hz, x1000 convert to ms
	_usbTransferRateMs = (double)(1.0 / (8 * 1000000) * 1000*1.0); //usb to parallel fifo readout rate = 8MHz, x1000000 convert to Hz, x1000 convert to ms
	_tExposure = (exposureMs);                                                                   //exposure time
	_tCleanHreg2 = (_hClkRateMs * 3084);                      //time to clean hreg before shifting rows into hreg after exposure
	_tShiftHreg = (_vClkRateMs * YSize);                                                          //time to shift rows into hreg
	_tReadHreg = (_hClkRateMs * (XSize / (XBin + 1)) * (YSize / (YBin + 1)));                     //time to shift charge in hreg to output amplifier
	_tCleanVert = (_vClkRateMs * YSize);                                                          //time to clean image area before new exposure
	_tUsbTransfer = ((XSize / (XBin + 1)) * (YSize / (YBin + 1)) * _usbTransferRateMs * 2);       //time to send last frame to usb fifo

	return _maxDelayPerFrameMs = (unsigned int)(_tExposure + _tCleanHreg2 + _tShiftHreg + _tReadHreg + _tCleanVert + _tUsbTransfer + addedDelayMs);
} 
void  zl_toucampro::getSingleFrame(double* pBuffer,int XSize,int YSize,int offset)
{
	// local vars
	double dTemp=0;
	unsigned int numBytesExpected = XSize*YSize*2; // num of bytes expected equals frame size x2 (2 bytes per pixeel)
	unsigned int numBytesAvailable = 0;                               // check num of bytes available to read from USB FIFO                             
	unsigned int nLength=numBytesExpected/2;
	DWORD numBytesRead = 0;                                    // count num of bytes read from USB FIFO
	unsigned int numBytesRx = 0;                                      // counts total number of bytes received (should equal frame size)
	unsigned int numBytesToRead = 0;                                  // counts number of bytes left to read for a full frame
	unsigned int pixCtr = 0;                                          // increments every 2bytes received from USB FIFO (1 pixel)
	byte*dataBuffer = new byte[65536];                        // byte array for storing data read from USB FIFO
	bool byteToWord = false;                                    // toggle to convert two consecutive bytes to one 16bit word
	int resultWord;                                             // store 16 bit pixel data
	unsigned int _expMs = 0;
	unsigned int _timeout = 0;
	int nMin=65535,nMax=0;
	unsigned short*Frame_16bit=new unsigned short[numBytesExpected/2];
	// load one full frame (numBytesExpected) from USB FIFO into framebuffer
	bRun=true;
	while ((numBytesRx < numBytesExpected)&&bRun==true)
	{
		// caluclate no. remaining bytes
		numBytesToRead = numBytesExpected - numBytesRx;
		// if full read of FTDI Buffer can be done, set numBytesAvailable bytes to 65535, else set numBytesAvailable to the number of remaining bytes
		if ((unsigned int)(numBytesToRead / 65535) != 0) numBytesAvailable = 65536;
		else numBytesAvailable = numBytesToRead;

		// determine frame period  before setting timeouts 
		_expMs = (int)(m_fExpTime*1000)+1;
		_timeout = calcMaxDelayPerFrameMs(_expMs, 0,YSize);

		// set FTDI timeout 
		SetTimeouts((unsigned int)(_timeout), 0);

		// read FTDI buffer 
		Read(dataBuffer, numBytesAvailable,&numBytesRead);  //reiad bytes from USB FIFO into dataBuffer

		// check all bytes received 

		// update bytes rxd counter 
		numBytesRx += numBytesRead; //update number of bytes received from USB FIFO


		// byte to pixel conversion 
		for (int i = 0; i < numBytesAvailable && pixCtr <nLength; i++)
		{
			//rtbxImageMemo.AppendText(DataBuffer[i].ToString("X") + ","); //print the byte
			byteToWord = !byteToWord; //used to concant the bytes into 16bit words, ie pixel data
			if (i >= 1 & !byteToWord) //every other byte convert to word ie 16 bit pixel info! 
			{
				resultWord = dataBuffer[i - 1] << 8;
				resultWord = resultWord + dataBuffer[i];    // little endian
				Frame_16bit[pixCtr] = (unsigned short)resultWord;   // add concatenated 16 bit word to array
				pixCtr++;
			}
		}
		numBytesAvailable = 0;  //reset to 0
	}

	bRun=false;

	for (int w=0;w<XSize;w++)
	{
		dTemp=0;
		for (int ww=0;ww<YSize;ww++)
		{		
			dTemp=dTemp+1.0*Frame_16bit[ww*XSize+w];
		}
		pBuffer[w]=dTemp/YSize-offset;
		//pBuffer[w]=dTemp;
	}

	delete [] Frame_16bit;
	delete [] dataBuffer;
}
void  zl_toucampro::getSingleFrameImage(BYTE* pBuffer,int XSize,int YSize)
{
	// local vars
	unsigned int numBytesExpected = XSize*YSize*2; // num of bytes expected equals frame size x2 (2 bytes per pixeel)
	unsigned int nLength=numBytesExpected/2;
	unsigned int numBytesAvailable = 0;                               // check num of bytes available to read from USB FIFO                             
	DWORD numBytesRead = 0;                                    // count num of bytes read from USB FIFO
	unsigned int numBytesRx = 0;                                      // counts total number of bytes received (should equal frame size)
	unsigned int numBytesToRead = 0;                                  // counts number of bytes left to read for a full frame
	unsigned int pixCtr = 0;                                          // increments every 2bytes received from USB FIFO (1 pixel)
	byte*dataBuffer = new byte[65536];                        // byte array for storing data read from USB FIFO
	bool byteToWord = false;                                    // toggle to convert two consecutive bytes to one 16bit word
	int resultWord;                                             // store 16 bit pixel data
	unsigned int _expMs = 0;
	unsigned int _timeout = 0;
	int nMin=65535,nMax=0;
	unsigned short*Frame_16bit=new unsigned short[numBytesExpected/2];
	// load one full frame (numBytesExpected) from USB FIFO into framebuffer
	bRun=true;
	while ((numBytesRx < numBytesExpected)&&bRun==true)
	{
		// caluclate no. remaining bytes
		numBytesToRead = numBytesExpected - numBytesRx;
		// if full read of FTDI Buffer can be done, set numBytesAvailable bytes to 65535, else set numBytesAvailable to the number of remaining bytes
		if ((unsigned int)(numBytesToRead / 65535) != 0) numBytesAvailable = 65536;
		else numBytesAvailable = numBytesToRead;

		// determine frame period  before setting timeouts 
		_expMs = (int)(m_fExpTime*1000);
		_timeout = calcMaxDelayPerFrameMs(_expMs, 0,YSize);
		
		// set FTDI timeout 
		SetTimeouts((unsigned int)(_timeout), 0);

		// read FTDI buffer 
		Read(dataBuffer, numBytesAvailable,&numBytesRead);  //reiad bytes from USB FIFO into dataBuffer

		// check all bytes received 

		// update bytes rxd counter 
		numBytesRx += numBytesRead; //update number of bytes received from USB FIFO

	
		// byte to pixel conversion 
		for (int i = 0; i < numBytesAvailable && pixCtr < nLength; i++)
		{
			//rtbxImageMemo.AppendText(DataBuffer[i].ToString("X") + ","); //print the byte
	
			byteToWord = !byteToWord; //used to concant the bytes into 16bit words, ie pixel data
			if (i >= 1 & !byteToWord) //every other byte convert to word ie 16 bit pixel info! 
			{
				
				resultWord = dataBuffer[i - 1] << 8;
				resultWord = resultWord + dataBuffer[i];    // little endian
				Frame_16bit[pixCtr] = (unsigned short)resultWord;   // add concatenated 16 bit word to array
				pixCtr++;

				if (nMin>=resultWord)
				{
					nMin=resultWord;
				}
				if (nMax<resultWord)
				{
					nMax=resultWord;
				}
			}
		}
		numBytesAvailable = 0;  //reset to 0
	}

	bRun=false;

	for (int w=0;w<numBytesExpected/2;w++)
	{
		pBuffer[w*3+0]=1.0*(nMax-Frame_16bit[w])/(nMax-nMin)*255;
		pBuffer[w*3+1]=1.0*(nMax-Frame_16bit[w])/(nMax-nMin)*255;
		pBuffer[w*3+2]=1.0*(nMax-Frame_16bit[w])/(nMax-nMin)*255;
	}
	


	delete [] Frame_16bit;
	delete [] dataBuffer;
}
void zl_toucampro::SetReadoutTriggerMode()
{
	unsigned char txbuf[25]={0}, rxbuf[25]={0},fbuf[4]={0};

	DWORD ret_bytes;
	txbuf[0]=0x53;
	txbuf[1]=0xe0;
	txbuf[2]=0x02;
	txbuf[3]=0xD4;
	txbuf[4]=0x84;
	txbuf[5]=0x50;
	txbuf[6]=txbuf[0]^txbuf[1]^txbuf[2]^txbuf[3]^txbuf[4]^txbuf[5];
	Write(txbuf,7,&ret_bytes);
	for (int i=0;i<5;i++)
	{
		Read(rxbuf+i,1,&ret_bytes);
	}

	memset(txbuf,0,25); memset(rxbuf,0,25);


	txbuf[0]=0x53;
	txbuf[1]=0xe0;
	txbuf[2]=0x02;
	txbuf[3]=0xf2;
	txbuf[4]=0x02;
	txbuf[5]=0x50;
	txbuf[6]=txbuf[0]^txbuf[1]^txbuf[2]^txbuf[3]^txbuf[4]^txbuf[5];
	Write(txbuf,7,&ret_bytes);
	for (int i=0;i<5;i++)
	{
		Read(rxbuf+i,1,&ret_bytes);
	}
	/*BYTE nSend[2];
	nSend[0]=0xd4;
	nSend[1]=0x84;
	WriteReg(nSend,2);


	nSend[0]=0xf2;
	nSend[1]=0x02;
	WriteReg(nSend,2);*/
}
void zl_toucampro::SetOneFrameNumber()
{
	//0x53 0xE0 0x02 0xF9 0xMM 0x50
	//	0x53 0xE0 0x02 0xF8 0xLL 0x50


	BYTE nSend[2];
	nSend[0]=0xf9;
	nSend[1]=0x00;
	WriteReg(nSend,2);


	nSend[0]=0xf8;
	nSend[1]=0x01;
	WriteReg(nSend,2);

}
void zl_toucampro::SetReadFraneNumber()
{
	//0x53 0xE0 0x02 0xFB 0xMM 0x50
	//	0x53 0xE0 0x02 0xFA 0xLL 0x50

	BYTE nSend[2];
	nSend[0]=0xfb;
	nSend[1]=0x00;
	WriteReg(nSend,2);

	
	nSend[0]=0xfa;
	nSend[1]=0x01;
	WriteReg(nSend,2);
}


void zl_toucampro::SetBin(int nMode,int nYF,int nYE)//0-None 7-FVB
{
	BYTE nbuf[10]={0x00,0x01,0x3,0x07,0x0f,0x1f,0x3f,0x80};

	if (nMode==2)
	{	
		BYTE nSend[2];
		nSend[0]=0xA2;
		nSend[1]=0x00;
		WriteReg(nSend,2);
	}
	if (nMode==0)
	{
		BYTE nSend[2];
		nSend[0]=0xA2;
		nSend[1]=0x80;
		WriteReg(nSend,2);
	}
	if (nMode==1)
	{
		BYTE nSend[2];
		
		nSend[0]=0xb4;
		nSend[1]=0x08;
		WriteReg(nSend,2);
		nSend[0]=0xb5;
		nSend[1]=0x00;
		WriteReg(nSend,2);
		
		nSend[0]=0xb6;
		nSend[1]=0x00;
		WriteReg(nSend,2);
		nSend[0]=0xb7;
		nSend[1]=0x00;
		WriteReg(nSend,2);

		
		int nBin=nYE-nYF+1;
		nSend[0]=0xb8;
		nSend[1]=nBin>>8;
		WriteReg(nSend,2);
		nSend[0]=0xb9;
		nSend[1]=nBin&0xff;
		WriteReg(nSend,2);
		
		nSend[0]=0xba;
		nSend[1]=nYF>>8;
		WriteReg(nSend,2);
		nSend[0]=0xbb;
		//nSend[1]=0x00;
		nSend[1]=nYF&0xff;
		WriteReg(nSend,2);

	
		
	}
	
	if (nMode==1)
	{
		int bin=nYE-nYF+1;
		if (bin==2)
		{
			BYTE nSend[2];
			nSend[0]=0xA2;
			nSend[1]=0x01;
			WriteReg(nSend,2);
		}
		if (bin==4)
		{
			BYTE nSend[2];
			nSend[0]=0xA2;
			nSend[1]=0x03;
			WriteReg(nSend,2);
		}
		if (bin==8)
		{
			BYTE nSend[2];
			nSend[0]=0xA2;
			nSend[1]=0x07;
			WriteReg(nSend,2);
		}
		if (bin==16)
		{
			BYTE nSend[2];
			nSend[0]=0xA2;
			nSend[1]=0x0f;
			WriteReg(nSend,2);
		}
		if (bin==32)
		{
			BYTE nSend[2];
			nSend[0]=0xA2;
			nSend[1]=0x1f;
			WriteReg(nSend,2);
		}
		if (bin>=64)
		{
			BYTE nSend[2];
			nSend[0]=0xA2;
			nSend[1]=0x3f;
			WriteReg(nSend,2);
		}
	}
}
void zl_toucampro::AbortCCD()
{
	bRun=false;
	unsigned char txbuf[25]={0xD4,0x8C}, rxbuf[25]={0};
	WriteReg(txbuf,2);
}

void zl_toucampro::SetRun(bool bRet)
{
	bRun = bRet;
}

void zl_toucampro::clear()
{
	BYTE nSend[2];
	nSend[0]=0xAd2;
	nSend[1]=0x04;
	WriteReg(nSend,2);
}