GXX = gcc

GXX_FLAGS = -g
LINKED_LIBS = -lWs2_32 -ldxgi -ldxguid -lole32

BUILD = build
OBJ = $(BUILD)/obj
EXEC = benji

SRC = $(wildcard src/*.c)
OBJS = $(subst src/, $(OBJ)/, $(addsuffix .o, $(basename $(SRC))))

all: clean compile

compile: $(BUILD)/$(EXEC)

$(BUILD)/$(EXEC): $(OBJS)
ifeq ($(OS), Windows_NT)
	$(GXX) $(OBJS) -o $@ $(LINKED_LIBS)
else ifeq ($(shell uname), Linux)
	$(GXX) $(OBJS) -o $@
endif

$(OBJ)/%.o: src/%.c
	$(GXX) $(GXX_FLAGS) -c $< -o $@

ifeq ($(OS), Windows_NT)
.SILENT: clean
endif

.PHONY: clean
clean: mkbuild
ifeq ($(OS), Windows_NT)
	del /Q /S $(BUILD)\*
else ifeq ($(shell uname), Linux)
	find $(BUILD) -maxdepth 1 -type f -exec rm {} \;
	rm -rf $(OBJ)/*
endif

mkbuild:
ifeq ($(OS), Windows_NT)
	if not exist "$(BUILD)" mkdir "$(BUILD)"
	if not exist "$(OBJ)" mkdir "$(OBJ)"
else ifeq ($(shell uname), Linux)
	mkdir -p $(BUILD)
	mkdir -p $(OBJ)
endif