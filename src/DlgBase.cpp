#include "StdAfx.h"
#include ".\DlgBase.h"

#include <windowsX.h>

CDlgBase::CDlgBase(void)
{
	m_hRgn	= NULL;
}

CDlgBase::~CDlgBase(void)
{
	m_bitmapBG.DeleteObject();

	if( m_hRgn)
		DeleteRgn(m_hRgn);	
}

//////////////////////////////////////////////////////////////////////////
// HWND의 투명도를 설정한다. 
//////////////////////////////////////////////////////////////////////////
BOOL CDlgBase::SetWindowTransparent(HWND hWnd, BYTE nAlpha)
{
	ASSERT(hWnd);
	if( FALSE == ::IsWindow(hWnd))
		return FALSE;

	#define LWA_ALPHA 0x00000002


	// 투명윈도우를 위한 LayeredWindow 설정
	long oldWinStyle = GetWindowLong(hWnd, GWL_STYLE);
	::SetWindowLong(hWnd, GWL_STYLE,  oldWinStyle & WS_POPUPWINDOW & ~WS_BORDER );

	LONG style = ::GetWindowLong(hWnd, GWL_EXSTYLE);
	if(!(style & 0x00080000))
	{
		::SetWindowLong(hWnd, GWL_EXSTYLE, style | 0x00080000);
	}

	typedef BOOL (WINAPI *SLWA)(HWND hWnd, COLORREF crKey, BYTE bAlpha, DWORD dwFlags);
	SLWA pSetLayeredWindowAttributes = NULL;
	HINSTANCE hUSER32 = LoadLibrary("USER32.DLL");
	pSetLayeredWindowAttributes=(SLWA)GetProcAddress(hUSER32,"SetLayeredWindowAttributes");
	BYTE chAlpha = nAlpha;
	pSetLayeredWindowAttributes(hWnd, NULL, chAlpha, LWA_ALPHA);
	FreeLibrary(hUSER32);

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
// HWND 의 제목표시줄을 없앤다.
//////////////////////////////////////////////////////////////////////////
BOOL CDlgBase::HideApplicationTitlebar(HWND hWnd)
{
	ASSERT(hWnd);
	if( FALSE == ::IsWindow(hWnd))
		return FALSE;

	DWORD dwStyle = ::GetWindowLong(hWnd, GWL_EXSTYLE);
	dwStyle &= ~WS_EX_APPWINDOW;
	dwStyle |= WS_EX_TOOLWINDOW;
	::SetWindowLong(hWnd, GWL_EXSTYLE, dwStyle);

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
// HWND의 윈도우 모양을 설정한다.
//////////////////////////////////////////////////////////////////////////
BOOL CDlgBase::SetWindowRegion(HWND hWnd, UINT nIDResource)
{
	ASSERT(hWnd);
	if( FALSE == ::IsWindow(hWnd))
		return FALSE;

	m_bitmapBG.LoadBitmap(nIDResource);
	HBITMAP hBmp = (HBITMAP)m_bitmapBG.GetSafeHandle();

	// Region을 생성한다. 
	m_hRgn = CreateRgnFromFile(hBmp, RGB(255,0,255));

	HDC hDC = ::GetDC(hWnd);

	m_dcBkGrnd = CreateCompatibleDC(hDC);
	::ReleaseDC(hWnd, hDC);

	SelectObject(m_dcBkGrnd, hBmp);
	::SetWindowPos(hWnd, NULL, 0, 0, m_dwWidth, m_dwHeight, SWP_NOZORDER|SWP_NOMOVE);

	::SetWindowRgn(hWnd, m_hRgn, TRUE);


	return TRUE;
}


//------------------------ Protected Function ---------------------------------------------

//////////////////////////////////////////////////////////////////////////
// HBITMAP으로 HRGB을 생성한다. 
// color : 마스크
//////////////////////////////////////////////////////////////////////////
HRGN CDlgBase::CreateRgnFromFile(HBITMAP hBmp, COLORREF color)
{
	// Written By Yuriy Zaporozhets's  http://www.codeproject.com/gdi/coolrgn.asp

	// get image properties
	BITMAP bmp = { 0 };
	GetObject( hBmp, sizeof(BITMAP), &bmp );
	// allocate memory for extended image information
	LPBITMAPINFO bi = (LPBITMAPINFO) new BYTE[ sizeof(BITMAPINFO) + 8 ];
	memset( bi, 0, sizeof(BITMAPINFO) + 8 );
	bi->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	// set window size
	m_dwWidth = bmp.bmWidth;  // bitmap width
	m_dwHeight = bmp.bmHeight;  // bitmap height
	// create temporary dc
	HDC dc = CreateIC( "DISPLAY",NULL,NULL,NULL );
	// get extended information about image (length, compression, length of color table if exist, ...)
	DWORD res = GetDIBits( dc, hBmp, 0, bmp.bmHeight, 0, bi, DIB_RGB_COLORS );
	// allocate memory for image data (colors)
	LPBYTE pBits = new BYTE[ bi->bmiHeader.biSizeImage + 4 ];
	// allocate memory for color table
	if ( bi->bmiHeader.biBitCount == 8 )
	{
		// actually color table should be appended to this header(BITMAPINFO),
		// so we have to reallocate and copy it
		LPBITMAPINFO old_bi = bi;
		// 255 - because there is one in BITMAPINFOHEADER
		bi = (LPBITMAPINFO)new char[ sizeof(BITMAPINFO) + 255 * sizeof(RGBQUAD) ];
		memcpy( bi, old_bi, sizeof(BITMAPINFO) );
		// release old header
		delete old_bi;
	}
	// get bitmap info header
	BITMAPINFOHEADER& bih = bi->bmiHeader;
	// get color table (for 256 color mode contains 256 entries of RGBQUAD(=DWORD))
	LPDWORD clr_tbl = (LPDWORD)&bi->bmiColors;
	// fill bits buffer
	res = GetDIBits( dc, hBmp, 0, bih.biHeight, pBits, bi, DIB_RGB_COLORS );
	DeleteDC( dc );

	BITMAP bm;
	GetObject( hBmp, sizeof(BITMAP), &bm );
	// shift bits and byte per pixel (for comparing colors)
	LPBYTE pClr = (LPBYTE)&color;
	// swap red and blue components
	BYTE tmp = pClr[0]; pClr[0] = pClr[2]; pClr[2] = tmp;
	// convert color if curent DC is 16-bit (5:6:5) or 15-bit (5:5:5)
	if ( bih.biBitCount == 16 )
	{
		// for 16 bit
		color = ((DWORD)(pClr[0] & 0xf8) >> 3) |
			((DWORD)(pClr[1] & 0xfc) << 3) |
			((DWORD)(pClr[2] & 0xf8) << 8);
		// for 15 bit
		//  color = ((DWORD)(pClr[0] & 0xf8) >> 3) |
		//    ((DWORD)(pClr[1] & 0xf8) << 2) |
		//    ((DWORD)(pClr[2] & 0xf8) << 7);
	}

	const DWORD RGNDATAHEADER_SIZE = sizeof(RGNDATAHEADER);
	const DWORD ADD_RECTS_COUNT  = 40;   // number of rects to be appended
	// to region data buffer

	// BitPerPixel
	BYTE Bpp = bih.biBitCount >> 3;    // bytes per pixel
	// bytes per line in pBits is DWORD aligned and bmp.bmWidthBytes is WORD aligned
	// so, both of them not
	DWORD m_dwAlignedWidthBytes = (bmp.bmWidthBytes & ~0x3) + (!!(bmp.bmWidthBytes & 0x3) << 2);
	// DIB image is flipped that's why we scan it from the last line
	LPBYTE pColor = pBits + (bih.biHeight - 1) * m_dwAlignedWidthBytes;
	DWORD dwLineBackLen = m_dwAlignedWidthBytes + bih.biWidth * Bpp; // offset of previous scan line
	// (after processing of current)
	DWORD dwRectsCount = bih.biHeight;   // number of rects in allocated buffer
	INT  i, j;         // current position in mask image
	INT  first = 0;        // left position of current scan line
	// where mask was found
	bool wasfirst = false;      // set when mask has been found in current scan line
	bool ismask;         // set when current color is mask color

	// allocate memory for region data
	// region data here is set of regions that are rectangles with height 1 pixel (scan line)
	// that's why first allocation is <bm.biHeight> RECTs - number of scan lines in image
	RGNDATAHEADER* pRgnData = 
		(RGNDATAHEADER*)new BYTE[ RGNDATAHEADER_SIZE + dwRectsCount * sizeof(RECT) ];
	// get pointer to RECT table
	LPRECT pRects = (LPRECT)((LPBYTE)pRgnData + RGNDATAHEADER_SIZE);
	// zero region data header memory (header  part only)
	memset( pRgnData, 0, RGNDATAHEADER_SIZE + dwRectsCount * sizeof(RECT) );
	// fill it by default
	pRgnData->dwSize = RGNDATAHEADER_SIZE;
	pRgnData->iType  = RDH_RECTANGLES;

	for ( i = 0; i < bih.biHeight; i++ )
	{
		for ( j = 0; j < bih.biWidth; j++ )
		{
			// get color
			switch ( bih.biBitCount )
			{
			case 8:
				ismask = (clr_tbl[ *pColor ] != color);
				break;
			case 16:
				ismask = (*(LPWORD)pColor != (WORD)color);
				break;
			case 24:
				ismask = ((*(LPDWORD)pColor & 0x00ffffff) != color);
				break;
			case 32:
				ismask = (*(LPDWORD)pColor != color);
			}
			// shift pointer to next color
			pColor += Bpp;
			// place part of scan line as RECT region if transparent color found after mask color or
			// mask color found at the end of mask image
			if ( wasfirst )
			{
				if ( !ismask )
				{
					// save current RECT
					pRects[ pRgnData->nCount++ ] = CRect( first, i, j, i + 1 );
					// if buffer full reallocate it with more room
					if ( pRgnData->nCount >= dwRectsCount )
					{
						dwRectsCount += ADD_RECTS_COUNT;
						// allocate new buffer
						LPBYTE pRgnDataNew = new BYTE[ RGNDATAHEADER_SIZE + dwRectsCount * sizeof(RECT) ];
						// copy current region data to it
						memcpy( pRgnDataNew, pRgnData, RGNDATAHEADER_SIZE + pRgnData->nCount * sizeof(RECT) );
						// delte old region data buffer
						delete pRgnData;
						// set pointer to new regiondata buffer to current
						pRgnData = (RGNDATAHEADER*)pRgnDataNew;
						// correct pointer to RECT table
						pRects = (LPRECT)((LPBYTE)pRgnData + RGNDATAHEADER_SIZE);
					}
					wasfirst = false;
				}
			}
			else if ( ismask )  // set wasfirst when mask is found
			{
				first = j;
				wasfirst = true;
			}
		}

		if ( wasfirst && ismask )
		{
			// save current RECT
			pRects[ pRgnData->nCount++ ] = CRect( first, i, j, i + 1 );
			// if buffer full reallocate it with more room
			if ( pRgnData->nCount >= dwRectsCount )
			{
				dwRectsCount += ADD_RECTS_COUNT;
				// allocate new buffer
				LPBYTE pRgnDataNew = new BYTE[ RGNDATAHEADER_SIZE + dwRectsCount * sizeof(RECT) ];
				// copy current region data to it
				memcpy( pRgnDataNew, pRgnData, RGNDATAHEADER_SIZE + pRgnData->nCount * sizeof(RECT) );
				// delte old region data buffer
				delete pRgnData;
				// set pointer to new regiondata buffer to current
				pRgnData = (RGNDATAHEADER*)pRgnDataNew;
				// correct pointer to RECT table
				pRects = (LPRECT)((LPBYTE)pRgnData + RGNDATAHEADER_SIZE);
			}
			wasfirst = false;
		}

		pColor -= dwLineBackLen;
	}
	// release image data
	delete pBits;
	delete bi;

	// create region
	HRGN hRgn = ExtCreateRegion( NULL, RGNDATAHEADER_SIZE + pRgnData->nCount * sizeof(RECT), (LPRGNDATA)pRgnData );
	// release region data
	delete pRgnData;

	return hRgn;

}