all: build

BUILD_DIR ?= build

# If the clean target was requested, enforce clean before regeneration
CLEAN := $(filter clean,${MAKECMDGOALS})
cmake: ${CLEAN}
	cmake -S. -B${BUILD_DIR} -DCMAKE_BUILD_TYPE=Debug

build: cmake
	cmake --build ${BUILD_DIR} -j$(shell nproc)

clean:
	rm -rf ${BUILD_DIR}


LANGS += en_GB
LANGS += de_DE
LANGS += fr_FR
TS_FILES := ${LANGS:%=languages/application_%.ts}
.PRECIOUS: ${TS_FILES}

${TS_FILES}: cmake
	@sed -i 's|TRANSLATIONS =.*|TRANSLATIONS = ${TS_FILES}|' application.pro
	lupdate-pro application.pro
	@sed -i 's|TRANSLATIONS =.*|TRANSLATIONS = $$$$files(languages/*.ts)|' application.pro
.PHONY: ${TS_FILES}

translate: ${TS_FILES}
	linguist ${TS_FILES}

docs:
	doxygen

IMAGE_NAME := centos
CONTAINER_NAME := centos
container:
	podman build -t ${IMAGE_NAME} .
	(podman container exists ${CONTAINER_NAME} && podman rm -f ${CONTAINER_NAME}) || true
	podman run -it \
		--name ${CONTAINER_NAME} \
		--device=/dev/dri:/dev/dri \
		--env QT_QPA_PLATFORM=wayland \
		--env WAYLAND_DISPLAY=${WAYLAND_DISPLAY} \
		--env XDG_RUNTIME_DIR=/tmp \
		--env XDG_SESSION_TYPE=wayland \
		-v ${XDG_RUNTIME_DIR}/${WAYLAND_DISPLAY}:/tmp/${WAYLAND_DISPLAY} \
		-v ${PWD}:/app \
		${IMAGE_NAME}

.PHONY: all build clean cmake translate docs
