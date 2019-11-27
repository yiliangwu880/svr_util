

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

		std::thread reader([&]() {
			int item;
			for (int i = 0; i != 100; ++i) {
				// Fully-blocking:
				q.wait_dequeue(item);

				// Blocking with timeout
				if (q.wait_dequeue_timed(item, std::chrono::milliseconds(5)))
					++i;
			}
		});
		std::thread writer([&]() {
			for (int i = 0; i != 100; ++i) {
				q.enqueue(i);
				std::this_thread::sleep_for(std::chrono::milliseconds(10));
			}
		});
		writer.join();
		reader.join();

		UNIT_ASSERT(q.size_approx() == 0);
	}
}//end namespace




UNITTEST(concurrency)
{
	Example();
	BlockExample();
}