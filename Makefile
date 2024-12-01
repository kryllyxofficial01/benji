GXX = g++

GXX_FLAGS = -g -std=c++17
LINKED_LIBS = -lWs2_32
INCLUDE =

BUILD = build
OBJ = $(BUILD)/obj

WINDOWS_SRC = $(wildcard src/windows/*.cpp)
WINDOWS_OBJS = $(subst src/windows/, $(OBJ)/, $(addsuffix .o, $(basename $(WINDOWS_SRC))))

all: clean windows

windows: $(BUILD)/benji-service

$(BUILD)/benji-service: $(WINDOWS_OBJS)
	$(GXX) $(WINDOWS_OBJS) -o $@ $(LINKED_LIBS)

$(OBJ)/%.o: src/windows/%.cpp
	$(GXX) $(GXX_FLAGS) -c $< -o $@

.PHONY: clean
.SILENT: clean
clean: mkbuild
ifeq ($(OS), Windows_NT)
	del /Q /S $(BUILD)\*
endif

mkbuild:
ifeq ($(OS), Windows_NT)
	if not exist "$(BUILD)" mkdir "$(BUILD)"
	if not exist "$(OBJ)" mkdir "$(OBJ)"
endif