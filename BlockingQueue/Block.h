#ifndef _BLOCK_H
#define _BLOCK_H

#include "Thread.h"
#include "Barrier.h"
#include "Condition.h"
#include "ScopedLock.h"

namespace OpenThreads {

	/** Block is a block that can be used to halt a thread that is waiting another thread to release it.*/
	class Block
	{
	public:

		Block() :
			_released(false) {}

		~Block()
		{
			release();
		}

		inline bool block()
		{
			ScopedLock<OpenThreads::Mutex> mutlock(_mut);
			if (!_released)
			{
				return _cond.wait(&_mut) == 0;
			}
			else
			{
				return true;
			}
		}

		inline bool block(unsigned long timeout)
		{
			ScopedLock<OpenThreads::Mutex> mutlock(_mut);
			if (!_released)
			{
				return _cond.wait(&_mut, timeout) == 0;
			}
			else
			{
				return true;
			}
		}

		inline bool block(Mutex& external_mutex)
		{
			ReverseScopedLock<Mutex> lock(external_mutex);

			ScopedLock<OpenThreads::Mutex> mutlock(_mut);
			if (!_released)
			{
				return _cond.wait(&_mut) == 0;
			}
			else
			{
				return true;
			}
		}

		inline void release()
		{
			ScopedLock<OpenThreads::Mutex> mutlock(_mut);
			if (!_released)
			{
				_released = true;
				_cond.broadcast();
			}
		}

		inline void reset()
		{
			ScopedLock<OpenThreads::Mutex> mutlock(_mut);
			_released = false;
		}

		inline void set(bool doRelease)
		{
			if (doRelease != _released)
			{
				if (doRelease) release();
				else reset();
			}
		}

	protected:

		Mutex _mut;
		Condition _cond;
		bool _released;

	private:

		Block(const Block&) {}
	};

	/** BlockCount is a block that can be used to halt a thread that is waiting for a specified number of operations to be completed.*/
	class BlockCount
	{
	public:

		BlockCount(unsigned int blockCount) :
			_blockCount(blockCount),
			_currentCount(0) {}

		~BlockCount()
		{
			_blockCount = 0;
			release();
		}

		inline void completed()
		{
			OpenThreads::ScopedLock<OpenThreads::Mutex> mutlock(_mut);
			if (_currentCount>0)
			{
				--_currentCount;

				if (_currentCount == 0)
				{
					_cond.broadcast();
				}
			}
		}

		inline void block()
		{
			OpenThreads::ScopedLock<OpenThreads::Mutex> mutlock(_mut);
			if (_currentCount)
				_cond.wait(&_mut);
		}

		inline void block(Mutex& external_mutex)
		{
			ReverseScopedLock<Mutex> lock(external_mutex);

			OpenThreads::ScopedLock<OpenThreads::Mutex> mutlock(_mut);
			if (_currentCount)
				_cond.wait(&_mut);
		}

		inline void reset()
		{
			OpenThreads::ScopedLock<OpenThreads::Mutex> mutlock(_mut);
			if (_currentCount != _blockCount)
			{
				if (_blockCount == 0) _cond.broadcast();
				_currentCount = _blockCount;
			}
		}

		inline void release()
		{
			OpenThreads::ScopedLock<OpenThreads::Mutex> mutlock(_mut);
			if (_currentCount)
			{
				_currentCount = 0;
				_cond.broadcast();
			}
		}

		inline void setBlockCount(unsigned int blockCount) { _blockCount = blockCount; }

		inline unsigned int getBlockCount() const { return _blockCount; }

		inline unsigned int getCurrentCount() const { return _currentCount; }

	protected:

		OpenThreads::Mutex _mut;
		OpenThreads::Condition _cond;
		unsigned int _blockCount;
		unsigned int _currentCount;

	private:

		BlockCount(const BlockCount&) {}

	};

}

#endif
