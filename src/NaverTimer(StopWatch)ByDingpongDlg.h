//////////////////////////////////////////////////////////////////////////
// File		: NaverTimer(StopWatch)ByDingpongDlg.h
// Author	: 한상철, 최인균
// Role		: 메인 UI Dialog
//			  
//
// Date		: 2007-10-03
//////////////////////////////////////////////////////////////////////////


// NaverTimer(StopWatch)ByDingpongDlg.h : 헤더 파일
//

#pragma once

// DesktopWidget 관련 헤더
#include "DesktopWidget.h"
#include "CWidgetProperties.h"
#include "DesktopWidgetSink.h"
#include "DefineGUID.h"

// Dialgo UI 관련 헤더
#include "DlgBase.h"
#include "ConfigDlg.h"

// 기타 UI 컨트롤 헤더
#include "BmButton.h"
#include "BmDoubleBufferingButton.h"

// CNaverTimerByDingpongDlg 대화 상자
class CNaverTimerByDingpongDlg : public CDialog, 
								private CDlgBase
{
// 생성
public:
	CNaverTimerByDingpongDlg(CWnd* pParent = NULL);	// 표준 생성자

// 대화 상자 데이터
	enum { IDD = IDD_NAVERTIMER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원


// 구현
protected:
	HICON m_hIcon;

	CDesktopWidget		m_DesktopWidget;			// DesktopWidget 객체
	CDesktopWidgetSink	m_DesktopWidgetSink;		// DesktopWidgetSink 객체
	DWORD				m_dwCookie;					// DesktopWidgetSink 와의 쿠키정보

	//BOOL				m_bPlaying;			// Play 중인지 flag

	// UI 초기화 함수
	void	InitUICtrl();
	void	InitButtonCtrl();
	// 버튼 변수
	CBmDoubleBufferingButton			m_btnMenu;
	CBmDoubleBufferingButton			m_btnPlay;
	CBmDoubleBufferingButton			m_btnStop;
	CBmDoubleBufferingButton			m_btnClose;
	CBmDoubleBufferingButton			m_btnHide;

	CBitmap				m_Bitmap;

	CString				nowTimeStr;
	CFont				currentTimeFont;
	DWORD				saveTime;
	bool				nowPlayingCheck; // 현재 타이머가 실행중인지.
	// 슬라이더 초기화 함수및 변수
	void	InitSliderCtrl();
public:
	// 위젯 Property 정보를 로드한다.
	void		LoadConfigData();
	// 위젯 Property 정보를 저장한다.
	void		SaveConfigData();
	// UI 윈도우들을 Topmost로 설정한다.
	void		SetTopmostWindow(BOOL bTopmost);

	// 선택된 노래를 플레이 한다.
	void	StartPlayer();
	// 노래를 스톱한다.
	void	StopPlayer();
	// UI 윈도우들을 Foreground로 설정한다. 
	void	ShowTopWindow();

	

public:	
	// 메시지 맵 함수를 생성했습니다.
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

	/////////////// UI 컨트롤&윈도우 에서 오는 메세지 핸들러 /////////////////////////
	// ConfigDlg : 옵션창 핸들러
	afx_msg LRESULT OnConfigOption(WPARAM wParam, LPARAM lParam);
	// ConfigDlg : 투명도 핸들러
	afx_msg LRESULT OnTransparent(WPARAM wParam, LPARAM lParam);
	// 현재 플레이중인 파일을 삭제시
	afx_msg LRESULT OnStopPlayer(WPARAM wParam, LPARAM lParam);
	// 윈도우를 top으로 보여준다.
	afx_msg LRESULT OnShowTop(WPARAM wParam, LPARAM lParam);

	
	
	//////////////// 메뉴의 Command 핸들러 ////////////////////////////////////////////
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

// 네이버 데스트톱이 실행중인지 확인하는 루틴
inline BOOL KeepAliveNDesktop()
{
	HWND hWnd = ::FindWindow(_T("#32770"), _T("네이버 데스크톱"));
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