FROM ghcr.io/bounverif/bazalt:latest-devel

USER root

RUN git clone --depth 1 --branch v3.4.0 https://github.com/catchorg/Catch2.git /tmp/catch2 \
    && cmake -S/tmp/catch2 -B/tmp/catch2/build \
    -DBUILD_TESTING=OFF \
    -DCATCH_INSTALL_DOCS=OFF \
    && cmake --build /tmp/catch2/build/ --target install -j$(nproc) \
    && rm -rf /tmp/catch2

RUN dnf update -y \
    && dnf install -y automake autoconf libtool

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
RUN git clone https://github.com/OmerFarukBOUN/zenoh-cpp.git \
    && cd zenoh-cpp \
    && mkdir build \
    && cd build \
    && cmake .. -DCMAKE_INSTALL_PREFIX=/opt/bazalt \
    && cmake --install .

RUN dnf install libXrandr-devel libXinerama-devel fontconfig-devel neovim mesa-dri-drivers mesa-libGL mesa-libGLU -y

RUN mv /opt/bazalt/include/google /opt/bazalt/include_

RUN git clone https://github.com/OmerFarukBOUN/esmini_new_controller.git \
    && cd esmini_new_controller \
    && mkdir build \
    && cd build \
    && cmake .. \
    && cmake --build . --config Release --target install

RUN mv /opt/bazalt/include_ /opt/bazalt/include/google && rm -rf /opt/bazalt/include_

# # Install OSI
RUN git clone https://github.com/OpenSimulationInterface/open-simulation-interface.git \
    && cd open-simulation-interface \
    && mkdir build \
    && cd build \
    && cmake .. \
  -DCMAKE_PREFIX_PATH=/opt/bazalt \
  -DProtobuf_INCLUDE_DIR=/opt/bazalt/include \
  -DProtobuf_LIBRARY=/opt/bazalt/lib/libprotobuf.so \
  -DProtobuf_PROTOC_EXECUTABLE=/opt/bazalt/bin/protoc \
  -G"Unix Makefiles" \
    && make \
    && make install


RUN git clone https://github.com/OmerFarukBOUN/reelay.git /zenoh-bridge \
    && cd /zenoh-bridge \
    && make install-zenoh

RUN rm -rf /zenoh-bridge/apps/zenoh_bridge/build \
    && mkdir -p /zenoh-bridge/apps/zenoh_bridge/build \
    && cd /zenoh-bridge/apps/zenoh_bridge/build \
    && cmake .. \
    && cmake --build . 

COPY . /home/nonroot

RUN mkdir -p /home/nonroot/protobufs

ENTRYPOINT ["/zenoh-bridge/apps/zenoh_bridge/build/zenoh_bridge"]



# # Add a container user (good practice)
# # RUN groupadd reelay -g 1000 \
# #     && useradd -ms /bin/bash reelay -g 1000 -u 1000 \
# #     && printf "reelay:reelay" | chpasswd \
# #     && printf "reelay ALL= NOPASSWD: ALL\\n" >> /etc/sudoers

# # USER reelay

# # ENV PATH "/home/reelay/.local/bin:${PATH}"

# # RUN python -m pip install black gcovr cmake-format



