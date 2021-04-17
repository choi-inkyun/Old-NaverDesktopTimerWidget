//////////////////////////////////////////////////////////////////////////
// File		: CBmDoubleBufferingButton.h
// Author	: 한상철 
// Role		: 4가지의 상태를 설정할수 있다.Normal, Hover, Pushed, Diable
//			  설정되는 비트맵은 4가지의 상태가 붙어있는 형태이어야 한다.
//
// Date		: 2007-06-04
//////////////////////////////////////////////////////////////////////////


#pragma once

class CBmDoubleBufferingButton : public CButton
{
	DECLARE_DYNAMIC(CBmDoubleBufferingButton)

public:
	CBmDoubleBufferingButton();
	virtual ~CBmDoubleBufferingButton();
	
	// 버튼의 비트맵을 설정한다. 
	// nResourceID : 리소스아이디 (비트맵은 Normal-Hover-Pushed-Disable의순서로된 비트맵)
	// nWidth : 비트맵에서 한상태일때의 길이 
	void	SetBkImage(UINT nResourceID, UINT nWidth);	


private:

	// 버튼의 상태
	enum	
	{	en_Button_Normal = 1,
		en_Button_Hovering,
		en_Button_Pushed,
		en_Button_Disable

	} m_BmButtonState;


	CBitmap		m_bitmap;
	CBitmap*	m_pOldBitmap;
	BITMAP		m_bitInfo;

	CDC			m_BkDC;
	UINT		m_nWidth;

	BOOL		m_bHovering;	

	LRESULT		OnMouseLeave(WPARAM, LPARAM);	

protected:

	void DrawItem(LPDRAWITEMSTRUCT lpdis);


	DECLARE_MESSAGE_MAP()
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
public:
	afx_msg void OnDestroy();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);

	void Draw(CDC* pDC);
};


