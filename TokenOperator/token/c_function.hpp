#pragma once
#include "memory/dynamic_string.hpp"
#include "semi_pointer.hpp"
namespace dte_token {
	struct data_stack;
	struct c_function {
		struct metadata {
			dte_utils::dynamic_cstring name;
			size_t max_jump;
			//bool accepts_ptr;
			/*
			later add metadata for complex function building
			*/
		};
		typedef size_t(*func)(data_stack&, const semi_pointer::data&);
		protected:
			metadata _meta;
			func _body;
		public:
			c_function(func body, const metadata& meta);
			c_function(func body, metadata&& meta);
			c_function(const c_function&) = delete;
			c_function(c_function&& other) noexcept;
			const metadata& get_meta() const;
			const func get_body() const;
			c_function& operator=(const c_function&) = delete;
			c_function& operator=(c_function&&) = delete;
			size_t operator()(data_stack& stack, const semi_pointer::data& spd) const;
	};
}