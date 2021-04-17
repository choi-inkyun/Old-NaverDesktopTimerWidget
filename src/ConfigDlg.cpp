// ConfigDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "NaverTimer(StopWatch)ByDingpong.h"
#include "ConfigDlg.h"
#include ".\configdlg.h"


// CConfigDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CConfigDlg, CDialog)
CConfigDlg::CConfigDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CConfigDlg::IDD, pParent)
{
}

CConfigDlg::~CConfigDlg()
{
}

void CConfigDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CConfigDlg, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)	
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_TRANSPARENT, OnNMReleasedcaptureSliderTransparent)
END_MESSAGE_MAP()


// CConfigDlg 메시지 처리기입니다.

//////////////////////////////////////////////////////////////////////////
// 초기화 작업 : 데이터로 UI를 설정한다. 
//////////////////////////////////////////////////////////////////////////
BOOL CConfigDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 슬라이더 Ctrl를 얻는다.
	CSliderCtrl* pSlider = static_cast<CSliderCtrl*>(GetDlgItem(IDC_SLIDER_TRANSPARENT));	
	pSlider->SetRange(20, 255);
	
	// 투명도 설정
	pSlider->SetPos(g_ConfigData.nTransparent);
	// 항상위 설정
	((CButton*)GetDlgItem(IDC_CHECK1))->SetCheck(g_ConfigData.bAlwaysTop);
	// 위치고정 설정
	((CButton*)GetDlgItem(IDC_CHECK2))->SetCheck(g_ConfigData.bPositionFix);
	

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

//////////////////////////////////////////////////////////////////////////
// UI정보를 데이터변수로 저장하고 종료한다.
//////////////////////////////////////////////////////////////////////////
void CConfigDlg::OnBnClickedOk()
{
	
	CSliderCtrl* pSlider = static_cast<CSliderCtrl*>(GetDlgItem(IDC_SLIDER_TRANSPARENT));

	// 투명도
	g_ConfigData.nTransparent = pSlider->GetPos();
	// 항상위
	g_ConfigData.bAlwaysTop = ((CButton*)GetDlgItem(IDC_CHECK1))->GetCheck();
	// 위치고정
	g_ConfigData.bPositionFix = ((CButton*)GetDlgItem(IDC_CHECK2))->GetCheck();
	
	OnOK();
}

//////////////////////////////////////////////////////////////////////////
// UI 변경사항을 데이터변수로 저장않하고 종료한다.
//////////////////////////////////////////////////////////////////////////
void CConfigDlg::OnBnClickedCancel()
{
	AfxGetMainWnd()->PostMessage(UM_TRANSPARENT, 0, g_ConfigData.nTransparent);
	OnCancel();
}

//////////////////////////////////////////////////////////////////////////
// 투명값이 변경된 것을 MainDlg로 통지한다.
//////////////////////////////////////////////////////////////////////////
void CConfigDlg::OnNMReleasedcaptureSliderTransparent(NMHDR *pNMHDR, LRESULT *pResult)
{
	CSliderCtrl* pSlider = static_cast<CSliderCtrl*>(GetDlgItem(IDC_SLIDER_TRANSPARENT));

	int nPos = pSlider->GetPos();

	//AfxGetMainWnd()->PostMessage(UM_TRANSPARENT, 0, LPARAM(nPos));
	*pResult = 0;
}
