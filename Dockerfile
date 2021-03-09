FROM ubuntu:20.04

ENV TZ=Europe/Berlin
RUN ln -snf /usr/share/zoneinfo/$TZ /etc/localtime && echo $TZ > /etc/timezone

RUN apt-get update && apt-get install -y sudo git build-essential cmake sudo dbus vim libsystemd-dev pkg-config

RUN mkdir -p /var/run/dbus

RUN echo $(date)
RUN git clone https://github.com/Kistler-Group/sdbus-cpp && cd sdbus-cpp && mkdir build && cd build && cmake .. -DCMAKE_INSTALL_PREFIX=/usr && make -j$(nproc) && sudo make install

RUN useradd testuser
