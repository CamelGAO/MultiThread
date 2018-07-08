#ifndef _BARRIER_H
#define _BARRIER_H

namespace OpenThreads {


	/**
	*  @class Barrier
	*  @brief This class provides an object-oriented thread barrier interface
	*
	*  @warning It is unwise to use the construct "Barrier barrier" in the
	*           global namespace on sgi's.  The object "barrier"
	*           will confilict with the c-library sproc function "barrier" and
	*           unpredictable results may occur. You have been warned.
	*/
	class Barrier {

	public:

		/**
		*  Constructor
		*/
		Barrier(int numThreads = 0);

		/**
		*  Destructor
		*/
		virtual ~Barrier();

		/**
		*  Reset the barrier to it's original state.
		*/
		virtual void reset();

		/**
		*  Block until numThreads threads have entered the barrier.
		*/
		virtual void block(unsigned int numThreads = 0);

		/**
		*  Release the barrier, now.
		*/
		virtual void release();

		/**
		*  Return the number of threads currently blocked in the barrier,
		*  Return -1 if error.
		*/
		virtual int numThreadsCurrentlyBlocked();


		void invalidate();

	private:

		/**
		*  Private copy constructor, to prevent tampering.
		*/
		Barrier(const Barrier &/*b*/) {};

		/**
		*  Private copy assignment, to prevent tampering.
		*/
		Barrier &operator=(const Barrier &/*b*/) { return *(this); };

		/**
		*  Implementation-specific private data.
		*/
		void *_prvData;


		bool _valid;

	};

}

#endif

