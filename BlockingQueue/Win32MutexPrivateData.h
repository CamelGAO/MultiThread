#ifndef _WIN32MUTEXPRIVATEDATA_H
#define _WIN32MUTEXPRIVATEDATA_H

#include <windows.h>

namespace OpenThreads {

	class Win32MutexPrivateData {
		friend class Mutex;
		friend class Condition;

	private:

		Win32MutexPrivateData();

		~Win32MutexPrivateData();
#define USE_CRITICAL_SECTION
#ifdef USE_CRITICAL_SECTION
		CRITICAL_SECTION _cs;
#else
		volatile unsigned long mutex;
#endif

	};

}

#endif





