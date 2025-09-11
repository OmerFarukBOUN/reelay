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

# # Install zenoh
# RUN echo "deb [trusted=yes] https://download.eclipse.org/zenoh/debian-repo/ /" | sudo tee -a /etc/dnf/sources.list.d/zenoh.list > /dev/null \
#     && sudo dnf update -y \
#     && sudo dnf install zenoh -y

# Install rust
# RUN curl https://sh.rustup.rs -sSf | bash -s -- -y

# ENV PATH="/root/.cargo/bin:${PATH}"

# # Install zenoh-c
# RUN rustup update

# RUN git clone https://github.com/eclipse-zenoh/zenoh-c.git \
#     && mkdir -p build \
#     && cd build \
#     && cmake ../zenoh-c \
#     && cmake --build . --config Release \
#     && cmake --build . --target install

# Install zenoh-cpp
RUN git clone https://github.com/OmerFarukBOUN/zenoh-cpp.git \
    && cd zenoh-cpp \
    && mkdir build \
    && cd build \
    && cmake .. -DCMAKE_INSTALL_PREFIX=/opt/bazalt \
    && cmake --install .

RUN dnf install libXrandr-devel libXinerama-devel fontconfig-devel neovim mesa-dri-drivers mesa-libGL mesa-libGLU -y

RUN mkdir -p /opt/bazalt/include_/google && mv /opt/bazalt/include/google /opt/bazalt/include_/google

RUN git clone https://github.com/OmerFarukBOUN/esmini_new_controller.git \
    && cd esmini_new_controller \
    && mkdir build \
    && cd build \
    && cmake .. \
    && cmake --build . --config Release --target install

# RUN mv /opt/bazalt/include_/google /opt/bazalt/include/google && rm -rf /opt/bazalt/include_


# RUN sudo dnf update \
#     && sudo dnf install libprotobuf-dev protobuf-compiler libprotoc-dev libprotobuf-c-dev -y

# # Install OSI
# RUN git clone https://github.com/OpenSimulationInterface/open-simulation-interface.git \
#     && cd open-simulation-interface \
#     && mkdir build \
#     && cd build \
#     && cmake .. -G"Unix Makefiles" \
#     && make \
#     && sudo make install

WORKDIR /

# # Install esmini
# RUN wget https://github.com/esmini/esmini/releases/download/v2.45.1/esmini-demo_Linux.zip \
#     && unzip esmini-demo_Linux.zip \
#     && wget https://github.com/esmini/esmini/releases/download/v2.45.1/esmini-bin_Linux.zip \
#     && unzip esmini-bin_Linux.zip \
#     && cp -r esmini esmini-demo


RUN dnf update \
    && dnf install build-essential gdb ninja-build git pkg-config libgl1-mesa-dev \
    libpthread-stubs0-dev libjpeg-dev libxml2-dev libpng-dev libtiff5-dev libgdal-dev \
    libpoppler-dev libdcmtk-dev libgstreamer1.0-dev libgtk2.0-dev libcairo2-dev \
    libpoppler-glib-dev libxrandr-dev libxinerama-dev curl cmake black -y

# RUN mkdir zenoh-bridge

# # Install Boost with -fPIC
# RUN git clone -j$(nproc) --recursive --depth 1 --branch boost-1.82.0 https://github.com/boostorg/boost.git /tmp/boost \
#     && cd /tmp/boost \
#     && ./bootstrap.sh --prefix=/usr/local --with-libraries=json \
#     && ./b2 -j$(nproc) cxxflags=-fPIC install \
#     && cd / && rm -rf /tmp/boost

# # Install rvstd
# RUN git clone https://github.com/OmerFarukBOUN/rvstd.git /tmp/rvstd \
#     && cd /tmp/rvstd \
#     && mkdir build \
#     && cd build \
#     && cmake .. -DCMAKE_PREFIX_PATH=/usr/local \
#     && cmake --build . \
#     && sudo cmake --install . \
#     && cd / && rm -rf /tmp/rvstd

RUN git clone https://github.com/OmerFarukBOUN/reelay.git /zenoh-bridge \
    && cd /zenoh-bridge \
    && make install-zenoh \
    && echo "I"

# COPY . /home/nonroot

# RUN mkdir -p /home/nonroot/protobufs

# RUN rm -rf /zenoh-bridge/apps/zenoh_bridge/build \
#     && mkdir -p /zenoh-bridge/apps/zenoh_bridge/build \
#     && cd /zenoh-bridge/apps/zenoh_bridge/build \
#     && cmake .. \
#     && cmake --build . 

# RUN cd /zenoh-bridge/apps/zenoh_bridge/build \
#     && chmod +x ./zenoh_bridge ./zenoh_test_pub \
#     && (timeout 4 ./zenoh_bridge &) \
#     && sleep 1 \
#     && (timeout 2 /zenoh-bridge/apps/zenoh_bridge/build/zenoh_test_pub &)



# ENTRYPOINT ["/zenoh-bridge/apps/zenoh_bridge/build/zenoh_bridge"]



# Add a container user (good practice)
# RUN groupadd reelay -g 1000 \
#     && useradd -ms /bin/bash reelay -g 1000 -u 1000 \
#     && printf "reelay:reelay" | chpasswd \
#     && printf "reelay ALL= NOPASSWD: ALL\\n" >> /etc/sudoers

# USER reelay

# ENV PATH "/home/reelay/.local/bin:${PATH}"

# RUN python -m pip install black gcovr cmake-format



