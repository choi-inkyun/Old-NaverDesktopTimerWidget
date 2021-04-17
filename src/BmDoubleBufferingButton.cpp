// BmButton.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "NaverTimer(StopWatch)ByDingpong.h"
#include "BmDoubleBufferingButton.h"

// CBmDoubleBufferingButton

IMPLEMENT_DYNAMIC(CBmDoubleBufferingButton, CButton)
CBmDoubleBufferingButton::CBmDoubleBufferingButton()
{
	m_bHovering		= false;
	m_pOldBitmap	= NULL;
	m_nWidth		= 0;

	m_BmButtonState = en_Button_Normal;
}

CBmDoubleBufferingButton::~CBmDoubleBufferingButton()
{
}


BEGIN_MESSAGE_MAP(CBmDoubleBufferingButton, CButton)
	ON_WM_DESTROY()
	ON_WM_MOUSEMOVE()
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)	
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()


void CBmDoubleBufferingButton::Draw(CDC* pDC)
{
	CRect r, d, c;
	GetClientRect(&r);	
	GetWindowRect(&d);
	GetParent()->GetWindowRect(&c);

	d.left-=c.left;
	d.top-=c.top;

	// ��Ʈ���� ���¼����� normal-hovering-pushed-disable �����̴�.
	// ��Ʈ���� ���¿� ���� �����ϰ� �ִ� DC���� ��Ʈ���� �׷��ش�.
	switch(m_BmButtonState)
	{
	case en_Button_Normal:
		::BitBlt(pDC->m_hDC, d.left, d.top, r.Width(), r.Height(), m_BkDC.m_hDC, 0, 0, SRCCOPY);
		break;

	case en_Button_Hovering:
		::BitBlt(pDC->m_hDC,d.left, d.top, r.Width(), r.Height(), m_BkDC.m_hDC, m_nWidth, 0, SRCCOPY);
		break;

	case en_Button_Pushed:
		::BitBlt(pDC->m_hDC, d.left, d.top, r.Width(), r.Height(), m_BkDC.m_hDC, m_nWidth*2, 0, SRCCOPY);
		break;	

	case en_Button_Disable:
		::BitBlt(pDC->m_hDC, d.left, d.top, r.Width(), r.Height(), m_BkDC.m_hDC, m_nWidth*3, 0, SRCCOPY);
		break;
	}
}

// CBmDoubleBufferingButton �޽��� ó�����Դϴ�.
void CBmDoubleBufferingButton::DrawItem(LPDRAWITEMSTRUCT lpdis)
{
	HDC hDC = lpdis->hDC;	

	CRect r;
	GetClientRect(&r);	

	// ��Ʈ���� ���¼����� normal-hovering-pushed-disable �����̴�.
	// ��Ʈ���� ���¿� ���� �����ϰ� �ִ� DC���� ��Ʈ���� �׷��ش�.
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
// �����Ǳ����� OWNERDRAW �Ӽ��� �ش�.
//////////////////////////////////////////////////////////////////////////
BOOL CBmDoubleBufferingButton::PreCreateWindow(CREATESTRUCT& cs)
{	
	cs.style |= BS_OWNERDRAW;
	return CButton::PreCreateWindow(cs);
}

//////////////////////////////////////////////////////////////////////////
//��ư�� ���¿����� �׷��ֱ����� ��Ʈ�������� DC�� ��
//////////////////////////////////////////////////////////////////////////
void CBmDoubleBufferingButton::SetBkImage(UINT nIDResource, UINT nWidth)
{
	// ��Ʈ�ʿ��� ������ ���¸� ������ ��������
	m_nWidth = nWidth;

	// ��Ʈ���� ������ �����.
	if( m_bitmap.GetSafeHandle())
		m_bitmap.DeleteObject();
	
	// ��Ʈ���� �ε��Ѵ�. 
	m_bitmap.LoadBitmap(nIDResource);
	// ��Ʈ�� ������ ��´�. 
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
// �����۾� ����
//////////////////////////////////////////////////////////////////////////
void CBmDoubleBufferingButton::OnDestroy()
{
	CButton::OnDestroy();

	// DC�� �������ִ� ��Ʈ���� �����Ѵ�.
	if( m_pOldBitmap)
		m_BkDC.SelectObject(m_pOldBitmap);

	// ��Ʈ���� ��ȯ�Ѵ�.
	if( m_bitmap.GetSafeHandle())
		m_bitmap.DeleteObject();

	// DC�� ��ȯ�Ѵ�.
	if( m_BkDC.GetSafeHdc())
		m_BkDC.DeleteDC();
}

//////////////////////////////////////////////////////////////////////////
// ��ư�� ������ ���콺�� ������ �޼����߻� 
// �� ��ư�� Hovering �� ���°� �ȴ�. 
// WM_MOUSELEAVE �޼����� �ޱ����� �̺�Ʈ�� �����Ѵ�.
//////////////////////////////////////////////////////////////////////////
void CBmDoubleBufferingButton::OnMouseMove(UINT nFlags, CPoint point)
{	
	if(!m_bHovering)
	{
		// Hovering �� ���·� �׷��ֱ�.
		m_bHovering = TRUE;
		m_BmButtonState = en_Button_Hovering;
		Invalidate(FALSE);

		// WM_MOUSELEAVE �޼����� �ޱ����� Ʈ���̺�Ʈ ����
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
LRESULT CBmDoubleBufferingButton::OnMouseLeave(WPARAM, LPARAM)
{
	m_bHovering = FALSE;
	m_BmButtonState = en_Button_Normal;

	Invalidate();
	return 1L;
}

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
void CBmDoubleBufferingButton::OnLButtonDown(UINT nFlags, CPoint point)
{
	// ��ư���� pushed ���� ����
	m_BmButtonState = en_Button_Pushed;
	Invalidate();

	CButton::OnLButtonDown(nFlags, point);
}

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
void CBmDoubleBufferingButton::OnLButtonUp(UINT nFlags, CPoint point)
{
	// ��ư���� Hovering ���� ����
	m_BmButtonState = en_Button_Hovering;
	Invalidate();

	CButton::OnLButtonUp(nFlags, point);
}
