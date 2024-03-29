/*
Compilation:
    nvc++ -acc=gpu -gpu=DEFAULT_TARGET openacc-gpu-memory.cpp -o openacc-gpu-memory

    To find Default Target, run the following command in the terminal: 
    $ nvaccelinfo

Example compilation:
    nvc++ -acc=gpu -gpu=cc50 openacc-gpu-memory.cpp -o openacc-gpu-memory
*/


#include <iostream>
#include <chrono>

using namespace std;

// Function to calculate the sum of an array
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

    // Perform multiple iterations of the summing operation using OpenACC with GPU
    int sum = 0;
    #pragma acc data copyin(arr [0:SIZE])
    {
        for(int i = 0; i < NUM_ITERATIONS; i++) {
            sum = 0;
            #pragma acc parallel loop reduction(+:sum)
            for(int j = 0; j < SIZE; j++) {
                sum += arr[j];
            }
        }
    }

    auto end_time = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(end_time - start_time).count();

    // Display the sum and time taken
    cout << "Sum: " << sum << endl;
    cout << "Time taken (OpenACC with GPU): " << duration << " microseconds" << endl;

    // Free the dynamically allocated memory
    delete[] arr;

    return 0;
}
