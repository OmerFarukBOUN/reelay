FROM ghcr.io/bounverif/bazalt:latest-devel

USER root

RUN dnf update -y \
    && dnf install automake autoconf libtool libXrandr-devel libXinerama-devel \
    fontconfig-devel neovim mesa-dri-drivers mesa-libGL mesa-libGLU libacl-devel \
    gcc-c++ glibc-devel libstdc++-devel clang clang-devel llvm-devel -y

RUN git clone --depth 1 --branch v3.4.0 https://github.com/catchorg/Catch2.git /tmp/catch2 \
    && cmake -S/tmp/catch2 -B/tmp/catch2/build \
    -DBUILD_TESTING=OFF \
    -DCATCH_INSTALL_DOCS=OFF \
    && cmake --build /tmp/catch2/build/ --target install -j$(nproc) \
    && rm -rf /tmp/catch2

RUN git clone https://github.com/doganulus/cudd.git /tmp/cudd \
    && cd /tmp/cudd \
    && ./configure && aclocal && autoconf \
    && ./configure --enable-silent-rules --enable-shared --enable-obj \
    && make && make install \
    && cd / && rm -rf /tmp/cudd

RUN git clone --depth 1 --branch v3.2.2 https://github.com/lemire/simdjson.git /tmp/simdjson \
    && cmake -S/tmp/simdjson -B/tmp/simdjson/build -DSIMDJSON_IMPLEMENTATION=westmere -DCMAKE_CXX_FLAGS="-march=native" \
    && cmake --build /tmp/simdjson/build/ --target install -j$(nproc) \
    && rm -rf /tmp/simdjson

# Install zenoh-cpp
RUN git clone https://github.com/OmerFarukBOUN/zenoh-cpp.git /tmp/zenoh-cpp \
    && cmake -S/tmp/zenoh-cpp -B/tmp/zenoh-cpp/build -DCMAKE_PREFIX_PATH=/opt/bazalt \
    && cmake --build /tmp/zenoh-cpp/build --target install -j$(nproc) \
    && rm -rf /tmp/zenoh-cpp

RUN git clone https://github.com/google/flatbuffers.git /tmp/flatbuffers \
    && cmake -S/tmp/flatbuffers -B/tmp/flatbuffers/build -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++ \
    && cmake --build /tmp/flatbuffers/build --target install -j$(nproc) \
    && rm -rf /tmp/flatbuffers

RUN git clone https://github.com/OmerFarukBOUN/reelay.git /zenoh-bridge \
    && cd /zenoh-bridge \
    && make install-zenoh -j$(nproc)

RUN git clone https://github.com/OpenSimulationInterface/open-simulation-interface.git /tmp/osi \
    && git -C /tmp/osi checkout v3.5.0

RUN mv /opt/bazalt/include/google /opt/bazalt/include_

# RUN git clone https://github.com/OmerFarukBOUN/esmini_new_controller.git /tmp/esmini \
#     && cmake -S/tmp/esmini -B/tmp/esmini/build  \
#     && cmake --build /tmp/esmini/build --config Release --target install -j$(nproc) \
#     && rm -rf /tmp/esmini

# RUN mv /opt/bazalt/include_ /opt/bazalt/include/google && rm -rf /opt/bazalt/include_



# # # # Install OSI
# # RUN git clone https://github.com/OpenSimulationInterface/open-simulation-interface.git \
# #     && cd open-simulation-interface \
# #     && mkdir build \
# #     && cd build \
# #     && cmake .. \
# #     -DCMAKE_PREFIX_PATH=/opt/bazalt \
# #     -DProtobuf_INCLUDE_DIR=/opt/bazalt/include \
# #     -DProtobuf_LIBRARY=/opt/bazalt/lib/libprotobuf.so \
# #     -DProtobuf_PROTOC_EXECUTABLE=/opt/bazalt/bin/protoc \
# #     -G"Unix Makefiles" \
# #     && make \
# #     && make install

# RUN rm -rf /zenoh-bridge/apps/zenoh_bridge/build \
#     && mkdir -p /zenoh-bridge/apps/zenoh_bridge/build \
#     && cd /zenoh-bridge/apps/zenoh_bridge/build \
#     && cmake .. \
#     && cmake --build . -j$(nproc)

# COPY . /home/nonroot

# RUN mkdir -p /home/nonroot/protobufs

# # Install rust
# RUN curl https://sh.rustup.rs -sSf | bash -s -- -y

# ENV PATH="/root/.cargo/bin:${PATH}"

# RUN rustup update


# RUN dnf install libacl-devel gcc-c++ glibc-devel \
#     libstdc++-devel \
#     clang \
#     clang-devel \
#     llvm-devel -y

# ENV PATH="/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin"

# RUN git clone https://github.com/eclipse-iceoryx/iceoryx.git \
#     && cd iceoryx \
#     && cmake -Bbuild -Hiceoryx_meta -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++ \
#     # && cmake -Bbuild -Hiceoryx_meta -DCMAKE_PREFIX_PATH=$(PWD)/build/dependencies/ \
#     && cmake --build build -j$(nproc) \
#     && cmake --build build --target install

# RUN git clone https://github.com/eclipse-iceoryx/iceoryx2.git \
#     && cd iceoryx2 \
#     && cmake -S . -B build -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++ \
#     && cmake --build build -j$(nproc) \
#     && cmake --install build



# # ENTRYPOINT ["/zenoh-bridge/apps/zenoh_bridge/build/zenoh_bridge"]



# # # Add a container user (good practice)
# # # RUN groupadd reelay -g 1000 \
# # #     && useradd -ms /bin/bash reelay -g 1000 -u 1000 \
# # #     && printf "reelay:reelay" | chpasswd \
# # #     && printf "reelay ALL= NOPASSWD: ALL\\n" >> /etc/sudoers

# # # USER reelay

# # # ENV PATH "/home/reelay/.local/bin:${PATH}"

# # # RUN python -m pip install black gcovr cmake-format



