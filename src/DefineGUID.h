//////////////////////////////////////////////////////////////////////////
// File		: DefineGUID.h
// Author	: �ѻ�ö 
// Role		: IDesktopWidgetEvents �� CLSID�� ����
//			  "OLE/COM Object Viewer" �� �̿��ϸ� ���� �ִ�.
//
// Date		: 2007-06-04
//////////////////////////////////////////////////////////////////////////

#pragma once

static const IID IID_IDesktopWidgetEvents =
{ 0xDA3652EF, 0x7DFA, 0x46FA, { 0xAE, 0x2E, 0x82, 0x6D, 0xC1, 0x78, 0x67, 0x15 } };


/* 
--	IDesktopWidgetEvents ���ǳ��� --

[
	uuid(DA3652EF-7DFA-46FA-AE2E-826DC1786715),
	helpstring("_IDesktopWidgetEvents �������̽�")
]
dispinterface _IDesktopWidgetEvents
{
properties:
methods:
	[id(1), helpstring("�޼��� Close")] HRESULT Close(void);
	[id(2), helpstring("�޼��� OnShowWidget")] HRESULT OnShowWidget([in] LONG nShow);
	[id(3), helpstring("�޼��� OnOpenOption")] HRESULT OnOpenOption(void);
	[id(4), helpstring("�޼��� OnOpenNewInstance")] HRESULT OnOpenNewInstance(void);
	[id(5), helpstring("�޼��� OnChangeLoginState")] HRESULT OnChangeLoginState([in] LONG nState);
	[id(6), helpstring("�޼��� OnCustomCommand")] HRESULT OnCustomCommand([in] LONG nCommand, [in] VARIANT value1, [in] VARIANT value2);
};

*/