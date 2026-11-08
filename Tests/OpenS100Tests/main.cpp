// OpenS100 unit tests (GoogleTest).
// The core libraries build without MFC since Phase 1 step 3-5, so this is a plain gtest host.
#include <gtest/gtest.h>

int main(int argc, char** argv)
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
