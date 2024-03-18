#include <iostream>
#include <chrono>
#include <vector>
#include <cstdlib>

using namespace std;

void vectorAssignRandom(vector<int> &v) {
    for (auto & i : v)
        i = rand();
}

unsigned long long int vectorSum(const vector<int> &v) {
    unsigned long long int sum = 0;
    for (auto & i : v)
        sum += i;

    return sum;
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
    sum_a = vectorSum(a);
    sum_b = vectorSum(b);
    sum_c = vectorSum(c);
    sum_d = vectorSum(d);
    sum_e = vectorSum(e);
    auto end = chrono::steady_clock::now();

    cout << "sum of vector a elements: " << sum_a << endl;
    cout << "sum of vector b elements: " << sum_b << endl;
    cout << "sum of vector c elements: " << sum_c << endl;
    cout << "sum of vector d elements: " << sum_d << endl;
    cout << "sum of vector e elements: " << sum_e << endl;
    cout << chrono::duration_cast<chrono::milliseconds>(end - start).count() << "ms" << endl;

    return 0;
}