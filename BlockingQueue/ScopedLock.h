#ifndef _SCOPEDLOCK_H
#define _SCOPEDLOCK_H

namespace OpenThreads {

	template <class M> class ScopedLock
	{
	private:
		M & m_lock;
		ScopedLock(const ScopedLock&); // prevent copy
		ScopedLock& operator=(const ScopedLock&); // prevent assign
	public:
		explicit ScopedLock(M& m) :m_lock(m) { m_lock.lock(); }
		~ScopedLock() { m_lock.unlock(); }
	};

	template <class M> class ReverseScopedLock
	{
	private:
		M & m_lock;
		ReverseScopedLock(const ReverseScopedLock&); // prevent copy
		ReverseScopedLock& operator=(const ReverseScopedLock&); // prevent assign
	public:
		explicit ReverseScopedLock(M& m) :m_lock(m) { m_lock.unlock(); }
		~ReverseScopedLock() { m_lock.lock(); }
	};


	template <class M> class ScopedPointerLock
	{
	private:
		M * m_lock;
		ScopedPointerLock(const ScopedPointerLock&); // prevent copy
		ScopedPointerLock& operator=(const ScopedPointerLock&); // prevent assign
	public:
		explicit ScopedPointerLock(M* m) :m_lock(m) { if (m_lock) m_lock->lock(); }
		~ScopedPointerLock() { if (m_lock) m_lock->unlock(); }
	};

	template <class M> class ReverseScopedPointerLock
	{
	private:
		M * m_lock;
		ReverseScopedPointerLock(const ReverseScopedPointerLock&); // prevent copy
		ReverseScopedPointerLock& operator=(const ReverseScopedPointerLock&); // prevent assign
	public:
		explicit ReverseScopedPointerLock(M* m) :m_lock(m) { if (m_lock) m_lock->unlock(); }
		~ReverseScopedPointerLock() { if (m_lock) m_lock->lock(); }
	};

}
#endif
