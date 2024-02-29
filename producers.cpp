#include <iostream>
#include <stack>
#include <thread>
#include <random>
#include <mutex>
#include <chrono>
#include <condition_variable>

using namespace std;

stack<int> buffer;
const int BUFFER_MAX = 10000;
condition_variable bufferNoFull;
condition_variable bufferNotEmpty;
mutex mut;

int produceCount = 0;
int consumeCount = 0;
int produceWaitCount = 0;
int consumeWaitCount = 0;
int totalProduce = 0;
const int PRODUCE_MAX = 20000;

void printCount(string stmt, int count, int label = -1)
{
    if (label != -1) {

    }
        
    else {
        cout << stmt << count << endl;
    }
}

/* sleep for a random �me
between (1 to 100ms to simulate random ac�vi�es being done by the producer in the background)
before removing/adding another number again from the stack */
void goToSleep() 
{
    mt19937 gen(random_device{}());
    uniform_int_distribution<> dist(1, 100);
    this_thread::sleep_for(chrono::milliseconds(dist(gen)));
}

/* this method will generate a random number from 1 to 10 and
whatever number it generated it will be pushed into the stack buffer.  */
void producer() 
{
    mt19937 gen(random_device{}());
    uniform_int_distribution<> dist(1, 10);

    for (int i = 0; i < 10000; ++i) 
    {
        unique_lock<mutex> lock(mut);
        bufferNoFull.wait(lock, [] { return buffer.size() < BUFFER_MAX; });

        int n = dist(gen);
        buffer.push(n);
        produceCount = produceCount + n;
        totalProduce++;
        
        cout << "Producer [" << this_thread::get_id() << "] produced: " << n << endl;

        bufferNotEmpty.notify_one();
        lock.unlock();

        goToSleep();

        /* a producer thread has to wait()
            before generating again a random number 
            and placing it into the stack.  
        */
        if (buffer.size() == BUFFER_MAX) 
        {
            produceWaitCount++;
            cout << "[!!!] Stack is Full, Producer has waited, id: [" << this_thread::get_id() << "]" << endl;
        }
    }
}

/* this method will remove an item from the stack buffer and
accumulate the removed(popped) value on a certain variable called SUM. */
void consumer() 
{
    while (true) 
    {
        unique_lock<mutex> lock(mut);

        bufferNotEmpty.wait(lock, [] { return !buffer.empty() || totalProduce == PRODUCE_MAX; });

        if (buffer.empty() == false) 
        {
            int num = buffer.top();
            buffer.pop();
            consumeCount += num;
            cout << "Consumer [" << this_thread::get_id() << "] consumed: " << num << endl;

            bufferNoFull.notify_one();
        }
        else if (totalProduce == PRODUCE_MAX) 
        {
            break;
        }
        else 
        {
            consumeWaitCount++;
            cout << "[!!!] Stack is Empty, Consumer has waited, id: [" << this_thread::get_id() << "]" << endl;
        }

        lock.unlock();
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

    printCount("producer sum: ", produceCount);
    printCount("consumer sum: ", consumeCount);
    printCount("producer wait: ", produceWaitCount);
    printCount("consumer wait: ", consumeWaitCount);

    return 0;
}