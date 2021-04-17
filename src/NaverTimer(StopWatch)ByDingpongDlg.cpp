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


// CNaverTimerByDingpongDlg ��ȭ ����
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
// �ʱ�ȭ �۾�
//////////////////////////////////////////////////////////////////////////
BOOL CNaverTimerByDingpongDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// �� ��ȭ ������ �������� �����մϴ�. ���� ���α׷��� �� â�� ��ȭ ���ڰ� �ƴ� ��쿡��
	// �����ӿ�ũ�� �� �۾��� �ڵ����� �����մϴ�.
	SetIcon(m_hIcon, TRUE);			// ū �������� �����մϴ�.
	SetIcon(m_hIcon, FALSE);		// ���� �������� �����մϴ�.

	// ����ǥ���� �����
	CDlgBase::HideApplicationTitlebar(m_hWnd);	
	// ������ ��缳��
	CDlgBase::SetWindowRegion(m_hWnd, IDB_PLAYER_BG);

	// ��ư, ����, ���������� UI ��Ʈ���� �����Ѵ�. 
	InitUICtrl();

	// ����ũ�����������üũ Ÿ�̸�
	SetTimer(TIMER_MAIN_PROCESS_CHECK, 5, NULL);

	COleException error;
	TCHAR szError[255] = {0};	
	if( m_DesktopWidget.CreateDispatch("NBoard.DesktopWidget", &error) )
	{
		//������ ���μ��� ID, WidgetID, �Ҵ���� WidgetKey�� ����		
		//dingpong. �̰Ͷ����� ���� ���-��-; �������̵� ���������� �������� ������ ������ �� ����. .ndw �� .nd �� �ٸ��ٴ� ���� .exe �� .ndw �� �ٲٱ� ������
		//�ڵ� �ȿ� ��� ���̵� ���� ���̶� �����ߴµ�.. ����� �����Ǿ� ���� ���̶� ������ �Ͱ��� �޸� �̷��� �ϵ� �ڵ� �Ǿ�����-��-;
		m_DesktopWidget.SetWidgetInfo(GetCurrentProcessId(), 2661, "bb4d613de682a4f9b632b19e54248d77");
	}
	else
	{
		error.GetErrorMessage(szError, 255);
		::OutputDebugString(szError);		
	}

	// ������ ���� ����Ǿ����� ����ũ�� �������μ������� �˸��ϴ�. 
	m_DesktopWidget.WidgetConnected();

	// ����ũ�� �̺�Ʈ��ũ ����
	m_DesktopWidgetSink.m_pMainDlg = this;
	LPUNKNOWN pUnkSink = m_DesktopWidgetSink.GetIDispatch(FALSE);
	AfxConnectionAdvise(m_DesktopWidget.m_lpDispatch, IID_IDesktopWidgetEvents, pUnkSink, FALSE, &m_dwCookie);

	// ȯ�漳�� �����͸� �о�´�. (WidgetProperties)
	LoadConfigData();


	SetTopmostWindow(g_ConfigData.bAlwaysTop);		// �׻��� ����
	OnTransparent(0, g_ConfigData.nTransparent);	// ���� ����

	DrawDoubleBuffering();

	return TRUE;  // ��Ʈ�ѿ� ���� ��Ŀ���� �������� ���� ��� TRUE�� ��ȯ�մϴ�.
}

// ���� ���۸��� �̿��Ͽ� �׸��� : �������� ����. �ð� ������ �ִ�.
// MemDC : ���� ȭ�� DC
// m_Bitmap : ���� ȭ�� DC �� �׸� ��ȭ��
void CNaverTimerByDingpongDlg::DrawDoubleBuffering(void)
{
     CClientDC dc(this);
 
     CRect rect;
     GetClientRect(&rect);

     CDC   MemDC;
     CBitmap* pOldBmp;

     // ��ȭ���� ���� ��ũ�� DC�� ��ġ ��Ų��.

     m_Bitmap.DeleteObject();
     m_Bitmap.CreateCompatibleBitmap(&dc, rect.Width(), rect.Height());
    
     // ���� ȭ�� DC�� ���� ��ũ�� DC�� ��ġ ��Ų��.
     MemDC.CreateCompatibleDC(&dc);

     // ���� ȭ�� DC �� ��ȭ���� ���.
     pOldBmp = MemDC.SelectObject(&m_Bitmap);

     // ���� ȭ�� DC�� ������� �����Ѵ�.
     MemDC.PatBlt(0,0,rect.Width(), rect.Height(), WHITENESS);
    
 	 BitBlt( MemDC, 0, 0, m_dwWidth, m_dwHeight, CDlgBase::m_dcBkGrnd, 0, 0, SRCCOPY );

	 // Text�� �����ش�.
	 _DrawText(&MemDC);	

	 m_btnMenu.Draw(&MemDC);
	 m_btnPlay.Draw(&MemDC);
	 m_btnStop.Draw(&MemDC);
	 m_btnClose.Draw(&MemDC);
	 m_btnHide.Draw(&MemDC);

     // ����ȭ�� ����� ���� OnDraw�� ����.
     Invalidate();

     MemDC.SelectObject(pOldBmp);
     MemDC.DeleteDC();
}



// ��ȭ ���ڿ� �ּ�ȭ ���߸� �߰��� ��� �������� �׸����� 
// �Ʒ� �ڵ尡 �ʿ��մϴ�. ����/�� ���� ����ϴ� MFC ���� ���α׷��� ��쿡��
// �����ӿ�ũ���� �� �۾��� �ڵ����� �����մϴ�.
void CNaverTimerByDingpongDlg::OnPaint() 
{
	CPaintDC dc(this); // �׸��⸦ ���� ����̽� ���ؽ�Ʈ
	if (IsIconic())
	{
		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Ŭ���̾�Ʈ �簢������ �������� ����� ����ϴ�.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// �������� �׸��ϴ�.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		// ���׷��ֱ�
		if( m_dcBkGrnd )
		{
//			BitBlt( dc.m_hDC, 0, 0, m_dwWidth, m_dwHeight, CDlgBase::m_dcBkGrnd, 0, 0, SRCCOPY );
		}

		CDialog::OnPaint();
	}
	// ȭ���� �׷��ش�.
	_Draw(&dc);
}

void CNaverTimerByDingpongDlg::_Draw(CDC* pDC)
{
    CDC dc;
    CRect rect;
    GetClientRect(&rect);

	// ��Ʈ�� ���
    dc.CreateCompatibleDC(pDC);
    dc.SelectObject(&m_Bitmap);

	pDC->BitBlt(0,0,rect.Width(), rect.Height(), &dc, 0, 0, SRCCOPY);

}
void CNaverTimerByDingpongDlg::_DrawText(CDC* pDC)
{
	CRect rText;
	GetClientRect(rText);
		
	rText.DeflateRect(3, 3);		// ���Ͽ��� 3 �ȼ�
	rText.left	= 80;
	rText.right	= rText.left + 150;

	// dingpong. �ٽ����� ���� Ÿ�̸� ��� ��ƾ. �̷��� ���������� ���� �� �ʿ�� ���� ������ ������ �� ����� �ֱ� ������.
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
	pDC->SetBkMode(TRANSPARENT); // ����� �������� �����Ѵ�. �ȱ׷��� ��� ���� ���� �� �ִ�.
	pDC->DrawText(nowTimeStr, rText, DT_LEFT | DT_SINGLELINE | DT_VCENTER | DT_END_ELLIPSIS); // ���� �ؽ�Ʈ�� �׸�.
	pDC->SelectObject(pOldFont);	
}
//////////////////////////////////////////////////////////////////////////
// ����ڰ� �ּ�ȭ�� â�� ���� ���ȿ� Ŀ���� ǥ�õǵ��� �ý��ۿ���
//  �� �Լ��� ȣ���մϴ�. 
//////////////////////////////////////////////////////////////////////////
HCURSOR CNaverTimerByDingpongDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//////////////////////////////////////////////////////////////////////////
// �����۾�
//////////////////////////////////////////////////////////////////////////
void CNaverTimerByDingpongDlg::OnDestroy()
{
	__super::OnDestroy();

	currentTimeFont.DeleteObject();

	StopPlayer();
	KillTimer(TIMER_MAIN_PROCESS_CHECK);

	// WidgetPropertie �������� �����Ѵ�.
	SaveConfigData();
	
	// �̺�Ʈ ��ũ ����
	LPUNKNOWN pUnkSink = m_DesktopWidgetSink.GetIDispatch(FALSE);
	AfxConnectionUnadvise(m_DesktopWidget.m_lpDispatch, IID_IDesktopWidgetEvents, pUnkSink, FALSE, m_dwCookie);

	m_DesktopWidget.ReleaseDispatch();

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
}

//////////////////////////////////////////////////////////////////////////
// ������ �̵��� ��Ÿ �����쵵 ���� �̵�
//////////////////////////////////////////////////////////////////////////
void CNaverTimerByDingpongDlg::OnMove(int x, int y)
{
	__super::OnMove(x, y);
}

//////////////////////////////////////////////////////////////////////////
// UI ��Ʈ���� �����ϰ� �ʱ�ȭ
//////////////////////////////////////////////////////////////////////////
void CNaverTimerByDingpongDlg::InitUICtrl()
{
	currentTimeFont.CreatePointFont(80, _T("MS Sans Serif"));
	InitButtonCtrl();
}

//////////////////////////////////////////////////////////////////////////
// ��ư���� �����Ѵ�.
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
// Menu��ư �ڵ鷯 : Popup�޴��� �����ش�.
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
// Play ��ư �ڵ鷯
//////////////////////////////////////////////////////////////////////////
void CNaverTimerByDingpongDlg::OnBnClickedPlay()
{	
	StartPlayer();
}

//////////////////////////////////////////////////////////////////////////
// Stop ��ư �ڵ鷯
//////////////////////////////////////////////////////////////////////////
void CNaverTimerByDingpongDlg::OnBnClickedStop()
{
	StopPlayer();
	m_btnPlay.SetBkImage(IDB_PLAY_BUTTON_SET, 19);
	m_btnPlay.Invalidate();
}

//////////////////////////////////////////////////////////////////////////
// Close ��ư �ڵ鷯 : �����Ѵ�.
//////////////////////////////////////////////////////////////////////////
void CNaverTimerByDingpongDlg::OnBnClickedClose()
{	
	EndDialog(0);
}

//////////////////////////////////////////////////////////////////////////
// Hide��ư �ڵ鷯 : ��ü�� �����.
//////////////////////////////////////////////////////////////////////////
void CNaverTimerByDingpongDlg::OnBnClickedHide()
{
	ShowWindow(SW_HIDE);
}

//////////////////////////////////////////////////////////////////////////
// ���õ� �뷡�� �÷��� �Ѵ�.
//////////////////////////////////////////////////////////////////////////
void CNaverTimerByDingpongDlg::StartPlayer()
{
	saveTime = GetTickCount();
	nowPlayingCheck = true;
}

//////////////////////////////////////////////////////////////////////////
// �뷡�� �����.
//////////////////////////////////////////////////////////////////////////
void CNaverTimerByDingpongDlg::StopPlayer()
{
	// Ÿ�̸� ����
	KillTimer(TIMER_PLAY);

	nowPlayingCheck = false;
}

//////////////////////////////////////////////////////////////////////////
// ListDlg���� ���õ� ������ ���� ������
//////////////////////////////////////////////////////////////////////////
LRESULT CNaverTimerByDingpongDlg::OnStopPlayer(WPARAM wParam, LPARAM lParam)
{
	OnBnClickedStop();	
	return 1L;
}

//////////////////////////////////////////////////////////////////////////
// �����츦 top���� �����ش�. (list, volume ���� �´�. )
//////////////////////////////////////////////////////////////////////////
LRESULT CNaverTimerByDingpongDlg::OnShowTop(WPARAM wParam, LPARAM lParam)
{
	ShowTopWindow();
	return 1L;
}

//////////////////////////////////////////////////////////////////////////
// �ɼ�â�� ����.
//////////////////////////////////////////////////////////////////////////
LRESULT CNaverTimerByDingpongDlg::OnConfigOption(WPARAM wParam, LPARAM lParam)
{
	CConfigDlg	dlg;

	//ShowWindow(SW_SHOW);

	if( dlg.DoModal() == IDOK)
	{
		// �׻��� ����
		SetTopmostWindow(g_ConfigData.bAlwaysTop);		
		// ���� ����
		OnTransparent(0, g_ConfigData.nTransparent);
	}

	return 1L;
}

//////////////////////////////////////////////////////////////////////////
// Ÿ�̸� �ڵ鷯
//////////////////////////////////////////////////////////////////////////
void CNaverTimerByDingpongDlg::OnTimer(UINT nIDEvent)
{	
	// �÷��� 1�ʰ���
	if( nIDEvent == TIMER_PLAY )
	{

	}
	// ���� Desktop�� ����ִ��� üũ 
	else if( nIDEvent == TIMER_MAIN_PROCESS_CHECK )
	{
	}

	if(nowPlayingCheck == true)
		DrawDoubleBuffering();

	__super::OnTimer(nIDEvent);
}

//////////////////////////////////////////////////////////////////////////
// ���� ���� : �ٸ� �����쵵 �Ա� ����
//////////////////////////////////////////////////////////////////////////
LRESULT CNaverTimerByDingpongDlg::OnTransparent(WPARAM wParam, LPARAM lParam)
{	
	BYTE nAlpha = static_cast<BYTE>(lParam);

	if( nAlpha < 20 || nAlpha > 255)	return 0L;

	// ���� ������
	CDlgBase::SetWindowTransparent(m_hWnd, nAlpha);
	
	// ��������� �ʺ����� ������ ���� ���̰�
	::ShowWindow(m_hWnd, SW_SHOW);	

	return 1L;
}

//////////////////////////////////////////////////////////////////////////
// WidgetProperty �ε��ϱ�
//////////////////////////////////////////////////////////////////////////
void CNaverTimerByDingpongDlg::LoadConfigData()
{		
	int nLeft = 0, nTop = 0, nRight = 0, nBottom = 0;
	char chMainShow, chAlwaysTop, chPositionFix;	
	CString szWidgetSize, szWidgetPosition, szConfig, szTransparent;

	// WidgetProperties ��ü ������
	VARIANT val;
	val = m_DesktopWidget.get_WidgetProperties(_T(""), WIDGET_ID);
	CWidgetProperties widgetProperties(val.pdispVal);


	chMainShow = widgetProperties.get_WidgetMainShow();

	// �׻���
	chAlwaysTop = widgetProperties.get_WidgetTopMost();
	g_ConfigData.bAlwaysTop = (chAlwaysTop == 'Y');

	// ��ġ����
	chPositionFix = widgetProperties.get_WidgetPositionFix();
	g_ConfigData.bPositionFix = (chPositionFix == 'Y');
	
	// ����
	szTransparent = widgetProperties.get_WidgetTransparent();
	g_ConfigData.nTransparent = atoi(szTransparent);
	if( g_ConfigData.nTransparent < 20 )
		g_ConfigData.nTransparent = 255;
	
	// ��ġ, ũ��
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

	// ��ġũ��� ������ ������ �̵�
	if (nRight && nBottom)
	{
		::MoveWindow(m_hWnd, nLeft, nTop, nRight-nLeft, nBottom-nTop, TRUE);
		g_ConfigData.bShowWidget = (chMainShow == 'N') ? FALSE:TRUE;
	}

}

//////////////////////////////////////////////////////////////////////////
// WidgetProperty �����ϱ�
//////////////////////////////////////////////////////////////////////////
void CNaverTimerByDingpongDlg::SaveConfigData()
{	
	// WidgetProperties ��ü ����
	COleException error;
	CWidgetProperties widgetProperties;
	widgetProperties.CreateDispatch("NBoard.WidgetProperties", &error);

	widgetProperties.put_WidgetDataType(1); 
	widgetProperties.put_WidgetType(5);
	widgetProperties.put_WidgetOrder(0);
	widgetProperties.put_WidgetViewType('F');

	// ���̱� 
	(g_ConfigData.bShowWidget) ? widgetProperties.put_WidgetMainShow('Y') : widgetProperties.put_WidgetMainShow('N');
	// �׻���
	(g_ConfigData.bAlwaysTop) 
		? widgetProperties.put_WidgetTopMost('Y') : widgetProperties.put_WidgetTopMost('N');
	// ��ġ����
	(g_ConfigData.bPositionFix) 
		? widgetProperties.put_WidgetPositionFix('Y') : widgetProperties.put_WidgetPositionFix('N');

	// ���� ��ġ ����
	CRect rcFrame;
	GetWindowRect(&rcFrame);
	CString strSize, strPosition, strTransparent, strVolumn;
	
	strSize.Format("%d*%d", m_dwWidth, m_dwHeight);
	strPosition.Format("%d,%d", rcFrame.left, rcFrame.top);

	// ����
	strTransparent.Format("%d", g_ConfigData.nTransparent);

	widgetProperties.put_WidgetSize(strSize);
	widgetProperties.put_WidgetPosition(strPosition);
	widgetProperties.put_WidgetTransparent(strTransparent);
	widgetProperties.put_WidgetConfig(strVolumn);

	// ������ WidgetProperties�� DesktopWiget�� ����
	VARIANT val;
	val.vt = VT_DISPATCH;
	val.pdispVal = widgetProperties;
	m_DesktopWidget.put_WidgetProperties(_T(""), WIDGET_ID, val);

	widgetProperties.ReleaseDispatch();

}

//////////////////////////////////////////////////////////////////////////
// �׻��� ���ý� ���� UI Wnd�� ��� TopMost �Ӽ� �ֱ�
//////////////////////////////////////////////////////////////////////////
void CNaverTimerByDingpongDlg::SetTopmostWindow(BOOL bTopmost)
{
	if( bTopmost)
	{
		::SetWindowPos(m_hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE );
	}
}

//////////////////////////////////////////////////////////////////////////
// ��ġ���� ���� ó��
//////////////////////////////////////////////////////////////////////////
void CNaverTimerByDingpongDlg::OnLButtonDown(UINT nFlags, CPoint point)
{	
	if( g_ConfigData.bPositionFix == FALSE )
		SendMessage(WM_NCLBUTTONDOWN, (WPARAM)HTCAPTION, MAKELPARAM(point.x, point.y));
	else
		__super::OnLButtonDown(nFlags, point);
}

//////////////////////////////////////////////////////////////////////////
// RButtonUp�� �˾��޴� 
//////////////////////////////////////////////////////////////////////////
void CNaverTimerByDingpongDlg::OnRButtonUp(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	CPoint ptPoint;
	GetCursorPos(&ptPoint);

	CMenu menuPopup;
	menuPopup.LoadMenu(IDR_MENU_POPUP);

	menuPopup.CheckMenuItem( IDM_TOPMOST, g_ConfigData.bAlwaysTop ? MF_CHECKED : MF_UNCHECKED | MF_BYCOMMAND);
	menuPopup.CheckMenuItem( IDM_POSITION_FIX, g_ConfigData.bPositionFix ? MF_CHECKED : MF_UNCHECKED | MF_BYCOMMAND);

	menuPopup.GetSubMenu(0)->TrackPopupMenu(TPM_LEFTALIGN | TPM_TOPALIGN | TPM_LEFTBUTTON, ptPoint.x, ptPoint.y, this);

	__super::OnRButtonUp(nFlags, point);
}

// �޴��ڵ鷯-�ȳ�
void CNaverTimerByDingpongDlg::OnAboutDesktop()
{
	// Desktop �ȳ��������� IE ����
	if (m_DesktopWidget)
		m_DesktopWidget.LaunchWebPage(_bstr_t("http://desktop.naver.com"), _bstr_t(""), _bstr_t(""));
}

// �޴��ڵ鷯-�ɼ�â
void CNaverTimerByDingpongDlg::OnConfig()
{
	OnConfigOption(0, 0);
}

// �޴��ڵ鷯-�����
void CNaverTimerByDingpongDlg::OnDesktopHide()
{
	OnBnClickedHide();
}

// �޴��ڵ鷯-����
void CNaverTimerByDingpongDlg::OnExit()
{	
	EndDialog(0);
}

// �޴��ڵ鷯-��ġ����
void CNaverTimerByDingpongDlg::OnPositionFix()
{
	g_ConfigData.bPositionFix = !g_ConfigData.bPositionFix;
}

// �޴��ڵ鷯-�׻���
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
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
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
