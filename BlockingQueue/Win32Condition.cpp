#include "stdafx.h"

#include "Condition.h"
#include "Thread.h"
#include "Win32ConditionPrivateData.h"

using namespace OpenThreads;
Win32ConditionPrivateData::~Win32ConditionPrivateData()
{
}

//----------------------------------------------------------------------------
//
// Description: Constructor
//
// Use: public.
//
Condition::Condition() {
	Win32ConditionPrivateData *pd =
		new Win32ConditionPrivateData();
	_prvData = static_cast<void *>(pd);
}
//----------------------------------------------------------------------------
//
// Description: Destructor
//
// Use: public.
//
Condition::~Condition() {
	Win32ConditionPrivateData *pd =
		static_cast<Win32ConditionPrivateData *>(_prvData);

	delete pd;
}

//----------------------------------------------------------------------------
//
// Description: wait on a condition
//
// Use: public.
//
int Condition::wait(Mutex *mutex) {

	Win32ConditionPrivateData *pd =
		static_cast<Win32ConditionPrivateData *>(_prvData);

	return pd->wait(*mutex, INFINITE);
}
//----------------------------------------------------------------------------
//
// Description: wait on a condition, for a specified period of time
//
// Use: public.
//
int Condition::wait(Mutex *mutex, unsigned long ms) {

	Win32ConditionPrivateData *pd = 
		static_cast<Win32ConditionPrivateData *>(_prvData);

	return pd->wait(*mutex, ms);
}
//----------------------------------------------------------------------------
//
// Description: signal a thread to wake up.
//
// Use: public.
//
int Condition::signal() {

	Win32ConditionPrivateData *pd =
		static_cast<Win32ConditionPrivateData *>(_prvData);
	return pd->signal();
}
//----------------------------------------------------------------------------
//
// Description: signal many threads to wake up.
//
// Use: public.
//
int Condition::broadcast() {

	Win32ConditionPrivateData *pd =
		static_cast<Win32ConditionPrivateData *>(_prvData);
	return pd->broadcast();
}
