FROM debian:11
RUN \
  export DEBIAN_FRONTEND=noninteractive \
  && apt-get update && apt-get -y install wget gnupg ca-certificates \
  && wget -O- https://apt.llvm.org/llvm-snapshot.gpg.key | apt-key add - \
  && echo 'deb http://apt.llvm.org/bullseye/ llvm-toolchain-bullseye main' >>/etc/apt/sources.list \
  && apt-get update && apt-get -y install --no-install-recommends \
  clang \
  clang-format \
  cmake \
  git \
  libegl-dev \
  libgles-dev \
  libx11-dev \
  libxext-dev \
  make \
  && rm -rf /var/lib/apt/lists/* \
  && echo 'export PS1="\W$ "' > ~/.bashrc
