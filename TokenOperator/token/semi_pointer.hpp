#pragma once
namespace dte_token {
	struct semi_pointer {
		union data {
			data() {}
			data(void* ptr) noexcept;
			data(size_t offset) noexcept;
			void* ptr;
			size_t offset;
		};
		protected:
			data _spu;
			bool _real_ptr;
		public:
			semi_pointer(void* ptr) noexcept;
			semi_pointer(size_t offset) noexcept;
			semi_pointer(const semi_pointer& other) noexcept;
			bool is_real_ptr() const noexcept;
			const data& get_spu() const noexcept;
			semi_pointer& operator=(void* ptr) noexcept;
			semi_pointer& operator=(size_t offset) noexcept;
			semi_pointer& operator=(const semi_pointer& other) noexcept;
	};
}