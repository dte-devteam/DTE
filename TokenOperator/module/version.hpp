#pragma once
#include "memory/static_array.hpp"
namespace dte_module {
	struct version {
		protected:
			dte_utils::static_array<char, 4> value;
		public:
			version(char lowest, char low, char high, char highest);
			version(dte_utils::static_array<char, 4> value = {0,0,0,0});
			bool greater(const version& other, char precision = 3) const;
			bool equal(const version& other, char precision = 3) const;
			bool less(const version& other, char precision = 3) const;
	};
}