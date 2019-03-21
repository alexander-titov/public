cmake_minimum_required(VERSION 3.12)
project(code)

set(CMAKE_CXX_STANDARD 17)

include_directories("/nfs/site/disks/vip_disk01/work/simvip01/atitov/.clion/projects/benchmark/include"
                    "/nfs/site/disks/vip_disk01/work/simvip01/atitov/.clion/projects/build/src")

add_library(libbenchmark STATIC IMPORTED)
set_property(TARGET libbenchmark PROPERTY IMPORTED_LOCATION /nfs/site/disks/vip_disk01/work/simvip01/atitov/.clion/projects/build/src/libbenchmark.a)

set( CMAKE_VERBOSE_MAKEFILE on )

set(CMAKE_SHARED_LINKER_FLAGS "-pthread -Wl,-no-as-needed")
set(CMAKE_CXX_FLAGS " -O3 -Wall -pedantic -Werror ")

add_executable(code
        main.cpp)

target_link_libraries(code libbenchmark pthread rt)

