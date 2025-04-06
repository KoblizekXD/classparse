TARGET ?= gcc

BUILD   := build
SRC     := src

ifeq ($(TARGET), wasm)
    CC := emcc
    LD := emcc
    OUTPUT := $(BUILD)/libclassparse.js
    LDFLAGS := -sERROR_ON_UNDEFINED_SYMBOLS=0
else
    CC := gcc
    LD := gcc
    OUTPUT := $(BUILD)/libclassparse.so
    LDFLAGS := -shared -fPIC
endif

CFLAGS  := -Wall -Wextra -Wpedantic -Wconversion -fPIC -std=gnu11 -I $(SRC)

CFILES := $(shell cd $(SRC) && find -L * -type f -name '*.c' | LC_ALL=C sort)
OBJ    := $(addprefix $(BUILD)/,$(CFILES:.c=.c.o)) 

.PHONY: all debug run clean examples clean-all install uninstall

all: examples

install: $(OUTPUT)
	install $(OUTPUT) /usr/lib/
	install $(SRC)/classparse.h /usr/include/

uninstall:
	rm -f /usr/include/classparse.h
	rm -f /usr/lib/libclassparse.so

debug: CFLAGS += -DDEBUG -g -O0 -fsanitize=address
debug: LDFLAGS += -g -pg -fsanitize=address
debug: examples

examples: $(OUTPUT)
	@printf "\033[32m==> Building examples\033[0m\n"
	$(MAKE) -C examples/

clean:
	@printf "\033[32m==> Clean up\033[0m\n"
	rm -rf $(BUILD)
	mkdir -p $(BUILD)

clean-all: clean
	@printf "\033[32m==> Clean up\033[0m\n"
	rm -rf examples/build/
	mkdir -p examples/build/

$(OUTPUT): $(OBJ)
	@printf "\033[32m==> Linking object files\033[0m\n"
	mkdir -p "$$(dirname $@)"
	$(LD) $(OBJ) $(LDFLAGS) -o $@

$(BUILD)/%.c.o: $(SRC)/%.c
	mkdir -p "$$(dirname $@)"
	$(CC) $(CFLAGS) -c $< -o $@
