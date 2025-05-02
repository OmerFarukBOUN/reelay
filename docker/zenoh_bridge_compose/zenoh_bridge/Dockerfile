FROM ghcr.io/bounverif/bazalt:latest-devel

USER root

RUN git clone --depth 1 --branch v3.4.0 https://github.com/catchorg/Catch2.git /tmp/catch2 \
    && cmake -S/tmp/catch2 -B/tmp/catch2/build \
    -DBUILD_TESTING=OFF \
    -DCATCH_INSTALL_DOCS=OFF \
    && cmake --build /tmp/catch2/build/ --target install -j$(nproc) \
    && rm -rf /tmp/catch2

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

# Install OSI
RUN git clone https://github.com/OpenSimulationInterface/open-simulation-interface.git \
    && cd open-simulation-interface \
    && mkdir build \
    && cd build \
    && cmake .. -G"Unix Makefiles" \
    && make \
    && sudo make install

WORKDIR /

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

RUN git clone https://github.com/OmerFarukBOUN/reelay.git /zenoh-bridge \
    && cd /zenoh-bridge \
    && sudo -E make install-zenoh \
    && echo "j"

COPY . /home/nonroot

RUN mkdir -p /home/nonroot/protobufs

RUN rm -rf /zenoh-bridge/apps/zenoh_bridge/build \
    && mkdir -p /zenoh-bridge/apps/zenoh_bridge/build \
    && cd /zenoh-bridge/apps/zenoh_bridge/build \
    && cmake .. \
    && cmake --build . 

# RUN cd /zenoh-bridge/apps/zenoh_bridge/build \
#     && chmod +x ./zenoh_bridge ./zenoh_test_pub \
#     && (timeout 4 ./zenoh_bridge &) \
#     && sleep 1 \
#     && (timeout 2 /zenoh-bridge/apps/zenoh_bridge/build/zenoh_test_pub &)



ENTRYPOINT ["/zenoh-bridge/apps/zenoh_bridge/build/zenoh_bridge"]



# Add a container user (good practice)
# RUN groupadd reelay -g 1000 \
#     && useradd -ms /bin/bash reelay -g 1000 -u 1000 \
#     && printf "reelay:reelay" | chpasswd \
#     && printf "reelay ALL= NOPASSWD: ALL\\n" >> /etc/sudoers

# USER reelay

# ENV PATH "/home/reelay/.local/bin:${PATH}"

# RUN python -m pip install black gcovr cmake-format



