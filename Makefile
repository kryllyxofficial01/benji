GXX = g++

GXX_FLAGS = -g -std=c++17 -Wno-return-type
LINKED_LIBS = -lWs2_32

BUILD = build

WINDOWS_SRC = $(wildcard src/windows/*.cpp)
LINUX_SRC = $(wildcard src/linux/*.cpp)

all: windows linux

windows:
	$(GXX) $(GXX_FLAGS) $(WINDOWS_SRC) -o $(BUILD)/benji-service $(LINKED_LIBS)

linux:
	mkdir -p $(BUILD)
	$(GXX) $(GXX_FLAGS) $(LINUX_SRC) -o $(BUILD)/benjid