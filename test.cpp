#include <chrono>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <thread>
using namespace std;
using namespace std::chrono;

void parallelSearch(int arr[], int start, int end, int key, int& result) {
    for (int i = start; i <= end; i++) {
        if (arr[i] == key) {
            result = i;
            return;
        }
    }
}

int parallelLinearSearch(int arr[], int n, int key, int numThreads) {
    int segmentSize = n / numThreads;
    int result = -1;
    thread threads[numThreads];

    for (int i = 0; i < numThreads; i++) {
        int start = i * segmentSize;
        int end = (i == numThreads - 1) ? n - 1 : start + segmentSize - 1;
        threads[i] = thread(parallelSearch, arr, start, end, key, ref(result));
    }

    for (int i = 0; i < numThreads; i++) {
        threads[i].join();
    }

    return result;
}

int linearSearch(int arr[], int n, int key) {
    for (int i = 0; i < n; i++) {
        if (arr[i] == key) {
            return i;
        }
    }
    return -1;
}

int main(int argc, char* argv[]) {
    int n = atoi(argv[1]);
    int numThreads = atoi(argv[2]);
    int key = atoi(argv[3]);

    int* arr = new int[n];
    for (int i = 0; i < n; i++) {
        arr[i] = i;
    }

    auto start = high_resolution_clock::now();
    int sequentialIndex = linearSearch(arr, n, key);
    auto end = high_resolution_clock::now();
    auto durationSequential = duration_cast<nanoseconds>(end - start);

    start = high_resolution_clock::now();
    int parallelIndex = parallelLinearSearch(arr, n, key, numThreads);
    end = high_resolution_clock::now();
    auto durationParallel = duration_cast<nanoseconds>(end - start);

    cout << "Tiempo de ejecucion de linearSearch: "
         << durationSequential.count() << " nanosegundos" << endl;

    cout << "Tiempo de ejecucion de parallelLinearSearch: "
         << durationParallel.count() << " nanosegundos" << endl;

    if (durationSequential < durationParallel) {
        cout << "La busqueda secuencial fue mas rapida." << endl;
    } else if (durationSequential > durationParallel) {
        cout << "La busqueda paralela fue mas rapida." << endl;
    } else {
        cout << "Ambas busquedas tomaron el mismo tiempo." << endl;
    }

    delete[] arr;

    return 0;
}
