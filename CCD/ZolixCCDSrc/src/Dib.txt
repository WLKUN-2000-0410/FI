#include "stdafx.h"
#include "Dib.h"

IMPLEMENT_DYNAMIC(CDIB, CGdiObject);

//////////////////////////////////////////////////////////////////////
//
// CDIB
//
// Device independent Bitmap(Version 1.0)
//
// Function:
//
// Create, Display, Read & Save the DIB.
//
//////////////////////////////////////////////////////////////////////

//Construction
CDIB::CDIB() : CGdiObject()
{
}

CDIB::~CDIB()
{
	DeleteObject();
}


//////////////////////////////////////////////////////////////////
// 
// CreateDIB(int cx, int cy, UINT ibitcount, const void* lpBits) 
// 
// Function:
//     Crate DIB
//
// Arguments:
//	   Width  : cx
//     Height : cy
//     Bits   : ibitcount 
//     Data   : lpBits 
//
// Return:
//	   Succeed or Failed
//
//////////////////////////////////////////////////////////////////

BOOL CDIB::CreateDIB(int cx, int cy, UINT ibitcount, const void* lpBits) 
{   //The value of ibitcount must be 1£¬4£¬8£¬16£¬24 or 32£¨the biBitCount in bitmap header)
	ASSERT((ibitcount == 1) || (ibitcount == 4) || 
			(ibitcount == 8) || (ibitcount == 16) 
			|| (ibitcount == 24) ||(ibitcount == 32))	;

	// Create a BITMAPINFOHEADER structure to describe the DIB
    int iSize = sizeof(BITMAPINFOHEADER);// + 256*sizeof(RGBQUAD);
	BITMAPINFO* pBMI;//Pointer of bitmap header
	BYTE *pByte;//Pointer of bitmap data.

	switch(ibitcount){
		case 8://256 colors
			iSize += 4*4;
			break;
		case 1://1 colors
		case 4://16 colors
		case 16://16 bits 
		case 24://24 bits
		case 32://32 bits
			break;
		default:
			break;
	}

	pByte = new BYTE[iSize];
	pBMI = (BITMAPINFO*) pByte;
    memset(pBMI, 0, iSize);
	pBMI->bmiHeader.biSize        = sizeof(BITMAPINFOHEADER);
	pBMI->bmiHeader.biWidth       = cx;
	pBMI->bmiHeader.biHeight      = cy;
	pBMI->bmiHeader.biPlanes      = 1;
	pBMI->bmiHeader.biBitCount    = ibitcount;
	pBMI->bmiHeader.biCompression = BI_RGB; // Umcompressed

	BOOL bRet = CreateDIBIndirect(pBMI, lpBits);
	delete[] pByte;
//	delete pBMI;
	return(bRet);
}


//////////////////////////////////////////////////////////////////
// 
// CreateDIBIndirect(LPBITMAPINFO pBMI, const void* lpBits)
// 
// Function:
//     Create DIB
//
// Arguments:
//	   Ptr of bitmap info : pBMI
//     Image data         : lpBits 
//
// Return:
//	   Succeed or Failed
//
//////////////////////////////////////////////////////////////////

BOOL CDIB::CreateDIBIndirect(LPBITMAPINFO pBMI, const void* lpBits)
{
	//Delete existing object
	if (m_hObject != NULL)
	{
		DeleteObject();
		delete pBMI;
	}

	// Create the DIB section.
	CDC *pDC = new CDC;
	pDC->CreateCompatibleDC(NULL);
	LPVOID pBits;
	HBITMAP hDIB = ::CreateDIBSection(pDC->GetSafeHdc(),
							pBMI,
							DIB_RGB_COLORS,
                           	&pBits,
                           	NULL,
                           	0);
//		::SetDIBits(pDC->GetSafeHdc(),hDIB,0,GetHeight(),LpBits,pBMI,DIB_RGB_COLORS);
	delete pDC;
	DeleteObject();
	ASSERT(hDIB);
	ASSERT(pBits);
	Attach(hDIB);

	//Duplicate the image data.
	SetDIBBits(GetWidthBytes() * GetHeight(), lpBits);
//	::DeleteObject(hDIB);
//	delete pBMI;

	return TRUE;
}


//////////////////////////////////////////////////////////////////
// 
// SetDIBBits(DWORD dwCount,const void * lpBits)
// 
// Function:
//     Set image data
//
// Arguments:
//	   Size     : dwCount
//     Data Ptr : lpBits 
//
// Return:
//	   Size of duplicated
//
//////////////////////////////////////////////////////////////////

DWORD CDIB::SetDIBBits(DWORD dwCount,const void * lpBits)
{
	if(lpBits != NULL){
		LPVOID pBits = GetBits();
		memcpy(pBits,lpBits,dwCount);
		return dwCount;
	//	::DeleteObject(pBits);

	}
	return 0;
}


//////////////////////////////////////////////////////////////////
// 
// GetDIBBits(DWORD dwCount, LPVOID lpBits)
// 
// Function:
//     Get the image data.
//     if lpBits is empty the returned is the image data pointer, 
//	   otherwise, the returned is the image data.
//
// Arguments:
//	   Size     : dwCount
//     Data Ptr : lpBits 
//
// Return:
//	   Data ptr
//
//////////////////////////////////////////////////////////////////

LPVOID CDIB::GetDIBBits(DWORD dwCount, LPVOID lpBits)
{
	LPVOID pBits = GetBits();
	if(lpBits != NULL){
		memcpy(lpBits,pBits,dwCount);
		return pBits;
	}
	else{
		return pBits;
	}
}

//////////////////////////////////////////////////////////////////
// 
// operator = (CDIB& copy)
// 
// Function:
//     operator =
//
// Arguments:
//	   The DIB to copy
//
// Return:
//	   New DIB
//
//////////////////////////////////////////////////////////////////

CDIB& CDIB::operator = (CDIB& copy)
{
 	DIBSECTION DibSection;
	//Get DibSection
	copy.GetDibSection(&DibSection);
	int nSize = DibSection.dsBmih.biClrUsed*sizeof(RGBQUAD) + sizeof(BITMAPINFOHEADER);

	//Allocate the memory for new bitmap info header
	BYTE *pByte = new BYTE[nSize];
	//Copy	
	memcpy(pByte, &(DibSection.dsBmih), sizeof(BITMAPINFOHEADER));

	CDC *pdc = copy.GetDC();
	//Get the color table
	::GetDIBColorTable(pdc->GetSafeHdc(), 0, DibSection.dsBmih.biClrUsed,
						(RGBQUAD*)(pByte+sizeof(BITMAPINFOHEADER)));
	copy.ReleaseDC(pdc);

	//Create new bitmap
	BITMAPINFO *pBMI = (BITMAPINFO*)pByte;
	CreateDIBIndirect(pBMI);

	//Copy the bitmap
	int nTotalSize = copy.GetWidthBytes() * copy.GetHeight();
	memcpy(GetBits(), copy.GetBits(), nTotalSize);

	delete[] pByte;
	return(*this);
}


//////////////////////////////////////////////////////////////////
// 
// SetPalette(UINT uStartIndex, UINT cEntries, CONST RGBQUAD *pColors)
// 
// Function:
//     Set Palette
//
// Arguments:
//	   Index of Palette  : uStartIndex
//     Entry for Palette : cEntries
//     Color data        : pColors
//
// Return:
//	   None
//
//////////////////////////////////////////////////////////////////

void CDIB::SetPalette(UINT uStartIndex, UINT cEntries, CONST RGBQUAD *pColors)
{
	HDC hdc = ::CreateCompatibleDC(NULL);
	HBITMAP hOld = (HBITMAP)::SelectObject(hdc, m_hObject);

	::SetDIBColorTable(hdc, uStartIndex, cEntries, pColors);
	
	::SelectObject(hdc, hOld);
	::DeleteObject(hdc);
}


//////////////////////////////////////////////////////////////////
// 
// SetPalette(CPalette* pPal)
// 
// Function:
//     Set Palette
//
// Arguments:
//	   Palette struct pointer : pPal
//
// Return:
//	   None
//
//////////////////////////////////////////////////////////////////

void CDIB::SetPalette(CPalette* pPal)
{
    ASSERT(pPal);

    // get the colors from the palette
    int iColors = 0;
    pPal->GetObject(sizeof(iColors), &iColors);
    ASSERT(iColors > 0);
    PALETTEENTRY* pPE = new PALETTEENTRY[iColors];
    pPal->GetPaletteEntries(0, iColors, pPE);

    // Build a table of RGBQUADS
    RGBQUAD* pRGB = new RGBQUAD[iColors];
    ASSERT(pRGB);
    for (int i = 0; i < iColors; i++) {
        pRGB[i].rgbRed = pPE[i].peRed;
        pRGB[i].rgbGreen = pPE[i].peGreen;
        pRGB[i].rgbBlue = pPE[i].peBlue;
        pRGB[i].rgbReserved = 0;
    }

	SetPalette(0, iColors, pRGB);

    delete [] pRGB;
    delete [] pPE;
}


//////////////////////////////////////////////////////////////////
// 
// GetDC(void)
// 
// Function:
//     Get the DC associated with DIB
//
// Arguments:
//	   None
//
// Return:
//	   DC ptr associated with DIB
//
//////////////////////////////////////////////////////////////////

CDC* CDIB::GetDC(void)
{
	CDibDC* pdc = new CDibDC;
	if(pdc == NULL)
		return(NULL);
	pdc->CreateCompatibleDC(NULL);
	pdc->m_hOld = (HBITMAP)::SelectObject(pdc->GetSafeHdc(), GetSafeHandle());

	return(pdc);
}


//////////////////////////////////////////////////////////////////
// 
// ReleaseDC(CDC *pdc)
// 
// Function:
//     Release the DC associated with DIB
//
// Arguments:
//	   DC ptr associated with DIB
//
// Return:
//	   Succeed or Failed
//
//////////////////////////////////////////////////////////////////

BOOL CDIB::ReleaseDC(CDC *pdc)
{
	ASSERT(pdc != NULL);
	if(pdc->IsKindOf(RUNTIME_CLASS(CDibDC))){
		delete pdc;
		return(TRUE);
	}
	return(FALSE);
}

#ifdef _DEBUG
void CDIB::Dump(CDumpContext& dc) const
{
	CGdiObject::Dump(dc);

	if (m_hObject == NULL)
		return;

	BITMAP bm;
	VERIFY(GetObject(sizeof(bm), &bm));
	dc << _T("bm.bmType = ") << bm.bmType;
	dc << _T("\nbm.bmHeight = ") << bm.bmHeight;
	dc << _T("\nbm.bmWidth = ") << bm.bmWidth;
	dc << _T("\nbm.bmWidthBytes = ") << bm.bmWidthBytes;
	dc << _T("\nbm.bmPlanes = ") << bm.bmPlanes;
	dc << _T("\nbm.bmBitsPixel = ") << bm.bmBitsPixel;

	dc << _T("\n");
}
#endif


//////////////////////////////////////////////////////////////////
// 
// LoadBmp(LPCSTR filename)
// 
// Function:
//     Load the bitmap file
//
// Arguments:
//	   File name : filename
//
// Return:
//	   Succeed or Failed
//
//////////////////////////////////////////////////////////////////

int CDIB::LoadBmp(LPCTSTR filename)
{
	//Open File
	CFile file((LPCTSTR)filename,CFile::modeRead|CFile::shareDenyNone);

	WORD	bfType;
	DWORD   bfSize; 
	
	//Load the file header
	file.Read(&bfType,sizeof(WORD));
	file.Read(&bfSize,sizeof(DWORD));
	if(bfSize <= 0)
	{
		file.Close();
		return -1;
	}
	
	//BMP or not
	if (bfType != (((WORD)'M'<<8) + 'B'))
	{
		file.Close();
		return -1;
	}

	DWORD   bfOffBits; 
	//Get the start index of image data and load
	file.Seek(2*sizeof(WORD),CFile::current);
	file.Read(&bfOffBits,sizeof(DWORD));

	LPVOID DibBuf;
	
	DibBuf = (LPVOID)new char[bfSize];

	if(DibBuf == NULL)
	{
		file.Close();
		return -1;
	}

	//Load image data
	file.Read(DibBuf,bfSize);
	file.Close();
	
	LPBITMAPINFO pBMI;
	
	int size = bfOffBits - 14;
	pBMI = (LPBITMAPINFO)new char[sizeof(BITMAPINFOHEADER)+256*sizeof(RGBQUAD)];

	//Generate the info header
	memcpy(pBMI,DibBuf,size);
	LPVOID lpBits = (LPVOID)((LPSTR)DibBuf + size);

	//Create DIB
	CreateDIBIndirect(pBMI, lpBits);

	delete pBMI;
	delete DibBuf;
	return 1;
}


//////////////////////////////////////////////////////////////////
// 
// GetColorUsed()
// 
// Function:
//     Get colors
//
// Arguments:
//	   None
//
// Return:
//	   Colors
//
//////////////////////////////////////////////////////////////////

int CDIB::GetColorUsed()
{
	LPBITMAPINFOHEADER pBMIH;
	DIBSECTION DibSection;
	GetDibSection(&DibSection);
	pBMIH = &DibSection.dsBmih;
	return pBMIH->biClrUsed;
}

//////////////////////////////////////////////////////////////////
// 
// GetPalette()
// 
// Function:
//     Get palette
//
// Arguments:
//	   None
//
// Return:
//	   Palette ptr
//
//////////////////////////////////////////////////////////////////

CPalette * CDibDC::GetPalette()
{
	LOGPALETTE * pLogPal = (LOGPALETTE *)new char[sizeof(LOGPALETTE) +
		 256 * sizeof(PALETTEENTRY)];

	pLogPal->palVersion = 0x300;
	pLogPal->palNumEntries = 256;

	HDC hdc = GetSafeHdc();
	RGBQUAD pRGB[256];
	::GetDIBColorTable(hdc, 0, 256,pRGB);
	
	for(int i = 0 ; i < 256 ; i ++)
	{
		pLogPal->palPalEntry[i].peRed = pRGB[i].rgbRed;
		pLogPal->palPalEntry[i].peGreen = pRGB[i].rgbGreen;
		pLogPal->palPalEntry[i].peBlue = pRGB[i].rgbBlue;
		pLogPal->palPalEntry[i].peFlags = 0;
	}
	
	CPalette * pPal = NULL;
	pPal = new CPalette;	
	pPal->CreatePalette(pLogPal);

	delete pLogPal;
	return pPal;
}

//////////////////////////////////////////////////////////////////
// 
// GetBitmapInfo(void)
// 
// Function:
//     Get the bitmap info
//
// Arguments:
//	   None
//
// Return:
//	   bitmap info ptr
//
//////////////////////////////////////////////////////////////////

LPBITMAPINFO CDIB::GetBitmapInfo(void)
{
	DIBSECTION DibSection;
	GetDibSection(&DibSection);
	int nSize = DibSection.dsBmih.biClrUsed*sizeof(RGBQUAD) + sizeof(BITMAPINFOHEADER);
	
	BYTE *pByte = new BYTE[nSize];
	memcpy(pByte, &(DibSection.dsBmih), sizeof(BITMAPINFOHEADER));
	CDC *pdc = GetDC();
	::GetDIBColorTable(pdc->GetSafeHdc(), 0, DibSection.dsBmih.biClrUsed,
						(RGBQUAD*)(pByte+sizeof(BITMAPINFOHEADER)));
	ReleaseDC(pdc);
	BITMAPINFO *pBMI = (BITMAPINFO*)pByte;
	return(pBMI);
}


//////////////////////////////////////////////////////////////////
// 
// SaveBmp(LPCSTR filename)
// 
// Function:
//     Save the bitmap
//
// Arguments:
//	   File name : filename
//
// Return:
//	   Succeed or Failed
//
//////////////////////////////////////////////////////////////////

int CDIB::SaveBmp(LPCTSTR filename)
{
	BITMAPFILEHEADER hdr;

	//Open file
	CFile file((LPCTSTR)filename,CFile::modeWrite|CFile::modeCreate);

	//File header
	hdr.bfType = ((WORD)'M'<<8) + 'B';
	LPBITMAPINFO pbi = GetBitmapInfo(); 
	PBITMAPINFOHEADER pbih = (PBITMAPINFOHEADER) pbi; 

	hdr.bfSize = (DWORD) (sizeof(BITMAPFILEHEADER) + 
                 pbih->biSize + pbih->biClrUsed 
                 * sizeof(RGBQUAD) + pbih->biSizeImage); 

    hdr.bfReserved1 = 0; 
    hdr.bfReserved2 = 0; 

    // Compute the offset to the array of color indices. 
    hdr.bfOffBits = (DWORD) sizeof(BITMAPFILEHEADER) + 
                    pbih->biSize + pbih->biClrUsed 
                    * sizeof (RGBQUAD); 
	
    //Write the file header
	file.Write((LPVOID) &hdr, sizeof(BITMAPFILEHEADER));
	//Write the info header
    file.Write((LPVOID) pbih, sizeof(BITMAPINFOHEADER) 
                  + pbih->biClrUsed * sizeof (RGBQUAD)); 

    // Copy the array of color indices into the .BMP file. 
    int nTotal = pbih->biSizeImage; 
    LPVOID lpBits = GetBits(); 
    file.Write(lpBits, nTotal);

	//Close file
	file.Close();	
	
	delete pbi;

	return 1;
}


//////////////////////////////////////////////////////////////////
// 
// CaptureDIB(CWnd * pWnd, const CRect& capRect)
// 
// Function:
//     Capture the bitmap form specified window
//
// Arguments:
//	   Handle of window : pWnd
//     Rect             : capRect
//
// Return:
//	   Succeed or Failed
//
//////////////////////////////////////////////////////////////////

BOOL CDIB::CaptureDIB(CWnd * pWnd, const CRect& capRect)
{
	BOOL ret = false;

	if(pWnd == NULL)
		return false;

	CDC * pPlayDc = pWnd->GetDC();

	if(pPlayDc == NULL)
		return false;

	CRect Rect;
	if(capRect.IsRectEmpty())
		pWnd->GetClientRect(Rect);
	else
		Rect = capRect;

	//Colors
	UINT nBitCount = pPlayDc->GetDeviceCaps(BITSPIXEL);

	//Create DIB
	if(CreateDIB(Rect.Width(), Rect.Height(), nBitCount))
	{
		CDC * pCopyDc = GetDC();
		
		if(pCopyDc == NULL)
		{
			pWnd->ReleaseDC(pPlayDc);		
			return false;
		}

		pWnd->ShowWindow(SW_SHOW);
		//Capture
		if(pCopyDc->BitBlt(0, 0, Rect.Width(), Rect.Height(), pPlayDc, 0, 0, SRCCOPY))
			ret = true;
		
		ReleaseDC(pCopyDc);
	}

	pWnd->ReleaseDC(pPlayDc);		
	return ret;
}


////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
//
// CDibDC
//
// DC for DIB(Version 1.0)
//
// Function:
//     Associated with DIB
//
//////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC(CDibDC, CDC);

CDibDC::CDibDC()
{
	m_hOld = NULL;
}

CDibDC::~CDibDC()
{
	if(m_hOld != NULL){
		::SelectObject(GetSafeHdc(), m_hOld);
	}
}
LPVOID CDIB::GetMemImgData(const void *LpMem)
{   DWORD i,Ipoint,Mpoint;
	LPVOID LpImg;
    LpImg=(LPVOID)new char[320*240*3];
    for(i=0,Ipoint=0,Mpoint=1;i<320*240;i++,Ipoint+=3,Mpoint+=4)
	     memcpy((LPSTR)LpImg+Ipoint,(LPSTR)LpMem+Mpoint,3);
	return LpImg;
}


