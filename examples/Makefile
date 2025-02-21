CC := gcc

BUILD  := build
CFLAGS := -Wall -Wextra -g -L ../build/ -lclassparse -pg -std=gnu11 -I ../src/
CFILES := $(shell find -L * -type f -name '*.c' | LC_ALL=C sort)
OUT    := $(addprefix $(BUILD)/,$(CFILES:.c=.out))

.PHONY: all run clean

all: $(OUT)

clean:
	@printf "\033[32m==> Clean up\033[0m\n"
	rm -rf $(BUILD)
	mkdir -p $(BUILD)

$(BUILD)/%.out: %.c
	mkdir -p "$$(dirname $@)"
	$(CC) $(CFLAGS) $< -o $@
