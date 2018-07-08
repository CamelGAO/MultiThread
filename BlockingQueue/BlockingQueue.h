#include "stdafx.h"

#include "Mutex.h"
#include "ScopedLock.h"
#include "Condition.h"

#include "queue"

using namespace OpenThreads;

template<typename T>
class BlockingQueue
{
	typedef std::queue<typename T> Queue;
public:
	explicit BlockingQueue(size_t _size) {size = _size;}
	~BlockingQueue() {}

	bool put(const T& _in, unsigned long _timeWait = INFINITE); //ms
	bool take(T& _out, unsigned long _timeWait = INFINITE);

protected:
private:
	BlockingQueue(const BlockingQueue&);
	BlockingQueue& operator=(const BlockingQueue&);

	Queue queue;
	Mutex mutex;
	Condition fullCondition;
	Condition emptyCondition;

	size_t size;

};

template<typename T>
bool BlockingQueue<typename T>::put(const T& _in, unsigned long _timeWait)
{
	ScopedLock<Mutex> _lock(mutex);

	while(queue.size() >= size)
	{
		if (_timeWait)
		{
			if (fullCondition.wait(&mutex, _timeWait) != WAIT_OBJECT_0)
				return false;
		}
		else
			return false;
	}

	queue.push(_in);
	emptyCondition.signal();
	
	return true;
}

template<typename T>
bool BlockingQueue<typename T>::take(T& _out, unsigned long _timeWait)
{
	ScopedLock<Mutex> _lock(mutex);

	while(queue.empty())
	{
		if (_timeWait)
		{
			if (emptyCondition.wait(&mutex, _timeWait) != WAIT_OBJECT_0)
				return false;
		}
		else
			return false;
	}

	_out = queue.front();
	queue.pop();
	fullCondition.signal();

	return true;
}