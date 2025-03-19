FROM ghcr.io/bounverif/bazalt:latest-devel

USER root

# ENV PIP_BREAK_SYSTEM_PACKAGES 1
# ENV CMAKE_GENERATOR=Ninja

# RUN apt-get update && export DEBIAN_FRONTEND=noninteractive \
#     && apt-get -y install \
#       sudo \
#       git \
#       build-essential \
#       tar curl wget zip unzip gnupg2 \
#       cmake \
#       ninja-build \
#       python3-pip \
#       python3-venv \
#       python-is-python3 \
#     && apt-get autoremove -y && apt-get clean -y && rm -rf /var/lib/apt/lists/*

# RUN apt-get update && export DEBIAN_FRONTEND=noninteractive \
#     && apt-get -y install \
#       clang \
#       clangd \
#       clang-format \
#       clang-tidy \
#       clang-tools \
#     && apt-get autoremove -y && apt-get clean -y && rm -rf /var/lib/apt/lists/*

# RUN git clone -j$(nproc) --recursive --depth 1 --branch boost-1.82.0 https://github.com/boostorg/boost.git /tmp/boost \
#     && cd /tmp/boost \
#     && ./bootstrap.sh --prefix=/usr/local --without-libraries=python \
#     && ./b2 -j$(nproc) install \
#     && cd / && rm -rf /tmp/boost

RUN git clone --depth 1 --branch v3.4.0 https://github.com/catchorg/Catch2.git /tmp/catch2 \
    && cmake -S/tmp/catch2 -B/tmp/catch2/build \
      -DBUILD_TESTING=OFF \
      -DCATCH_INSTALL_DOCS=OFF \
    && cmake --build /tmp/catch2/build/ --target install -j$(nproc) \
    && rm -rf /tmp/catch2

# RUN apt-get update && export DEBIAN_FRONTEND=noninteractive \
#     && apt-get -y install \
#       automake \
#       autotools-dev \
#     && apt-get autoremove -y && apt-get clean -y && rm -rf /var/lib/apt/lists/*

RUN apt-get update \
    && apt-get install -y automake autoconf libtool

RUN git clone https://github.com/doganulus/cudd.git /tmp/cudd \
    && cd /tmp/cudd \
    && ./configure && aclocal && autoconf \
    && ./configure --enable-silent-rules --enable-shared --enable-obj \
    && make && make install \
    && cd / && rm -rf /tmp/cudd

RUN git clone --depth 1 --branch v3.2.2 https://github.com/lemire/simdjson.git /tmp/simdjson \
    && cmake -S/tmp/simdjson -B/tmp/simdjson/build \
    && cmake --build /tmp/simdjson/build/ --target install -j$(nproc) \
    && rm -rf /tmp/simdjson

# RUN python -m pip install --upgrade pip \
#     && pip install \
#       gcovr \
#       py-build-cmake \
#       "pybind11[global]"

# Install reelay
# RUN git clone https://github.com/doganulus/reelay.git --recurse-submodules \
#     && cd reelay \
#     && sudo -E make install

# # Install zenoh
# RUN echo "deb [trusted=yes] https://download.eclipse.org/zenoh/debian-repo/ /" | sudo tee -a /etc/apt/sources.list.d/zenoh.list > /dev/null \
#     && sudo apt update -y \
#     && sudo apt install zenoh -y

# Install rust
RUN curl https://sh.rustup.rs -sSf | bash -s -- -y

ENV PATH="/root/.cargo/bin:${PATH}"

# Install zenoh-c
RUN rustup update

RUN git clone https://github.com/eclipse-zenoh/zenoh-c.git \
    && mkdir -p build \
    && cd build \
    && cmake ../zenoh-c \
    && cmake --build . --config Release \
    && cmake --build . --target install

# Install zenoh-cpp
RUN git clone https://github.com/eclipse-zenoh/zenoh-cpp.git \
    && cd zenoh-cpp \
    && mkdir build \
    && cd build \
    && cmake .. \
    && sudo cmake --install .



RUN sudo apt-get update \
    && sudo apt-get install libprotobuf-dev protobuf-compiler libprotoc-dev libprotobuf-c-dev -y

# RUN git clone https://github.com/protocolbuffers/protobuf.git \
#     && cd protobuf \
#     && cmake . -DCMAKE_CXX_STANDARD=17 \
#     && cmake --build . \
#     && sudo cmake --install . \
#     && sudo ldconfig

# Install OSI
RUN git clone https://github.com/OpenSimulationInterface/open-simulation-interface.git \
    && cd open-simulation-interface \
    && mkdir build \
    && cd build \
    && cmake .. -G"Unix Makefiles" \
    && make \
    && sudo make install

# Install esmini
RUN wget https://github.com/esmini/esmini/releases/download/v2.45.1/esmini-demo_Linux.zip \
    && unzip esmini-demo_Linux.zip \
    && wget https://github.com/esmini/esmini/releases/download/v2.45.1/esmini-bin_Linux.zip \
    && unzip esmini-bin_Linux.zip \
    && cp -r esmini esmini-demo


RUN sudo apt-get update \
    && sudo apt-get install build-essential gdb ninja-build git pkg-config libgl1-mesa-dev \
    libpthread-stubs0-dev libjpeg-dev libxml2-dev libpng-dev libtiff5-dev libgdal-dev \
    libpoppler-dev libdcmtk-dev libgstreamer1.0-dev libgtk2.0-dev libcairo2-dev \
    libpoppler-glib-dev libxrandr-dev libxinerama-dev curl cmake black -y

RUN mkdir zenoh-bridge

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
    && sudo -E make install-zenoh \
    && echo "B"





RUN rm -rf /zenoh-bridge/apps/zenoh_bridge/build \
    && mkdir -p /zenoh-bridge/apps/zenoh_bridge/build \
    && cd /zenoh-bridge/apps/zenoh_bridge/build \
    && cmake .. \
    && cmake --build . 
RUN cd /zenoh-bridge/apps/zenoh_bridge/build \
    && chmod +x ./zenoh_bridge ./zenoh_test_pub \
    && (timeout 4 ./zenoh_bridge &) \
    && sleep 1 \
    && (timeout 2 /zenoh-bridge/apps/zenoh_bridge/build/zenoh_test_pub &)



# Add a container user (good practice)
# RUN groupadd reelay -g 1000 \
#     && useradd -ms /bin/bash reelay -g 1000 -u 1000 \
#     && printf "reelay:reelay" | chpasswd \
#     && printf "reelay ALL= NOPASSWD: ALL\\n" >> /etc/sudoers

# USER reelay

# ENV PATH "/home/reelay/.local/bin:${PATH}"

# RUN python -m pip install black gcovr cmake-format



