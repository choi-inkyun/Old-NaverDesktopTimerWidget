//////////////////////////////////////////////////////////////////////////
// File		: NaverTimer(StopWatch)ByDingpongDlg.h
// Author	: �ѻ�ö, ���α�
// Role		: ���� UI Dialog
//			  
//
// Date		: 2007-10-03
//////////////////////////////////////////////////////////////////////////


// NaverTimer(StopWatch)ByDingpongDlg.h : ��� ����
//

#pragma once

// DesktopWidget ���� ���
#include "DesktopWidget.h"
#include "CWidgetProperties.h"
#include "DesktopWidgetSink.h"
#include "DefineGUID.h"

// Dialgo UI ���� ���
#include "DlgBase.h"
#include "ConfigDlg.h"

// ��Ÿ UI ��Ʈ�� ���
#include "BmButton.h"
#include "BmDoubleBufferingButton.h"

// CNaverTimerByDingpongDlg ��ȭ ����
class CNaverTimerByDingpongDlg : public CDialog, 
								private CDlgBase
{
// ����
public:
	CNaverTimerByDingpongDlg(CWnd* pParent = NULL);	// ǥ�� ������

// ��ȭ ���� ������
	enum { IDD = IDD_NAVERTIMER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ����


// ����
protected:
	HICON m_hIcon;

	CDesktopWidget		m_DesktopWidget;			// DesktopWidget ��ü
	CDesktopWidgetSink	m_DesktopWidgetSink;		// DesktopWidgetSink ��ü
	DWORD				m_dwCookie;					// DesktopWidgetSink ���� ��Ű����

	//BOOL				m_bPlaying;			// Play ������ flag

	// UI �ʱ�ȭ �Լ�
	void	InitUICtrl();
	void	InitButtonCtrl();
	// ��ư ����
	CBmDoubleBufferingButton			m_btnMenu;
	CBmDoubleBufferingButton			m_btnPlay;
	CBmDoubleBufferingButton			m_btnStop;
	CBmDoubleBufferingButton			m_btnClose;
	CBmDoubleBufferingButton			m_btnHide;

	CBitmap				m_Bitmap;

	CString				nowTimeStr;
	CFont				currentTimeFont;
	DWORD				saveTime;
	bool				nowPlayingCheck; // ���� Ÿ�̸Ӱ� ����������.
	// �����̴� �ʱ�ȭ �Լ��� ����
	void	InitSliderCtrl();
public:
	// ���� Property ������ �ε��Ѵ�.
	void		LoadConfigData();
	// ���� Property ������ �����Ѵ�.
	void		SaveConfigData();
	// UI ��������� Topmost�� �����Ѵ�.
	void		SetTopmostWindow(BOOL bTopmost);

	// ���õ� �뷡�� �÷��� �Ѵ�.
	void	StartPlayer();
	// �뷡�� �����Ѵ�.
	void	StopPlayer();
	// UI ��������� Foreground�� �����Ѵ�. 
	void	ShowTopWindow();

	

public:	
	// �޽��� �� �Լ��� �����߽��ϴ�.
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnDestroy();
	afx_msg void OnMove(int x, int y);
	afx_msg void OnBnClickedMenu();
	afx_msg void OnBnClickedPlay();
	afx_msg void OnBnClickedStop();
	afx_msg void OnBnClickedClose();
	afx_msg void OnBnClickedHide();

	/////////////// UI ��Ʈ��&������ ���� ���� �޼��� �ڵ鷯 /////////////////////////
	// ConfigDlg : �ɼ�â �ڵ鷯
	afx_msg LRESULT OnConfigOption(WPARAM wParam, LPARAM lParam);
	// ConfigDlg : ���� �ڵ鷯
	afx_msg LRESULT OnTransparent(WPARAM wParam, LPARAM lParam);
	// ���� �÷������� ������ ������
	afx_msg LRESULT OnStopPlayer(WPARAM wParam, LPARAM lParam);
	// �����츦 top���� �����ش�.
	afx_msg LRESULT OnShowTop(WPARAM wParam, LPARAM lParam);

	
	
	//////////////// �޴��� Command �ڵ鷯 ////////////////////////////////////////////
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnAboutDesktop();
	afx_msg void OnConfig();
	afx_msg void OnDesktopHide();
	afx_msg void OnExit();
	afx_msg void OnPositionFix();
	afx_msg void OnTopmost();

	DECLARE_MESSAGE_MAP()

protected:
	virtual void OnOK();
	virtual void OnCancel();
public:
	
	virtual BOOL PreTranslateMessage(MSG* pMsg);
private:
	void _Draw(CDC* pDC);
	void _DrawText(CDC* pDC);

	void DrawDoubleBuffering(void);


};

// ���̹� ����Ʈ���� ���������� Ȯ���ϴ� ��ƾ
inline BOOL KeepAliveNDesktop()
{
	HWND hWnd = ::FindWindow(_T("#32770"), _T("���̹� ����ũ��"));
	if( FALSE == ::IsWindow(hWnd) )
		return FALSE;

	return TRUE;
}

inline void ForceForegroundWindow(HWND hWnd)
{
	if(hWnd == NULL) return;

	//	DWORD SPI_GETFOREGROUNDLOCKTIMEOUT = 0x2000;
	//	const DWORD SPI_SETFOREGROUNDLOCKTIMEOUT = 0x2001;

	DWORD ForegroundThreadID;
	DWORD ThisThreadID;
	DWORD Timeout;
	BOOL Result;

	if(GetForegroundWindow() == hWnd) {
		return;
	}
	else {
		// Windows 98/2000 doesn't want to foreground a window when some other
		// window has keyboard focus

		OSVERSIONINFO os;
		os.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
		GetVersionEx(&os);
		if((os.dwPlatformId == VER_PLATFORM_WIN32_NT && os.dwMajorVersion > 4) ||
			(os.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS && 
			(os.dwMajorVersion > 4 || (os.dwMajorVersion == 4 && os.dwMinorVersion > 0)))){

				Result = FALSE;
				ForegroundThreadID = GetWindowThreadProcessId(GetForegroundWindow(), NULL);
				ThisThreadID = GetWindowThreadProcessId(hWnd, NULL);
				if(AttachThreadInput(ThisThreadID, ForegroundThreadID, TRUE)) {
					BringWindowToTop(hWnd);
					SetForegroundWindow(hWnd);
					AttachThreadInput(ThisThreadID, ForegroundThreadID, FALSE);
					Result = GetForegroundWindow() == hWnd;
				}

				if(!Result) {
					SystemParametersInfo(0x2000, 0, &Timeout, 0);
					SystemParametersInfo(0x2001, 0, NULL, SPIF_SENDCHANGE);
					BringWindowToTop(hWnd);
					SetForegroundWindow(hWnd);
					SystemParametersInfo(0x2001, 0, &Timeout, SPIF_SENDCHANGE);
				}
			}
		else
		{
			BringWindowToTop(hWnd);
			SetForegroundWindow(hWnd);
		}
	}
}