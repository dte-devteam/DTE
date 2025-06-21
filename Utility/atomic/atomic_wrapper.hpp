#pragma once
#include <atomic>
namespace dte_utils {
	template<typename T>
	struct atomic_wrapper {
		std::atomic<T> value;
		atomic_wrapper(const T& value) : value(value) {}
		atomic_wrapper(const atomic_wrapper& other) : value(other.value.load()) {}
		atomic_wrapper& operator=(const atomic_wrapper& other) {
			if (this == &other) {
				return *this;
			}
			value.store(other.value.load());
			return *this;
		}
	};
}