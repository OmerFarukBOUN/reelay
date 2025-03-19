WORKSPACE := ${PWD}
BUILD_DIRECTORY := /tmp/$(basename $(notdir ${WORKSPACE}))/build

.PHONY: all configure build test cbuild cryjson

configure:
	cmake -S $(WORKSPACE) -B $(BUILD_DIRECTORY) -DUSE_ZENOH=OFF

configure-zenoh:
	cmake -S $(WORKSPACE) -B $(BUILD_DIRECTORY) -DUSE_ZENOH=ON

build: configure
	cmake --build $(BUILD_DIRECTORY) 

build-zenoh: configure-zenoh
	cmake --build $(BUILD_DIRECTORY) 

test: build
	ctest --test-dir $(BUILD_DIRECTORY) --output-on-failure

install: build
	cmake --build $(BUILD_DIRECTORY) --target install

install-zenoh: build-zenoh
	cmake --build $(BUILD_DIRECTORY) --target install

cdevel:
	docker build -t ghcr.io/doganulus/reelay:devel docker/devel

cryjson:
	docker build -t ghcr.io/doganulus/reelay:ryjson docker/ryjson

cbenchmark:
	docker build -t ghcr.io/doganulus/reelay-benchmark:latest docker/benchmark
