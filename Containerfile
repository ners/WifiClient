FROM centos:centos8

RUN echo -e 'max_parallel_downloads=20\ndiskspacecheck=0' >> /etc/dnf/dnf.conf
RUN dnf update -y
RUN dnf install -y epel-release dnf-plugins-core

RUN dnf config-manager --set-enabled powertools

RUN dnf install -y \
	clang-devel \
	cmake \
	gcc-c++ \
	libarchive \
	llvm-devel \
	make \
	mesa-dri-drivers \
	qt-creator \
	qt5-devel \
	qt5-qtquickcontrols2-devel

WORKDIR /app

CMD qtcreator
