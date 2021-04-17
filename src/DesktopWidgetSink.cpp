// DesktopWidgetSink.cpp : ���� �����Դϴ�.
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
	// �ڵ�ȭ ��ü�� ���� ������ ������ �����Ǹ� OnFinalRelease��
	// ȣ��˴ϴ�. �⺻ Ŭ�������� �ڵ����� ��ü�� �����մϴ�.
	// �⺻ Ŭ������ ȣ���ϱ� ���� ��ü�� �ʿ��� �߰� ���� �۾���
	// �߰��Ͻʽÿ�.

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

// ����: IID_IDesktopWidgetSink�� ���� ������ �߰��Ͽ�
// VBA���� ���� �������� ���ε��� �����մϴ�. 
// �� IID�� .IDL ���Ͽ� �ִ� dispinterface�� GUID�� ��ġ�ؾ� �մϴ�.


BEGIN_INTERFACE_MAP(CDesktopWidgetSink, CCmdTarget)
	INTERFACE_PART(CDesktopWidgetSink, IID_IDesktopWidgetEvents, Dispatch)
END_INTERFACE_MAP()


// CDesktopWidgetSink �޽��� ó�����Դϴ�.
//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
void CDesktopWidgetSink::OnCloseWidget(void)
{
	TRACE(_T("��������\n"));
	m_pMainDlg->DestroyWindow();
}

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
void CDesktopWidgetSink::OnShowWidget(long nShow)
{
	TRACE(_T("�������̱�\n"));
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
	TRACE(_T("�ɼ�â\n"));
	AfxGetMainWnd()->PostMessage(UM_CONFIG_OPTION, 0, 0);
}

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
void CDesktopWidgetSink::OnOpenNewInstance()
{
	TRACE(_T("���οü����\n"));
}

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
void CDesktopWidgetSink::OnChangeLoginState(long nState)
{
	TRACE(_T("�α��� ���º���\n"));
}

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
void CDesktopWidgetSink::OnCustomCommand(long nCommand, VARIANT value1, VARIANT value2)
{
	TRACE(_T("Ŀ���� ���\n"));
}
