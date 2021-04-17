// DesktopWidgetSink.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "NaverTimer(StopWatch)ByDingpong.h"
#include "DesktopWidgetSink.h"

#include "NaverTimer(StopWatch)ByDingpongDlg.h"

// CDesktopWidgetSink

IMPLEMENT_DYNAMIC(CDesktopWidgetSink, CCmdTarget)
CDesktopWidgetSink::CDesktopWidgetSink()
{
	EnableAutomation();
}

CDesktopWidgetSink::~CDesktopWidgetSink()
{

}


void CDesktopWidgetSink::OnFinalRelease()
{
	// 자동화 개체에 대한 마지막 참조가 해제되면 OnFinalRelease가
	// 호출됩니다. 기본 클래스에서 자동으로 개체를 삭제합니다.
	// 기본 클래스를 호출하기 전에 개체에 필요한 추가 정리 작업을
	// 추가하십시오.

	CCmdTarget::OnFinalRelease();
}


BEGIN_MESSAGE_MAP(CDesktopWidgetSink, CCmdTarget)
END_MESSAGE_MAP()


BEGIN_DISPATCH_MAP(CDesktopWidgetSink, CCmdTarget)
	DISP_FUNCTION(CDesktopWidgetSink, "1", OnCloseWidget, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CDesktopWidgetSink, "1", OnShowWidget, VT_EMPTY, VTS_I4)
	DISP_FUNCTION(CDesktopWidgetSink, "1", OnOpenOption, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CDesktopWidgetSink, "1", OnOpenNewInstance, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CDesktopWidgetSink, "1", OnChangeLoginState, VT_EMPTY, VTS_I4)
	DISP_FUNCTION(CDesktopWidgetSink, "1", OnCustomCommand, VT_EMPTY, VTS_I4 VTS_VARIANT VTS_VARIANT)
END_DISPATCH_MAP()

// 참고: IID_IDesktopWidgetSink에 대한 지원을 추가하여
// VBA에서 형식 안정적인 바인딩을 지원합니다. 
// 이 IID는 .IDL 파일에 있는 dispinterface의 GUID와 일치해야 합니다.


BEGIN_INTERFACE_MAP(CDesktopWidgetSink, CCmdTarget)
	INTERFACE_PART(CDesktopWidgetSink, IID_IDesktopWidgetEvents, Dispatch)
END_INTERFACE_MAP()


// CDesktopWidgetSink 메시지 처리기입니다.
//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
void CDesktopWidgetSink::OnCloseWidget(void)
{
	TRACE(_T("위젯종료\n"));
	m_pMainDlg->DestroyWindow();
}

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
void CDesktopWidgetSink::OnShowWidget(long nShow)
{
	TRACE(_T("위젯보이기\n"));
	if( nShow == TRUE )
	{		
		//m_pMainDlg->ShowTopWindow();
		AfxGetMainWnd()->PostMessage(UM_SHOW_TOP, 0, 0);
	}
	else
	{
		m_pMainDlg->ShowWindow(SW_HIDE);
	}
}

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
void CDesktopWidgetSink::OnOpenOption()
{	
	TRACE(_T("옵션창\n"));
	AfxGetMainWnd()->PostMessage(UM_CONFIG_OPTION, 0, 0);
}

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
void CDesktopWidgetSink::OnOpenNewInstance()
{
	TRACE(_T("새로운객체생성\n"));
}

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
void CDesktopWidgetSink::OnChangeLoginState(long nState)
{
	TRACE(_T("로그인 상태변경\n"));
}

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
void CDesktopWidgetSink::OnCustomCommand(long nCommand, VARIANT value1, VARIANT value2)
{
	TRACE(_T("커스텀 명령\n"));
}
