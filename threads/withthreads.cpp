#include <iostream>
#include <thread>
#include <vector>
#include <chrono>
#include <utility>
#include <cstdlib>
#include <map>
#include <numeric>

void vectorSum(std::pair<std::vector<int>, unsigned long long int>& p)
{
    p.second = std::accumulate(p.first.begin(), p.first.end(), 0ULL);
}


int main()
{
    std::vector<std::thread> threads;
    std::string labels[] = {"A", "B", "C", "D", "E"};
    std::map<std::string, std::pair<std::vector<int>, unsigned long long int>> mymap;
    const int SIZE = 50000000;

    for (int i = 0; i < 5; i++)
    {
        std::vector<int> v(SIZE);
        for (auto& e : v) e = rand();
        mymap[labels[i]].first = v;
    }
    
    auto start = std::chrono::steady_clock::now();

    /* Create a thread for find the sum of each vector */
    for (int i = 0; i < 5; i++)
    {
        threads.push_back(std::thread(vectorSum,  std::ref(mymap[labels[i]]) ) );
    }
    for (auto& t : threads)
    {
        t.join();
    }

    auto end = std::chrono::steady_clock::now();

    for (const auto& pair : mymap)
    {
        std::cout << "sum of vector {" << pair.first  << "} : " << pair.second.second << std::endl;
    }

    std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << "ms" << std::endl;
}