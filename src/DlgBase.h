//////////////////////////////////////////////////////////////////////////
// File		: DlgBase.h
// Author	: 한상철 
// Role		: 다이얼로그의 Base 클래스
//			  윈도우 투명도 조정, 모양변경, 제목표시줄등의 기능이 있다.
//
// Date		: 2007-06-04
//////////////////////////////////////////////////////////////////////////



#pragma once


class CDlgBase
{
public:
	CDlgBase(void);
	~CDlgBase(void);


	//	윈도우 투명값 설정
	// nAlpha : 0~255이 값으로 투명하게 설정
	BOOL	SetWindowTransparent(HWND hWnd, BYTE nAlpha);

	//  윈도우 모양 변경
	//  nIDResource : 윈도우의 배경이 되는 비트맵
	BOOL	SetWindowRegion(HWND hWnd, UINT nIDResource);

	//  윈도우 제목표시줄 숨기기
	BOOL	HideApplicationTitlebar(HWND hWnd);	

	


protected:

	// 비트맵으로 Region 생성
	HRGN CreateRgnFromFile(HBITMAP hBmp, COLORREF color);
	DWORD		m_dwWidth;		// 윈도우의 넓이
	DWORD		m_dwHeight;		// 윈도우의 높이
	CBitmap		m_bitmapBG;		// 배경 비트맵
	HDC			m_dcBkGrnd;		// 배경 DC
	HRGN		m_hRgn;			// 배경 Region 핸든
	


};
