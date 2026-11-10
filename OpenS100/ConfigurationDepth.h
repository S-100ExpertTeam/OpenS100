#pragma once
#include "resource.h"

// The "S-102 / S-104" tab of the settings window: the S-98 Appendix D depth settings, the
// enhanced safety contour (D-1.4), vertical uncertainty (D-1.5) and water level adjustment by
// S-104 (D-2, Options 1 and 2). They take effect for every S-102 dataset on Apply.
class CConfigurationDepth : public CDialogEx
{
	DECLARE_DYNAMIC(CConfigurationDepth)

public:
	explicit CConfigurationDepth(CWnd* pParent = nullptr);
	~CConfigurationDepth() override = default;

	enum { IDD = IDD_DIALOG_CONFIG_DEPTH };

	// Hands the controls' values to the S-102 datasets; true when they changed.
	bool Apply();

protected:
	void DoDataExchange(CDataExchange* pDX) override;
	BOOL OnInitDialog() override;
	void OnOK() override {}
	void OnCancel() override {}
	DECLARE_MESSAGE_MAP()

	afx_msg void OnMethodChanged();
	afx_msg void OnFromS104();

private:
	void EnableTimes();
	void ShowS104Span();

	CButton m_esc;
	CButton m_uncertainty;
	CButton m_ignoreDatum;
	CComboBox m_method;
	CDateTimeCtrl m_atDate, m_atTime;
	CDateTimeCtrl m_fromDate, m_fromTime;
	CDateTimeCtrl m_toDate, m_toTime;
};
