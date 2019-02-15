//
// Created by Alexander on 30.09.2018.
//

#include <cstdint>
#include <cstring>
#include <cstdlib>
#include <cerrno>

#include <sys/mman.h>

#include <utility>
#include <string>
#include <memory>
#include <vector>
#include <iostream>

#include <benchmark/benchmark.h>


#define MMAP_AVAILABLE

const constexpr unsigned LINE_SIZE = 64;
const constexpr unsigned KB = 1024;
const constexpr unsigned MB = KB*KB;

struct alignas(LINE_SIZE) MemoryLine {
    std::uint8_t stub[LINE_SIZE];
};

constexpr unsigned long long int operator "" _KB(unsigned long long int val) {
    return val * KB;
}

constexpr unsigned long long int operator "" _MB(unsigned long long int val) {
    return val * MB;
}

const constexpr int INTERATIONS = 10000;
const constexpr int MEM_BUF_SIZE = 64_MB;

static void liniar_traversal(benchmark::State& state) {

#ifdef MMAP_AVAILABLE    
    // IMPORTANT: do not forget to allocate enough large pages!
    // See HugePages_Total and HugePages_Free in /proc/meminfo
    // Execute the following cmd to increase their number: echo 400 > /proc/sys/vm/nr_hugepages
    // For mmap docs see: http://man7.org/linux/man-pages/man2/mmap.2.html
    // and https://www.kernel.org/doc/Documentation/vm/hugetlbpage.txt
    //
    void* addr = NULL; // let mmap decide on its own
    auto length = MEM_BUF_SIZE;
    auto protection = PROT_READ | PROT_WRITE;
    auto flags = MAP_SHARED | MAP_ANONYMOUS | MAP_HUGETLB;
    auto fd = -1; // file descriptor must be -1 if MAP_ANONYMOUS is passed
    auto offset = 0; // must be zero if MAP_ANONYMUOS is passed
    auto liniar_array = (std::uint8_t *)mmap(addr, length, protection, flags, fd, offset);
    if (liniar_array == MAP_FAILED) {
    state.SkipWithError(std::strerror(errno));    
    }
#else    
    auto mem_buf_up = std::make_unique<MemoryLine[]>(MEM_BUF_SIZE / sizeof(MemoryLine));
    std::uint8_t* liniar_array = reinterpret_cast<std::uint8_t*>(mem_buf_up.get());
#endif

    int sum0 = 0;
    const std::size_t bytes_to_read = state.range(0);
    for (auto _: state) {
        for (auto j = 0; j < INTERATIONS; ++j) {
            for (auto ptr = liniar_array; ptr < liniar_array + bytes_to_read; ptr += (4*LINE_SIZE)) {
                benchmark::DoNotOptimize(sum0 = *ptr);
                benchmark::DoNotOptimize(sum0 = *(ptr + LINE_SIZE));
                benchmark::DoNotOptimize(sum0 = *(ptr + 2*LINE_SIZE));
                benchmark::DoNotOptimize(sum0 = *(ptr + 3*LINE_SIZE));
            }
        }
    }

    state.SetBytesProcessed(int64_t(state.iterations()) *
                            int64_t(bytes_to_read) * INTERATIONS);
    state.counters["Bytes/clk"] = benchmark::Counter(state.bytes_processed() / (2.8 * 1000 * 1000 * 1000), benchmark::Counter::kIsRate);

#ifdef MMAP_AVAILABLE 
    munmap(liniar_array, length);
#endif    
}

int main(int argc, char** argv) {
    
    auto nthreads = 1;
    for (auto i = 0; i < argc; ++i) {
    auto arg = std::string(argv[i]);
        if ( arg == "-nthreads") {
        if (i + 1 < argc) {
        std::cout << "Number of threads to run: " << arg << '\n';
        nthreads = std::stoi(std::string(argv[i + 1]));
        } else {
        std::cerr << "There should be an integer value after '-nthreads'.\n";
        return 1;
        } 
    }    
    }

    std::vector<std::pair<const char*, unsigned long long int>> runs = {
        {"1 KB",   1_KB},
        {"2 KB",   2_KB},
        {"4 KB",   4_KB},
        {"8 KB",   8_KB},
        {"16 KB",  16_KB},
        {"24 KB",  24_KB},
        {"28 KB",  28_KB},
        {"32 KB",  32_KB},
        {"34 KB",  34_KB},
        {"36 KB",  36_KB},
        {"40 KB",  40_KB},
        {"48 KB",  48_KB},
        {"64 KB",  64_KB},
        {"128 KB", 128_KB},
        {"256 KB", 256_KB},
        {"384 KB", 384_KB},
        {"448 KB", 448_KB},
        {"480 KB", 480_KB},
        {"512 KB", 512_KB},
        {"544 KB", 544_KB},
        {"608 KB", 608_KB},
        {"640 KB", 640_KB},
        {"768 KB", 768_KB},
        {"896 KB", 896_KB},
        {"960 KB", 960_KB},
        {"1 MB",   1_MB},
        {"1.0625 MB",1088_KB},
        {"1.125 MB",1152_KB},
        {"1.25 MB",1280_KB},
        {"1.5 MB", 1536_KB},
        {"2 MB",   2_MB},
        {"2.5 MB", 2056_KB},
        {"3 MB",   3_MB},
        {"4 MB",   4_MB},
        {"6 MB",   6_MB},
        {"8 MB",   8_MB},
        {"12 MB",  12_MB},
        {"16 MB",  16_MB},
        {"20 MB",  20_MB},
        {"21 MB",  21_MB},
        {"22 MB",  22_MB},
        {"23 MB",  23_MB},
        {"24 MB",  24_MB},
        {"25 MB",  25_MB},
        {"26 MB",  26_MB},
        {"27 MB",  27_MB},
        {"28 MB",  28_MB},
        {"32 MB",  32_MB},
        {"36 MB",  36_MB},
        {"40 MB",  40_MB},
    };
    for (auto& r : runs)
      benchmark::RegisterBenchmark("linear traversal", liniar_traversal)->ArgName(r.first)->Arg(r.second)->Threads(nthreads);
  benchmark::Initialize(&argc, argv);
  benchmark::RunSpecifiedBenchmarks();
}
