CC = gcc
TARGET = auri
FLAGS = -W
FLAGS += -Wall
FLAGS += -Werror
FLAGS += -pedantic
FLAGS += -g
FLAGS += -Wno-unused-parameter
FLAGS += -Wno-implicit-fallthrough
FLAGS += -std=c99

SRC = $(shell find . -type f \( -name '*.c' \))
OUT_DIR = out
BIN = $(OUT_DIR)/$(TARGET)

all: $(OUT_DIR) $(BIN)

$(BIN): $(SRC)
	$(CC) $(SRC) -o $(BIN) $(FLAGS) -I .

$(OUT_DIR):
	mkdir -p $(OUT_DIR)

clean:
	rm -rf $(OUT_DIR)

format:
	@echo "Applying Format..."
	@find . -type f \( -name "*.c" \) -exec clang-format -i {} \;
	@find . -type f \( -name "*.h" -o -name "*.h" \) -exec clang-format -i {} \;
	@echo "Done! "

.PHONY: all clean
