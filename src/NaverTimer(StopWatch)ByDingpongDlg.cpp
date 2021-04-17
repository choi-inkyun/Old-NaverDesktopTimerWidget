#include "stdafx.h"
#include "NaverTimer(StopWatch)ByDingpong.h"
#include "NaverTimer(StopWatch)ByDingpongDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// Timer ID
#define TIMER_PLAY					100
#define TIMER_MAIN_PROCESS_CHECK	200

// UI Control ID
#define IDC_BTN_MENU				1000
#define IDC_BTN_FORWARD				1001
#define IDC_BTN_BACKWARD			1002
#define IDC_BTN_PLAY				1004
#define IDC_BTN_STOP				1005
#define IDC_BTN_VOLUME				1006
#define IDC_BTN_FILE				1007
#define IDC_BTN_LIST				1008
#define IDC_BTN_CLOSE				1009
#define IDC_BTN_HIDE				1010
#define IDC_SLIDER_VOLUME			1101
#define IDC_SLIDER_SEEK				1102


// CNaverTimerByDingpongDlg 대화 상자
CNaverTimerByDingpongDlg::CNaverTimerByDingpongDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CNaverTimerByDingpongDlg::IDD, pParent)
{
	EnableActiveAccessibility();
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	// global config data
	g_ConfigData.bAlwaysTop		= FALSE;
	g_ConfigData.bPositionFix	= FALSE;
	g_ConfigData.bShowWidget	= TRUE;
	g_ConfigData.nTransparent	= 255;

	saveTime = 0;
	nowPlayingCheck = false;
	nowTimeStr = "";
}

void CNaverTimerByDingpongDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CNaverTimerByDingpongDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP	
	ON_WM_DESTROY()
	//ON_BN_CLICKED(IDC_BTN_FORWARD, OnBnClickedForward)
	//ON_BN_CLICKED(IDC_BTN_BACKWARD, OnBnClickedBackward)
	//ON_BN_CLICKED(IDC_BTN_MENU, OnBnClickedMenu)
	//ON_BN_CLICKED(IDC_BTN_PLAY, OnBnClickedPlay)
	//ON_BN_CLICKED(IDC_BTN_STOP, OnBnClickedStop)
	//ON_BN_CLICKED(IDC_BTN_VOLUME, OnBnClickedVolume)
	//ON_BN_CLICKED(IDC_BTN_FILE, OnBnClickedFile)
	//ON_BN_CLICKED(IDC_BTN_LIST, OnBnClickedList)
	//ON_BN_CLICKED(IDC_BTN_CLOSE, OnBnClickedClose)
	//ON_BN_CLICKED(IDC_BTN_HIDE, OnBnClickedHide)
	ON_WM_TIMER()
	ON_WM_MOVE()
	ON_MESSAGE(UM_CONFIG_OPTION, OnConfigOption)
	ON_MESSAGE(UM_TRANSPARENT, OnTransparent)
	ON_MESSAGE(UM_STOP_PLAYER, OnStopPlayer)
	ON_MESSAGE(UM_SHOW_TOP, OnShowTop)
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_COMMAND(IDM_ABOUT_DESKTOP, OnAboutDesktop)
	ON_COMMAND(IDM_CONFIG, OnConfig)
	ON_COMMAND(IDM_DESKTOP_HIDE, OnDesktopHide)
	ON_COMMAND(IDM_EXIT, OnExit)
	ON_COMMAND(IDM_POSITION_FIX, OnPositionFix)
	ON_COMMAND(IDM_TOPMOST, OnTopmost)	
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
void CNaverTimerByDingpongDlg::ShowTopWindow()
{
	ForceForegroundWindow(m_hWnd);
	//BringWindowToTop();
	//SetForegroundWindow();

	if( g_ConfigData.bShowWidget )		ShowWindow(SW_SHOW);

}

//////////////////////////////////////////////////////////////////////////
// 초기화 작업
//////////////////////////////////////////////////////////////////////////
BOOL CNaverTimerByDingpongDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 이 대화 상자의 아이콘을 설정합니다. 응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	// 프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// 제목표시줄 숨기기
	CDlgBase::HideApplicationTitlebar(m_hWnd);	
	// 윈도우 모양설정
	CDlgBase::SetWindowRegion(m_hWnd, IDB_PLAYER_BG);

	// 버튼, 볼륨, 진행정보등 UI 컨트롤을 생성한다. 
	InitUICtrl();

	// 데스크톱비정상종료체크 타이머
	SetTimer(TIMER_MAIN_PROCESS_CHECK, 5, NULL);

	COleException error;
	TCHAR szError[255] = {0};	
	if( m_DesktopWidget.CreateDispatch("NBoard.DesktopWidget", &error) )
	{
		//위젯의 프로세스 ID, WidgetID, 할당받은 WidgetKey를 전달		
		//dingpong. 이것때문에 많이 헤맴-ㅁ-; 위젯아이디를 정상적으로 셋팅하지 않으면 실행할 수 없음. .ndw 와 .nd 가 다르다는 말은 .exe 를 .ndw 로 바꾸기 때문에
		//코드 안에 어딘가 아이디가 있을 것이라 생각했는데.. 상수로 지정되어 있을 것이라 예상한 것과는 달리 이렇게 하드 코딩 되어있음-ㅁ-;
		m_DesktopWidget.SetWidgetInfo(GetCurrentProcessId(), 2661, "bb4d613de682a4f9b632b19e54248d77");
	}
	else
	{
		error.GetErrorMessage(szError, 255);
		::OutputDebugString(szError);		
	}

	// 위젯이 정상 실행되었음을 데스크톱 메인프로세스에게 알립니다. 
	m_DesktopWidget.WidgetConnected();

	// 데스크톱 이벤트싱크 설정
	m_DesktopWidgetSink.m_pMainDlg = this;
	LPUNKNOWN pUnkSink = m_DesktopWidgetSink.GetIDispatch(FALSE);
	AfxConnectionAdvise(m_DesktopWidget.m_lpDispatch, IID_IDesktopWidgetEvents, pUnkSink, FALSE, &m_dwCookie);

	// 환경설정 데이터를 읽어온다. (WidgetProperties)
	LoadConfigData();


	SetTopmostWindow(g_ConfigData.bAlwaysTop);		// 항상위 설정
	OnTransparent(0, g_ConfigData.nTransparent);	// 투명도 설정

	DrawDoubleBuffering();

	return TRUE;  // 컨트롤에 대한 포커스를 설정하지 않을 경우 TRUE를 반환합니다.
}

// 더블 버퍼링을 이용하여 그리기 : 깜박임이 없다. 시간 지연이 있다.
// MemDC : 가상 화면 DC
// m_Bitmap : 가상 화면 DC 에 그릴 도화지
void CNaverTimerByDingpongDlg::DrawDoubleBuffering(void)
{
     CClientDC dc(this);
 
     CRect rect;
     GetClientRect(&rect);

     CDC   MemDC;
     CBitmap* pOldBmp;

     // 도화지를 현재 스크린 DC와 일치 시킨다.

     m_Bitmap.DeleteObject();
     m_Bitmap.CreateCompatibleBitmap(&dc, rect.Width(), rect.Height());
    
     // 가상 화면 DC를 현재 스크린 DC와 일치 시킨다.
     MemDC.CreateCompatibleDC(&dc);

     // 가상 화면 DC 에 도화지를 깐다.
     pOldBmp = MemDC.SelectObject(&m_Bitmap);

     // 가상 화면 DC를 흰색으로 설정한다.
     MemDC.PatBlt(0,0,rect.Width(), rect.Height(), WHITENESS);
    
 	 BitBlt( MemDC, 0, 0, m_dwWidth, m_dwHeight, CDlgBase::m_dcBkGrnd, 0, 0, SRCCOPY );

	 // Text를 구려준다.
	 _DrawText(&MemDC);	

	 m_btnMenu.Draw(&MemDC);
	 m_btnPlay.Draw(&MemDC);
	 m_btnStop.Draw(&MemDC);
	 m_btnClose.Draw(&MemDC);
	 m_btnHide.Draw(&MemDC);

     // 가상화면 출력을 위해 OnDraw로 간다.
     Invalidate();

     MemDC.SelectObject(pOldBmp);
     MemDC.DeleteDC();
}



// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면 
// 아래 코드가 필요합니다. 문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
// 프레임워크에서 이 작업을 자동으로 수행합니다.
void CNaverTimerByDingpongDlg::OnPaint() 
{
	CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트
	if (IsIconic())
	{
		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		// 배경그려주기
		if( m_dcBkGrnd )
		{
//			BitBlt( dc.m_hDC, 0, 0, m_dwWidth, m_dwHeight, CDlgBase::m_dcBkGrnd, 0, 0, SRCCOPY );
		}

		CDialog::OnPaint();
	}
	// 화면을 그려준다.
	_Draw(&dc);
}

void CNaverTimerByDingpongDlg::_Draw(CDC* pDC)
{
    CDC dc;
    CRect rect;
    GetClientRect(&rect);

	// 비트맵 출력
    dc.CreateCompatibleDC(pDC);
    dc.SelectObject(&m_Bitmap);

	pDC->BitBlt(0,0,rect.Width(), rect.Height(), &dc, 0, 0, SRCCOPY);

}
void CNaverTimerByDingpongDlg::_DrawText(CDC* pDC)
{
	CRect rText;
	GetClientRect(rText);
		
	rText.DeflateRect(3, 3);		// 상하여백 3 픽셀
	rText.left	= 80;
	rText.right	= rText.left + 150;

	// dingpong. 핵심적인 위젯 타이머 계산 루틴. 이렇게 세부적으로 까지 할 필요는 없긴 하지만 도움이 될 사람이 있기 때문에.
	if(nowPlayingCheck == true)
	{
		DWORD nowTime = GetTickCount();
		nowTime-=saveTime;

		int min = nowTime / 60000;
		nowTime = nowTime % 60000;
		int sec = nowTime / 1000;
		nowTime = nowTime % 1000;
		int mili = nowTime / 10;
		nowTime = nowTime % 10;

		nowTimeStr.Format("%d  :  %d  :  %d   : %d", min, sec, mili, nowTime);
	}

	CFont* pOldFont = (CFont*)pDC->SelectObject(&currentTimeFont);
	pDC->SetBkMode(TRANSPARENT); // 배경은 투명으로 셋팅한다. 안그러면 흰색 등이 나올 수 있다.
	pDC->DrawText(nowTimeStr, rText, DT_LEFT | DT_SINGLELINE | DT_VCENTER | DT_END_ELLIPSIS); // 실제 텍스트를 그림.
	pDC->SelectObject(pOldFont);	
}
//////////////////////////////////////////////////////////////////////////
// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다. 
//////////////////////////////////////////////////////////////////////////
HCURSOR CNaverTimerByDingpongDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//////////////////////////////////////////////////////////////////////////
// 종료작업
//////////////////////////////////////////////////////////////////////////
void CNaverTimerByDingpongDlg::OnDestroy()
{
	__super::OnDestroy();

	currentTimeFont.DeleteObject();

	StopPlayer();
	KillTimer(TIMER_MAIN_PROCESS_CHECK);

	// WidgetPropertie 정보들을 저장한다.
	SaveConfigData();
	
	// 이벤트 싱크 종료
	LPUNKNOWN pUnkSink = m_DesktopWidgetSink.GetIDispatch(FALSE);
	AfxConnectionUnadvise(m_DesktopWidget.m_lpDispatch, IID_IDesktopWidgetEvents, pUnkSink, FALSE, m_dwCookie);

	m_DesktopWidget.ReleaseDispatch();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}

//////////////////////////////////////////////////////////////////////////
// 윈도우 이동이 기타 윈도우도 같이 이동
//////////////////////////////////////////////////////////////////////////
void CNaverTimerByDingpongDlg::OnMove(int x, int y)
{
	__super::OnMove(x, y);
}

//////////////////////////////////////////////////////////////////////////
// UI 컨트롤을 생성하고 초기화
//////////////////////////////////////////////////////////////////////////
void CNaverTimerByDingpongDlg::InitUICtrl()
{
	currentTimeFont.CreatePointFont(80, _T("MS Sans Serif"));
	InitButtonCtrl();
}

//////////////////////////////////////////////////////////////////////////
// 버튼들을 생성한다.
//////////////////////////////////////////////////////////////////////////
void CNaverTimerByDingpongDlg::InitButtonCtrl()
{
	DWORD dwStyle = WS_VISIBLE | WS_CHILD | BS_BITMAP;

	CRect r;
	CRect rMenu, rPlay, rStop, rClose, rHide;

	GetClientRect(&r);

	rMenu = r;
	rMenu.left = 4;
	rMenu.top = 10;
	rMenu.right = rMenu.left + 27;
	rMenu.bottom = rMenu.top + 14;

	rPlay.top	= 7;
	rPlay.bottom= rPlay.top + 19;
	rPlay.left	= 31+7+143+7  + 5;
	rPlay.right = rPlay.left+18;

	rStop = rPlay;

	rStop.left	= rPlay.right;
	rStop.right = rStop.left + 19;

	rClose.left		= rStop.right+6;
	rClose.right	= rClose.left+12;
	rClose.top		= 4;
	rClose.bottom	= rClose.top+11;

	rHide = rClose;
	rHide.top	= rClose.bottom +1;
	rHide.bottom= rHide.top+11;

	// create buttons
	m_btnMenu.Create(_T(""), dwStyle, rMenu, this, IDC_BTN_MENU);
	m_btnPlay.Create(_T(""), dwStyle, rPlay, this, IDC_BTN_PLAY);
	m_btnStop.Create(_T(""), dwStyle, rStop, this, IDC_BTN_STOP);
	m_btnClose.Create(_T(""), dwStyle, rClose, this, IDC_BTN_CLOSE);	
	m_btnHide.Create(_T(""), dwStyle, rHide, this, IDC_BTN_HIDE);	

	// load bitmaps	
	m_btnMenu.SetBkImage(IDB_MENU_BUTTON_SET, 27);
	m_btnPlay.SetBkImage(IDB_PLAY_BUTTON_SET, 19);
	m_btnStop.SetBkImage(IDB_STOP_BUTTON_SET, 19);
	m_btnClose.SetBkImage(IDB_PLAYER_CLOSE_SET, 12);
	m_btnHide.SetBkImage(IDB_PLAYER_MINI_SET, 12);
}

//////////////////////////////////////////////////////////////////////////
// Menu버튼 핸들러 : Popup메뉴를 보여준다.
//////////////////////////////////////////////////////////////////////////
void CNaverTimerByDingpongDlg::OnBnClickedMenu()
{
	CPoint ptPoint;
	GetCursorPos(&ptPoint);

	CMenu menuPopup;
	menuPopup.LoadMenu(IDR_MENU_POPUP);

	menuPopup.CheckMenuItem( IDM_TOPMOST, g_ConfigData.bAlwaysTop ? MF_CHECKED : MF_UNCHECKED | MF_BYCOMMAND);
	menuPopup.CheckMenuItem( IDM_POSITION_FIX, g_ConfigData.bPositionFix ? MF_CHECKED : MF_UNCHECKED | MF_BYCOMMAND);

	menuPopup.GetSubMenu(0)->TrackPopupMenu(TPM_LEFTALIGN | TPM_TOPALIGN | TPM_LEFTBUTTON, ptPoint.x, ptPoint.y, this);

}

//////////////////////////////////////////////////////////////////////////
// Play 버튼 핸들러
//////////////////////////////////////////////////////////////////////////
void CNaverTimerByDingpongDlg::OnBnClickedPlay()
{	
	StartPlayer();
}

//////////////////////////////////////////////////////////////////////////
// Stop 버튼 핸들러
//////////////////////////////////////////////////////////////////////////
void CNaverTimerByDingpongDlg::OnBnClickedStop()
{
	StopPlayer();
	m_btnPlay.SetBkImage(IDB_PLAY_BUTTON_SET, 19);
	m_btnPlay.Invalidate();
}

//////////////////////////////////////////////////////////////////////////
// Close 버튼 핸들러 : 종료한다.
//////////////////////////////////////////////////////////////////////////
void CNaverTimerByDingpongDlg::OnBnClickedClose()
{	
	EndDialog(0);
}

//////////////////////////////////////////////////////////////////////////
// Hide버튼 핸들러 : 전체를 숨긴다.
//////////////////////////////////////////////////////////////////////////
void CNaverTimerByDingpongDlg::OnBnClickedHide()
{
	ShowWindow(SW_HIDE);
}

//////////////////////////////////////////////////////////////////////////
// 선택된 노래를 플레이 한다.
//////////////////////////////////////////////////////////////////////////
void CNaverTimerByDingpongDlg::StartPlayer()
{
	saveTime = GetTickCount();
	nowPlayingCheck = true;
}

//////////////////////////////////////////////////////////////////////////
// 노래를 멈춘다.
//////////////////////////////////////////////////////////////////////////
void CNaverTimerByDingpongDlg::StopPlayer()
{
	// 타이머 종료
	KillTimer(TIMER_PLAY);

	nowPlayingCheck = false;
}

//////////////////////////////////////////////////////////////////////////
// ListDlg에서 선택된 파일을 삭제 했을때
//////////////////////////////////////////////////////////////////////////
LRESULT CNaverTimerByDingpongDlg::OnStopPlayer(WPARAM wParam, LPARAM lParam)
{
	OnBnClickedStop();	
	return 1L;
}

//////////////////////////////////////////////////////////////////////////
// 윈도우를 top으로 보여준다. (list, volume 에서 온다. )
//////////////////////////////////////////////////////////////////////////
LRESULT CNaverTimerByDingpongDlg::OnShowTop(WPARAM wParam, LPARAM lParam)
{
	ShowTopWindow();
	return 1L;
}

//////////////////////////////////////////////////////////////////////////
// 옵션창을 띄운다.
//////////////////////////////////////////////////////////////////////////
LRESULT CNaverTimerByDingpongDlg::OnConfigOption(WPARAM wParam, LPARAM lParam)
{
	CConfigDlg	dlg;

	//ShowWindow(SW_SHOW);

	if( dlg.DoModal() == IDOK)
	{
		// 항상위 설정
		SetTopmostWindow(g_ConfigData.bAlwaysTop);		
		// 투명도 설정
		OnTransparent(0, g_ConfigData.nTransparent);
	}

	return 1L;
}

//////////////////////////////////////////////////////////////////////////
// 타이머 핸들러
//////////////////////////////////////////////////////////////////////////
void CNaverTimerByDingpongDlg::OnTimer(UINT nIDEvent)
{	
	// 플레이 1초간격
	if( nIDEvent == TIMER_PLAY )
	{

	}
	// 메인 Desktop이 살아있는지 체크 
	else if( nIDEvent == TIMER_MAIN_PROCESS_CHECK )
	{
	}

	if(nowPlayingCheck == true)
		DrawDoubleBuffering();

	__super::OnTimer(nIDEvent);
}

//////////////////////////////////////////////////////////////////////////
// 투명도 설정 : 다른 윈도우도 함깨 적용
//////////////////////////////////////////////////////////////////////////
LRESULT CNaverTimerByDingpongDlg::OnTransparent(WPARAM wParam, LPARAM lParam)
{	
	BYTE nAlpha = static_cast<BYTE>(lParam);

	if( nAlpha < 20 || nAlpha > 255)	return 0L;

	// 메인 윈도우
	CDlgBase::SetWindowTransparent(m_hWnd, nAlpha);
	
	// 윈도우들이 않보여서 설정에 따라서 보이게
	::ShowWindow(m_hWnd, SW_SHOW);	

	return 1L;
}

//////////////////////////////////////////////////////////////////////////
// WidgetProperty 로드하기
//////////////////////////////////////////////////////////////////////////
void CNaverTimerByDingpongDlg::LoadConfigData()
{		
	int nLeft = 0, nTop = 0, nRight = 0, nBottom = 0;
	char chMainShow, chAlwaysTop, chPositionFix;	
	CString szWidgetSize, szWidgetPosition, szConfig, szTransparent;

	// WidgetProperties 객체 얻어오기
	VARIANT val;
	val = m_DesktopWidget.get_WidgetProperties(_T(""), WIDGET_ID);
	CWidgetProperties widgetProperties(val.pdispVal);


	chMainShow = widgetProperties.get_WidgetMainShow();

	// 항상위
	chAlwaysTop = widgetProperties.get_WidgetTopMost();
	g_ConfigData.bAlwaysTop = (chAlwaysTop == 'Y');

	// 위치고정
	chPositionFix = widgetProperties.get_WidgetPositionFix();
	g_ConfigData.bPositionFix = (chPositionFix == 'Y');
	
	// 투명도
	szTransparent = widgetProperties.get_WidgetTransparent();
	g_ConfigData.nTransparent = atoi(szTransparent);
	if( g_ConfigData.nTransparent < 20 )
		g_ConfigData.nTransparent = 255;
	
	// 위치, 크기
	szWidgetSize = widgetProperties.get_WidgetSize();
	szWidgetPosition = widgetProperties.get_WidgetPosition();
		
	int nFInd = szWidgetPosition.Find(",");
	if(nFInd != -1)
	{
		nLeft = atoi(szWidgetPosition.Left(nFInd));
		szWidgetPosition = szWidgetPosition.Mid(nFInd+1);
		nTop = atoi(szWidgetPosition);
	}

	nFInd = szWidgetSize.Find("*");
	if(nFInd != -1)
	{
		nRight = atoi(szWidgetSize.Left(nFInd)) + nLeft;
		szWidgetSize = szWidgetSize.Mid(nFInd+1);
		nBottom = atoi(szWidgetSize) + nTop;
	}

	// 위치크기로 현재의 윈도우 이동
	if (nRight && nBottom)
	{
		::MoveWindow(m_hWnd, nLeft, nTop, nRight-nLeft, nBottom-nTop, TRUE);
		g_ConfigData.bShowWidget = (chMainShow == 'N') ? FALSE:TRUE;
	}

}

//////////////////////////////////////////////////////////////////////////
// WidgetProperty 저장하기
//////////////////////////////////////////////////////////////////////////
void CNaverTimerByDingpongDlg::SaveConfigData()
{	
	// WidgetProperties 객체 생성
	COleException error;
	CWidgetProperties widgetProperties;
	widgetProperties.CreateDispatch("NBoard.WidgetProperties", &error);

	widgetProperties.put_WidgetDataType(1); 
	widgetProperties.put_WidgetType(5);
	widgetProperties.put_WidgetOrder(0);
	widgetProperties.put_WidgetViewType('F');

	// 보이기 
	(g_ConfigData.bShowWidget) ? widgetProperties.put_WidgetMainShow('Y') : widgetProperties.put_WidgetMainShow('N');
	// 항상위
	(g_ConfigData.bAlwaysTop) 
		? widgetProperties.put_WidgetTopMost('Y') : widgetProperties.put_WidgetTopMost('N');
	// 위치고정
	(g_ConfigData.bPositionFix) 
		? widgetProperties.put_WidgetPositionFix('Y') : widgetProperties.put_WidgetPositionFix('N');

	// 위젯 위치 저장
	CRect rcFrame;
	GetWindowRect(&rcFrame);
	CString strSize, strPosition, strTransparent, strVolumn;
	
	strSize.Format("%d*%d", m_dwWidth, m_dwHeight);
	strPosition.Format("%d,%d", rcFrame.left, rcFrame.top);

	// 투명도
	strTransparent.Format("%d", g_ConfigData.nTransparent);

	widgetProperties.put_WidgetSize(strSize);
	widgetProperties.put_WidgetPosition(strPosition);
	widgetProperties.put_WidgetTransparent(strTransparent);
	widgetProperties.put_WidgetConfig(strVolumn);

	// 생성한 WidgetProperties를 DesktopWiget에 설정
	VARIANT val;
	val.vt = VT_DISPATCH;
	val.pdispVal = widgetProperties;
	m_DesktopWidget.put_WidgetProperties(_T(""), WIDGET_ID, val);

	widgetProperties.ReleaseDispatch();

}

//////////////////////////////////////////////////////////////////////////
// 항상위 선택시 관련 UI Wnd를 모두 TopMost 속성 주기
//////////////////////////////////////////////////////////////////////////
void CNaverTimerByDingpongDlg::SetTopmostWindow(BOOL bTopmost)
{
	if( bTopmost)
	{
		::SetWindowPos(m_hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE );
	}
}

//////////////////////////////////////////////////////////////////////////
// 위치고정 관련 처리
//////////////////////////////////////////////////////////////////////////
void CNaverTimerByDingpongDlg::OnLButtonDown(UINT nFlags, CPoint point)
{	
	if( g_ConfigData.bPositionFix == FALSE )
		SendMessage(WM_NCLBUTTONDOWN, (WPARAM)HTCAPTION, MAKELPARAM(point.x, point.y));
	else
		__super::OnLButtonDown(nFlags, point);
}

//////////////////////////////////////////////////////////////////////////
// RButtonUp시 팝업메뉴 
//////////////////////////////////////////////////////////////////////////
void CNaverTimerByDingpongDlg::OnRButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CPoint ptPoint;
	GetCursorPos(&ptPoint);

	CMenu menuPopup;
	menuPopup.LoadMenu(IDR_MENU_POPUP);

	menuPopup.CheckMenuItem( IDM_TOPMOST, g_ConfigData.bAlwaysTop ? MF_CHECKED : MF_UNCHECKED | MF_BYCOMMAND);
	menuPopup.CheckMenuItem( IDM_POSITION_FIX, g_ConfigData.bPositionFix ? MF_CHECKED : MF_UNCHECKED | MF_BYCOMMAND);

	menuPopup.GetSubMenu(0)->TrackPopupMenu(TPM_LEFTALIGN | TPM_TOPALIGN | TPM_LEFTBUTTON, ptPoint.x, ptPoint.y, this);

	__super::OnRButtonUp(nFlags, point);
}

// 메뉴핸들러-안내
void CNaverTimerByDingpongDlg::OnAboutDesktop()
{
	// Desktop 안내페이지로 IE 띄우기
	if (m_DesktopWidget)
		m_DesktopWidget.LaunchWebPage(_bstr_t("http://desktop.naver.com"), _bstr_t(""), _bstr_t(""));
}

// 메뉴핸들러-옵션창
void CNaverTimerByDingpongDlg::OnConfig()
{
	OnConfigOption(0, 0);
}

// 메뉴핸들러-숨기기
void CNaverTimerByDingpongDlg::OnDesktopHide()
{
	OnBnClickedHide();
}

// 메뉴핸들러-종료
void CNaverTimerByDingpongDlg::OnExit()
{	
	EndDialog(0);
}

// 메뉴핸들러-위치고정
void CNaverTimerByDingpongDlg::OnPositionFix()
{
	g_ConfigData.bPositionFix = !g_ConfigData.bPositionFix;
}

// 메뉴핸들러-항상위
void CNaverTimerByDingpongDlg::OnTopmost()
{
	g_ConfigData.bAlwaysTop = !g_ConfigData.bAlwaysTop;
	SetTopmostWindow(g_ConfigData.bAlwaysTop);
}

void CNaverTimerByDingpongDlg::OnOK()
{	
}

void CNaverTimerByDingpongDlg::OnCancel()
{	
}


BOOL CNaverTimerByDingpongDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if( pMsg->message == WM_LBUTTONDOWN)
	{
		//PostMessage(UM_SHOW_TOP);
		//::BringWindowToTop(m_ListDlg.m_hWnd);	
		//::SetForegroundWindow(m_ListDlg.m_hWnd);

		//::BringWindowToTop(m_volume.m_hWnd);
		//::SetForegroundWindow(m_volume.m_hWnd);

		ShowTopWindow();
		
		
		if( pMsg->hwnd == m_btnMenu )
			OnBnClickedMenu();
		else if( pMsg->hwnd == m_btnPlay.m_hWnd)
			OnBnClickedPlay();		
		else if( pMsg->hwnd == m_btnStop.m_hWnd)
			OnBnClickedStop();
		else if( pMsg->hwnd == m_btnClose.m_hWnd)
			OnBnClickedClose();
		else if( pMsg->hwnd == m_btnHide.m_hWnd)
			OnBnClickedHide();
		else
		{
			if( g_ConfigData.bPositionFix == FALSE )
				SendMessage(WM_NCLBUTTONDOWN, (WPARAM)HTCAPTION, MAKELPARAM(pMsg->pt.x, pMsg->pt.y));
			else
				return FALSE;
		}
		

		//::OutputDebugString("PreTranslateMessage::WM_LBUTTONDOWN\n");

		return TRUE;
	}

	//return FALSE;
	return CDialog::PreTranslateMessage(pMsg);
}
