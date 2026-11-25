cmake_minimum_required(VERSION 3.16)
project(OrderManagement LANGUAGES CXX)

add_executable(OrderManagement
    main.cpp
    order.cpp order.h
    hashtable.cpp hashtable.h
    queue.cpp queue.h
    crud.cpp crud.h
    OrderBST.cpp OrderBST.h
    OrderStatusHandler.cpp OrderStatusHandler.h
    statistics.cpp statistics.h
)
target_compile_features(OrderManagement PRIVATE cxx_std_17)
set_target_properties(OrderManagement PROPERTIES CXX_EXTENSIONS OFF)

if(MSVC)
    target_compile_options(OrderManagement PRIVATE /W4 /utf-8)
else()
    target_compile_options(OrderManagement PRIVATE -Wall -Wextra -Wpedantic)
endif()
