WORKSPACE := ${PWD}
BUILD_DIRECTORY := /tmp/$(basename $(notdir ${WORKSPACE}))/build
JOBSERVER_FLAGS := $(filter --jobserver-fds%,$(MAKEFLAGS))
JLAGS := $(filter -j%,$(MAKEFLAGS)) $(filter -j,$(MAKEFLAGS))
CMAKE := cmake -- $(JOBSERVER_FLAGS) $(JLAGS)

.PHONY: all configure build test cbuild cryjson

configure:
	$(CMAKE) -S $(WORKSPACE) -B $(BUILD_DIRECTORY) -DUSE_ZENOH=OFF

configure-zenoh:
	$(CMAKE) -S $(WORKSPACE) -B $(BUILD_DIRECTORY) -DUSE_ZENOH=ON

build: configure
	$(CMAKE) --build $(BUILD_DIRECTORY) 

build-zenoh: configure-zenoh
	$(CMAKE) --build $(BUILD_DIRECTORY) 

test: build
	ctest --test-dir $(BUILD_DIRECTORY) --output-on-failure

install: build
	$(CMAKE) --build $(BUILD_DIRECTORY) --target install

install-zenoh: build-zenoh
	$(CMAKE) --build $(BUILD_DIRECTORY) --target install

cdevel:
	docker build -t ghcr.io/doganulus/reelay:devel docker/devel

cryjson:
	docker build -t ghcr.io/doganulus/reelay:ryjson docker/ryjson

cbenchmark:
	docker build -t ghcr.io/doganulus/reelay-benchmark:latest docker/benchmark
