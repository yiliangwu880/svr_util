

#include "su_include.h"
#include "time/su_timestamp.h"
#include "time/su_timer.h"
#include   <stdio.h>      
#include   <stdio.h>      
#include   <time.h>   
#include "easy_code.h"
#include "unit_test.h"
#include "concurrency/readerwriterqueue.h"
#include "concurrency/atomicops.h"
#include <thread>
#include "concurrency/swap_queue.h"
#include "str_util.h"


using namespace std;
using namespace su;
using namespace moodycamel;
namespace
{
	void Example()
	{

		ReaderWriterQueue<int> q(100);       // Reserve space for at least 100 elements up front

		q.enqueue(17);                       // Will allocate memory if the queue is full
		bool succeeded = q.try_enqueue(18);  // Will only succeed if the queue has an empty slot (never allocates)
		UNIT_ASSERT(succeeded);

		int number;
		succeeded = q.try_dequeue(number);  // Returns false if the queue was empty

		UNIT_ASSERT(succeeded && number == 17);

		// You can also peek at the front item of the queue (consumer only)
		int* front = q.peek();
		UNIT_ASSERT(*front == 18);
		succeeded = q.try_dequeue(number);
		UNIT_ASSERT(succeeded && number == 18);
		front = q.peek();
		UNIT_ASSERT(front == nullptr);           // Returns nullptr if the queue was empty
	}

	void BlockExample()
	{
		BlockingReaderWriterQueue<int> q;
		int read_cnt = 0;
		const int MAX_NUM = 10;
		std::thread reader([&]() {
			int item=1;
			for (int i = 0; i != MAX_NUM; ++i) {
				// Fully-blocking:
				q.wait_dequeue(item);

				//UNIT_INFO("wait_dequeue=%d", item);
				// Blocking with timeout
				if (q.wait_dequeue_timed(item, std::chrono::milliseconds(5)))
				{
					++i;
				//	UNIT_INFO("wait_dequeue_timed=%d", item);
				}
				read_cnt = i;
			}
		});
		std::thread writer([&]() {
			for (int i = 0; i != MAX_NUM; ++i) {
				q.enqueue(i);
				std::this_thread::sleep_for(std::chrono::milliseconds(10));
			}
		});
		writer.join();
		reader.join();

//		UNIT_INFO("read_cnt=%d", read_cnt);
		UNIT_ASSERT(q.size_approx() == 0);
	}

	//固定大小长度读写
	void TestSwapQueue1()
	{
		SwapQueue sq;
#define LOG_THREAD(x, ...)  printf(x"\n", ##__VA_ARGS__);
		std::thread reader([&]() 
		{
			LOG_THREAD("------------------read start");
			uint64 idx = 0;
			while (1)
			{
				uint64 read_idx = 0;
				uint32 len = sizeof(read_idx);
				sq.Read((char *)(&read_idx), len);
				if (0 == len)
				{
					continue;
				}
				//LOG_THREAD("read %llu", read_idx);
				UNIT_ASSERT(len == 8);
				UNIT_ASSERT(read_idx == idx);
				idx++;
				//if (idx % 1000 == 0)
				//{
				//	LOG_THREAD("read idx=%llu", idx);
				//}
				if (idx==50000)
				{
					LOG_THREAD("read end");
					return;
				}
			}

		});
		std::thread writer([&]()
		{
			LOG_THREAD("------------------writer start");
			uint64 idx = 0;
			while (1)
			{
				sq.Write((char *)(&idx), 8);
				idx++;
				if (sq.IsNeedTimer())
				{
					sq.OnTimer();
				}
				//if (idx % 1000 == 0)
				//{
				//	LOG_THREAD("write %llu", idx);
				//}
				if (idx==50000)
				{
					LOG_THREAD("write end");
					break;
				}
			}

			//再跑一秒timer, 确保read完
			time_t start = 0;
			time(&start);
			while (1)
			{
				if (sq.IsNeedTimer())
				{
					sq.OnTimer();
				}

				time_t cur = 0;
				time(&cur);
				if (cur -start >1)
				{
					LOG_THREAD("timer end");
					return;
				}
			}
		});
		writer.join();
		reader.join();
	}

	//
	void TestSwapQueue2()
	{
		SwapQueue sq;
#define LOG_THREAD(x, ...)  printf(x"\n", ##__VA_ARGS__);
		std::thread reader([&]()
		{
			LOG_THREAD("------------------read start");
			uint64 idx = 0;
			while (1)
			{
				uint64 read_idx = 0;
				//读完整 一个
				uint32 total_len = 0;
				while (true)
				{
					uint32 len = sizeof(read_idx)- total_len;
					sq.Read((char *)(&read_idx)+ total_len, len);
					if (0 == len)
					{
						continue;
					}
					total_len += len;
					if (total_len == sizeof(read_idx))
					{
						break;
					}
					//LOG_THREAD("read part %d", total_len);
					UNIT_ASSERT(total_len <= sizeof(read_idx));
				}

				//LOG_THREAD("read %llu", read_idx);
				UNIT_ASSERT(read_idx == idx);
				idx++;
				//if (idx % 1000 == 0)
				//{
				//	LOG_THREAD("read idx=%llu", idx);
				//}
				if (idx == 50000)
				{
					LOG_THREAD("2 read end");
					return;
				}
			}

		});
		std::thread writer([&]()
		{
			LOG_THREAD("------------------writer start");
			uint64 idx = 0;
			while (1)
			{
				sq.Write((char *)(&idx), 5);
				if (sq.IsNeedTimer())
				{
					sq.OnTimer();
				}

				sq.Write(((char *)(&idx)+5), 3);
				idx++;
				//if (idx % 1000 == 0)
				//{
				//	LOG_THREAD("write %llu", idx);
				//}
				if (idx == 50000)
				{
					LOG_THREAD("2 write end");
					break;
				}
			}

			//再跑一秒timer, 确保read完
			time_t start = 0;
			time(&start);
			while (1)
			{
				if (sq.IsNeedTimer())
				{
					sq.OnTimer();
				}

				time_t cur = 0;
				time(&cur);
				if (cur - start > 1)
				{
					LOG_THREAD("timer end");
					return;
				}
			}
		});
		writer.join();
		reader.join();
	}

}//end namespace




UNITTEST(concurrency)
{
	Example();
	BlockExample();
	TestSwapQueue1();
	TestSwapQueue2();
	exit(1);
}