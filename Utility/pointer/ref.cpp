#include "ref.hpp"
#include <atomic>
using namespace dte_utils;
ref_counter::ref_counter(size_type weak, size_type strong) : _weak_owners(weak), _strong_owners(strong) {}
ref_counter::size_type ref_counter::get_weak() const noexcept {
	return _weak_owners;
}
ref_counter::size_type ref_counter::get_strong() const noexcept {
	return _strong_owners;
}
ref_counter::size_type ref_counter::add_weak() {
	++_weak_owners;
	return _weak_owners;
}
ref_counter::size_type ref_counter::add_strong() {
	++_strong_owners;
	return _strong_owners;
}
ref_counter::size_type ref_counter::sub_weak() {
	--_weak_owners;
	return _weak_owners;
}
ref_counter::size_type ref_counter::sub_strong() {
	--_strong_owners;
	return _strong_owners;
}
