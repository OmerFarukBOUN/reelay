// #include "reelay/monitors.hpp"
// #include "rvstd/tableau.hpp"
// #include "zenoh.hxx"
// #include <chrono>
// #include <fstream>
// #include <iostream>
// #include <stdio.h>
// #include <thread>

// auto main_tableau = new rvstd::tableau();

// int main() {

// }

// #include "rvstd/resource.hpp"
// #include "rvstd/tableau.hpp"
// #include "rvstd/unique_identifier.hpp"

// #include <iostream>

// int main() {
//   using namespace rvstd;

//   // Step 2: Create a tableau instance
//   tableau ctx;

//   // Step 3: Create resource instances
//   auto type = unique_identifier("type");
//   auto rel1 = unique_identifier("rel1");
//   auto arg1 = unique_identifier("arg1");
//   auto arg2 = unique_identifier("arg2");

//   resource res1(type, {{rel1, {arg1, arg2}}});
//   resource res2(type, {{rel1, {arg2, arg1}}});

//   // Step 4: Emplace resources into the tableau
//   auto id1 = ctx.emplace("resource1", res1);
//   auto id2 = ctx.emplace("resource2", std::move(res2));

//   std::cout << "Resource ID1: " << id1.to_string() << std::endl;
//   std::cout << "Resource ID2: " << id2.to_string() << std::endl;

//   auto id3 = ctx.emplace("resource1", res1);

//   std::cout << "Resource ID3: " << id3.to_string() << std::endl;

//   // Step 5: Lookup resources
//   auto lookup_id1 = ctx.lookup("resource1");
//   auto lookup_id2 = ctx.lookup("resource2");

//   std::cout << "Lookup ID1: " << lookup_id1.to_string() << std::endl;
//   std::cout << "Lookup ID2: " << lookup_id2.to_string() << std::endl;

//   // Step 6: Iterate over the tableau
//   for (const auto &[id, res] : ctx) {
//     std::cout << "Resource ID: " << id.to_string() << std::endl;
//   }

//   return 0;
// }

#include "rvstd/resource.hpp"
#include "rvstd/tableau.hpp"
#include "rvstd/unique_identifier.hpp"

#include <iostream>

int main() {
  using namespace rvstd;

  // Step 2: Create a tableau instance
  tableau ctx;

  // Step 3: Create resource instances
  auto type = unique_identifier("type");
  auto rel1 = unique_identifier("rel1");
  auto arg1 = unique_identifier("arg1");
  auto arg2 = unique_identifier("arg2");

  resource res1(type, {{rel1, {arg1, arg2}}});
  resource res2(type, {{rel1, {arg2, arg1}}});

  // Step 4: Emplace resources into the tableau
  auto id1 = ctx.emplace("resource1", res1);
  auto id2 = ctx.emplace("resource2", std::move(res2));

  // Step 5: Find and access resources
  auto it1 = ctx.find(id1);
  if (it1 != ctx.end()) {
    const auto &found_resource1 = it1->second;
    std::cout << "Found resource1 with ID: " << id1.to_string() << std::endl;
    // Access resource attributes
    std::cout << "Resource1 code: " << found_resource1.code().to_string()
              << std::endl;
    std::cout << "Resource1 rel1 arg1: "
              << found_resource1.relations().at(rel1).at(0).to_string()
              << std::endl;
  } else {
    std::cout << "resource1 not found" << std::endl;
  }

  auto it2 = ctx.find(id2);
  if (it2 != ctx.end()) {
    const auto &found_resource2 = it2->second;
    std::cout << "Found resource2 with ID: " << id2.to_string() << std::endl;
    // Access resource attributes
    std::cout << "Resource2 code: " << found_resource2.code().to_string()
              << std::endl;
  } else {
    std::cout << "resource2 not found" << std::endl;
  }

  return 0;
}