#include <iostream>
#include <stack>
#include <thread>
#include <random>
#include <mutex>
#include <chrono>
#include <semaphore>

using namespace std;

stack<int> buffer;
const int BUFFER_MAX = 10000;
std::counting_semaphore<BUFFER_MAX> bufferNotEmpty(0);
std::counting_semaphore<BUFFER_MAX> bufferNoFull(BUFFER_MAX);

mutex mut;
bool finished = false; // Flag to indicate production is finished

int produceCount = 0;
int consumeCount = 0;
int produceWaitCount = 0;
int consumeWaitCount = 0;
int totalProduce = 0;
const int PRODUCE_MAX = 20000;

void goToSleep()
{
    mt19937 gen(random_device{}());
    uniform_int_distribution<> dist(1, 100);
    this_thread::sleep_for(chrono::milliseconds(dist(gen)));
}

void producer()
{
    mt19937 gen(random_device{}());
    uniform_int_distribution<> dist(1, 10);

    for (int i = 0; i < 10000; ++i)
    {
        bufferNoFull.acquire();

        {
            lock_guard<mutex> lock(mut);
            int n = dist(gen);
            buffer.push(n);
            produceCount += n;
            totalProduce++;
            cout << "Producer [" << this_thread::get_id() << "] produced: " << n << endl;
        }

        bufferNotEmpty.release();
        goToSleep();
    }

    // Signal that production is finished
    {
        lock_guard<mutex> lock(mut);
        if (totalProduce >= PRODUCE_MAX)
        {
            finished = true;
            // Release for all consumer threads
            for (int i = 0; i < 2; ++i)
            {  // i < [# of consumers]
                bufferNotEmpty.release();
            }
        }
    }
}

void consumer()
{
    while (true)
    {
        bufferNotEmpty.acquire();

        {
            lock_guard<mutex> lock(mut);
            if (finished && buffer.empty())
            {
                break;
            }
            if (!buffer.empty())
            {
                int num = buffer.top();
                buffer.pop();
                consumeCount += num;
                cout << "Consumer [" << this_thread::get_id() << "] consumed: " << num << endl;
            }
        }

        bufferNoFull.release();
        goToSleep();
    }
}

int main()
{
    thread producer_1(producer);
    thread consumer_1(consumer);

    thread producer_2(producer);
    thread consumer_2(consumer);

    producer_1.join();
    producer_2.join();

    consumer_1.join();
    consumer_2.join();

    cout << "producer sum: " << produceCount << endl;
    cout << "consumer sum: " << consumeCount << endl;
    cout << "producer wait: " << produceWaitCount << endl;
    cout << "consumer wait: " << consumeWaitCount << endl;

    return 0;
}
