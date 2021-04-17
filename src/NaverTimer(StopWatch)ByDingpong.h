// NaverTimer(StopWatch)ByDingpong.h : PROJECT_NAME 응용 프로그램에 대한 주 헤더 파일입니다.
//

#pragma once

#ifndef __AFXWIN_H__
	#error PCH에서 이 파일을 포함하기 전에 'stdafx.h'를 포함하십시오.
#endif

#include "resource.h"		// 주 기호

// CNaverTimerByDingpongApp:
// 이 클래스의 구현에 대해서는 NaverTimer(StopWatch)ByDingpong.cpp을 참조하십시오.
//

class CNaverTimerByDingpongApp : public CWinApp
{
public:
	CNaverTimerByDingpongApp();

// 재정의
	public:
	virtual BOOL InitInstance();

// 구현

	DECLARE_MESSAGE_MAP()
};

extern CNaverTimerByDingpongApp theApp;

typedef struct _config_data_tag
{
	// 위젯 설정데이타
	BOOL bAlwaysTop;
	BOOL bPositionFix;
	int	 nTransparent;
	BOOL bShowWidget;

	// 프로그램 정보
//	BOOL bShowList;
//	BOOL bShowVolume;

}CONFIG_DATA;

extern CONFIG_DATA g_ConfigData;



