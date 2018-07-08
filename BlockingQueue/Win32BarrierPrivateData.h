#ifndef _WIN32BARRIERPRIVATEDATA_H
#define _WIN32BARRIERPRIVATEDATA_H

#include "Mutex.h"
#include "Condition.h"

namespace OpenThreads {

	class Barrier;

	class Win32BarrierPrivateData {
		friend class Barrier;
	private:
		Win32BarrierPrivateData(int mc, int c, int p) :
			maxcnt(mc), cnt(c), phase(p) {}

		~Win32BarrierPrivateData();

		Condition cond;            // cv for waiters at barrier

		Mutex    lock;            // mutex for waiters at barrier

		volatile int       maxcnt;          // number of threads to wait for

		volatile int       cnt;             // number of waiting threads

		volatile int       phase;           // flag to seperate two barriers


	};

}

#endif



