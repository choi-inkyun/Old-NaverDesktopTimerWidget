// ConfigDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "NaverTimer(StopWatch)ByDingpong.h"
#include "ConfigDlg.h"
#include ".\configdlg.h"


// CConfigDlg ��ȭ �����Դϴ�.

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


// CConfigDlg �޽��� ó�����Դϴ�.

//////////////////////////////////////////////////////////////////////////
// �ʱ�ȭ �۾� : �����ͷ� UI�� �����Ѵ�. 
//////////////////////////////////////////////////////////////////////////
BOOL CConfigDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// �����̴� Ctrl�� ��´�.
	CSliderCtrl* pSlider = static_cast<CSliderCtrl*>(GetDlgItem(IDC_SLIDER_TRANSPARENT));	
	pSlider->SetRange(20, 255);
	
	// ���� ����
	pSlider->SetPos(g_ConfigData.nTransparent);
	// �׻��� ����
	((CButton*)GetDlgItem(IDC_CHECK1))->SetCheck(g_ConfigData.bAlwaysTop);
	// ��ġ���� ����
	((CButton*)GetDlgItem(IDC_CHECK2))->SetCheck(g_ConfigData.bPositionFix);
	

	return TRUE;  // return TRUE unless you set the focus to a control
	// ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}

//////////////////////////////////////////////////////////////////////////
// UI������ �����ͺ����� �����ϰ� �����Ѵ�.
//////////////////////////////////////////////////////////////////////////
void CConfigDlg::OnBnClickedOk()
{
	
	CSliderCtrl* pSlider = static_cast<CSliderCtrl*>(GetDlgItem(IDC_SLIDER_TRANSPARENT));

	// ����
	g_ConfigData.nTransparent = pSlider->GetPos();
	// �׻���
	g_ConfigData.bAlwaysTop = ((CButton*)GetDlgItem(IDC_CHECK1))->GetCheck();
	// ��ġ����
	g_ConfigData.bPositionFix = ((CButton*)GetDlgItem(IDC_CHECK2))->GetCheck();
	
	OnOK();
}

//////////////////////////////////////////////////////////////////////////
// UI ��������� �����ͺ����� ������ϰ� �����Ѵ�.
//////////////////////////////////////////////////////////////////////////
void CConfigDlg::OnBnClickedCancel()
{
	AfxGetMainWnd()->PostMessage(UM_TRANSPARENT, 0, g_ConfigData.nTransparent);
	OnCancel();
}

//////////////////////////////////////////////////////////////////////////
// ������ ����� ���� MainDlg�� �����Ѵ�.
//////////////////////////////////////////////////////////////////////////
void CConfigDlg::OnNMReleasedcaptureSliderTransparent(NMHDR *pNMHDR, LRESULT *pResult)
{
	CSliderCtrl* pSlider = static_cast<CSliderCtrl*>(GetDlgItem(IDC_SLIDER_TRANSPARENT));

	int nPos = pSlider->GetPos();

	//AfxGetMainWnd()->PostMessage(UM_TRANSPARENT, 0, LPARAM(nPos));
	*pResult = 0;
}
