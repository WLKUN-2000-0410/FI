#ifndef __DIB_H
#define __DIB_H
//
// CDIB -
// CDibDC -
//
// Implements a simple encapsulation of a DIB section and a DC.
//
//

//////////////////////////////////////////////////////////////////////
//
// CDIB
//
// Device Independent Bitmap
//
// Function:
//
// Create, Display, Read & Save the DIB.
//
//////////////////////////////////////////////////////////////////////


class CDIB;
class CDibDC;

class CDIB : public CGdiObject
{
	DECLARE_DYNAMIC(CDIB)

public:
	//Create bitmap from handle
	static CDIB* PASCAL FromHandle(HBITMAP hBitmap);

// Constructors
	CDIB();

	//Create bitmap
	BOOL CreateDIB(int nWidth, int nHeight, UINT nBitcount, const void* lpBits=NULL);

	//Create bitmap
	BOOL CreateDIBIndirect(LPBITMAPINFO lpBitmap, const void* lpBits=NULL);

	//Capture the bitmap from the specified window.
	BOOL CaptureDIB(CWnd * pWnd, const CRect& capRect = CRect(0,0,0,0));

// Attributes
	//Get the handle
	operator HBITMAP() const;

	//Copy bitmap
	CDIB& operator = (CDIB& copy);

// Operations
	//Set the bits of bitmap.
	DWORD SetDIBBits(DWORD dwCount, const void* lpBits);
	//Get the bits form bitmap.
	LPVOID GetDIBBits(DWORD dwCount = 0, LPVOID lpBits = NULL);

// Implementation
public:
	virtual ~CDIB();
#ifdef _DEBUG
	virtual void Dump(CDumpContext& dc) const;
#endif

// Newly added functions
public:
	//Get the used color num.
	int GetColorUsed();

	//Load bitmap
	int LoadBmp(LPCTSTR filename);
	//Save bitmap
	int SaveBmp(LPCTSTR filename);

	//Set palette
	void SetPalette(UINT uStartIndex, UINT cEntries, CONST RGBQUAD *pColors);
	//Set palette
	void SetPalette(CPalette* pPal);
	
	//Get DC
	CDC* GetDC(void);
	//Release DC
	static BOOL ReleaseDC(CDC *pdc);

	//Get the bitmap.
	int GetBitmap(BITMAP* pBitMap);
	//Get DibSection
	int GetDibSection(DIBSECTION* pDibSection);
	//Get the image data from memory
	LPVOID GetMemImgData(const void *pMem);
	//Get the width of bitmap
	int GetWidth();
	//Get bytes of pixel
	int CDIB::GetBytesPixel(void);
	//Get the height of bitmap
	int GetHeight();
	//Get size of bitmap
	SIZE GetSize();
	//Get bytes of row
	int GetWidthBytes();
	//Get bits of image
	int GetBitCount();
	//Get the image data.
	LPVOID GetBits();
	//Get header of bitmap.
	LPBITMAPINFO GetBitmapInfo(void);
};

////////////////////////////////////////////////////////////////////
// inline functions

//////////////////////////////////////////////////////////////////
// 
// GetBitmap(BITMAP* pBitMap)
// 
// Function:
//     Get bitmap
//
// Arguments:
//	   ptr for bitmap : pBitMap
//
// Return:
//	   Succeed or Failed
//
//////////////////////////////////////////////////////////////////

inline int CDIB::GetBitmap(BITMAP* pBitMap)
{
	return(::GetObject(m_hObject, sizeof(BITMAP), pBitMap));
}


//////////////////////////////////////////////////////////////////
// 
// GetDibSection(DIBSECTION *pDibSection)
// 
// Function:
//     Get DibSection
//
// Arguments:
//	   ptr of DibSection : pDibSection
//
// Return:
//	   Succeed or Failed
//
//////////////////////////////////////////////////////////////////

inline int CDIB::GetDibSection(DIBSECTION *pDibSection)
{
	return(::GetObject(m_hObject, sizeof(DIBSECTION), pDibSection));
}


//////////////////////////////////////////////////////////////////
// 
// HBITMAP()
// 
// Function:
//     Get handle of bitmap
//
// Argument:
//	   None
//
// Return:
//	   handle of bitmap
//
//////////////////////////////////////////////////////////////////

inline CDIB::operator HBITMAP() const
{
	return (HBITMAP)(this == NULL ? NULL : m_hObject);
}


//////////////////////////////////////////////////////////////////
// 
// FromHandle(HBITMAP hDib)
// 
// Function:
//     Create DIB form handle
//
// Arguments:
//	   handle of bitmap : hDib
//
// Return:
//	   Object of DIB
//
//////////////////////////////////////////////////////////////////

inline CDIB* PASCAL CDIB::FromHandle(HBITMAP hDib)
{
	return((CDIB*)CGdiObject::FromHandle(hDib));
}


//////////////////////////////////////////////////////////////////
// 
// GetWidth(void)
// 
// Function:
//     Get width of bitmap
//
// Arguments:
//	   None
//
// Return:
//	   Width of bitmap
//
//////////////////////////////////////////////////////////////////

inline int CDIB::GetWidth(void)
{
	BITMAP bmp;
	GetBitmap(&bmp);
	return(bmp.bmWidth);
}

//////////////////////////////////////////////////////////////////
// 
// GetBitsPixel(void)
// 
// Function:
//     Get bits of pixel
//
// Arguments:
//	   None
//
// Return:
//	   Bits of pixel
//
//////////////////////////////////////////////////////////////////

inline int CDIB::GetBytesPixel(void)
{
	BITMAP bmp;
	GetBitmap(&bmp);
	return(bmp.bmBitsPixel/8);
}
//////////////////////////////////////////////////////////////////
// 
// GetHeight(void)
// 
// Function:
//     Get height of bitmap
//
// Arguments:
//	   None
//
// Return:
//	   Height of bitmap
//
//////////////////////////////////////////////////////////////////

inline int CDIB::GetHeight(void)
{
	BITMAP bmp;
	GetBitmap(&bmp);
	return(bmp.bmHeight);
}


//////////////////////////////////////////////////////////////////
// 
// GetSize(void)
// 
// Function:
//     Get size of bitmap
//
// Arguments:
//	   None
//
// Return:
//	   Size of bitmap
//
//////////////////////////////////////////////////////////////////

inline SIZE CDIB::GetSize(void)
{
	BITMAP bmp;
	GetBitmap(&bmp);
	CSize size(bmp.bmWidth, bmp.bmHeight);
	return(size);
}


//////////////////////////////////////////////////////////////////
// 
// GetWidthBytes(void)
// 
// Function:
//     Get bytes of row
//
// Arguments:
//	   None
//
// Return:
//	   Bytes of row
//
//////////////////////////////////////////////////////////////////

inline int CDIB::GetWidthBytes(void)
{
	BITMAP bmp;
	GetBitmap(&bmp);
	return(bmp.bmWidthBytes);
}


//////////////////////////////////////////////////////////////////
// 
// GetBitCount(void)
// 
// Function:
//     Get bits of bitmap
//
// Arguments:
//	   None
//
// Return:
//	   Bits of bitmap
//
//////////////////////////////////////////////////////////////////

inline int CDIB::GetBitCount(void)
{
	BITMAP bmp;
	GetBitmap(&bmp);
	return(bmp.bmBitsPixel);
}


//////////////////////////////////////////////////////////////////
// 
// GetBits(void)
// 
// Funciton:
//     Get image data
//
// Arguments:
//	   None
//
// Return:
//	   Image data
//
//////////////////////////////////////////////////////////////////

inline LPVOID CDIB::GetBits(void)
{
	BITMAP bmp;
	GetBitmap(&bmp);
	return(bmp.bmBits);
}

//////////////////////////////////////////////////////////////////////

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


class CDibDC : public CDC
{
	DECLARE_DYNAMIC(CDibDC)

// Constructors
public:
	CDibDC();

// Attributes
protected:
	HBITMAP m_hOld;

// Implementation
public:
	CPalette * GetPalette();
	virtual ~CDibDC();
	
	friend class CDIB;
};

#endif //__DIB_H