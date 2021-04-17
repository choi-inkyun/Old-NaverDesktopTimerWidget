//////////////////////////////////////////////////////////////////////////
// File		: DlgBase.h
// Author	: �ѻ�ö 
// Role		: ���̾�α��� Base Ŭ����
//			  ������ ���� ����, ��纯��, ����ǥ���ٵ��� ����� �ִ�.
//
// Date		: 2007-06-04
//////////////////////////////////////////////////////////////////////////



#pragma once


class CDlgBase
{
public:
	CDlgBase(void);
	~CDlgBase(void);


	//	������ ���� ����
	// nAlpha : 0~255�� ������ �����ϰ� ����
	BOOL	SetWindowTransparent(HWND hWnd, BYTE nAlpha);

	//  ������ ��� ����
	//  nIDResource : �������� ����� �Ǵ� ��Ʈ��
	BOOL	SetWindowRegion(HWND hWnd, UINT nIDResource);

	//  ������ ����ǥ���� �����
	BOOL	HideApplicationTitlebar(HWND hWnd);	

	


protected:

	// ��Ʈ������ Region ����
	HRGN CreateRgnFromFile(HBITMAP hBmp, COLORREF color);
	DWORD		m_dwWidth;		// �������� ����
	DWORD		m_dwHeight;		// �������� ����
	CBitmap		m_bitmapBG;		// ��� ��Ʈ��
	HDC			m_dcBkGrnd;		// ��� DC
	HRGN		m_hRgn;			// ��� Region �ڵ�
	


};
