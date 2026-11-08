#include "TestPaths.h"
#include <windows.h>
#include <cstdlib>

namespace fs = std::filesystem;

namespace testpaths
{
	fs::path repoRoot()
	{
		wchar_t buf[MAX_PATH];
		::GetModuleFileNameW(nullptr, buf, MAX_PATH);
		return fs::path(buf).parent_path().parent_path();
	}

	fs::path tdsRoot()
	{
		wchar_t* value = nullptr;
		size_t len = 0;
		if (_wdupenv_s(&value, &len, L"S100_TDS_ROOT") == 0 && value != nullptr)
		{
			fs::path p(value);
			free(value);
			return p;
		}
		return fs::path(L"D:\\s100-reference\\tds");
	}

	fs::path korS101Cell()
	{
		return tdsRoot() / L"KOR_TDS" / L"S100_ROOT" / L"S-101" / L"DATASET_FILES" / L"101KR003G2M00" / L"101KR003G2M00.000";
	}

	fs::path korS101Catalog()
	{
		return tdsRoot() / L"KOR_TDS" / L"S100_ROOT" / L"CATALOG.XML";
	}

	fs::path korS124Gml()
	{
		return tdsRoot() / L"KOR_TDS" / L"KOR_S-124" / L"S-124" / L"124KR.KCG.XI.S100.TEST1.gml";
	}

	fs::path tempDir()
	{
		auto dir = fs::temp_directory_path() / L"OpenS100Tests";
		fs::create_directories(dir);
		return dir;
	}
}
