//////////////////////////////////////////////////////////////////////////
// File		: DefineGUID.h
// Author	: 한상철 
// Role		: IDesktopWidgetEvents 의 CLSID를 정의
//			  "OLE/COM Object Viewer" 를 이용하면 볼수 있다.
//
// Date		: 2007-06-04
//////////////////////////////////////////////////////////////////////////

#pragma once

static const IID IID_IDesktopWidgetEvents =
{ 0xDA3652EF, 0x7DFA, 0x46FA, { 0xAE, 0x2E, 0x82, 0x6D, 0xC1, 0x78, 0x67, 0x15 } };


/* 
--	IDesktopWidgetEvents 정의내용 --

[
	uuid(DA3652EF-7DFA-46FA-AE2E-826DC1786715),
	helpstring("_IDesktopWidgetEvents 인터페이스")
]
dispinterface _IDesktopWidgetEvents
{
properties:
methods:
	[id(1), helpstring("메서드 Close")] HRESULT Close(void);
	[id(2), helpstring("메서드 OnShowWidget")] HRESULT OnShowWidget([in] LONG nShow);
	[id(3), helpstring("메서드 OnOpenOption")] HRESULT OnOpenOption(void);
	[id(4), helpstring("메서드 OnOpenNewInstance")] HRESULT OnOpenNewInstance(void);
	[id(5), helpstring("메서드 OnChangeLoginState")] HRESULT OnChangeLoginState([in] LONG nState);
	[id(6), helpstring("메서드 OnCustomCommand")] HRESULT OnCustomCommand([in] LONG nCommand, [in] VARIANT value1, [in] VARIANT value2);
};

*/