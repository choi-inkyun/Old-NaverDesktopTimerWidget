// NaverTimer(StopWatch)ByDingpong.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error PCH���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����Ͻʽÿ�.
#endif

#include "resource.h"		// �� ��ȣ

// CNaverTimerByDingpongApp:
// �� Ŭ������ ������ ���ؼ��� NaverTimer(StopWatch)ByDingpong.cpp�� �����Ͻʽÿ�.
//

class CNaverTimerByDingpongApp : public CWinApp
{
public:
	CNaverTimerByDingpongApp();

// ������
	public:
	virtual BOOL InitInstance();

// ����

	DECLARE_MESSAGE_MAP()
};

extern CNaverTimerByDingpongApp theApp;

typedef struct _config_data_tag
{
	// ���� ��������Ÿ
	BOOL bAlwaysTop;
	BOOL bPositionFix;
	int	 nTransparent;
	BOOL bShowWidget;

	// ���α׷� ����
//	BOOL bShowList;
//	BOOL bShowVolume;

}CONFIG_DATA;

extern CONFIG_DATA g_ConfigData;



