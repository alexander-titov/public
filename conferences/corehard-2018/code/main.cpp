//
// Created by Alexander on 30.09.2018.
//

#include <cstdint>
#include <cstdlib>

#include <utility>
#include <string>
#include <memory>
#include <vector>

#include <benchmark/benchmark.h>


const constexpr unsigned LINE_SIZE = 64;

struct alignas(LINE_SIZE) MemoryLine {
    std::uint8_t stub[LINE_SIZE];
};


const constexpr unsigned Ki = 1024;
const constexpr unsigned Mi = Ki*Ki;
const constexpr unsigned Gi = Ki*Ki*Ki;

constexpr unsigned long long int operator "" _Ki(unsigned long long int val) {
    return val * Ki;
}
constexpr unsigned long long int operator "" _Mi(unsigned long long int val) {
    return val * Mi;
}
constexpr unsigned long long int operator "" _Gi(unsigned long long int val) {
    return val * Gi;
}

const constexpr unsigned K = 1000;
const constexpr unsigned M = K*K;
const constexpr unsigned G = K*K*K;

constexpr unsigned long long int operator "" _K(unsigned long long int val) {
    return val * K;
}
constexpr unsigned long long int operator "" _M(unsigned long long int val) {
    return val * M;
}
constexpr unsigned long long int operator "" _G(unsigned long long int val) {
    return val * G;
}


const constexpr unsigned CPU_TURBO_FREQ = 3700_K;


const constexpr int ITERATIONS = 10000;


static void linear_traversal(benchmark::State &state) {

    auto mem_buf_up = std::make_unique<MemoryLine[]>(64_Mi / sizeof(MemoryLine));
    std::uint8_t* liniar_array = reinterpret_cast<std::uint8_t*>(mem_buf_up.get());

    int sum0 = 0;
    const std::size_t bytes_to_read = state.range(0);
    for (auto _: state) {
        for (auto j = 0; j < ITERATIONS; ++j) {
            for (auto ptr = liniar_array; ptr < liniar_array + bytes_to_read; ptr += (4*LINE_SIZE)) {
                benchmark::DoNotOptimize(sum0 = *ptr);
                benchmark::DoNotOptimize(sum0 = *(ptr + LINE_SIZE));
                benchmark::DoNotOptimize(sum0 = *(ptr + 2*LINE_SIZE));
                benchmark::DoNotOptimize(sum0 = *(ptr + 3*LINE_SIZE));
            }
        }
    }

    state.SetBytesProcessed(int64_t(state.iterations()) *
                            int64_t(bytes_to_read) * ITERATIONS);
    state.counters["Bytes/clk"] = benchmark::Counter(state.bytes_processed() / CPU_TURBO_FREQ, benchmark::Counter::kIsRate);

}

int main(int argc, char** argv) {
    
    // Configure all the runs
    //    first -- the name of the run
    //    second -- the size of the data set
    //
    std::vector<std::pair<const char*, unsigned long long int>> runs = {
		{"1 KB",   1_Ki},
		{"2 KB",   2_Ki},
		{"4 KB",   4_Ki},
		{"8 KB",   8_Ki},
		{"16 KB",  16_Ki},
		{"24 KB",  24_Ki},
		{"28 KB",  28_Ki},
		{"32 KB",  32_Ki},
		{"34 KB",  34_Ki},
		{"36 KB",  36_Ki},
		{"40 KB",  40_Ki},
		{"48 KB",  48_Ki},
		{"64 KB",  64_Ki},
		{"128 KB", 128_Ki},
		{"256 KB", 256_Ki},
		{"384 KB", 384_Ki},
		{"448 KB", 448_Ki},
		{"480 KB", 480_Ki},
		{"512 KB", 512_Ki},
		{"544 KB", 544_Ki},
		{"608 KB", 608_Ki},
		{"640 KB", 640_Ki},
		{"768 KB", 768_Ki},
		{"896 KB", 896_Ki},
		{"960 KB", 960_Ki},
		{"1 MB",   1_Mi},
		{"1.0625 MB",1088_Ki},
		{"1.125 MB",1152_Ki},
		{"1.25 MB",1280_Ki},
		{"1.5 MB", 1536_Ki},
		{"2 MB",   2_Mi},
		{"2.5 MB", 2056_Ki},
		{"3 MB",   3_Mi},
		{"4 MB",   4_Mi},
		{"6 MB",   6_Mi},
		{"8 MB",   8_Mi},
		{"12 MB",  12_Mi},
		{"16 MB",  16_Mi},
		{"20 MB",  20_Mi},
		{"21 MB",  21_Mi},
		{"22 MB",  22_Mi},
		{"23 MB",  23_Mi},
		{"24 MB",  24_Mi},
		{"25 MB",  25_Mi},
		{"26 MB",  26_Mi},
		{"27 MB",  27_Mi},
		{"28 MB",  28_Mi},
		{"32 MB",  32_Mi},
		{"36 MB",  36_Mi},
		{"40 MB",  40_Mi},
	};
    
    for (auto& r : runs)
        benchmark::RegisterBenchmark("linear traversal", linear_traversal)->ArgName(r.first)->Arg(r.second);
  
    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
}
