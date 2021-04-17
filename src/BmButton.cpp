// BmButton.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "NaverTimer(StopWatch)ByDingpong.h"
#include "BmButton.h"

// CBmButton

IMPLEMENT_DYNAMIC(CBmButton, CButton)
CBmButton::CBmButton()
{
	m_bHovering		= false;
	m_pOldBitmap	= NULL;
	m_nWidth		= 0;

	m_BmButtonState = en_Button_Normal;
}

CBmButton::~CBmButton()
{
}


BEGIN_MESSAGE_MAP(CBmButton, CButton)
	ON_WM_DESTROY()
	ON_WM_MOUSEMOVE()
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)	
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()



// CBmButton 메시지 처리기입니다.

void CBmButton::DrawItem(LPDRAWITEMSTRUCT lpdis)
{
	HDC hDC = lpdis->hDC;	

	CRect r;
	GetClientRect(&r);	

	// 비트맵의 상태순서는 normal-hovering-pushed-disable 순서이다.
	// 비트맵의 상태에 따라서 저장하고 있던 DC에서 비트맵을 그려준다.
	switch(m_BmButtonState)
	{
	case en_Button_Normal:
		::BitBlt(hDC, 0, 0, r.Width(), r.Height(), m_BkDC.m_hDC, 0, 0, SRCCOPY);
		break;

	case en_Button_Hovering:
		::BitBlt(hDC, 0, 0, r.Width(), r.Height(), m_BkDC.m_hDC, m_nWidth, 0, SRCCOPY);
		break;

	case en_Button_Pushed:
		::BitBlt(hDC, 0, 0, r.Width(), r.Height(), m_BkDC.m_hDC, m_nWidth*2, 0, SRCCOPY);
		break;	

	case en_Button_Disable:
		::BitBlt(hDC, 0, 0, r.Width(), r.Height(), m_BkDC.m_hDC, m_nWidth*3, 0, SRCCOPY);
		break;
	}
}

//////////////////////////////////////////////////////////////////////////
// 생성되기전에 OWNERDRAW 속성을 준다.
//////////////////////////////////////////////////////////////////////////
BOOL CBmButton::PreCreateWindow(CREATESTRUCT& cs)
{	
	cs.style |= BS_OWNERDRAW;
	return CButton::PreCreateWindow(cs);
}

//////////////////////////////////////////////////////////////////////////
//버튼의 상태에따라서 그려주기위한 비트맵정보를 DC에 저
//////////////////////////////////////////////////////////////////////////
void CBmButton::SetBkImage(UINT nIDResource, UINT nWidth)
{
	// 비트맵에서 각각의 상태를 구분할 길이정보
	m_nWidth = nWidth;

	// 비트맵이 있으면 지운다.
	if( m_bitmap.GetSafeHandle())
		m_bitmap.DeleteObject();
	
	// 비트맵을 로드한다. 
	m_bitmap.LoadBitmap(nIDResource);
	// 비트맵 정보를 얻는다. 
	m_bitmap.GetBitmap(&m_bitInfo);

	CDC* pDC;
	pDC = GetDC();
	if( m_BkDC.GetSafeHdc() )
		m_BkDC.DeleteDC();
	m_BkDC.CreateCompatibleDC(pDC);
	m_pOldBitmap = m_BkDC.SelectObject(&m_bitmap);

	ReleaseDC(pDC);
}

//////////////////////////////////////////////////////////////////////////
// 종료작업 수행
//////////////////////////////////////////////////////////////////////////
void CBmButton::OnDestroy()
{
	CButton::OnDestroy();

	// DC가 가지고있던 비트맵을 해제한다.
	if( m_pOldBitmap)
		m_BkDC.SelectObject(m_pOldBitmap);

	// 비트맵을 반환한다.
	if( m_bitmap.GetSafeHandle())
		m_bitmap.DeleteObject();

	// DC를 반환한다.
	if( m_BkDC.GetSafeHdc())
		m_BkDC.DeleteDC();
}

//////////////////////////////////////////////////////////////////////////
// 버튼의 영역에 마우스가 있으면 메세지발생 
// 즉 버튼의 Hovering 된 상태가 된다. 
// WM_MOUSELEAVE 메세지를 받기위한 이벤트를 설정한다.
//////////////////////////////////////////////////////////////////////////
void CBmButton::OnMouseMove(UINT nFlags, CPoint point)
{	
	if(!m_bHovering)
	{
		// Hovering 된 상태로 그려주기.
		m_bHovering = TRUE;
		m_BmButtonState = en_Button_Hovering;
		Invalidate(FALSE);

		// WM_MOUSELEAVE 메세지를 받기위한 트랙이벤트 설정
		TRACKMOUSEEVENT tmEvent;
		tmEvent.cbSize = sizeof(tmEvent);
		tmEvent.dwFlags = TME_LEAVE;
		tmEvent.hwndTrack = m_hWnd;

		::_TrackMouseEvent(&tmEvent);
	}	

	CButton::OnMouseMove(nFlags, point);
}

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
LRESULT CBmButton::OnMouseLeave(WPARAM, LPARAM)
{
	m_bHovering = FALSE;
	m_BmButtonState = en_Button_Normal;

	Invalidate();
	return 1L;
}

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
void CBmButton::OnLButtonDown(UINT nFlags, CPoint point)
{
	// 버튼상태 pushed 으로 변경
	m_BmButtonState = en_Button_Pushed;
	Invalidate();

	CButton::OnLButtonDown(nFlags, point);
}

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
void CBmButton::OnLButtonUp(UINT nFlags, CPoint point)
{
	// 버튼상태 Hovering 으로 변경
	m_BmButtonState = en_Button_Hovering;
	Invalidate();

	CButton::OnLButtonUp(nFlags, point);
}
