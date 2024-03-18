#include <chrono>
#include <iostream>
#include <mutex>
#include <random>
#include <semaphore>
#include <stack>
#include <thread>

constexpr int MAX_BUFFER_SIZE = 10000;
constexpr int TOTAL_NUMBERS_TO_PRODUCE = 20000;

std::stack<int> buffer;
std::mutex mtx;
std::counting_semaphore<MAX_BUFFER_SIZE> buffer_slots_available(MAX_BUFFER_SIZE); // Semaphore for available slots in the buffer
std::counting_semaphore<MAX_BUFFER_SIZE> items_in_buffer(0);                      // Semaphore for items in the buffer

int sum_produced = 0;
int sum_consumed = 0;
int producer_wait_count = 0;
int consumer_wait_count = 0;
int total_produced = 0;
bool production_finished = false;

void sleep_random()
{
    /* std::mt19937 gen(std::random_device{}());
    std::uniform_int_distribution<> dist(1, 100);
    std::this_thread::sleep_for(std::chrono::milliseconds(dist(gen))); */
}

void producer()
{
    std::mt19937 gen(std::random_device{}());
    std::uniform_int_distribution<> dist(1, 10);

    for (int i = 0; i < 10000; ++i)
    {
        buffer_slots_available.acquire(); // Wait for an available slot in the buffer

        std::lock_guard<std::mutex> lock(mtx);
        int num = dist(gen);
        buffer.push(num);
        sum_produced += num;
        total_produced++;
        std::cout << "Producer [" << std::this_thread::get_id() << "] produced: " << num << std::endl;

        items_in_buffer.release(); // Signal that an item has been added to the buffer

        sleep_random();

        if (buffer.size() == MAX_BUFFER_SIZE)
        {
            producer_wait_count++;
            std::cout << "Stack Full Producer Waiting [" << std::this_thread::get_id() << "]" << std::endl;
        }
    }
}

void consumer()
{
    while (true)
    {
        std::unique_lock<std::mutex> lock(mtx);
        if (production_finished && buffer.empty())
        {
            break;
        }

        lock.unlock();

        items_in_buffer.acquire(); // Wait for an item in the buffer

        lock.lock();

        if (!buffer.empty())
        {
            const int num = buffer.top();
            buffer.pop();
            sum_consumed += num;
            std::cout << "Consumer [" << std::this_thread::get_id() << "] consumed: " << num << std::endl;

            buffer_slots_available.release(); // Signal that a slot has been freed in the buffer
        }
        else
        {
            consumer_wait_count++;
            std::cout << "Stack Empty Consumer Waiting [" << std::this_thread::get_id() << "]" << std::endl;
        }

        lock.unlock();
        sleep_random();
    }
}

int main()
{
    std::thread producers[2];
    std::thread consumers[2];

    for (int i = 0; i < 2; ++i)
    {
        producers[i] = std::thread(producer);
        consumers[i] = std::thread(consumer);
    }

    for (int i = 0; i < 2; ++i)
    {
        producers[i].join();
    }

    {
        std::lock_guard<std::mutex> lock(mtx);
        production_finished = true;
        items_in_buffer.release(2); // Release semaphores to wake up consumers if they are waiting
    }

    for (int i = 0; i < 2; ++i)
    {
        consumers[i].join();
    }

    std::cout << "Total Sum Produced: " << sum_produced << std::endl;
    std::cout << "Total Sum Consumed: " << sum_consumed << std::endl;
    std::cout << "Producer Wait Count: " << producer_wait_count << std::endl;
    std::cout << "Consumer Wait Count: " << consumer_wait_count << std::endl;
}
