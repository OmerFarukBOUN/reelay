//
// Copyright (c) 2022 ZettaScale Technology
//
// This program and the accompanying materials are made available under the
// terms of the Eclipse Public License 2.0 which is available at
// http://www.eclipse.org/legal/epl-2.0, or the Apache License, Version 2.0
// which is available at https://www.apache.org/licenses/LICENSE-2.0.
//
// SPDX-License-Identifier: EPL-2.0 OR Apache-2.0
//
// Contributors:
//   ZettaScale Zenoh Team, <zenoh@zettascale.tech>
//
#include <stdio.h>

#include <chrono>
#include <iostream>
#include <thread>

#include "getargs.hxx"
#include "zenoh.hxx"

using namespace zenoh;
using namespace std::chrono_literals;

const char *kind_to_str(SampleKind kind) {
    switch (kind) {
        case SampleKind::Z_SAMPLE_KIND_PUT:
            return "PUT";
        case SampleKind::Z_SAMPLE_KIND_DELETE:
            return "DELETE";
        default:
            return "UNKNOWN";
    }
}

int _main(int argc, char **argv) {
    auto &&[config, args] =
        ConfigCliArgParser(argc, argv)
            .named_value({"k", "key"}, "KEY_EXPRESSION", "Key expression to subscribe to (string)", "demo/example/**")
            .run();

    KeyExpr keyexpr(args.value("key"));

    std::cout << "Opening session..." << std::endl;
    auto session = Session::open(std::move(config));

    auto data_handler = [](const Sample &sample) {
        std::cout << ">> [Subscriber] Received " << kind_to_str(sample.get_kind()) << " ('"
                  << sample.get_keyexpr().as_string_view() << "' : '" << sample.get_payload().as_string() << "')";

        auto attachment = sample.get_attachment();
        if (attachment.has_value()) {
            std::cout << "  (" << attachment->get().as_string() << ")";
        }
        std::cout << std::endl;
    };

    std::cout << "Declaring Subscriber on '" << keyexpr.as_string_view() << "'..." << std::endl;
    auto subscriber = session.declare_subscriber(keyexpr, data_handler, closures::none);

    std::cout << "Press CTRL-C to quit...\n";
    while (true) {
        std::this_thread::sleep_for(1s);
    }

    return 0;
}

int main(int argc, char **argv) {
    try {
#ifdef ZENOHCXX_ZENOHC
        init_log_from_env_or("error");
#endif
        _main(argc, argv);
    } catch (ZException e) {
        std::cout << "Received an error :" << e.what() << "\n";
    }
}