#include "version.hpp"
using namespace dte_module;
using namespace dte_utils;
version::version(char lowest, char low, char high, char highest) : value{lowest, low, high, highest} {}
version::version(dte_utils::static_array<char, 4> value) : value(value) {}
bool version::greater(const version& other, char precision) const {
	precision &= 0x3;	//if precision > 3: overflow
	++precision;
	while (precision) {
		--precision;
		if (value[precision] > other.value[precision]) {
			return true;
		}
	}
	return false;
}
bool version::equal(const version& other, char precision) const {
	precision &= 0x3;	//if precision > 3: overflow
	++precision;
	while (precision) {
		--precision;
		if (value[precision] != other.value[precision]) {
			return false;
		}
	}
	return true;
}
bool version::less(const version& other, char precision) const {
	precision &= 0x3;	//if precision > 3: overflow
	++precision;
	while (precision) {
		--precision;
		if (value[precision] < other.value[precision]) {
			return true;
		}
	}
	return false;
}