ERR = $(shell which clang >/dev/null; echo $$?)
ifeq "$(ERR)" "0"
    CC = clang
else
    CC = gcc
endif

override CFLAGS := $(CFLAGS) -std=gnu99 
debug: export CFLAGS += -g

export REL_OBJ_PATH = build/release
export DBG_OBJ_PATH = build/debug
release: export BUILD_PATH = $(REL_OBJ_PATH)
debug: export BUILD_PATH = $(DBG_OBJ_PATH)

export REL_BIN_PATH = bin/release
export DBG_BIN_PATH = bin/debug
release: export BIN_PATH = $(REL_BIN_PATH)
debug: export BIN_PATH = $(DBG_BIN_PATH)

export SOURCES = decodeStage.c dump.c executeStage.c fetchStage.c main.c \
					memory.c memoryStage.c registers.c tools.c writebackStage.c \
					loader.c
export OBJECTS = $(SOURCES:%.c=$(BUILD_PATH)/%.o)
export BIN = yess

release: dirs
	make all

debug: dirs
	make all

dirs:
	mkdir -p $(BUILD_PATH)
	mkdir -p $(BIN_PATH)

all: $(BIN_PATH)/$(BIN)
	$(RM) $(BIN)
	ln -s $(BIN_PATH)/$(BIN) $(BIN)

$(BIN_PATH)/$(BIN): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@

$(BUILD_PATH)/%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) -r build
	$(RM) -r bin
	$(RM) $(BIN)

.PHONY: release debug clean dirs
