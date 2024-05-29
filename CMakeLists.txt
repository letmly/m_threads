cmake_minimum_required(VERSION 3.27)
project(vstu_MultiThreads)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenMP REQUIRED)

if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /openmp:llvm /openmp:experimental")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp")
endif()

add_executable(vstu_MultiThreads
        mt1/threads1.cpp
        main.cpp
        mt3/floid.cpp
        mt4/bsp.cpp
        mt4/BSPJson.cpp
        mt4/BSP.h
        mt4/BSPJson.h
)

if(OpenMP_CXX_FOUND)
    target_link_libraries(vstu_MultiThreads PUBLIC OpenMP::OpenMP_CXX)
endif()
