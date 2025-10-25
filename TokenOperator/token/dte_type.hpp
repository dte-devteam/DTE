#pragma once
#include "pointer/strong_ref.hpp"
#include "memory/dynamic_string.hpp"
namespace dte_token {
	struct dte_type {
		struct super_type {
			//data offset in bytes
			size_t offset;
			dte_utils::atomic_strong_ref<dte_type> parent;
		};
		//protected:
		void add_parent(const dte_utils::atomic_strong_ref<dte_type>& parent) {
			_parents.emplace_back(get_size(), parent);
			_size += parent->get_size();
		}
		size_t _size;
		dte_utils::dynamic_cstring _name;
		//arrays are not thread safe!
		dte_utils::dynamic_array<super_type> _parents;
		//arrays are not thread safe!
		dte_utils::dynamic_array<dte_utils::atomic_weak_ref<dte_type>> _childs;
		//public:
		size_t get_size() const noexcept {
			return _size;
		}
		const dte_utils::dynamic_cstring& get_name() const noexcept {
			return _name;
		}
		bool is_basic_type() const noexcept {
			return !_parents.get_used();
		}
	};
}