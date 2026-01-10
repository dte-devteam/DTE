#pragma once
#include "ref_counter.hpp"
#include "complex_pointer.hpp"
namespace dte_utils {
	//_counter = invalid/nullptr -> UB
	template<typename RC = ref_counter>
	requires is_ref_counter_v<RC>
	struct ref_base {
		using size_type = typename RC::size_type;
		protected:
			complex_pointer<RC> _counter;
			void _weak_decrease() noexcept(std::is_nothrow_destructible_v<RC>) {
				if (!_counter->sub_weak()) {
					cdelete(_counter);
				}
			}
		public:
			ref_base(const raw_pointer<RC>& counter) noexcept : _counter(counter) {}
			ref_base(const ref_base& other) noexcept : _counter(other._counter) {
				_counter->add_weak();
			}

			~ref_base() {
				_weak_decrease();
			}

			complex_pointer<const RC> get_counter() const noexcept {
				return _counter;
			}
			bool expired() const noexcept {
				return !_counter->get_strong();
			}
	};
}