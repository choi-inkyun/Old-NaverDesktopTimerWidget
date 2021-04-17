//////////////////////////////////////////////////////////////////////////
// File		: ConfigDlg.h
// Author	: �ѻ�ö 
// Role		: ȯ�漳�� Dialog
//			  �׻���, ��ġ����, ���� ���� �����ϴ� UI
//
// Date		: 2007-06-04
//////////////////////////////////////////////////////////////////////////



#pragma once


// CConfigDlg ��ȭ �����Դϴ�.

class CConfigDlg : public CDialog
{
	DECLARE_DYNAMIC(CConfigDlg)

public:
	CConfigDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CConfigDlg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_CONFIG_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnNMReleasedcaptureSliderTransparent(NMHDR *pNMHDR, LRESULT *pResult);
};
