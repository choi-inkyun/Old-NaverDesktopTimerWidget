//////////////////////////////////////////////////////////////////////////
// File		: ConfigDlg.h
// Author	: 한상철 
// Role		: 환경설정 Dialog
//			  항상위, 위치고정, 투명도 등을 설정하는 UI
//
// Date		: 2007-06-04
//////////////////////////////////////////////////////////////////////////



#pragma once


// CConfigDlg 대화 상자입니다.

class CConfigDlg : public CDialog
{
	DECLARE_DYNAMIC(CConfigDlg)

public:
	CConfigDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CConfigDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_CONFIG_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnNMReleasedcaptureSliderTransparent(NMHDR *pNMHDR, LRESULT *pResult);
};
