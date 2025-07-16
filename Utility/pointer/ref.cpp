#include "ref.hpp"
using namespace dte_utils;
ref_counter::ref_counter(size_type weak, size_type strong) : _weak_owners(weak), _strong_owners(strong) {}
ref_counter::size_type ref_counter::get_weak() const noexcept {
	return _weak_owners;
}
ref_counter::size_type ref_counter::get_strong() const noexcept {
	return _strong_owners;
}
ref_counter::size_type ref_counter::add_weak() noexcept {
	++_weak_owners;
	return _weak_owners;
}
ref_counter::size_type ref_counter::add_strong() noexcept {
	++_strong_owners;
	return _strong_owners;
}
ref_counter::size_type ref_counter::sub_weak() noexcept {
	--_weak_owners;
	return _weak_owners;
}
ref_counter::size_type ref_counter::sub_strong() noexcept {
	--_strong_owners;
	return _strong_owners;
}



atomic_ref_counter::atomic_ref_counter(size_type weak, size_type strong) : _weak_owners(weak), _strong_owners(strong) {}
atomic_ref_counter::size_type atomic_ref_counter::get_weak() const noexcept { 
	return _weak_owners; 
}
atomic_ref_counter::size_type atomic_ref_counter::get_strong() const noexcept { 
	return _strong_owners; 
}
atomic_ref_counter::size_type atomic_ref_counter::add_weak() noexcept { 
	return ++_weak_owners; 
}
atomic_ref_counter::size_type atomic_ref_counter::add_strong() noexcept { 
	return ++_strong_owners; 
}
atomic_ref_counter::size_type atomic_ref_counter::sub_weak() noexcept { 
	return --_weak_owners; 
}
atomic_ref_counter::size_type atomic_ref_counter::sub_strong() noexcept {
	return --_strong_owners; 
}
