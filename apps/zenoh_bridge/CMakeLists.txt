cmake_minimum_required(VERSION 3.10)

# Set the project name
project(zenoh_bridge)

include_directories(/usr/local/include/reelay)
include_directories(/usr/local/include/osi3)

find_package(zenohc) #if using zenoh-c backend
find_package(zenohcxx)
find_package(rvstd)
find_package(Protobuf REQUIRED)
find_library(CUDD_LIB libcudd.so PATHS /usr/local/lib)
find_library(OSI_LIB libopen_simulation_interface.so PATHS /usr/local/lib/osi3)


# Add the executable
add_executable(zenoh_bridge deneme.cpp)

find_package(Boost REQUIRED COMPONENTS json)

target_link_libraries(zenoh_bridge PUBLIC zenohcxx::zenohc ${CUDD_LIB} rvstd Boost::json Boost::boost ${OSI_LIB} protobuf::libprotobuf) #if using zenoh-c backend

add_executable(zenoh_test_sub test_sub.cpp)

target_link_libraries(zenoh_test_sub PUBLIC zenohcxx::zenohc ${CUDD_LIB}) #if using zenoh-c backend

add_executable(zenoh_test_pub test_pub.cpp)

target_link_libraries(zenoh_test_pub PUBLIC zenohcxx::zenohc ${CUDD_LIB}) #if using zenoh-c backend

add_executable(esmini_zenoh esmini_zenoh.cpp)

target_link_libraries(esmini_zenoh PUBLIC zenohcxx::zenohc ${CUDD_LIB} rvstd Boost::json Boost::boost ${OSI_LIB} protobuf::libprotobuf) #if using zenoh-c backend
