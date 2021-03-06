// BlockingQueue.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "BlockingQueue.h"
#include "Thread.h"

volatile bool stop = false;

class Test1 : public OpenThreads::Thread
{
public:
	explicit Test1(BlockingQueue<int>* _bq) : _blockQueue(_bq) {}

	virtual void run()
	{
		for (int i = 0; i < 10000; ++i)
		{
			_blockQueue->put(i, INFINITE);
			//cout << "                             " << i << endl;
		}
	}

private:
	BlockingQueue<int>* _blockQueue;
};

class Test2 : public OpenThreads::Thread
{
public:
	explicit Test2(BlockingQueue<int>* _bq) : _blockQueue(_bq) {}

	virtual void run()
	{
		while(!stop)
		{
			int temp = 0;
			if(_blockQueue->take(temp, 250))
				cout << temp << endl;
		}
	}

private:
	BlockingQueue<int>* _blockQueue;
};

int main()
{
	BlockingQueue<int> bq(100);
	Test1 t1(&bq);
	Test2 t2(&bq);

	t2.startThread();

	system("pause");

	t1.startThread();

	system("pause");

	stop = true;
	t2.join();

    return 0;
}

