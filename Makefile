GXX = g++

GXX_FLAGS = -g -std=c++17 -Wno-return-type

BUILD = build

WINDOWS_SRC = $(wildcard src/windows/*.cpp)
LINUX_SRC = $(wildcard src/linux/*.cpp)

all: windows linux

windows:
	$(GXX) $(GXX_FLAGS) $(WINDOWS_SRC) -o $(BUILD)/server-manager-service

linux:
	mkdir -p $(BUILD)
	$(GXX) $(GXX_FLAGS) $(LINUX_SRC) -o $(BUILD)/server-managerd