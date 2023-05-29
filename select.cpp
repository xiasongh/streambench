#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <thread>
#include <chrono>
#include <cstdint>


void Select(int data_length, int64_t* in_data, int64_t* out_data) {
    std::transform(in_data, in_data + data_length, out_data, [](int64_t x) { return x + 10; });
}

int64_t SelectBench(int64_t data_length) {
    int64_t* in_data = new int64_t[data_length];
    int64_t* out_data = new int64_t[data_length];

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int64_t> distribution(1, 100);
    for (int i = 0; i < data_length; ++i) {
        in_data[i] = distribution(gen);
    }

    auto start_time = std::chrono::high_resolution_clock::now();

    Select(data_length, in_data, out_data);

    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);

    std::cout << "Execution time: " << duration.count() << " milliseconds" << std::endl;

    return 0;
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cout << "Usage: " << argv[0] << " <thread_count> <data_length>" << std::endl;
        return 1;
    }

    int thread_count = std::stoi(argv[1]);
    int data_length = std::stoi(argv[2]);

    std::vector<std::thread> threads(thread_count);
    for (int tid = 0; tid < thread_count; tid++) {
        threads[tid] = std::thread(SelectBench, data_length);
    }
    
    for (auto& thread : threads) {
        thread.join();
    }

    return 0;
}
