find_package(Catch2 REQUIRED)

include(Catch)
include(CTest)

add_executable(
  reelay_tests
  src/dense_timed.test.cpp
  src/dense_timed_data.test.cpp
  src/dense_timed_robustness_0.test.cpp
  src/discrete_timed.test.cpp
  src/discrete_timed_data.test.cpp
  src/discrete_timed_robustness.test.cpp
  src/monitors.test.cpp)

target_link_libraries(reelay_tests PRIVATE reelay::reelay)
target_link_libraries(reelay_tests PRIVATE Catch2::Catch2WithMain)
target_link_libraries(reelay_tests PRIVATE reelay::reelay protobuf::libprotobuf)

catch_discover_tests(reelay_tests)
