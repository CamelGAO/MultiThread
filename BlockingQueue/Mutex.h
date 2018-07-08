#ifndef _MUTEX_H
#define _MUTEX_H

namespace OpenThreads {

	/**
	*  @class Mutex
	*  @brief  This class provides an object-oriented thread mutex interface.
	*/
	class Mutex {

		friend class Condition;

	public:

		enum MutexType
		{
			MUTEX_NORMAL,
			MUTEX_RECURSIVE
		};

		/**
		*  Constructor
		*/
		Mutex(MutexType type = MUTEX_NORMAL);

		/**
		*  Destructor
		*/
		virtual ~Mutex();


		MutexType getMutexType() const { return _mutexType; }


		/**
		*  Lock the mutex
		*
		*  @return 0 if normal, -1 if errno set, errno code otherwise.
		*/
		virtual int lock();

		/**
		*  Unlock the mutex
		*
		*  @return 0 if normal, -1 if errno set, errno code otherwise.
		*/
		virtual int unlock();

		/**
		*  Test if mutex can be locked.
		*
		*  @return 0 if normal, -1 if errno set, errno code otherwise.
		*/
		virtual int trylock();

	private:

		/**
		*  Private copy constructor, to prevent tampering.
		*/
		Mutex(const Mutex &/*m*/) {};

		/**
		*  Private copy assignment, to prevent tampering.
		*/
		Mutex &operator=(const Mutex &/*m*/) { return *(this); };

		/**
		*  Implementation-specific private data.
		*/
		void *_prvData;
		MutexType _mutexType;

	};

}

#endif // _OPENTHREADS_MUTEX_
