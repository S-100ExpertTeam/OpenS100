#pragma once
#include "resource.h"

#include "../Coverage/CoverageTime.h"

#include <vector>

// The "S-102 / S-104 / S-111" tab of the settings window: the S-98 Appendix D depth settings, the
// enhanced safety contour (D-1.4), vertical uncertainty (D-1.5) and water level adjustment by
// S-104 (D-2, Options 1 and 2). They take effect for every S-102 dataset on Apply. And how S-111
// surface currents are shown: the time whose record is drawn (S-111 9.4) and whether the arrows
// are thinned as the chart is zoomed out (9.3.2).
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

	// S-111: "System clock", then every record time of the S-111 on the chart; the time of item
	// i > 0 is m_currentTimes[i - 1].
	CComboBox m_currentRecord;
	std::vector<s100::coverage::TimePoint> m_currentTimes;
	CButton m_currentThinning;
};
