//////////////////////////////////////////////////////////////////////////
// File		: DefineGUID.h
// Author	: �ѻ�ö 
// Role		: IDesktopWidgetEvents �� �̺�Ʈ�� �޾Ƽ� ó���Ѵ�. 
//			  
//
// Date		: 2007-06-04
//////////////////////////////////////////////////////////////////////////

#pragma once
#include "DefineGUID.h"

// CDesktopWidgetSink ��� ����Դϴ�.

class CNaverTimerByDingpongDlg;		// ����Dlg ����������
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


	// Desktop���� �����ų�� �߻�
	afx_msg void OnCloseWidget();

	// Desktop���� ���� ���̱� �Ҷ� �߻�
	afx_msg void OnShowWidget(long nShow);

	// Desktop���� �ɼǸ޴��ÿ� �߻�
	afx_msg void OnOpenOption();

	// Desktop���� ���ο� ��ü �����ÿ� �߻�
	afx_msg void OnOpenNewInstance();

	// Desktop�� �α��� ���°� ����Ǿ������� �߻�
	afx_msg void OnChangeLoginState(long nState);

	// Ŀ���� Ŀ�ǵ�
	afx_msg void OnCustomCommand(long nCommand, VARIANT value1, VARIANT value2);
};


