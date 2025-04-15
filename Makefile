# YES THIS MAKEFILE WAS GENERATE BY AI IF YOU HAVENT GUESSED YET

# Project Configuration
NAME := classparse
SRC_DIR := src
BUILD_DIR := build
BIN_DIR := bin
HEADER := $(SRC_DIR)/$(NAME).h

# Toolchain Configuration
CC.linux := gcc
CC.win32 := x86_64-w64-mingw32-gcc
CC.wasm := emcc

CFLAGS.linux := -fPIC
CFLAGS.win32 := 
CFLAGS.wasm := -s SIDE_MODULE=1 -s WASM=1

LDFLAGS.linux := -shared -fPIC
LDFLAGS.win32 := -shared
LDFLAGS.wasm := -s SIDE_MODULE=1 -s WASM=1 -s EXPORT_ALL=1

EXT.linux := .so
EXT.win32 := .dll
EXT.wasm := .wasm

# Build Configuration
CFLAGS_DEV := -std=gnu11 -O0 -g -Wall -Wextra -fsanitize=address -fno-omit-frame-pointer -pg
LDFLAGS_DEV := -g -fsanitize=address -pg

CFLAGS_PROD := -std=gnu11 -O3 -flto -Wall -Wextra
LDFLAGS_PROD := -O3 -flto

# File Lists
SRCS := $(wildcard $(SRC_DIR)/*.c)
OBJS := $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRCS))

# Default to host platform if not specified
ifeq ($(OS),Windows_NT)
    HOST_PLATFORM := win32
else
    UNAME_S := $(shell uname -s)
    ifeq ($(UNAME_S),Linux)
        HOST_PLATFORM := linux
    endif
endif
TARGET ?= $(HOST_PLATFORM)

# Validate target
SUPPORTED_TARGETS := linux win32 wasm
ifneq ($(filter $(TARGET),$(SUPPORTED_TARGETS)),)
    # Set toolchain based on target
    CC := $(CC.$(TARGET))
    CFLAGS_PLATFORM := $(CFLAGS.$(TARGET))
    LDFLAGS_PLATFORM := $(LDFLAGS.$(TARGET))
    EXT := $(EXT.$(TARGET))
else
    $(error Unsupported TARGET. Choose from: $(SUPPORTED_TARGETS))
endif

# Default to dev mode
MODE ?= dev

# Set build flags based on mode
ifeq ($(MODE),prod)
    CFLAGS := $(CFLAGS_PROD) $(CFLAGS_PLATFORM)
    LDFLAGS := $(LDFLAGS_PROD) $(LDFLAGS_PLATFORM)
else
    CFLAGS := $(CFLAGS_DEV) $(CFLAGS_PLATFORM)
    LDFLAGS := $(LDFLAGS_DEV) $(LDFLAGS_PLATFORM)
endif

# Final Output
LIBRARY := $(BIN_DIR)/lib$(NAME)$(EXT)

CFLAGS += -I$(SRC_DIR)

# Phony Targets
.PHONY: all dev prod install uninstall clean

all: $(LIBRARY)

dev:
	$(MAKE) MODE=dev

prod:
	$(MAKE) MODE=prod

# Main Build Rule
$(LIBRARY): $(OBJS) | $(BIN_DIR)
	$(CC) $(OBJS) $(LDFLAGS) -o $@

# Object File Rule
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Directory Creation
$(BUILD_DIR) $(BIN_DIR):
	mkdir -p $@

# Clean
clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)

# Install/Uninstall
install: $(LIBRARY)
	@if [ "$(TARGET)" != "linux" ]; then \
		echo "Error: Can only install Linux target (.so)"; \
		exit 1; \
	fi
	cp $(HEADER) /usr/local/include/
	cp $(LIBRARY) /usr/local/lib/
	ldconfig

uninstall:
	@if [ "$(TARGET)" != "linux" ]; then \
		echo "Error: Can only uninstall Linux target (.so)"; \
		exit 1; \
	fi
	rm -f /usr/local/include/$(NAME).h
	rm -f /usr/local/lib/$(NAME)$(EXT)
	ldconfig
