#ifndef _WIN32PRIVATEDATA_H
#define _WIN32PRIVATEDATA_H

#include "Thread.h"
#include "Block.h"
#include "HandleHolder.h"

namespace OpenThreads {

	class Win32ThreadPrivateData {
		//-------------------------------------------------------------------------
		// We're friendly to Thread, so it can use our data.
		//
		friend class Thread;
		//-------------------------------------------------------------------------
		// We're friendly to Win32PrivateActions, so it can get at some 
		// variables.
		//
		friend class ThreadPrivateActions;

	private:

		Win32ThreadPrivateData();
		~Win32ThreadPrivateData();

		size_t stackSize;
		bool isRunning;

		Block threadStartedBlock;

		int  cancelMode; // 0 - deffered (default) 1-asynch 2-disabled  

		bool detached;

		Thread::ThreadPriority threadPriority;

		Thread::ThreadPolicy threadPolicy;

		HandleHolder tid;

		int uniqueId;

		int cpunum;

	public:

		HandleHolder cancelEvent;

		struct TlsHolder { // thread local storage slot
			DWORD getId()
			{
				if (!initialized) {
					ID = TlsAlloc();
					initialized = true;
				}
				return ID;
			}
			TlsHolder() : ID(TLS_OUT_OF_INDEXES), initialized(false) {}
			~TlsHolder() {
				if (initialized)
					TlsFree(ID);
				ID = TLS_OUT_OF_INDEXES;
			}
		private:
			DWORD ID;
			bool initialized;
		};

		static TlsHolder TLS;

	};

	DWORD cooperativeWait(HANDLE waitHandle, unsigned long timeout);


}

#endif



