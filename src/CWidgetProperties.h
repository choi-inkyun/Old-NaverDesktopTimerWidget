// ��ǻ�Ϳ��� ���� ���̺귯�� �������� [Ŭ���� �߰�]�� ����Ͽ� ������ IDispatch ���� Ŭ�����Դϴ�.

// CWidgetProperties ���� Ŭ����

class CWidgetProperties : public COleDispatchDriver
{
public:
	CWidgetProperties(){} // COleDispatchDriver �⺻ �����ڸ� ȣ���մϴ�.
	CWidgetProperties(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CWidgetProperties(const CWidgetProperties& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// Ư��
public:

	// �۾�
public:


	// IWidgetProperties �޼���
public:
	long get_WidgetType()
	{
		long result;
		InvokeHelper(0x1, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_WidgetType(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x1, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	long get_WidgetOrder()
	{
		long result;
		InvokeHelper(0x2, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_WidgetOrder(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x2, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	signed char get_WidgetViewType()
	{
		signed char result;
		InvokeHelper(0x3, DISPATCH_PROPERTYGET, VT_I1, (void*)&result, NULL);
		return result;
	}
	void put_WidgetViewType(signed char newValue)
	{
		static BYTE parms[] = VTS_I1 ;
		InvokeHelper(0x3, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	signed char get_WidgetMainShow()
	{
		signed char result;
		InvokeHelper(0x4, DISPATCH_PROPERTYGET, VT_I1, (void*)&result, NULL);
		return result;
	}
	void put_WidgetMainShow(signed char newValue)
	{
		static BYTE parms[] = VTS_I1 ;
		InvokeHelper(0x4, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	signed char get_WidgetTopMost()
	{
		signed char result;
		InvokeHelper(0x5, DISPATCH_PROPERTYGET, VT_I1, (void*)&result, NULL);
		return result;
	}
	void put_WidgetTopMost(signed char newValue)
	{
		static BYTE parms[] = VTS_I1 ;
		InvokeHelper(0x5, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	signed char get_WidgetPositionFix()
	{
		signed char result;
		InvokeHelper(0x6, DISPATCH_PROPERTYGET, VT_I1, (void*)&result, NULL);
		return result;
	}
	void put_WidgetPositionFix(signed char newValue)
	{
		static BYTE parms[] = VTS_I1 ;
		InvokeHelper(0x6, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	CString get_WidgetSize()
	{
		CString result;
		InvokeHelper(0x7, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	void put_WidgetSize(LPCTSTR newValue)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x7, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	CString get_WidgetPosition()
	{
		CString result;
		InvokeHelper(0x8, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	void put_WidgetPosition(LPCTSTR newValue)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x8, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	CString get_WidgetTransparent()
	{
		CString result;
		InvokeHelper(0x9, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	void put_WidgetTransparent(LPCTSTR newValue)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x9, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	CString get_WidgetConfig()
	{
		CString result;
		InvokeHelper(0xa, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	void put_WidgetConfig(LPCTSTR newValue)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0xa, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	long get_WidgetDataType()
	{
		long result;
		InvokeHelper(0xb, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_WidgetDataType(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0xb, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}

	// IWidgetProperties �Ӽ�
public:

};
