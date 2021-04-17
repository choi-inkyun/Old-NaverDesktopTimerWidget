// 컴퓨터에서 형식 라이브러리 마법사의 [클래스 추가]를 사용하여 생성한 IDispatch 래퍼 클래스입니다.

// CDesktopWidget 래퍼 클래스

class CDesktopWidget : public COleDispatchDriver
{
public:
	CDesktopWidget(){} // COleDispatchDriver 기본 생성자를 호출합니다.
	CDesktopWidget(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CDesktopWidget(const CDesktopWidget& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// 특성
public:

	// 작업
public:


	// IDesktopWidget 메서드
public:
	void SetWidgetInfo(long nProcessID, long nWidgetID, LPCTSTR bsWidgetKey)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_BSTR ;
		InvokeHelper(0x1, DISPATCH_METHOD, VT_EMPTY, NULL, parms, nProcessID, nWidgetID, bsWidgetKey);
	}
	VARIANT get_AuthMgr()
	{
		VARIANT result;
		InvokeHelper(0x2, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, NULL);
		return result;
	}
	VARIANT get_BuddyMgr()
	{
		VARIANT result;
		InvokeHelper(0x3, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, NULL);
		return result;
	}
	void _SendEventToWidget(long nEventID, long nValue, VARIANT * pValue1, VARIANT * pValue2)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_PVARIANT VTS_PVARIANT ;
		InvokeHelper(0x4, DISPATCH_METHOD, VT_EMPTY, NULL, parms, nEventID, nValue, pValue1, pValue2);
	}
	void SetChangeWidgetIcon(long nWidgetID, LPCTSTR bsResName, LPCTSTR bsResType, long nGrow)
	{
		static BYTE parms[] = VTS_I4 VTS_BSTR VTS_BSTR VTS_I4 ;
		InvokeHelper(0x5, DISPATCH_METHOD, VT_EMPTY, NULL, parms, nWidgetID, bsResName, bsResType, nGrow);
	}
	VARIANT get_NotiMgr()
	{
		VARIANT result;
		InvokeHelper(0x6, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, NULL);
		return result;
	}
	void put_WidgetProperties(LPCTSTR bsUserID, long nWidgetID, VARIANT newValue)
	{
		static BYTE parms[] = VTS_BSTR VTS_I4 VTS_VARIANT ;
		InvokeHelper(0x7, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, bsUserID, nWidgetID, &newValue);
	}
	VARIANT get_WidgetProperties(LPCTSTR bsUserID, long nWidgetID)
	{
		VARIANT result;
		static BYTE parms[] = VTS_BSTR VTS_I4 ;
		InvokeHelper(0x7, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, parms, bsUserID, nWidgetID);
		return result;
	}
	VARIANT get_P2PMgr()
	{
		VARIANT result;
		InvokeHelper(0x8, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, NULL);
		return result;
	}
	void RequestLogin(long nWidgetID)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x9, DISPATCH_METHOD, VT_EMPTY, NULL, parms, nWidgetID);
	}
	void SendWidgetCommand(long nWidgetID, long nCommandID, VARIANT value1, VARIANT value2)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_VARIANT VTS_VARIANT ;
		InvokeHelper(0xa, DISPATCH_METHOD, VT_EMPTY, NULL, parms, nWidgetID, nCommandID, &value1, &value2);
	}
	void WidgetConnected()
	{
		InvokeHelper(0xb, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void LaunchWebPage(LPCTSTR bsURL, LPCTSTR bsFeature, LPCTSTR bsPostData)
	{
		static BYTE parms[] = VTS_BSTR VTS_BSTR VTS_BSTR ;
		InvokeHelper(0xc, DISPATCH_METHOD, VT_EMPTY, NULL, parms, bsURL, bsFeature, bsPostData);
	}

	// IDesktopWidget 속성
public:

};
