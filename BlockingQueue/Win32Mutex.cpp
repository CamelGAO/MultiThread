#include "stdafx.h"

#include "Mutex.h"
#include "Win32MutexPrivateData.h"
using namespace OpenThreads;

Win32MutexPrivateData::Win32MutexPrivateData()
{
#ifdef USE_CRITICAL_SECTION
	InitializeCriticalSection(&_cs);
#else
	mutex = 0;
#endif
}
Win32MutexPrivateData::~Win32MutexPrivateData()
{
#ifdef USE_CRITICAL_SECTION
	DeleteCriticalSection(&_cs);
#endif
}


#ifndef USE_CRITICAL_SECTION

template <int instance>
struct WIN32MutexSpin {

	enum { __low_max = 30, __high_max = 1000 };
	// Low if we suspect uniprocessor, high for multiprocessor.
	static unsigned __max;
	static unsigned __last;
};

template <int instance>
unsigned WIN32MutexSpin <instance>::__max = WIN32MutexSpin <instance>::__low_max;

template <int instance>
unsigned WIN32MutexSpin <instance>::__last = 0;



static void _S_nsec_sleep(int __log_nsec) {

	if (__log_nsec <= 20) {
		SwitchToThread(); //Sleep(0); // adegli replaced it Sleep by SwitchToThread
	}
	else {
		Sleep(1 << (__log_nsec - 20));
	}
}


#if defined(_MSC_VER) && _MSC_VER <= 1300
template WIN32MutexSpin <0>;
#endif

#endif // USE_CRITICAL_SECTION

//----------------------------------------------------------------------------
//
// Description: Constructor
//
// Use: public.
//
Mutex::Mutex(MutexType type) :
	_mutexType(type)
{
	Win32MutexPrivateData *pd = new Win32MutexPrivateData();
	_prvData = static_cast<void *>(pd);
}

//----------------------------------------------------------------------------
//
// Description: Destructor
//
// Use: public.
//
Mutex::~Mutex() {
	delete static_cast<Win32MutexPrivateData*>(_prvData);
}
//----------------------------------------------------------------------------
//
// Description: lock the mutex
//
// Use: public.
//
int Mutex::lock() {
	Win32MutexPrivateData *pd =
		static_cast<Win32MutexPrivateData*>(_prvData);

#ifdef USE_CRITICAL_SECTION

	// Block until we can take this lock.
	EnterCriticalSection(&(pd->_cs));

	return 0;

#else

	volatile unsigned long* lock = &pd->mutex;
	// InterlockedExchange returns old value
	// if old_value  == 0 mutex wasn't locked , now it is
	if (!InterlockedExchange((long*)lock, 1L)) {
		return 0;
	}

	unsigned my_spin_max = WIN32MutexSpin<0>::__max;
	unsigned my_last_spins = WIN32MutexSpin<0>::__last;
	volatile unsigned junk = 17;
	unsigned i;

	for (i = 0; i < my_spin_max; i++) {
		if (i < my_last_spins / 2 || *lock) {
			junk *= junk; junk *= junk;
			junk *= junk; junk *= junk;
			continue;
		}

		if (!InterlockedExchange((long*)lock, 1L)) {
			// got it!
			// Spinning worked.  Thus we're probably not being scheduled
			// against the other process with which we were contending.
			// Thus it makes sense to spin longer the next time.
			WIN32MutexSpin<0>::__last = i;
			WIN32MutexSpin<0>::__max = WIN32MutexSpin<0>::__high_max;
			return 0;
		}
	}
	// We are probably being scheduled against the other process.  Sleep.
	WIN32MutexSpin<0>::__max = WIN32MutexSpin<0>::__low_max;
	for (i = 0;; ++i) {
		int __log_nsec = i + 6;
		if (__log_nsec > 27) __log_nsec = 27;
		if (!InterlockedExchange((long*)lock, 1L)) {
			return 0;
		}
		_S_nsec_sleep(__log_nsec);
	}
	return -1;

#endif // USE_CRITICAL_SECTION
}

//----------------------------------------------------------------------------
//
// Description: unlock the mutex
//
// Use: public.
//
int Mutex::unlock() {
	Win32MutexPrivateData *pd =
		static_cast<Win32MutexPrivateData*>(_prvData);

#ifdef USE_CRITICAL_SECTION

	// Release this lock. CRITICAL_SECTION is nested, thus
	//   unlock() calls must be paired with lock() calls.
	LeaveCriticalSection(&(pd->_cs));

	return 0;

#else

	volatile unsigned long* lock = &pd->mutex;
	*lock = 0;
	// This is not sufficient on many multiprocessors, since
	// writes to protected variables and the lock may be reordered.
	return 0;

#endif // USE_CRITICAL_SECTION
}

//----------------------------------------------------------------------------
//
// Description: test if the mutex may be locked
//
// Use: public.
//
int Mutex::trylock() {
	Win32MutexPrivateData *pd =
		static_cast<Win32MutexPrivateData*>(_prvData);

#ifdef USE_CRITICAL_SECTION

	// Take the lock if we can; regardless don't block.
	// 'result' is FALSE if we took the lock or already held
	//   it amd TRUE if another thread already owns the lock.
	BOOL result = TryEnterCriticalSection(&(pd->_cs));

	return((result == TRUE) ? 0 : 1);

#else

	volatile unsigned long* lock = &pd->mutex;

	if (!InterlockedExchange((long*)lock, 1L)) {
		return 1; // TRUE
	}

	return 0; // FALSE

#endif // USE_CRITICAL_SECTION
}

