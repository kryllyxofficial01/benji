GXX = gcc

GXX_FLAGS = -g -Wall -Werror -Wno-unknown-pragmas -Wno-unused-function -Wno-unused-variable -Wno-unused-value
LINKED_LIBS = -lWs2_32 -ldxgi -ldxguid

BUILD = build
OBJ = $(BUILD)/obj
EXEC = benji

SRC = $(wildcard src/*.c)
OBJS = $(subst src/, $(OBJ)/, $(addsuffix .o, $(basename $(SRC))))

all: mkbuild compile

compile: $(BUILD)/$(EXEC)

$(BUILD)/$(EXEC): $(OBJS)
ifeq ($(OS), Windows_NT)
	$(GXX) $(OBJS) -o $@ $(LINKED_LIBS)
else ifeq ($(shell uname), Linux)
	$(GXX) $(OBJS) -o $@
endif

$(OBJ)/%.o: src/%.c
	$(GXX) $(GXX_FLAGS) -c $< -o $@

.PHONY: clean
.SILENT: clean
clean:
ifeq ($(OS), Windows_NT)
	del /Q /S $(BUILD)\*
else ifeq ($(shell uname), Linux)
	rm -rf $(BUILD)/*
endif

mkbuild: clean
ifeq ($(OS), Windows_NT)
	if not exist "$(BUILD)" mkdir "$(BUILD)"
	if not exist "$(OBJ)" mkdir "$(OBJ)"
else ifeq ($(shell uname), Linux)
	mkdir -p $(BUILD)
	mkdir -p $(OBJ)
endif