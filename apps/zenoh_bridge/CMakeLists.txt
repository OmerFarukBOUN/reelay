cmake_minimum_required(VERSION 3.10)

# Set the project name
project(zenoh_bridge)

include_directories(/usr/local/include/reelay)
include_directories(/usr/local/include/osi3)
# set(Protobuf_INCLUDE_DIR "/opt/bazalt/include")
# set(Protobuf_LIBRARIES "/opt/bazalt/lib/libprotobuf.so")
# set(Protobuf_LIBRARY "/opt/bazalt/lib/libprotobuf.so")
# set(Protobuf_PROTOC_EXECUTABLE "/opt/bazalt/bin/protoc")

find_package(zenohc) #if using zenoh-c backend
find_package(zenohcxx)
# find_package(rvstd)
find_package(Protobuf REQUIRED)
find_library(CUDD_LIB libcudd.so PATHS /usr/local/lib)
find_library(OSI_LIB libopen_simulation_interface.so PATHS /usr/local/lib/osi3)


# Add the executable
add_executable(zenoh_bridge zenoh_bridge.cpp)

find_package(Boost REQUIRED COMPONENTS json)

target_link_libraries(zenoh_bridge PUBLIC zenohcxx::zenohc ${CUDD_LIB} Boost::json Boost::boost ${OSI_LIB} protobuf::libprotobuf) #if using zenoh-c backend

add_executable(zenoh_test_sub test_sub.cpp)

target_link_libraries(zenoh_test_sub PUBLIC zenohcxx::zenohc ${CUDD_LIB} protobuf::libprotobuf Boost::json Boost::boost) #if using zenoh-c backend

add_executable(zenoh_test_pub test_pub.cpp)

target_link_libraries(zenoh_test_pub PUBLIC zenohcxx::zenohc ${CUDD_LIB} protobuf::libprotobuf Boost::json Boost::boost) #if using zenoh-c backend

add_executable(zenoh_test_pub_esmini test_pub_esmini.cpp)

target_link_libraries(zenoh_test_pub_esmini PUBLIC zenohcxx::zenohc ${CUDD_LIB} Boost::json Boost::boost ${OSI_LIB} protobuf::libprotobuf) #if using zenoh-c backend