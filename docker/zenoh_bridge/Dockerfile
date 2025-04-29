FROM ghcr.io/doganulus/reelay-devel:latest

# Install reelay
RUN git clone https://github.com/doganulus/reelay.git --recurse-submodules \
    && cd reelay \
    && sudo -E make install

# Install zenoh
RUN echo "deb [trusted=yes] https://download.eclipse.org/zenoh/debian-repo/ /" | sudo tee -a /etc/apt/sources.list.d/zenoh.list > /dev/null \
    && sudo apt update -y \
    && sudo apt install zenoh -y

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

# Install rvstd
RUN git clone https://github.com/OmerFarukBOUN/rvstd.git \
    && cd rvstd \
    && mkdir build \
    && cd build \
    && cmake .. \
    && cmake --build . \
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


# Add a container user (good practice)
# RUN groupadd reelay -g 1000 \
#     && useradd -ms /bin/bash reelay -g 1000 -u 1000 \
#     && printf "reelay:reelay" | chpasswd \
#     && printf "reelay ALL= NOPASSWD: ALL\\n" >> /etc/sudoers

# USER reelay

# ENV PATH "/home/reelay/.local/bin:${PATH}"

# RUN python -m pip install black gcovr cmake-format



