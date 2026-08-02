cmake_minimum_required(VERSION 3.10)

# Set the project name
project(zenoh_bridge)

include_directories(/usr/local/include/reelay)
include_directories(/usr/local/include/osi3)
include_directories(/usr/local/include/yaml-cpp)
include_directories(/usr/local/lib64/)
include_directories(/usr/local/include/)
include_directories(/opt/bazalt/include/)
include_directories(/opt/bazalt/include/zenoh/)
include_directories(/opt/bazalt/lib64/)
include_directories(/pub_sub_reelay/bruh/)

set(Protobuf_INCLUDE_DIR "/opt/bazalt/include")
set(Protobuf_LIBRARIES "/opt/bazalt/lib/libprotobuf.so")
set(Protobuf_LIBRARY "/opt/bazalt/lib/libprotobuf.so")
set(Protobuf_PROTOC_EXECUTABLE "/opt/bazalt/bin/protoc")

find_package(zenohc) #if using zenoh-c backend
find_package(zenohcxx)
# find_package(rvstd)
find_package(Protobuf REQUIRED)
# find_package(yaml-cpp REQUIRED)

# if(TARGET yaml-cpp::yaml-cpp)
#   set(YAML_CPP_LIB yaml-cpp::yaml-cpp)
# else()
#   if(DEFINED YAML_CPP_LIBRARIES)
#     set(YAML_CPP_LIB ${YAML_CPP_LIBRARIES})
#   elseif(DEFINED YAML_CPP_LIBRARY)
#     set(YAML_CPP_LIB ${YAML_CPP_LIBRARY})
#   else()
#     find_library(YAML_CPP_LIB NAMES yaml-cpp libyaml-cpp yaml-cpp.so yaml-cpp.a PATHS /usr/lib64 /usr/local/lib64 /usr/lib /usr/local/lib)
#   endif()
#   if(NOT YAML_CPP_LIB)
#     message(FATAL_ERROR "yaml-cpp target not found and no YAML_CPP_LIBRARIES/YAML_CPP_LIBRARY set")
#   endif()
# endif()

find_package(Boost REQUIRED COMPONENTS json)
find_library(CUDD_LIB libcudd.so PATHS /usr/local/lib)
find_library(OSI_LIB libopen_simulation_interface.so PATHS /usr/local/lib/osi3)




# Add the executable
add_executable(proto_fbs proto_fbs_converter.cpp)
target_include_directories(proto_fbs PUBLIC /pub_sub_reelay/osi_fbs/)

target_link_libraries(proto_fbs PUBLIC ${OSI_LIB} protobuf::libprotobuf)# ${YAML_CPP_LIB}) #if using zenoh-c backend


# add_executable(test test.cpp pqrs.pb.cc)

# target_link_libraries(test PUBLIC zenohcxx::zenohc ${CUDD_LIB} Boost::json Boost::boost ${OSI_LIB} protobuf::libprotobuf)# ${YAML_CPP_LIB}) #if using zenoh-c backend

# add_executable(test_shm_pub test_shm_pub.cpp pqrs.pb.cc)

# target_link_libraries(test_shm_pub PUBLIC zenohcxx::zenohc ${CUDD_LIB} Boost::json Boost::boost ${OSI_LIB} protobuf::libprotobuf)# ${YAML_CPP_LIB}) #if using zenoh-c backend

# add_executable(test_shm_sub test_shm_sub.cpp pqrs.pb.cc)

# target_link_libraries(test_shm_sub PUBLIC zenohcxx::zenohc ${CUDD_LIB} Boost::json Boost::boost ${OSI_LIB} protobuf::libprotobuf)# ${YAML_CPP_LIB}) #if using zenoh-c backend


# include(GNUInstallDirs)

# find_package(iceoryx_platform REQUIRED)
# find_package(iceoryx_posh CONFIG REQUIRED)
# find_package(iceoryx_hoofs CONFIG REQUIRED)

# include(IceoryxPackageHelper)
# include(IceoryxPlatform)
# include(IceoryxPlatformSettings)

# iox_add_executable(
#     TARGET iceoryx_pub
#     FILES iceoryx_pub.cpp
#     LIBS iceoryx_posh::iceoryx_posh
# )

# iox_add_executable(
#     TARGET iceoryx_sub
#     FILES iceoryx_sub.cpp
#     LIBS iceoryx_posh::iceoryx_posh
# )



# find_package(iceoryx2-cxx 0.7.0 REQUIRED)

# add_executable(example_cxx_publish_subscribe_publisher ice2_pub.cpp)
# target_link_libraries(example_cxx_publish_subscribe_publisher iceoryx2-cxx::static-lib-cxx)

# add_executable(example_cxx_publish_subscribe_subscriber ice2_sub.cpp)
# target_link_libraries(example_cxx_publish_subscribe_subscriber iceoryx2-cxx::static-lib-cxx)