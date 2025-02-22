CC := gcc
LD := gcc

BUILD   := build
OUTPUT  := $(BUILD)/libclassparse.so
INCLUDE := include
SRC     := src

CFLAGS  := -Wall -Wextra -shared -fPIC -O0 -pg -g -std=gnu11 -I $(SRC) -I $(INCLUDE)
LDFLAGS := -g -pg -shared -fPIC

CFILES := $(shell cd $(SRC) && find -L * -type f -name '*.c' | LC_ALL=C sort)
OBJ    := $(addprefix $(BUILD)/,$(CFILES:.c=.c.o)) 

.PHONY: all run clean examples clean-all

all: examples

examples: $(OUTPUT)
	@printf "\033[32m==> Building examples\033[0m\n"
	$(MAKE) -C examples/

run: $(OUTPUT)
	chmod +x $(OUTPUT)
	@printf "\033[32m==> Running $(OUTPUT)\033[0m\n"
	@./$(OUTPUT)

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
