#include "pch.h"
#include "ConfigurationDepth.h"

#include "../GISLibrary/CoverageDataset.h"

#include <algorithm>
#include <chrono>
#include <format>

IMPLEMENT_DYNAMIC(CConfigurationDepth, CDialogEx)

namespace
{
	using GISLibrary::CoverageDataset;
	using s100::coverage::TimePoint;
	using s100::coverage::WlaMethod;

	enum MethodIndex { MethodOff, MethodInstant, MethodPeriod };

	SYSTEMTIME toSystemTime(TimePoint time)
	{
		using namespace std::chrono;
		const auto day = floor<days>(time);
		const year_month_day date{ day };
		const hh_mm_ss clock{ floor<seconds>(time - day) };
		SYSTEMTIME st{};
		st.wYear = static_cast<WORD>(static_cast<int>(date.year()));
		st.wMonth = static_cast<WORD>(static_cast<unsigned>(date.month()));
		st.wDay = static_cast<WORD>(static_cast<unsigned>(date.day()));
		st.wHour = static_cast<WORD>(clock.hours().count());
		st.wMinute = static_cast<WORD>(clock.minutes().count());
		st.wSecond = static_cast<WORD>(clock.seconds().count());
		return st;
	}

	// A UTC date from one picker and a time of day from the other.
	TimePoint fromPickers(CDateTimeCtrl& datePicker, CDateTimeCtrl& timePicker)
	{
		using namespace std::chrono;
		SYSTEMTIME date{};
		SYSTEMTIME time{};
		datePicker.GetTime(&date);
		timePicker.GetTime(&time);
		const sys_days day{ year{ date.wYear } / month{ date.wMonth } / std::chrono::day{ date.wDay } };
		return time_point_cast<milliseconds>(day + hours{ time.wHour } + minutes{ time.wMinute } + seconds{ time.wSecond });
	}

	void setPickers(CDateTimeCtrl& datePicker, CDateTimeCtrl& timePicker, TimePoint time)
	{
		SYSTEMTIME st = toSystemTime(time);
		datePicker.SetTime(&st);
		timePicker.SetTime(&st);
	}

	TimePoint now()
	{
		return std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now());
	}
}

CConfigurationDepth::CConfigurationDepth(CWnd* pParent)
	: CDialogEx(IDD_DIALOG_CONFIG_DEPTH, pParent)
{
}

void CConfigurationDepth::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_DEPTH_ESC, m_esc);
	DDX_Control(pDX, IDC_DEPTH_UNCERTAINTY, m_uncertainty);
	DDX_Control(pDX, IDC_DEPTH_IGNORE_DATUM, m_ignoreDatum);
	DDX_Control(pDX, IDC_DEPTH_WLA_METHOD, m_method);
	DDX_Control(pDX, IDC_DEPTH_WLA_DATE, m_atDate);
	DDX_Control(pDX, IDC_DEPTH_WLA_TIME, m_atTime);
	DDX_Control(pDX, IDC_DEPTH_WLA_START_DATE, m_fromDate);
	DDX_Control(pDX, IDC_DEPTH_WLA_START_TIME, m_fromTime);
	DDX_Control(pDX, IDC_DEPTH_WLA_END_DATE, m_toDate);
	DDX_Control(pDX, IDC_DEPTH_WLA_END_TIME, m_toTime);
	DDX_Control(pDX, IDC_CURRENT_RECORD, m_currentRecord);
	DDX_Control(pDX, IDC_CURRENT_THINNING, m_currentThinning);
}

BEGIN_MESSAGE_MAP(CConfigurationDepth, CDialogEx)
	ON_CBN_SELCHANGE(IDC_DEPTH_WLA_METHOD, &CConfigurationDepth::OnMethodChanged)
	ON_BN_CLICKED(IDC_DEPTH_FROM_S104, &CConfigurationDepth::OnFromS104)
END_MESSAGE_MAP()

BOOL CConfigurationDepth::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_method.AddString(L"Off");
	m_method.AddString(L"At a time (Option 1)");
	m_method.AddString(L"Over a period (Option 2)");

	const auto& settings = CoverageDataset::GetDepthSettings();
	m_esc.SetCheck(settings.enhancedSafetyContour ? BST_CHECKED : BST_UNCHECKED);
	m_uncertainty.SetCheck(settings.adjustment.uncertainty ? BST_CHECKED : BST_UNCHECKED);
	m_ignoreDatum.SetCheck(settings.adjustment.ignoreIncompatibleDatums ? BST_CHECKED : BST_UNCHECKED);

	const WlaMethod& wla = settings.adjustment.wla;
	m_method.SetCurSel(wla.kind == WlaMethod::Kind::Instant ? MethodInstant : wla.kind == WlaMethod::Kind::Period ? MethodPeriod : MethodOff);
	// Times not set yet start from the S-104 records, or from now.
	const auto span = CoverageDataset::WaterLevelTimeSpan();
	const TimePoint start = span ? span->first : now();
	setPickers(m_atDate, m_atTime, wla.kind == WlaMethod::Kind::Instant ? wla.time : start);
	setPickers(m_fromDate, m_fromTime, wla.kind == WlaMethod::Kind::Period ? wla.start : start);
	setPickers(m_toDate, m_toTime, wla.kind == WlaMethod::Kind::Period ? wla.end : start + std::chrono::hours(6));

	// S-111: the system clock, or one of the record times of the S-111 on the chart. A time set
	// before that is not a record of what is on the chart now stays in the list.
	const auto& display = CoverageDataset::GetDisplaySettings();
	m_currentThinning.SetCheck(display.thinSymbols ? BST_CHECKED : BST_UNCHECKED);
	m_currentTimes = CoverageDataset::DisplayRecordTimes();
	const std::size_t recordCount = m_currentTimes.size();
	if (display.time && !std::ranges::binary_search(m_currentTimes, *display.time))
	{
		m_currentTimes.insert(std::ranges::upper_bound(m_currentTimes, *display.time), *display.time);
	}
	const bool nowHasRecord = recordCount > 0 && now() >= m_currentTimes.front() && now() <= m_currentTimes.back();
	m_currentRecord.AddString(nowHasRecord ? L"System clock (now)" : L"System clock (now): no record");
	int selected = 0;
	for (std::size_t i = 0; i < m_currentTimes.size(); ++i)
	{
		m_currentRecord.AddString(CString(s100::coverage::formatWlaDateTime(m_currentTimes[i]).c_str()));
		if (display.time && m_currentTimes[i] == *display.time)
		{
			selected = static_cast<int>(i) + 1;
		}
	}
	m_currentRecord.SetCurSel(selected);
	const std::string recordsText = recordCount > 0
		? std::format("{} records, {} to {}", recordCount, s100::coverage::formatWlaDateTime(m_currentTimes.front()),
			s100::coverage::formatWlaDateTime(m_currentTimes.back()))
		: std::string("No S-111 on the chart.");
	SetDlgItemTextW(IDC_CURRENT_SPAN, CString(recordsText.c_str()));

	ShowS104Span();
	EnableTimes();
	return TRUE;
}

void CConfigurationDepth::EnableTimes()
{
	const int method = m_method.GetCurSel();
	for (CWnd* control : std::initializer_list<CWnd*>{ &m_atDate, &m_atTime })
	{
		control->EnableWindow(method == MethodInstant);
	}
	for (CWnd* control : std::initializer_list<CWnd*>{ &m_fromDate, &m_fromTime, &m_toDate, &m_toTime })
	{
		control->EnableWindow(method == MethodPeriod);
	}
	m_ignoreDatum.EnableWindow(method != MethodOff);
}

void CConfigurationDepth::ShowS104Span()
{
	const auto span = CoverageDataset::WaterLevelTimeSpan();
	const std::string text = span
		? "S-104 records from " + s100::coverage::formatWlaDateTime(span->first) + " to " + s100::coverage::formatWlaDateTime(span->second)
		: std::string("No S-104 open: water level adjustment needs S-104 on the chart.");
	SetDlgItemTextW(IDC_DEPTH_S104_SPAN, CString(text.c_str()));
	GetDlgItem(IDC_DEPTH_FROM_S104)->EnableWindow(span.has_value());
}

void CConfigurationDepth::OnMethodChanged()
{
	EnableTimes();
}

void CConfigurationDepth::OnFromS104()
{
	// The first S-104 record, and a period of up to six hours from it (as opens100-web's viewer).
	const auto span = CoverageDataset::WaterLevelTimeSpan();
	if (!span)
	{
		return;
	}
	setPickers(m_atDate, m_atTime, span->first);
	setPickers(m_fromDate, m_fromTime, span->first);
	setPickers(m_toDate, m_toTime, (std::min)(span->second, span->first + std::chrono::hours(6)));
}

bool CConfigurationDepth::Apply()
{
	if (!GetSafeHwnd())
	{
		return false;
	}
	// S-111: nothing is computed for these; the next drawing reads them.
	CoverageDataset::DisplaySettings display = CoverageDataset::GetDisplaySettings();
	const CoverageDataset::DisplaySettings displayBefore = display;
	const int record = m_currentRecord.GetCurSel();
	display.time = record > 0 && static_cast<std::size_t>(record) <= m_currentTimes.size()
		? std::optional<TimePoint>(m_currentTimes[static_cast<std::size_t>(record) - 1]) : std::nullopt;
	display.thinSymbols = m_currentThinning.GetCheck() == BST_CHECKED;
	const bool displayChanged = display.time != displayBefore.time || display.thinSymbols != displayBefore.thinSymbols;
	if (displayChanged)
	{
		CoverageDataset::SetDisplaySettings(display);
	}
	CoverageDataset::DepthSettings settings = CoverageDataset::GetDepthSettings();
	const CoverageDataset::DepthSettings before = settings;
	settings.enhancedSafetyContour = m_esc.GetCheck() == BST_CHECKED;
	settings.adjustment.uncertainty = m_uncertainty.GetCheck() == BST_CHECKED;
	settings.adjustment.ignoreIncompatibleDatums = m_ignoreDatum.GetCheck() == BST_CHECKED;
	switch (m_method.GetCurSel())
	{
	case MethodInstant:
		settings.adjustment.wla = WlaMethod::instant(fromPickers(m_atDate, m_atTime));
		break;
	case MethodPeriod:
		settings.adjustment.wla = WlaMethod::period(fromPickers(m_fromDate, m_fromTime), fromPickers(m_toDate, m_toTime));
		break;
	default:
		settings.adjustment.wla = WlaMethod::off();
		break;
	}
	const auto& a = before.adjustment;
	const auto& b = settings.adjustment;
	const bool changed = before.enhancedSafetyContour != settings.enhancedSafetyContour || a.uncertainty != b.uncertainty
		|| a.ignoreIncompatibleDatums != b.ignoreIncompatibleDatums || a.wla.kind != b.wla.kind || a.wla.time != b.wla.time
		|| a.wla.start != b.wla.start || a.wla.end != b.wla.end;
	if (changed)
	{
		CoverageDataset::SetDepthSettings(settings);
	}
	return changed || displayChanged;
}
