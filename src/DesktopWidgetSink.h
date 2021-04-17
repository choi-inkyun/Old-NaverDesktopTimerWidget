//////////////////////////////////////////////////////////////////////////
// File		: DefineGUID.h
// Author	: 한상철 
// Role		: IDesktopWidgetEvents 의 이벤트를 받아서 처리한다. 
//			  
//
// Date		: 2007-06-04
//////////////////////////////////////////////////////////////////////////

#pragma once
#include "DefineGUID.h"

// CDesktopWidgetSink 명령 대상입니다.

class CNaverTimerByDingpongDlg;		// 메인Dlg 전방위선언
class CDesktopWidgetSink : public CCmdTarget
{
	DECLARE_DYNAMIC(CDesktopWidgetSink)

public:
	CDesktopWidgetSink();
	virtual ~CDesktopWidgetSink();

	virtual void OnFinalRelease();

	CNaverTimerByDingpongDlg*	m_pMainDlg;

protected:
	DECLARE_MESSAGE_MAP()
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()


	// Desktop에서 종료시킬때 발생
	afx_msg void OnCloseWidget();

	// Desktop에서 위젯 보이기 할때 발생
	afx_msg void OnShowWidget(long nShow);

	// Desktop에서 옵션메뉴시에 발생
	afx_msg void OnOpenOption();

	// Desktop에서 새로운 객체 생성시에 발생
	afx_msg void OnOpenNewInstance();

	// Desktop의 로그인 상태가 변경되었을씨에 발생
	afx_msg void OnChangeLoginState(long nState);

	// 커스텀 커맨드
	afx_msg void OnCustomCommand(long nCommand, VARIANT value1, VARIANT value2);
};


