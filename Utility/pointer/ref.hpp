#pragma once
#include "function_traits.hpp"
#include <concepts>
#include <atomic>
namespace dte_utils {
	template<typename T>
	inline constexpr bool is_ref_counter_v = requires(T t) {
		T(
			std::declval<typename T::size_type>(), 
			std::declval<typename T::size_type>()
		);

		{ t.get_weak()		} -> std::same_as<typename T::size_type>;
		{ t.get_strong()	} -> std::same_as<typename T::size_type>;

		{ t.add_weak()		} -> std::same_as<typename T::size_type>;
		{ t.add_strong()	} -> std::same_as<typename T::size_type>;
		{ t.sub_weak()		} -> std::same_as<typename T::size_type>;
		{ t.sub_strong()	} -> std::same_as<typename T::size_type>;
	};
	struct ref_counter {
		using size_type = size_t;
		protected:
			size_type _weak_owners;
			size_type _strong_owners;
		public:
			ref_counter(size_type weak, size_type strong);
			ref_counter(const ref_counter&) = delete;
			size_type get_weak() const noexcept;
			size_type get_strong() const noexcept;
			size_type add_weak() noexcept;
			size_type add_strong() noexcept;
			size_type sub_weak() noexcept;
			size_type sub_strong() noexcept;
			ref_counter& operator=(const ref_counter&) = delete;
	};
	struct atomic_ref_counter {
		using size_type = size_t;
		protected:
			std::atomic<size_type> _weak_owners;
			std::atomic<size_type> _strong_owners;
		public:
			atomic_ref_counter(size_type weak, size_type strong);
			atomic_ref_counter(const atomic_ref_counter&) = delete;
			size_type get_weak() const noexcept;
			size_type get_strong() const noexcept;
			size_type add_weak() noexcept;
			size_type add_strong() noexcept;
			size_type sub_weak() noexcept;
			size_type sub_strong() noexcept;
			atomic_ref_counter& operator=(const atomic_ref_counter&) = delete;
	};
}