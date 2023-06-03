#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <random>
#include <thread>
#include <chrono>
#include <cstdint>


#define DATATYPE int64_t
#define _STRINGIFY(s) #s
#define STRINGIFY(s) _STRINGIFY(s)


template<typename T>
void Select(int data_length, T* in_data, T* out_data) {
    std::transform(in_data, in_data + data_length, out_data, [](T x) { return x + 10; });
}

template<typename T>
void SelectBench(int64_t data_length) {
    T* in_data = new T[data_length];
    T* out_data = new T[data_length];

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<T> distribution(1, 100);
    for (int i = 0; i < data_length; ++i) {
        in_data[i] = distribution(gen);
    }

    Select(data_length, in_data, out_data);
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cout << "Usage: " << argv[0] << " <thread_count> <data_length>" << std::endl;
        return 1;
    }

    int thread_count = std::stoi(argv[1]);
    int data_length = std::stoi(argv[2]);

    std::vector<std::thread> threads(thread_count);

    auto start_time = std::chrono::high_resolution_clock::now();
    for (int tid = 0; tid < thread_count; tid++) {
        threads[tid] = std::thread(SelectBench<DATATYPE>, data_length);
    }
    
    for (auto& thread : threads) {
        thread.join();
    }
    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);
    double time = duration.count();

    std::cout << "Throughput[1M/s]: " << std::setprecision(3) << thread_count * data_length / time
              << ", Threads: " << thread_count
              << ", Datatype: " STRINGIFY(DATATYPE) << std::endl;

    return 0;
}
