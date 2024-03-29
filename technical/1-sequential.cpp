/*
Compilation: 
    g++ sequential.cpp -o sequential
*/

#include <iostream>
#include <chrono>

using namespace std;


int array_sum(int* arr, int size) {
    int sum = 0;
    for(int i = 0; i < size; i++) {
        sum += arr[i];
    }
    return sum;
}

int main() {
    const int SIZE = 100000000; 
    const int NUM_ITERATIONS = 100; 
    int* arr = new int[SIZE]; 

    
    for(int i = 0; i < SIZE; i++) {
        arr[i] = i % 2; 
    }

    auto start_time = chrono::high_resolution_clock::now();

    int sum = 0;
    for(int i = 0; i < NUM_ITERATIONS; i++) {
        sum = array_sum(arr, SIZE);
    }

    auto end_time = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(end_time - start_time).count();

    cout << "Sum: " << sum << endl;
    cout << "Time taken (sequential): " << duration << " microseconds" << endl;
    
    delete[] arr;

    return 0;
}
