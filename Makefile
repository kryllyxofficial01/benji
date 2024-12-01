GXX = g++

GXX_FLAGS = -g -std=c++17
LINKED_LIBS = -lWs2_32
INCLUDE =

BUILD = build

WINDOWS_SRC = $(wildcard src/windows/*.cpp)
WINDOWS_OBJS = $(subst src/windows/, $(BUILD)/, $(addsuffix .o, $(basename $(WINDOWS_SRC))))

all: clean windows

windows: $(BUILD)/benji-service

$(BUILD)/benji-service: $(WINDOWS_OBJS)
	$(GXX) $(WINDOWS_OBJS) -o $@ $(LINKED_LIBS)

$(BUILD)/%.o: src/windows/%.cpp
	$(GXX) $(GXX_FLAGS) -c $< -o $@

.PHONY: clean
.SILENT: clean
clean:
ifeq ($(OS), Windows_NT)
	del /Q /S $(BUILD)\*
endif