# Pushed to hub.docker.com, image jengelen/finitefield

FROM gcc:latest

RUN apt-get update && apt-get install -y \
        build-essential \
        git \
        libgmp3-dev \
        libgtest-dev \
        libprocps-dev \
        libssl-dev \
        pkg-config \
    && wget -O cmake-Linux-x86_64.sh https://cmake.org/files/v3.10/cmake-3.10.3-Linux-x86_64.sh \
    && sh ./cmake-Linux-x86_64.sh --prefix=/usr/local --skip-license \
    && cmake --version \
    && rm -f ./cmake-Linux-x86_64.sh

# NTL takes a long time, so do this step as early as possible.
RUN wget -O ntl.tar.xz http://www.shoup.net/ntl/ntl-10.5.0.tar.gz \
    && mkdir ntl \
    && tar -xf ntl.tar.* --strip 1 -C ntl \
    && cd ntl/src \
    && ./configure NTL_STD_CXX14=on \
    && make \
    && make install \
    && cd ../.. \
    && rm -rf ntl \
    && rm -f ntl.*

RUN git clone https://github.com/scipr-lab/libff.git \
    && mv libff/libff /usr/local/include/ \
    && rm -rf libff \
    && mkdir gtest && cd gtest \
    && cmake /usr/src/googletest/googletest -DCMAKE_BUILD_TYPE=RELEASE \
    && make \
    && mv libgtest* /usr/lib/ \
    && cd .. \
    && rm -rf gtest \
    && git clone https://github.com/google/benchmark.git \
    && cd benchmark \
    && mkdir build && cd build \
    && cmake .. -DCMAKE_BUILD_TYPE=RELEASE \
    && make install \
    && cd ../.. \
    && rm -rf benchmark

CMD ["/bin/bash"]
