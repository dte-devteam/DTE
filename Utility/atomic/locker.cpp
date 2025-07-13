#include "locker.hpp"
using namespace dte_utils;
locker::locker() : _locked(false) {}
void locker::lock() {
	_locked.wait(true);
	_locked.store(true);
}
void locker::unlock() {
	_locked.store(false);
	_locked.notify_one();
}
bool locker::is_locked() {
	return _locked;
}