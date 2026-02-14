#include <chrono>
#include <cstring>
#include <iostream>
#include <string>
#include <thread>
#include <vector>
#include <stdio.h>
#include <string.h>

#include <chrono>
#include <iostream>
#include <limits>
#include <sstream>
#include <thread>
#include "zenoh.hxx"
#include <zenoh/api/shm/protocol_implementations/posix/posix_shm_provider.hxx>
#include <zenoh/api/shm/buffer/zshmmut.hxx>
using namespace zenoh;
using namespace std::chrono;

int main() {
#ifdef ZENOHCXX_ZENOHC
    init_log_from_env_or("error");
#endif
#define Z_FEATURE_SHARED_MEMORY
#define Z_FEATURE_UNSTABLE_API

    std::cout << "[Publisher] Opening session...\n";
    auto session = Session::open(Config::create_default());
    KeyExpr keyexpr("demo/latency");
    auto pub = session.declare_publisher(keyexpr);

    PosixShmProvider provider(MemoryLayout(65536, AllocAlignment({2})));

    std::vector<size_t> sizes = {10, 100, 1024, 2048, 4096, 8192};
    constexpr int N = 1000;

    for (size_t sz : sizes) {
        std::cout << "[Publisher] Sending " << N << " messages of size " << sz << " bytes...\n";

        for (int i = 0; i < N; ++i) {
            size_t msg_size = sz < sizeof(uint64_t) ? sizeof(uint64_t) : sz;

            // Allocate shared memory buffer
            auto alloc_result = provider.alloc_gc_defrag_blocking(msg_size, AllocAlignment({0}));
            ZShmMut &&buf = std::get<ZShmMut>(std::move(alloc_result));

            // Write timestamp (ns since epoch)
            uint64_t send_ns =
                duration_cast<nanoseconds>(steady_clock::now().time_since_epoch()).count();
            memcpy(buf.data(), &send_ns, sizeof(uint64_t));

            // Fill remaining bytes with dummy data
            if (msg_size > sizeof(uint64_t))
                memset(buf.data() + sizeof(uint64_t), 'A', msg_size - sizeof(uint64_t));

            pub.put(std::move(buf));

            // Ensure messages don't overlap
            std::this_thread::sleep_for(1ms);
        }

        std::cout << "[Publisher] Done with " << sz << " bytes.\n";
        std::this_thread::sleep_for(1s);
    }

    std::cout << "[Publisher] Test complete.\n";
    return 0;
}
