cmake_minimum_required(VERSION 3.10)

# Set the project name
project(zenoh_bridge)

include_directories(/usr/local/include/reelay)
include_directories(/usr/local/include/rvstd)


find_package(zenohc) #if using zenoh-c backend
find_package(zenohcxx)
find_package(rvstd)
find_package(Boost REQUIRED COMPONENTS json)
find_library(CUDD_LIB libcudd.so PATHS /usr/local/lib)

# Add the executable
add_executable(zenoh_bridge main.cpp)
target_link_libraries(zenoh_bridge PUBLIC zenohcxx::zenohc ${CUDD_LIB} rvstd Boost::json) #if using zenoh-c backend

