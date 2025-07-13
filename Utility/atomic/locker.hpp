#pragma once
#include <atomic>
namespace dte_utils {
	struct locker {
		protected:
			std::atomic_bool _locked;
		public:
			locker();
			locker(const locker&) = delete;
			void lock();
			void unlock();
			bool is_locked();
			locker& operator=(const locker&) = delete;
	};
}