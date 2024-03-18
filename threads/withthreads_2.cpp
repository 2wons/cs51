#include <iostream>
#include <chrono>
#include <vector>
#include <cstdlib>
#include <thread>

using namespace std;

void vectorAssignRandom(vector<int> &v) {
    for (auto & i : v)
        i = rand();
}

void vectorSum(const vector<int> &v, unsigned long long int &sum) {
    for (auto & i : v)
        sum += i;
}

int main(){
    const long SIZE = 50000000;
    vector<int> a(SIZE), b(SIZE), c(SIZE), d(SIZE), e(SIZE);
    unsigned long long int sum_a = 0, sum_b = 0, sum_c = 0, sum_d = 0, sum_e = 0;

    vectorAssignRandom(a);
    vectorAssignRandom(b);
    vectorAssignRandom(c);
    vectorAssignRandom(d);
    vectorAssignRandom(e);

    auto start = chrono::steady_clock::now();
    thread t1(vectorSum, cref(a), ref(sum_a));
    thread t2(vectorSum, cref(b), ref(sum_b));
    thread t3(vectorSum, cref(c), ref(sum_c));
    thread t4(vectorSum, cref(d), ref(sum_d));
    thread t5(vectorSum, cref(e), ref(sum_e));
    t1.join();
    t2.join();
    t3.join();
    t4.join();
    t5.join();
    auto end = chrono::steady_clock::now();

    cout << "sum of vector a elements: " << sum_a << endl;
    cout << "sum of vector b elements: " << sum_b << endl;
    cout << "sum of vector c elements: " << sum_c << endl;
    cout << "sum of vector d elements: " << sum_d << endl;
    cout << "sum of vector e elements: " << sum_e << endl;
    cout << chrono::duration_cast<chrono::milliseconds>(end - start).count() << "ms" << endl;

    return 0;
}