#ifndef _CONDITION_H
#define _CONDITION_H

#include "Mutex.h"

namespace OpenThreads {

	/**
	*  @class Condition
	*  @brief  This class provides an object-oriented thread condition interface.
	*/
	class Condition {

	public:

		/**
		*  Constructor
		*/
		Condition();

		/**
		*  Destructor
		*/
		virtual ~Condition();

		/**
		*  Wait on a mutex.
		*/
		virtual int wait(Mutex *mutex);

		/**
		*  Wait on a mutex for a given amount of time (ms)
		*
		*  @return 0 if normal, -1 if errno set, errno code otherwise.
		*/
		virtual int wait(Mutex *mutex, unsigned long int ms);

		/**
		*  Signal a SINGLE thread to wake if it's waiting.
		*
		*  @return 0 if normal, -1 if errno set, errno code otherwise.
		*/
		virtual int signal();

		/**
		*  Wake all threads waiting on this condition.
		*
		*  @return 0 if normal, -1 if errno set, errno code otherwise.
		*/
		virtual int broadcast();

	private:

		/**
		*  Private copy constructor, to prevent tampering.
		*/
		Condition(const Condition &/*c*/) {};

		/**
		*  Private copy assignment, to prevent tampering.
		*/
		Condition &operator=(const Condition &/*c*/) { return *(this); };

		/**
		*  Implementation-specific data
		*/
		void *_prvData;

	};

}

#endif
