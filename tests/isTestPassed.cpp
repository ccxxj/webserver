#include "catch.hpp"

// TODO: remove testing functions

bool isTestPassed(int num) {
	if (num == 1) {
		return true;
	} else {
		return false;
	}
}

TEST_CASE("Initial test", "[test]") {
	int num = 0;
	CHECK(isTestPassed(num) == true);
}