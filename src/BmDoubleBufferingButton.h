//////////////////////////////////////////////////////////////////////////
// File		: CBmDoubleBufferingButton.h
// Author	: �ѻ�ö 
// Role		: 4������ ���¸� �����Ҽ� �ִ�.Normal, Hover, Pushed, Diable
//			  �����Ǵ� ��Ʈ���� 4������ ���°� �پ��ִ� �����̾�� �Ѵ�.
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
	
	// ��ư�� ��Ʈ���� �����Ѵ�. 
	// nResourceID : ���ҽ����̵� (��Ʈ���� Normal-Hover-Pushed-Disable�Ǽ����ε� ��Ʈ��)
	// nWidth : ��Ʈ�ʿ��� �ѻ����϶��� ���� 
	void	SetBkImage(UINT nResourceID, UINT nWidth);	


private:

	// ��ư�� ����
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


