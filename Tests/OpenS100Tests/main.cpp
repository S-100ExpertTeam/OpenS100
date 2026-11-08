// OpenS100 unit tests (GoogleTest).
// GISLibrary still depends on MFC (CString, CFile), so the test host initialises
// MFC before running the suites. That dependency is slated for removal in Phase 1.
#include <afxwin.h>
#include <gtest/gtest.h>

int main(int argc, char** argv)
{
	if (!AfxWinInit(::GetModuleHandle(nullptr), nullptr, ::GetCommandLine(), 0))
	{
		fprintf(stderr, "AfxWinInit failed\n");
		return 1;
	}

	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
